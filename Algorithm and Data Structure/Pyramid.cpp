// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// File:        Pyramid.cpp
// Programmer Name:      Unique Chambers
// Date:        March 31, 2024
// Version:     1.0
// Description: OpenGL program to render a simple colored triangle.
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                 
//                                                                    Narrative 
//   The artifact that I chose was a assignment from class CS 330 that I had to create a 3D pyramid, I made it a solid red color. I chose to enhance this artifact because it aligned with
// the course outcome for algorithms and data structure. The skills that I used was linear algebra concepts, particularly matrix transformations. I used the following linear algebra concepts, 
// matrix multiplication and matrix operations. My skills in C++ programming was displayed, by me using structs and functions, to define structures (GLMesh) to organize data and functions
// (URender, UCreateMesh, etc.) to manage rendering.With using these skills I was able to meet the course outcome from Module One. The artifact was improved by making changing the color from a solid 
// color red to each side being a different color and making the pyramid rotate on the y-axis. The course outcome that was met was, I designed, developed, and delivered a professional-quality 
// oral, written, and visual communications that are coherent, technically sound, and appropriately adapted to specific audiences and contexts. I also was able to demonstrate an ability to use 
// well-founded and innovative techniques, skills, and tools in computing practices for the purpose of implementing computer solutions that deliver value and accomplish industry-specific goals 
// (software engineering/design/database). The remaining that I would like to improve on is developing a security mindset that anticipates adversarial exploits in software architecture and designs to 
// expose potential vulnerabilities, mitigate design flaws, and ensure privacy and enhanced security of data and resources.I did not face any challenges on the enhancement of this artifact. As I was
// enhancing the artifact, I learned that in order to make the pyramid to rotate, it needs to rotate on the y-axis. 
//  
// 
// 
// 
// 
// 
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


#include <iostream>        
#include <GL/glew.h>        
#include <GLFW/glfw3.h>     
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Unnamed namespace
namespace
{
    const char* const WINDOW_TITLE = "Unique Chambers"; // Window title
    // Variables for window width and height
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    // Stores the GL data relative to a given mesh
    struct GLMesh
    {
        GLuint vao;
        GLuint vbos[2];
        GLuint nIndices;
    };
    // Main GLFW window
    GLFWwindow* gWindow = nullptr;
    // Triangle mesh data
    GLMesh gMesh;
    // Shader program
    GLuint gProgramId;
}
/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UCreateMesh(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
void URender();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);

/* Vertex Shader Source Code*/
const GLchar* vertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
out vec4 vertexColor;
//Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexColor = color; // references incoming color data
}
);
// Fragment Shader Source Code
const GLchar* fragmentShaderSource = GLSL(440,
    in vec4 vertexColor;
out vec4 fragmentColor;
void main()
{   // Set the output color to be the same as the input color
    fragmentColor = vec4(vertexColor);
}
);
// Main
int main(int argc, char* argv[])
{   // Initialize GLFW, GLEW, and create a window
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;
    // Create the mesh
    UCreateMesh(gMesh);
    // Create the shader program
    if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId))
        return EXIT_FAILURE;
    // Set the background color of the window to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Render Loop
    while (!glfwWindowShouldClose(gWindow))
    {
        // Input
        UProcessInput(gWindow);
        // Render frame
        URender();
        glfwPollEvents();
    }
    // Release mesh
    UDestroyMesh(gMesh);
    // Release shader program
    UDestroyShaderProgram(gProgramId);
    // Exits Program
    exit(EXIT_SUCCESS);
}
// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // GLFW: window creation
    *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return false;
    }
    // Make the newly created window the current OpenGL context
    glfwMakeContextCurrent(*window);
    // Set the framebuffer size callback function to handle window resizing
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    // Enable GLEW experimental mode
    glewExperimental = GL_TRUE;
    // Initialize GLEW and store the initialization result
    GLenum GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult)
    {
        cerr << glewGetErrorString(GlewInitResult) << endl;
        return false;
    }
    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;
    return true;
}
// Process input
void UProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
// Window resize
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
// Render frame function
void URender()
{
    // Enable z-depth, set clear color, and clear buffers
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotation speed
    static float rotateAngle = 0.0f;
    rotateAngle += 0.01f;


    // Transformation matrices
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), rotateAngle, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y axis
    model = glm::scale(model, glm::vec3(1.5f)); // Scale the pyramid

    glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -5.0f)); // Camera position
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f); // Perspective projection

    // Use shader program and pass matrices
    glUseProgram(gProgramId);
    glUniformMatrix4fv(glGetUniformLocation(gProgramId, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(gProgramId, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(gProgramId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    // Bind VAO, draw elements, unbind VAO, swap buffers
    glBindVertexArray(gMesh.vao);
    glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);
    glfwSwapBuffers(gWindow);
}
// Implements the UCreateMesh function
void UCreateMesh(GLMesh& mesh)
{
    // Position and Color data
    GLfloat vertices[] = {
        // Vertex Positions    // Colors 
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f, // Top Right, Red
         0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f, // Bottom Right, Yellow
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f, 1.0f,  // Bottom Left, Cyan
        -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,  // Top Left, Blue
         0.0f,  0.0f, 1.0f,   1.0f, 0.0f, 1.0f, 1.0f, // Center, Purple
    };
    // Index data to share position data
    GLushort indices[] = {
        0, 1, 2,
        0, 3, 2,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4,
    };
    // Define the number of floats per vertex and color
    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerColor = 4;
    // Generate a single Vertex Array Object (VAO) and bind it for use
    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);
    // Generate two buffer objects
    glGenBuffers(2, mesh.vbos);
    // Bind the first buffer object (VBO) for vertex data and send vertex coordinates to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Calculate the number of indices in the mesh
    mesh.nIndices = sizeof(indices) / sizeof(indices[0]);
    // Bind the second buffer object (VBO) for indices and send index data to the GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Calculate the stride between vertex attributes in bytes
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor);
    // Set up vertex attribute pointers for position data
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);
    // Set up vertex attribute pointers for color data
    glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);
}
// Delete mesh's Vertex Array Object and Vertex Buffer Objects
void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbos);
}
// Create a shader program and check for success
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
    int success = 0;
    char infoLog[512];
    // Create a shader program object
    programId = glCreateProgram();
    // Create vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    // Load shader source code into the shader objects
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);
    // Compile and check compilation status of the vertex shader
    glCompileShader(vertexShaderId);
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        return false;
    }
    // Compile and check the compilation status of the fragment shader
    glCompileShader(fragmentShaderId);
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        return false;
    }
    // Attach the vertex and fragment shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    // Link and check link status of shader program
    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
        return false;
    }
    // Uses the shader program
    glUseProgram(programId);
    return true;
}
void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}