# 同学 C 专属说明 —— 查询模块

> 你负责的文件：**只改 [src/BookManager_Search.cpp](../../src/BookManager_Search.cpp) 这一个文件**，别的不用动。

## 💬 给你的话

你负责的是"信息检索"功能——帮用户从一堆书里快速找到想要的那本。这是图书管理系统最常用的功能之一，用户体验的好坏很大程度取决于你的工作。

**不用担心**：查询的核心就是"遍历 + 条件判断"，逻辑非常直观。这份文档会把字符串查找的方法讲清楚，代码可以直接复制。

**设计思路**：我们用"模糊匹配"而不是"精确匹配"——用户输入 `C++` 就能找到 `C++Primer`，不需要输入完整书名。这样更人性化，也是大多数搜索引擎的做法。

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

### 前置条件

先确保同学 B 的「添加图书」能用，你需要先有几本书才能测试查询。

### 第一步：编译

```bash
g++.exe -std=c++17 -g src/*.cpp -o build/main.exe
./build/main.exe
```

**编译成功的标志**：没有红色的 `error:` 行。

### 第二步：准备测试数据

添加几本书，建议用这些数据（方便测试各种情况）：

```text
B001, C++Primer, Lippman, TP312, 人民邮电, 2013, 99
B002, Java核心技术, Horstmann, TP312, 机械工业, 2016, 108.5
B003, Python编程, Lutz, TP311, 机械工业, 2017, 119
B004, C++标准库, Josuttis, TP312, 人民邮电, 2015, 128
```

### 第三步：测试按书名查询

选择「5. 按书名查询」，输入 `C++`

**预期输出**：

```text
登录号: B001  书名: C++Primer  作者: Lippman  价格: 99
登录号: B004  书名: C++标准库  作者: Josuttis  价格: 128
```

再试试输入 `Python`，应该只找到一本。

### 第四步：测试按作者查询

选择「6. 按作者查询」，输入 `Lippman`

**预期输出**：

```text
登录号: B001  书名: C++Primer  作者: Lippman  价格: 99
```

### 第五步：测试边界情况

- 输入一个不存在的关键字（比如 `xyz`）

**预期输出**：

```text
没有找到书名包含「xyz」的图书。
```

- 输入空字符串（直接回车），应该匹配所有书（因为所有字符串都"包含"空字符串）

**如果什么都不显示**：
- 检查 `found` 标记是否正确设置
- 检查输出语句是否在 `if` 里面

**如果显示的书不对**：
- 检查 `find` 的返回值判断是否正确（`!= npos` 表示找到）
- 检查是用 `getTitle()` 还是 `getAuthor()`，别搞混了

## 五、容易踩的坑

- **两个函数后面的 `const` 不能删**：表示「只查不改」。所以你在里面**不能**修改 `books`，只能读和打印。
  
- **判断条件别写反**：是 `!= std::string_view::npos`（找到了），不是 `==`。写成 `==` 就变成「没找到才打印」，逻辑全反。
  
- **别忘了 `found` 标记**：如果一本都没匹配上，要给用户一句「没找到」的提示，否则用户会以为程序卡住了。
  
- **关键字也别带空格**：和录入一样，`std::cin >>` 读到空格就停，输入关键字时尽量用完整的词。

## 六、做完之后

### ✅ 自我检查清单

- [ ] 编译通过，没有 error
- [ ] 能按书名查询，找到包含关键字的书
- [ ] 能按作者查询，找到包含关键字的书
- [ ] 查询不存在的关键字会提示"没有找到"
- [ ] 查询结果显示完整信息（登录号、书名、作者、价格）

### 🤝 告诉队友

你的模块做完后，**在群里说一声**。查询功能是用户最常用的，你的工作让整个系统的实用性大大提升！

### 🚀 进阶挑战（选做）

如果你提前做完，可以尝试：

1. **不区分大小写查询**：把关键字和书名都转成小写再比较
2. **多关键字查询**：支持用空格分隔多个关键字，同时满足才显示
3. **高亮显示**：在结果中用特殊符号标出匹配的关键字位置
4. **按分类查询**：增加 `searchByCategory` 方法

不会做也没关系，完成基本功能就很棒了！有问题随时问组里 💪
