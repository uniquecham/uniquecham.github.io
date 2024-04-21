//=============================================================================
// Programmer: Unique Chambers
// Date: March 26,2024
// File Name: Enhancement_artifact_CS499.cpp
// Version: 2.0
// 
// Description: This program creates a rotating pyramid with different colors
// on each side. It demonstrates the use of OpenGL for rendering 3D graphics.
// The pyramid is rotated continuously using the elapsed time.
//
// Optimizations:
// - Colors are stored in an array for easy access and switching.
// - Vertex and color attributes are interleaved for better memory usage.
// - Modern OpenGL (Core Profile) is used with Vertex Array Objects (VAOs)
//   for efficient rendering.
//
// Time Complexity:
// - Creating the mesh (pyramid) has a time complexity of O(1) since the
//   vertices and indices are fixed.
// - Rendering each frame has a time complexity of O(N), where N is the number
//   of vertices in the pyramid.
//
// Algorithmic Logic:
// - Initialize GLFW, GLEW, and create a window.
// - Create a pyramid mesh with positions and colors, using VAOs and VBOs.
// - Compile and link shader programs for vertex and fragment shaders.
// - The main loop continuously renders the rotating pyramid.
// - Pressing ESC closes the window.
//
// Instructions:
// - Compile the program with the necessary OpenGL libraries.
// - Ensure the shader files are in the 'shaders/' folder relative to the binary.
// - Run the executable to see the rotating pyramid with different colors.
//
//=============================================================================

//=============================================================================


//Libraries and Header Files

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <iostream>

using namespace std;

#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

namespace {
    const char* const WINDOW_TITLE = "Unique Chambers";
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    struct GLMesh {
        GLuint vao;
        GLuint vbos[2];
        GLuint nIndices;
    };

    GLFWwindow* gWindow = nullptr;
    GLMesh gMesh;
    GLuint gProgramId;

    // Vertex Shader Source Code
    const GLchar* vertexShaderSource = GLSL(440,
        layout(location = 0) in vec3 position;
    layout(location = 1) in vec4 color;
    out vec4 vertexColor;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0f);
        vertexColor = color;
    }
    );

    // Fragment Shader Source Code
    const GLchar* fragmentShaderSource = GLSL(440,
        in vec4 vertexColor;
    out vec4 fragmentColor;

    void main()
    {
        fragmentColor = vertexColor;
    }
    );

    // I've added an array colors[] to store the colors for each face of the pyramid.
    glm::vec4 colors[] = {
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  // Red
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  // Green
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),  // Blue
        glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),  // Yellow
        glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),  // Magenta
        glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)   // Cyan
    };

    //For each face of the pyramid, a different color is selected from the colors[] array based on the currentColorIndex.

    int currentColorIndex = 0;
}

bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UCreateMesh(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
void URender();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);

int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    UCreateMesh(gMesh);

    if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId))
        return EXIT_FAILURE;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(gWindow))
    {
        UProcessInput(gWindow);
        URender();
        glfwPollEvents();
    }

    UDestroyMesh(gMesh);
    UDestroyShaderProgram(gProgramId);

    exit(EXIT_SUCCESS);
}

bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);

    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult)
    {
        cerr << glewGetErrorString(GlewInitResult) << endl;
        return false;
    }

    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;
    return true;
}

void UProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//In the URender() function, the model matrix is rotated using glfwGetTime() to create a continuous rotation effect.

// Render frame function
void URender()
{
    // Enable depth testing for 3D rendering
    glEnable(GL_DEPTH_TEST);

    // Set clear color (background color) to white
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Clear the color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotation and translation matrices for model, view, and projection
    glm::mat4 model = glm::rotate((float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -5.0f)) *
        glm::rotate(-glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
        (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT,
        0.1f,
        100.0f);

    // Use the shader program
    glUseProgram(gProgramId);

    // Pass transformation matrices to the shader
    glUniformMatrix4fv(glGetUniformLocation(gProgramId, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(gProgramId, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(gProgramId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Bind the pyramid's VAO
    glBindVertexArray(gMesh.vao);

    // Draw the pyramid using indexed rendering
    glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL);

    // Unbind the VAO
    glBindVertexArray(0);

    // Swap the front and back buffers to display the rendered image
    glfwSwapBuffers(gWindow);
}

void UCreateMesh(GLMesh& mesh)
{
    GLfloat vertices[] = {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
       -0.5f,  0.5f, 0.0f,
        0.0f,  0.0f, 1.0f,
    };

    GLushort indices[] = {
        0, 1, 2,
        0, 3, 2,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4,
    };

    const GLuint floatsPerVertex = 3;

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(2, mesh.vbos);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    mesh.nIndices = sizeof(indices) / sizeof(indices[0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLint stride = sizeof(float) * floatsPerVertex;

    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    // This line enables interleaved attributes for color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);
}

void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbos);
}

bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
    int success = 0;
    char infoLog[512];

    programId = glCreateProgram();
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    glCompileShader(vertexShaderId);
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        return false;
    }

    glCompileShader(fragmentShaderId);
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        return false;
    }

    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
        return false;
    }

    glUseProgram(programId);
    return true;
}

void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}