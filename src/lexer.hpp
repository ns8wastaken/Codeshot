#pragma once
#include <raylib.h>
#include <string>
#include <unordered_set>
#include <vector>
#include <math.h>
#include <cstring>


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
        Space,
        Tab,
        Newline,
        Unknown
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
        std::string_view value;
        TokenType type;
        Color color;
    };

    void setSource(const std::string sourceCode);
    void setLanguage(Language language);

    std::vector<Token> lex();
    void render(const Font& font, const std::vector<Token>& tokens);

private:
    std::string m_source;
    size_t m_lineCount       = 0;
    size_t m_lineCountDigits = 0;
    Language m_language      = Language::Python;
    size_t m_pos             = 0;

    Color tokenColors[8] = {
        Color{ 0,   0,   255, 255 }, // Blue
        Color{ 255, 255, 255, 255 }, // White
        Color{ 0,   255, 0,   255 }, // Green
        Color{ 255, 255, 0,   255 }, // Yellow
        Color{ 128, 128, 128, 255 }, // Gray
        Color{ 255, 0,   0,   255 }, // Red
        Color{ 0,   0,   0,   0   }  // Blank
    };

    Color getTokenColor(TokenType tokenType);

    const std::unordered_set<std::string_view> m_keywords[1] = {
        // Python
        { "and", "as", "assert", "async", "continue", "else", "if", "not", "while", "def", "except", "import", "or", "with", "del", "finally", "in", "pass", "yield", "elif", "for", "is", "raise", "await", "False", "from", "lambda", "return", "break", "none", "global", "nonlocal", "try", "class", "True" }
    };

    const std::unordered_set<std::string_view> m_operators[1] = {
        // Python
        { "+", "-", "*", "/", "%", "=", "<", ">", "!", "&", "|", "^", "~", ">>", "<<", ".", ",", ":", ";", ":=", "<=", ">=", "[", "]", "{", "}", "(", ")", "+=", "-=", "*=", "/=", "//=", "**=", "%=", "&=", "|=", "^=", "~=", ">>=", "<<=" }
    };

    const std::unordered_set<std::string_view>& getKeywords() const;
    const std::unordered_set<std::string_view>& getOperators() const;

    bool isKeyword(const std::string_view& value) const;
    bool isOperator(const std::string_view& value) const;

    Token readIdentifierOrKeyword();
    Token readNumber();
    Token readString();
    Token readComment();
    Token readOperator();
    Token readSpaces();
    Token readTabs();
};
