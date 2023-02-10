#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

/**
    Compiles a single shader so it can be attached

    @param type The type of the shader
    @param source The source code of the shader as string
    @return an identifier for the compiled shader
*/
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type); // create the shader
    const char* src = source.c_str(); // return the pointer of the first character of the source
    glShaderSource(id, 1, &src, nullptr); // specify the shader source code
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); // Returns the compile status parameter
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) alloca(length * sizeof(char)); // Allocate this on the stack dynamically because 'char message[length]' is not allowed
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

/**
    Links the given shaders into a single shader so that it can be bound.

    @param vertexShader Source of the vertexshader as string
    @param fragmentShader Source of the fragmentshader as string
    @return an identifier for the newly created shader
*/
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram(); // create a shader program to attach shader to
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attach both shaders to the program
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program); // Link the program so the shaders are used
    glValidateProgram(program); // Check if the program can be executed

    // The shaders are linked to the progam, so the shaders can be deleted
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    /* Initialize Glew */
    if (glewInit() != GLEW_OK)
        std::cout << "glewInit error!" << std::endl;

    /* Log the OpenGL version used because we can */
    std::cout << glGetString(GL_VERSION) << std::endl;


    /* 
        Create and select the data & buffer for drawing
    */

    float positions[6] = {
        -0.5, -0.5,
         0.0,  0.5,
         0.5, -0.5
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer); // Generate a single buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // Select the buffer to be drawn
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); // Add the data to the buffer

    /* Create a layout for the buffer we created */
    glEnableVertexAttribArray(0);

    /*
     first argument(index) - specify the kind of attribute you want to work with
        * In this case I want to work with positions so I specify 0
        * If I would want to work with normal coord I would specify 2
     second argument(size) - specify the size of each attribute. 
        * For example, if you're working with position attribute, you would specify if its 1-D or 2-D or 3-D
        * Likewise for texture and normal attributes, specify the number of elements that are in that attribute
     Third and fourth arguments are pretty straightforward
     Fifth argument (stride) - specify the total number of elements (i.e the attributes) per vertex in bytes. 
        Other ways to think of this: 'size of vertex', 'size of consecutive vertices' etc. 
            Take arbitrary example where I have [3,2,3] -> [position, texture, normal]
            Then your stride would be (3*4) + (2*4) + (3*4) = 32
     Sixth argument (offset) - connected to the first arg. If I really wanted to work with normal (index: 2). 
     I would have to jump (3*4) + (2*4) = 20 bytes to start accessing the subset array of the normal coord
            Recall the array begins [0...19...31]
    */
        
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    /* 
        Creating the shaders
    */

    // Will run for every vector
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n" 
        // ^^ gl_Position needs to be a vec4, 'Make sure the '0' matches the '0' in the glVertexAtrribPointer index: '0' '
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n" 
        /*
        * Remember this line :         "layout(location = 0) in vec4 position;\n" 
        * That's supposed to be in vec2 position, Yes the '2' matches the size argument : '2' given in the glVertexAttribPointer 
        * However, the reason why its changed from vec2 to vec4 is because the gl_Position is a type vec4. So its inevitable that things will always end up in vec4 
        *   for the position attribute. 
        */
        "}\n";

    // Will run for every pixel
    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw the current selected buffer */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader); // Clean up the the program with shaders
    glfwTerminate();
    return 0;
}