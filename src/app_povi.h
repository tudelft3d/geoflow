#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/quaternion.hpp>
// #include <glm/gtx/quaternion.hpp>

#include "app.h"

enum mouse_drag{
    NO_DRAG,
    TRANSLATE,
    ROTATE
};

typedef struct {double x,y;} xy_pos;

inline glm::quat arcball(xy_pos p){
	double h,h2 = p.x*p.x+p.y*p.y;
	if (h2 > 1.){
		h = glm::sqrt(h2);
		return glm::quat(0., p.x/h, p.y/h, 0.);
	} else
		return glm::quat(0., p.x, p.y, glm::sqrt(1.-h2));
}

class poviApp: public App {
public:
poviApp(int width, int height, std::string title):App(width, height, title){}

protected:
void on_initialise();
void on_resize(int new_width, int new_height);
void on_draw();
void on_key_press(int key, int action, int mods);
void on_mouse_wheel(double xoffset, double yoffset);
void on_mouse_move(double xpos, double ypos);
void on_mouse_press(int button, int action, int mods);
GLuint VBO, VAO;
Shader shader;
Buffer buffer;
Painter painter;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

mouse_drag drag = NO_DRAG;
xy_pos drag_init_pos;
xy_pos last_mouse_pos;

float fov = 60;
float clip_near = 0.01;
float clip_far = 100;

float cam_pos = -2;
float scale = 0.6;
glm::vec3 translation;
glm::vec3 translation_ondrag;
glm::quat rotation;
glm::quat rotation_ondrag;

void update_projection_matrix();
void update_view_matrix();
inline xy_pos screen2view(xy_pos p);

};
