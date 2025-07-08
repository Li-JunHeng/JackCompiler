#include "SymbolTable.h"
#include <stdexcept>
#include "../Logger/Logger.h"

namespace Memory {

SymbolTable::SymbolTable() {
    // Initialize counts
    staticCount = fieldCount = 0;
    argCount = varCountSub = 0;
}

void SymbolTable::startClass() {
    classTable.clear();
    subroutineTable.clear();
    staticCount = fieldCount = 0;
    argCount = varCountSub = 0;
}

void SymbolTable::startSubroutine() {
    // Clear only subroutine scope symbols
    subroutineTable.clear();
    argCount = varCountSub = 0;
}

// Define a new symbol in the appropriate scope and assign index
void SymbolTable::define(const std::string& name, const std::string& type, Kind kind) {
    SymbolInfo info { type, kind, 0 };
    switch(kind) {
        case Kind::STATIC:
            info.index = staticCount++;
            classTable[name] = info;
            break;
        case Kind::FIELD:
            info.index = fieldCount++;
            classTable[name] = info;
            break;
        case Kind::ARG:
            info.index = argCount++;
            subroutineTable[name] = info;
            break;
        case Kind::VAR:
            info.index = varCountSub++;
            subroutineTable[name] = info;
            break;
        default:
            // Kind::NONE should not be defined
            Logger::ErrorLogger::cmdLogError(std::invalid_argument("Invalid symbol kind"), -1, name);
    }
}

int SymbolTable::varCount(Kind kind) const {
    switch(kind) {
        case Kind::STATIC: return staticCount;
        case Kind::FIELD:  return fieldCount;
        case Kind::ARG:    return argCount;
        case Kind::VAR:    return varCountSub;
        default:           return 0;
    }
}

Kind SymbolTable::kindOf(const std::string& name) const {
    // Check subroutine scope first, then class scope
    auto itSub = subroutineTable.find(name);
    if(itSub != subroutineTable.end()) {
        return itSub->second.kind;
    }
    auto itClass = classTable.find(name);
    if(itClass != classTable.end()) {
        return itClass->second.kind;
    }
    return Kind::NONE;
}

std::string SymbolTable::typeOf(const std::string& name) const {
    auto itSub = subroutineTable.find(name);
    if(itSub != subroutineTable.end()) {
        return itSub->second.type;
    }
    auto itClass = classTable.find(name);
    if(itClass != classTable.end()) {
        return itClass->second.type;
    }
    return "";
}

int SymbolTable::indexOf(const std::string& name) const {
    auto itSub = subroutineTable.find(name);
    if(itSub != subroutineTable.end()) {
        return itSub->second.index;
    }
    auto itClass = classTable.find(name);
    if(itClass != classTable.end()) {
        return itClass->second.index;
    }
    return -1;
}

} // namespace Memory
