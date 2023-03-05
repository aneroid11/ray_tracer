#ifndef TEXTURE_RECTANGLE_H
#define TEXTURE_RECTANGLE_H

#include "shader_program.h"

class texture_rectangle {
public:
    texture_rectangle();
    texture_rectangle(float x_pos, float y_pos, float width, float height);
    virtual ~texture_rectangle();
    virtual void destroy();
    void draw(shader_program& program);
    float get_x() const;
    float get_y() const;
    float get_width() const;
    float get_height() const;
    virtual void update();

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
