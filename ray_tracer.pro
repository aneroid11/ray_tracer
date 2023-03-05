TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        graphics.cpp \
        main.cpp \
        rectangle.cpp \
        shader_program.cpp

HEADERS += \
    graphics.h \
    rectangle.h \
    shader_program.h

LIBS +=	-fopenmp -lglfw -lGL -lGLEW

QMAKE_CXXFLAGS += -Ofast

DISTFILES += \
    ../build-ray_tracer-Desktop-Debug/vertex_shader.glsl \
    ../build-ray_tracer-Desktop-Debug/vertex_shader.glsl \
    fragment_shader.glsl \
    vertex_shader.glsl
