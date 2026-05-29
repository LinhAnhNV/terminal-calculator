#ifndef VARIABLES_H
#define VARIABLES_H

/* ================================================
   variables.h — Quản lý biến người dùng tự đặt
   Ví dụ: x = 5, y = x + 3

   Dùng mảng song song:
     var_names[i]  → tên biến
     var_values[i] → giá trị tương ứng
   ================================================ */

#define MAX_VARS     64   /* Tối đa 64 biến cùng lúc      */
#define MAX_VAR_NAME 32   /* Tên biến tối đa 32 ký tự     */

/* Khởi tạo — nạp sẵn hằng số pi, e */
void vars_init(void);

/* Tìm giá trị của biến 'name'.
   Trả về 1 nếu tìm thấy (ghi vào *out), 0 nếu không có. */
int vars_get(const char *name, double *out);

/* Gán hoặc cập nhật biến 'name' = value.
   Trả về 1 nếu thành công, 0 nếu bảng đã đầy. */
int vars_set(const char *name, double value);

/* In toàn bộ biến đang có ra màn hình */
void vars_print(void);

/* Xóa hết biến người dùng đặt (giữ lại pi, e, ans) */
void vars_reset(void);

#endif /* VARIABLES_H */