#include "InputHelper.h"

namespace StreamControl {

InputHelper::InputHelper(const std::string& filename)
    : fileStream_(filename), inputStream_(&fileStream_), ownsStream_(true) {
}

InputHelper::InputHelper(std::istream& stream)
    : inputStream_(&stream), ownsStream_(false) {
}

InputHelper::InputHelper(InputHelper& other)
    : inputStream_(other.inputStream_), ownsStream_(false) {}

std::istream* InputHelper::getActiveStream() {
    if(fileStream_.is_open()) return &fileStream_;
    if(inputStream_) return inputStream_;
    return nullptr;
}

bool InputHelper::isValid() const {
    return (fileStream_.is_open() && fileStream_.good()) || (inputStream_ != nullptr);
}

std::pair<std::string, int> InputHelper::ReadLine() {
    static int lineNumber = 1;
    std::string line;
    std::istream* active = getActiveStream();
    while(active && std::getline(*active, line)) {
        if(!line.empty()) {
            return { line, lineNumber++ };
        }
        // if line is empty, skip it but still increment line number
        lineNumber++;
    }
    return { "", -1 };
}

std::vector<std::pair<std::string, int>> InputHelper::ReadLines() {
    std::string line;
    std::vector<std::pair<std::string, int>> lines;
    int lineNumber = 0;
    std::istream* active = getActiveStream();
    while(active && std::getline(*active, line)) {
        lineNumber++;
        if(!line.empty()) {
            lines.emplace_back(line, lineNumber);
        }
    }
    return lines;
}

InputHelper::~InputHelper() {
    if(ownsStream_ && fileStream_.is_open()) {
        fileStream_.close();
    }
}

} // namespace StreamControl
