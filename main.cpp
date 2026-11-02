// Platformer 2D - main minimale: GLFW/GLAD, input, update e rendering.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Scene.h"
#include "Shader.h"
#include "Renderer.h"
#include "BackgroundRenderer.h"
#include "CoinRenderer.h"
#include "Landscape.h"
#include "Game.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Legge il testo di uno shader; restituisce una stringa vuota se manca.
std::string loadTextFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "File shader non trovato: " << path << '\n';
        return "";
    }
    std::ostringstream out;
    out << file.rdbuf();
    return out.str();
}

int main()
{
    // 1. Finestra e contesto OpenGL 3.3 Core.
    if (!glfwInit()) {
        std::cerr << "Errore inizializzazione GLFW\n";
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        1100, 700, "Mountain Quest 2D - Robot e cristalli", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // 2. GLAD deve essere caricato dopo la creazione del contesto.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Errore inizializzazione GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    const std::string vertexSource = loadTextFile("shaders/basic.vert");
    const std::string fragmentSource = loadTextFile("shaders/basic.frag");
    if (vertexSource.empty() || fragmentSource.empty()) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Carica anche i due sorgenti del programma shader dello sfondo.
    const std::string backgroundVertex = loadTextFile("shaders/background.vert");
    const std::string backgroundFragment = loadTextFile("shaders/background.frag");
    if (backgroundVertex.empty() || backgroundFragment.empty()) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Terzo programma shader, riservato alle monete.
    const std::string coinVertex = loadTextFile("shaders/coin.vert");
    if (coinVertex.empty()) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // 3. Questo scope distrugge le risorse OpenGL prima del contesto GLFW.
    {
        Shader shader(vertexSource, fragmentSource);
        // Shader separato per lo sfondo (fullscreen quad in NDC).
        Shader backgroundShader(backgroundVertex, backgroundFragment);
        BackgroundRenderer background(backgroundShader); // sfondo
        Renderer renderer(shader); // generico
        Shader coinShader(coinVertex, fragmentSource);
		CoinRenderer coinRenderer(coinShader); // renderer per le monete, usa coin.vert + basic.frag
        Scene scene;
        Game game(scene); // Crea personaggio, terreno, ostacoli e monete.
        Landscape landscape; // Montagne e alberi: geometrie e istanze indipendenti.

        bool previousJump = false;
        bool previousReset = false;
        double previousTime = glfwGetTime();

        // 4. Game loop: tempo, input, simulazione, rendering.
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GLFW_TRUE);

            const double now = glfwGetTime();
            const float dt = std::clamp(
                static_cast<float>(now - previousTime), 0.0f, 0.03f);
            previousTime = now;

            int width = 0, height = 0;
            glfwGetFramebufferSize(window, &width, &height);
            if (width <= 0 || height <= 0) continue;

            const float halfHeight = GameConfig::HALF_HEIGHT;
            const float halfWidth = halfHeight * static_cast<float>(width) / height;
            glViewport(0, 0, width, height);
            renderer.setOrthoProjection(halfWidth, halfHeight);
            game.resize(halfWidth);

            const bool left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
                              glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
            const bool right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
                               glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
            const bool jump = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
            const bool restart = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;

            // Rileva il passaggio da tasto rilasciato a premuto.
            if (restart && !previousReset) game.reset();
            previousReset = restart;
            game.update(dt, halfWidth, left, right, jump && !previousJump,
                        static_cast<float>(now));
            previousJump = jump;

            const std::string title = "Mountain Quest 2D | Cristalli: " +
                std::to_string(game.score()) + " | A/D muovi - SPACE salta - R reset";
            glfwSetWindowTitle(window, title.c_str());

            glClearColor(0.06f, 0.11f, 0.20f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            // Ordine: prima il fondale, poi gli oggetti del mondo.
            background.draw(static_cast<float>(now));
            // Il paesaggio usa basic.vert/basic.frag ma una Scene distinta.
            // Deve essere disegnato PRIMA del personaggio e degli ostacoli.
            landscape.draw(renderer);
            // Il renderer ordinario ESCLUDE la geometria delle monete.
            renderer.drawScene(scene, game.coinGeometryIndex());
            // Le monete sono disegnate una sola volta, con coin.vert.
            coinRenderer.draw(scene, game, renderer.projectionMatrix(),
                              static_cast<float>(now));
            glfwSwapBuffers(window);
        }
        // Scene viene svuotata mentre il contesto OpenGL e' ancora attivo.
        landscape.clear();
        scene.clear();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
