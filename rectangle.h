#ifndef TEXTURE_RECTANGLE_H
#define TEXTURE_RECTANGLE_H

#include "shader_program.h"

class rectangle {
public:
    rectangle();
    rectangle(float x_pos, float y_pos, float width, float height);
    virtual ~rectangle();
    void draw(shader_program& program);
    float get_x() const { return x_pos; }
    float get_y() const { return y_pos; }
    float get_width() const { return width; }
    float get_height() const { return height; }
    bool is_visible() const { return visible; }
    
    void make_invisible() { visible = false; }
    void make_visible() { visible = true; }

protected:
    GLuint vertex_array_object;
    GLuint vertex_buffer;
    GLuint rectangle_element_buffer;
    float width, height;
    float x_pos, y_pos;
    bool visible = true;
};

#endif //TEXTURE_RECTANGLE_H
