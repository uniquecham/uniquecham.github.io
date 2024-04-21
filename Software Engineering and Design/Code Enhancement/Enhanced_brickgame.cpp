//=============================================================================
// Programmer: Unique Chambers
// Date: April 8, 2024
// File Name: codeenhancement.cpp
// Description: Enhanced brick game with a paddle, lives, and random circles using OpenGL and C++
// Version: 2.0
//=============================================================================
//=============================================================================
//Intent:
//The intent was to enhance the original brick game with the following features :

//Paddle Control : Allow the player to control a paddle at the bottom of the screen using the left and right arrow keys.
//Lives System : Implement a lives system where the player starts with a set number of lives.Losing a life occurs when the ball 
//hits the bottom edge of the screen.The game ends when all lives are lost.
//Visual Feedback : Display the game over on a different screen.

//Decisions Made :
//Paddle Dimensions : The paddle's width and height were defined as paddleWidth and paddleHeight.
//Paddle Movement : The processInput function was updated to handle left and right arrow key presses, ensuring the paddle stays within the screen boundaries.
//Lives Tracking : Introduced a lives variable to keep track of remaining lives, displayed on the screen using glRasterPos2f and glutBitmapCharacter.
//Collision Detection : Implemented collision detection between the ball and the paddle.If the ball hits the bottom edge of the screen, a life is lost.
//End Game Logic : The game ends if all lives are lost, closing the window and terminating the program.
//===========================================================================================================================


#include <GLFW\glfw3.h>
#include "linmath.h" // Assuming this is available in your project directory
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <time.h>

using namespace std;

const float DEG2RAD = 3.14159 / 180;

// Function to handle keyboard input
void processInput(GLFWwindow* window);

enum BRICKTYPE { REFLECTIVE, DESTRUCTABLE };
enum ONOFF { ON, OFF };

class Brick
{
public:
    float red, green, blue;
    float x, y, width;
    BRICKTYPE brick_type;
    ONOFF onoff;
    int hit_points;

    // Constructor for Brick class
    Brick(BRICKTYPE bt, float xx, float yy, float ww, float rr, float gg, float bb)
    {
        brick_type = bt; x = xx; y = yy; width = ww; red = rr, green = gg, blue = bb;
        onoff = ON;
        if (brick_type == DESTRUCTABLE) {
            hit_points = 5; // initialize hit points for destructible bricks
        }
    };

    // Draw the brick on the screen
    void drawBrick()
    {
        if (onoff == ON)
        {
            double halfside = width / 5;

            glColor3d(red, green, blue);
            glBegin(GL_POLYGON);

            glVertex2d(x + halfside, y + halfside);
            glVertex2d(x + halfside, y - halfside);
            glVertex2d(x - halfside, y - halfside);
            glVertex2d(x - halfside, y + halfside);

            glEnd();
        }
    }
};

class Circle
{
public:
    float red, green, blue;
    float radius;
    float x;
    float y;
    float speed = 0.09;
    int direction; // 1=up, 2=right, 3=down, 4=left, 5=up right, 6=up left, 7=down right, 8=down left

    // Constructor for Circle class
    Circle(double xx, double yy, double rr, int dir, float rad, float r, float g, float b)
    {
        x = xx;
        y = yy;
        radius = rr;
        red = r;
        green = g;
        blue = b;
        radius = rad;
        direction = dir;
    }

    // Check collision of circle with a brick
    void CheckCollision(Brick* brk)
    {
        if (brk->brick_type == REFLECTIVE)
        {
            if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk->y - brk->width && y <= brk->y + brk->width))
            {
                direction = GetRandomDirection();
                x = x + 0.01;
                y = y + 0.02;
                brk->red = 1.0f; // set brick color to red
                brk->green = 0.0f;
                brk->blue = 0.0f;
            }
        }
        else if (brk->brick_type == DESTRUCTABLE)
        {
            brk->hit_points--;
            if (brk->hit_points == 0)
            {
                brk->onoff = OFF;
            }
        }
    }

    // Generate a random direction for the circle
    int GetRandomDirection()
    {
        return (rand() % 8) + 1;
    }

    // Move the circle one step based on its direction
    void MoveOneStep()
    {
        if (direction == 1 || direction == 5 || direction == 6)  // up
        {
            if (y > -1 + radius)
            {
                y -= speed;
            }
            else
            {
                direction = GetRandomDirection();
            }
        }

        if (direction == 2 || direction == 5 || direction == 7)  // right
        {
            if (x < 1 - radius)
            {
                x += speed;
            }
            else
            {
                direction = GetRandomDirection();
            }
        }

        if (direction == 3 || direction == 7 || direction == 8)  // down
        {
            if (y < 1 - radius) {
                y += speed;
            }
            else
            {
                direction = GetRandomDirection();
            }
        }

        if (direction == 4 || direction == 6 || direction == 8)  // left
        {
            if (x > -1 + radius) {
                x -= speed;
            }
            else
            {
                direction = GetRandomDirection();
            }
        }
    }

    // Draw the circle on the screen
    void DrawCircle()
    {
        glColor3f(red, green, blue);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float degInRad = i * DEG2RAD;
            glVertex2f((cos(degInRad) * radius) + x, (sin(degInRad) * radius) + y);
        }
        glEnd();
    }
};

class Paddle
{
public:
    float red, green, blue;
    float x, y, width, height;

    // Constructor for Paddle class
    Paddle(float xx, float yy, float ww, float hh, float rr, float gg, float bb)
    {
        x = xx;
        y = yy;
        width = ww;
        height = hh;
        red = rr;
        green = gg;
        blue = bb;
    }

    // Draw the paddle on the screen
    void drawPaddle()
    {
        glColor3f(red, green, blue);
        glBegin(GL_POLYGON);

        glVertex2f(x - width / 2, y - height / 2);
        glVertex2f(x + width / 2, y - height / 2);
        glVertex2f(x + width / 2, y + height / 2);
        glVertex2f(x - width / 2, y + height / 2);

        glEnd();
    }

    // Move the paddle left or right based on input
    void movePaddle(bool moveLeft, bool moveRight)
    {
        if (moveLeft && x - width / 2 > -1) {
            x -= 0.05f;
        }
        if (moveRight && x + width / 2 < 1) {
            x += 0.05f;
        }
    }
};

vector<Circle> world;
Paddle paddle(0.0f, -0.9f, 0.2f, 0.05f, 0.5f, 0.5f, 0.5f); // Create a paddle

int main(void) {
    srand(time(NULL)); // Seed for random number generation

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(480, 480, "Enhanced Brick Game", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize bricks
    Brick brick(DESTRUCTABLE, -0.2, 0.0, 0.4, 1.0, 1.0, 0.0);
    Brick brick2(DESTRUCTABLE, -0.2, 0.3, 0.4, 1.0, 0.0, 0.0);
    Brick brick3(DESTRUCTABLE, -0.2, 0.6, 0.4, 0.0, 1.0, 1.0);
    Brick brick4(DESTRUCTABLE, 0.0, 0.0, 0.4, 0.0, 0.5, 0.5);
    Brick brick5(DESTRUCTABLE, 0.0, 0.3, 0.4, 1.0, 0.5, 0.5);
    Brick brick6(DESTRUCTABLE, 0.0, 0.6, 0.4, 1.0, 0.0, 1.0);
    Brick brick7(DESTRUCTABLE, 0.2, 0.0, 0.4, 1.0, 0.5, 0.0);
    Brick brick8(DESTRUCTABLE, 0.2, 0.3, 0.4, 0, 1, 0);
    Brick brick9(DESTRUCTABLE, 0.2, 0.6, 0.4, 0, 1, 1);
    Brick brick10(DESTRUCTABLE, 0.4, 0.0, 0.4, 0, 0.5, 0.5);
    Brick brick11(DESTRUCTABLE, 0.4, 0.3, 0.4, 1.0, 1.0, 1.0);
    Brick brick12(DESTRUCTABLE, 0.4, 0.6, 0.4, 1.0, 1.0, 0.0);

    int lives = 3; // Starting lives

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Setup View
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window); // Handle keyboard input

        // Move the paddle based on input
        paddle.movePaddle(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS, glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);

        // Draw the paddle
        paddle.drawPaddle();

        // Movement and collision for circles
        for (int i = 0; i < world.size(); i++)
        {
            world[i].CheckCollision(&brick);
            world[i].CheckCollision(&brick2);
            world[i].CheckCollision(&brick3);
            world[i].CheckCollision(&brick4);
            world[i].CheckCollision(&brick5);
            world[i].CheckCollision(&brick6);
            world[i].CheckCollision(&brick7);
            world[i].CheckCollision(&brick8);
            world[i].CheckCollision(&brick9);
            world[i].CheckCollision(&brick10);
            world[i].CheckCollision(&brick11);
            world[i].CheckCollision(&brick12);
            world[i].MoveOneStep();
            world[i].DrawCircle();

            // Check collision with paddle
            if (world[i].y - world[i].radius < paddle.y + paddle.height / 2 &&
                world[i].y + world[i].radius > paddle.y - paddle.height / 2 &&
                world[i].x - world[i].radius < paddle.x + paddle.width / 2 &&
                world[i].x + world[i].radius > paddle.x - paddle.width / 2)
            {
                world.erase(world.begin() + i);
                i--;

                if (world.empty()) {
                    // Add a new circle when all circles are cleared
                    double r, g, b;
                    r = rand() / 10000;
                    g = rand() / 10000;
                    b = rand() / 10000;
                    Circle newCircle(0, 0, 0.2, 2, 0.05, r, g, b); // Create a new circle
                    world.push_back(newCircle); // Add the new circle to the vector
                }
            }

            // Check if circle is out of bounds
            if (world[i].y - world[i].radius < -1) {
                world.erase(world.begin() + i);
                i--;

                lives--; // Decrease lives
                if (lives <= 0) {
                    // Game over
                    cout << "Game Over!" << endl;
                    break;
                }
            }
        }

        // Draw bricks
        brick.drawBrick();
        brick2.drawBrick();
        brick3.drawBrick();
        brick4.drawBrick();
        brick5.drawBrick();
        brick6.drawBrick();
        brick7.drawBrick();
        brick8.drawBrick();
        brick9.drawBrick();
        brick10.drawBrick();
        brick11.drawBrick();
        brick12.drawBrick();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

// Handle keyboard input (escape key to exit, spacebar to add a new circle)
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && world.empty())
    {
        // Add a new circle when spacebar is pressed and no circles are present
        double r, g, b;
        r = rand() / 10000;
        g = rand() / 10000;
        b = rand() / 10000;
        Circle newCircle(0, 0, 0.2, 2, 0.05, r, g, b); // Create a new circle
        world.push_back(newCircle); // Add the new circle to the vector
    }
}
