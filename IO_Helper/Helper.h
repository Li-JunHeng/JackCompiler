#pragma once
#ifndef HELPER_H
#define HELPER_H

#include "InputHelper.h"
#include "OutputHelper.h"
#include "../Command/Tokenizer.h"
#include "../Command/Parser.h"
#include "../Command/CodeGenerator.h"
#include "../Memory/SymbolTable.h"

namespace StreamControl {

class Helper {
public:
    Helper();
    Helper(const std::string &inputFile, const std::string &outputFile)
        : input(inputFile), output(outputFile) {}
    Helper(std::ifstream& inputStream, std::ofstream& outputStream)
        : input(inputStream), output(outputStream) {}
    Helper(InputHelper& in, OutputHelper& out)
        : input(in), output(out) {}
    ~Helper();

    // Compiles the entire input (which may be a single Jack source file) and writes the VM output
    void compile();

private:
    InputHelper input;
    OutputHelper output;
};

} // namespace StreamControl

#endif // HELPER_H
