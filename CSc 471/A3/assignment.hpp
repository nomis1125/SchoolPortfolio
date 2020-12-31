<<<<<<< HEAD
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
	GLuint vao{ 0 };
	GLuint vbo{ 0 };
	GLuint ibo{ 0 };
	GLsizei count{ -1 };
};

struct Framebuffer
{
	GLuint handle{ 0 };
	GLuint colourTexture{ 0 };
	GLuint depthBuffer{ 0 };
};

struct Pipeline
{
	GLuint vertexHandle{ 0 };
	GLuint fragmentHandle{ 0 };
	GLuint programHandle{ 0 };

	glx::ShaderFile vertexSource{};
	glx::ShaderFile fragmentSource{};
	std::vector<std::string> includeDirs{ ShaderPath };
};

struct ProgramData
{
	utils::MayaCamera::CameraData camera{};
	gui::GuiWindowData windowData{};
	gui::GuiRenderData renderData{};
	GLFWwindow* window{ nullptr };
};

struct UniformMatrices
{
	GLuint ubo{ 0 };
	std::size_t count{ 4 };
};

struct UniformLights
{
	GLuint ubo;
	std::size_t count{ 2 };
};

void onError(int code, char const* message);
void onMouseDown(ProgramData& data, int button, int action, int mode,
	double xPos, double yPos);
void onMouseMove(ProgramData& data, double xPos, double yPos);
void onMouseScroll(ProgramData& data, double xOffset, double yOffset);
void onKeyPress(int key, int scancode, int action, int mods);
void onChar(unsigned int codepoint);

void init(ProgramData& data);

Mesh loadMeshFromFile(std::string const& name);
Mesh createQuad();
Pipeline loadPipeline(std::string const& name);
UniformMatrices createUniformMatrices();
UniformLights createUniformLights();
Framebuffer createFramebuffer();

void reloadPipeline(Pipeline& p);
void updateUniformMatrices(UniformMatrices const& matrices, int width,
	int height, ProgramData const& data);
void updateUniformLights(UniformLights const& lights, ProgramData const& data);
=======
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
	GLuint vao{ 0 };
	GLuint vbo{ 0 };
	GLuint ibo{ 0 };
	GLsizei count{ -1 };
};

struct Framebuffer
{
	GLuint handle{ 0 };
	GLuint colourTexture{ 0 };
	GLuint depthBuffer{ 0 };
};

struct Pipeline
{
	GLuint vertexHandle{ 0 };
	GLuint fragmentHandle{ 0 };
	GLuint programHandle{ 0 };

	glx::ShaderFile vertexSource{};
	glx::ShaderFile fragmentSource{};
	std::vector<std::string> includeDirs{ ShaderPath };
};

struct ProgramData
{
	utils::MayaCamera::CameraData camera{};
	gui::GuiWindowData windowData{};
	gui::GuiRenderData renderData{};
	GLFWwindow* window{ nullptr };
};

struct UniformMatrices
{
	GLuint ubo{ 0 };
	std::size_t count{ 4 };
};

struct UniformLights
{
	GLuint ubo;
	std::size_t count{ 2 };
};

void onError(int code, char const* message);
void onMouseDown(ProgramData& data, int button, int action, int mode,
	double xPos, double yPos);
void onMouseMove(ProgramData& data, double xPos, double yPos);
void onMouseScroll(ProgramData& data, double xOffset, double yOffset);
void onKeyPress(int key, int scancode, int action, int mods);
void onChar(unsigned int codepoint);

void init(ProgramData& data);

Mesh loadMeshFromFile(std::string const& name);
Mesh createQuad();
Pipeline loadPipeline(std::string const& name);
UniformMatrices createUniformMatrices();
UniformLights createUniformLights();
Framebuffer createFramebuffer();

void reloadPipeline(Pipeline& p);
void updateUniformMatrices(UniformMatrices const& matrices, int width,
	int height, ProgramData const& data);
void updateUniformLights(UniformLights const& lights, ProgramData const& data);
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
