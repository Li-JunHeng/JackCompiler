#include "Tokenizer.h"
#include <cctype>
#include <stdexcept>
#include "../Logger/Logger.h"

using namespace std;

namespace Command {

Tokenizer::Tokenizer(const std::string& inputStr)
    : input(inputStr), pos(0), currentLine(1), currentTokenType(TokenType::NONE), currentToken(""), currentTokenLine(0) { }

bool Tokenizer::hasMoreTokens() {
    skipWhitespaceAndComments();
    return pos < input.size();
}

void Tokenizer::advance() {
    skipWhitespaceAndComments();
    currentToken.clear();
    currentTokenType = TokenType::NONE;
    currentTokenLine = currentLine;
    if(pos >= input.size()) {
        return;
    }
    char c = input[pos];
    if(isalpha(c) || c == '_') {
        while(pos < input.size() && (isalnum(input[pos]) || input[pos] == '_')) {
            currentToken += input[pos++];
        }
        if(isKeyword(currentToken)) {
            currentTokenType = TokenType::KEYWORD;
        } else {
            currentTokenType = TokenType::IDENTIFIER;
        }
    }
    else if(isdigit(c)) {
        while(pos < input.size() && isdigit(input[pos])) {
            currentToken += input[pos++];
        }
        currentTokenType = TokenType::INT_CONST;
    }
    else if(c == '\"') {
        pos++;
        while(pos < input.size() && input[pos] != '\"') {
            char sc = input[pos++];
            if(sc == '\n') {
                Logger::ErrorLogger::cmdLogError(invalid_argument("String constant not terminated before newline"), currentLine, currentToken);
            }
            currentToken += sc;
        }
        if(pos >= input.size()) {
            Logger::ErrorLogger::cmdLogError(invalid_argument("Unterminated string constant"), currentLine, currentToken);
        }
        pos++;
        currentTokenType = TokenType::STRING_CONST;
    }
    else if(symbols.find(c) != string::npos) {
        currentToken += c;
        currentTokenType = TokenType::SYMBOL;
        pos++;
    }
    else {
        Logger::ErrorLogger::cmdLogError(invalid_argument("Invalid character in input"), currentLine, string(1, c));
        pos++;
    }
}

TokenType Tokenizer::tokenType() const {
    return currentTokenType;
}

std::string Tokenizer::tokenValue() const {
    return currentToken;
}

int Tokenizer::tokenLine() const {
    return currentTokenLine;
}

void Tokenizer::skipWhitespaceAndComments() {
    while(pos < input.size()) {
        char c = input[pos];
        if(isspace(c)) {
            if(c == '\n') {
                currentLine++;
            }
            pos++;
            continue;
        }
        if(c == '/' && pos + 1 < input.size()) {
            char nextChar = input[pos+1];
            if(nextChar == '/') {
                pos += 2;
                while(pos < input.size() && input[pos] != '\n') {
                    pos++;
                }
                continue;
            } else if(nextChar == '*') {
                pos += 2;
                while(pos < input.size() && !(input[pos] == '*' && pos+1 < input.size() && input[pos+1] == '/')) {
                    if(input[pos] == '\n') {
                        currentLine++;
                    }
                    pos++;
                }
                if(pos < input.size()) {
                    pos += 2;
                }
                continue;
            }
        }
        break;
    }
}

bool Tokenizer::isKeyword(const std::string& str) const {
    for(const string& kw : keywords) {
        if(str == kw) return true;
    }
    return false;
}

} // namespace Command
