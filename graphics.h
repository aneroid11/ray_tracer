#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class graphics {
public:
    graphics(int sw = 1024, int sh = 768);
    ~graphics();
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void resize(int new_width, int new_height);
    GLFWwindow* get_window();
    void set_to_close();
    bool window_should_close();
    bool key_pressed(GLint key);
    void clear_screen(float r, float g, float b, float a);
    void flip_display();
    void convert_to_opengl_coords(float x_screen, float y_screen, float& x_opengl, float& y_opengl);
    float width_in_gl(int width_on_screen);
    float height_in_gl(int height_on_screen);
    int get_width();
    int get_height();
    int get_monitor_width();
    int get_monitor_height();
    void check_window_dimensions();
    void maximize_window();

private:
    GLFWwindow* window;
    int screen_width, screen_height;
};

#endif //GRAPHICS_H
