#pragma once
#ifndef USAGE_H
#define USAGE_H

#include <string>
#include <string_view>
#include <getopt.h>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include "../Logger/Logger.h"

namespace Usage_Helper {

class Usage {
public:
    Usage(int argc, char* argv[]);

    const std::string& getInputPath() const noexcept;
    const std::string& getOutputPath() const noexcept;
    bool isHelp()    const noexcept;
    bool isVersion() const noexcept;

    static void printHelp(std::string_view program_name = "JackCompiler");
    static void printVersion();

private:
    void parse(int argc, char* argv[]);
    static std::string defaultOutputPath(const std::string& input);

    std::string m_input_path;
    std::string m_output_path;
    bool        m_help    = false;
    bool        m_version = false;
};

} // namespace Usage_Helper

#endif // USAGE_H
