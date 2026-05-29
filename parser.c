#include "parser.h"
#include "variables.h"
#include "functions.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

/* ================================================
   parser.c — Recursive Descent Parser

   Mỗi hàm parse_X() xử lý 1 mức ưu tiên,
   gọi hàm parse_Y() ở mức cao hơn để lấy toán hạng.

   Ví dụ: "3 + 4 * 2"
     parse_add_sub()
       → lấy 3 qua parse_mul_div()
       → thấy '+', lấy "4 * 2" qua parse_mul_div()
           → lấy 4, thấy '*', lấy 2
           → trả về 8
       → 3 + 8 = 11 ✓
   ================================================ */

/* --- Helper: xem token hiện tại --- */
static Token cur(Parser *p) {
    return p->tl->tokens[p->pos];
}

/* --- Helper: tiêu thụ token hiện tại, tiến lên 1 --- */
static Token consume(Parser *p) {
    Token t = p->tl->tokens[p->pos];
    if (t.type != TOK_END) p->pos++;
    return t;
}

/* --- Helper: kiểm tra và tiêu thụ nếu đúng loại --- */
static int match(Parser *p, TokenType t) {
    if (cur(p).type == t) { consume(p); return 1; }
    return 0;
}

/* --- Forward declarations (khai báo trước vì đệ quy) --- */
static double parse_assign  (Parser *p);
static double parse_add_sub (Parser *p);
static double parse_mul_div (Parser *p);
static double parse_unary   (Parser *p);
static double parse_power   (Parser *p);
static double parse_primary (Parser *p);

/* ------------------------------------------------
   parser_init & parser_eval
   ------------------------------------------------ */
void parser_init(Parser *p, const TokenList *tl) {
    p->tl  = tl;
    p->pos = 0;
    p->err = 0;
}

double parser_eval(Parser *p) {
    double result = parse_assign(p);
    if (!p->err && cur(p).type != TOK_END) {
        printf("Loi: Bieu thuc khong hop le (co ky tu thua).\n");
        p->err = 1;
    }
    return result;
}

/* ------------------------------------------------
   Mức 1: Gán biến   x = biểu_thức
   ------------------------------------------------ */
static double parse_assign(Parser *p) {
    /* Nhìn trước 2 token: nếu là IDENT rồi '=' thì là phép gán */
    if (p->pos + 1 < p->tl->count &&
        p->tl->tokens[p->pos].type     == TOK_IDENT &&
        p->tl->tokens[p->pos + 1].type == TOK_ASSIGN)
    {
        char name[64];
        strncpy(name, cur(p).str_val, 63);
        name[63] = '\0';
        p->pos += 2;                          /* Bỏ qua IDENT và '=' */
        double val = parse_assign(p);         /* Đệ quy (right-assoc) */
        if (!p->err) vars_set(name, val);
        return val;
    }
    return parse_add_sub(p);
}

/* ------------------------------------------------
   Mức 2: Cộng / Trừ   a + b - c
   ------------------------------------------------ */
static double parse_add_sub(Parser *p) {
    double left = parse_mul_div(p);
    while (!p->err) {
        if      (match(p, TOK_PLUS))  left += parse_mul_div(p);
        else if (match(p, TOK_MINUS)) left -= parse_mul_div(p);
        else break;
    }
    return left;
}

/* ------------------------------------------------
   Mức 3: Nhân / Chia / Modulo   a * b / c % d
   ------------------------------------------------ */
static double parse_mul_div(Parser *p) {
    double left = parse_unary(p);
    while (!p->err) {
        if (match(p, TOK_STAR)) {
            left *= parse_unary(p);
        } else if (cur(p).type == TOK_SLASH) {
            consume(p);
            double r = parse_unary(p);
            if (r == 0.0) { printf("Loi: Chia cho 0!\n"); p->err = 1; return 0; }
            left /= r;
        } else if (match(p, TOK_PERCENT)) {
            double r = parse_unary(p);
            if (r == 0.0) { printf("Loi: Modulo voi 0!\n"); p->err = 1; return 0; }
            left = fmod(left, r);
        } else break;
    }
    return left;
}

/* ------------------------------------------------
   Mức 4: Dấu âm / dương   -x  +x
   ------------------------------------------------ */
static double parse_unary(Parser *p) {
    if (match(p, TOK_MINUS)) return -parse_unary(p);
    if (match(p, TOK_PLUS))  return  parse_unary(p);
    return parse_power(p);
}

/* ------------------------------------------------
   Mức 5: Lũy thừa   a ^ b
   Right-associative: 2^3^2 = 2^(3^2) = 512
   ------------------------------------------------ */
static double parse_power(Parser *p) {
    double base = parse_primary(p);
    if (!p->err && match(p, TOK_CARET)) {
        double exp = parse_unary(p);   /* Đệ quy phải */
        return pow(base, exp);
    }
    return base;
}

/* ------------------------------------------------
   Mức 6: Đơn vị cơ bản
     - Số: 3.14
     - Biến: x, pi, ans
     - Hàm: sin(x), min(a,b)
     - Ngoặc: (biểu thức)
   ------------------------------------------------ */
static double parse_primary(Parser *p) {
    Token t = cur(p);

    /* Số */
    if (t.type == TOK_NUMBER) {
        consume(p);
        return t.num_val;
    }

    /* Tên biến hoặc hàm */
    if (t.type == TOK_IDENT) {
        consume(p);
        char name[64];
        strncpy(name, t.str_val, 63);
        name[63] = '\0';

        /* Hàm: tên theo sau là '(' */
        if (cur(p).type == TOK_LPAREN) {
            consume(p);   /* bỏ '(' */

            double args[16];
            int    argc = 0;

            /* Đọc danh sách tham số */
            if (cur(p).type != TOK_RPAREN) {
                args[argc++] = parse_assign(p);
                while (!p->err && match(p, TOK_COMMA)) {
                    if (argc >= 16) { printf("Loi: Qua nhieu tham so!\n"); p->err=1; return 0; }
                    args[argc++] = parse_assign(p);
                }
            }

            if (cur(p).type != TOK_RPAREN) {
                printf("Loi: Thieu ')' sau tham so ham '%s'.\n", name);
                p->err = 1;
                return 0;
            }
            consume(p);   /* bỏ ')' */

            double result = 0.0;
            if (!functions_call(name, args, argc, &result)) p->err = 1;
            return result;
        }

        /* Biến */
        {
            double val = 0.0;
            if (!vars_get(name, &val)) {
                printf("Loi: Bien '%s' chua duoc khai bao.\n", name);
                p->err = 1;
            }
            return val;
        }
    }

    /* Ngoặc ( biểu_thức ) */
    if (t.type == TOK_LPAREN) {
        consume(p);
        double val = parse_assign(p);
        if (cur(p).type != TOK_RPAREN) {
            printf("Loi: Thieu ')' dong ngoac.\n");
            p->err = 1;
            return 0;
        }
        consume(p);
        return val;
    }

    printf("Loi: Bieu thuc khong hop le.\n");
    p->err = 1;
    return 0;
}