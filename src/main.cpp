#include <raylib.h>
#include <iostream>

#include "lexer.cpp"


constexpr const char* fontPath = "src/fonts/JetBrains-Mono.ttf";
// constexpr const char* fontPath = "src/fonts/Ancient-Medium.ttf";
// constexpr const char* fontPath = "src/fonts/agenda-king.regular.ttf";
// constexpr const char* fontPath = "src/fonts/comic-sans.ttf";


int main(int argc, char *argv[])
{
    InitWindow(800, 800, "Codeshot");

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

for x in range(10):
    z = x

print(sieve(200))
print(f"chicken \"yes\" {sieve(20)}"))";

    Lexer lexer;

    lexer.setSource(sourceCode);
    lexer.setLanguage(Lexer::Language::Python);

    std::vector<Lexer::Token> tokens = lexer.lex();
    TraceLog(LOG_INFO, TextFormat("Token Count: %lld", tokens.size()));

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
