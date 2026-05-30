# 同学 E 专属说明 —— 排序模块

> 你负责的文件：**只改 [src/BookManager_Sort.cpp](../../src/BookManager_Sort.cpp) 这一个文件**，别的不用动。

## 💬 给你的话

你负责的是"数据排序"功能——让用户能按不同方式整理图书列表。这个功能会用到 C++ 标准库的 `std::sort` 和 lambda 表达式，是学习现代 C++ 特性的好机会。

**不用担心**：`std::sort` 是标准库提供的强大工具，你只需要告诉它"怎么比较两本书"就行。lambda 表达式看起来陌生，但其实就是"匿名函数"，这份文档会详细解释。

**设计思路**：我们提供多种排序方式让用户选择（价格、书名、登录号等），而不是固定一种。这样更灵活，满足不同场景的需求。排序是"原地"进行的，会直接改变 `books` 的顺序。

## 一、你要做什么（用大白话）

把内存里的所有图书按某种顺序重新排列。比如按价格从低到高、按书名字母顺序、按登录号顺序等等。

- **`sortBooks`（排序图书）**：让用户选一个排序方式，然后把 `books` 数组重新排列。

菜单里「9. 排序图书」调用你的 `sortBooks`。

## 二、先弄懂几个概念

**1. 图书存在哪？**
`BookManager` 里有成员变量 `books`，是 `std::vector<Book>`（一个图书数组）。你要做的就是把这个数组里的元素重新排列顺序。

**2. C++ 怎么排序？**
用标准库的 `std::sort`，在 `<algorithm>` 头文件里。基本用法：
```cpp
std::sort(数组开头, 数组结尾, 比较函数);
```
对于 `books`，就是：
```cpp
std::sort(books.begin(), books.end(), 比较函数);
```

**3. 什么是「比较函数」？**
就是告诉 `sort`「两本书谁排前面」的规则。用 lambda 表达式（匿名函数）写最方便：
```cpp
[](const Book& a, const Book& b) {
    return a.getPrice() < b.getPrice();  // 价格小的排前面（升序）
}
```
这段代码的意思是：给你两本书 `a` 和 `b`，如果 `a` 的价格比 `b` 小，就返回 `true`（`a` 排前面）。

**4. 怎么让用户选排序方式？**
用 `std::cin` 读一个数字，然后用 `switch` 或 `if` 判断，调用不同的 `std::sort`。

## 三、完整代码（可直接复制，建议自己读懂每一行）

把下面整段替换掉 [BookManager_Sort.cpp](../BookManager_Sort.cpp) 里的内容：

```cpp
#include "BookManager.h"

#include <algorithm>  // std::sort
#include <iostream>

// Module E: 排序控制（同学E负责）

void BookManager::sortBooks() {
    if (books.empty()) {
        std::cout << "目前没有图书，无需排序。\n";
        return;
    }

    std::cout << "\n请选择排序方式：\n"
              << "1. 按价格升序（从低到高）\n"
              << "2. 按价格降序（从高到低）\n"
              << "3. 按书名字母顺序\n"
              << "4. 按登录号顺序\n"
              << "请输入选项: ";

    int choice = 0;
    std::cin >> choice;

    switch (choice) {
    case 1:  // 按价格升序
        std::sort(books.begin(), books.end(),
                  [](const Book& a, const Book& b) {
                      return a.getPrice() < b.getPrice();
                  });
        std::cout << "已按价格升序排序。\n";
        break;

    case 2:  // 按价格降序
        std::sort(books.begin(), books.end(),
                  [](const Book& a, const Book& b) {
                      return a.getPrice() > b.getPrice();  // 注意这里是 >
                  });
        std::cout << "已按价格降序排序。\n";
        break;

    case 3:  // 按书名字母顺序
        std::sort(books.begin(), books.end(),
                  [](const Book& a, const Book& b) {
                      return a.getTitle() < b.getTitle();
                  });
        std::cout << "已按书名字母顺序排序。\n";
        break;

    case 4:  // 按登录号顺序
        std::sort(books.begin(), books.end(),
                  [](const Book& a, const Book& b) {
                      return a.getLoginId() < b.getLoginId();
                  });
        std::cout << "已按登录号顺序排序。\n";
        break;

    default:
        std::cout << "无效选项，排序取消。\n";
        break;
    }
}
```

> **lambda 表达式解释**：
> - `[]` 表示这是一个 lambda（匿名函数）。
> - `(const Book& a, const Book& b)` 是参数：两本书。
> - `{ return ... }` 是函数体：返回 `true` 表示 `a` 排前面。
> - `<` 是升序（小的在前），`>` 是降序（大的在前）。

## 四、怎么测试你的代码

1. 先确保同学 B 的「添加图书」和「浏览全部」能用。
2. 编译并运行：
   ```bash
   g++.exe -std=c++17 -g src/*.cpp -o build/main.exe
   ./build/main.exe
   ```
3. 添加几本价格不同的书，比如 99、50、120。
4. 选「4. 浏览全部」，记住当前顺序。
5. 选「9. 排序图书」→ 选「1. 按价格升序」。
6. 再选「4. 浏览全部」，确认价格从小到大排列了（50、99、120）。
7. 再试试其他排序方式，每次排完都浏览一下确认顺序变了。

## 五、容易踩的坑

- **升序和降序别搞反**：`<` 是升序（小的在前），`>` 是降序（大的在前）。写反了顺序就反了。
  
- **`std::sort` 会直接改 `books`**：排序是「原地」进行的，不会生成新数组。排完之后 `books` 的顺序就永久变了，除非再排一次或者重新加载文件。
  
- **`string_view` 可以直接比较**：`getTitle()` 返回的是 `string_view`，但它支持 `<` 比较（按字典序），所以可以直接用来排序。
  
- **这个方法不是 `const`**：因为要改 `books` 的顺序，所以头文件里它后面没有 `const`，你也别加。
  
- **lambda 里的 `const Book&` 不能省**：参数必须是引用（`&`），否则每次比较都会拷贝一本书，效率低。加 `const` 是因为比较时不需要改书的内容。

## 六、做完之后

### ✅ 自我检查清单

- [ ] 编译通过，没有 error
- [ ] 能按价格升序排序，浏览时确认顺序正确
- [ ] 能按价格降序排序，浏览时确认顺序正确
- [ ] 能按书名排序，浏览时确认字母顺序正确
- [ ] 能按登录号排序，浏览时确认顺序正确
- [ ] 输入无效选项会提示"无效选项"

### 🤝 告诉队友

你的模块做完后，**在群里说一声**。排序功能让用户能更方便地浏览和管理图书，你的工作让整个系统的易用性大大提升！

### 🚀 进阶挑战（选做）

如果你提前做完，可以尝试：

1. **更多排序方式**：按作者、按出版时间、按分类号排序
2. **稳定排序**：价格相同时，按书名排序（二级排序）
3. **自定义排序**：让用户输入排序表达式（高级）
4. **排序历史**：记住上次的排序方式，下次默认使用

不会做也没关系，完成基本功能就很棒了！有问题随时问组里 💪

---

**特别提示**：lambda 表达式是 C++11 引入的现代特性，掌握它对你以后学习 STL 算法很有帮助。如果觉得难理解，可以先把它当成"比较函数的简写"来用，慢慢就熟悉了。
