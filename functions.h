#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/* ================================================
   functions.h — Các hàm toán học tích hợp sẵn
   Ví dụ: sin(x), sqrt(x), log(x),...
   ================================================ */

/* Gọi hàm tên 'name' với 'argc' tham số trong 'argv'.
   Ghi kết quả vào *result.
   Trả về 1 nếu OK, 0 nếu lỗi (tên sai, số tham số sai,...) */
int functions_call(const char *name,
                   double *argv, int argc,
                   double *result);

/* Kiểm tra 'name' có phải tên hàm hợp lệ không */
int functions_exists(const char *name);

/* In danh sách hàm ra màn hình */
void functions_print_list(void);

#endif /* FUNCTIONS_H */