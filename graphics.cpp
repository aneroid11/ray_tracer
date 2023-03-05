#include "graphics.h"

void resize_callback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);   
}

graphics::graphics(int sw, int sh) {
    screen_width = sw;
    screen_height = sh;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //window = glfwCreateWindow(screen_width, screen_height, "yok", glfwGetPrimaryMonitor(), nullptr);
    window = glfwCreateWindow(screen_width, screen_height, "", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize_callback);

    glewInit();

    glViewport(0, 0, screen_width, screen_height);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

graphics::~graphics() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void graphics::resize(int new_width, int new_height) {
    glfwSetWindowSize(window, new_width, new_height);
    screen_width = new_width;
    screen_height = new_height;
    glViewport(0, 0, new_width, new_height);
}

GLFWwindow* graphics::get_window() {
    return window;
}

void graphics::set_to_close() {
    glfwSetWindowShouldClose(window, true);
}

bool graphics::window_should_close() {
    return glfwWindowShouldClose(window);
}

bool graphics::key_pressed(GLint key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void graphics::clear_screen(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void graphics::flip_display() {
    glfwSwapBuffers(window);
}

void graphics::convert_to_opengl_coords(float x_screen, float y_screen, float& x_opengl, float& y_opengl) {
    x_opengl = x_screen;
    x_opengl /= (screen_width / 2);
    x_opengl -= 1.0f;

    y_opengl = screen_height - y_screen - 1;
    y_opengl /= (screen_height / 2);
    y_opengl -= 1.0f;
}

int graphics::get_width() {
    return screen_width;
}

int graphics::get_height() {
    return screen_height;
}

int graphics::get_monitor_width() {
    int mon_count = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&mon_count);
    const GLFWvidmode* video_mode = glfwGetVideoMode(monitors[0]);
    return video_mode->width;
}

int graphics::get_monitor_height() {
    int mon_count = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&mon_count);
    const GLFWvidmode* video_mode = glfwGetVideoMode(monitors[0]);
    return video_mode->height;
}

void graphics::check_window_dimensions() {
    if (get_width() > get_monitor_width() || get_height() > get_monitor_height()) {
        resize(get_monitor_width(), get_monitor_height());
    }
}

void graphics::maximize_window() {
    glfwMaximizeWindow(window);   
    screen_width = graphics::get_monitor_width();
    screen_height = graphics::get_monitor_height();
}

float graphics::width_in_gl(int width_on_screen) {
    return float(width_on_screen) / float(screen_width) * 2.0f;
}

float graphics::height_in_gl(int height_on_screen) {
    return float(height_on_screen) / float(screen_height) * 2.0f;
}
