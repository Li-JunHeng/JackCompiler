#pragma once
#ifndef OUTPUTHELPER_H
#define OUTPUTHELPER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

namespace StreamControl {

class OutputHelper {
    friend class Helper;
public:
    explicit OutputHelper(const std::string& filename);
    explicit OutputHelper(std::ostream& stream);
    OutputHelper(OutputHelper& other);

    void WriteLine(const std::string& line, bool noNewline = false);
    void WriteLines(const std::vector<std::string>& lines);

    bool isValid() const;

    ~OutputHelper();
private:
    std::ostream* outputStream_;
    std::ofstream fileStream_;
    bool ownsStream_;
    std::ostream* getActiveStream();
};

} // namespace StreamControl

#endif // OUTPUTHELPER_H
