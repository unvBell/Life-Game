#define GLFW_INCLUDE_GLU

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <thread>

#include "RendererGL.hpp"
#include "World.hpp"
#include "Matrix3x2.hpp"

#ifdef _DEBUG
#  pragma comment(lib, "opengl32.lib")
#  pragma comment(lib, "glu32.lib")
#  pragma comment(lib, "GLFW/glfw3_d.lib")
#else
#  pragma comment(lib, "opengl32.lib")
#  pragma comment(lib, "glu32.lib")
#  pragma comment(lib, "GLFW/glfw3.lib")
#endif

namespace {
	const int			width	= 1024;
	const int			height	= width*9/16;
	const std::string	title	= "glfw";

	void setMatrix(const Matrix3x2& m) {
		const double mat[16] = {
			m.a11, m.a12, 0, 0,
			m.a21, m.a22, 0, 0,
			    0,     0, 1, 0,
			m.a31, m.a32, 0, 1,
		};

		glLoadMatrixd(mat);
	}

	double wheel = 0;
}

int main() {
	using namespace std;

	try {
		//	initialize
		if(!glfwInit())
			throw runtime_error{ "glfwInit" };

		GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if(!window)
			throw runtime_error{ "glfwCreateWindow" };

		glfwMakeContextCurrent(window);

		//	set up gl
		glClearColor(0, 0, 0, 1);
		glMatrixMode(GL_PROJECTION);
		glOrtho(0, width, height, 0, 0, 1);

		glMatrixMode(GL_MODELVIEW);

		//	init lifegame
		auto mat = Matrix3x2::scaling(5, 5);
		World world{ make_unique<RendererGL>() };

		//	main loop
		double cursorX	= 0;
		double cursorY	= 0;
		double prevX	= 0;
		double prevY	= 0;

		//	zoom
		glfwSetScrollCallback(window, [](GLFWwindow* window, double x, double y) { wheel = y; });

		while(!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT);

			//	cursor
			prevX = cursorX;
			prevY = cursorY;
			glfwGetCursorPos(window, &cursorX, &cursorY);

			bool leftClick		= !!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
			bool rightClick		= !!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);
			bool middleClick	= !!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3);

			//	paint cell
			if(leftClick || rightClick) {
				//	screen -> world
				auto matInv = mat.invert().get();
				double x = matInv.a11*cursorX + matInv.a21*cursorY + matInv.a31;
				double y = matInv.a12*cursorX + matInv.a22*cursorY + matInv.a32;

				world.set((int)floor(x), (int)floor(y), leftClick);
			}

			//	move
			if(middleClick) {
				mat *= Matrix3x2::translation(cursorX-prevX, cursorY-prevY);
			}

			//	zoom
			double scale = pow(1.1, wheel);
			mat *= Matrix3x2::scaling(scale, scale, cursorX, cursorY);

			setMatrix(mat);

			if(glfwGetKey(window, GLFW_KEY_SPACE)) world.update();
			world.render();

			wheel = 0;

			glfwSwapBuffers(window);
			glfwPollEvents();

			this_thread::sleep_for(chrono::milliseconds{ 1 });
		}
	}
	catch(const std::exception& e) {
		cout << typeid(e).name()	<< endl;
		cout << e.what()			<< endl;
	}

	//	finalize
	glfwTerminate();
}
