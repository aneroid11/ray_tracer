#ifndef PPM_HPP
#define PPM_HPP

#include <iostream>
#include <cstdio>
#include <cstring>

typedef unsigned char byte;

struct color {
    color(byte r = 0, byte g = 0, byte b = 0);

    byte r, g, b;
};

struct ppm_image {
    ppm_image(int w = 0, int h = 0);
    ppm_image(const char* file);
    ~ppm_image();
    void write_in_file(const char* file);
    void clear(color cl);
    void clear_to_black();
    void put_pixel(int x, int y, color cl);
    color get_pixel(int x, int y);
    void flip_vertically();
    void flip_horizontally();

    int width, height;
    color* pixels;
};

#endif // PPM_HPP
