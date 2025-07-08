#pragma once
#ifndef ERRORLOGGER_H
#define ERRORLOGGER_H

#include <iostream>
#include <string>
#include <exception>

namespace Logger {

class ErrorLogger {
public:
    static void cmdLogError(const std::exception &error, int line, const std::string& details);
    static void transLogError(const std::exception &error, const std::string& details);
    static void IOError(const std::exception &error, int line = -1, const std::string& details = "");
    static void usageError(const std::exception &error);
};

} // namespace Logger

#endif // ERRORLOGGER_H
