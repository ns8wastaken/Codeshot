#pragma once
#include "language.hpp"


namespace Languages
{
    inline const LanguageRules Cpp = LanguageRules{
        .keywords = { "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor", "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t", "class", "compl", "concept", "const", "consteval", "constexpr", "constinit", "const_cast", "continue", "co_await", "co_return", "co_yield", "decltype", "default", "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "final", "float", "for", "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected", "public", "register", "reinterpret_cast", "requires", "return", "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct", "switch", "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "override", "void", "volatile", "wchar_t", "while", "xor", "xor_eq", "main" },

        .operators = { "+", "-", "*", "/", "%", "=", "<", ">", "!", "&", "|", "^", "~", ">>", "<<", ".", ",", ":", ";", "<=", ">=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "~=", ">>=", "<<=" },

        .singleLineComments = { "//" },

        .multiLineComments = { { "/*", "*/" }, { "<", ">" } },

        .literals = {
            .numberDelimiters = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.' },
            .stringDelimiters = { '"', '\'' },
            .numberPrefixes   = { 'b', 'x' }
        },

        .identifier = {
            .startChars = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_' },

            .allowedChars = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' }
        },

        .brackets = { '(', ')',  '{', '}',  '[', ']' },

        .preprocessorDirectives = { "#include", "#define", "#if", "#else", "#endif", "#ifdef", "#ifndef", "#pragma" }
    };
}