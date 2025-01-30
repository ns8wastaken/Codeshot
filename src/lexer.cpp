#include "lexer.hpp"


inline void Lexer::setSource(const std::string source)
{
    m_source = source;

    m_lineCount = 1;
    for (const char& c : source) {
        if (c == '\n')
            ++m_lineCount;
    }

    m_lineCountDigits = static_cast<size_t>(std::ceil(std::log10(m_lineCount)));
}


inline void Lexer::setLanguage(Language language)
{
    m_language = language;
}


inline Color Lexer::getTokenColor(TokenType tokenType)
{
    return tokenColors[static_cast<int>(tokenType)];
}


inline const std::unordered_set<std::string_view>& Lexer::getKeywords() const
{
    return m_keywords[static_cast<size_t>(m_language)];
}


inline const std::unordered_set<std::string_view>& Lexer::getOperators() const
{
    return m_operators[static_cast<size_t>(m_language)];
}


inline bool Lexer::isKeyword(const std::string_view& value) const
{
    return getKeywords().contains(value);
}


inline bool Lexer::isOperator(const std::string_view& str) const
{
    return getOperators().contains(str);
}


Lexer::Token Lexer::readIdentifierOrKeyword()
{
    size_t start = m_pos;
    while (std::isalnum(m_source[m_pos]) || m_source[m_pos] == '_') {
        ++m_pos;
    }

    std::string_view value(m_source.c_str() + start, m_pos - start);

    if (isKeyword(value)) {
        return Token{ value, TokenType::Keyword, tokenColors[static_cast<int>(TokenType::Keyword)] };
    }
    else {
        return Token{ value, TokenType::Identifier, tokenColors[static_cast<int>(TokenType::Identifier)] };
    }
}


Lexer::Token Lexer::readNumber()
{
    size_t start = m_pos;

    while (std::isdigit(m_source[m_pos])) {
        ++m_pos;
    }

    if (m_source[m_pos] == '.') {
        ++m_pos;

        while (std::isdigit(m_source[m_pos])) {
            ++m_pos;
        }
    }

    return Token{ std::string_view(m_source.c_str() + start, m_pos - start), TokenType::Literal, tokenColors[static_cast<int>(TokenType::Literal)] };
}


Lexer::Token Lexer::readString()
{
    size_t start   = m_pos;
    char quoteType = m_source[m_pos++];

    while (m_source[m_pos] != quoteType && m_source[m_pos] != '\0') {
        if (m_source[m_pos] == '\\') {
            ++m_pos; // Escape character
        }

        ++m_pos;
    }

    ++m_pos; // Include the closing quote
    return Token{ std::string_view(m_source.c_str() + start, m_pos - start + 1), TokenType::String, tokenColors[static_cast<int>(TokenType::String)] };
}


Lexer::Token Lexer::readComment()
{
    size_t start = m_pos;

    while (m_source[m_pos] != '\n' && m_source[m_pos] != '\0') {
        ++m_pos;
    }

    return Token{ std::string_view(m_source.c_str() + start, m_pos - start), TokenType::Comment, tokenColors[static_cast<int>(TokenType::Comment)] };
}


Lexer::Token Lexer::readOperator()
{
    // size_t start = m_pos;
    // std::string_view op(m_source.c_str() + start, (++m_pos) - start);

    // while (!isOperator(op)) {
    //     op = std::string_view(m_source.c_str() + start, (++m_pos) - start);
    // }

    return Token{ std::string_view(m_source.c_str() + (m_pos++), 1), TokenType::Operator, tokenColors[static_cast<int>(TokenType::Operator)] };
}


Lexer::Token Lexer::readSpaces()
{
    size_t start = m_pos;

    while (m_source[m_pos] == ' ') {
        ++m_pos;
    }

    return Token{ std::string_view(m_source.c_str() + start, m_pos - start), TokenType::Space, getTokenColor(TokenType::Space) };
}


Lexer::Token Lexer::readTabs()
{
    size_t start = m_pos;

    while (m_source[m_pos] == '\t') {
        ++m_pos;
    }

    return Token{ std::string_view(m_source.c_str() + start, m_pos - start), TokenType::Space, getTokenColor(TokenType::Space) };
}


std::vector<Lexer::Token> Lexer::lex()
{
    std::vector<Token> tokens = {};

    while (m_source[m_pos] != '\0') {
        const char current = m_source[m_pos];

        if (std::isspace(current)) {
            switch (current) {
                case '\n': {
                    tokens.push_back(Token{ "\n", TokenType::Newline, getTokenColor(TokenType::Newline) });
                    ++m_pos;
                } break;

                case ' ': {
                    tokens.push_back(readSpaces());
                } break;

                case '\t': {
                    tokens.push_back(readTabs());
                } break;
            }
        }

        else if (std::isalpha(current) || current == '_') {
            tokens.push_back(readIdentifierOrKeyword());
        }

        else if (std::isdigit(current)) {
            tokens.push_back(readNumber());
        }

        else if (current == '"' || current == '\'') {
            tokens.push_back(readString());
        }

        else if (current == '#') {
            tokens.push_back(readComment());
        }

        else if (isOperator(std::string_view(m_source.c_str() + m_pos, 1))) {
            tokens.push_back(readOperator());
        }

        else {
            tokens.push_back(Token{ std::string_view(m_source.c_str() + m_pos, 1), TokenType::Unknown, getTokenColor(TokenType::Unknown) });
            ++m_pos;
        }
    }

    return tokens;
}


void Lexer::render(const Font& font, const std::vector<Token>& tokens)
{
    Vector2 spaceSize = MeasureTextEx(font, " ", font.baseSize, 0);

    float xStart = (m_lineCountDigits + 1) * spaceSize.x;

    float x = xStart;
    float y = 0.0f;

    size_t token_i    = 0;
    size_t lineNumber = 1;

    // Draw line number
    DrawTextEx(font, TextFormat("%lld", lineNumber), Vector2{ 0, y }, font.baseSize, 0, WHITE);

    char buffer[256]; // because std::string_view is stupid and isnt null terminated

    for (const Token& token : tokens) {
        if (token.type == TokenType::Newline) {
            x = xStart;
            y += spaceSize.y;
            ++lineNumber;

            // Draw line number
            DrawTextEx(font, TextFormat("%lld", lineNumber), Vector2{ 0, y }, font.baseSize, 0, WHITE);
        }

        if (token.value.size() < sizeof(buffer)) {
            std::memcpy(buffer, token.value.data(), token.value.size());
            buffer[token.value.size()] = '\0';
        }
        else {
            std::memcpy(buffer, token.value.data(), sizeof(buffer) - 1);
            buffer[sizeof(buffer) - 1] = '\0';
        }

        DrawTextEx(font, buffer, Vector2{ x, y }, font.baseSize, 0, token.color);
        x += MeasureTextEx(font, buffer, font.baseSize, 0).x;

        ++token_i;
    }
}
