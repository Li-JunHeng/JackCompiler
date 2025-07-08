#pragma once
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

namespace Command {

enum class TokenType { KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST, NONE };

class Tokenizer {
public:
    // Construct a tokenizer for the given input string (Jack source code)
    Tokenizer(const std::string& input);

    // Are there more tokens in the input?
    bool hasMoreTokens();
    // Get the next token from the input and make it the current token
    void advance();

    // Accessors for the current token's type and value
    TokenType tokenType() const;
    std::string tokenValue() const;
    int tokenLine() const; // line number of current token (for error reporting)

private:
    // Helper: skip whitespace and comments to find the next token
    void skipWhitespaceAndComments();
    // Helper: check if a string is a Jack keyword
    bool isKeyword(const std::string& str) const;

    std::string input;   // the full input string
    size_t pos;          // current position in input
    int currentLine;     // current line number in input
    TokenType currentTokenType;
    std::string currentToken;
    int currentTokenLine;

    // Sets of Jack keywords and symbols for quick lookup
    const std::vector<std::string> keywords = {
        "class", "constructor", "function", "method", "field", "static", "var",
        "int", "char", "boolean", "void", "true", "false", "null", "this",
        "let", "do", "if", "else", "while", "return"
    };
    const std::string symbols = "{}()[].,;+-*/&|<>=~";
};

} // namespace Command

#endif // TOKENIZER_H
