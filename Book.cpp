#include "Book.h"

// Book.cpp
// 数据模型基础实现：仅实现 Book 类的构造函数与 Getter/Setter。

Book::Book(std::string_view bookId,
           std::string_view title,
           std::string_view author,
           std::string_view categoryId,
           std::string_view publisher,
           std::string_view publishTime,
           double price)
    : bookId_(bookId),
      title_(title),
      author_(author),
      categoryId_(categoryId),
      publisher_(publisher),
      publishTime_(publishTime),
      price_(price) {
}

std::string_view Book::getBookId() const {
    return bookId_;
}

std::string_view Book::getTitle() const {
    return title_;
}

std::string_view Book::getAuthor() const {
    return author_;
}

std::string_view Book::getCategoryId() const {
    return categoryId_;
}

std::string_view Book::getPublisher() const {
    return publisher_;
}

std::string_view Book::getPublishTime() const {
    return publishTime_;
}

double Book::getPrice() const {
    return price_;
}

void Book::setBookId(std::string_view bookId) {
    bookId_ = bookId;
}

void Book::setTitle(std::string_view title) {
    title_ = title;
}

void Book::setAuthor(std::string_view author) {
    author_ = author;
}

void Book::setCategoryId(std::string_view categoryId) {
    categoryId_ = categoryId;
}

void Book::setPublisher(std::string_view publisher) {
    publisher_ = publisher;
}

void Book::setPublishTime(std::string_view publishTime) {
    publishTime_ = publishTime;
}

void Book::setPrice(double price) {
    price_ = price;
}

