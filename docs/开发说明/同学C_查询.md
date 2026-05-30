# 同学 C 专属说明 —— 查询模块

> 你负责的文件：**只改 [src/BookManager_Search.cpp](../../src/BookManager_Search.cpp) 这一个文件**，别的不用动。

## 一、你要做什么（用大白话）

用户想找书，给你一个关键字，你就在所有书里找出**书名（或作者）里包含这个关键字**的书，打印出来。

- **`searchByTitle`（按书名查）**：用户给个书名关键字，找书名里含它的书。
- **`searchByAuthor`（按作者查）**：用户给个作者关键字，找作者里含它的书。

菜单里「5. 按书名查询」调用 `searchByTitle`，「6. 按作者查询」调用 `searchByAuthor`。

## 二、先弄懂几个概念

**1. 图书存在哪？**
`BookManager` 里有成员变量 `books`，是 `std::vector<Book>`（一个图书数组）。你要做的就是把它从头到尾走一遍，挑出符合条件的。

**2. 怎么遍历所有书？**
用「范围 for 循环」：
```cpp
for (const auto& b : books) {
    // b 就是当前这本书，b.getTitle() 是它的书名
}
```

**3. 怎么判断一个字符串里「包含」另一个字符串？**
用 `.find(...)`。如果找到了，返回出现的位置；如果没找到，返回一个特殊值 `npos`。所以判断「包含」就是看返回值**不等于** `npos`：
```cpp
if (b.getTitle().find(关键字) != std::string_view::npos) {
    // 说明书名里含有这个关键字
}
```
> `getTitle()` 返回的是 `std::string_view` 类型，所以这里写的是 `std::string_view::npos`。

**4. 参数 `title` / `author` 是什么类型？**
是 `std::string_view`，你可以把它当普通字符串用，直接拿去 `.find` 比较就行，不用转换。

## 三、完整代码（可直接复制，建议自己读懂每一行）

把下面整段替换掉 [BookManager_Search.cpp](../BookManager_Search.cpp) 里的内容：

```cpp
#include "BookManager.h"

#include <iostream>
#include <string_view>

// Module C: 信息查询（同学C负责）

// 按书名关键字查询：打印所有书名里包含该关键字的书
void BookManager::searchByTitle(std::string_view title) const {
    bool found = false;  // 记录有没有找到，用来最后给提示

    for (const auto& b : books) {
        // 如果这本书的书名里含有关键字
        if (b.getTitle().find(title) != std::string_view::npos) {
            std::cout << "登录号: " << b.getBookId()
                      << "  书名: " << b.getTitle()
                      << "  作者: " << b.getAuthor()
                      << "  价格: " << b.getPrice() << '\n';
            found = true;
        }
    }

    if (!found) {
        std::cout << "没有找到书名包含「" << title << "」的图书。\n";
    }
}

// 按作者关键字查询：打印所有作者里包含该关键字的书
void BookManager::searchByAuthor(std::string_view author) const {
    bool found = false;

    for (const auto& b : books) {
        if (b.getAuthor().find(author) != std::string_view::npos) {
            std::cout << "登录号: " << b.getBookId()
                      << "  书名: " << b.getTitle()
                      << "  作者: " << b.getAuthor()
                      << "  价格: " << b.getPrice() << '\n';
            found = true;
        }
    }

    if (!found) {
        std::cout << "没有找到作者包含「" << author << "」的图书。\n";
    }
}
```

> 两个函数几乎一模一样，区别只在于一个查 `getTitle()`、一个查 `getAuthor()`。看懂一个就懂另一个了。

## 四、怎么测试你的代码

1. 先确保同学 B 的「添加图书」能用（你需要先有几本书才能查）。
2. 编译并运行：
   ```bash
   g++.exe -std=c++17 -g src/*.cpp -o build/main.exe
   ./build/main.exe
   ```
3. 添加几本书，比如书名 `C++Primer`、`JavaCore`、`Python基础`。
4. 选「5. 按书名查询」，输入 `C++`，应该能查到 `C++Primer`。
5. 输入一个不存在的词，比如 `xyz`，应该提示「没有找到」。
6. 同样方法测试「6. 按作者查询」。

## 五、容易踩的坑

- **两个函数后面的 `const` 不能删**：表示「只查不改」。所以你在里面**不能**修改 `books`，只能读和打印。
- **判断条件别写反**：是 `!= std::string_view::npos`（找到了），不是 `==`。写成 `==` 就变成「没找到才打印」，逻辑全反。
- **别忘了 `found` 标记**：如果一本都没匹配上，要给用户一句「没找到」的提示，否则用户会以为程序卡住了。
- **关键字也别带空格**：和录入一样，`std::cin >>` 读到空格就停，输入关键字时尽量用完整的词。
