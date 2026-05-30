#include "BookManager.h"

#include <iostream>
#include <string>

// main.cpp
// 系统主入口：只负责菜单调度，各模块逻辑由对应同学在独立 .cpp 文件中实现。

namespace {
void showMenu() {
    std::cout << "\n===== 图书信息管理系统 =====\n"
              << "1. 从文件加载图书信息\n"
              << "2. 保存图书信息到文件\n"
              << "3. 添加图书\n"
              << "4. 浏览全部图书\n"
              << "5. 按书名查询\n"
              << "6. 按作者查询\n"
              << "7. 删除图书\n"
              << "8. 修改图书\n"
              << "9. 排序图书\n"
              << "0. 退出系统\n"
              << "请选择操作: ";
}
}

int main() {
    BookManager manager;
    bool running = true;

    while (running) {
        showMenu();

        int choice = -1;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "输入无效，请重新输入。\n";
            continue;
        }

        std::string input;

        switch (choice) {
        case 1:
            std::cout << "请输入文件名: ";
            std::cin >> input;
            manager.loadFromFile(input);
            break;
        case 2:
            std::cout << "请输入文件名: ";
            std::cin >> input;
            manager.saveToFile(input);
            break;
        case 3:
            manager.addBook();
            break;
        case 4:
            manager.displayAll();
            break;
        case 5:
            std::cout << "请输入书名关键字: ";
            std::cin >> input;
            manager.searchByTitle(input);
            break;
        case 6:
            std::cout << "请输入作者名关键字: ";
            std::cin >> input;
            manager.searchByAuthor(input);
            break;
        case 7:
            std::cout << "请输入登录号: ";
            std::cin >> input;
            manager.deleteBook(input);
            break;
        case 8:
            std::cout << "请输入登录号: ";
            std::cin >> input;
            manager.modifyBook(input);
            break;
        case 9:
            manager.sortBooks();
            break;
        case 0:
            running = false;
            break;
        default:
            std::cout << "没有该选项，请重新选择。\n";
            break;
        }
    }

    std::cout << "系统已退出。\n";
    return 0;
}
