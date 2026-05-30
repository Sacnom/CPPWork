# 同学 B 专属说明 —— 录入与浏览模块

> 你负责的文件：**只改 [src/BookManager_Add.cpp](../../src/BookManager_Add.cpp) 这一个文件**，别的不用动。

你这块是整个项目里最基础、最适合先上手的部分。做完它，你就把「怎么造一本书、怎么遍历所有书」摸透了，其他同学的模块也都建立在这之上。

## 一、你要做什么（用大白话）

- **`addBook`（添加图书）**：问用户要 7 个信息（书名、作者……），造出一本新书，存进内存。
- **`displayAll`（浏览全部）**：把内存里所有的书一本一本打印到屏幕上。

菜单里的「3. 添加图书」调用你的 `addBook`，「4. 浏览全部图书」调用你的 `displayAll`。

## 二、先弄懂几个概念

**1. 图书存在哪？**
`BookManager` 类里有个成员变量 `books`，类型是 `std::vector<Book>`，可以理解成「一个能自动变长的图书数组」。你添加书就是往它里面塞，浏览就是把它从头到尾走一遍。

**2. 怎么造一本书？**
一本书是一个 `Book` 对象，造它的时候要按固定顺序给 7 个信息：
```
bookId(登录号), title(书名), author(作者), categoryId(分类号),
publisher(出版社), publishTime(出版时间), price(价格)
```
前 6 个是文字（`std::string`），最后一个价格是小数（`double`）。

**3. 怎么往 vector 里加东西？**
用 `books.emplace_back(...)`，把 7 个信息按顺序传进去，它就会在数组末尾造一本新书。

**4. 怎么读用户输入？**
用 `std::cin >> 变量名;`。比如 `std::cin >> title;` 会把用户敲的内容读进 `title`。
⚠️ 注意：`std::cin >>` 读到空格就停。所以书名这种最好让用户输入时**不要带空格**（比如用 `C++Primer` 而不是 `C++ Primer`）。这是本项目目前的统一约定。

**5. 怎么遍历所有书？**
用「范围 for 循环」：
```cpp
for (const auto& b : books) {
    // 这里的 b 就是当前这本书，可以 b.getTitle() 拿它的书名
}
```

## 三、完整代码（可直接复制，建议自己读懂每一行）

把下面整段替换掉 [BookManager_Add.cpp](../BookManager_Add.cpp) 里的内容：

```cpp
#include "BookManager.h"

#include <iostream>
#include <string>

// Module B: 信息录入与浏览（同学B负责）

// 添加一本书：依次问用户 7 个信息，造书，存进 books
void BookManager::addBook() {
    std::string id, title, author, categoryId, publisher, publishTime;
    double price = 0;

    std::cout << "请输入登录号: ";   std::cin >> id;
    std::cout << "请输入书名: ";     std::cin >> title;
    std::cout << "请输入作者: ";     std::cin >> author;
    std::cout << "请输入分类号: ";   std::cin >> categoryId;
    std::cout << "请输入出版社: ";   std::cin >> publisher;
    std::cout << "请输入出版时间: "; std::cin >> publishTime;
    std::cout << "请输入价格: ";     std::cin >> price;

    // 用 7 个信息造一本新书，直接放进 books 数组末尾
    books.emplace_back(id, title, author, categoryId,
                       publisher, publishTime, price);

    std::cout << "添加成功！当前共有 " << books.size() << " 本书。\n";
}

// 浏览全部：把 books 里的每一本都打印出来
void BookManager::displayAll() const {
    if (books.empty()) {   // 一本书都没有时，友好提示
        std::cout << "目前还没有任何图书，请先添加。\n";
        return;
    }

    std::cout << "\n===== 全部图书（共 " << books.size() << " 本）=====\n";
    // 表头
    std::cout << "登录号\t书名\t作者\t分类号\t出版社\t出版时间\t价格\n";

    // 一本一本打印
    for (const auto& b : books) {
        std::cout << b.getBookId()      << '\t'
                  << b.getTitle()       << '\t'
                  << b.getAuthor()      << '\t'
                  << b.getCategoryId()  << '\t'
                  << b.getPublisher()   << '\t'
                  << b.getPublishTime() << '\t'
                  << b.getPrice()       << '\n';
    }
}
```

> 说明：`'\t'` 是制表符（Tab），让各列稍微对齐一点。看起来不够整齐也没关系，功能正确最重要。

## 四、怎么测试你的代码

1. 编译：在项目根目录运行
   ```bash
   g++.exe -std=c++17 -g src/*.cpp -o build/main.exe
   ./build/main.exe
   ```
2. 在菜单里选「3. 添加图书」，按提示输入一本书的信息。
3. 再选「4. 浏览全部图书」，看刚才输入的书有没有正确显示。
4. 多加几本，再浏览，确认数量和内容都对。

## 五、容易踩的坑

- **`displayAll` 后面的 `const` 不能删**：它表示「这个方法只看不改」。所以在 `displayAll` 里你**不能**往 `books` 加东西或改东西，只能读。这是头文件规定好的。
- **输入带空格会出问题**：`std::cin >>` 遇到空格就停。如果用户书名输入 `C++ Primer`，只会读到 `C++`，剩下的 `Primer` 会被当成下一个输入。初学阶段先约定不输空格。
- **`emplace_back` 的参数顺序不能乱**：必须严格按 `id, title, author, categoryId, publisher, publishTime, price` 的顺序，错位了书的信息就乱了。
- **价格用 `double` 不要用 `int`**：价格可能是小数，`int` 会把 99.5 变成 99。
