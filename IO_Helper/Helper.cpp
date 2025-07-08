#include "Helper.h"
#include <sstream>
#include "../Logger/Logger.h"

namespace StreamControl {

Helper::Helper() : input(std::cin), output(std::cout) { }

Helper::~Helper() = default;

void Helper::compile() {
    if(!input.isValid() || !output.isValid()) {
        Logger::ErrorLogger::IOError(std::runtime_error("Input or Output stream is not properly initialized."), -1, "Helper::compile() called with invalid streams.");
        return;
    }
    std::vector<std::pair<std::string, int>> lines = input.ReadLines();
    std::string content;
    content.reserve(1024);
    for(const auto& [line, lineNumber] : lines) {
        content += line;
        content.push_back('\n');
    }
    Command::Tokenizer tokenizer(content);
    Memory::SymbolTable symbolTable;
    Command::CodeGenerator codeGen;
    Command::Parser parser(tokenizer, symbolTable, codeGen);
    parser.compileClass();
    output.WriteLines(codeGen.getOutput());
}

} // namespace StreamControl
