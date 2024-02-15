#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.hpp"

// SHADERS
    //OBJ SHADERS
    const char* VertShader = 
    "#version 460 core\n" 
    "\n" 
    "layout(location = 0) in vec3 aPos;\n" 
    "\n"
    " uniform mat4 model;"
    " uniform mat4 view;"
    " uniform mat4 proj;"

    "void main()\n" 
    "{\n" 
    "  gl_Position = proj * view * model * vec4(aPos, 1.0f);"
    "}\n\0";

    const char*  FragShader = 
    "#version 460 core\n" 
    "\n" 
    "out vec4 FragColor;\n" 
    "\n"
    " uniform vec3 objectcolor;"
    " uniform vec3 lightcolor;"  

    "void main()\n" 
    "{\n" 
    "  FragColor = vec4(lightcolor * objectcolor, 1.0f);\n"
    "}\n\0";

    const char*  lFragShader = 
    "#version 460 core\n" 
    "\n" 
    "out vec4 FragColor;\n"
    "\n"
    "void main()\n" 
    "{\n" 
    "  FragColor = vec4(1.0f);\n"
    "}\n\0";
// START OF CAMERA PROTOTYPES
/* void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void ScrollCallback( GLFWwindow *window, double xOffset, double yOffset );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( ); */

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = 800 / 2.0;
GLfloat lastY = 800 / 2.0f;

// COMPILE FUNCTION
static GLuint CompileShader(GLuint type, const std::string& source)
{
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id , 1 , &src , nullptr);
    glCompileShader(id);
    return id;                                                                                                                                                                                                                                                      
}
/* static GLuint CreateShader(const std::string& VertShader, const std::string& FragShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER , VertShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER , FragShader);
   
    

    glAttachShader(program,vs);
    glAttachShader(program,fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
} */
// LIGHT POSITION
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main(void){
// INITIALIZE
    glfwInit();



// HINTS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

// WINDOW
    GLFWwindow* window = glfwCreateWindow(800,800,"First", NULL , NULL);

 

    glfwMakeContextCurrent(window);
/* 
    glfwSetKeyCallback(window,KeyCallback);
    glfwSetCursorPosCallback(window,MouseCallback);
    glfwSetScrollCallback(window,ScrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); */

    gladLoadGL();

//BUFFERS
    GLfloat vertices [] = {
       -0.25f,  0.0f,  0.25f, //0
       -0.25f,  0.0f, -0.25f, //1
        0.25f,  0.0f, -0.25f, //2
        0.25f,  0.0f,  0.25f, //3
       -0.25f,  0.5f,  0.25f, //4
       -0.25f,  0.5f, -0.25f, //5
        0.25f,  0.5f, -0.25f, //6
        0.25f,  0.5f,  0.25f  //7

    };
    glm::vec3 CubePositions [] = 
    {
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(2.0f,5.0f,-15.0f),
        glm::vec3(-1.5f,-2.2f,-2.5f),
        glm::vec3(-3.8f,2.0f,-12.3f),
        glm::vec3(-1.7f,3.0f,-7.5f),
        glm::vec3(1.5f,-2.0f,-2.5f)
    };
    GLuint indices [] =  {
       0, 1, 2,
       0, 2, 3,
       0, 1, 4,
       0, 3, 4,
       1, 4, 5,
       1, 6, 5,
       2, 3, 6,
       1, 3, 6,
       2, 4, 7,
       3, 7, 6
    };
    GLuint vs = CompileShader(GL_VERTEX_SHADER, VertShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, FragShader);
    GLuint lfs = CompileShader(GL_FRAGMENT_SHADER, lFragShader);
    GLuint shader = glCreateProgram();
    glAttachShader(shader,vs);
    glAttachShader(shader,fs);
    glLinkProgram(shader);
    glValidateProgram(shader);  
    GLuint lightshader = glCreateProgram();
    glAttachShader(lightshader, vs);
    glAttachShader(lightshader, lfs);
    glLinkProgram(lightshader);
    glValidateProgram(lightshader);
    GLuint EBO,VBO,VAO,L_VAO;
    glGenVertexArrays(1 , &VAO);
    glGenBuffers(1 , &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
// VAO| VBO | EBO
    glBindBuffer(GL_ARRAY_BUFFER , VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * sizeof(vertices) , vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER , sizeof(indices) , indices , GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , sizeof(GLfloat) * 3, (void*)0);
// LIGHT SOURCE
    glGenVertexArrays(1 , &L_VAO);
    glBindVertexArray(L_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , EBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 , 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
// ROTATION FOR MODEL
    float rotation = 0.0f;
    double deltaTime = glfwGetTime();
// RENDER LOOP
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f , 0.0f , 0.0f , 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);

        double crntTime = glfwGetTime();
        if(crntTime - deltaTime >= 1 / 60){
            rotation += 0.005f;
            deltaTime = crntTime;
        }
    // VertShader UNIFORMS
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
        view = camera.GetViewMatrix( );
        proj = glm::perspective(glm::radians(45.0f), (float)(800 / 800), 0.1f , 100.0f);
        int modelLoc = glGetUniformLocation(shader, "model");
        int viewLoc = glGetUniformLocation(shader, "view");
        glUniformMatrix4fv(viewLoc, 1 , GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(shader, "proj");
        glUniformMatrix4fv(projLoc, 1 , GL_FALSE, glm::value_ptr(proj));
    // FRAG SHADER UNIFORMS

        int objectcolors = glGetUniformLocation(shader,"objectcolor");
        glUniform3f(objectcolors, 1.0f, 0.5f, 0.31f);   
        int lightcolors = glGetUniformLocation(shader,"lightcolor");
        glUniform3f(lightcolors, 1.0f, 1.0f, 1.0f);   
        glBindVertexArray(VAO);
        for (GLuint i = 0; i < 7; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, CubePositions[i]);
            GLfloat angle = 20.0f * i;
            model = glm::rotate(model , angle, glm::vec3(1.0f,0.3f,0.5f));
            glUniformMatrix4fv(modelLoc, 1 , GL_FALSE, glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, sizeof(indices),GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);

// LIGHT CUBE
        /* glUseProgram(lightshader);
    // LIGHT CUBE UNIFORMS

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); 
        int lightmodel = glGetUniformLocation(lightshader, "model");
        glUniformMatrix4fv(lightmodel, 1 , GL_FALSE, glm::value_ptr(model));
        int lightview = glGetUniformLocation(shader, "view");
        glUniformMatrix4fv(lightview, 1 , GL_FALSE, glm::value_ptr(view));
        int lightproj = glGetUniformLocation(shader, "proj");
        glUniformMatrix4fv(lightproj, 1 , GL_FALSE, glm::value_ptr(proj) );
        glBindVertexArray(L_VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices),GL_UNSIGNED_INT, 0); */
        //Swap BUFFERS

        glfwSwapBuffers(window);
        
        //TRACK POLL EVENTS
        glfwPollEvents();
        //DoMovement();
    }
    



    // deletion of vertices
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &L_VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    //Terminations
    glfwTerminate();
    return 0;
}