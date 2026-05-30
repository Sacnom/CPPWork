# 同学 D 专属说明 —— 修改与删除模块

> 你负责的文件：**只改 [src/BookManager_Edit.cpp](../../src/BookManager_Edit.cpp) 这一个文件**，别的不用动。

## 💬 给你的话

你负责的是"数据维护"功能——让用户能够纠正错误信息或删除不需要的图书。这个功能看起来简单，但需要细心处理边界情况（比如删除不存在的书）。

**不用担心**：删除和修改的核心都是"先找到那本书"，找到了就好办。这份文档会把迭代器和引用的用法讲清楚，代码可以直接复制。

**设计思路**：我们用登录号（bookId）作为唯一标识来定位图书，因为登录号不会重复。修改时不允许改登录号本身，因为改了就找不到了——如果真要改登录号，只能删掉重新添加。

## 一、你要做什么（用大白话）

用户给你一个「登录号」（每本书的唯一标识），你要么把这本书删掉，要么让用户重新输入信息把它改掉。

- **`deleteBook`（删除图书）**：根据登录号找到那本书，从内存里删掉。
- **`modifyBook`（修改图书）**：根据登录号找到那本书，让用户重新输入各个字段，更新它。

菜单里「7. 删除图书」调用 `deleteBook`，「8. 修改图书」调用 `modifyBook`。

## 二、先弄懂几个概念

**1. 图书存在哪？**
`BookManager` 里有成员变量 `books`，是 `std::vector<Book>`（一个图书数组）。你要做的就是在这个数组里找到指定登录号的那本书，然后删掉或改掉。

**2. 怎么找到那本书？**
用循环遍历 `books`，每本书都用 `b.getBookId()` 拿它的登录号，和用户给的登录号比较。找到了就停下来操作。

**3. 怎么删除 vector 里的一个元素？**
用 `books.erase(迭代器)`。迭代器可以理解成「指向数组某个位置的指针」。遍历的时候用 `auto it = books.begin(); it != books.end(); ++it`，找到了就 `books.erase(it)`。

**4. 怎么修改一本书的信息？**
找到那本书后，用它的 setter 方法（`setTitle`、`setAuthor` 等）把新值写进去。

## 三、完整代码（可直接复制，建议自己读懂每一行）

把下面整段替换掉 [BookManager_Edit.cpp](../BookManager_Edit.cpp) 里的内容：

```cpp
#include "BookManager.h"

#include <iostream>
#include <string>

// Module D: 修改与删除（同学D负责）

// 删除图书：根据登录号找到那本书，从 books 里删掉
void BookManager::deleteBook(std::string_view bookId) {
    // 用迭代器遍历，因为 erase 需要迭代器
    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->getBookId() == bookId) {   // 找到了
            books.erase(it);               // 删掉这本书
            std::cout << "已删除登录号为「" << bookId << "」的图书。\n";
            return;  // 删完就结束，不用继续找了
        }
    }
    // 循环走完都没找到
    std::cout << "未找到登录号为「" << bookId << "」的图书，删除失败。\n";
}

// 修改图书：根据登录号找到那本书，让用户重新输入各字段，用 setter 更新
void BookManager::modifyBook(std::string_view bookId) {
    // 用普通引用遍历，因为要改它
    for (auto& b : books) {
        if (b.getBookId() == bookId) {   // 找到了
            std::cout << "找到图书：" << b.getTitle() << "，请输入新的信息。\n";

            std::string newTitle, newAuthor, newCategoryId;
            std::string newPublisher, newPublishTime;
            double newPrice = 0;

            std::cout << "新书名: ";       std::cin >> newTitle;
            std::cout << "新作者: ";       std::cin >> newAuthor;
            std::cout << "新分类号: ";     std::cin >> newCategoryId;
            std::cout << "新出版社: ";     std::cin >> newPublisher;
            std::cout << "新出版时间: ";   std::cin >> newPublishTime;
            std::cout << "新价格: ";       std::cin >> newPrice;

            // 用 setter 把新值写进去
            b.setTitle(newTitle);
            b.setAuthor(newAuthor);
            b.setCategoryId(newCategoryId);
            b.setPublisher(newPublisher);
            b.setPublishTime(newPublishTime);
            b.setPrice(newPrice);

            std::cout << "修改成功！\n";
            return;  // 改完就结束
        }
    }
    // 循环走完都没找到
    std::cout << "未找到登录号为「" << bookId << "」的图书，修改失败。\n";
}
```

> **关键区别**：
> - `deleteBook` 用 `auto it = books.begin()` 这种迭代器写法，因为 `erase` 必须传迭代器。
> - `modifyBook` 用 `auto& b` 这种引用写法，因为要改 `b` 的内容。注意这里**不能**加 `const`，否则改不了。

## 四、怎么测试你的代码

1. 先确保同学 B 的「添加图书」和「浏览全部」能用（你需要先有几本书）。
2. 编译并运行：
   ```bash
   g++.exe -std=c++17 -g src/*.cpp -o build/main.exe
   ./build/main.exe
   ```
3. 添加几本书，记住其中一本的登录号，比如 `B001`。
4. 选「7. 删除图书」，输入 `B001`，应该提示「已删除」。
5. 再选「4. 浏览全部」，确认那本书不见了。
6. 再添加一本书，选「8. 修改图书」，输入它的登录号，按提示输入新信息，再浏览确认改成功了。
7. 试试输入一个不存在的登录号，应该提示「未找到」。

## 五、容易踩的坑

- **`deleteBook` 里用 `it->getBookId()`**：迭代器要用 `->` 访问成员，不是 `.`。
  
- **`modifyBook` 里必须用 `auto&`（引用）**：如果写成 `auto b`（没有 `&`），你改的只是一个副本，原数组里的书不会变。
  
- **`erase` 之后不能再用那个迭代器**：`erase(it)` 之后 `it` 就失效了，所以紧接着 `return`，不要继续循环。
  
- **登录号不能改**：注意代码里没有 `setBookId`，因为登录号是唯一标识，改了就找不到了。如果用户真要改登录号，只能删掉重新添加。
  
- **这两个方法都不是 `const`**：因为要改 `books`（删除或修改），所以头文件里它们后面没有 `const`，你也别加。

## 六、做完之后

### ✅ 自我检查清单

- [ ] 编译通过，没有 error
- [ ] 能删除存在的图书，浏览时确认已删除
- [ ] 删除不存在的登录号会提示"未找到"
- [ ] 能修改存在的图书，浏览时确认信息已更新
- [ ] 修改不存在的登录号会提示"未找到"

### 🤝 告诉队友

你的模块做完后，**在群里说一声**。数据维护功能让整个系统更完善，用户可以放心地纠正错误了！

### 🚀 进阶挑战（选做）

如果你提前做完，可以尝试：

1. **删除前确认**：删除前显示图书信息，问用户"确定删除吗？(y/n)"
2. **批量删除**：支持一次删除多本书（输入多个登录号）
3. **部分修改**：不强制重新输入所有字段，允许只改某几个字段
4. **撤销功能**：删除前先备份，提供"撤销上一次删除"的功能

不会做也没关系，完成基本功能就很棒了！有问题随时问组里 💪
