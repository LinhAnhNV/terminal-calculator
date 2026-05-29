#include <stdio.h>
#include <string.h>
#include <math.h>

#include "lexer.h"
#include "parser.h"
#include "variables.h"
#include "history.h"
#include "functions.h"

#define BUFFER_SIZE 256

/* ================================================
   main.c — Điểm vào của chương trình

   Luồng xử lý mỗi lần người dùng nhập:
     input (chuỗi)
       → Lexer  → TokenList
       → Parser → double result
       → In kết quả, lưu vào history & ans
   ================================================ */

/* ---- Các hàm tiện ích nhập liệu (cậu tự viết) ---- */

void xoa_ky_tu_xuong_dong(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
}

void nhap_chuoi(const char *msg, char *out, size_t max_len) {
    char buffer[BUFFER_SIZE];
    while (1) {
        printf("%s", msg);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Loi nhap. Vui long nhap lai.\n");
            continue;
        }
        xoa_ky_tu_xuong_dong(buffer);
        if (strlen(buffer) == 0) {
            continue;   /* Bỏ qua dòng trống, không báo lỗi */
        }
        if (strlen(buffer) > max_len) {
            printf("Do dai vuot qua %d ky tu. Vui long nhap lai.\n", (int)max_len);
            continue;
        }
        strcpy(out, buffer);
        break;
    }
}

/* ---- In banner ---- */
static void print_banner(void) {
    printf("\n|==============================|\n");
    printf(  "|   TERMINAL CALCULATOR v1.0   |\n");
    printf(  "|   Go 'help' de xem huong dan |\n");
    printf(  "|==============================|\n\n");
}

/* ---- In hướng dẫn ---- */
static void print_help(void) {
    printf("\n--- HUONG DAN ---\n");
    printf("  Phep tinh:  3 + 4 * 2    (ho tro +, -, *, /, %%, ^)\n");
    printf("  Ngoac:      (1 + 2) * 3\n");
    printf("  Bien:       x = 5        roi dung lai: x * 2\n");
    printf("  Hang so:    pi, e, ans   (ket qua lan truoc)\n");
    printf("  Ham:        sin, cos, sqrt, log, abs,...\n\n");
    functions_print_list();
    printf("  Lenh dac biet:\n");
    printf("    help     - xem huong dan nay\n");
    printf("    history  - xem lich su tinh toan\n");
    printf("    vars     - xem danh sach bien\n");
    printf("    clear    - xoa lich su\n");
    printf("    reset    - reset bien ve mac dinh\n");
    printf("    exit     - thoat chuong trinh\n\n");
}

/* ---- Xử lý 1 dòng input, trả về 0 nếu muốn thoát ---- */
static int xu_ly_lenh(const char *input) {

    /* Lệnh đặc biệt */
    if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
        printf("Tam biet!\n");
        return 0;
    }
    if (strcmp(input, "help")    == 0) { print_help();       return 1; }
    if (strcmp(input, "history") == 0) { history_print();    return 1; }
    if (strcmp(input, "vars")    == 0) { vars_print();       return 1; }
    if (strcmp(input, "clear")   == 0) { history_clear();    return 1; }
    if (strcmp(input, "reset")   == 0) { vars_reset();       return 1; }

    /* Tính toán biểu thức */
    {
        TokenList tl;
        Parser    parser;
        double    result;

        /* Bước 1: Tokenize */
        if (!lexer_tokenize(input, &tl)) return 1;   /* Lỗi đã được in trong lexer */

        /* Bước 2: Parse & Evaluate */
        parser_init(&parser, &tl);
        result = parser_eval(&parser);
        if (parser.err) return 1;                    /* Lỗi đã được in trong parser */

        /* Bước 3: Hiển thị kết quả */
        /* In đẹp: nếu là số nguyên thì không in .000000 */
        if (result == (long long)result && fabs(result) < 1e15)
            printf("  = %lld\n\n", (long long)result);
        else
            printf("  = %g\n\n", result);

        /* Bước 4: Lưu vào history và cập nhật 'ans' */
        history_add(input, result);
        vars_set("ans", result);
    }

    return 1;
}

/* ---- main ---- */
int main(void) {
    char input[BUFFER_SIZE];

    /* Khởi tạo các module */
    vars_init();
    history_init();

    print_banner();

    while (1) {
        nhap_chuoi("> ", input, sizeof(input) - 1);
        if (!xu_ly_lenh(input)) break;
    }

    return 0;
}