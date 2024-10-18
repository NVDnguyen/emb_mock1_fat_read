
# Get Started

## Git

### 1. Clone repository về máy tính

```bash
git clone https://github.com/NVDnguyen/emb_mock1_fat_read.git
```

---

### 2. Tạo branch để phát triển

Để tránh xung đột khi nhiều người làm việc cùng một file, mỗi thành viên nên tạo một branch riêng bằng lệnh sau:

```bash
git checkout -b <tên_branch>
```

Bạn sẽ làm việc trên nhánh này, thêm code, và commit các thay đổi vào nhánh này.

---

### 3. Làm việc trên branch cá nhân

Sau khi tạo nhánh, bạn có thể chỉnh sửa mã, thêm tính năng mới hoặc sửa lỗi. Sau khi hoàn thành, hãy commit các thay đổi:

```bash
git add .
git commit -m "mô tả bạn đã thay đổi"
```

---

### 4. Push branch lên GitHub

Sau khi commit, bạn cần push nhánh của mình lên GitHub:

```bash
git push origin <tên_branch>
```

---

### 5. Tạo Pull Request (PR)

Khi muốn hợp nhất nhánh cá nhân vào nhánh chính  `main`, bạn sẽ tạo một Pull Request (PR):

1. Vào trang repository trên GitHub.
2. Chọn **Pull requests** > **New pull request**.
3. Chọn nhánh của bạn để so sánh với nhánh chính và tạo PR.


---

### 6. Review và hợp nhất Pull Request

Bấm nút **Merge pull request** để hợp nhất thay đổi.

---

### 7. Cập nhật nhánh chính

Sau khi PR được merge, các thành viên nên pull các thay đổi mới nhất từ nhánh chính về máy tính của mình:

```bash
git checkout main
git pull origin main
```

---

## Build and run

- .\auto.bat  (trick: gõ a and click *tab*)


---

## FAT File System

| **Mô tả khu vực**                              | **Kích thước khu vực**               |
|-------------------------------------------------|--------------------------------------|
| Boot block                                      | 1 block                             |
| File Allocation Table (FAT)                     | Phụ thuộc vào kích thước hệ thống file |
| Disk root directory                             | Biến đổi (chọn khi định dạng đĩa)    |
| File data area                                  | Phần còn lại của đĩa                 |

---

## References

- [YouTube - FAT File System Explanation](https://www.youtube.com/watch?v=5oeiHwzZSj0)
- [Tavi - FAT File System Overview](http://www.tavi.co.uk/phobos/fat.html)
- [Simple Disk Explorer - GitHub](https://github.com/baonq-me/simple-disk-explorer)
- [Hexed.it - Online Hex Editor](https://hexed.it/)
```

