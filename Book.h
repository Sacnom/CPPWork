#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <string_view>

// Book.h
// 数据模型声明：全组共享的图书实体契约。
class Book {
public:
    Book(std::string_view bookId,
         std::string_view title,
         std::string_view author,
         std::string_view categoryId,
         std::string_view publisher,
         std::string_view publishTime,
         double price);

    std::string_view getBookId() const;
    std::string_view getTitle() const;
    std::string_view getAuthor() const;
    std::string_view getCategoryId() const;
    std::string_view getPublisher() const;
    std::string_view getPublishTime() const;
    double getPrice() const;

<<<<<<< HEAD
    void setLoginId(std::string_view bookId);
=======
    void setBookId(std::string_view bookId);
>>>>>>> c2a2bb8c23bf984cb8a267f6f182762894a9b339
    void setTitle(std::string_view title);
    void setAuthor(std::string_view author);
    void setCategoryId(std::string_view categoryId);
    void setPublisher(std::string_view publisher);
    void setPublishTime(std::string_view publishTime);
    void setPrice(double price);

private:
    std::string bookId_;
    std::string title_;
    std::string author_;
    std::string categoryId_;
    std::string publisher_;
    std::string publishTime_;
    double price_;
};

#endif
