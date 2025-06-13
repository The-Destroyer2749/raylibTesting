#include "raylib.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"
#include "vector"
#include "imgui.h"
#include <cmath>
#include <numeric>
#include <iostream>
#include <chrono>


struct Block {
    Rectangle rectangle;
    Color color;
    unsigned int health;
};

struct Ball {
    Vector2 position;
    Color color;
    unsigned int damage;
};

struct Player {
    Vector2 position;
    float xPositionOffset;
    Color color;
    unsigned int health;
};

struct Defaults {
    int blockGridWidth;
    int blockGridHeight;
    Vector3 backgroundColor;
    Vector3 playerColor;
    float playerWidthMult;
    float playerHeightMult;
    float playerSpeed;
};

void recalculateBlockGrid(int blockGridWidth, int blockGridHeight, int& blockGridSize, std::vector<Block>& blockGrid) {
    blockGridSize = blockGridHeight * blockGridWidth;

    blockGrid.clear();

    // if you static_cast to float for these values it causes no rounding so the blocks end up having spaces around them
    float blockWidth = ceil(static_cast<float>(GetScreenWidth()) / static_cast<float>(blockGridWidth));
    float blockHeight = ceil((static_cast<float>(GetScreenHeight()) / 3) / static_cast<float>(blockGridHeight));


    for (int i=0; i<blockGridSize; i++) {
        int randomColorValue = GetRandomValue(160, 255);
        blockGrid.push_back(Block {
            Rectangle {
                static_cast<float>(i % blockGridWidth) * blockWidth,
                static_cast<float>(i / blockGridWidth) * blockHeight,
                static_cast<float>(blockWidth),
                static_cast<float>(blockHeight)
                },
            Color {
                static_cast<unsigned char>(randomColorValue),
                static_cast<unsigned char>(randomColorValue),
                static_cast<unsigned char>(randomColorValue),
                255
            },
            static_cast<unsigned int>(GetRandomValue(1, 3))
        });
    }
}

void solvePositions() {

}

int getNumberOfDigits(int num) {
    if (num != 0) {
        return static_cast<int>(log10(num));
    }
    else {
        return 1;
    }
}

char* findAspectRatio(int width, int height) { // Since I'm working with char* I just decided to use a mix of c and c++ as I'm most comfortable with c when I'm dealing with them
    int gcd = std::gcd(width, height);
    // std::cout << "gcd: " << gcd << std::endl;

    int numOfDigits = getNumberOfDigits(width / gcd);

    char* temp = (char*)malloc(numOfDigits * sizeof(char));

    int resultStringLength = getNumberOfDigits(numOfDigits + (height / gcd) + 1);
    char* result = (char*)malloc(numOfDigits * sizeof(char)); // I don't free the result char* as it is what's returned for the function

    sprintf(result, "%d", width / gcd);
    sprintf(temp, "%d", height / gcd);

    result = strcat(result, ":");
    result = strcat(result, temp);

    free(temp);
    return result;
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(960, 540, "raylibTesting");
    rlImGuiSetup(true);
    SetExitKey(0);

    std::vector<Block> blockGrid;
    float playerColor[3] = {170.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f};
    int playerWidth = GetScreenWidth() / 20;
    int playerHeight = GetScreenHeight() / 40;
    float playerWidthMult = 1;
    float playerHeightMult = 1;
    float playerSpeed = 1.0f;
    Player player = {Vector2 {static_cast<float>(GetScreenWidth()) / 2, static_cast<float>(GetScreenHeight()) / 2}, 0.0f, ColorFromNormalized({playerColor[0], playerColor[1], playerColor[2], 255.0f / 255.0f}), 5};
    Ball ball;

    int blockGridMinWidth = 10;
    int blockGridMinHeight = 3;
    int blockGridMaxWidth = 320;
    int blockGridMaxHeight = 180;
    int blockGridWidth = 32;
    int blockGridHeight = 18;
    int blockGridSize = blockGridMaxWidth * blockGridMaxHeight;

    std::chrono::time_point<std::chrono::high_resolution_clock> clockStart;
    std::chrono::time_point<std::chrono::high_resolution_clock> clockEnd;
    std::chrono::duration<double> deltaTime;
    std::array<float, 30> fpsBuffer = {}; // 30 or 60 is a good number to use generally It's best to go with what you know will be the worst case scenario for performance
    int microsecondsPerFrame;
    int mspf;
    float fps = 0;
    unsigned long long frameNumber = 0;


    float backgroundColor[3] = {80.0f / 255.0f, 80.0f / 255.0f, 80.0f / 255.0f};

    blockGrid.reserve(blockGridSize);


    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();

    recalculateBlockGrid(blockGridWidth, blockGridHeight, blockGridSize, blockGrid);

    Defaults defaultsList = {blockGridWidth, blockGridHeight, backgroundColor[0], backgroundColor[1], backgroundColor[2], playerColor[0], playerColor[1], playerColor[2], playerWidthMult, playerHeightMult, playerSpeed};

    while (!WindowShouldClose()) {
        clockStart = std::chrono::high_resolution_clock::now();

        BeginDrawing();
        ClearBackground(ColorFromNormalized({backgroundColor[0], backgroundColor[1], backgroundColor[2], 255.0f}));


        if (windowWidth != GetScreenWidth() || windowHeight != GetScreenHeight()) {
            windowWidth = GetScreenWidth();
            windowHeight = GetScreenHeight();

            player.position.x = GetScreenWidth() / 2;
            player.position.y = GetScreenHeight() / 2;

            blockGridSize = blockGridHeight * blockGridWidth;

            recalculateBlockGrid(blockGridWidth, blockGridHeight, blockGridSize, blockGrid);

            playerWidth = GetScreenWidth() / 25;
            playerHeight = GetScreenHeight() / 50;
        }

        if (IsKeyDown(KEY_A)) {
            player.xPositionOffset -= playerSpeed* deltaTime.count();
        }
        if (IsKeyDown(KEY_D)) {
            player.xPositionOffset += playerSpeed * deltaTime.count();
        }


        for (int i=0; i<blockGridSize; i++) {
            if (blockGrid[i].health != 0) {
                DrawRectangle(blockGrid[i].rectangle.x, blockGrid[i].rectangle.y, blockGrid[i].rectangle.width, blockGrid[i].rectangle.height, blockGrid[i].color);
            }
        }

        player.color = ColorFromNormalized({playerColor[0], playerColor[1], playerColor[2], 255.0f / 255.0f});


        DrawRectangle(
            player.position.x + (player.xPositionOffset * static_cast<float>(windowWidth) * 0.75f),
            player.position.y,
            playerWidth * playerWidthMult,
            playerHeight * playerHeightMult,
            player.color
            );



        rlImGuiBegin();

        if (ImGui::Begin("Settings")) {
            ImVec4 textColor;
            if (mspf <= 8.33f) { // 120 fps
                textColor = ImColor(0,255,0,255);
            }
            else if (mspf <= 16.66f) { // 60 fps
                textColor = ImColor(255,255,0,255);
            }
            else {
                textColor = ImColor(255,0,0,255);
            }

            ImGui::Text("milliseconds per frame: ");
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::SameLine();
            ImGui::Text("%d", mspf);
            ImGui::PopStyleColor();

            ImGui::Text("microseconds per frame: ");
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::SameLine();
            ImGui::Text("%d", microsecondsPerFrame % 1000);
            ImGui::PopStyleColor();

            ImGui::Text("fps: ");
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::SameLine();
            ImGui::Text("%.0f", fps);
            ImGui::PopStyleColor();

            if (ImGui::Button("Reset all parameters")) { // incredibly bad way to reset all params maybe make this a function later on
                blockGridWidth = defaultsList.blockGridWidth;
                blockGridHeight = defaultsList.blockGridHeight;
                backgroundColor[0] = defaultsList.backgroundColor.x;
                backgroundColor[1] = defaultsList.backgroundColor.y;
                backgroundColor[2] = defaultsList.backgroundColor.z;
                playerColor[0] = defaultsList.playerColor.x;
                playerColor[1] = defaultsList.playerColor.y;
                playerColor[2] = defaultsList.playerColor.z;
                playerWidthMult = defaultsList.playerWidthMult;
                playerHeightMult = defaultsList.playerHeightMult;
                playerSpeed = defaultsList.playerSpeed;
            }

            if (ImGui::CollapsingHeader("Block Grid", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Text("Block grid sizing:");
                if (ImGui::SliderInt("Block grid width", &blockGridWidth, blockGridMinWidth, blockGridMaxWidth)) {
                    recalculateBlockGrid(blockGridWidth, blockGridHeight, blockGridSize, blockGrid);
                }
                if (ImGui::SliderInt("Block grid height", &blockGridHeight, blockGridMinHeight, blockGridMaxHeight)) {
                    recalculateBlockGrid(blockGridWidth, blockGridHeight, blockGridSize, blockGrid);
                }

                ImGui::Text("Total Blocks: %d", blockGridSize);
                ImGui::Text("Grid aspect ratio: %s", findAspectRatio(blockGridWidth, blockGridHeight));
            }
            if (ImGui::CollapsingHeader("Coloring")) {
                ImGui::ColorEdit3("Background color", backgroundColor);

                ImGui::ColorEdit3("Player color", playerColor);
            }
            if (ImGui::CollapsingHeader("Player")) {
                ImGui::SliderFloat("Player width: ", &playerWidthMult, 0.25f, 4.0f);
                ImGui::SliderFloat("Player height: ", &playerHeightMult, 0.25f, 4.0f);
                ImGui::SliderFloat("Player speed: ", &playerSpeed, 0.25f, 4.0f);
            }
        }
        ImGui::End();



        rlImGuiEnd();
        EndDrawing();
        frameNumber++;
        clockEnd = std::chrono::high_resolution_clock::now();
        deltaTime = clockEnd - clockStart;
        microsecondsPerFrame = static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>(deltaTime).count());
        mspf = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime).count());
        fpsBuffer[frameNumber % std::size(fpsBuffer)] = 1000000.0f / static_cast<float>(microsecondsPerFrame);

        float fpsSum = 0.0f;
        for (float fpsNum : fpsBuffer) {
            fpsSum += fpsNum;

            fps = fpsSum / static_cast<float>(std::size(fpsBuffer));
        }
    }

    CloseWindow();
    rlImGuiShutdown();

    return 0;
}