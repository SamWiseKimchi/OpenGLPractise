//
//  main.cpp
//  OpenGLPractise
//
//  Created by Se Yeun Kim on 15/7/17.
//  Copyright © 2017 Se Yeun Kim. All rights reserved.
//

#include <iostream>
#include <math.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

const GLchar *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main() {\n"
"gl_Position = vec4(aPos, 1.0);\n"
"ourColor = aColor;\n"
"}";

const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main() {\n"
"FragColor = vec4(ourColor, 1.0f);\n"
"}";

int main() {

  // instantiate the GLFW window
  // init GLFW
  glfwInit();

  /* configure GLFW using 'glfwWindowHint'
   first arg tells us what option we want to config (from a large list of enums
   second arg is an integer that sets the value of our option
   tells GLFW to use the 3.3 (major.minor) as the version of OpenGL
   */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // To make MacOS happy. window == NULL without this. reasons unknown.
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // telling GLFW explicitly that we want to use the core-profile means we'll get access to a smaller subset of OpenGL features
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /*
   create a window object
   holds all the windowing data and is used quite frequently by GLFW's other functions
   */
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGLTest", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Needed in core profile
  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }
  
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "Failed to compile vertex Shader" << infoLog << std::endl;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  // takes in the string code above :S this is weird
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "Failed to compile Fragment Shader" << infoLog << std::endl;
  }

  // link in the shaders
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // check for any linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "Failed to link Shaders via shader program" << infoLog << std::endl;
  }

  // since we're no longer using the shaders as they are part of our shader program, we delete the vert/frag shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // MAKING A TRIANGLE
  /*
   Screen Co-ordinates
   X - right
   Y - up
   Z - outwards (from screen)
   An array of 3 vectors which represents 3 vertices
   */
  /*
  static const GLfloat g_vertex_buffer_data[] = {
    -0.5f,  -0.5f,  0.0f,
    0.5f,   -0.5f,  0.0f,
    0.0f,   0.5f,   0.0f
  };
   */
  static const GLfloat g_vertex_buffer_data[] = {
    0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom left
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f   // top left
  };
  unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
  };

  // This will identify our vertex buffer
  GLuint vertexBufferObject;
  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &vertexBufferObject);
  // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  // Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  GLuint vertexArrayObject;
  glGenVertexArrays(1, &vertexArrayObject);
  glBindVertexArray(vertexArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexArrayObject);
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) ;

  /*
   As of now we stored the vertex data within memory on the graphics card as managed by a vertex buffer
   object named vertexbuffer. Next we want to create a vertex and fragment shader that actually processes this data
   Now we start building those.
   */

  // VERTEX SHADING
  do {

    // check if any events have been activated, i.e. poll IO events
    glfwPollEvents();

    // render
    // clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // now render the shape
    glBindVertexArray(vertexArrayObject);
//    glDrawArrays(GL_TRIANGLES, 0, 3); //
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Swap buffers
    glfwSwapBuffers(window);

  } // Check if the ESC key was pressed or the window was closed
  while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

  glDeleteVertexArrays(1, &vertexArrayObject);
  glDeleteVertexArrays(1, &vertexBufferObject);

  glfwTerminate();

  return 0;
}
