#ifndef LEXER_H
#define LEXER_H

/* ================================================
   lexer.h — Bộ phân tích từ vựng (Tokenizer)

   Nhiệm vụ: Chia chuỗi "3 + 4 * 2" thành danh sách
   các "token" (đơn vị nhỏ nhất có ý nghĩa):
     [NUMBER:3] [PLUS] [NUMBER:4] [STAR] [NUMBER:2]

   Đây là bước đầu tiên của mọi trình biên dịch/thông dịch.
   ================================================ */

/* Các loại token */
typedef enum {
    TOK_NUMBER,   /* Số: 3, 3.14, 1e5          */
    TOK_IDENT,    /* Tên biến/hàm: x, sin, pi  */
    TOK_PLUS,     /* +                          */
    TOK_MINUS,    /* -                          */
    TOK_STAR,     /* *                          */
    TOK_SLASH,    /* /                          */
    TOK_PERCENT,  /* %                          */
    TOK_CARET,    /* ^ (lũy thừa)               */
    TOK_LPAREN,   /* (                          */
    TOK_RPAREN,   /* )                          */
    TOK_COMMA,    /* ,                          */
    TOK_ASSIGN,   /* =                          */
    TOK_END,      /* Kết thúc chuỗi             */
    TOK_ERROR     /* Ký tự không hợp lệ         */
} TokenType;

/* Cấu trúc 1 token */
typedef struct {
    TokenType type;
    double    num_val;        /* Dùng khi type == TOK_NUMBER */
    char      str_val[64];    /* Dùng khi type == TOK_IDENT  */
} Token;

#define MAX_TOKENS 256

/* Kết quả sau khi tokenize */
typedef struct {
    Token tokens[MAX_TOKENS];
    int   count;
} TokenList;

/* Hàm chính: tokenize chuỗi src → danh sách token
   Trả về 1 nếu thành công, 0 nếu có lỗi */
int lexer_tokenize(const char *src, TokenList *out);

/* Debug: in danh sách token ra màn hình */
void lexer_print(const TokenList *tl);

#endif