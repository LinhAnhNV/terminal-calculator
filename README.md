# Terminal Calculator

Máy tính dòng lệnh viết bằng C — hỗ trợ biểu thức toán học phức tạp, biến, hàm, và lịch sử tính toán.

Điểm đặc biệt: tự viết Lexer và Recursive Descent Parser từ đầu, không dùng thư viện eval ngoài.

> A terminal calculator in C with a hand-written lexer and recursive descent parser. Supports expressions, variables, built-in functions, and persistent history.

---

## Tính năng / Features

- Phép tính cơ bản: `+`, `-`, `*`, `/`, `%`, `^`
- Thứ tự ưu tiên toán tử và ngoặc
- Biến tự định nghĩa: `x = 5`, `y = x * 2`
- Hằng số: `pi`, `e`, `ans`
- Hàm toán học: `sin`, `cos`, `sqrt`, `log`, `gcd`, `lcm`, `fact`,...
- Lịch sử tính toán — lưu file, tải lại khi khởi động
- Lệnh: `help`, `history`, `vars`, `clear`, `reset`, `exit`

---

## Kiến trúc / Architecture

terminal-calculator/
├── main.c        # Vòng lặp chính, xử lý lệnh
├── lexer.c/h     # Tokenizer
├── parser.c/h    # Recursive Descent Parser
├── variables.c/h # Quản lý biến
├── functions.c/h # Hàm toán học
├── history.c/h   # Lịch sử tính toán
├── Makefile
└── README.md

---

## Build & Run

**Windows:**
```bash
mingw32-make
.\calculator.exe
```

**Linux / macOS:**
```bash
make
./calculator
```

**Chạy trực tiếp:**
```bash
make run
```

---

## Ví dụ / Examples

3 + 4 * 2
= 11


x = 10
= 10


sin(pi / 2)
= 1


gcd(48, 18)
= 6


fact(10)
= 3628800

---

## Tác giả / Author

**Nguyễn Văn Linh Anh**  
Sinh viên CNTT — ĐH Công Thương TP.HCM (HUIT)  
Củ Chi, TP.HCM · 2025