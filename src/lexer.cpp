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
    m_language      = language;
    m_languageRules = m_languages[static_cast<size_t>(language)];
}


inline void Lexer::setColor(TokenType tokenType, Color color)
{
    m_tokenColors[static_cast<size_t>(tokenType)] = color;
}


// inline void Lexer::setBackgroundColor(Color color)
// {
//     m_backgroundColor = color;
// }


inline Color Lexer::getTokenColor(TokenType tokenType)
{
    return m_tokenColors[static_cast<size_t>(tokenType)];
}


inline bool Lexer::isOperator()
{
    size_t start = m_pos;
    m_pos += 3;
    std::string_view op(m_source.c_str() + start, m_pos - start);

    while (!op.empty() && !m_languageRules->hasOperator(op)) {
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
    m_pos += 2;
    std::string_view com(m_source.c_str() + start, m_pos - start);

    // TODO: Add support for multiline comments
    while (!com.empty() && !m_languageRules->hasSingleLineComment(com)) {
        --m_pos;
        com = std::string_view(m_source.c_str() + start, m_pos - start);
    }
    m_pos = start;

    if (com.empty())
        return false;

    return true;
}


Token Lexer::readIdentifierOrKeyword()
{
    size_t start = m_pos;
    while (std::isalnum(m_source[m_pos]) || m_source[m_pos] == '_') {
        ++m_pos;
    }

    std::string_view value(m_source.c_str() + start, m_pos - start);

    if (m_languageRules->hasKeyword(value)) {
        return Token{ value, TokenType::Keyword, m_tokenColors[static_cast<int>(TokenType::Keyword)] };
    }
    else {
        return Token{ value, TokenType::Identifier, m_tokenColors[static_cast<int>(TokenType::Identifier)] };
    }
}


Token Lexer::readLiteral()
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

    return Token{ std::string_view(m_source.c_str() + start, m_pos - start), TokenType::Literal, m_tokenColors[static_cast<int>(TokenType::Literal)] };
}


Token Lexer::readString()
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

    return Token{ std::string_view(m_source.c_str() + start, m_pos - start), TokenType::String, m_tokenColors[static_cast<int>(TokenType::String)] };
}


Token Lexer::readComment()
{
    size_t start = m_pos;

    while (m_source[m_pos] != '\n' && m_source[m_pos] != '\0') {
        ++m_pos;
    }

    return Token{ std::string_view(m_source.c_str() + start, m_pos - start), TokenType::CommentSingleLine, m_tokenColors[static_cast<int>(TokenType::CommentSingleLine)] };
}


Token Lexer::readOperator()
{
    size_t start = m_pos;
    m_pos += 3;
    std::string_view op(m_source.c_str() + start, m_pos - start);

    while (!m_languageRules->hasOperator(op)) {
        --m_pos;
        op = std::string_view(m_source.c_str() + start, m_pos - start);
    }

    return Token{ op, TokenType::Operator, m_tokenColors[static_cast<int>(TokenType::Operator)] };
}


Token Lexer::readSpaces()
{
    size_t start = m_pos;

    while (m_source[m_pos] == ' ') {
        ++m_pos;
    }

    return Token{ std::string_view(m_source.c_str() + start, m_pos - start), TokenType::Space, getTokenColor(TokenType::Space) };
}


Token Lexer::readTabs()
{
    size_t start = m_pos;

    while (m_source[m_pos] == '\t') {
        ++m_pos;
    }

    return Token{ std::string_view(m_source.c_str() + start, m_pos - start), TokenType::Tab, getTokenColor(TokenType::Tab) };
}


std::vector<Token> Lexer::lex()
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
            tokens.push_back(readLiteral());
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

        else if (m_languageRules->hasBracket(current)) {
            tokens.push_back(Token{ std::string_view(m_source.c_str() + m_pos, 1), TokenType::Bracket, getTokenColor(TokenType::Bracket) });
            ++m_pos;
        }

        else {
            tokens.push_back(Token{ std::string_view(m_source.c_str() + m_pos, 1), TokenType::Unknown, getTokenColor(TokenType::Unknown) });
            ++m_pos;
        }
    }

    return tokens;
}


void Lexer::render(const Vector2& windowSize, const Font& font, const std::vector<Token>& tokens)
{
    Vector2 spaceSize = MeasureTextEx(font, " ", font.baseSize, 0);

    float xStart = (m_lineCountDigits + 1) * spaceSize.x;

    float x = xStart;
    float y = 0;

    size_t token_i    = 0;
    size_t lineNumber = 1;

    // Draw line number
    DrawTextEx(font, TextFormat("%lld", lineNumber), Vector2{ 0, y }, font.baseSize, 0, WHITE);

    char textBuffer[256]; // because std::string_view is stupid and isnt null terminated

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (x > windowSize.x) {
            if (tokens[i].type != TokenType::Newline) {
                continue;
            }

            x = 0.0f;
        }
        if (y > windowSize.y) break;

        const Token& token = tokens[i];

        if (token.type == TokenType::Newline) {
            x = xStart;
            y += spaceSize.y;
            ++lineNumber;

            // Draw line number
            DrawTextEx(font, TextFormat("%lld", lineNumber), Vector2{ 0, y }, font.baseSize, 0, WHITE);
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
        // if (token.type == TokenType::Space) {
        //     for (size_t i = 0; i < token.value.size(); ++i)
        //         DrawCircleV(Vector2{ x + i * spaceSize.x + spaceSize.x / 2.0f, y + spaceSize.y / 2.0f }, static_cast<float>(font.baseSize) / 10.0f, Color{ 255, 255, 255, 40 });
        // }
        // else if (token.type == TokenType::Tab) {
        //     DrawLineV(
        //         Vector2{ x + spaceSize.x / 2.0f, y + spaceSize.y / 2.0f },
        //         Vector2{ x + spaceSize.x / 2.0f + spaceSize.x * token.value.size() * 3.0f + (token.value.size() - 1) * spaceSize.x, y + spaceSize.y / 2.0f },
        //         Color{ 255, 255, 255, 40 }
        //     );
        // }

        x += token.type == TokenType::Tab
               ? spaceSize.x * 4 * token.value.size()
               : MeasureTextEx(font, textBuffer, font.baseSize, 0).x;

        ++token_i;
    }
}
