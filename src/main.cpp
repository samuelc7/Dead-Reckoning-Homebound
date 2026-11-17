#include <iostream>

#include <SDL3/SDL.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "stb_image.h"

void processMouseMovement(float& yaw, float& pitch, glm::vec3& cameraFront, float xOffset, float yOffset);

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO || SDL_INIT_AUDIO))
    {
        std::cerr << "SDL initialization failed: "
            << SDL_GetError() << '\n';
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


    SDL_Window* window = SDL_CreateWindow(
        "Battlefront Engine",
        1280,
        720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    
    if (!window) 
    {
        std::cerr << "Window creation failed: "
            << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "GLAD load failed: "
            << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_SetWindowRelativeMouseMode(window, true);

    float yaw = -90.0f;
    float pitch = 0.0f;

    float fov = 45.0f;

    float deltaTime = 0.0f; // Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    // Tell OpenGL the size of the rendering window 
    //  (-1, 1) to (0, 1280) and (0, 720)
    glViewport(0, 0, 1280, 720);

    glEnable(GL_DEPTH_TEST);
    // Wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Default mode (fill)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    // Make buffer swap sync w/ monitors vertical refresh
    //SDL_GL_SetSwapInterval(1);
   
    // ---------------------------------------------------- //
    // tmp test shape


    // TRIANGLE
    //float vertices_t[] =
    //{
    //    -0.5f, -0.5f, 0.0f, // bottom left
    //    0.5f, -0.5f, 0.0f, // bottom right
    //    0.0f, 0.5f, 0.0f, // top middle

    //    0.5f, -0.5f, 0.0f, // bottom left
    //    1.0f, -0.5f, 0.0f, // bottom right
    //    0.75f, 0.5f, 0.0f, // top middle
    //};


    // TRIANGLE W/ COLORS
    //float vertices[] =
    //{
    //    // positions // colors
    //     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
    //    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
    //     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top
    //};

    //float vertices[] =
    //{
    //    // positions // colors // texture coords
    //    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    //    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    //    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    //    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
    //};

	float vertices[] = 
    {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    // RECTANGLE
    //float vertices_r[] = 
    //{
    //    0.5f, 0.5f, 0.0f, // top right
    //    0.5f, -0.5f, 0.0f, // bottom right
    //    -0.5f, -0.5f, 0.0f, // bottom left
    //    -0.5f, 0.5f, 0.0f // top left
    //};
    unsigned int indices[] =
    {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

    // Shader
    Shader basicShader("shaders/basic.vert", "shaders/basic.frag");
    basicShader.use();
    //basicShader.setFloat("offset_x", 0.25f);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    //unsigned int VAO_t2;
    //glGenVertexArrays(1, &VAO_t2);

    //unsigned int VBO_t2;
    //glGenBuffers(1, &VBO_t2);

    // An EBO is a buffer, just like a vertex buffer object,
    //  that stores indices that OpenGL uses to decide what vertices to draw
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // then set the vertex attributes pointers
    // position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
    //    (void*)0);
    //glEnableVertexAttribArray(0);
    //// color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
    //    (void*)(3* sizeof(float)));
    //glEnableVertexAttribArray(1);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    stbi_set_flip_vertically_on_load(true);

    // load and create texture
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/container.jpg", &width, &height,
        &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture." << std::endl;
    }

    stbi_image_free(data);


    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("assets/awesomeface.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture." << std::endl;
    }

    stbi_image_free(data);

    // copy index array in a element buffer for OpenGL to use
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_r), indices_r,
    //    GL_STATIC_DRAW);
    // 
    //glBindVertexArray(VAO_t2);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO_t2);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //// then set the vertex attributes pointers
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    //    (void*)0);
    //glEnableVertexAttribArray(0);

    //glBindVertexArray(VAO_t);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO_t);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_t), vertices_t, GL_STATIC_DRAW);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    //    (void*)0);
    //glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // use our shader program when we want to render an object
    //glUseProgram(shaderProgram);
    // now draw the object
    //someOpenGLFunctionThatDrawsOurTriangle();
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    // ---------------------------------------------------- //

    //model = glm::rotate(model, glm::radians(-55.0f),
    //    glm::vec3(1.0f, 0.0f, 0.0f));


    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    //glm::mat4 view = glm::mat4(1.0f);
    //// note that we’re translating the scene in the reverse direction
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    //projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,
    //    100.0f);

    //glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    //glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    //glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    //glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    //glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    //glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    //glm::mat4 view;
    //view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
    //    glm::vec3(0.0f, 0.0f, 0.0f),
    //    glm::vec3(0.0f, 1.0f, 0.0f));
    
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    bool running = true;
    SDL_Event event;

    while (running) 
    {
        float currentFrame = SDL_GetTicks() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        while (SDL_PollEvent(&event))
        {

            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
            {
                int w = event.window.data1;
                int h = event.window.data2;
                glViewport(0, 0, w, h);
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION)
            {
                processMouseMovement(
                    yaw,
                    pitch,
                    cameraFront,
                    event.motion.xrel,
                    event.motion.yrel
                );
            }
            if (event.type == SDL_EVENT_MOUSE_WHEEL)
            {
                fov -= event.wheel.y;

                if (fov < 1.0f) fov = 1.0f;
                if (fov > 45.0f) fov = 45;
            }
        }
        const float cameraSpeed = 2.5f * deltaTime;

        const bool* key_states = SDL_GetKeyboardState(NULL);
        if (key_states[SDL_SCANCODE_W])
            cameraPos += cameraSpeed * cameraFront;
        
        if (key_states[SDL_SCANCODE_S])
            cameraPos -= cameraSpeed * cameraFront;

        if (key_states[SDL_SCANCODE_A])
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        if (key_states[SDL_SCANCODE_D])
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //float timeValue = SDL_GetTicks() / 1000.0f;
        //float greenValue = sin(timeValue) / 2.0f + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgramYellow, "ourColor");
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        basicShader.use(); // active shader

        glUniform1i(glGetUniformLocation(basicShader.ID, "texture1"), 0); // manual
        basicShader.setInt("texture2", 1);


        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)SDL_GetTicks() / 1000.0f,
            glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 model = glm::mat4(1.0f);

        projection = glm::perspective(
            glm::radians(fov),
            1280.0f / 720.0f,
            0.1f,
            100.0f
        );

        //const float radius = 10.0f;
        //float camX = sin(SDL_GetTicks() / 1000.0f) * radius;
        //float camZ = cos(SDL_GetTicks() / 1000.0f) * radius;
        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        //view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0),
        //    glm::vec3(0.0, 1.0, 0.0));
        //model = glm::rotate(model, (float)SDL_GetTicks() / 1000.0f * glm::radians(50.0f),
        //    glm::vec3(0.5f, 1.0f, 0.0f));
        unsigned int modelLoc = glGetUniformLocation(basicShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        unsigned int viewLoc = glGetUniformLocation(basicShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        unsigned int projectionLoc = glGetUniformLocation(basicShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle),
                glm::vec3(1.0f, 0.3f, 0.5f));
            basicShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }



        //glUseProgram(shaderProgramYellow);
        //glBindVertexArray(VAO_t2);
        //glDrawArrays(GL_TRIANGLES, 3, 3);
        //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    }



    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void processMouseMovement(float& yaw, float& pitch, glm::vec3& cameraFront, float xOffset, float yOffset)
{
    float sensitivity = 0.1f;

    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f)
        pitch = 89.0f;

    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}