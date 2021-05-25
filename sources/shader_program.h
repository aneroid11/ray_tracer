#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

class shader_program {
public:
    shader_program(const std::string& vertex_shader_file, const std::string& fragment_shader_file);
    ~shader_program();

    void use();
    void set_uniform(const std::string& name, bool b) const;
    void set_uniform(const std::string& name, int i) const;
    void set_uniform(const std::string& name, float f) const;
    void set_uniform(const char* name, float a, float b, float c) const;
    void set_uniform(const std::string& name, float a, float b, float c, float d) const;
    void set_uniform(const std::string& name, float a, float b);
    void set_uniform(const char* name, const glm::mat4x4& matrix);

private:
    GLuint vertex_shader, fragment_shader;
    GLuint id;
};

#endif //SHADER_PROGRAM_H
