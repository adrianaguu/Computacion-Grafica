#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "matrix.h"


/*TECLAS PARA TRANSFORMAR LA FIGURA

TRANSLATION: 'A'->LEFT, 'D'->RIGHT, 'W'->UP, 'S'->DOWN, change the variarion: '1' to decrease, '2' to increase
SCALE: 'J'->DECREASE WIDTH, 'L'->INCREASE WIDTH, 'I'->INCREASE HEIGHT, 'K'->DECREASE HEIGHT, change the variarion: '3' to decrease, '4' to increase
ROTATION: 'X'->rotate on x axis,'Y'->rotate on y axis, 'Z'->rotate on z axis, 'ENTER'->rotate on all axis, change the angule: 'DOWN' to decrease, 'UP' to increase


*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const int dim_size = 4;
double angule = 45;
float variation_t = 0.25;
float variation_s = 0.25;
int num_triangules = 3;

//Define triangule vertices

vector<float> vertices = {
    // first triangle
     0.75f,  -0.5f, 0.0f,  // top right
     0.25f, 0.5f, 0.0f,  // bottom right
     0.0f,  -0.5f, 0.0f  // top left 
};

vector<float> ivertices = {
    // first triangle
     0.75f,  -0.5f, 0.0f,  // top right
     0.25f, 0.5f, 0.0f,  // bottom right
     0.0f,  -0.5f, 0.0f  // top left 
};



const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


matrix create_m_vertices(vector<float> vertices, int index)
{
    vector<float> m_vertices = vector<float>(vertices.begin() + index, vertices.begin() + index + 3);
    m_vertices.push_back(1.0f);
    return matrix (m_vertices);
}

void apply(matrix m, int num_triangules)
{
    for (int i = 0; i < 9*num_triangules; i+=3)
    {
        matrix m_vertices = create_m_vertices(vertices, i);
        matrix res = m.mult(m_vertices);
       
        vertices[i] = res.m[0][0];
        vertices[i+1] = res.m[1][0];
        vertices[i+2] = res.m[2][0];

        cout << "vertice " << i / 3<<":"<<endl;
        res.print();
    }

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

}

void showmenu()
{
    cout << "Presione R para aplicar la transformacion de la columna R,\nL para la transformacion de la columna L\ne I para volver a la figura inicial" << endl;
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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

    showmenu();


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);


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

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //apply(effect_matrix, 1);
        ///glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_I && action == GLFW_PRESS )
    {
        vertices = ivertices;
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        cout << "Figura reinicializada" << endl<<endl;
    }


    if (key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        vector<float> t_axis = { 0.23f,0.41f ,0.25 };
        matrix T = matrix::GenTranslation(t_axis, dim_size);

        vector<float> s_axis = { 1.25f, 1.0f,1.25f };
        matrix S = matrix::GenScale(s_axis, dim_size);

        cout << "Matrix de translation:" << endl;
        T.print();
        cout << "Matrix de scalation:" << endl;
        S.print();

        matrix RotateX = matrix::GenRotate(53, 'x', dim_size);
        cout << "Matrix de rotation x:" << endl;
        RotateX.print();
        matrix RotateY = matrix::GenRotate(31, 'y', dim_size);
        cout << "Matrix de rotation y:" << endl;
        RotateY.print();
        matrix RotateZ = matrix::GenRotate(69, 'z', dim_size);
        cout << "Matrix de rotation z:" << endl;
        RotateZ.print();


        matrix TRX = T.mult(RotateX);
        cout << "TRx" << endl;
        TRX.print();

        cout << "RyTRx" << endl;
        matrix temp = RotateY.mult(TRX);
        temp.print();

        cout << "SRyTRx" << endl;
        matrix SRyTRx = S.mult(temp);
        SRyTRx.print();

        apply(SRyTRx, 1);
        cout << "Transformacion L aplicada" << endl << endl;
        showmenu();
        
    }

    if (key == GLFW_KEY_R && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        vector<float> t_axis = { 0.23f,0.41f ,0.25 };
        matrix T = matrix::GenTranslation(t_axis, dim_size);

        vector<float> s_axis = { 1.25f, 1.0f,1.25f };
        matrix S = matrix::GenScale(s_axis, dim_size);

        cout << "Matrix de translation:" << endl;
        T.print();
        cout << "Matrix de scalation:" << endl;
        S.print();

        matrix RotateX = matrix::GenRotate(53, 'x', dim_size);
        cout << "Matrix de rotation x:" << endl;
        RotateX.print();
        matrix RotateY = matrix::GenRotate(31, 'y', dim_size);
        cout << "Matrix de rotation y:" << endl;
        RotateY.print();
        matrix RotateZ = matrix::GenRotate(69, 'z', dim_size);
        cout << "Matrix de rotation z:" << endl;
        RotateZ.print();


        matrix RxT = RotateX.mult(T);
        cout << "RxT" << endl;
        RxT.print();

        cout << "TRxT" << endl;
        matrix TRxT = T.mult(RxT);
        TRxT.print();

        cout << "RzTRxT" << endl;
        matrix RzTRxT = RotateZ.mult(TRxT);
        RzTRxT.print();

        cout << "RyRzTRxT" << endl;
        matrix RyRzTRxT = RotateY.mult(RzTRxT);
        RyRzTRxT.print();

        cout << "SRyRzTRxT" << endl;
        matrix SRyRzTRxT = S.mult(RyRzTRxT);
        SRyRzTRxT.print();

        apply(SRyRzTRxT, 1);
        cout << "Transformacion R aplicada" << endl << endl;
        showmenu();
        
    }

}