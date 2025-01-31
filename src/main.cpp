#include <raylib.h>
#include <iostream>

#include "lexer.cpp"


constexpr const char* fontPath = "src/fonts/JetBrains-Mono.ttf";
// constexpr const char* fontPath = "src/fonts/Ancient-Medium.ttf";
// constexpr const char* fontPath = "src/fonts/agenda-king.regular.ttf";
// constexpr const char* fontPath = "src/fonts/comic-sans.ttf";


int main(int argc, char* argv[])
{
    Vector2 windowSize = { 800.0f, 800.0f };
    InitWindow(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y), "Codeshot");

    std::string sourceCode = LoadFileText("src/lexer.cpp");

    Lexer lexer;

    lexer.setSource(sourceCode);
    lexer.setLanguage(Lexer::Language::Cpp);

    lexer.setColor(TokenType::Keyword, Color{ 255, 255, 0, 255 });
    lexer.setColor(TokenType::Identifier, Color{ 255, 255, 255, 255 });
    lexer.setColor(TokenType::Literal, Color{ 255, 100, 0, 255 });
    lexer.setColor(TokenType::Operator, Color{ 255, 255, 255, 255 });
    lexer.setColor(TokenType::Bracket, Color{ 255, 255, 255, 255 });
    lexer.setColor(TokenType::CommentSingleLine, Color{ 120, 120, 120, 255 });
    lexer.setColor(TokenType::String, Color{ 0, 255, 0, 255 });
    // lexer.setBackgroundColor(Color{51, 51, 51, 255});

    std::vector<Token> tokens = lexer.lex();
    TraceLog(LOG_INFO, TextFormat("Token Count: %lld", tokens.size()));

    int fontSize = 25;
    Font font    = LoadFontEx(fontPath, fontSize, 0, 0);

    Vector2 renderOffset = { 0, 0 };

    Camera2D camera = {
        .offset   = Vector2{ windowSize.x / 2, windowSize.y / 2 },
        .target   = Vector2{ windowSize.x / 2, windowSize.y / 2 },
        .rotation = 0.0f,
        .zoom     = 1.0f
    };

    while (!WindowShouldClose()) {
        float mouseWheelMove = GetMouseWheelMove();

        Vector2 mouseDelta = GetMouseDelta();

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            camera.target  = Vector2{ camera.target.x - mouseDelta.x / camera.zoom, camera.target.y - mouseDelta.y / camera.zoom };
        }

        camera.zoom += (mouseWheelMove * 0.2f * camera.zoom);

        if (IsKeyPressed(KEY_R)) {
            camera.zoom   = 1.0f;
            camera.target = Vector2{ windowSize.x / 2.0f, windowSize.y / 2.0f };
        }

        // if (mouseWheelMove) {
        //     fontSize += static_cast<int>(mouseWheelMove) * 2;
        //     UnloadFont(font);
        //     font = LoadFontEx(fontPath, fontSize, 0, 0);
        // }

        BeginDrawing();
        ClearBackground(Color{ 51, 51, 51, 255 });

        BeginMode2D(camera);
        lexer.render(windowSize, font, tokens);
        EndMode2D();

        DrawFPS(0, 0);

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
