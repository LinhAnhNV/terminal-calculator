#ifndef PARSER_H
#define PARSER_H

/* ================================================
   parser.h — Bộ phân tích cú pháp + tính toán

   Nhận vào danh sách token từ Lexer,
   tính ra kết quả theo đúng thứ tự ưu tiên toán tử.

   Kỹ thuật: Recursive Descent Parser
   → Mỗi mức ưu tiên = 1 hàm đệ quy riêng

   Thứ tự ưu tiên (thấp → cao):
     1. Gán:        x = 5
     2. Cộng/Trừ:  + -
     3. Nhân/Chia: * / %
     4. Dấu âm:   -x
     5. Lũy thừa:  ^  (right-associative)
     6. Cơ bản:    số, biến, hàm, (biểu thức)
   ================================================ */

#include "lexer.h"

/* Trạng thái parser (dùng con trỏ vào TokenList) */
typedef struct {
    const TokenList *tl;   /* Danh sách token               */
    int              pos;  /* Vị trí token hiện tại         */
    int              err;  /* 1 nếu đã xảy ra lỗi           */
} Parser;

/* Khởi tạo parser */
void parser_init(Parser *p, const TokenList *tl);

/* Hàm chính: tính toàn bộ biểu thức
   Trả về kết quả, ghi lỗi vào p->err */
double parser_eval(Parser *p);

#endif /* PARSER_H */