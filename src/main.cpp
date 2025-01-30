#include <raylib.h>
#include <iostream>

#include "lexer.cpp"


constexpr const char* fontPath = "src/fonts/JetBrains-Mono.ttf";
// constexpr const char* fontPath = "src/fonts/Ancient-Medium.ttf";
// constexpr const char* fontPath = "src/fonts/agenda-king.regular.ttf";
// constexpr const char* fontPath = "src/fonts/comic-sans.ttf";


int main(int argc, char* argv[])
{
    InitWindow(800, 800, "Codeshot");

//     std::string sourceCode = R"(def sieve(n: int):
// 	if n < 2:
// 		return None

// 	l = list(range(2, n+1))

// 	index = 2
// 	while index < l[-1]:
// 		for i in l:
// 			if i % index == 0 and i > index:
// 				l.remove(i)
// 		index += 1

// 	return l

// for x in range(10):
// 	z = x

// print(sieve(200))
// print(f"chicken \"yes\" {sieve(20)}"))";

    std::string sourceCode = R"(#include <iostream>
using namespace std;

const int MAXPOLY = 200;
double EPSILON = 0.000001;

struct Point
{
    double x, y;
};

struct Polygon
{
    Point p[MAXPOLY];
    int n;

    Polygon()
    {
        for (int i = 0; i < MAXPOLY; i++)
            Point p[i];// = new Point();
    }
};

double area(Polygon p)
{
    double total = 0;

    for (int i = 0; i < p.n; i++)
    {
        int j = (i + 1) % p.n;
        total += (p.p[i].x * p.p[j].y) - (p.p[j].x * p.p[i].y);
    }

    return total / 2;
}

int main(int argc, char **argv)
{
    Polygon p;

    cout << "Enter the number of points in Polygon: ";
    cin >> p.n;
    cout << "Enter the coordinates of each point: <x> <y>";

    for (int i = 0; i < p.n; i++)
    {
        cin >> p.p[i].x;
        cin >> p.p[i].y;
    }

    double a = area(p);

    if (a > 0) cout << "The Area of Polygon with " << (p.n)
                    << " points using Slicker Algorithm is : "
                    << a;

    else cout << "The Area of Polygon with " << p.n
              << " points using Slicker Algorithm is : "
              << (a * -1);
})";

    Lexer lexer;

    lexer.setSource(sourceCode);
    lexer.setLanguage(Lexer::Language::Cpp);

    std::vector<Lexer::Token> tokens = lexer.lex();
    TraceLog(LOG_INFO, TextFormat("Token Count: %lld", tokens.size()));

    int fontSize = 25;
    Font font    = LoadFontEx(fontPath, fontSize, 0, 0);

    Vector2 renderOffset = { 0, 0 };

    while (!WindowShouldClose()) {
        float mouseWheelMove = GetMouseWheelMove();

        Vector2 mouseDelta = GetMouseDelta();

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            renderOffset.x += mouseDelta.x;
            renderOffset.y += mouseDelta.y;
        }

        if (mouseWheelMove) {
            fontSize += static_cast<int>(mouseWheelMove) * 2;
            UnloadFont(font);
            font = LoadFontEx(fontPath, fontSize, 0, 0);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        lexer.render(renderOffset, font, tokens);
        // DrawFPS(0, 0);

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
