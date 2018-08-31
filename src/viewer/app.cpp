#include "app.h"

App::App(int width, int height, std::string title)
	:width(width), height(height), clear_color(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)) {

    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
        exit(EXIT_FAILURE);    // Set all the required options for GLFW

        // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
    
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetWindowUserPointer(window, this);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    glEnable(GL_PROGRAM_POINT_SIZE);

    glfwSwapInterval(1);

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // setup_callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetCharCallback(window, char_callback);
    glfwSetDropCallback(window, drop_callback);
}


void App::run(){
    
	on_initialise();
    bool show_demo_window = false;

    while (!glfwWindowShouldClose(window))
    { 
 

        glfwMakeContextCurrent(window);
        // get framebuffer size, which could be different from the window size in case of eg a retina display  
        glfwGetFramebufferSize(window, &viewport_width, &viewport_height);
        glViewport(0, 0, viewport_width, viewport_height);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // process events:
               // don't do anything if window not in focus
        if(!glfwGetWindowAttrib(window, GLFW_FOCUSED))
            glfwWaitEvents();
        // glfwWaitEvents(); // and sleep until there is an event
        else
            glfwPollEvents(); // don't sleep, eg needed for animations

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        on_draw();
        ImGui::ColorEdit4("Clear color", (float*)&clear_color);
        ImGui::Checkbox("Show demo window", &show_demo_window);

        if (show_demo_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        // ImGui Rendering
        ImGui::Render();
        int display_w, display_h;
        
        // glfwGetFramebufferSize(window, &display_w, &display_h);
        // glViewport(0, 0, display_w, display_h);
        
        // glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
        // for(int i=1; i>0; i--){
        //     draw(); //second draw to ensure imgui is updated in case of glfwWaitEvents()
        //     // not very elegant this and problem with modals...
        // }
        
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void App::draw(){
    
}

void App::key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods
    ){
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    if (ImGui::GetIO().WantCaptureKeyboard) return;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    void *data = glfwGetWindowUserPointer(window);  
    App *a = static_cast<App *>(data);
    a->on_key_press(key, action, mods);
}

void App::cursor_pos_callback(
    GLFWwindow* window, double xpos, double ypos
    ){
    if (ImGui::GetIO().WantCaptureMouse) return;
    
    void *data = glfwGetWindowUserPointer(window);  
    App *a = static_cast<App *>(data);
    a->on_mouse_move(xpos, ypos);
}

void App::mouse_button_callback(
    GLFWwindow* window, int button, int action, int mods
    ){
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }

    void *data = glfwGetWindowUserPointer(window);  
    App *a = static_cast<App *>(data);
    a->on_mouse_press(button, action, mods);
}

void App::scroll_callback(
    GLFWwindow* window, double xoffset, double yoffset
    ){
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    if (ImGui::GetIO().WantCaptureMouse) return;

    void *data = glfwGetWindowUserPointer(window);  
    App *a = static_cast<App *>(data);
    a->on_scroll(xoffset, yoffset);
}

void App::char_callback(
    GLFWwindow* window, unsigned int c
    ){
    ImGui_ImplGlfw_CharCallback(window, c);
}

void App::window_size_callback(
	GLFWwindow* window, int width, int height
	){

    void *data = glfwGetWindowUserPointer(window);
    App *a = static_cast<App *>(data);
    a->on_resize(width, height);
}

void App::error_callback(int error, const char* description) {
	std::cerr << error << " " << description;
}

void App::drop_callback(GLFWwindow* window, int count, const char** paths) {
    for (int i = 0;  i < count;  i++)
	    std::cout << paths[i] << "\n";

    void *data = glfwGetWindowUserPointer(window);
    App *a = static_cast<App *>(data);
    a->on_drop(count, paths);
}