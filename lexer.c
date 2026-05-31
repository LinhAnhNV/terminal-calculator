#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>   /* strtod */
#include <string.h>
#include <ctype.h>    /* isdigit, isalpha, isspace */

/* ================================================
   lexer.c — Cài đặt Tokenizer

   Ý tưởng: duyệt từng ký tự của chuỗi,
   nhóm lại thành các token có nghĩa.
   ================================================ */

int lexer_tokenize(const char *src, TokenList *out) {
    int   pos   = 0;           /* Vị trí hiện tại trong src */
    int   count = 0;           /* Số token đã tạo           */
    int   len   = (int)strlen(src);

    while (pos < len) {

        /* --- Bỏ qua khoảng trắng --- */
        if (isspace((unsigned char)src[pos])) {
            pos++;
            continue;
        }

        /* --- Số (integer hoặc thập phân) --- */
        if (isdigit((unsigned char)src[pos]) ||
            (src[pos] == '.' && pos+1 < len && isdigit((unsigned char)src[pos+1])))
        {
            char  *end;
            double val = strtod(src + pos, &end);   /* Đọc số thực */
            out->tokens[count].type    = TOK_NUMBER;
            out->tokens[count].num_val = val;
            pos += (int)(end - (src + pos));         /* Nhảy qua phần số vừa đọc */
            count++;
            continue;
        }

        /* --- Tên biến / hàm (bắt đầu bằng chữ cái hoặc _) --- */
        if (isalpha((unsigned char)src[pos]) || src[pos] == '_') {
            int start = pos;
            int slen  = 0;
            while (pos < len && (isalnum((unsigned char)src[pos]) || src[pos] == '_')) {
                pos++;
                slen++;
            }
            out->tokens[count].type = TOK_IDENT;
            strncpy(out->tokens[count].str_val, src + start, slen);
            out->tokens[count].str_val[slen] = '\0';
            count++;
            continue;
        }

        /* --- Ký tự đơn (toán tử, ngoặc,...) --- */
        {
            TokenType t;
            switch (src[pos]) {
                case '+': t = TOK_PLUS;    break;
                case '-': t = TOK_MINUS;   break;
                case '*': t = TOK_STAR;    break;
                case '/': t = TOK_SLASH;   break;
                case '%': t = TOK_PERCENT; break;
                case '^': t = TOK_CARET;   break;
                case '(': t = TOK_LPAREN;  break;
                case ')': t = TOK_RPAREN;  break;
                case ',': t = TOK_COMMA;   break;
                case '=': t = TOK_ASSIGN;  break;
                default:
                    printf("Loi: Ky tu khong hop le '%c'\n", src[pos]);
                    return 0;
            }
            out->tokens[count].type = t;
            pos++;
            count++;
        }

        if (count >= MAX_TOKENS - 1) {
            printf("Loi: Bieu thuc qua dai!\n");
            return 0;
        }
    }

    /* Token kết thúc */
    out->tokens[count].type = TOK_END;
    count++;
    out->count = count;
    return 1;
}

/* Debug: in từng token */
void lexer_print(const TokenList *tl) {
    int i;
    const char *names[] = {
        "NUMBER","IDENT","PLUS","MINUS","STAR","SLASH",
        "PERCENT","CARET","LPAREN","RPAREN","COMMA","ASSIGN","END","ERROR"
    };
    printf("  [Tokens: %d]\n", tl->count);
    for (i = 0; i < tl->count; i++) {
        Token t = tl->tokens[i];
        if (t.type == TOK_NUMBER)
            printf("    %s(%g)\n", names[t.type], t.num_val);
        else if (t.type == TOK_IDENT)
            printf("    %s(%s)\n", names[t.type], t.str_val);
        else
            printf("    %s\n", names[t.type]);
    }
}