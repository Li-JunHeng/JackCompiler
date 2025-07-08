#pragma once
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <unordered_map>

namespace Memory {

enum class Kind { STATIC, FIELD, ARG, VAR, NONE };

// Struct to hold information about each symbol
struct SymbolInfo {
    std::string type;
    Kind kind;
    int index;
};

class SymbolTable {
public:
    SymbolTable();
    // Resets the symbol table for starting a new class (clears all symbols)
    void startClass();
    // Resets the subroutine scope (clears subroutine symbols, retains class symbols)
    void startSubroutine();

    // Defines a new identifier with given name, type, and kind, and assigns it an index
    void define(const std::string& name, const std::string& type, Kind kind);

    // Returns the number of variables of the given kind defined so far in the current scope(s)
    int varCount(Kind kind) const;
    // Returns the kind of the named identifier in the current scope (NONE if not found)
    Kind kindOf(const std::string& name) const;
    // Returns the type of the named identifier
    std::string typeOf(const std::string& name) const;
    // Returns the index assigned to the named identifier
    int indexOf(const std::string& name) const;

private:
    std::unordered_map<std::string, SymbolInfo> classTable;      // holds class-scope symbols (statics, fields)
    std::unordered_map<std::string, SymbolInfo> subroutineTable; // holds subroutine-scope symbols (args, vars)
    int staticCount, fieldCount;
    int argCount, varCountSub;
};

} // namespace Memory

#endif // SYMBOLTABLE_H
