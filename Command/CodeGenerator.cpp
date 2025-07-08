#include "CodeGenerator.h"
#include <sstream>

namespace Command {

CodeGenerator::CodeGenerator() : ifLabelCounter(0), whileLabelCounter(0) {}

void CodeGenerator::writePush(const std::string& segment, int index) {
    outputLines.emplace_back("push " + segment + " " + std::to_string(index));
}

void CodeGenerator::writePop(const std::string& segment, int index) {
    outputLines.emplace_back("pop " + segment + " " + std::to_string(index));
}

void CodeGenerator::writeArithmetic(const std::string& command) {
    outputLines.emplace_back(command);
}

void CodeGenerator::writeLabel(const std::string& label) {
    outputLines.emplace_back("label " + label);
}

void CodeGenerator::writeGoto(const std::string& label) {
    outputLines.emplace_back("goto " + label);
}

void CodeGenerator::writeIf(const std::string& label) {
    outputLines.emplace_back("if-goto " + label);
}

void CodeGenerator::writeCall(const std::string& name, int nArgs) {
    outputLines.emplace_back("call " + name + " " + std::to_string(nArgs));
}

void CodeGenerator::writeFunction(const std::string& name, int nLocals) {
    outputLines.emplace_back("function " + name + " " + std::to_string(nLocals));
    resetLabelCounters();
}

void CodeGenerator::writeReturn() {
    outputLines.emplace_back("return");
}

const std::vector<std::string>& CodeGenerator::getOutput() const {
    return outputLines;
}

int CodeGenerator::getNextIfLabelIndex() {
    return ifLabelCounter++;
}

int CodeGenerator::getNextWhileLabelIndex() {
    return whileLabelCounter++;
}

void CodeGenerator::resetLabelCounters() {
    ifLabelCounter = 0;
    whileLabelCounter = 0;
}

std::string CodeGenerator::generateUniqueLabel(const std::string& prefix) {
    static int labelCounter = 0;
    return prefix + std::to_string(labelCounter++);
}

void CodeGenerator::writeComment(const std::string& comment) {
    outputLines.emplace_back("// " + comment);
}

} // namespace Command
