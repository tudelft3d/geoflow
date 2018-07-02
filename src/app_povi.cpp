#include "app_povi.h"
#include <array>

void poviApp::on_initialise(){

    model = glm::mat4();

    // Set up vertex data (and buffer(s)) and attribute pointers
    std::array<GLfloat,18> vertices = {
        // Positions         // Colors
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
       -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
        0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Top 
    };

    buffer.init();
    painter.init();

    buffer.set_data(vertices.data(), vertices.size());

    painter.set_buffer(buffer);
    painter.setup_VertexArray();

    // Shader shader;
    shader.init();
    shader.attach("basic.vert");
    shader.attach("basic.frag");
    shader.link();

    painter.set_program(shader);

    glfwGetCursorPos(window, &last_mouse_pos.x, &last_mouse_pos.y);

    // std::cout << "prog." << shader.get() << std::endl;
}

void poviApp::on_resize(int new_width, int new_height) {
    width = new_width;
    height = new_height;
}

void poviApp::on_draw(){
    // Render
    update_view_matrix();
    update_projection_matrix();
    painter.render(model, view, projection);

    ImGui::Begin("View parameters");
    xy_pos p0 = screen2view(last_mouse_pos);
    ImGui::Text("Mouse pos [screen]: (%g, %g)", last_mouse_pos.x, last_mouse_pos.y);
    ImGui::Text("Mouse pos [view]: (%g, %g)", p0.x, p0.y);
    ImGui::SliderFloat("Field of view", &fov, 1, 180);
    ImGui::SliderFloat("Clip near", &clip_near, 0.01, 100);
    ImGui::SliderFloat("Clip far", &clip_far, 1, 1000);
    ImGui::SliderFloat("Camera position", &cam_pos, -20, -1);
    ImGui::SliderFloat("Scale", &scale, 0.01, 100);
    ImGui::End();
}

void poviApp::on_key_press(int key, int action, int mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_LEFT_SHIFT) {

    }
}

void poviApp::on_mouse_press(int button, int action, int mods) {   
    
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        if(mods & GLFW_MOD_SHIFT)
            drag = TRANSLATE;
        else
            drag = ROTATE;

        glfwGetCursorPos(window, &drag_init_pos.x, &drag_init_pos.y);
    } else if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT) {
        if(drag==TRANSLATE){
            translation += translation_ondrag;
            translation_ondrag = glm::vec3(0);
        }
        drag = NO_DRAG;
    }
}


void poviApp::on_mouse_move(double xpos, double ypos) {
    if (drag==TRANSLATE) {
        xy_pos delta = { xpos-drag_init_pos.x, ypos-drag_init_pos.y };
        // std::cout << "in drag " << screen2view({xpos,ypos}).x << screen2view({xpos,ypos}).y << std::endl;

        double scale_ = -cam_pos * 2* std::tan(glm::radians(fov/2.));
        // multiply with inverse view matrix and apply translation in world coordinates
        double radius = std::min(width, height);
        glm::vec4 t_screen = glm::vec4(scale_*delta.x/radius, scale_*-delta.y/radius, 0., 0.);
        translation_ondrag =  glm::vec3( glm::inverse(view) * t_screen );
        // update_view_matrix();
    } else if (drag==ROTATE) {
        xy_pos p0 = screen2view(last_mouse_pos);
        xy_pos p1 = screen2view({xpos, ypos});

        glm::quat q0 = arcball(p0);
        glm::quat q1 = arcball(p1);
        rotation =  q1 * q0 * rotation;
        // update_view_matrix();
    }
    last_mouse_pos.x = xpos;
    last_mouse_pos.y = ypos;
}
void poviApp::on_mouse_wheel(double xoffset, double yoffset){
    scale *= yoffset/50 + 1;
    // update_view_matrix();
}

void poviApp::update_view_matrix(){
    auto t = glm::mat4();
    t = glm::translate(t, glm::vec3(0.0f,0.0f,cam_pos));
    t = glm::scale(t, glm::vec3(scale));
    t = t * glm::mat4_cast(rotation);
    t = glm::translate(t, translation);
    t = glm::translate(t, translation_ondrag);
    view = t;
}
void poviApp::update_projection_matrix(){
    projection = glm::perspective(glm::radians(fov), float(width)/float(height), clip_near, clip_far );       
}

inline xy_pos poviApp::screen2view(xy_pos p){
    double radius = std::min(width, height);
    return {(p.x-width/2.)/radius, ((height-p.y)-height/2.)/radius};
}