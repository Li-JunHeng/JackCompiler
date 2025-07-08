#include "OutputHelper.h"

namespace StreamControl {

OutputHelper::OutputHelper(const std::string& filename)
    : fileStream_(filename), outputStream_(&fileStream_), ownsStream_(true) {
}

OutputHelper::OutputHelper(std::ostream& stream)
    : outputStream_(&stream), ownsStream_(false) {
}

OutputHelper::OutputHelper(OutputHelper& other)
    : outputStream_(other.outputStream_), ownsStream_(false) { }

std::ostream* OutputHelper::getActiveStream() {
    if(fileStream_.is_open()) return &fileStream_;
    if(outputStream_) return outputStream_;
    return nullptr;
}

bool OutputHelper::isValid() const {
    return (fileStream_.is_open() && fileStream_.good()) || (outputStream_ != nullptr);
}

void OutputHelper::WriteLine(const std::string& line, bool noNewline) {
    std::ostream* active = getActiveStream();
    if(active) {
        if(noNewline) (*active) << line;
        else (*active) << line << std::endl;
    }
}

void OutputHelper::WriteLines(const std::vector<std::string>& lines) {
    if(lines.empty()) return;
    // Find index of last non-empty line, to avoid trailing newline at end of file
    int last_line = lines.size() - 1;
    for(int i = lines.size() - 1; i >= 0; --i) {
        if(!lines[i].empty()) {
            last_line = i;
            break;
        }
    }
    for(int i = 0; i < lines.size(); ++i) {
        if(lines[i].empty()) continue;
        if(i == last_line) WriteLine(lines[i], true);
        else WriteLine(lines[i]);
    }
}

OutputHelper::~OutputHelper() {
    if(ownsStream_ && fileStream_.is_open()) {
        fileStream_.close();
    }
}

} // namespace StreamControl
