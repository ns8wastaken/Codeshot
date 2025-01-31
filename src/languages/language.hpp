#pragma once
#include <string>
#include <unordered_set>
#include <utility>


namespace std
{
    template <>
    struct hash<std::pair<std::string_view, std::string_view>>
    {
        size_t operator()(const std::pair<std::string_view, std::string_view>& p) const
        {
            size_t h1 = std::hash<std::string_view>{}(p.first);
            size_t h2 = std::hash<std::string_view>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };
}


struct LanguageRules
{
    std::unordered_set<std::string_view> keywords;
    std::unordered_set<std::string_view> operators;
    std::unordered_set<std::string_view> singleLineComments;
    std::unordered_set<std::pair<std::string_view, std::string_view>> multiLineComments;

    struct Literals
    {
        std::unordered_set<char> numberDelimiters;  // Characters that can start a number ('.', '0'-'9', ...)
        std::unordered_set<char> stringDelimiters;  // Characters that can start a string ('"', '\'', ...)
        std::unordered_set<char> numberPrefixes;    // Prefixes for numbers of special bases (0b1001 or 0xABC) ('b', 'x', ...)
    } literals;

    struct Identifier
    {
        std::unordered_set<char> startChars;    // Characters that can start an identifier ('a'-'z', 'A'-'Z', '_')
        std::unordered_set<char> allowedChars;  // Characters allowed in identifiers ('a'-'z', 'A'-'Z', '0'-'9', '_')
    } identifier;

    std::unordered_set<char> brackets;

    std::unordered_set<std::string_view> preprocessorDirectives;


    inline bool hasKeyword(const std::string_view& keyword) const { return keywords.contains(keyword); }
    inline bool hasOperator(const std::string_view& op) const { return operators.contains(op); }
    inline bool hasSingleLineComment(const std::string_view& comment) const { return singleLineComments.contains(comment); }
    inline bool hasMultiLineComment(const std::string_view& first, const std::string_view& second) const { return multiLineComments.contains(std::pair(first, second)); }
    inline bool idCanStartWith(const char& chr) const { return identifier.startChars.contains(chr); }
    inline bool idCanContain(const char& chr) const { return identifier.allowedChars.contains(chr); }
    inline bool hasBracket(const char& bracket) const { return brackets.contains(bracket); }
    inline bool hasPreprocessorDirective(const std::string_view& dir) const { return preprocessorDirectives.contains(dir); }
};
