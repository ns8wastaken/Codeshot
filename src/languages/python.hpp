#pragma once
#include "language.hpp"


namespace Languages
{
    const LanguageRules Python = LanguageRules{
        .keywords = { "and", "as", "assert", "async", "continue", "else", "if", "not", "while", "def", "except", "import", "or", "with", "del", "finally", "in", "pass", "yield", "elif", "for", "is", "raise", "await", "False", "from", "lambda", "return", "break", "none", "global", "nonlocal", "try", "class", "True" },

        .operators = { "+", "-", "*", "/", "//", "%", "=", "<", ">", "!", "&", "|", "^", "~", ">>", "<<", ".", ",", ":", ";", ":=", "<=", ">=", "[", "]", "{", "}", "(", ")", "+=", "-=", "*=", "/=", "//=", "**=", "%=", "&=", "|=", "^=", "~=", ">>=", "<<=" },

        .singleLineComments = { "#" },

        .multiLineComments = { { "\"\"\"", "\"\"\"" } },

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

        .preprocessorDirectives = {}
    };
}
