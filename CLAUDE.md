# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run

Single-translation-unit-per-file project compiled together as one executable. From the repo root:

```bash
g++.exe -std=c++17 -g src/*.cpp -o build/main.exe
./build/main.exe
```

The same command is wired up as the default VSCode build task in [.vscode/tasks.json](.vscode/tasks.json). There are no tests and no other build system.

Build artifacts go in `build/` (covered by [.gitignore](.gitignore) — locally built binaries should not be committed).

## Architecture

This is a 5-person team project (图书信息管理系统 / Book Information Management System). The architecture is deliberately split so each teammate owns one `.cpp` file while sharing a single `BookManager` class declaration.

### The split-implementation pattern

`BookManager` is declared once in [src/BookManager.h](src/BookManager.h) but its methods are implemented across five separate `.cpp` files, one per module/teammate. The linker reassembles them into a single class:

| File | Module | Methods |
|------|--------|---------|
| [src/BookManager_IO.cpp](src/BookManager_IO.cpp) | A — persistence | `loadFromFile`, `saveToFile` |
| [src/BookManager_Add.cpp](src/BookManager_Add.cpp) | B — entry & browse | `addBook`, `displayAll` |
| [src/BookManager_Search.cpp](src/BookManager_Search.cpp) | C — query | `searchByTitle`, `searchByAuthor` |
| [src/BookManager_Edit.cpp](src/BookManager_Edit.cpp) | D — modify/delete | `deleteBook`, `modifyBook` |
| [src/BookManager_Sort.cpp](src/BookManager_Sort.cpp) | E — sort | `sortBooks` |

When changing behavior, edit the module file — never add a method body to a different module's `.cpp`. The header is the shared contract; do not change signatures without coordinating across all modules.

As of writing, every `BookManager_*.cpp` method is a stub returning immediately. The header, [src/main.cpp](src/main.cpp) menu dispatch, and [src/Book.h](src/Book.h) data model are complete; the modules are the implementation work.

### Other files

- [src/Book.h](src/Book.h) / [src/Book.cpp](src/Book.cpp) — shared data model. Getters return `std::string_view`; setters and the constructor accept `std::string_view`. All seven fields (bookId, title, author, categoryId, publisher, publishTime, price) are stored as owning members.
- [src/main.cpp](src/main.cpp) — menu loop only. Reads a numeric choice, prompts for a single `std::string` argument when needed, dispatches to one `BookManager` method. Do not put business logic here.
