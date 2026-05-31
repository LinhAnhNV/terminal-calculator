#define _GNU_SOURCE
#include "functions.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

/* ================================================
   functions.c — Cài đặt các hàm toán học

   Kỹ thuật: Dùng chuỗi if-else để "dispatch"
   tên hàm -> hàm C tương ứng trong <math.h>
   ================================================ */

/* Macro kiểm tra số tham số cho gọn */
#define NEED(n) \
    if (argc != (n)) { \
        printf("Loi: Ham '%s' can %d tham so, nhan %d.\n", name, (n), argc); \
        return 0; \
    }

int functions_call(const char *name, double *argv, int argc, double *result) {

    /* ---- Lượng giác (radian) ---- */
    if (strcmp(name, "sin")  == 0) { NEED(1); *result = sin(argv[0]);  return 1; }
    if (strcmp(name, "cos")  == 0) { NEED(1); *result = cos(argv[0]);  return 1; }
    if (strcmp(name, "tan")  == 0) { NEED(1); *result = tan(argv[0]);  return 1; }
    if (strcmp(name, "asin") == 0) { NEED(1); *result = asin(argv[0]); return 1; }
    if (strcmp(name, "acos") == 0) { NEED(1); *result = acos(argv[0]); return 1; }
    if (strcmp(name, "atan") == 0) { NEED(1); *result = atan(argv[0]); return 1; }
    if (strcmp(name, "atan2")== 0) { NEED(2); *result = atan2(argv[0], argv[1]); return 1; }

    /* ---- Lượng giác (độ) ---- */
    if (strcmp(name, "sind") == 0) { NEED(1); *result = sin(argv[0] * M_PI / 180.0); return 1; }
    if (strcmp(name, "cosd") == 0) { NEED(1); *result = cos(argv[0] * M_PI / 180.0); return 1; }
    if (strcmp(name, "tand") == 0) { NEED(1); *result = tan(argv[0] * M_PI / 180.0); return 1; }

    /* ---- Căn / Lũy thừa ---- */
    if (strcmp(name, "sqrt") == 0) {
        NEED(1);
        if (argv[0] < 0) { printf("Loi: sqrt cua so am!\n"); return 0; }
        *result = sqrt(argv[0]);
        return 1;
    }
    if (strcmp(name, "cbrt") == 0) { NEED(1); *result = cbrt(argv[0]); return 1; }
    if (strcmp(name, "pow")  == 0) { NEED(2); *result = pow(argv[0], argv[1]); return 1; }
    if (strcmp(name, "exp")  == 0) { NEED(1); *result = exp(argv[0]); return 1; }

    /* ---- Logarithm ---- */
    if (strcmp(name, "log") == 0) {
        NEED(1);
        if (argv[0] <= 0) { printf("Loi: log cua so khong duong!\n"); return 0; }
        *result = log(argv[0]);
        return 1;
    }
    if (strcmp(name, "log2") == 0) {
        NEED(1);
        if (argv[0] <= 0) { printf("Loi: log2 cua so khong duong!\n"); return 0; }
        *result = log2(argv[0]);
        return 1;
    }
    if (strcmp(name, "log10") == 0) {
        NEED(1);
        if (argv[0] <= 0) { printf("Loi: log10 cua so khong duong!\n"); return 0; }
        *result = log10(argv[0]);
        return 1;
    }

    /* ---- Làm tròn ---- */
    if (strcmp(name, "abs")   == 0) { NEED(1); *result = fabs(argv[0]);  return 1; }
    if (strcmp(name, "ceil")  == 0) { NEED(1); *result = ceil(argv[0]);  return 1; }
    if (strcmp(name, "floor") == 0) { NEED(1); *result = floor(argv[0]); return 1; }
    if (strcmp(name, "round") == 0) { NEED(1); *result = round(argv[0]); return 1; }

    /* ---- Tiện ích ---- */
    if (strcmp(name, "hypot") == 0) { NEED(2); *result = hypot(argv[0], argv[1]); return 1; }
    if (strcmp(name, "min")   == 0) {
        int i;
        if (argc < 2) { printf("Loi: min can it nhat 2 tham so.\n"); return 0; }
        *result = argv[0];
        for (i = 1; i < argc; i++) if (argv[i] < *result) *result = argv[i];
        return 1;
    }
    if (strcmp(name, "max")   == 0) {
        int i;
        if (argc < 2) { printf("Loi: max can it nhat 2 tham so.\n"); return 0; }
        *result = argv[0];
        for (i = 1; i < argc; i++) if (argv[i] > *result) *result = argv[i];
        return 1;
    }
    if (strcmp(name, "gcd")   == 0) {
        long long a, b, t;
        NEED(2);
        a = (long long)fabs(argv[0]);
        b = (long long)fabs(argv[1]);
        while (b) { t = b; b = a % b; a = t; }   /* Thuật toán Euclid */
        *result = (double)a;
        return 1;
    }
    if (strcmp(name, "lcm") == 0) {
        long long a, b, t, ga, gb;
        NEED(2);
        a = (long long)fabs(argv[0]);
        b = (long long)fabs(argv[1]);
        ga = a; gb = b;
        while (gb) { t = gb; gb = ga % gb; ga = t; }
        *result = (double)(a / ga * b);
        return 1;
    }
    if (strcmp(name, "fact") == 0) {
        long long n, r;
        NEED(1);
        n = (long long)argv[0];
        if (n < 0) { printf("Loi: fact cua so am!\n"); return 0; }
        if (n > 20) { printf("Loi: fact(%lld) qua lon!\n", n); return 0; }
        r = 1;
        while (n > 1) r*= n--;
        *result = (double)r;
        return 1;
    }
    printf("Loi: Ham '%s' khong ton tai. Go 'help' de xem danh sach.\n", name);
    return 0;
}

int functions_exists(const char *name) {
    const char *list[] = {
        "sin","cos","tan","asin","acos","atan","atan2",
        "sind","cosd","tand",
        "sqrt","cbrt","pow","exp",
        "log","log2","log10",
        "abs","ceil","floor","round",
        "hypot","min","max","gcd","lcm","fact",
        NULL
    };
    int i;
    for (i = 0; list[i] != NULL; i++)
        if (strcmp(name, list[i]) == 0) return 1;
    return 0;
}

void functions_print_list(void) {
    printf("\n  Luong giac (rad): sin, cos, tan, asin, acos, atan, atan2\n");
    printf("  Luong giac (do):  sind, cosd, tand\n");
    printf("  Can / Luy thua:   sqrt, cbrt, pow, exp\n");
    printf("  Logarithm:        log (ln), log2, log10\n");
    printf("  Lam tron:         abs, ceil, floor, round\n");
    printf("  Tien ich:         min, max, hypot, gcd, lcm, fact\n\n");
}