#include <iostream>
#include <vector>
#include "OpenGL.h"
#include "glm/glm.hpp"
#include "VRMultithreadedApp.h"
using namespace MinVR;

/**
 * MyVRApp is an example of a modern OpenGL using VBOs, VAOs, and shaders.  MyVRApp inherits
 * from VRGraphicsApp, which allows you to override onVREvent to get input events, onRenderContext
 * to setup context sepecific objects, and onRenderScene that renders to each viewport.
 */
class MyVRApp : public VRMultithreadedApp {
public:
    MyVRApp(int argc, char** argv) : VRMultithreadedApp(argc, argv) {
        nodes.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        nodes.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
        nodes.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        nodes.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        nodes.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        nodes.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(5);

    }


    /// onVREvent is called when a new intput event happens.
    void onVREvent(const VRDataIndex &event) {

        //event.printStructure();

        // Set time since application began
        if (event.getName() == "FrameStart") {
            float time = event.getValue("ElapsedSeconds");
            // Calculate model matrix based on time
            VRMatrix4 modelMatrix = VRMatrix4::rotationX(0.5f*time);
            modelMatrix = modelMatrix * VRMatrix4::rotationY(0.5f*time);
            for (int f = 0; f < 16; f++) {
                model[f] = modelMatrix.getArray()[f];
            }
            return;
        }

        // Quit if the escape button is pressed
        if (event.getName() == "KbdEsc_Down") {
            running = false;
        }
    }

    class Context : public VRAppSharedContext {
    public:
        Context(const MyVRApp& app, const VRGraphicsState &renderState) : app(app) {
                glewExperimental = GL_TRUE;
                GLenum err = glewInit();
                if (GLEW_OK != err)
                {
                    std::cout << "Error initializing GLEW." << std::endl;
                }

                glGenBuffers(1, &elementBuffer);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, app.indices.size() * sizeof(unsigned int), &app.indices[0], GL_STATIC_DRAW);

                // Allocate space and send Vertex Data
                glGenBuffers(1, &vbo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, 3*sizeof(float)*(app.nodes.size() + app.normals.size() + app.colors.size()), 0, GL_DYNAMIC_DRAW);
                glBufferSubData(GL_ARRAY_BUFFER, 0, 3*sizeof(float)*(app.nodes.size()), &app.nodes[0]);
                glBufferSubData(GL_ARRAY_BUFFER, 3*sizeof(float)*app.nodes.size(), 3*sizeof(float)*app.normals.size(), &app.normals[0]);
                glBufferSubData(GL_ARRAY_BUFFER, 3*sizeof(float)*(app.nodes.size() + app.normals.size()), 3*sizeof(float)*app.colors.size(), &app.colors[0]);
        }

        ~Context() {
                glDeleteBuffers(1, &vbo);
        }

        void update(const VRGraphicsState &renderState) {
        }

        GLuint getVbo() { return vbo; }
        GLuint getElementBuffer() { return elementBuffer; }

    private:
        const MyVRApp& app;
        GLuint vbo;
        GLuint elementBuffer;
    };

    class Renderer : public VRAppRenderer {
    public:
        Renderer(const MyVRApp& app, Context& sharedContext, const VRGraphicsState &renderState) : app(app), sharedContext(sharedContext) {   
                // Init GL
                glEnable(GL_DEPTH_TEST);
                glClearDepth(1.0f);
                glDepthFunc(GL_LEQUAL);
                glClearColor(0, 0, 0, 1);

                // Create vao
                glGenVertexArrays(1, &vao);
                glBindVertexArray(vao);
                glBindBuffer(GL_ARRAY_BUFFER, sharedContext.getVbo());
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sharedContext.getElementBuffer());
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (char*)0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (char*)0 + 3*sizeof(float)*app.nodes.size());
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (char*)0 + 3*sizeof(float)*(app.nodes.size() + app.normals.size()));

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
                        "   gl_Position = ProjectionMatrix*ViewMatrix*ModelMatrix*vec4(position, 1.0); "
                        "   col = color;"
                        "}";
                vshader = compileShader(vertexShader, GL_VERTEX_SHADER);

                std::string fragmentShader =
                        "#version 330 \n"
                        "in vec3 col;"
                        "out vec4 colorOut;"
                        ""
                        "void main() { "
                        "   colorOut = vec4(col, 1.0); "
                        "}";
                fshader = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

                // Create shader program
                shaderProgram = glCreateProgram();
                glAttachShader(shaderProgram, vshader);
                glAttachShader(shaderProgram, fshader);
                linkShaderProgram(shaderProgram);
        }

        virtual ~Renderer() {
                glDeleteVertexArrays(1, &vao);
                glDetachShader(shaderProgram, vshader);
                glDetachShader(shaderProgram, fshader);
                glDeleteShader(vshader);
                glDeleteShader(fshader);
                glDeleteProgram(shaderProgram);
        }

        void update(const MinVR::VRGraphicsState &renderState) {
        }

        void render(const MinVR::VRGraphicsState &renderState) {
            const float* projMat = renderState.getProjectionMatrix();
            const float* viewMat = renderState.getViewMatrix();

            // clear screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            // Set shader parameters
            glUseProgram(shaderProgram);
            GLint loc = glGetUniformLocation(shaderProgram, "ProjectionMatrix");
            glUniformMatrix4fv(loc, 1, GL_FALSE, projMat);
            loc = glGetUniformLocation(shaderProgram, "ViewMatrix");
            glUniformMatrix4fv(loc, 1, GL_FALSE, viewMat);
            loc = glGetUniformLocation(shaderProgram, "ModelMatrix");
            glUniformMatrix4fv(loc, 1, GL_FALSE, app.model);

            // Draw cube
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, app.indices.size(), GL_UNSIGNED_INT, (void*)0);
            glBindVertexArray(0);

            // reset program
            glUseProgram(0);
        }

        /// Compiles shader
        GLuint compileShader(const std::string& shaderText, GLuint shaderType) {
            const char* source = shaderText.c_str();
            int length = (int)shaderText.size();
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
        const MyVRApp& app;
        GLuint vao, vshader, fshader, shaderProgram;
        const Context& sharedContext;
    };

    VRAppSharedContext* createSharedContext(const VRGraphicsState &renderState) {
        return new Context(*this, renderState);
    }

    VRAppRenderer* createRenderer(VRAppSharedContext& sharedContext, const VRGraphicsState &renderState) {
        return new Renderer(*this, *static_cast<Context*>(&sharedContext), renderState);
    }

private:
    float model[16];
    VRMain *vrMain;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> nodes;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colors;
};

/// Main method which creates and calls application
int main(int argc, char **argv) {
    MyVRApp app(argc, argv);
    app.run();
    return 0;
}