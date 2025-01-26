#include <raylib.h>
#include <iostream>

#include "lexer.cpp"


constexpr const char* fontPath = "src/fonts/JetBrains-Mono.ttf";


int main()
{
    InitWindow(800, 800, "Raylib Default Window");

    std::string sourceCode = R"(def sieve(n: int):
    if n < 2:
        return None

    l = list(range(2, n+1))

    index = 2
    while index < l[-1]:
        for i in l:
            if i % index == 0 and i > index:
                l.remove(i)
        index += 1

    return l

print(sieve(200))
print(f"chicken {sieve(20)}"))";

    Lexer lexer;

    lexer.setSource(sourceCode);
    lexer.setLanguage(Lexer::Language::Python);

    std::vector<Lexer::Token> tokens = lexer.lex();

    // for (const Lexer::Token& token : tokens) {
    //     std::cout << token.value << " : " << static_cast<int>(token.color.r) << " " << static_cast<int>(token.color.g) << " " << static_cast<int>(token.color.b) << "\n";
    // }

    int fontSize = 25;
    Font font    = LoadFontEx(fontPath, fontSize, 0, 0);

    while (!WindowShouldClose()) {
        float mouseWheelMove = GetMouseWheelMove();

        if (mouseWheelMove) {
            fontSize += static_cast<int>(mouseWheelMove) * 2;
            UnloadFont(font);
            font = LoadFontEx(fontPath, fontSize, 0, 0);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        lexer.render(font, tokens);
        // DrawFPS(0, 0);

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
