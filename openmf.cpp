#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec4 aColor;\n"
    "out vec4 vertexColor;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos, 1.0);\n"
    " vertexColor = aColor;\n"
    "}\n";

const char *fragmentShaderSource =
    "#version 330 core\n"
    "in vec4 vertexColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    " FragColor = vertexColor;\n"
    "}\n";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
  if (!glfwInit())
    return -1;

  int success;
  char infoLog[512];

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  // Set framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize OpenGL context\n";
    return -1;
  }

  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

  unsigned int vertexShader, fragmentShader;
  unsigned int shaderProgram;
  unsigned int VBO, VAO, CBO, EBO;
  int width, height;

  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);
  glGenBuffers(1, &CBO);

  shaderProgram = glCreateProgram();

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR" << infoLog << std::endl;
  }

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR" << infoLog << std::endl;
  }

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::" << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float vertices_for_f[] = {
    -0.9f, -0.3f, 0.0f, // 0 - Bottom-left-of-f_rectangle
    -0.8f, -0.3f, 0.0f, // 1 - Bottom-right-of-f_rectangle
    -0.8f,  0.9f, 0.0f, // 2 - Top-right-of-f_rectangle
    -0.9f,  0.9f, 0.0f, // 3 -Top-left-of-f_rectangle
     0.1f,  0.9f, 0.0f, // 4 -Top-right-of-f_first_mouth
     0.1f,  0.8f, 0.0f, // 5 -Bottom-right-of-f_first_mouth
    -0.8f,  0.8f, 0.0f, // 6 - Bottom-left-of-f_first_mouth
    -0.8f,  0.5f, 0.0f, // 7 - Bottom-left-of-f_second_mouth
     0.1f,  0.6f, 0.0f, // 8 -Top-right-of-f_second_mouth
    -0.8f,  0.6f, 0.0f, // 9 - Top-left-of-f_second_mouth
     0.1f,  0.5f, 0.0f  // 10 -Bottom-right-of-f_second_mouth
  };

  float colors[] = {
    1.0f, 0.0f, 0.0f, 1.0f, // 0
    0.0f, 1.0f, 0.0f, 1.0f, // 1
    0.0f, 0.0f, 1.0f, 1.0f, // 2
    1.0f, 1.0f, 0.0f, 1.0f, // 3
    1.0f, 0.0f, 1.0f, 1.0f, // 4
    0.0f, 1.0f, 1.0f, 1.0f, // 5
    1.0f, 1.0f, 1.0f, 1.0f, // 6
    1.0f, 0.5f, 0.0f, 1.0f, // 7
    0.5f, 1.0f, 0.0f, 1.0f, // 8
    0.0f, 0.5f, 1.0f, 1.0f, // 9
    0.5f, 0.0f, 1.0f, 1.0f  // 10
  };

  unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3,
    2, 4, 5, // First Mouth- first traingle
    2, 6, 5, // First Mouth- second triangle
    9, 8, 10, // Second Mouth- first triangle
    7, 10, 9  // Second Mouth- second triangle
  };

  // Bind VAO

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_for_f), vertices_for_f, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, CBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Set initial viewport size
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  while (!glfwWindowShouldClose(window)) {

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
