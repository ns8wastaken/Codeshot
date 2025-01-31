#pragma once
#include <string>
#include <raylib.h>


enum class TokenType
{
    Keyword,
    Identifier,
    Literal,
    Operator,
    Bracket,
    CommentSingleLine,
    CommentMultiLine,
    String,
    Space,
    Tab,
    Newline,
    Unknown
};


struct Token
{
    std::string_view value;
    TokenType type;
    Color color;
};
