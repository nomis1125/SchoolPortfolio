<<<<<<< HEAD
#include "Paths.hpp"
#include "UniformLocations.glsl"
#include "assignment.hpp"
#include <fmt/printf.h>

int main()
{
	ProgramData programData;
	init(programData);

	auto knot = loadMeshFromFile("torusknot.obj");
	auto quad = createQuad();
	auto shaded = loadPipeline("shaded");
	auto frame = loadPipeline("fbo");
	auto fbo = createFramebuffer();
	auto uniformMatrices = createUniformMatrices();
	auto uniformLights = createUniformLights();

	glEnable(GL_DEPTH_TEST);
	int renderMode{ 0 };
	while (!glfwWindowShouldClose(programData.window))
	{
		int width, height;
		glfwGetFramebufferSize(programData.window, &width, &height);
		glViewport(0, 0, width, height);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		reloadPipeline(shaded);
		reloadPipeline(frame);
		updateUniformMatrices(uniformMatrices, width, height, programData);
		updateUniformLights(uniformLights, programData);

		if (renderMode == 0)
		{
			glUseProgram(shaded.programHandle);
			glBindVertexArray(knot.vao);
			glDrawElements(GL_TRIANGLES, knot.count, GL_UNSIGNED_INT,
				glx::bufferOffset<GLuint>(0));
		}
		else if (renderMode == 1)
		{
			// pass1: bind, clear, render to buffer, bind default back
			glBindFramebuffer(GL_FRAMEBUFFER, fbo.handle);
			glViewport(0, 0, width, height);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// if only care for depth just clear depth not color

			glUseProgram(shaded.programHandle);

			glBindVertexArray(knot.vao);

			glDrawElements(GL_TRIANGLES, knot.count, GL_UNSIGNED_INT,
				glx::bufferOffset<GLuint>(0));

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			//pass2: bind tex from buffer, render quad

			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(frame.programHandle);

			glBindVertexArray(quad.vao);
			glBindTextureUnit(IMAGE_TEXTURE_BINDING, fbo.colourTexture);

			glDrawElements(GL_TRIANGLES, quad.count, GL_UNSIGNED_INT,
				glx::bufferOffset<GLuint>(0));

		}

		gui::newFrame(programData.windowData);
		ImGui::Begin("Filter Controls");
		if (ImGui::Button("Reset Camera"))
		{
			utils::MayaCamera::resetCamera(programData.camera);
		}

		std::vector<const char*> names{ "Normal", "Framebuffer" };
		ImGui::Combo("Render mode", &renderMode, names.data(),
			static_cast<int>(names.size()));
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			100.0f / ImGui::GetIO().Framerate,
			ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Render();
		gui::endFrame(programData.windowData, programData.renderData);

		glfwSwapBuffers(programData.window);
		glfwPollEvents();
	}

	gui::destroyGuiRenderData(programData.renderData);
	gui::destroyGuiWindow(programData.windowData);
	ImGui::DestroyContext();

	glx::destroyGLFWWindow(programData.window);
	glx::terminateGLFW();

	return 0;
}

void onError(int code, char const* message)
{
	fmt::print("error ({}): {}\n", code, message);
}

void onMouseDown(ProgramData& data, int button, int action, int mode,
	double xPos, double yPos)
{
	using namespace atlas::utils;

	gui::mousePressedCallback(data.windowData, button, action, mode);
	if (action == GLFW_PRESS)
	{
		math::Point2 point{ xPos, yPos };

		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			data.camera.movement = utils::MayaCamera::Movements::Tumble;
			MayaCamera::mouseDown(data.camera, point);
		}
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		{
			data.camera.movement = utils::MayaCamera::Movements::Track;
			MayaCamera::mouseDown(data.camera, point);
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			data.camera.movement = utils::MayaCamera::Movements::Dolly;
			MayaCamera::mouseDown(data.camera, point);
		}
	}
	else
	{
		MayaCamera::mouseUp(data.camera);
	}
}

void onMouseMove(ProgramData& data, double xPos, double yPos)
{
	utils::MayaCamera::mouseMove(data.camera, { xPos, yPos });
}

void onMouseScroll(ProgramData& data, double xOffset, double yOffset)
{
	gui::mouseScrollCallback(xOffset, yOffset);
	utils::MayaCamera::mouseScroll(data.camera, { xOffset, yOffset });
}

void onKeyPress(int key, int scancode, int action, int mods)
{
	gui::keyPressCallback(key, scancode, action, mods);
}

void onChar(unsigned int codepoint)
{
	gui::charCallback(codepoint);
}

void init(ProgramData& data)
{
	glx::WindowSettings settings;
	settings.size = { 1280, 720 };
	settings.title = "Assignment 3 Solution";

	glx::WindowCallbacks callbacks;
	callbacks.mousePressCallback = [&data](int button, int action, int mode,
		double xPos, double yPos) {
			onMouseDown(data, button, action, mode, xPos, yPos);
	};
	callbacks.mouseMoveCallback = [&data](double xPos, double yPos) {
		onMouseMove(data, xPos, yPos);
	};
	callbacks.mouseScrollCallback = [&data](double xOffset, double yOffset) {
		onMouseScroll(data, xOffset, yOffset);
	};
	callbacks.keyPressCallback = onKeyPress;
	callbacks.charCallback = onChar;

	glx::initializeGLFW(onError);
	data.window = glx::createGLFWWindow(settings);

	glx::bindWindowCallbacks(data.window, callbacks);
	glfwMakeContextCurrent(data.window);
	glfwSwapInterval(1);

	glx::createGLContext(data.window, settings.version);
	using namespace atlas::core;
	glx::initializeGLCallback(glx::ErrorSource::All, glx::ErrorType::All,
		glx::ErrorSeverity::High |
		glx::ErrorSeverity::Medium);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	gui::initializeGuiWindowData(data.windowData);
	gui::initializeGuiRenderData(data.renderData);
	gui::setGuiWindow(data.windowData, data.window);
}

Mesh loadMeshFromFile(std::string const& name)
{
	std::string path{ DataPath };
	path += name;
	auto mesh = utils::loadObjMesh(path).value();

	std::vector<float> data;
	for (auto& vertData : mesh.shapes[0].vertices)
	{
		data.push_back(vertData.position.x);
		data.push_back(vertData.position.y);
		data.push_back(vertData.position.z);

		data.push_back(vertData.normal.x);
		data.push_back(vertData.normal.y);
		data.push_back(vertData.normal.z);
	}

	std::vector<GLuint> indices(mesh.shapes[0].indices.size());
	std::transform(mesh.shapes[0].indices.begin(), mesh.shapes[0].indices.end(),
		indices.begin(), [](std::size_t i) -> GLuint {
			return static_cast<GLuint>(i);
		});

	GLuint vbo, ibo;
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, glx::size<float>(data.size()), data.data(), 0);

	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, glx::size<GLuint>(indices.size()), indices.data(),
		0);

	GLuint vao;
	glCreateVertexArrays(1, &vao);

	glVertexArrayVertexBuffer(vao, 0, vbo, 0, glx::stride<float>(6));
	glVertexArrayElementBuffer(vao, ibo);

	glEnableVertexArrayAttrib(vao, VERTEX_ATTRIB_LOCATION);
	glEnableVertexArrayAttrib(vao, NORMAL_ATTRIB_LOCATION);

	glVertexArrayAttribFormat(vao, VERTEX_ATTRIB_LOCATION, 3, GL_FLOAT,
		GL_FALSE, glx::relativeOffset<float>(0));
	glVertexArrayAttribFormat(vao, NORMAL_ATTRIB_LOCATION, 3, GL_FLOAT,
		GL_FALSE, glx::relativeOffset<float>(3));

	glVertexArrayAttribBinding(vao, VERTEX_ATTRIB_LOCATION, 0);
	glVertexArrayAttribBinding(vao, NORMAL_ATTRIB_LOCATION, 0);

	GLsizei count = static_cast<GLsizei>(mesh.shapes[0].indices.size());

	return { vao, vbo, ibo, count };
}

Mesh createQuad()
{
	// clang-format off
	std::vector<float> data
	{
		// Vertex Positions     Texture coordinates
		 1.0f,  1.0f, 0.0f,     1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,     0.0f, 1.0f

	};

	std::vector<GLuint> indices
	{
	   0, 1, 3,
	   1, 2, 3
	};
	// clang-format on

	GLuint vbo, ibo;
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, glx::size<float>(data.size()), data.data(), 0);

	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, glx::size<GLuint>(indices.size()), indices.data(),
		0);

	GLuint vao;
	glCreateVertexArrays(1, &vao);

	glVertexArrayVertexBuffer(vao, 0, vbo, 0, glx::stride<float>(5));
	glVertexArrayElementBuffer(vao, ibo);

	glEnableVertexArrayAttrib(vao, VERTEX_ATTRIB_LOCATION);
	glEnableVertexArrayAttrib(vao, TEXTURE_ATTRIB_LOCATION);

	glVertexArrayAttribFormat(vao, VERTEX_ATTRIB_LOCATION, 3, GL_FLOAT,
		GL_FALSE, glx::relativeOffset<float>(0));
	glVertexArrayAttribFormat(vao, TEXTURE_ATTRIB_LOCATION, 2, GL_FLOAT,
		GL_FALSE, glx::relativeOffset<float>(3));

	glVertexArrayAttribBinding(vao, VERTEX_ATTRIB_LOCATION, 0);
	glVertexArrayAttribBinding(vao, TEXTURE_ATTRIB_LOCATION, 0);

	GLsizei count = static_cast<GLsizei>(indices.size());

	return { vao, vbo, ibo, count };
}

Pipeline loadPipeline(std::string const& name)
{
	Pipeline p;
	p.vertexHandle = glCreateShader(GL_VERTEX_SHADER);
	p.fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
	p.programHandle = glCreateProgram();

	std::string root{ ShaderPath };
	std::string vertexFilename{ root + name + ".vert" };
	std::string fragmentFilename{ root + name + ".frag" };

	p.vertexSource = glx::readShaderSource(vertexFilename, p.includeDirs);
	p.fragmentSource = glx::readShaderSource(fragmentFilename, p.includeDirs);

	if (auto res =
		glx::compileShader(p.vertexSource.sourceString, p.vertexHandle);
		res)
	{
		fmt::print("{}\n", glx::parseErrorLog(p.vertexSource, *res));
		std::abort();
	}

	if (auto res =
		glx::compileShader(p.fragmentSource.sourceString, p.fragmentHandle);
		res)
	{
		fmt::print("{}\n", glx::parseErrorLog(p.fragmentSource, *res));
		std::abort();
	}

	glAttachShader(p.programHandle, p.vertexHandle);
	glAttachShader(p.programHandle, p.fragmentHandle);

	if (auto res = glx::linkShaders(p.programHandle); res)
	{
		fmt::print("{}\n", *res);
		std::abort();
	}

	return p;
}

UniformMatrices createUniformMatrices()
{
	UniformMatrices mat;
	glCreateBuffers(1, &mat.ubo);
	glNamedBufferStorage(mat.ubo, glx::size<math::Matrix4>(mat.count), nullptr,
		GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_UNIFORM_BUFFER, MATRIX_BUFFER_BINDING, mat.ubo);
	return mat;
}

UniformLights createUniformLights()
{
	UniformLights lights;
	glCreateBuffers(1, &lights.ubo);
	glNamedBufferStorage(lights.ubo, glx::size<math::Vector4>(lights.count),
		nullptr, GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_BUFFER_BINDING, lights.ubo);
	return lights;
}

Framebuffer createFramebuffer() // hold colour(texture or render buffer) and
								// depth(render buffer)
{
	GLuint fbo;
	glCreateFramebuffers(1, &fbo);

	GLuint colourMap;
	glCreateTextures(GL_TEXTURE_2D, 1, &colourMap);

	glTextureParameteri(colourMap, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(colourMap, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureParameteri(colourMap, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(colourMap, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureStorage2D(colourMap, 1, GL_RGBA8, 1280, 720); // change this size to match size of window

	glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, colourMap,
		0); // texture contiained in this box

	GLuint depthBuffer;
	glCreateRenderbuffers(1, &depthBuffer);
	glNamedRenderbufferStorage(depthBuffer, GL_DEPTH_COMPONENT24, 1280, 720);

	glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer); // attatch to frambuffer

	// check if framebuffer is complete
	if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fmt::print("Incomplete framebuffer\n");
	}

	return { fbo, colourMap, depthBuffer };
}

void reloadPipeline(Pipeline& p)
{
	if (glx::shouldShaderBeReloaded(p.vertexSource))
	{
		glx::reloadShader(p.programHandle, p.vertexHandle, p.vertexSource,
			p.includeDirs);
	}

	if (glx::shouldShaderBeReloaded(p.fragmentSource))
	{
		glx::reloadShader(p.programHandle, p.fragmentHandle, p.fragmentSource,
			p.includeDirs);
	}
}

void updateUniformMatrices(UniformMatrices const& matrices, int width,
	int height, ProgramData const& data)
{
	static constexpr float nearVal{ 1.0f };
	static constexpr float farVal{ 10000000000.0f };
	auto projection =
		glm::perspective(glm::radians(data.camera.fov),
			static_cast<float>(width) / height, nearVal, farVal);
	auto view = utils::MayaCamera::computeViewMatrix(data.camera);
	auto model =
		glm::scale(math::Matrix4{ 1.0f }, math::Vector{ 1.0f, 1.0f, 1.0f });

	std::vector<math::Matrix4> uniformMatrices(matrices.count);
	uniformMatrices[0] = model;
	uniformMatrices[1] = view * model;
	uniformMatrices[2] = projection * view * model;
	uniformMatrices[3] = glm::transpose(glm::inverse(model));

	glNamedBufferSubData(matrices.ubo, 0,
		glx::size<math::Matrix4>(matrices.count),
		uniformMatrices.data());
}

void updateUniformLights(UniformLights const& lights, ProgramData const& data)
{
	const math::Vector4 lightPos{ 10000.0f };//light position
	auto view = utils::MayaCamera::computeViewMatrix(data.camera);
	math::Vector4 cameraPos = glm::inverse(view) * math::Vector4{ 0, 0, 0, 1 };

	std::vector<math::Vector4> lightData{ lightPos, cameraPos };
	glNamedBufferSubData(lights.ubo, 0,
		glx::size<math::Vector4>(lightData.size()),
		lightData.data());
}
=======
#include "Paths.hpp"
#include "UniformLocations.glsl"
#include "assignment.hpp"
#include <fmt/printf.h>

int main()
{
	ProgramData programData;
	init(programData);

	auto knot = loadMeshFromFile("torusknot.obj");
	auto quad = createQuad();
	auto shaded = loadPipeline("shaded");
	auto frame = loadPipeline("fbo");
	auto fbo = createFramebuffer();
	auto uniformMatrices = createUniformMatrices();
	auto uniformLights = createUniformLights();

	glEnable(GL_DEPTH_TEST);
	int renderMode{ 0 };
	while (!glfwWindowShouldClose(programData.window))
	{
		int width, height;
		glfwGetFramebufferSize(programData.window, &width, &height);
		glViewport(0, 0, width, height);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		reloadPipeline(shaded);
		reloadPipeline(frame);
		updateUniformMatrices(uniformMatrices, width, height, programData);
		updateUniformLights(uniformLights, programData);

		if (renderMode == 0)
		{
			glUseProgram(shaded.programHandle);
			glBindVertexArray(knot.vao);
			glDrawElements(GL_TRIANGLES, knot.count, GL_UNSIGNED_INT,
				glx::bufferOffset<GLuint>(0));
		}
		else if (renderMode == 1)
		{
			// pass1: bind, clear, render to buffer, bind default back
			glBindFramebuffer(GL_FRAMEBUFFER, fbo.handle);
			glViewport(0, 0, width, height);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// if only care for depth just clear depth not color

			glUseProgram(shaded.programHandle);

			glBindVertexArray(knot.vao);

			glDrawElements(GL_TRIANGLES, knot.count, GL_UNSIGNED_INT,
				glx::bufferOffset<GLuint>(0));

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			//pass2: bind tex from buffer, render quad

			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(frame.programHandle);

			glBindVertexArray(quad.vao);
			glBindTextureUnit(IMAGE_TEXTURE_BINDING, fbo.colourTexture);

			glDrawElements(GL_TRIANGLES, quad.count, GL_UNSIGNED_INT,
				glx::bufferOffset<GLuint>(0));

		}

		gui::newFrame(programData.windowData);
		ImGui::Begin("Filter Controls");
		if (ImGui::Button("Reset Camera"))
		{
			utils::MayaCamera::resetCamera(programData.camera);
		}

		std::vector<const char*> names{ "Normal", "Framebuffer" };
		ImGui::Combo("Render mode", &renderMode, names.data(),
			static_cast<int>(names.size()));
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			100.0f / ImGui::GetIO().Framerate,
			ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Render();
		gui::endFrame(programData.windowData, programData.renderData);

		glfwSwapBuffers(programData.window);
		glfwPollEvents();
	}

	gui::destroyGuiRenderData(programData.renderData);
	gui::destroyGuiWindow(programData.windowData);
	ImGui::DestroyContext();

	glx::destroyGLFWWindow(programData.window);
	glx::terminateGLFW();

	return 0;
}

void onError(int code, char const* message)
{
	fmt::print("error ({}): {}\n", code, message);
}

void onMouseDown(ProgramData& data, int button, int action, int mode,
	double xPos, double yPos)
{
	using namespace atlas::utils;

	gui::mousePressedCallback(data.windowData, button, action, mode);
	if (action == GLFW_PRESS)
	{
		math::Point2 point{ xPos, yPos };

		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			data.camera.movement = utils::MayaCamera::Movements::Tumble;
			MayaCamera::mouseDown(data.camera, point);
		}
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		{
			data.camera.movement = utils::MayaCamera::Movements::Track;
			MayaCamera::mouseDown(data.camera, point);
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			data.camera.movement = utils::MayaCamera::Movements::Dolly;
			MayaCamera::mouseDown(data.camera, point);
		}
	}
	else
	{
		MayaCamera::mouseUp(data.camera);
	}
}

void onMouseMove(ProgramData& data, double xPos, double yPos)
{
	utils::MayaCamera::mouseMove(data.camera, { xPos, yPos });
}

void onMouseScroll(ProgramData& data, double xOffset, double yOffset)
{
	gui::mouseScrollCallback(xOffset, yOffset);
	utils::MayaCamera::mouseScroll(data.camera, { xOffset, yOffset });
}

void onKeyPress(int key, int scancode, int action, int mods)
{
	gui::keyPressCallback(key, scancode, action, mods);
}

void onChar(unsigned int codepoint)
{
	gui::charCallback(codepoint);
}

void init(ProgramData& data)
{
	glx::WindowSettings settings;
	settings.size = { 1280, 720 };
	settings.title = "Assignment 3 Solution";

	glx::WindowCallbacks callbacks;
	callbacks.mousePressCallback = [&data](int button, int action, int mode,
		double xPos, double yPos) {
			onMouseDown(data, button, action, mode, xPos, yPos);
	};
	callbacks.mouseMoveCallback = [&data](double xPos, double yPos) {
		onMouseMove(data, xPos, yPos);
	};
	callbacks.mouseScrollCallback = [&data](double xOffset, double yOffset) {
		onMouseScroll(data, xOffset, yOffset);
	};
	callbacks.keyPressCallback = onKeyPress;
	callbacks.charCallback = onChar;

	glx::initializeGLFW(onError);
	data.window = glx::createGLFWWindow(settings);

	glx::bindWindowCallbacks(data.window, callbacks);
	glfwMakeContextCurrent(data.window);
	glfwSwapInterval(1);

	glx::createGLContext(data.window, settings.version);
	using namespace atlas::core;
	glx::initializeGLCallback(glx::ErrorSource::All, glx::ErrorType::All,
		glx::ErrorSeverity::High |
		glx::ErrorSeverity::Medium);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	gui::initializeGuiWindowData(data.windowData);
	gui::initializeGuiRenderData(data.renderData);
	gui::setGuiWindow(data.windowData, data.window);
}

Mesh loadMeshFromFile(std::string const& name)
{
	std::string path{ DataPath };
	path += name;
	auto mesh = utils::loadObjMesh(path).value();

	std::vector<float> data;
	for (auto& vertData : mesh.shapes[0].vertices)
	{
		data.push_back(vertData.position.x);
		data.push_back(vertData.position.y);
		data.push_back(vertData.position.z);

		data.push_back(vertData.normal.x);
		data.push_back(vertData.normal.y);
		data.push_back(vertData.normal.z);
	}

	std::vector<GLuint> indices(mesh.shapes[0].indices.size());
	std::transform(mesh.shapes[0].indices.begin(), mesh.shapes[0].indices.end(),
		indices.begin(), [](std::size_t i) -> GLuint {
			return static_cast<GLuint>(i);
		});

	GLuint vbo, ibo;
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, glx::size<float>(data.size()), data.data(), 0);

	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, glx::size<GLuint>(indices.size()), indices.data(),
		0);

	GLuint vao;
	glCreateVertexArrays(1, &vao);

	glVertexArrayVertexBuffer(vao, 0, vbo, 0, glx::stride<float>(6));
	glVertexArrayElementBuffer(vao, ibo);

	glEnableVertexArrayAttrib(vao, VERTEX_ATTRIB_LOCATION);
	glEnableVertexArrayAttrib(vao, NORMAL_ATTRIB_LOCATION);

	glVertexArrayAttribFormat(vao, VERTEX_ATTRIB_LOCATION, 3, GL_FLOAT,
		GL_FALSE, glx::relativeOffset<float>(0));
	glVertexArrayAttribFormat(vao, NORMAL_ATTRIB_LOCATION, 3, GL_FLOAT,
		GL_FALSE, glx::relativeOffset<float>(3));

	glVertexArrayAttribBinding(vao, VERTEX_ATTRIB_LOCATION, 0);
	glVertexArrayAttribBinding(vao, NORMAL_ATTRIB_LOCATION, 0);

	GLsizei count = static_cast<GLsizei>(mesh.shapes[0].indices.size());

	return { vao, vbo, ibo, count };
}

Mesh createQuad()
{
	// clang-format off
	std::vector<float> data
	{
		// Vertex Positions     Texture coordinates
		 1.0f,  1.0f, 0.0f,     1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,     0.0f, 1.0f

	};

	std::vector<GLuint> indices
	{
	   0, 1, 3,
	   1, 2, 3
	};
	// clang-format on

	GLuint vbo, ibo;
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, glx::size<float>(data.size()), data.data(), 0);

	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, glx::size<GLuint>(indices.size()), indices.data(),
		0);

	GLuint vao;
	glCreateVertexArrays(1, &vao);

	glVertexArrayVertexBuffer(vao, 0, vbo, 0, glx::stride<float>(5));
	glVertexArrayElementBuffer(vao, ibo);

	glEnableVertexArrayAttrib(vao, VERTEX_ATTRIB_LOCATION);
	glEnableVertexArrayAttrib(vao, TEXTURE_ATTRIB_LOCATION);

	glVertexArrayAttribFormat(vao, VERTEX_ATTRIB_LOCATION, 3, GL_FLOAT,
		GL_FALSE, glx::relativeOffset<float>(0));
	glVertexArrayAttribFormat(vao, TEXTURE_ATTRIB_LOCATION, 2, GL_FLOAT,
		GL_FALSE, glx::relativeOffset<float>(3));

	glVertexArrayAttribBinding(vao, VERTEX_ATTRIB_LOCATION, 0);
	glVertexArrayAttribBinding(vao, TEXTURE_ATTRIB_LOCATION, 0);

	GLsizei count = static_cast<GLsizei>(indices.size());

	return { vao, vbo, ibo, count };
}

Pipeline loadPipeline(std::string const& name)
{
	Pipeline p;
	p.vertexHandle = glCreateShader(GL_VERTEX_SHADER);
	p.fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
	p.programHandle = glCreateProgram();

	std::string root{ ShaderPath };
	std::string vertexFilename{ root + name + ".vert" };
	std::string fragmentFilename{ root + name + ".frag" };

	p.vertexSource = glx::readShaderSource(vertexFilename, p.includeDirs);
	p.fragmentSource = glx::readShaderSource(fragmentFilename, p.includeDirs);

	if (auto res =
		glx::compileShader(p.vertexSource.sourceString, p.vertexHandle);
		res)
	{
		fmt::print("{}\n", glx::parseErrorLog(p.vertexSource, *res));
		std::abort();
	}

	if (auto res =
		glx::compileShader(p.fragmentSource.sourceString, p.fragmentHandle);
		res)
	{
		fmt::print("{}\n", glx::parseErrorLog(p.fragmentSource, *res));
		std::abort();
	}

	glAttachShader(p.programHandle, p.vertexHandle);
	glAttachShader(p.programHandle, p.fragmentHandle);

	if (auto res = glx::linkShaders(p.programHandle); res)
	{
		fmt::print("{}\n", *res);
		std::abort();
	}

	return p;
}

UniformMatrices createUniformMatrices()
{
	UniformMatrices mat;
	glCreateBuffers(1, &mat.ubo);
	glNamedBufferStorage(mat.ubo, glx::size<math::Matrix4>(mat.count), nullptr,
		GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_UNIFORM_BUFFER, MATRIX_BUFFER_BINDING, mat.ubo);
	return mat;
}

UniformLights createUniformLights()
{
	UniformLights lights;
	glCreateBuffers(1, &lights.ubo);
	glNamedBufferStorage(lights.ubo, glx::size<math::Vector4>(lights.count),
		nullptr, GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_BUFFER_BINDING, lights.ubo);
	return lights;
}

Framebuffer createFramebuffer() // hold colour(texture or render buffer) and
								// depth(render buffer)
{
	GLuint fbo;
	glCreateFramebuffers(1, &fbo);

	GLuint colourMap;
	glCreateTextures(GL_TEXTURE_2D, 1, &colourMap);

	glTextureParameteri(colourMap, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(colourMap, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureParameteri(colourMap, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(colourMap, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureStorage2D(colourMap, 1, GL_RGBA8, 1280, 720); // change this size to match size of window

	glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, colourMap,
		0); // texture contiained in this box

	GLuint depthBuffer;
	glCreateRenderbuffers(1, &depthBuffer);
	glNamedRenderbufferStorage(depthBuffer, GL_DEPTH_COMPONENT24, 1280, 720);

	glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer); // attatch to frambuffer

	// check if framebuffer is complete
	if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fmt::print("Incomplete framebuffer\n");
	}

	return { fbo, colourMap, depthBuffer };
}

void reloadPipeline(Pipeline& p)
{
	if (glx::shouldShaderBeReloaded(p.vertexSource))
	{
		glx::reloadShader(p.programHandle, p.vertexHandle, p.vertexSource,
			p.includeDirs);
	}

	if (glx::shouldShaderBeReloaded(p.fragmentSource))
	{
		glx::reloadShader(p.programHandle, p.fragmentHandle, p.fragmentSource,
			p.includeDirs);
	}
}

void updateUniformMatrices(UniformMatrices const& matrices, int width,
	int height, ProgramData const& data)
{
	static constexpr float nearVal{ 1.0f };
	static constexpr float farVal{ 10000000000.0f };
	auto projection =
		glm::perspective(glm::radians(data.camera.fov),
			static_cast<float>(width) / height, nearVal, farVal);
	auto view = utils::MayaCamera::computeViewMatrix(data.camera);
	auto model =
		glm::scale(math::Matrix4{ 1.0f }, math::Vector{ 1.0f, 1.0f, 1.0f });

	std::vector<math::Matrix4> uniformMatrices(matrices.count);
	uniformMatrices[0] = model;
	uniformMatrices[1] = view * model;
	uniformMatrices[2] = projection * view * model;
	uniformMatrices[3] = glm::transpose(glm::inverse(model));

	glNamedBufferSubData(matrices.ubo, 0,
		glx::size<math::Matrix4>(matrices.count),
		uniformMatrices.data());
}

void updateUniformLights(UniformLights const& lights, ProgramData const& data)
{
	const math::Vector4 lightPos{ 10000.0f };//light position
	auto view = utils::MayaCamera::computeViewMatrix(data.camera);
	math::Vector4 cameraPos = glm::inverse(view) * math::Vector4{ 0, 0, 0, 1 };

	std::vector<math::Vector4> lightData{ lightPos, cameraPos };
	glNamedBufferSubData(lights.ubo, 0,
		glx::size<math::Vector4>(lightData.size()),
		lightData.data());
}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
