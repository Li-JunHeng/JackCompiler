#include "ErrorLogger.h"

namespace Logger {

void ErrorLogger::cmdLogError(const std::exception &error, int line, const std::string& details) {
    std::cerr << "Error: " << error.what() << std::endl;
    if(line != -1) {
        std::cerr << "In Line " << line << std::endl;
    }
    std::cerr << details << std::endl
              << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
    std::cerr << "...From Command Parser..." << std::endl;
    throw error;
}

void ErrorLogger::transLogError(const std::exception &error, const std::string& details) {
    std::cerr << "Error: " << error.what() << std::endl;
    std::cerr << details << std::endl
              << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
    std::cerr << "...From Code Generator..." << std::endl;
    throw error;
}

void ErrorLogger::IOError(const std::exception &error, int line, const std::string &details) {
    std::cerr << "Error: " << error.what() << std::endl;
    if(line != -1) {
        std::cerr << "In Line " << line << std::endl;
    }
    std::cerr << details << std::endl
              << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
    std::cerr << "...From IO Helper..." << std::endl;
    throw error;
}

void ErrorLogger::usageError(const std::exception &error) {
    std::cerr << "Error: " << error.what() << std::endl;
    std::cerr << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
    std::cerr << "...From Usage Helper..." << std::endl;
    exit(-1);
}

} // namespace Logger
