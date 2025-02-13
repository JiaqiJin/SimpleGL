#include "Window.h"
#include "common.h"

namespace Kawaii
{
    GLFWwindow* Window::m_window = nullptr;
    std::string  Window::m_title = "";
    glm::vec2    Window::m_window_size = glm::vec2(0);
    glm::mat4    Window::m_viewport_matrix = glm::mat4(1.0);

    Window::Window()
    {
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::createWindow(unsigned int width, unsigned int height, const std::string& title)
    {
        m_title = title;
        m_window_size = glm::vec2(width, height);

        if (!glfwInit())
        {
            std::cerr << "ERROR: Could not initialize GLFW." << std::endl;
            exit(EXIT_FAILURE);
        }

        glfwSetErrorCallback(error_callback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MIN_GL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MIN_GL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef _DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

        m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        if (!m_window)
        {
            std::cerr << "ERROR: Could not create window and OpenGL context." << std::endl;

            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(m_window);

        /* Initialize GLAD */
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "ERROR: Could not initialize GLAD." << std::endl;
            exit(EXIT_FAILURE);
        }

        /* Set the viewport */
        glViewport(0, 0, width, height);
        setViewportMatrix(width, height);

        setVSync(false);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    }

    void Window::endFrame()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }

    int Window::isCloseRequested()
    {
        return glfwWindowShouldClose(m_window);
    }

    int Window::getWidth()
    {
        return int(m_window_size.x);
    }

    int Window::getHeight()
    {
        return  int(m_window_size.y);
    }

    glm::vec2 Window::getCenter()
    {
        return m_window_size / 2.0f;
    }

    glm::vec2 Window::getSize()
    {
        return m_window_size;
    }

    float Window::getAspectRatio()
    {
        return (m_window_size.x / m_window_size.y);
    }

    glm::mat4 Window::getViewportMatrix()
    {
        return m_viewport_matrix;
    }

    const std::string& Window::getTitle()
    {
        return m_title;
    }

    void Window::setVSync(bool enabled)
    {
        auto value = enabled ? 1 : 0;

        glfwSwapInterval(value);
    }

    void Window::bindDefaultFramebuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, m_window_size.x, m_window_size.y);
    }

    void Window::setViewportMatrix(int width, int height)
    {
        float w2 = width / 2.0f;
        float h2 = height / 2.0f;

        m_viewport_matrix = glm::mat4(glm::vec4(w2, 0.0, 0.0, 0.0),
            glm::vec4(0.0, h2, 0.0, 0.0),
            glm::vec4(0.0, 0.0, 1.0, 0.0),
            glm::vec4(w2, h2, 0.0, 1.0));
    }

    void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        setViewportMatrix(width, height);

        m_window_size.x = float(width);
        m_window_size.y = float(height);
    }
}