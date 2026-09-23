#include "raylib.h"
#include "raymath.h"
#include <vector>

enum PhysicsScene { SCENE_BOUNCE = 1, SCENE_SPRING, SCENE_ATTRACTOR, SCENE_COLLISION };

struct Ball {
    Vector2 position;
    Vector2 velocity;
    float radius;
    float restitution;
    Color color;
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "AETHER ENGINE - Light Mode");
    SetTargetFPS(60);

    PhysicsScene currentScene = SCENE_BOUNCE;

    // --- ESCENA 1: Pelota Rebotando ---
    Ball bounceBall = { { 400.0f, 200.0f }, { 200.0f, -100.0f }, 22.0f, 0.75f, BLACK };
    const float gravity = 980.0f;

    // --- ESCENA 2: Resorte ---
    Vector2 anchor = { 400.0f, 150.0f };
    Vector2 springPos = { 400.0f, 350.0f };
    Vector2 springVel = { 0.0f, 0.0f };
    const float k = 15.0f;
    const float damping = 0.95f;
    bool draggingSpring = false;

    // --- ESCENA 3: Atracción Gravitatoria ---
    std::vector<Ball> particles;
    for (int i = 0; i < 40; i++) {
        Ball p;
        p.position = { (float)GetRandomValue(50, 750), (float)GetRandomValue(80, 550) };
        p.velocity = { (float)GetRandomValue(-50, 50), (float)GetRandomValue(-50, 50) };
        p.radius = 6.0f;
        p.restitution = 1.0f;
        p.color = DARKGRAY;
        particles.push_back(p);
    }

    // --- ESCENA 4: Colisión entre Pelotas ---
    std::vector<Ball> balls = {
        { { 200.0f, 300.0f }, { 200.0f, -100.0f }, 25.0f, 1.0f, BLACK },
        { { 600.0f, 300.0f }, { -150.0f, 120.0f }, 30.0f, 1.0f, DARKBLUE },
        { { 400.0f, 200.0f }, { 80.0f, 200.0f },   20.0f, 1.0f, RED }
    };

    // Paleta para Fondo Blanco / Modo Claro
    Color bgLight      = WHITE;
    Color headerBg     = { 240, 240, 240, 255 };
    Color textDim      = { 120, 120, 120, 255 };
    Color accentActive = { 0, 122, 255, 255 }; // Azul vibrante para activos
    Color titleDark    = { 20, 20, 20, 255 };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Vector2 mousePos = GetMousePosition();
        bool isPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || GetTouchPointCount() > 0;

        // --- CONTROLES TÁCTILES / CLIC EN EL MENÚ SUPERIOR ---
        if (isPressed && mousePos.y < 40) {
            if (mousePos.x >= 110 && mousePos.x < 240) currentScene = SCENE_BOUNCE;
            else if (mousePos.x >= 240 && mousePos.x < 330) currentScene = SCENE_SPRING;
            else if (mousePos.x >= 330 && mousePos.x < 430) currentScene = SCENE_ATTRACTOR;
            else if (mousePos.x >= 430 && mousePos.x < 550) currentScene = SCENE_COLLISION;
        }

        // TECLADO (PC)
        if (IsKeyPressed(KEY_ONE))   currentScene = SCENE_BOUNCE;
        if (IsKeyPressed(KEY_TWO))   currentScene = SCENE_SPRING;
        if (IsKeyPressed(KEY_THREE)) currentScene = SCENE_ATTRACTOR;
        if (IsKeyPressed(KEY_FOUR))  currentScene = SCENE_COLLISION;

        // FÍSICAS
        switch (currentScene) {
            case SCENE_BOUNCE: {
                bounceBall.velocity.y += gravity * dt;
                bounceBall.position.x += bounceBall.velocity.x * dt;
                bounceBall.position.y += bounceBall.velocity.y * dt;

                if (bounceBall.position.y + bounceBall.radius >= screenHeight - 10) {
                    bounceBall.position.y = (screenHeight - 10) - bounceBall.radius;
                    bounceBall.velocity.y *= -bounceBall.restitution;
                    bounceBall.velocity.x *= 0.98f;
                }
                if (bounceBall.position.x + bounceBall.radius >= screenWidth || bounceBall.position.x - bounceBall.radius <= 0) {
                    bounceBall.velocity.x *= -bounceBall.restitution;
                }

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mousePos.y > 40) {
                    bounceBall.position = mousePos;
                    bounceBall.velocity = { 300.0f, -200.0f };
                }
                break;
            }

            case SCENE_SPRING: {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointCircle(mousePos, springPos, 30.0f)) {
                    draggingSpring = true;
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) draggingSpring = false;

                if (draggingSpring) {
                    springPos = mousePos;
                    springVel = { 0, 0 };
                } else {
                    Vector2 displacement = Vector2Subtract(springPos, anchor);
                    Vector2 force = Vector2Scale(displacement, -k);
                    springVel = Vector2Add(springVel, Vector2Scale(force, dt));
                    springVel = Vector2Scale(springVel, damping);
                    springPos = Vector2Add(springPos, Vector2Scale(springVel, dt));
                }
                break;
            }

            case SCENE_ATTRACTOR: {
                Vector2 center = mousePos;
                for (auto& p : particles) {
                    Vector2 dir = Vector2Subtract(center, p.position);
                    float dist = Vector2Length(dir);
                    if (dist < 10.0f) dist = 10.0f;
                    
                    Vector2 normDir = Vector2Normalize(dir);
                    float forceStrength = 30000.0f / (dist * dist);
                    
                    p.velocity = Vector2Add(p.velocity, Vector2Scale(normDir, forceStrength * dt));
                    p.position = Vector2Add(p.position, Vector2Scale(p.velocity, dt));
                }
                break;
            }

            case SCENE_COLLISION: {
                for (size_t i = 0; i < balls.size(); i++) {
                    balls[i].position = Vector2Add(balls[i].position, Vector2Scale(balls[i].velocity, dt));

                    if (balls[i].position.x - balls[i].radius < 0 || balls[i].position.x + balls[i].radius > screenWidth) balls[i].velocity.x *= -1;
                    if (balls[i].position.y - balls[i].radius < 40 || balls[i].position.y + balls[i].radius > screenHeight) balls[i].velocity.y *= -1;

                    for (size_t j = i + 1; j < balls.size(); j++) {
                        if (CheckCollisionCircles(balls[i].position, balls[i].radius, balls[j].position, balls[j].radius)) {
                            Vector2 tempVel = balls[i].velocity;
                            balls[i].velocity = balls[j].velocity;
                            balls[j].velocity = tempVel;
                        }
                    }
                }
                break;
            }
        }

        // RENDERIZADO
        BeginDrawing();
            ClearBackground(bgLight); // Limpieza completa de pantalla (sin blur)

            switch (currentScene) {
                case SCENE_BOUNCE:
                    DrawRectangle(0, screenHeight - 10, screenWidth, 10, RED);
                    DrawCircleV(bounceBall.position, bounceBall.radius, bounceBall.color);
                    break;

                case SCENE_SPRING:
                    DrawLineEx(anchor, springPos, 4, BLUE);
                    DrawCircleV(anchor, 10, RED);
                    DrawCircleV(springPos, 25, BLACK);
                    break;

                case SCENE_ATTRACTOR:
                    DrawCircleV(mousePos, 14, RED);
                    for (const auto& p : particles) {
                        DrawCircleV(p.position, p.radius, p.color);
                    }
                    break;

                case SCENE_COLLISION:
                    for (const auto& b : balls) {
                        DrawCircleV(b.position, b.radius, b.color);
                    }
                    break;
            }

            // BARRA INTERACTIVA SUPERIOR
            DrawRectangle(0, 0, screenWidth, 40, headerBg);
            DrawText("[AETHER]", 12, 12, 18, titleDark);

            DrawText("KINEMATICS", 120, 12, 18, (currentScene == SCENE_BOUNCE) ? accentActive : textDim);
            DrawText("|",          235, 12, 18, textDim);
            DrawText("HOOKE",      250, 12, 18, (currentScene == SCENE_SPRING) ? accentActive : textDim);
            DrawText("|",          325, 12, 18, textDim);
            DrawText("GRAVITY",    340, 12, 18, (currentScene == SCENE_ATTRACTOR) ? accentActive : textDim);
            DrawText("|",          425, 12, 18, textDim);
            DrawText("IMPULSE",    440, 12, 18, (currentScene == SCENE_COLLISION) ? accentActive : textDim);

            // INSTRUCCIONES ADAPTADAS A MÓVIL Y NAVEGADOR
            const char* testName = "";
            switch (currentScene) {
                case SCENE_BOUNCE:    testName = "TEST 01: Kinematics (Toca/Haz clic para lanzar)"; break;
                case SCENE_SPRING:    testName = "TEST 02: Resorte Hooke (Arrastra con la pantalla)"; break;
                case SCENE_ATTRACTOR: testName = "TEST 03: Atraccion (Mueve el dedo/cursor)"; break;
                case SCENE_COLLISION: testName = "TEST 04: Colisiones Elasticas"; break;
            }
            DrawText(testName, 12, 48, 15, DARKGRAY);

            DrawFPS(screenWidth - 85, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
