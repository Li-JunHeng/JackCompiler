cmake_minimum_required(VERSION 3.31)
project(JackCompiler VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)

# 源文件列表
set(JACK_SRCS
    main.cpp
    Memory/SymbolTable.cpp
    Memory/SymbolTable.h
    Command/Tokenizer.h
    Command/Tokenizer.cpp
    Command/CodeGenerator.cpp
    Command/CodeGenerator.h
    Command/Parser.cpp
    Command/Parser.h
    Helper/Usage.cpp
    Helper/Usage.h
    IO_Helper/Helper.cpp
    IO_Helper/Helper.h
    IO_Helper/InputHelper.cpp
    IO_Helper/InputHelper.h
    IO_Helper/OutputHelper.cpp
    IO_Helper/OutputHelper.h
    Logger/Logger.h
    Logger/ErrorLogger.cpp
    Logger/ErrorLogger.h
)

add_executable(JackCompiler ${JACK_SRCS})

# 编译期宏
target_compile_definitions(JackCompiler
    PRIVATE
        PROJECT_VERSION=\"${PROJECT_VERSION}\"
)

# Release 模式加入高优化和隐藏符号
target_compile_options(JackCompiler
    PRIVATE
      $<$<CONFIG:Release>:-O3>
)
target_link_options(JackCompiler
    PRIVATE
      $<$<CONFIG:Release>:-s>
)

set(EXTRA_OUTPUT_DIR "/Users/lijunheng/Tools/Nand2Tetris")
file(MAKE_DIRECTORY ${EXTRA_OUTPUT_DIR})

add_custom_command(TARGET JackCompiler POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:JackCompiler>
        ${EXTRA_OUTPUT_DIR}/
    COMMENT "复制 JackCompiler 到 ${EXTRA_OUTPUT_DIR}"
)