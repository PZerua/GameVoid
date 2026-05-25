#include "gb/GameBoy.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "utils/tinyfiledialogs.h"

#include <iostream>
#include <chrono>

void drawGUI(GameBoy** gameboy)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open game (.gb)"))
            {
                std::vector<const char*> filter_patterns = { "*.gb" };
                char const* romPath = tinyfd_openFileDialog(
                    "Scene loader",
                    "",
                    static_cast<int>(filter_patterns.size()),
                    filter_patterns.data(),
                    "Scene formats",
                    0
                );

                if (romPath) {
                    if (*gameboy) {
                        delete *gameboy;
                    }

                    *gameboy = new GameBoy();
                    if (!(*gameboy)->init(romPath)) {
                        system("pause");
                        delete *gameboy;
                        exit(1);
                    }
                }
            }

            if (ImGui::MenuItem("Exit")) {
                exit(0);
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

int main(int argc, char* argv[])
{
    std::string romPath;

    if (argc > 1) {
        romPath = argv[1];
    }

    gfx::Window window;
    if (!window.init("GameVoid", 960, 864)) {
        return false;
    }

    GameBoy* gameboy = nullptr;
    
    if (!romPath.empty()) {
        gameboy = new GameBoy();
        if (!gameboy->init(romPath)) {
            system("pause");
            delete gameboy;
            return 1;
        }
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.getGlfwWindow(), true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init("#version 130");

    input::Keyboard& input = input::Keyboard::getInstance();

    double deltaTime = 0.0;

    while (!input.isPressed(GLFW_KEY_ESCAPE) && !window.isClosed())
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        window.pollEvents();
        window.clear();

        if (gameboy) {
            gameboy->tick(deltaTime);
        }
        else {
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            drawGUI(&gameboy);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        window.swap();

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() * 0.001;
    }

    delete gameboy;

    return 0;
}
