#include <iostream>

#include <iostream>

#include <SDL3/SDL.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "mesh.h"
#include "model.h"

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

    Camera camera;
    camera.max_fov = 80.0f;

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
    // Shader
    Shader basicShader("shaders/basic.vert", "shaders/basic.frag");
    basicShader.use();

    // ---------------------------------------------------- //
    // Load textures
    stbi_set_flip_vertically_on_load(true);

    Model z95("assets/models/z-95/z-95.obj");

    glm::mat4 projection;

    bool running = true;

    bool first_loop = true;

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
                camera.processMouseMovement(event.motion.xrel, event.motion.yrel);
            }
            if (event.type == SDL_EVENT_MOUSE_WHEEL)
            {
                camera.processMouseWheel(event.wheel.y);
            }
        }
        const float cameraSpeed = 2.5f * deltaTime;
        const bool* key_states = SDL_GetKeyboardState(NULL);
        camera.handleCameraMovement(cameraSpeed, key_states);

        glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        basicShader.use(); // active shader

        basicShader.setInt("texture1", 0);
        basicShader.setInt("texture2", 1);

        glm::mat4 model = glm::mat4(1.0f);

        projection = glm::perspective(
            glm::radians(camera.fov),
            1280.0f / 720.0f,
            0.1f,
            100.0f
        );

        glm::mat4 view;
        view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);

        basicShader.setMat4("projection", projection);
        basicShader.setMat4("view", view);

        model = glm::translate(model, glm::vec3(0.0f, -1.3f, 0.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        
        basicShader.setMat4("model", model);
        z95.Draw(basicShader);

        SDL_GL_SwapWindow(window);

        first_loop = false;
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}