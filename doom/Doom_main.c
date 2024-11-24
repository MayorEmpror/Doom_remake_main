#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rlgl.h"
void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color); // Draw cube textured
void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color);
// Function prototypes
int titleScreen(void);
int start_Game(void);
int credits(void);

int main(void) {
    titleScreen();
    return 0;
}

//------------------------------------------------------------------------------------
// Ttile Screen main entry point
//------------------------------------------------------------------------------------

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
    Image image = LoadImage("./resources/Main_Game/cubicmac_Mesh.png");
    Texture2D cubicmap = LoadTextureFromImage(image);
    Mesh mesh = GenMeshCubicmap(image, (Vector3){ 1.0f, 1.0f, 1.0f });
    Model model = LoadModelFromMesh(mesh);
    Texture2D texture = LoadTexture("./resources/Main_Game/cubicmap_atlas.png");
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
            ClearBackground(DARKBROWN);

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

//------------------------------------------------------------------------------------
// Game main entry point
//------------------------------------------------------------------------------------
float dotProduct(Vector3 a, Vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

int start_Game(void) {
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Welcome to Doom!");

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.6f, 0.6f, 0.6f };
    camera.target = (Vector3){ 0.185f, 0.4f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Image imMap = LoadImage("./resources/Main_Game/cubicmac_Mesh.png");
    Texture2D cubicmap = LoadTextureFromImage(imMap);
    Mesh mesh = GenMeshCubicmap(imMap, (Vector3){ 1.0f, 1.0f, 1.0f });
    Model model = LoadModelFromMesh(mesh);

    Texture2D texture = LoadTexture("./resources/Main_Game/cubicmap_atlas.png");
    Texture2D Enemy_one = LoadTexture("./resources/Main_Game/Enemy.png");
    // Texture2D Enemy_two = LoadTexture("./resources/Main_Game/flyingimp.png");
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    Color *mapPixels = LoadImageColors(imMap);
    UnloadImage(imMap);

    Vector3 mapPosition = { -16.0f, 0.0f, -8.0f };
    DisableCursor();
    SetTargetFPS(60);
Vector3 enemy_one_position = { 5.0f, 0.5f, 1.0f }; // Offset from camera
// Vector3 enemy_two_position = { 5.0f, 0.5f, 1.0f }; // Offset from camera
 float size = 1.0f; 
                          
    Mesh cubeMesh = GenMeshCube(size, size, size); // Generate cube mesh
    Model EnemyModel = LoadModelFromMesh(cubeMesh);
    SetMaterialTexture(&EnemyModel.materials[0], MATERIAL_MAP_ALBEDO, Enemy_one);
   rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE, RL_FUNC_ADD, RL_MAX);
    while (!WindowShouldClose()) {
        Vector3 oldCamPos = camera.position;
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);
     
         Vector3 directionToCamera = {
            camera.position.x - enemy_one_position.x,
            camera.position.y - enemy_one_position.y,
            camera.position.z - enemy_one_position.z
        };
         float angleY = atan2(directionToCamera.x, directionToCamera.z) * (180.0f / PI); // Convert radians to degrees


         //  enemy_one_position.y = camera.position.y + 2.0f;
             
        Vector2 playerPos = { camera.position.x, camera.position.z };
        float playerRadius = 0.1f;
        int playerCellX = (int)(playerPos.x - mapPosition.x + 0.5f);
        int playerCellY = (int)(playerPos.y - mapPosition.z + 0.5f);

        if (playerCellX < 0) playerCellX = 0;
        else if (playerCellX >= cubicmap.width) playerCellX = cubicmap.width - 1;

        if (playerCellY < 0) playerCellY = 0;
        else if (playerCellY >= cubicmap.height) playerCellY = cubicmap.height - 1;

        for (int y = 0; y < cubicmap.height; y++) {
            for (int x = 0; x < cubicmap.width; x++) {
                if ((mapPixels[y*cubicmap.width + x].r == 255) &&
                    (CheckCollisionCircleRec(playerPos, playerRadius, (Rectangle){ mapPosition.x - 0.5f + x*1.0f, mapPosition.z - 0.5f + y*1.0f, 1.0f, 1.0f }))) {
                    camera.position = oldCamPos;
                }
            }
        }

        BeginDrawing();
             ClearBackground(BLACK);
            HideCursor();
            BeginMode3D(camera);
             if (camera.position.x - enemy_one_position.x > 0.1) {
            enemy_one_position.x += 0.025;
        }
        if (camera.position.z - enemy_one_position.z > 0.1) {
            enemy_one_position.z += 0.025;
        }
        
        if (camera.position.x - enemy_one_position.x < -0.1) {
            enemy_one_position.x -= 0.025;
        }
        if (camera.position.z - enemy_one_position.z < -0.1) {
            enemy_one_position.z -= 0.025;
        }
         BeginBlendMode(RL_BLEND_CUSTOM_SEPARATE);
          DrawModelEx(EnemyModel, enemy_one_position, (Vector3){0.0f, 1.0f, 0.0f}, angleY, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
                EndBlendMode();
               DrawModel(model, mapPosition, 1.0f, WHITE); //Enemy_two
            //    DrawCubeTexture(Enemy_one,enemy_one_position, 0.0f, 1.0f, 1.0f, WHITE);
                //    DrawCubeTexture(Enemy_two,enemy_two_position, 0.0f, 1.0f, 1.0f, WHITE);
            EndMode3D();
            DrawTextureEx(cubicmap, (Vector2){ GetScreenWidth() - cubicmap.width*4.0f - 20, 20.0f }, 0.0f, 4.0f, RED);
            DrawRectangleLines(GetScreenWidth() - cubicmap.width*4 - 20, 20, cubicmap.width*4, cubicmap.height*4, GREEN);
            DrawRectangle(GetScreenWidth() - cubicmap.width*4 - 20 + playerCellX*4, 20 + playerCellY*4, 4, 4, RED);
            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadImageColors(mapPixels);
    UnloadTexture(cubicmap);
    UnloadTexture(texture);
    UnloadModel(model);
    UnloadModel(EnemyModel);
    CloseWindow();

    return 0;
}

// Credits Screen (Placeholder)
int credits(void) {
    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Credits");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Credits", screenWidth / 2 - MeasureText("Credits", 40) / 2, screenHeight / 2 - 20, 40, BLACK);
            DrawText("Press ESC to return", screenWidth / 2 - MeasureText("Press ESC to return", 20) / 2, screenHeight / 2 + 30, 20, GRAY);
        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE)) break;  // Exit to title screen
    }

    CloseWindow();
    return 0;
}

void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    // Set desired texture to be enabled while drawing following vertex data
    rlSetTexture(texture.id);

    // Vertex data transformation can be defined with the commented lines,
    // but in this example we calculate the transformed vertex data directly when calling rlVertex3f()
    //rlPushMatrix();
        // NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
        //rlTranslatef(2.0f, 0.0f, 0.0f);
        //rlRotatef(45, 0, 1, 0);
        //rlScalef(2.0f, 2.0f, 2.0f);

        rlBegin(RL_QUADS);
            rlColor4ub(color.r, color.g, color.b, color.a);
            // Front Face
            rlNormal3f(0.0f, 0.0f, 1.0f);       // Normal Pointing Towards Viewer
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
            // Back Face
            rlNormal3f(0.0f, 0.0f, - 1.0f);     // Normal Pointing Away From Viewer
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
            // Top Face
            rlNormal3f(0.0f, 1.0f, 0.0f);       // Normal Pointing Up
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
            // Bottom Face
            rlNormal3f(0.0f, - 1.0f, 0.0f);     // Normal Pointing Down
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            // Right face
            rlNormal3f(1.0f, 0.0f, 0.0f);       // Normal Pointing Right
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            // Left Face
            rlNormal3f( - 1.0f, 0.0f, 0.0f);    // Normal Pointing Left
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
        rlEnd();
    //rlPopMatrix();

    rlSetTexture(0);
}

// Draw cube with texture piece applied to all faces
void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;
    float texWidth = (float)texture.width;
    float texHeight = (float)texture.height;

    // Set desired texture to be enabled while drawing following vertex data
    rlSetTexture(texture.id);

    // We calculate the normalized texture coordinates for the desired texture-source-rectangle
    // It means converting from (tex.width, tex.height) coordinates to [0.0f, 1.0f] equivalent 
    rlBegin(RL_QUADS);
        rlColor4ub(color.r, color.g, color.b, color.a);

        // Front face
        rlNormal3f(0.0f, 0.0f, 1.0f);
        rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x - width/2, y - height/2, z + length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x + width/2, y - height/2, z + length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
        rlVertex3f(x + width/2, y + height/2, z + length/2);
        rlTexCoord2f(source.x/texWidth, source.y/texHeight);
        rlVertex3f(x - width/2, y + height/2, z + length/2);

        // Back face
        rlNormal3f(0.0f, 0.0f, - 1.0f);
        rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x - width/2, y - height/2, z - length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
        rlVertex3f(x - width/2, y + height/2, z - length/2);
        rlTexCoord2f(source.x/texWidth, source.y/texHeight);
        rlVertex3f(x + width/2, y + height/2, z - length/2);
        rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x + width/2, y - height/2, z - length/2);

        // Top face
        rlNormal3f(0.0f, 1.0f, 0.0f);
        rlTexCoord2f(source.x/texWidth, source.y/texHeight);
        rlVertex3f(x - width/2, y + height/2, z - length/2);
        rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x - width/2, y + height/2, z + length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x + width/2, y + height/2, z + length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
        rlVertex3f(x + width/2, y + height/2, z - length/2);

        // Bottom face
        rlNormal3f(0.0f, - 1.0f, 0.0f);
        rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
        rlVertex3f(x - width/2, y - height/2, z - length/2);
        rlTexCoord2f(source.x/texWidth, source.y/texHeight);
        rlVertex3f(x + width/2, y - height/2, z - length/2);
        rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x + width/2, y - height/2, z + length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x - width/2, y - height/2, z + length/2);

        // Right face
        rlNormal3f(1.0f, 0.0f, 0.0f);
        rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x + width/2, y - height/2, z - length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
        rlVertex3f(x + width/2, y + height/2, z - length/2);
        rlTexCoord2f(source.x/texWidth, source.y/texHeight);
        rlVertex3f(x + width/2, y + height/2, z + length/2);
        rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x + width/2, y - height/2, z + length/2);

        // Left face
        rlNormal3f( - 1.0f, 0.0f, 0.0f);
        rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x - width/2, y - height/2, z - length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x - width/2, y - height/2, z + length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
        rlVertex3f(x - width/2, y + height/2, z + length/2);
        rlTexCoord2f(source.x/texWidth, source.y/texHeight);
        rlVertex3f(x - width/2, y + height/2, z - length/2);

    rlEnd();

    rlSetTexture(0);
}