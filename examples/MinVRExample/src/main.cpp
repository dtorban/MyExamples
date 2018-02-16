#include <iostream>

#include "OpenGL.h"

#include <main/VREventHandler.h>
#include <main/VRGraphicsHandler.h>

// MinVR header
#include <main/VRMain.h>
using namespace MinVR;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MyApp : public VREventHandler, public VRGraphicsHandler {
public:
	MyApp(int argc, char **argv) : model(1.0) {
		vrmain = new VRMain();
		vrmain->initialize(argc, argv);
		vrmain->addEventHandler(this);
		vrmain->addRenderHandler(this);
	}

	~MyApp() {
		vrmain->shutdown();
		delete vrmain;
	}

	void onVREvent(const VREvent &event) {
		//event.print();

		// Set time since application began
		if (event.getName() == "FrameStart") {
			float time = event.getDataAsFloat("ElapsedSeconds");
			model = glm::rotate(glm::mat4(1), 0.5f*time, glm::vec3(1.0,0.0,0.0));
			model = glm::rotate(model, 0.5f*time, glm::vec3(0.0,1.0,0.0));
			return;
		}

		// Quit if the escape button is pressed
		if (event.getName() == "KbdEsc_Down") {
			running = false;
		}
	}

	void onVRRenderGraphicsContext(const VRGraphicsState& renderState) {
		// If this is the inital call, initialize context variables
		if (renderState.isInitialRenderCall()) {
			glewExperimental = GL_TRUE;
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				std::cout << "Error initializing GLEW." << std::endl;
			}

			// Init GL
			glEnable(GL_DEPTH_TEST);
			glClearDepth(1.0f);
			glDepthFunc(GL_LEQUAL);
			glClearColor(0, 0, 0, 1);

			// Create VBO
			GLfloat vertices[]  = { 1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,  -1.0f,-1.0f, 1.0f,      // v0-v1-v2 (front)
					-1.0f,-1.0f, 1.0f,   1.0f,-1.0f, 1.0f,   1.0f, 1.0f, 1.0f,      // v2-v3-v0

					1.0f, 1.0f, 1.0f,   1.0f,-1.0f, 1.0f,   1.0f,-1.0f,-1.0f,      // v0-v3-v4 (right)
					1.0f,-1.0f,-1.0f,   1.0f, 1.0f,-1.0f,   1.0f, 1.0f, 1.0f,      // v4-v5-v0

					1.0f, 1.0f, 1.0f,   1.0f, 1.0f,-1.0f,  -1.0f, 1.0f,-1.0f,      // v0-v5-v6 (top)
					-1.0f, 1.0f,-1.0f,  -1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,      // v6-v1-v0

					-1.0f, 1.0f, 1.0f,  -1.0f, 1.0f,-1.0f,  -1.0f,-1.0f,-1.0f,      // v1-v6-v7 (left)
					-1.0f,-1.0f,-1.0f,  -1.0f,-1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,      // v7-v2-v1.0

					-1.0f,-1.0f,-1.0f,   1.0f,-1.0f,-1.0f,   1.0f,-1.0f, 1.0f,      // v7-v4-v3 (bottom)
					1.0f,-1.0f, 1.0f,  -1.0f,-1.0f, 1.0f,  -1.0f,-1.0f,-1.0f,      // v3-v2-v7

					1.0f,-1.0f,-1.0f,  -1.0f,-1.0f,-1.0f,  -1.0f, 1.0f,-1.0f,      // v4-v7-v6 (back)
					-1.0f, 1.0f,-1.0f,   1.0f, 1.0f,-1.0f,   1.0f,-1.0f,-1.0f };    // v6-v5-v4

			// normal array
			GLfloat normals[]   = { 0, 0, 1,   0, 0, 1,   0, 0, 1,      // v0-v1-v2 (front)
					0, 0, 1,   0, 0, 1,   0, 0, 1,      // v2-v3-v0

					1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v3-v4 (right)
					1, 0, 0,   1, 0, 0,   1, 0, 0,      // v4-v5-v0

					0, 1, 0,   0, 1, 0,   0, 1, 0,      // v0-v5-v6 (top)
					0, 1, 0,   0, 1, 0,   0, 1, 0,      // v6-v1-v0

					-1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v1-v6-v7 (left)
					-1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v7-v2-v1

					0,-1, 0,   0,-1, 0,   0,-1, 0,      // v7-v4-v3 (bottom)
					0,-1, 0,   0,-1, 0,   0,-1, 0,      // v3-v2-v7

					0, 0,-1,   0, 0,-1,   0, 0,-1,      // v4-v7-v6 (back)
					0, 0,-1,   0, 0,-1,   0, 0,-1 };    // v6-v5-v4

			// color array
			GLfloat colors[]    = { 1, 1, 1,   1, 1, 0,   1, 0, 0,      // v0-v1-v2 (front)
					1, 0, 0,   1, 0, 1,   1, 1, 1,      // v2-v3-v0

					1, 1, 1,   1, 0, 1,   0, 0, 1,      // v0-v3-v4 (right)
					0, 0, 1,   0, 1, 1,   1, 1, 1,      // v4-v5-v0

					1, 1, 1,   0, 1, 1,   0, 1, 0,      // v0-v5-v6 (top)
					0, 1, 0,   1, 1, 0,   1, 1, 1,      // v6-v1-v0

					1, 1, 0,   0, 1, 0,   0, 0, 0,      // v1-v6-v7 (left)
					0, 0, 0,   1, 0, 0,   1, 1, 0,      // v7-v2-v1

					0, 0, 0,   0, 0, 1,   1, 0, 1,      // v7-v4-v3 (bottom)
					1, 0, 1,   1, 0, 0,   0, 0, 0,      // v3-v2-v7

					0, 0, 1,   0, 0, 0,   0, 1, 0,      // v4-v7-v6 (back)
					0, 1, 0,   0, 1, 1,   0, 0, 1 };    // v6-v5-v4

			// Allocate space and send Vertex Data
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)+sizeof(normals)+sizeof(colors), 0, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices)+sizeof(normals), sizeof(colors), colors);

			// Create vao
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (char*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (char*)0 + sizeof(vertices));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (char*)0 + sizeof(vertices) + sizeof(normals));

			// Create shader
			std::string vertexShader =
					"#version 330 \n"
					"layout(location = 0) in vec3 position; "
					"layout(location = 1) in vec3 normal; "
					"layout(location = 2) in vec3 color; "
					""
					"uniform mat4 ProjectionMatrix; "
					"uniform mat4 ViewMatrix; "
					"uniform mat4 ModelMatrix; "
					"uniform mat4 NormalMatrix; "
					""
					"out vec3 col;"
					""
					"void main() { "
					"	gl_Position = ProjectionMatrix*ViewMatrix*ModelMatrix*vec4(position, 1.0); "
					"	col = color;"
					"}";
			vshader = compileShader(vertexShader, GL_VERTEX_SHADER);

			std::string fragmentShader =
					"#version 330 \n"
					"in vec3 col;"
					"out vec4 colorOut;"
					""
					"void main() { "
					"	colorOut = vec4(col, 1.0); "
					"}";
			fshader = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

			// Create shader program
			shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vshader);
			glAttachShader(shaderProgram, fshader);
			linkShaderProgram(shaderProgram);
		}

		// Destroy context items if the program is no longer running
		if (!running) {
			glDeleteBuffers(1, &vbo);
			glDeleteVertexArrays(1, &vao);
			glDetachShader(shaderProgram, vshader);
			glDetachShader(shaderProgram, fshader);
			glDeleteShader(vshader);
			glDeleteShader(fshader);
			glDeleteProgram(shaderProgram);
			return;
		}
	}

	/// onVRRenderScene will run draw calls on each viewport inside a context.
	void onVRRenderGraphics(const VRGraphicsState &renderState) {
		// Only draw if the application is still running.
		if (running) {
			// clear screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// Set shader parameters
			glUseProgram(shaderProgram);
			GLint loc = glGetUniformLocation(shaderProgram, "ProjectionMatrix");
			glUniformMatrix4fv(loc, 1, GL_FALSE, renderState.getProjectionMatrix());
			loc = glGetUniformLocation(shaderProgram, "ViewMatrix");
			glUniformMatrix4fv(loc, 1, GL_FALSE, renderState.getViewMatrix());
			loc = glGetUniformLocation(shaderProgram, "ModelMatrix");
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));

			// Draw cube
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			// reset program
			glUseProgram(0);
		}
	}

	void run() {
		running = true;
		while (vrmain->mainloop() && running) {}
	}

	/// Compiles shader
	GLuint compileShader(const std::string& shaderText, GLuint shaderType) {
		const char* source = shaderText.c_str();
		int length = shaderText.size();
		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &source, &length);
		glCompileShader(shader);
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE) {
			GLint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> log(length);
			glGetShaderInfoLog(shader, length, &length, &log[0]);
			std::cerr << &log[0];
		}

		return shader;
	}

	/// links shader program
	void linkShaderProgram(GLuint shaderProgram) {
		glLinkProgram(shaderProgram);
		GLint status;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
		if(status == GL_FALSE) {
			GLint length;
			glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> log(length);
			glGetProgramInfoLog(shaderProgram, length, &length, &log[0]);
			std::cerr << "Error compiling program: " << &log[0] << std::endl;
		}
	}

private:
	GLuint vbo, vao, vshader, fshader, shaderProgram;
	glm::mat4 model;
	VRMain* vrmain;
	bool running;
};

/// Main method which creates and calls application
int main(int argc, char **argv) {
	MyApp app(argc, argv);
	app.run();

	return 0;
}
