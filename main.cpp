#include <iostream>
#include <raylib.h>

const int screenWidth = 1280;
const int screenHeight = 720;

int playerScore = 0;
int computerScore = 0;

Color green = {27, 201, 0, 255};
Color darkGreen = {15, 115, 0, 255};
Color yellow = {242, 221, 102, 255};
Color orange = {219, 162, 43, 255};

class Ball {
    public:
        int radius = 10;
        int ballSpeedX = 7;
        int ballSpeedY = 7;
        int posX = screenWidth / 2;
        int posY = screenHeight / 2;
        int direction[2] = {1, -1};

        void Draw() {
            DrawCircle(posX, posY, radius, yellow);
        }

        void Update() {
            Draw();

            posX += ballSpeedX;
            posY += ballSpeedY;

            if (posY + radius >= screenHeight) {
                ballSpeedY *= -1;
            }

            if (posY - radius < 0) {
                ballSpeedY *= -1;
            }

            if (posX > screenWidth) {
                Reset();
                computerScore++;
            }

            if (posX < 0) {
                Reset();
                playerScore++;
            }
        }

        void Reset() {
            posX = screenWidth / 2;
            posY = screenHeight / 2;

            ballSpeedX *= direction[GetRandomValue(0, 1)];
            ballSpeedY *= direction[GetRandomValue(0, 1)];
        }
};

class Paddle {
    public:
        int width;
        int height;
        int posX;
        int posY;
        int speedY = 6;
        Rectangle paddle;

        Paddle() {
            width = 20;
            height = 120;
            posX = screenWidth - 30;
            posY = screenHeight / 2 - height / 2;
        
            paddle = Rectangle{(float)posX, (float)posY, (float)width, (float)height};
        }

        void Draw(Rectangle paddle) {
            DrawRectangleRounded(paddle, 0.8, 0, orange);
        }

        void Update() {
            Draw(paddle);

            if (IsKeyDown(KEY_UP)) {
                paddle.y -= speedY;
            }

            if (IsKeyDown(KEY_DOWN)) {
                paddle.y += speedY;
            }

            if (paddle.y < 0) {
                paddle.y = 0;
            }

            if (paddle.y + height > screenHeight) {
                paddle.y = screenHeight - height;
            }
        }
};

class Computer : public Paddle {
    public:
        int width;
        int height;
        int posX;
        int posY;
        int speedY = 6;
        Rectangle cpu;

        Computer() {
            width = 20;
            height = 120;
            posX = 10;
            posY = screenHeight / 2 - height / 2;
        
            cpu = Rectangle{(float)posX, (float)posY, (float)width, (float)height};
        }

        void Update(int ballY) {
            Draw(cpu);

            if (cpu.y < ballY) {
                cpu.y += speedY;
            } else if (cpu.y > ballY) {
                cpu.y -= speedY;
            }
        }
};

Ball ball;
Paddle player;
Computer cpu;

int main() {
    InitWindow(screenWidth, screenHeight, "Pong CPP");
    SetTargetFPS(60);

    while (WindowShouldClose() == false) {
        BeginDrawing();

        ClearBackground(green);

        DrawRectangle(screenWidth / 2, 0, screenWidth / 2 + 27, screenHeight, darkGreen);

        DrawText(TextFormat("%i", playerScore), screenWidth / 4 * 3, 25, 70, WHITE);
        DrawText(TextFormat("%i", computerScore), screenWidth / 4, 25, 70, WHITE);

        ball.Update();
        player.Update();
        cpu.Update(ball.posY);

        if (CheckCollisionCircleRec(Vector2{(float)ball.posX, (float)ball.posY}, ball.radius, cpu.cpu)) {
            ball.ballSpeedX *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{(float)ball.posX, (float)ball.posY}, ball.radius, player.paddle)) {
            ball.ballSpeedX *= -1;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}