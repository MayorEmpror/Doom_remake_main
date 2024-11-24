#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int titleScreen(void) {
    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Welcome to DOOM!");

    // Initialize the 3D camera
    Camera camera = { 0 };
    camera.position = (Vector3){ 16.0f, 14.0f, 16.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Load cubic map resources
    Image image = LoadImage("C:/Users/SyedA/Desktop/doom/resources/Main_Game/cubicmac_Mesh.png");
    Texture2D cubicmap = LoadTextureFromImage(image);
    Mesh mesh = GenMeshCubicmap(image, (Vector3){ 1.0f, 1.0f, 1.0f });
    Model model = LoadModelFromMesh(mesh);
    Texture2D texture = LoadTexture("C:/Users/SyedA/Desktop/doom/resources/Main_Game/cubicmap_atlas.png");
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    Vector3 mapPosition = { -16.0f, 0.0f, -8.0f };
    UnloadImage(image);

    // Button positions and sizes
    Rectangle startButton = { screenWidth / 2 - 100, screenHeight / 2 - 100, 200, 50 };
    Rectangle creditsButton = { screenWidth / 2 - 100, screenHeight / 2, 200, 50 };
    Rectangle exitButton = { screenWidth / 2 - 100, screenHeight / 2 + 100, 200, 50 };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mousePoint, startButton)) {
                start_Game();
            } else if (CheckCollisionPointRec(mousePoint, creditsButton)) {
                credits();
            } else if (CheckCollisionPointRec(mousePoint, exitButton)) {
                break;
            }
        }

        // Update camera to rotate around the map
        UpdateCamera(&camera, CAMERA_ORBITAL);

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Draw the rotating 3D scene
            BeginMode3D(camera);
                DrawModel(model, mapPosition, 1.0f, WHITE);
            EndMode3D();

            // Draw buttons
            DrawRectangleRec(startButton, LIGHTGRAY);
            DrawRectangleRec(creditsButton, LIGHTGRAY);
            DrawRectangleRec(exitButton, LIGHTGRAY);

            // Button text
            DrawText("Start Game", screenWidth / 2 - MeasureText("Start Game", 20) / 2, screenHeight / 2 - 90, 20, BLACK);
            DrawText("Credits", screenWidth / 2 - MeasureText("Credits", 20) / 2, screenHeight / 2 + 10, 20, BLACK);
            DrawText("Exit Game", screenWidth / 2 - MeasureText("Exit Game", 20) / 2, screenHeight / 2 + 110, 20, BLACK);

            DrawFPS(10, 10);
        EndDrawing();
    }

    // Cleanup
    UnloadTexture(cubicmap);
    UnloadTexture(texture);
    UnloadModel(model);
    CloseWindow();

    return 0;
}

int main(void) {
    titleScreen();
    return 0;
}


