#include <cstdio>
#include <cstdbool>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <cfloat>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics.h"
#include "shader_program.h"
#include "rectangle.h"

const float PLAYER_SPEED = 0.1f;
const float PLAYER_ROTATION_SPEED = 0.6f;

int main() {
    graphics* gr = new graphics(1366, 768);
    
    shader_program* shaders = new shader_program("../ray_tracer/vertex_shader.glsl", "../ray_tracer/fragment_shader.glsl");
    rectangle* screen = new rectangle(0.0f, 0.0f, 2.0f, 2.0f);
    
    glm::mat4 rotate = glm::mat4(1.0f);
    
    glm::vec3 pl_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    
    while (!gr->window_should_close()) {
        float begin_time = glfwGetTime();
        
        glfwPollEvents();
        
        glm::vec4 pl_dir = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        
        if (gr->key_pressed(GLFW_KEY_ESCAPE)) {
            gr->set_to_close();
        }
        if (gr->key_pressed(GLFW_KEY_W)) {
            pl_dir = rotate * pl_dir;
            pl_pos.x += pl_dir.x * PLAYER_SPEED; 
            pl_pos.y += pl_dir.y * PLAYER_SPEED;
            pl_pos.z += pl_dir.z * PLAYER_SPEED;
        }
        else if (gr->key_pressed(GLFW_KEY_S)) {
            pl_dir = rotate * pl_dir;
            pl_pos.x -= pl_dir.x * PLAYER_SPEED;
            pl_pos.y -= pl_dir.y * PLAYER_SPEED;
            pl_pos.z -= pl_dir.z * PLAYER_SPEED;
        }
        else if (gr->key_pressed(GLFW_KEY_A)) {
            glm::mat4 r = rotate;
            r = glm::rotate(r, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            pl_dir = r * pl_dir;
            pl_pos.x += pl_dir.x * PLAYER_SPEED;
            pl_pos.y += pl_dir.y * PLAYER_SPEED;
            pl_pos.z += pl_dir.z * PLAYER_SPEED;
        }
        else if (gr->key_pressed(GLFW_KEY_D)) {
            glm::mat4 r = rotate;
            r = glm::rotate(r, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            pl_dir = r * pl_dir;
            pl_pos.x += pl_dir.x * PLAYER_SPEED;
            pl_pos.y += pl_dir.y * PLAYER_SPEED;
            pl_pos.z += pl_dir.z * PLAYER_SPEED;
        }
        if (gr->key_pressed(GLFW_KEY_LEFT)) {
            rotate = glm::rotate(rotate, -glm::radians(5.0f) * PLAYER_ROTATION_SPEED, glm::vec3(0.0, 1.0, 0.0));
        }
        if (gr->key_pressed(GLFW_KEY_RIGHT)) {
            rotate = glm::rotate(rotate, glm::radians(5.0f) * PLAYER_ROTATION_SPEED, glm::vec3(0.0, 1.0, 0.0));
        }
        
        shaders->set_uniform("player_position", pl_pos.x, pl_pos.y, pl_pos.z); 
        shaders->set_uniform("camera_rotation", rotate);
        
        gr->clear_screen(0.0f, 0.0f, 0.0f, 1.0f);
        screen->draw(*shaders);
        gr->flip_display();
        
        std::cout << glfwGetTime() - begin_time << " s\n";
    }
    
    delete screen;
    delete shaders;
    delete gr;
    
    return 0;
}
