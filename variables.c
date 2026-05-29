#define _GNU_SOURCE         /* Cần để dùng M_PI, M_E trên Linux */
#include "variables.h"
#include <stdio.h>
#include <string.h>
#include <math.h>   /* M_PI, M_E */

/* ================================================
   variables.c — Cài đặt quản lý biến

   Kỹ thuật: "parallel arrays" (mảng song song)
   → Đây là CTDL đơn giản nhất để lưu cặp key-value
      trước khi học Hash Table hay Linked List
   ================================================ */

/* --- Dữ liệu nội bộ (static = chỉ dùng trong file này) --- */
static char   var_names [MAX_VARS][MAX_VAR_NAME];
static double var_values[MAX_VARS];
static int    var_count = 0;   /* Số biến hiện tại */

/* ------------------------------------------------
   vars_init: Nạp hằng số mặc định
   ------------------------------------------------ */
void vars_init(void) {
    var_count = 0;
    vars_set("pi",  M_PI);   /* 3.14159...  */
    vars_set("e",   M_E);    /* 2.71828...  */
    vars_set("ans", 0.0);    /* Kết quả lần trước */
}

/* ------------------------------------------------
   vars_get: Tìm kiếm tuyến tính (Linear Search)
   → Duyệt từ đầu đến cuối, so sánh từng tên
   Độ phức tạp: O(n)
   ------------------------------------------------ */
int vars_get(const char *name, double *out) {
    int i;
    for (i = 0; i < var_count; i++) {
        if (strcmp(var_names[i], name) == 0) {
            *out = var_values[i];   /* Tìm thấy → ghi ra */
            return 1;
        }
    }
    return 0;   /* Không tìm thấy */
}

/* ------------------------------------------------
   vars_set: Gán biến
   → Nếu đã tồn tại thì cập nhật (update)
   → Nếu chưa có thì thêm mới (insert)
   ------------------------------------------------ */
int vars_set(const char *name, double value) {
    int i;

    /* Tìm xem đã tồn tại chưa */
    for (i = 0; i < var_count; i++) {
        if (strcmp(var_names[i], name) == 0) {
            var_values[i] = value;   /* Cập nhật */
            return 1;
        }
    }

    /* Chưa có → thêm mới, kiểm tra bảng có đầy không */
    if (var_count >= MAX_VARS) {
        printf("Loi: Bang bien da day (%d bien)!\n", MAX_VARS);
        return 0;
    }

    strncpy(var_names[var_count], name, MAX_VAR_NAME - 1);
    var_names[var_count][MAX_VAR_NAME - 1] = '\0';  /* Đảm bảo null-terminated */
    var_values[var_count] = value;
    var_count++;
    return 1;
}

/* ------------------------------------------------
   vars_print: In danh sách biến
   ------------------------------------------------ */
void vars_print(void) {
    int i;
    if (var_count == 0) {
        printf("  (Chua co bien nao)\n");
        return;
    }
    printf("\n  %-16s %s\n", "Ten bien", "Gia tri");
    printf("  %-16s %s\n", "--------", "-------");
    for (i = 0; i < var_count; i++) {
        printf("  %-16s = %g\n", var_names[i], var_values[i]);
    }
    printf("\n");
}

/* ------------------------------------------------
   vars_reset: Xóa biến người dùng, giữ hằng số
   ------------------------------------------------ */
void vars_reset(void) {
    var_count = 0;
    vars_set("pi",  M_PI);
    vars_set("e",   M_E);
    vars_set("ans", 0.0);
    printf("Da reset bien ve mac dinh.\n");
}