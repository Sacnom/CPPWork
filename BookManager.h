#ifndef BOOK_MANAGER_H
#define BOOK_MANAGER_H

#include "Book.h"

#include <string_view>
#include <vector>

// BookManager.h
// 核心控制中枢声明：5 位同学共同遵守的关键接口契约。
class BookManager {
public:
    // Module A: 文件持久化（同学A负责）
    void loadFromFile(std::string_view filename);
    void saveToFile(std::string_view filename) const;

    // Module B: 信息录入与浏览（同学B负责）
    void addBook();
    void displayAll() const;

    // Module C: 信息查询（同学C负责）
    void searchByTitle(std::string_view title) const;
    void searchByAuthor(std::string_view author) const;

    // Module D: 修改与删除（同学D负责）
    void deleteBook(std::string_view loginId);
    void modifyBook(std::string_view loginId);

    // Module E: 排序控制（同学E负责）
    void sortBooks();

private:
    std::vector<Book> books;
};

#endif
