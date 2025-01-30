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


inline const std::unordered_set<std::string_view>& Lexer::getComments() const
{
    return m_comments[static_cast<size_t>(m_language)];
}


inline bool Lexer::isKeyword(const std::string_view& value) const
{
    return getKeywords().contains(value);
}


inline bool Lexer::isOperator()
{
    size_t start = m_pos;
    m_pos += 3;
    std::string_view op(m_source.c_str() + start, m_pos - start);

    while (!op.empty() && !getOperators().contains(op)) {
        --m_pos;
        op = std::string_view(m_source.c_str() + start, m_pos - start);
    }
    m_pos = start;

    if (op.empty())
        return false;

    return true;
}


inline bool Lexer::isComment()
{
    size_t start = m_pos;
    m_pos += 3;
    std::string_view com(m_source.c_str() + start, m_pos - start);

    while (!com.empty() && !getComments().contains(com)) {
        --m_pos;
        com = std::string_view(m_source.c_str() + start, m_pos - start);
    }
    m_pos = start;

    if (com.empty())
        return false;

    return true;
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

    return Token{ std::string_view(m_source.c_str() + start, m_pos - start), TokenType::String, tokenColors[static_cast<int>(TokenType::String)] };
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
    size_t start = m_pos;
    m_pos += 3;
    std::string_view op(m_source.c_str() + start, m_pos - start);

    while (!getOperators().contains(op)) {
        --m_pos;
        op = std::string_view(m_source.c_str() + start, m_pos - start);
    }

    return Token{ op, TokenType::Operator, tokenColors[static_cast<int>(TokenType::Operator)] };
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

    return Token{ std::string_view(m_source.c_str() + start, m_pos - start), TokenType::Tab, getTokenColor(TokenType::Tab) };
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

        else if (isComment()) {
            tokens.push_back(readComment());
        }

        else if (isOperator()) {
            tokens.push_back(readOperator());
        }

        else {
            tokens.push_back(Token{ std::string_view(m_source.c_str() + m_pos, 1), TokenType::Unknown, getTokenColor(TokenType::Unknown) });
            ++m_pos;
        }
    }

    return tokens;
}


void Lexer::render(const Vector2& offset, const Font& font, const std::vector<Token>& tokens)
{
    Vector2 spaceSize = MeasureTextEx(font, " ", font.baseSize, 0);

    float xStart = (m_lineCountDigits + 1) * spaceSize.x + offset.x;

    float x = xStart;
    float y = offset.y;

    size_t token_i    = 0;
    size_t lineNumber = 1;

    // Draw line number
    DrawTextEx(font, TextFormat("%lld", lineNumber), Vector2{ offset.x, y }, font.baseSize, 0, WHITE);

    char textBuffer[256]; // because std::string_view is stupid and isnt null terminated

    for (const Token& token : tokens) {
        if (token.type == TokenType::Newline) {
            x = xStart;
            y += spaceSize.y;
            ++lineNumber;

            // Draw line number
            DrawTextEx(font, TextFormat("%lld", lineNumber), Vector2{ offset.x, y }, font.baseSize, 0, WHITE);
        }

        // Put the string into textBuffer
        if (token.value.size() < sizeof(textBuffer)) {
            std::memcpy(textBuffer, token.value.data(), token.value.size());
            textBuffer[token.value.size()] = '\0';
        }
        else {
            std::memcpy(textBuffer, token.value.data(), sizeof(textBuffer) - 1);
            textBuffer[sizeof(textBuffer) - 1] = '\0';
        }

        // Draw token
        DrawTextEx(font, textBuffer, Vector2{ x, y }, font.baseSize, 0, token.color);

        // Draw whitespace
        if (token.type == TokenType::Space) {
            for (size_t i = 0; i < token.value.size(); ++i)
                DrawCircleV(Vector2{ x + i * spaceSize.x + spaceSize.x / 2.0f, y + spaceSize.y / 2.0f }, static_cast<float>(font.baseSize) / 10.0f, Color{ 255, 255, 255, 40 });
        }
        else if (token.type == TokenType::Tab) {
            DrawLineV(
                Vector2{ x + spaceSize.x / 2.0f, y + spaceSize.y / 2.0f },
                Vector2{ x + spaceSize.x / 2.0f + spaceSize.x * token.value.size() * 3.0f + (token.value.size() - 1) * spaceSize.x, y + spaceSize.y / 2.0f },
                Color{ 255, 255, 255, 40 }
            );
        }

        x += token.type == TokenType::Tab ? spaceSize.x * 4 * token.value.size() : MeasureTextEx(font, textBuffer, font.baseSize, 0).x;

        ++token_i;
    }
}
