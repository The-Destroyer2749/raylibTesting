#include "raylib.h"
#include "rlImGui.h"
#include "vector"
#include <iostream>


int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "raylibTesting");
    rlImGuiSetup(true);


    struct Block {
        Rectangle rectangle;
        Color color;
        unsigned int health;
    };

    std::vector<Block> blockGrid;

    int blockGridWidth = 30;
    int blockGridHeight = 20;
    int blockGridSize = blockGridHeight * blockGridWidth;

    blockGrid.reserve(blockGridSize);



    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();

    while (!WindowShouldClose()) {


        BeginDrawing();
        ClearBackground(DARKGRAY);

        rlImGuiBegin();
        rlImGuiEnd();

        if (windowWidth != GetScreenWidth() || windowHeight != GetScreenHeight()) {
            windowWidth = GetScreenWidth();
            windowHeight = GetScreenHeight();

            blockGrid.clear();

            float blockWidth = GetScreenWidth() / blockGridWidth;
            float blockHeight = GetScreenHeight() / blockGridHeight;


            for (int i=0; i<blockGridSize; i++) {
                int randomColorValue = GetRandomValue(160, 255);
                blockGrid.push_back(Block {
                    static_cast<float>(i % blockGridWidth) * blockWidth,
                    static_cast<float>(i / blockGridWidth) * blockHeight,
                    static_cast<float>(blockWidth),
                    static_cast<float>(blockHeight),
                    Color {static_cast<unsigned char>(randomColorValue), static_cast<unsigned char>(randomColorValue), static_cast<unsigned char>(randomColorValue), 255},
                    static_cast<unsigned int>(GetRandomValue(0, 10))
                });
            }
            /*
            for (int i=0; i<blockGridSize; i++) {
                std::cout << "X pos: " << static_cast<float>((i % blockGridWidth) * blockWidth) << " | Y pos: " << static_cast<float>((i / blockGridWidth) * blockHeight) << " ][ Width: " << blockWidth << " | Height: " << blockHeight << std::endl;
            }
            std::cout << std::endl << std::endl;
            */
        }

        for (int i=0; i<blockGridSize; i++) {
            if (blockGrid[i].health != 0) {
                DrawRectangle(blockGrid[i].rectangle.x, blockGrid[i].rectangle.y, blockGrid[i].rectangle.width, blockGrid[i].rectangle.height, blockGrid[i].color);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    rlImGuiShutdown();

    return 0;
}