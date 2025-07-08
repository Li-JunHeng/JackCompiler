#include "Parser.h"
#include <stdexcept>
#include "../Logger/Logger.h"

using namespace std;
using namespace Memory;

namespace Command {

Parser::Parser(Tokenizer& tokenizerRef, SymbolTable& symbolTableRef, CodeGenerator& codeGenRef)
    : tokenizer(tokenizerRef), symbolTable(symbolTableRef), codeGen(codeGenRef) { }

void Parser::compileClass() {
    tokenizer.advance();
    if(tokenizer.tokenType() != TokenType::KEYWORD || tokenizer.tokenValue() != "class") {
        Logger::ErrorLogger::cmdLogError(invalid_argument("Expected 'class' keyword"), tokenizer.tokenLine(), tokenizer.tokenValue());
    }
    tokenizer.advance();
    if(tokenizer.tokenType() != TokenType::IDENTIFIER) {
        Logger::ErrorLogger::cmdLogError(invalid_argument("Expected class name identifier"), tokenizer.tokenLine(), tokenizer.tokenValue());
    }
    currentClassName = tokenizer.tokenValue();
    symbolTable.startClass();  // reset symbol table for new class
    tokenizer.advance();
    expect("{");
    while(tokenizer.tokenType() == TokenType::KEYWORD &&
         (tokenizer.tokenValue() == "static" || tokenizer.tokenValue() == "field")) {
        compileClassVarDec();
    }
    while(tokenizer.tokenType() == TokenType::KEYWORD &&
         (tokenizer.tokenValue() == "constructor" || tokenizer.tokenValue() == "function" || tokenizer.tokenValue() == "method")) {
        compileSubroutineDec();
    }
    expect("}");
}

void Parser::compileClassVarDec() {
    string kindKeyword = tokenizer.tokenValue(); // "static" or "field"
    Kind kind = (kindKeyword == "static" ? Kind::STATIC : Kind::FIELD);
    tokenizer.advance();
    if(tokenizer.tokenType() != TokenType::KEYWORD && tokenizer.tokenType() != TokenType::IDENTIFIER) {
        Logger::ErrorLogger::cmdLogError(invalid_argument("Expected type in class variable declaration"), tokenizer.tokenLine(), tokenizer.tokenValue());
    }
    string varType = tokenizer.tokenValue();
    tokenizer.advance();
    if(tokenizer.tokenType() != TokenType::IDENTIFIER) {
        Logger::ErrorLogger::cmdLogError(invalid_argument("Expected variable name"), tokenizer.tokenLine(), tokenizer.tokenValue());
    }
    string varName = tokenizer.tokenValue();
    symbolTable.define(varName, varType, kind);
    tokenizer.advance();
    while(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.tokenValue() == ",") {
        tokenizer.advance();
        if(tokenizer.tokenType() != TokenType::IDENTIFIER) {
            Logger::ErrorLogger::cmdLogError(invalid_argument("Expected variable name after ','"), tokenizer.tokenLine(), tokenizer.tokenValue());
        }
        varName = tokenizer.tokenValue();
        symbolTable.define(varName, varType, kind);
        tokenizer.advance();
    }
    expect(";");
}

void Parser::compileSubroutineDec() {
    string subroutineType = tokenizer.tokenValue();
    symbolTable.startSubroutine();

    if(subroutineType == "method") {
        symbolTable.define("this", currentClassName, Kind::ARG);
    }

    tokenizer.advance();
    if(tokenizer.tokenType() != TokenType::KEYWORD && tokenizer.tokenType() != TokenType::IDENTIFIER) {
        Logger::ErrorLogger::cmdLogError(invalid_argument("Expected return type"), tokenizer.tokenLine(), tokenizer.tokenValue());
    }

    tokenizer.advance();
    if(tokenizer.tokenType() != TokenType::IDENTIFIER) {
        Logger::ErrorLogger::cmdLogError(invalid_argument("Expected subroutine name"), tokenizer.tokenLine(), tokenizer.tokenValue());
    }
    string subroutineName = tokenizer.tokenValue();

    tokenizer.advance();
    expect("(");
    compileParameterList(subroutineType);
    expect(")");
    expect("{");

    while(tokenizer.tokenType() == TokenType::KEYWORD && tokenizer.tokenValue() == "var") {
        compileVarDec();
    }

    int numLocals = symbolTable.varCount(Kind::VAR);
    codeGen.writeFunction(currentClassName + "." + subroutineName, numLocals);

    if(subroutineType == "constructor") {
        int fieldCount = symbolTable.varCount(Kind::FIELD);
        codeGen.writePush("constant", fieldCount);
        codeGen.writeCall("Memory.alloc", 1);
        codeGen.writePop("pointer", 0);
    } else if(subroutineType == "method") {
        codeGen.writePush("argument", 0);
        codeGen.writePop("pointer", 0);
    }

    compileStatements();
    expect("}");
}

void Parser::compileParameterList(const std::string& subroutineType) {
    if(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.tokenValue() == ")") {
        return;
    }
    while(true) {
        if(tokenizer.tokenType() != TokenType::KEYWORD && tokenizer.tokenType() != TokenType::IDENTIFIER) {
            Logger::ErrorLogger::cmdLogError(invalid_argument("Expected parameter type"), tokenizer.tokenLine(), tokenizer.tokenValue());
        }
        string paramType = tokenizer.tokenValue();
        tokenizer.advance();
        if(tokenizer.tokenType() != TokenType::IDENTIFIER) {
            Logger::ErrorLogger::cmdLogError(invalid_argument("Expected parameter name"), tokenizer.tokenLine(), tokenizer.tokenValue());
        }
        string paramName = tokenizer.tokenValue();
        symbolTable.define(paramName, paramType, Kind::ARG);
        tokenizer.advance();
        if(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.tokenValue() == ",") {
            tokenizer.advance();
            continue;
        }
        break;
    }
}

void Parser::compileVarDec() {
    tokenizer.advance();
    if(tokenizer.tokenType() != TokenType::KEYWORD && tokenizer.tokenType() != TokenType::IDENTIFIER) {
        Logger::ErrorLogger::cmdLogError(invalid_argument("Expected type in var declaration"), tokenizer.tokenLine(), tokenizer.tokenValue());
    }
    string varType = tokenizer.tokenValue();
    tokenizer.advance();
    if(tokenizer.tokenType() != TokenType::IDENTIFIER) {
        Logger::ErrorLogger::cmdLogError(invalid_argument("Expected var name"), tokenizer.tokenLine(), tokenizer.tokenValue());
    }
    string varName = tokenizer.tokenValue();
    symbolTable.define(varName, varType, Kind::VAR);
    tokenizer.advance();
    while(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.tokenValue() == ",") {
        tokenizer.advance();
        if(tokenizer.tokenType() != TokenType::IDENTIFIER) {
            Logger::ErrorLogger::cmdLogError(invalid_argument("Expected var name after ','"), tokenizer.tokenLine(), tokenizer.tokenValue());
        }
        varName = tokenizer.tokenValue();
        symbolTable.define(varName, varType, Kind::VAR);
        tokenizer.advance();
    }
    expect(";");
}

void Parser::compileStatements() {
    while(tokenizer.tokenType() == TokenType::KEYWORD) {
        string keyword = tokenizer.tokenValue();
        if(keyword == "let") {
            compileLet();
        } else if(keyword == "if") {
            compileIf();
        } else if(keyword == "while") {
            compileWhile();
        } else if(keyword == "do") {
            compileDo();
        } else if(keyword == "return") {
            compileReturn();
        } else {
            break;
        }
    }
}

void Parser::compileLet() {
    tokenizer.advance();
    if(tokenizer.tokenType() != TokenType::IDENTIFIER) {
        Logger::ErrorLogger::cmdLogError(invalid_argument("Expected variable name after 'let'"), tokenizer.tokenLine(), tokenizer.tokenValue());
    }
    string varName = tokenizer.tokenValue();
    Kind kind = symbolTable.kindOf(varName);
    if(kind == Kind::NONE) {
        Logger::ErrorLogger::cmdLogError(invalid_argument("Undefined variable in let"), tokenizer.tokenLine(), varName);
    }
    string segment;
    switch(kind) {
        case Kind::STATIC: segment = "static"; break;
        case Kind::FIELD:  segment = "this";   break;
        case Kind::ARG:    segment = "argument"; break;
        case Kind::VAR:    segment = "local"; break;
        default: segment = "";
    }
    int index = symbolTable.indexOf(varName);
    bool isArray = false;
    tokenizer.advance();
    if(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.tokenValue() == "[") {
        isArray = true;
        codeGen.writePush(segment, index);
        tokenizer.advance();
        compileExpression();
        expect("]");
        codeGen.writeArithmetic("add");
        codeGen.writePop("temp", 2);
        expect("=");
        compileExpression();
        codeGen.writePop("temp", 0);
        codeGen.writePush("temp", 2);
        codeGen.writePop("pointer", 1);
        codeGen.writePush("temp", 0);
        codeGen.writePop("that", 0);
        expect(";");
        return;
    }
    expect("=");
    compileExpression();
    expect(";");
    codeGen.writePop(segment, index);
}

void Parser::compileIf() {
    tokenizer.advance();
    expect("(");
    compileExpression();
    expect(")");
    int labelId = codeGen.getNextIfLabelIndex();
    string trueLabel = "IF_TRUE" + to_string(labelId);
    string falseLabel = "IF_FALSE" + to_string(labelId);
    string endLabel = "IF_END" + to_string(labelId);
    codeGen.writeIf(trueLabel);
    codeGen.writeGoto(falseLabel);
    codeGen.writeLabel(trueLabel);
    expect("{");
    compileStatements();
    expect("}");
    if(tokenizer.tokenType() == TokenType::KEYWORD && tokenizer.tokenValue() == "else") {
        codeGen.writeGoto(endLabel);
        codeGen.writeLabel(falseLabel);
        tokenizer.advance();
        expect("{");
        compileStatements();
        expect("}");
        codeGen.writeLabel(endLabel);
    } else {
        codeGen.writeLabel(falseLabel);
    }
}

void Parser::compileWhile() {
    tokenizer.advance();
    expect("(");
    int labelId = codeGen.getNextWhileLabelIndex();
    string expLabel = "WHILE_EXP" + to_string(labelId);
    string endLabel = "WHILE_END" + to_string(labelId);
    codeGen.writeLabel(expLabel);
    compileExpression();
    expect(")");
    codeGen.writeIf("WHILE_TRUE" + to_string(labelId));
    codeGen.writeGoto(endLabel);
    codeGen.writeLabel("WHILE_TRUE" + to_string(labelId));
    expect("{");
    compileStatements();
    expect("}");
    codeGen.writeGoto(expLabel);
    codeGen.writeLabel(endLabel);
}

void Parser::compileDo() {
    tokenizer.advance();
    compileTerm();
    expect(";");
    codeGen.writePop("temp", 0);
}

void Parser::compileReturn() {
    tokenizer.advance();
    if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.tokenValue() == ";") {
        codeGen.writePush("constant", 0); // push 0 as a placeholder return value
        codeGen.writeReturn();
        tokenizer.advance(); // Advance past the semicolon
        return;
    }
    compileExpression(); // Compile the return expression
    expect(";");         // Consume the semicolon
    codeGen.writeReturn();
}

void Parser::compileExpression() {
    compileTerm();
    static const std::string opSymbols = "+-*/&|<>=";
    while(tokenizer.tokenType() == TokenType::SYMBOL && opSymbols.find(tokenizer.tokenValue()) != string::npos) {
        string op = tokenizer.tokenValue();
        tokenizer.advance();
        compileTerm();
        if(op == "+") {
            codeGen.writeArithmetic("add");
        } else if(op == "-") {
            codeGen.writeArithmetic("sub");
        } else if(op == "*") {
            codeGen.writeCall("Math.multiply", 2);
        } else if(op == "/") {
            codeGen.writeCall("Math.divide", 2);
        } else if(op == "&") {
            codeGen.writeArithmetic("and");
        } else if(op == "|") {
            codeGen.writeArithmetic("or");
        } else if(op == "<") {
            codeGen.writeArithmetic("lt");
        } else if(op == ">") {
            codeGen.writeArithmetic("gt");
        } else if(op == "=") {
            codeGen.writeArithmetic("eq");
        }
    }
}

void Parser::compileTerm() {
    if(tokenizer.tokenType() == TokenType::INT_CONST) {
        int value = stoi(tokenizer.tokenValue());
        codeGen.writePush("constant", value);
        tokenizer.advance();
    }
    else if(tokenizer.tokenType() == TokenType::STRING_CONST) {
        string str = tokenizer.tokenValue();
        codeGen.writePush("constant", (int)str.size());
        codeGen.writeCall("String.new", 1);
        for(char ch : str) {
            codeGen.writePush("constant", (int)static_cast<unsigned char>(ch));
            codeGen.writeCall("String.appendChar", 2);
        }
        tokenizer.advance();
    }
    else if(tokenizer.tokenType() == TokenType::KEYWORD) {
        string kw = tokenizer.tokenValue();
        if(kw == "true") {
            codeGen.writePush("constant", 1);
            codeGen.writeArithmetic("neg");
        } else if(kw == "false" || kw == "null") {
            codeGen.writePush("constant", 0);
        } else if(kw == "this") {
            codeGen.writePush("pointer", 0);
        }
        tokenizer.advance();
    }
    else if(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.tokenValue() == "(") {
        tokenizer.advance();
        compileExpression();
        expect(")");
    }
    else if(tokenizer.tokenType() == TokenType::SYMBOL && (tokenizer.tokenValue() == "-" || tokenizer.tokenValue() == "~")) {
        string unaryOp = tokenizer.tokenValue();
        tokenizer.advance();
        compileTerm();
        if(unaryOp == "-") {
            codeGen.writeArithmetic("neg");
        } else if(unaryOp == "~") {
            codeGen.writeArithmetic("not");
        }
    }
    else if(tokenizer.tokenType() == TokenType::IDENTIFIER) {
        string identifier = tokenizer.tokenValue();
        tokenizer.advance();
        if(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.tokenValue() == "[") {
            Kind kind = symbolTable.kindOf(identifier);
            if(kind == Kind::NONE) {
                Logger::ErrorLogger::cmdLogError(invalid_argument("Undefined array variable"), tokenizer.tokenLine(), identifier);
            }
            string baseSegment;
            switch(kind) {
                case Kind::STATIC: baseSegment = "static"; break;
                case Kind::FIELD:  baseSegment = "this"; break;
                case Kind::ARG:    baseSegment = "argument"; break;
                case Kind::VAR:    baseSegment = "local"; break;
                default: baseSegment = "";
            }
            int baseIndex = symbolTable.indexOf(identifier);
            codeGen.writePush(baseSegment, baseIndex);
            tokenizer.advance();
            compileExpression();
            expect("]");
            codeGen.writeArithmetic("add");
            codeGen.writePop("pointer", 1); // put computed address into pointer1
            codeGen.writePush("that", 0);   // push value at that address
        }
        else if(tokenizer.tokenType() == TokenType::SYMBOL && (tokenizer.tokenValue() == "(" || tokenizer.tokenValue() == ".")) {
            string callName;
            int nArgs = 0;
            if(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.tokenValue() == "(") {
                callName = currentClassName + "." + identifier;
                codeGen.writePush("pointer", 0);
                nArgs = 1;
                tokenizer.advance();
            } else if(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.tokenValue() == ".") {
                tokenizer.advance();
                string subName = tokenizer.tokenValue();
                Kind kind = symbolTable.kindOf(identifier);
                if(kind != Kind::NONE) {
                    string objType = symbolTable.typeOf(identifier);
                    string objSegment;
                    switch(kind) {
                        case Kind::STATIC: objSegment = "static"; break;
                        case Kind::FIELD:  objSegment = "this"; break;
                        case Kind::ARG:    objSegment = "argument"; break;
                        case Kind::VAR:    objSegment = "local"; break;
                        default: objSegment = "";
                    }
                    int objIndex = symbolTable.indexOf(identifier);
                    codeGen.writePush(objSegment, objIndex);
                    nArgs = 1;
                    callName = objType + "." + subName;
                } else {
                    callName = identifier + "." + subName;
                }
                tokenizer.advance();
                expect("(");
            }
            compileExpressionList(nArgs);
            expect(")");
            codeGen.writeCall(callName, nArgs);
        }
        else {
            Kind kind = symbolTable.kindOf(identifier);
            if(kind == Kind::NONE) {
                Logger::ErrorLogger::cmdLogError(invalid_argument("Undefined variable in expression"), tokenizer.tokenLine(), identifier);
            }
            string seg;
            switch(kind) {
                case Kind::STATIC: seg = "static"; break;
                case Kind::FIELD:  seg = "this"; break;
                case Kind::ARG:    seg = "argument"; break;
                case Kind::VAR:    seg = "local"; break;
                default: seg = "";
            }
            int idx = symbolTable.indexOf(identifier);
            codeGen.writePush(seg, idx);
        }
    }
    else {
        Logger::ErrorLogger::cmdLogError(invalid_argument("Invalid term in expression"), tokenizer.tokenLine(), tokenizer.tokenValue());
    }
}

void Parser::compileExpressionList(int& argCount) {
    if(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.tokenValue() == ")") {
        return;
    }
    compileExpression();
    argCount++;
    while(tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.tokenValue() == ",") {
        tokenizer.advance();
        compileExpression();
        argCount++;
    }
}

void Parser::expect(const std::string& expected) {
    if(tokenizer.tokenType() == TokenType::SYMBOL || tokenizer.tokenType() == TokenType::KEYWORD) {
        if(tokenizer.tokenValue() == expected) {
            tokenizer.advance();
            return;
        }
    }
    Logger::ErrorLogger::cmdLogError(invalid_argument(std::string("Expected '") + expected + "'"), tokenizer.tokenLine(), tokenizer.tokenValue());
}

} // namespace Command
