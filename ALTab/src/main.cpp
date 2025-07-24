#include "Application.hpp"

int main() {
	glfwInit();
	GLFWmonitor* mon = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(mon);
	int w = mode->width, h = mode->height;
	glfwTerminate();

	Application app(w, h, "ALTab");
	return app.Run();
}