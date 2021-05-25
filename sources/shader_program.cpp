#include "shader_program.h"
#include <dirent.h>

static std::string load_shader(std::string shader_file_name) {
    std::string shader_code = "";
    std::ifstream fin(shader_file_name);
    
    if (!fin) {
        std::cerr << "Cannot find the shader file!\n";
        exit(EXIT_FAILURE);
    }

    std::string current = "";
    while (fin) {
        shader_code += current;
        std::getline(fin, current);
        current += "\n";
    }

    fin.close();

    return shader_code;
}

static bool compile_shader(GLuint shader, const std::string& code) {
    const char* c_code = code.c_str();

    glShaderSource(shader, 1, &c_code, nullptr);
    glCompileShader(shader);

    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        std::cerr << "Cannot compile shader:\n";
        std::cerr << info_log << "\n";
        glDeleteShader(shader);
        return false;
    }

    return true;
}

static bool link_shader_program(GLuint shader_program, GLuint vertex_shader, GLuint fragment_shader) {
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    int success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if(!success) {
        char info_log[512];
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cerr << "Cannot link shader program:\n";
        std::cerr << info_log << "\n";
        glDeleteProgram(shader_program);
        return false;
    }

    return true;
}

shader_program::shader_program(const std::string& vertex_shader_file, const std::string& fragment_shader_file) {
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    std::string vertex_shader_code = load_shader(vertex_shader_file);
    if (!compile_shader(vertex_shader, vertex_shader_code)) {
        std::cerr << "-----------------------\n";
        std::cerr << "Error white compiling " << vertex_shader_file << "\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string fragment_shader_code = load_shader(fragment_shader_file);
    if (!compile_shader(fragment_shader, fragment_shader_code)) {
        std::cerr << "-----------------------\n";
        std::cerr << "Error white compiling " << fragment_shader_file << "\n";
        glDeleteShader(vertex_shader);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    id = glCreateProgram();
    if (!link_shader_program(id, vertex_shader, fragment_shader)) {
        std::cerr << "-----------------------\n";
        std::cerr << "Error white linking " << fragment_shader_file << " and " << vertex_shader_file << "\n";
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}

shader_program::~shader_program() {
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteProgram(id);
}

void shader_program::use() {
    glUseProgram(id);
}

void shader_program::set_uniform(const std::string& name, bool b) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)b);
}

void shader_program::set_uniform(const std::string& name, int i) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), i);
}

void shader_program::set_uniform(const std::string& name, float f) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), f);
}

void shader_program::set_uniform(const std::string& name, float a, float b, float c, float d) const {
    glUniform4f(glGetUniformLocation(id, name.c_str()), a, b, c, d);
}

void shader_program::set_uniform(const char* name, float a, float b, float c) const {
    glUniform3f(glGetUniformLocation(id, name), a, b, c);
}

void shader_program::set_uniform(const std::string& name, float a, float b) {
    glUniform2f(glGetUniformLocation(id, name.c_str()), a, b);
}

void shader_program::set_uniform(const char* name, const glm::mat4x4& matrix) {
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(matrix));
}
