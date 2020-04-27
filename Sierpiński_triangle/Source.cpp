#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

unsigned int depth = 6;
unsigned int num_triangules = 1;

float colorValue = 0.75;
float inc = 0.004;
int vertexColorLocation;
unsigned int c1 = 0, c2 = 0, c3 = 0;

vector<float> vertices;


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";


const char* fragmentShader1Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";



float lenghtT = 0.8;

struct Point2D {
    float x, y;

    Point2D(float _x, float _y)
    {
        x = _x;
        y = _y;
    }
};

struct Triangule {
    Point2D* top;
    Point2D* left;
    Point2D* right;

    Triangule(Point2D* t, Point2D* l, Point2D* r) {
        top = t;
        left = l;
        right = r;
    }

};

Point2D* midPoint(Point2D* A, Point2D* B)
{
    float x = (A->x + B->x) / 2;
    float y = (A->y + B->y) / 2;
    return new Point2D(x, y);
}


void saveVertices(Triangule* T)
{
    vertices.push_back(T->left->x);
    vertices.push_back(T->left->y);
    vertices.push_back(0.0);
    vertices.push_back(T->right->x);
    vertices.push_back(T->right->y);
    vertices.push_back(0.0);
    vertices.push_back(T->top->x);
    vertices.push_back(T->top->y);
    vertices.push_back(0.0);

}

Triangule* GenerateMidTriangule(Triangule* T)
{
    Point2D* top = midPoint(T->left, T->right);
    Point2D* left = midPoint(T->left, T->top);
    Point2D* right = midPoint(T->right, T->top);

    Triangule* newTriangule = new Triangule(top, left, right);
    saveVertices(newTriangule);
    return newTriangule;
}

void buildSTriangule(Triangule* T, int local_depth)
{
    
    if (local_depth == 0)
        return;
    
    Triangule* MidTriangule = GenerateMidTriangule(T); 
    
    buildSTriangule(new Triangule(MidTriangule->left,T->left, MidTriangule->top), local_depth-1);
    buildSTriangule(new Triangule(T->top, MidTriangule->left, MidTriangule->right), local_depth-1);
    buildSTriangule(new Triangule(MidTriangule->right, MidTriangule->top, T->right), local_depth-1);
}




void setColor(int c)
{
    if ( c == 1 )
        glUniform4f(vertexColorLocation, colorValue, 0.3f, 1.0f, 1.0f);//morado
    else if ( c == 2 )
        glUniform4f(vertexColorLocation, 0.9f, 0.3f, colorValue, 1.0f);//rosado
    else if (c == 3 )
        glUniform4f(vertexColorLocation, 0.3f, 0.42f, colorValue, 1.0f);//celeste
    else
        glUniform4f(vertexColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);
}


int main()
{ 
    // create initial triangule
    Triangule* T = new Triangule(new Point2D(0.0, lenghtT), new Point2D(-lenghtT, -lenghtT), new Point2D(lenghtT, -lenghtT));
    saveVertices(T);
    int local_depth = depth;

    // build Sierpinski triangle

    buildSTriangule(T,local_depth);

    
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



    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<std::endl;
    }

    unsigned int fragmentShader1;
    fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShader1);

    glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

    }

    unsigned int shaderProgram1;
    shaderProgram1 = glCreateProgram();


    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);



    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);



    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);



    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   
    // render loop
    // -----------

    cout << vertices.size()<< endl;
    cout << vertices.size()/9 << endl;

    
    int num_vertices = (vertices.size() / 9) * 3;
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram1);
        
        if (colorValue >= 0.9 || colorValue <= 0.3)
            inc *= -1;
            
        colorValue += inc;

        //cout << "cor value: " << colorValue << endl;

        vertexColorLocation = glGetUniformLocation(shaderProgram1, "ourColor"); 
        glUniform4f(vertexColorLocation, 0.35f, colorValue, 0.75f, 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, 3);
      
        int i;
        glUniform4f(vertexColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);

        glDrawArrays(GL_TRIANGLES, 3, num_vertices-3);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
       
        for (i=6; i < num_vertices/3 + 3; i += 3)
        {
            vertexColorLocation = glGetUniformLocation(shaderProgram1, "ourColor");
            setColor(c1);           
            glDrawArrays(GL_TRIANGLES, i, 3);
       
        }
        
        for (i; i< ((num_vertices/3)*2);i += 3)
        {

            vertexColorLocation = glGetUniformLocation(shaderProgram1, "ourColor");
            setColor(c2);
            glDrawArrays(GL_TRIANGLES, i, 3);

        }
        
        for (i ; i < num_vertices; i += 3)
        {

            vertexColorLocation = glGetUniformLocation(shaderProgram1, "ourColor");
            setColor(c3);
            glDrawArrays(GL_TRIANGLES, i, 3);

        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glfwSwapBuffers(window);
     



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------

        glfwPollEvents();
    }

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

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        c1 = (c1 + 1)%4;
        c2 = (c1 + 1)%4;
        c3 = (c2 + 1)%4;
    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        c1 = (c1 + 3) % 4;
        c2 = (c1 + 1) % 4;
        c3 = (c2 + 1) % 4;
    }

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        if(c1!=0)
            c1 = 0;
        else if(c2!=0)
            c2 = 0;
        else if(c3!=0)
            c3 = 0;
    }

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        if (c1==0)
            c1 = (c1 + 1) % 4;
        else if (c2==0)
            c2 = (c1 + 1) % 4;
        else if (c3 == 0)
            c3 = (c2 + 1) % 4;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        c1 = c2 = c3 = 0;
    }

}
