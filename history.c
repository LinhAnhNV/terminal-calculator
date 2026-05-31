#include "history.h"
#include <stdio.h>
#include <string.h>

/* ================================================
   history.c — Cài đặt lịch sử tính toán

   Kỹ thuật: Mảng tĩnh + biến đếm
   -> Đây là cách đơn giản nhất trước khi học
      Linked List hay Stack động
   ================================================ */

static HistoryEntry entries[MAX_HISTORY];
static int          count = 0;

/* ------------------------------------------------
   history_init: Khởi tạo (reset về 0)
   ------------------------------------------------ */
void history_init(void) {
    count = 0;
}

/* ------------------------------------------------
   history_add: Thêm 1 mục vào lịch sử
   → Nếu đầy thì dịch chuyển mảng lên 1 (xóa cũ nhất)
      Kỹ thuật này gọi là "shifting" — O(n)
      Sau này học Linked List sẽ làm O(1) hơn!
   ------------------------------------------------ */
void history_add(const char *expr, double result) {
    if (count >= MAX_HISTORY) {
        /* Dịch toàn bộ mảng lên 1 vị trí → mất entry[0] */
        int i;
        for (i = 0; i < MAX_HISTORY - 1; i++) {
            entries[i] = entries[i + 1];
        }
        count = MAX_HISTORY - 1;
    }

    strncpy(entries[count].expr, expr, MAX_EXPR_LEN - 1);
    entries[count].expr[MAX_EXPR_LEN - 1] = '\0';
    entries[count].result = result;
    count++;
}

/* ------------------------------------------------
   history_print: In lịch sử ra màn hình
   ------------------------------------------------ */
void history_print(void) {
    int i;
    if (count == 0) {
        printf("  (Chua co lich su nao)\n");
        return;
    }
    printf("\n  --- Lich su (%d muc) ---\n", count);
    for (i = 0; i < count; i++) {
        printf("  [%2d] %s = %g\n", i + 1, entries[i].expr, entries[i].result);
    }
    printf("\n");
}

/* ------------------------------------------------
   history_clear: Xóa toàn bộ lịch sử
   ------------------------------------------------ */
void history_clear(void) {
    count = 0;
    printf("Da xoa lich su.\n");
}

int history_count(void) {
    return count;
}

void history_save(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Error: Could not open file %s for writing\n", filename);
        return;
    }
    int i;
    for (i = 0; i < count; i++)
        fprintf(f,"%s|%g\n", entries[i].expr, entries[i].result);
    fclose(f);
}

void history_load(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;
    char buffer[MAX_EXPR_LEN + 32];
    while (fgets(buffer, sizeof(buffer), f)) {
        char expr[MAX_EXPR_LEN];
        double result;
        if (sscanf(buffer, "%[^|]|%lf", expr, &result) == 2)
            history_add(expr, result);
    }
    fclose(f);
}