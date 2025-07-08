# JackCompiler

JackCompiler 是一个用于将 Jack 语言（.jack 文件）翻译为 VM 代码（.vm 文件）的命令行工具，适用于 Nand2Tetris 课程的第 11 个项目。

## 功能
- 将 Jack 类文件编译为 VM 代码
- 支持类、方法、函数、变量和数组
- 处理控制流语句（if、while）和表达式
- 生成符合 Nand2Tetris VM 规范的代码

## 用法

### 基本用法
```sh
./JackCompiler <input_file.jack> or <directory>
```

### 选项
```sh
./JackCompiler --help      # 显示帮助信息
./JackCompiler --version   # 显示版本信息
./JackCompiler -i <path>   # 指定输入文件或目录
./JackCompiler -o <path>   # 指定输出目录或文件
```

### 示例
编译单个文件：
```sh
./JackCompiler Main.jack
```

编译目录中的所有 .jack 文件：
```sh
./JackCompiler ./MyProject
```

## 构建

本项目使用 CMake 构建：
```sh
mkdir build
cd build
cmake ..
make
```

生成的可执行文件位于 `build/` 目录下。

## 依赖
- C++23 或更高版本
- CMake 3.10+

## 项目结构
- **Command/**: 词法分析器(Tokenizer)、语法分析器(Parser)和代码生成器
- **Memory/**: 符号表(SymbolTable)实现
- **IO_Helper/**: 输入输出流管理
- **Helper/**: 命令行参数解析
- **Logger/**: 错误处理和日志记录

## 参考
- [Nand2Tetris 官方网站](https://www.nand2tetris.org/)
- [Jack 语言规范](https://www.nand2tetris.org/project10)
- [VM 语言规范](https://www.nand2tetris.org/project07)

## 许可证
本项目采用 MIT 许可证 - 详见 LICENSE 文件