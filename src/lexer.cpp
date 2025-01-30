#include "lexer.hpp"


inline void Lexer::setSource(const std::string& source)
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


inline const std::unordered_set<std::string>& Lexer::getKeywords() const
{
    return m_keywords[static_cast<size_t>(m_language)];
}


inline const std::unordered_set<std::string>& Lexer::getOperators() const
{
    return m_operators[static_cast<size_t>(m_language)];
}


inline bool Lexer::isKeyword(const std::string& value) const
{
    return getKeywords().contains(value);
}


inline bool Lexer::isOperator(const std::string& str) const
{
    return getOperators().contains(str);
}


Lexer::Token Lexer::readIdentifierOrKeyword()
{
    size_t start = m_pos;
    while (std::isalnum(m_source[m_pos]) || m_source[m_pos] == '_') {
        ++m_pos;
    }

    std::string value = m_source.substr(start, m_pos - start);

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

    return Token{ m_source.substr(start, m_pos - start), TokenType::Literal, tokenColors[static_cast<int>(TokenType::Literal)] }; // Number type
}


Lexer::Token Lexer::readString()
{
    char quoteType = m_source[m_pos++];
    size_t start   = m_pos;

    while (m_source[m_pos] != quoteType && m_source[m_pos] != '\0') {
        if (m_source[m_pos] == '\\') {
            ++m_pos; // Escape character
        }

        ++m_pos;
    }

    ++m_pos; // Include the closing quote
    return Token{ m_source.substr(start - 1, m_pos - start + 1), TokenType::String, tokenColors[static_cast<int>(TokenType::String)] };
}


Lexer::Token Lexer::readComment()
{
    size_t start = m_pos;

    while (m_source[m_pos] != '\n' && m_source[m_pos] != '\0') {
        ++m_pos;
    }

    return Token{ m_source.substr(start, m_pos - start), TokenType::Comment, tokenColors[static_cast<int>(TokenType::Comment)] };
}


Lexer::Token Lexer::readOperator()
{
    std::string op(1, m_source[m_pos++]);

    while (!op.empty() && isOperator(op + m_source[m_pos + 1])) {
        op += m_source[m_pos++];
    }

    return Token{ op, TokenType::Operator, tokenColors[static_cast<int>(TokenType::Operator)] };
}


std::vector<Lexer::Token> Lexer::lex()
{
    std::vector<Token> tokens = {};

    while (m_source[m_pos] != '\0') {
        const char current = m_source[m_pos];

        if (std::isspace(current)) {
            if (current == '\n')
                tokens.push_back(Token{ "\n", TokenType::Newline, getTokenColor(TokenType::Newline) });

            else if (current == ' ')
                tokens.push_back(Token{ " ", TokenType::Space, getTokenColor(TokenType::Space) });

            else if (current == '\t')
                tokens.push_back(Token{ "\t", TokenType::Tab, getTokenColor(TokenType::Tab) });

            ++m_pos; // Skip whitespace
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

        else if (isOperator(std::string(1, current))) {
            tokens.push_back(readOperator());
        }

        else {
            tokens.push_back(Token{ std::string(1, current), TokenType::Unknown, getTokenColor(TokenType::Unknown) });
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

    DrawLineV(Vector2{ (m_lineCountDigits * spaceSize.x) + (spaceSize.x / 2.0f), 0 }, Vector2{ (m_lineCountDigits * spaceSize.x) + (spaceSize.x / 2.0f), m_lineCount * spaceSize.y }, WHITE);

    for (const Token& token : tokens) {
        if (token.type == TokenType::Newline) {
            x = xStart;
            y += spaceSize.y;
            ++lineNumber;

            // Draw line number
            DrawTextEx(font, TextFormat("%lld", lineNumber), Vector2{ 0, y }, font.baseSize, 0, WHITE);
        }

        DrawTextEx(font, token.value.c_str(), Vector2{ x, y }, font.baseSize, 0, token.color);
        x += MeasureTextEx(font, token.value.c_str(), font.baseSize, 0).x;

        ++token_i;
    }
}
