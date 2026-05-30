# 同学 A 专属说明 —— 文件持久化模块

> 你负责的文件：**只改 [src/BookManager_IO.cpp](../../src/BookManager_IO.cpp) 这一个文件**，别的不用动。

## 💬 给你的话

你负责的是整个系统的"记忆功能"——让数据能够保存下来，下次打开还在。这个功能看起来简单，但非常关键：没有你的工作，用户每次关掉程序数据就全丢了。

**不用担心**：文件读写听起来复杂，但其实就是"把内存的东西写成文本"和"把文本读回内存"。这份文档会把每个步骤都讲清楚，代码可以直接复制。

**设计思路**：我们用最简单的文本格式存储——一行一本书，字段用 `|` 分隔。为什么用 `|` 而不是逗号？因为书名、出版社里可能有逗号，但很少有 `|`，这样不容易出错。

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

### 前置条件
先确保同学 B 的「添加图书」能用，或者你可以临时自己实现一个简单版本来测试。

### 第一步：编译
在项目根目录运行：

```bash
g++.exe -std=c++17 -g src/*.cpp -o build/main.exe
./build/main.exe
```

**编译成功的标志**：没有红色的 `error:` 行。

**常见编译错误**：
- `error: 'stringstream' is not a member of 'std'`：忘了 `#include <sstream>`
- `error: 'stod' is not a member of 'std'`：检查是否用了 `-std=c++17` 编译选项

### 第二步：测试保存功能
1. 启动程序，先添加 1-2 本书（选菜单 3）
2. 选择「2. 保存图书信息到文件」
3. 输入文件名：`test_books.txt`

**预期输出**：

```text
已保存 2 条记录到 test_books.txt
```

4. 用记事本或 VSCode 打开项目根目录下的 `test_books.txt`

**预期内容**（类似这样）：

```text
B001|C++Primer|Lippman|TP312|人民邮电|2013|99
B002|Java核心技术|Horstmann|TP312|机械工业|2016|108.5
```

**如果文件是空的**：检查 `saveToFile` 里的循环是否正确执行。

**如果格式不对**：检查输出语句里的 `|` 和 `\n` 是否都写对了。

### 第三步：测试加载功能
1. 退出程序，重新运行 `./build/main.exe`
2. 选择「1. 从文件加载图书信息」
3. 输入文件名：`test_books.txt`

**预期输出**：

```text
已从 test_books.txt 加载 2 条记录
```

4. 选择「4. 浏览全部图书」，确认刚才保存的书都回来了

**如果加载后数量不对**：
- 数量翻倍？忘了 `books.clear()`
- 数量为 0？检查文件路径是否正确，或者 `getline` 循环是否有问题

**如果程序崩溃**：
- 可能是 `std::stod` 遇到了非数字。用记事本检查文件里价格那一列是否都是数字。

### 第四步：边界测试
- 测试加载一个不存在的文件（应该提示"无法打开文件"）
- 测试保存 0 本书（应该提示"已保存 0 条记录"）
- 测试多次加载同一个文件（确认不会重复累加）

## 五、容易踩的坑

- **忘了 `books.clear()`**：每次加载都会把数据再追加一遍，越加越多。`loadFromFile` 开头一定要清空。
  
- **`string_view` 不能直接给 ofstream 当文件名**：必须 `std::string(filename)` 包一下（代码里已经处理）。
  
- **`std::stod` 遇到非数字会崩**：如果文件被手动改坏了，价格那一段不是数字就会出错。初学阶段先保证自己存的格式正确即可。
  
- **不要去改 `saveToFile` 的 `const`**：它后面那个 `const` 表示「这个方法不会修改图书」，是头文件里规定好的，别删。

## 六、做完之后

### ✅ 自我检查清单

- [ ] 编译通过，没有 error
- [ ] 能保存图书到文件，文件内容格式正确
- [ ] 能从文件加载图书，数量和内容都对
- [ ] 多次加载不会重复累加数据
- [ ] 加载不存在的文件会友好提示

### 🤝 告诉队友

你的模块做完后，**在群里说一声**，因为：
- 所有人都需要你的保存/加载功能来持久化测试数据
- 有了你的模块，大家就不用每次启动都重新输入测试数据了

### 🚀 进阶挑战（选做）

如果你提前做完，可以尝试：

1. **错误恢复**：如果文件某一行格式错误，跳过这一行继续读后面的
2. **备份功能**：保存时自动备份旧文件为 `.bak`
3. **CSV 格式**：改用标准的 CSV 格式（处理字段内的逗号和引号）

不会做也没关系，完成基本功能就很棒了！有问题随时问组里 💪
