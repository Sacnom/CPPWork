# 同学 A 专属说明 —— 文件持久化模块

> 你负责的文件：**只改 [src/BookManager_IO.cpp](../../src/BookManager_IO.cpp) 这一个文件**，别的不用动。

## 一、你要做什么（用大白话）

程序运行的时候，所有图书都存在内存里。一旦关掉程序，内存里的东西就没了。
你的任务就是给程序加上「存档」和「读档」两个功能：

- **`saveToFile`（存档）**：把内存里的所有图书写进一个文本文件，关机也不丢。
- **`loadFromFile`（读档）**：下次打开程序，从文件里把图书读回内存。

菜单里的「2. 保存」会调用你的 `saveToFile`，「1. 加载」会调用你的 `loadFromFile`。

## 二、先弄懂几个概念

**1. 图书存在哪？**
`BookManager` 类里有一个成员变量 `books`，类型是 `std::vector<Book>`，可以理解成「一个能自动伸缩的图书数组」。你的代码就是在内存的 `books` 和硬盘文件之间搬数据。

**2. 怎么读写文件？**
C++ 用 `<fstream>` 这个头文件操作文件：
- `std::ofstream` = output file stream，往文件**写**东西（o = out）。
- `std::ifstream` = input file stream，从文件**读**东西（i = in）。
用法和 `std::cout` / `std::cin` 几乎一样，只是对象换成了文件。

**3. 一本书有 7 个字段**，顺序固定（这点全组统一）：
`bookId(登录号), title(书名), author(作者), categoryId(分类号), publisher(出版社), publishTime(出版时间), price(价格)`

**4. 文件里怎么存一本书？**
我们约定：**一行存一本书，7 个字段之间用竖线 `|` 隔开**。比如：
```
B001|C++Primer|Lippman|TP312|人民邮电|2013|99
```
存的时候按这个格式写，读的时候按 `|` 切开，就能一一对应。

## 三、完整代码（可直接复制，建议自己读懂每一行）

把下面整段替换掉 [src/BookManager_IO.cpp](../../src/BookManager_IO.cpp) 里的内容：

```cpp
#include "BookManager.h"

#include <fstream>    // 文件读写
#include <iostream>   // 屏幕输出提示
#include <sstream>    // 把一行字符串按 | 切开
#include <string>

// Module A: 文件持久化（同学A负责）

// 存档：把内存里所有图书写进文件
void BookManager::saveToFile(std::string_view filename) const {
    // 用文件名打开一个「写文件流」。string_view 要转成 string 才能传给 ofstream
    std::ofstream out{std::string(filename)};
    if (!out) {  // 打开失败（比如路径不对）
        std::cout << "无法打开文件，保存失败。\n";
        return;
    }

    // 遍历 books 里的每一本书，一行一本，字段用 | 隔开
    for (const auto& b : books) {
        out << b.getBookId()      << '|'
            << b.getTitle()       << '|'
            << b.getAuthor()      << '|'
            << b.getCategoryId()  << '|'
            << b.getPublisher()   << '|'
            << b.getPublishTime() << '|'
            << b.getPrice()       << '\n';   // 每本书末尾换行
    }

    std::cout << "已保存 " << books.size() << " 条记录到 " << filename << "\n";
}

// 读档：从文件把图书读回内存
void BookManager::loadFromFile(std::string_view filename) {
    std::ifstream in{std::string(filename)};
    if (!in) {
        std::cout << "无法打开文件，加载失败。\n";
        return;
    }

    books.clear();  // 关键！先清空内存，否则会和原来的数据叠加（重复）

    std::string line;
    while (std::getline(in, line)) {   // 一行一行读
        if (line.empty()) continue;    // 跳过空行

        // 把这一行按 | 切成 7 段
        std::stringstream ss{line};
        std::string parts[7];
        std::string field;
        int i = 0;
        while (i < 7 && std::getline(ss, field, '|')) {
            parts[i] = field;
            i++;
        }
        if (i < 7) continue;  // 字段不够 7 个，这行格式不对，跳过

        // parts[6] 是价格，是文字，要用 std::stod 转成 double
        double price = std::stod(parts[6]);

        // 用 7 个字段造一本书，放进 books
        books.emplace_back(parts[0], parts[1], parts[2],
                           parts[3], parts[4], parts[5], price);
    }

    std::cout << "已从 " << filename << " 加载 " << books.size() << " 条记录\n";
}
```

## 四、怎么测试你的代码

1. 先让同学 B 的「添加图书」能用（或者你临时自己加几本测试）。
2. 编译：在项目根目录运行
   ```bash
   g++.exe -std=c++17 -g src/*.cpp -o build/main.exe
   ./build/main.exe
   ```
3. 在菜单里：先添加 1~2 本书 → 选「2. 保存」，文件名输入比如 `books.txt`。
4. 用记事本打开 `books.txt`，确认里面是 `B001|C++Primer|...` 这种格式。
5. 退出程序重新打开 → 选「1. 加载」，输入 `books.txt` → 再选「4. 浏览」，能看到书就成功了。

## 五、容易踩的坑

- **忘了 `books.clear()`**：每次加载都会把数据再追加一遍，越加越多。`loadFromFile` 开头一定要清空。
- **`string_view` 不能直接给 ofstream 当文件名**：必须 `std::string(filename)` 包一下（代码里已经处理）。
- **`std::stod` 遇到非数字会崩**：如果文件被手动改坏了，价格那一段不是数字就会出错。初学阶段先保证自己存的格式正确即可。
- **不要去改 `saveToFile` 的 `const`**：它后面那个 `const` 表示「这个方法不会修改图书」，是头文件里规定好的，别删。
