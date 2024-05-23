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

#define PI 3.14159265358979323846

#define NUMER_WIERZCHOLKOW 20

using namespace std;

void stworz_zebatke(GLfloat vertices[], GLuint indices[], int poczatkowy_punkt_vertices, int poczatkowy_punkt_indices, float x, float y, float kolor_x, float kolor_y, float kolor_z, float promien, float obrot);
void stworz_kolo(float vertices[], GLuint elements[], int poczatkowy_punkt_vertices, int poczatkowy_punkt_elements, float x, float y, float kolor_x, float kolor_y, float kolor_z, float promien, int wierzcholki, float obrot, int ilosczebatek, int ilosckolek);

const unsigned int width = 800;
const unsigned int height = 800;

int obiekty = 0;



GLuint LoadTexture(const char* filename) {
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Załaduj obraz, stwórz teksturę i generuj mipmapy
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Ustaw parametry tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureID;
}

int main()
{
	// Initialize GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Okno OpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	glViewport(0, 0, width, height);

	Shader shaderProgram("default.vert", "default.frag");


	VAO VAO1;
	VAO1.Bind();

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, -1.0f, 5.0f));

	float promien = 0.4f;
	float promien2 = 0.3f;

	int licznik = 5;
	int licznik_elements = 0;

	float x1 = -0.46f, y1 = -0.55f;
	float x2 = 0.46f, y2 = -0.45f; //WSPOLRZEDNE ZEBATEK
	float x3 = 0.68f, y3 = -1.25f;


	float obrot = PI / 64;
	float obrot2 = PI / 19; //TUTAJ MOZNA SYNCHRONIZOWAC FAZE KOL ZEBATYCH
	float obrot3 = PI / 9;
	float obrot4 = PI / 9;

	GLuint texture1 = LoadTexture("tex.jpg");

	while (!glfwWindowShouldClose(window))
	{
		GLfloat vertices[(((NUMER_WIERZCHOLKOW + 1) * 8) + (NUMER_WIERZCHOLKOW * 8)) * 80];

		GLuint indices[(NUMER_WIERZCHOLKOW * 2 * 3 * 2 + 3 * NUMER_WIERZCHOLKOW + 6 * NUMER_WIERZCHOLKOW + 3 * NUMER_WIERZCHOLKOW) * 80];
		//razy dwa bo trzeba zrobic kolko plus zebatki czyli razem 20 trojkatow, razy trzy bo kazdy trojkat ma 3 punkty do polaczenia (razem 60)

		obrot = obrot + (PI / 64);
		obrot2 = obrot2 - (PI / 64);
		obrot3 = obrot3 + (PI / 64);

		stworz_zebatke(vertices, indices, 0, 0, x1, y1, 0.0f, 1.0f, 1.0f, promien, obrot);
		stworz_zebatke(vertices, indices, (((NUMER_WIERZCHOLKOW + 1) * 8) + (NUMER_WIERZCHOLKOW * 8)) * 2, NUMER_WIERZCHOLKOW * 2 * 3 * 2 + (3 * NUMER_WIERZCHOLKOW) + (6 * NUMER_WIERZCHOLKOW) + (3 * NUMER_WIERZCHOLKOW), x2, y2, 1.0f, 0.0f, 0.0f, promien, obrot2);
		stworz_zebatke(vertices, indices, (((NUMER_WIERZCHOLKOW + 1) * 8) + (NUMER_WIERZCHOLKOW * 8)) * 4, (NUMER_WIERZCHOLKOW * 2 * 3 * 2 + (3 * NUMER_WIERZCHOLKOW) + (6 * NUMER_WIERZCHOLKOW) + (3 * NUMER_WIERZCHOLKOW)) * 2, x3, y3, 1.0f, 1.0f, 0.0f, promien2, obrot3);


		stworz_kolo(vertices, indices, (((NUMER_WIERZCHOLKOW + 1) * 8) + (NUMER_WIERZCHOLKOW * 8)) * 6, (NUMER_WIERZCHOLKOW * 2 * 3 * 2 + (3 * NUMER_WIERZCHOLKOW) + (6 * NUMER_WIERZCHOLKOW) + (3 * NUMER_WIERZCHOLKOW)) * 3, x1, y1, 0.0f, 0.0f, 0.0f, 0.2f, 10, obrot, 3, 0);
		stworz_kolo(vertices, indices, (((NUMER_WIERZCHOLKOW + 1) * 8) + (NUMER_WIERZCHOLKOW * 8)) * 6 + ((NUMER_WIERZCHOLKOW + 1) * 8 * 2), (NUMER_WIERZCHOLKOW * 2 * 3 * 2 + (3 * NUMER_WIERZCHOLKOW) + (6 * NUMER_WIERZCHOLKOW) + (3 * NUMER_WIERZCHOLKOW)) * 3 + (NUMER_WIERZCHOLKOW * 4) * 3, x2, y2, 0.0f, 0.0f, 0.0f, 0.2f, 10, obrot2, 3, 1);
		stworz_kolo(vertices, indices, (((NUMER_WIERZCHOLKOW + 1) * 8) + (NUMER_WIERZCHOLKOW * 8)) * 6 + ((NUMER_WIERZCHOLKOW + 1) * 8 * 2) * 2, (NUMER_WIERZCHOLKOW * 2 * 3 * 2 + (3 * NUMER_WIERZCHOLKOW) + (6 * NUMER_WIERZCHOLKOW) + (3 * NUMER_WIERZCHOLKOW)) * 3 + (NUMER_WIERZCHOLKOW * 4) * 6, x3, y3, 0.0f, 0.0f, 0.0f, 0.15f, 10, obrot3, 3, 2);

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

		VAO1.Bind();

		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		glfwPollEvents();

		Sleep(16); //60 KLATEK NA SEKUNDE
	}

	VAO1.Delete();
	//VBO1.Delete();
	//EBO1.Delete();

	shaderProgram.Delete();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

void stworz_zebatke(GLfloat vertices[], GLuint elements[], int poczatkowy_punkt_vertices, int poczatkowy_punkt_elements, float x, float y, float kolor_x, float kolor_y, float kolor_z, float promien, float obrot)
{
	int licznik = poczatkowy_punkt_vertices;
	float promien_zebatek = promien * 1.3f;
	int poczatek = poczatkowy_punkt_elements;

	int nr_zebatki = 0;

	//PIERWSZA ZEBATKA

	vertices[licznik] = x; licznik++;
	vertices[licznik] = y; licznik++;
	vertices[licznik] = -0.1; licznik++; //wspolrzedna "z" ustawiona na sztywno
	vertices[licznik] = kolor_x; licznik++; //WPROWADZENIE KOORDYNATOW PUNKTU W SRODKU KOLA ABY LATWIEJ RYSOWALO SIE TROJKATY
	vertices[licznik] = kolor_y; licznik++;
	vertices[licznik] = kolor_z; licznik++;
	vertices[licznik] = 1.0f; licznik++;
	vertices[licznik] = 0.0f; licznik++; //textcord ktore nie sa potrzebne jezeli kolorujemy shaderami


	for (double i = 0; i < 2 * PI; i += ((2 * PI) / (NUMER_WIERZCHOLKOW))) //PIERWSZE KOŁO
	{
		vertices[licznik] = cos(i + obrot) * promien + x; licznik++;
		vertices[licznik] = sin(i + obrot) * promien + y; licznik++;
		vertices[licznik] = -0.1; licznik++;  //wspolrzedna "z" ustawiona na sztywno
		vertices[licznik] = kolor_x; licznik++;
		vertices[licznik] = kolor_y; licznik++;
		vertices[licznik] = kolor_z; licznik++;
		//vertices[licznik] = 0.0f; licznik++;
		//vertices[licznik] = 0.0f; licznik++; //textcord ktore nie sa potrzebne jezeli kolorujemy shaderami
		vertices[licznik] = (cos(i + obrot) + 1) / 2; licznik++;
		vertices[licznik] = (sin(i + obrot) + 1) / 2; licznik++;
	}

	int pom = 0;

	for (double i = 0; i < 2 * PI; i += ((2 * PI) / (NUMER_WIERZCHOLKOW * 4.0f))) //ZEBATKI WIERZCHOLKI
	{
		if (pom % 8 == 1 || pom % 8 == 3)
		{
			vertices[licznik] = (cos(i + obrot) * promien_zebatek) + x; licznik++;
			vertices[licznik] = (sin(i + obrot) * promien_zebatek) + y; licznik++;
			vertices[licznik] = -0.1; licznik++;
			vertices[licznik] = kolor_x; licznik++;
			vertices[licznik] = kolor_y; licznik++;
			vertices[licznik] = kolor_z; licznik++;
			vertices[licznik] = 0.0f; licznik++;
			vertices[licznik] = 0.0f; licznik++; //textcord ktore nie sa potrzebne jezeli kolorujemy shaderami
		}

		pom++;

	}

	//DRUGA ZEBATKA


	vertices[licznik] = x; licznik++;
	vertices[licznik] = y; licznik++;
	vertices[licznik] = 0.1; licznik++; //wspolrzedna "z" ustawiona na sztywno
	vertices[licznik] = kolor_x; licznik++; //WPROWADZENIE KOORDYNATOW PUNKTU W SRODKU KOLA ABY LATWIEJ RYSOWALO SIE TROJKATY
	vertices[licznik] = kolor_y; licznik++;
	vertices[licznik] = kolor_z; licznik++;
	vertices[licznik] = 0.0f; licznik++;
	vertices[licznik] = 0.0f; licznik++; //textcord ktore nie sa potrzebne jezeli kolorujemy shaderami


	for (double i = 0; i < 2 * PI; i += ((2 * PI) / (NUMER_WIERZCHOLKOW))) //PIERWSZE KOŁO
	{
		vertices[licznik] = cos(i + obrot) * promien + x; licznik++;
		vertices[licznik] = sin(i + obrot) * promien + y; licznik++;
		vertices[licznik] = 0.1; licznik++;  //wspolrzedna "z" ustawiona na sztywno
		vertices[licznik] = kolor_x; licznik++;
		vertices[licznik] = kolor_y; licznik++;
		vertices[licznik] = kolor_z; licznik++;
		vertices[licznik] = 0.0f; licznik++;
		vertices[licznik] = 0.0f; licznik++; //textcord ktore nie sa potrzebne jezeli kolorujemy shaderami
	}

	pom = 0;

	for (double i = 0; i < 2 * PI; i += ((2 * PI) / (NUMER_WIERZCHOLKOW * 4.0f))) //ZEBATKI WIERZCHOLKI
	{
		if (pom % 8 == 1 || pom % 8 == 3)
		{
			vertices[licznik] = (cos(i + obrot) * promien_zebatek) + x; licznik++;
			vertices[licznik] = (sin(i + obrot) * promien_zebatek) + y; licznik++;
			vertices[licznik] = 0.1; licznik++;
			vertices[licznik] = kolor_x; licznik++;
			vertices[licznik] = kolor_y; licznik++;
			vertices[licznik] = kolor_z; licznik++;
			vertices[licznik] = 0.0f; licznik++;
			vertices[licznik] = 0.0f; licznik++; //textcord ktore nie sa potrzebne jezeli kolorujemy shaderami
		}
		pom++;
	}


	//ELEMENTS

	licznik = poczatkowy_punkt_elements;

	for (int i = 0; i < 100; i++)
	{
		if (poczatkowy_punkt_elements <= i * (NUMER_WIERZCHOLKOW * 2 * 3 * 2 + 3 * NUMER_WIERZCHOLKOW + 6 * NUMER_WIERZCHOLKOW + 3 * NUMER_WIERZCHOLKOW))
		{
			nr_zebatki = i;
			break;
		}
	}

	//PIERWSZA ZEBATKA (kolko)
	for (int i = 1; i < NUMER_WIERZCHOLKOW; i++)
	{
		elements[licznik] = ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	}
	elements[licznik] = ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	elements[licznik] = 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	elements[licznik] = NUMER_WIERZCHOLKOW + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;

	//PIERWSZA ZEBATKA (trapezy)
	for (int i = 1; i < NUMER_WIERZCHOLKOW; i = i + 2)
	{
		elements[licznik] = i + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + NUMER_WIERZCHOLKOW + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	}
	for (int i = 2; i < NUMER_WIERZCHOLKOW + 1; i = i + 2)
	{
		elements[licznik] = i + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + NUMER_WIERZCHOLKOW + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + NUMER_WIERZCHOLKOW - 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	}

	//DRUGA ZEBATKA (kolko) 
	for (int i = 1; i < NUMER_WIERZCHOLKOW; i++)
	{
		elements[licznik] = ((NUMER_WIERZCHOLKOW * 2) + 1) + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + ((NUMER_WIERZCHOLKOW * 2) + 1) + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + 1 + ((NUMER_WIERZCHOLKOW * 2) + 1) + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	}
	elements[licznik] = ((NUMER_WIERZCHOLKOW * 2) + 1) + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	elements[licznik] = ((NUMER_WIERZCHOLKOW * 2) + 1) + 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	elements[licznik] = ((NUMER_WIERZCHOLKOW * 2) + 1) + NUMER_WIERZCHOLKOW + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;

	//DRUGA ZEBATKA (trapezy)
	for (int i = 1; i < NUMER_WIERZCHOLKOW; i = i + 2)
	{
		elements[licznik] = i + ((NUMER_WIERZCHOLKOW * 2) + 1) + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + 1 + ((NUMER_WIERZCHOLKOW * 2) + 1) + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + ((NUMER_WIERZCHOLKOW * 2) + 1) + NUMER_WIERZCHOLKOW + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	}
	for (int i = 2; i < NUMER_WIERZCHOLKOW + 1; i = i + 2)
	{
		elements[licznik] = i + ((NUMER_WIERZCHOLKOW * 2) + 1) + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + NUMER_WIERZCHOLKOW + ((NUMER_WIERZCHOLKOW * 2) + 1) + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + NUMER_WIERZCHOLKOW - 1 + ((NUMER_WIERZCHOLKOW * 2) + 1) + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	}


	//LACZENIE 3D TROJKATOW
	for (int i = NUMER_WIERZCHOLKOW + 1; i < NUMER_WIERZCHOLKOW * 2; i = i + 2)
	{
		elements[licznik] = i + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + 1 + (NUMER_WIERZCHOLKOW * 2) + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;

		elements[licznik] = i + 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + 1 + (NUMER_WIERZCHOLKOW * 2) + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + 2 + (NUMER_WIERZCHOLKOW * 2) + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	}

	for (int i = 1; i < NUMER_WIERZCHOLKOW + 1; i++)
	{
		elements[licznik] = i + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + NUMER_WIERZCHOLKOW + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + (NUMER_WIERZCHOLKOW * 2) + 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;

		elements[licznik] = i + NUMER_WIERZCHOLKOW + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + (NUMER_WIERZCHOLKOW * 2) + 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + (NUMER_WIERZCHOLKOW * 3) + 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;

	}

	for (int i = 2; i < NUMER_WIERZCHOLKOW; i = i + 2)
	{
		elements[licznik] = i + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + (NUMER_WIERZCHOLKOW * 2) + 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;

		elements[licznik] = i + 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + (NUMER_WIERZCHOLKOW * 2) + 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
		elements[licznik] = i + (NUMER_WIERZCHOLKOW * 2) + 2 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	}

	elements[licznik] = NUMER_WIERZCHOLKOW + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	elements[licznik] = 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	elements[licznik] = (NUMER_WIERZCHOLKOW * 3) + 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;

	elements[licznik] = 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	elements[licznik] = (NUMER_WIERZCHOLKOW * 3) + 1 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
	elements[licznik] = (NUMER_WIERZCHOLKOW * 2) + 2 + ((NUMER_WIERZCHOLKOW * 4) + 2) * nr_zebatki; licznik++;
}

void stworz_kolo(float vertices[], GLuint elements[], int poczatkowy_punkt_vertices, int poczatkowy_punkt_elements, float x, float y, float kolor_x, float kolor_y, float kolor_z, float promien, int wierzcholki, float obrot, int ilosczebatek, int ilosckolek)
{
	int licznik = poczatkowy_punkt_vertices;

	//PIERWSZE KOŁO

	vertices[licznik] = x; licznik++;
	vertices[licznik] = y; licznik++;
	vertices[licznik] = -0.1001; licznik++; //wspolrzedna "z" ustawiona na sztywno
	vertices[licznik] = kolor_x; licznik++; //WPROWADZENIE KOORDYNATOW PUNKTU W SRODKU KOLA ABY LATWIEJ RYSOWALO SIE TROJKATY
	vertices[licznik] = kolor_y; licznik++;
	vertices[licznik] = kolor_z; licznik++;
	vertices[licznik] = 0.0f; licznik++;
	vertices[licznik] = 0.0f; licznik++; //textcord ktore nie sa potrzebne jezeli kolorujemy shaderami

	for (double i = 0; i < 2 * PI; i += ((2 * PI) / (wierzcholki))) //KOŁO
	{
		vertices[licznik] = cos(i + obrot) * promien + x; licznik++;
		vertices[licznik] = sin(i + obrot) * promien + y; licznik++;
		vertices[licznik] = -0.1001; licznik++;
		vertices[licznik] = kolor_x; licznik++;
		vertices[licznik] = kolor_y; licznik++;
		vertices[licznik] = kolor_z; licznik++;
		vertices[licznik] = 0.0f; licznik++;
		vertices[licznik] = 0.0f; licznik++;
	}

	//DRUGIE KOLO

	vertices[licznik] = x; licznik++;
	vertices[licznik] = y; licznik++;
	vertices[licznik] = 0.1001; licznik++; //wspolrzedna "z" ustawiona na sztywno
	vertices[licznik] = kolor_x; licznik++; //WPROWADZENIE KOORDYNATOW PUNKTU W SRODKU KOLA ABY LATWIEJ RYSOWALO SIE TROJKATY
	vertices[licznik] = kolor_y; licznik++;
	vertices[licznik] = kolor_z; licznik++;
	vertices[licznik] = 0.0f; licznik++;
	vertices[licznik] = 0.0f; licznik++; //textcord ktore nie sa potrzebne jezeli kolorujemy shaderami

	for (double i = 0; i < 2 * PI; i += ((2 * PI) / (wierzcholki))) //KOŁO
	{
		vertices[licznik] = cos(i + obrot) * promien + x; licznik++;
		vertices[licznik] = sin(i + obrot) * promien + y; licznik++;
		vertices[licznik] = 0.1001; licznik++;
		vertices[licznik] = kolor_x; licznik++;
		vertices[licznik] = kolor_y; licznik++;
		vertices[licznik] = kolor_z; licznik++;
		vertices[licznik] = 0.0f; licznik++;
		vertices[licznik] = 0.0f; licznik++;
	}

	licznik = poczatkowy_punkt_elements;
	//LACZENIE PIERWSZEGO KOLKA

	int offset = (((NUMER_WIERZCHOLKOW * 4) + 2) * ilosczebatek + ((NUMER_WIERZCHOLKOW * 2) + 2) * ilosckolek); //WAZNE
	int offset2 = (((NUMER_WIERZCHOLKOW * 4) + 2) * ilosczebatek + ((NUMER_WIERZCHOLKOW * 2) + 2) * ilosckolek) + wierzcholki + 1;

	for (int i = 1; i < wierzcholki; i++)
	{
		elements[licznik] = offset; licznik++;
		elements[licznik] = i + offset; licznik++;
		elements[licznik] = i + 1 + offset; licznik++;
	}
	elements[licznik] = offset; licznik++;
	elements[licznik] = wierzcholki + offset; licznik++;
	elements[licznik] = 1 + offset; licznik++;

	//LACZENIE DRUGIEGO KOLKA
	for (int i = 1; i < wierzcholki; i++)
	{
		elements[licznik] = offset2; licznik++;
		elements[licznik] = i + offset2; licznik++;
		elements[licznik] = i + 1 + offset2; licznik++;
	}
	elements[licznik] = offset2; licznik++;
	elements[licznik] = wierzcholki + offset2; licznik++;
	elements[licznik] = 1 + offset2; licznik++;

	//LACZENIE KÓLEK ZE SOBA
	for (int i = 1; i < wierzcholki; i++)
	{
		elements[licznik] = i + offset; licznik++;
		elements[licznik] = i + 1 + offset; licznik++;
		elements[licznik] = i + offset2; licznik++;
	}
	elements[licznik] = wierzcholki + offset; licznik++;
	elements[licznik] = 1 + offset; licznik++;
	elements[licznik] = wierzcholki + offset2; licznik++;

	for (int i = 2; i < wierzcholki + 1; i++)
	{
		elements[licznik] = i + offset; licznik++;
		elements[licznik] = i - 1 + offset2; licznik++;
		elements[licznik] = i + offset2; licznik++;
	}
	elements[licznik] = 1 + offset; licznik++;
	elements[licznik] = 1 + offset2; licznik++;
	elements[licznik] = wierzcholki + offset2; licznik++;
}