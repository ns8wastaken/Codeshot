#pragma once
#include <raylib.h>
#include <string>
#include <unordered_set>
#include <vector>
#include <math.h>
#include <cstring>
#include <array>

#include "token.hpp"

#include "languages/python.hpp"
#include "languages/cpp.hpp"


class Lexer
{
public:
    enum class Language
    {
        Python,
        Cpp,
        Javascript
    };

    inline void setSource(const std::string sourceCode);
    inline void setLanguage(Language language);

    inline void setColor(TokenType tokenType, Color color);
    // inline void setBackgroundColor(Color color);

    std::vector<Token> lex();
    void render(const Vector2& windowSize, const Font& font, const std::vector<Token>& tokens);

private:
    std::string m_source;
    size_t m_pos = 0;

    size_t m_lineCount       = 0;
    size_t m_lineCountDigits = 0;
    Language m_language      = Language::Python;
    const LanguageRules* m_languageRules;

    Color m_tokenColors[static_cast<size_t>(TokenType::Unknown)];
    // Color m_backgroundColor;

    const LanguageRules* m_languages[2] = {
        &Languages::Python,
        &Languages::Cpp
    };

    Color getTokenColor(TokenType tokenType);

    bool isOperator();
    bool isComment();

    Token readIdentifierOrKeyword();
    Token readLiteral();
    Token readString();
    Token readComment();
    Token readOperator();
    Token readSpaces();
    Token readTabs();
};
