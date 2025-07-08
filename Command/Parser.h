#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "Tokenizer.h"
#include "CodeGenerator.h"
#include "../Memory/SymbolTable.h"

namespace Command {

class Parser {
public:
    Parser(Tokenizer& tokenizer, Memory::SymbolTable& symbolTable, CodeGenerator& codeGen);

    // Compiles a complete class from the tokenizer input
    void compileClass();

private:
    // Compilation methods for each non-terminal grammar rule:
    void compileClassVarDec();
    void compileSubroutineDec();
    void compileParameterList(const std::string& subroutineType);
    void compileVarDec();
    void compileStatements();
    void compileLet();
    void compileIf();
    void compileWhile();
    void compileDo();
    void compileReturn();
    void compileExpression();
    void compileTerm();
    void compileExpressionList(int& argCount);

    // Helper to require the current token to match an expected string (symbol or keyword), advance if matches
    void expect(const std::string& expected);

    Tokenizer& tokenizer;
    Memory::SymbolTable& symbolTable;
    CodeGenerator& codeGen;
    std::string currentClassName;
};

} // namespace Command

#endif // PARSER_H
