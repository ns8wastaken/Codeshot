#pragma once
#include <raylib.h>
#include <string>
#include <unordered_set>
#include <vector>


class Lexer
{
private:
    enum class TokenType
    {
        Keyword,
        Identifier,
        Literal,
        Operator,
        Comment,
        String,
        Unknown
    };

    struct LanguageConfig
    {
        std::unordered_set<std::string> keywords;             // if, return, while
        std::unordered_set<std::string> operators;            // +, -, +=
        std::pair<std::string, std::string> multiLineComment; // "/*", "*/"
        std::unordered_set<char> stringDelimiters;            // '"', '\''
    };

public:
    enum class Language
    {
        Python,
        Cpp,
        Javascript
    };

    struct Token
    {
        std::string value;
        TokenType type;
        Color color;
    };

    void setSource(const std::string& sourceCode);
    void setLanguage(Language language);

    std::vector<Token> lex();
    void render(const Font& font, const std::vector<Token>& tokens);

private:
    std::string m_source;
    Language m_language = Language::Python;
    size_t pos          = 0;

    Color tokenColors[8] = {
        Color{ 0,   0,   255, 255 }, // Blue
        Color{ 255, 255, 255, 255 }, // White
        Color{ 0,   255, 0,   255 }, // Green
        Color{ 255, 255, 0,   255 }, // Yellow
        Color{ 128, 128, 128, 255 }, // Gray
        Color{ 255, 0,   0,   255 }, // Red
        Color{ 0,   0,   0,   0   }  // Blank
    };

    std::unordered_set<std::string> m_keywords[1] = {
        { "and", "as", "assert", "async", "continue", "else", "if", "not", "while", "def", "except", "import", "or", "with", "del", "finally", "in", "pass", "yield", "elif", "for", "is", "raise", "await", "False", "from", "lambda", "return", "break", "none", "global", "nonlocal", "try", "class", "True" }
    };

    std::unordered_set<std::string> m_operators[1] = {
        { "+", "-", "*", "/", "%", "=", "<", ">", "!", "&", "|", "^", "~", ">>", "<<", ".", ",", ":", ";", ":=", "<=", ">=", "[", "]", "{", "}", "(", ")", "+=", "-=", "*=", "/=", "//=", "**=", "%=", "&=", "|=", "^=", "~=", ">>=", "<<=" }
    };

    const std::unordered_set<std::string>& getKeywords() const;
    const std::unordered_set<std::string>& getOperators() const;

    bool isKeyword(const std::string& value) const;
    bool isOperator(const std::string& str) const;

    Token readIdentifierOrKeyword();
    Token readNumber();
    Token readString();
    Token readComment();
    Token readOperator();
};
