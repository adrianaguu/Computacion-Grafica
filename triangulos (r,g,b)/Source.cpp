#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
///Cambio de colores en triangulos
////Para el primer triangulo usar Q para red, W para blue y E para green, la intensidad cambia con O para arriba y L para abajo
////Para el segundo triangulo usar A para red, S para blue y D para green, la intensidad cambia con I para arriba y K para abajo


float r1, g1, b1;
float r2, g2, b2;

float intensity1 = 1;
float intensity2 = 1;

float inc = 0.1;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";
const char* fragmentShader1Source = "#version 330 core\n"
"out vec4 FragColor1;\n"
"uniform vec4 ourColor1;\n"
"void main()\n"
"{\n"
"   FragColor1 = ourColor1;\n"
"}\n\0";


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //for (int i=0;i<30;i++)
    //    std::cout << fragmentShader1Source[72+i];
    /*td::cout << fragmentShader1Source[72];
    std::cout << fragmentShader1Source[74];
    std::cout << '\n';
    std::cout << fragmentShader1Source[78];
    std::cout << fragmentShader1Source[80];
    std::cout << '\n';
    std::cout << fragmentShader1Source[84];
    std::cout << fragmentShader1Source[86];*/


    // build and compile our shader program
    // ------------------------------------
    // we skipped compile log checks this time for readability (if you do encounter issues, add the compile-checks! see previous code samples)
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER); // the first fragment shader that outputs the color orange
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER); // the second fragment shader that outputs the color yellow
    unsigned int shaderProgramOrange = glCreateProgram();
    unsigned int shaderProgramYellow = glCreateProgram(); // the second shader program
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaderOrange);
    glShaderSource(fragmentShaderYellow, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderYellow);
    // link the first program object
    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, fragmentShaderOrange);
    glLinkProgram(shaderProgramOrange);
    // then link the second program object using a different fragment shader (but same vertex shader)
    // this is perfectly allowed since the inputs and outputs of both the vertex and fragment shaders are equally matched.
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
    };
    float secondTriangle[] = {
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top 
    };
    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(2, VBOs);
    // first triangle setup
    // --------------------
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines
    // second triangle setup
    // ---------------------
    glBindVertexArray(VAOs[1]);	// note that we bind to a different VAO now
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// and a different VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // not really necessary as well, but beware of calls that could affect VAOs while this one is bound (like binding element buffer objects, or enabling/disabling vertex attributes)


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {



        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // now when we draw the triangle we first use the vertex and orange fragment shader from the first program
        glUseProgram(shaderProgramOrange);
        // draw the first triangle using the data from our first VAO
        int vertexColorLocation = glGetUniformLocation(shaderProgramOrange, "ourColor");
        glUniform4f(vertexColorLocation, r1 * intensity1, g1 * intensity1, b1 * intensity1, 1.0f);

        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);	// this call should output an orange triangle
        // then we draw the second triangle using the data from the second VAO
        // when we draw the second triangle we want to use a different shader program so we switch to the shader program with our yellow fragment shader.
        glUseProgram(shaderProgramYellow);

        int vertexColorLocation1 = glGetUniformLocation(shaderProgramYellow, "ourColor1");
        glUniform4f(vertexColorLocation1, r2 * intensity2, g2 * intensity2, b2 * intensity2, 1.0f);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);	// this call should output a yellow triangle

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        if (r1 == 0)
            r1 = 1;
        else
            r1 = 0;
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        if (b1 == 0)
            b1 = 1;
        else
            b1 = 0;
    }

    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        if (g1 == 0)
            g1 = 1;
        else
            g1 = 0;
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        if (r2 == 0)
            r2 = 1;
        else
            r2 = 0;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        if (b2 == 0)
            b2 = 1;
        else
            b2 = 0;
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        if (g2 == 0)
            g2 = 1;
        else
            g2 = 0;
    }


    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        if (intensity1 != 1)
            intensity1 += inc;
    }

    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (intensity1 != 0)
            intensity1 -= inc;

    }

    if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {
        if (intensity2 != 1)
            intensity2 += inc;
    }

    if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        if (intensity2 != 0)
            intensity2 -= inc;

    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}