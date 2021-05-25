#include "rectangle.h"

rectangle::rectangle() {
    vertex_array_object = 0;
    vertex_buffer = 0;
    rectangle_element_buffer = 0;
    width = 0;
    height = 0;
    x_pos = 0;
    y_pos = 0;
}

rectangle::rectangle(float x_pos, float y_pos, float width, float height) {
    float hw = width / 2.0f;
    float hh = height / 2.0f;

    float rectangle_vertices[] = {
        hw,  hh,
        hw, -hh, 
        -hw, -hh,
        -hw,  hh,   
    };
    GLuint rectangle_indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &rectangle_element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangle_element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_indices), rectangle_indices, GL_STATIC_DRAW);

    this->x_pos = x_pos;
    this->y_pos = y_pos;
    this->width = width;
    this->height = height;
}

rectangle::~rectangle() {
    if (!vertex_array_object) { return; }

    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &rectangle_element_buffer);
}

void rectangle::draw(shader_program& program) {
    if (!vertex_array_object) { return; }
    if (!visible) { return; }

    program.use();

    glBindVertexArray(vertex_array_object);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
