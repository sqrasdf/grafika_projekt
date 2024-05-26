#include<iostream>
#include <Windows.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"
#include"Functions.h"


#define PI 3.14159265358979323846
#define N 20

using namespace std;

const unsigned int width = 800;
const unsigned int height = 800;

int obiekty = 0;


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "the most average computer graphics final project", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, width, height);

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();
	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, -1.0f, 5.0f));

	float radius = 0.4f;
	float radius2 = 0.3f;
	float x1 = -0.46f, y1 = -0.55f;
	float x2 = 0.46f, y2 = -0.45f;
	float x3 = 0.68f, y3 = -1.25f;
	float rotation = PI / 64;
	float rotation2 = PI / 19;
	float rotation3 = PI / 9;

	GLuint texture1 = LoadTexture("tex5.jpg");

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	while (!glfwWindowShouldClose(window)) {
		GLfloat vertices[(((N + 1) * 8) + (N * 8)) * 80];
		GLuint indices[(N * 2 * 3 * 2 + 3 * N + 6 * N + 3 * N) * 80];

		rotation = rotation + (PI / 64);
		rotation2 = rotation2 - (PI / 64);
		rotation3 = rotation3 + (PI / 64);

		create_gear(vertices, indices, 0, 0, x1, y1, 1.0f, 0.0f, 0.0f, radius, rotation);
		create_gear(vertices, indices, (((N + 1) * 8) + (N * 8)) * 2, N * 2 * 3 * 2 + (3 * N) + (6 * N) + (3 * N), x2, y2, 0.5f, 1.0f, 0.0f, radius, rotation2);
		create_gear(vertices, indices, (((N + 1) * 8) + (N * 8)) * 4, (N * 2 * 3 * 2 + (3 * N) + (6 * N) + (3 * N)) * 2, x3, y3, 0.0f, 1.0f, 1.0f, radius2, rotation3);

		create_bolt(vertices, indices, (((N + 1) * 8) + (N * 8)) * 6, (N * 2 * 3 * 2 + (3 * N) + (6 * N) + (3 * N)) * 3, x1, y1, 0.0f, 0.0f, 0.0f, 0.2f, 10, rotation, 3, 0);
		create_bolt(vertices, indices, (((N + 1) * 8) + (N * 8)) * 6 + ((N + 1) * 8 * 2), (N * 2 * 3 * 2 + (3 * N) + (6 * N) + (3 * N)) * 3 + (N * 4) * 3, x2, y2, 0.0f, 0.0f, 0.0f, 0.2f, 10, rotation2, 3, 1);
		create_bolt(vertices, indices, (((N + 1) * 8) + (N * 8)) * 6 + ((N + 1) * 8 * 2) * 2, (N * 2 * 3 * 2 + (3 * N) + (6 * N) + (3 * N)) * 3 + (N * 4) * 6, x3, y3, 0.0f, 0.0f, 0.0f, 0.15f, 10, rotation3, 3, 2);

		VBO VBO1(vertices, sizeof(vertices));
		EBO EBO1(indices, sizeof(indices));

		VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
		VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		VAO1.Unbind();
		VBO1.Unbind();
		EBO1.Unbind();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();
		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		shaderProgram.setInt("texture1", 0);

		shaderProgram.setVec3("lightPos", lightPos);
		shaderProgram.setVec3("viewPos", camera.Position);
		shaderProgram.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		Sleep(16);
	}

	VAO1.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

