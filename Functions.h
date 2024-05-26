#include<iostream>
#include <Windows.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>


GLuint LoadTexture(const char* filename);

void create_gear(GLfloat vertices[], GLuint indices[], int poczatkowy_punkt_vertices, int poczatkowy_punkt_indices, float x, float y, float kolor_x, float kolor_y, float kolor_z, float radius, float rotation);

void create_bolt(float vertices[], GLuint elements[], int poczatkowy_punkt_vertices, int poczatkowy_punkt_elements, float x, float y, float kolor_x, float kolor_y, float kolor_z, float radius, int wierzcholki, float rotation, int ilosczebatek, int ilosckolek);
