#ifndef HISTORY_H
#define HISTORY_H

/* ================================================
   history.h — Lưu lịch sử các biểu thức đã tính

   Dùng mảng tĩnh (circular buffer đơn giản):
     entries[i].expr   -> biểu thức cậu gõ vào
     entries[i].result -> kết quả tính được
   ================================================ */

#define MAX_HISTORY  50    /* Lưu tối đa 50 dòng lịch sử */
#define MAX_EXPR_LEN 256   /* Độ dài tối đa 1 biểu thức  */

typedef struct {
    char   expr  [MAX_EXPR_LEN];
    double result;
} HistoryEntry;

void history_init  (void);
void history_add   (const char *expr, double result);
void history_print (void);
void history_clear (void);
int  history_count (void);
void history_save(const char *filename);
void history_load(const char *filename);

#endif