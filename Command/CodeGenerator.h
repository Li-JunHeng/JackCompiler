#pragma once
#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <string>
#include <vector>

namespace Command {

class CodeGenerator {
public:
    CodeGenerator();

    // VM writing methods for different VM commands:
    void writePush(const std::string& segment, int index);
    void writePop(const std::string& segment, int index);
    void writeArithmetic(const std::string& command);
    void writeLabel(const std::string& label);
    void writeGoto(const std::string& label);
    void writeIf(const std::string& label);
    void writeCall(const std::string& name, int nArgs);
    void writeFunction(const std::string& name, int nLocals);
    void writeReturn();

    // Get all generated VM instructions
    const std::vector<std::string>& getOutput() const;

    // Label index generators for if and while (ensures unique labels per function)
    int getNextIfLabelIndex();
    int getNextWhileLabelIndex();

    // Reset label counters (call at start of a new VM function)
    void resetLabelCounters();

    // Generate unique labels for complex control structures
    std::string generateUniqueLabel(const std::string& prefix);

    // Write comments for debugging (optional)
    void writeComment(const std::string& comment);

private:
    std::vector<std::string> outputLines;
    int ifLabelCounter;
    int whileLabelCounter;
};

} // namespace Command

#endif // CODEGENERATOR_H
