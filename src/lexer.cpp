#include "lexer.hpp"


inline void Lexer::setSource(const std::string& source)
{
    m_source = source;
}


inline void Lexer::setLanguage(Language language)
{
    m_language = language;
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
    size_t start = pos;
    while (std::isalnum(m_source[pos]) || m_source[pos] == '_') {
        ++pos;
    }

    std::string value = m_source.substr(start, pos - start);

    if (isKeyword(value)) {
        return Token{ value, TokenType::Keyword, tokenColors[static_cast<int>(TokenType::Keyword)] };
    }
    else {
        return Token{ value, TokenType::Identifier, tokenColors[static_cast<int>(TokenType::Identifier)] };
    }
}


Lexer::Token Lexer::readNumber()
{
    size_t start = pos;

    while (std::isdigit(m_source[pos])) {
        ++pos;
    }

    if (m_source[pos] == '.') {
        ++pos;

        while (std::isdigit(m_source[pos])) {
            ++pos;
        }
    }

    return Token{ m_source.substr(start, pos - start), TokenType::Literal, tokenColors[static_cast<int>(TokenType::Literal)] }; // Number type
}


Lexer::Token Lexer::readString()
{
    char quoteType = m_source[pos++]; // Consume the opening quote
    size_t start   = pos;

    while (m_source[pos] != quoteType && m_source[pos] != '\0') {
        if (m_source[pos] == '\\') {
            ++pos; // Escape character
        }

        ++pos;
    }

    ++pos; // Consume the closing quote
    return Token{ m_source.substr(start - 1, pos - start + 1), TokenType::String, tokenColors[static_cast<int>(TokenType::String)] };
}


Lexer::Token Lexer::readComment()
{
    size_t start = pos;

    while (m_source[pos] != '\n' && m_source[pos] != '\0') {
        ++pos;
    }

    return Token{ m_source.substr(start, pos - start), TokenType::Comment, tokenColors[static_cast<int>(TokenType::Comment)] };
}


Lexer::Token Lexer::readOperator()
{
    std::string op(1, m_source[pos++]);

    while (!op.empty() && isOperator(op + m_source[pos + 1])) {
        op += m_source[pos++];
    }

    return Token{ op, TokenType::Operator, tokenColors[static_cast<int>(TokenType::Operator)] };
}


std::vector<Lexer::Token> Lexer::lex()
{
    std::vector<Token> tokens = {};

    while (m_source[pos] != '\0') {
        const char current = m_source[pos];

        if (std::isspace(current)) {
            ++pos; // Skip whitespace
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
            tokens.push_back(Token{ std::string(1, current), TokenType::Unknown, tokenColors[static_cast<int>(TokenType::Unknown)] });
            ++pos;
        }
    }

    return tokens;
}


void Lexer::render(const Font& font, const std::vector<Token>& tokens)
{
    float charWidth = MeasureTextEx(font, "A", font.baseSize, 0).x;

    float x = 0.0f;
    float y = 0.0f;

    size_t src_i   = 0;
    size_t token_i = 0;
    while (src_i < m_source.size()) {
        if (std::isspace(m_source[src_i])) {
            switch (m_source[src_i]) {
                case '\n': {
                    x = 0.0f;
                    y += font.baseSize;
                } break;

                case ' ': {
                    x += charWidth;
                } break;

                case '\t': {
                    x += charWidth * 4;
                } break;
            }

            ++src_i;
            continue;
        }

        const Lexer::Token& token = tokens[token_i];

        DrawTextEx(font, token.value.c_str(), Vector2{ x, y }, font.baseSize, 0, token.color);
        x += MeasureTextEx(font, token.value.c_str(), font.baseSize, 0).x;

        src_i += token.value.size();
        ++token_i;
    }
}
