#include "raylib.h"
#include "rlImGui.h"
#include "vector"
#include "imgui.h"
#include <iostream>


struct Block {
    Rectangle rectangle;
    Color color;
    unsigned int health;
};

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "raylibTesting");
    rlImGuiSetup(true);

    std::vector<Block> blockGrid;

    int blockGridWidth = 30;
    int blockGridHeight = 20;
    int blockGridSize;

    blockGrid.reserve(blockGridSize);

    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        if (windowWidth != GetScreenWidth() || windowHeight != GetScreenHeight()) {
            windowWidth = GetScreenWidth();
            windowHeight = GetScreenHeight();
            blockGridSize = blockGridHeight * blockGridWidth;

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

        rlImGuiBegin();

        if (ImGui::Begin("Settings")) {
            ImGui::Text("Block grid width");
            ImGui::SliderInt("Block grid width slider", &blockGridWidth, 1, 100);

            ImGui::Text("Block grid height");
            ImGui::SliderInt("Block grid height slider", &blockGridHeight, 1, 100);
        }
        ImGui::End();

        rlImGuiEnd();
        EndDrawing();
    }

    CloseWindow();
    rlImGuiShutdown();

    return 0;
}