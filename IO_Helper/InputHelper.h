#pragma once
#ifndef INPUTHELPER_H
#define INPUTHELPER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

namespace StreamControl {

class InputHelper {
    friend class Helper;
public:
    explicit InputHelper(const std::string& filename);
    explicit InputHelper(std::istream& stream);
    // Copy constructor: share stream pointer (does not own the stream)
    InputHelper(InputHelper& other);

    std::pair<std::string, int> ReadLine();
    std::vector<std::pair<std::string, int>> ReadLines();
    bool isValid() const;

    ~InputHelper();
private:
    std::istream* inputStream_;
    std::ifstream fileStream_;
    bool ownsStream_;
    std::istream* getActiveStream();
};

} // namespace StreamControl

#endif // INPUTHELPER_H
