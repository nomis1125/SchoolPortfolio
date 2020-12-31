#pragma once

#pragma once

#include "Paths.hpp"

#include <atlas/glx/Buffer.hpp>
#include <atlas/glx/Context.hpp>
#include <atlas/glx/ErrorCallback.hpp>
#include <atlas/glx/GLSL.hpp>
#include <atlas/gui/GUI.hpp>
#include <atlas/utils/Cameras.hpp>
#include <atlas/utils/LoadObjFile.hpp>

using namespace atlas;

struct Mesh
{
    GLuint vao{0};
    GLuint vbo{0};
    GLuint ibo{0};
    GLsizei count{-1};
};

struct Pipeline
{
    GLuint vertHandle{0};
    GLuint tescHandle{0};
    GLuint teseHandle{0};
    GLuint geomHandle{0};
    GLuint fragHandle{0};
    GLuint pipelineHandle{0};

    glx::ShaderFile vertSource{};
    glx::ShaderFile tescSource{};
    glx::ShaderFile teseSource{};
    glx::ShaderFile geomSource{};
    glx::ShaderFile fragSource{};
    std::vector<std::string> includeDirs{ShaderPath};
};

struct ProgramData
{
    utils::MayaCamera::CameraData camera{};
    gui::GuiWindowData windowData{};
    gui::GuiRenderData renderData{};
    GLFWwindow* window{nullptr};
};

void onError(int code, char const* message);
void onMouseDown(ProgramData& data, int button, int action, int mode,
                 double xPos, double yPos);
void onMouseMove(ProgramData& data, double xPos, double yPos);
void onMouseScroll(ProgramData& data, double xOffset, double yOffset);
void onKeyPress(int key, int scancode, int action, int mods);
void onChar(unsigned int codepoint);

void init(ProgramData& data);

Mesh createIcosahedron();
Pipeline loadPipeline(std::string const& name);

void reloadPipeline(Pipeline& p);
