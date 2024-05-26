#include"Functions.h"

#define PI 3.14159265358979323846
#define N 20


GLuint LoadTexture(const char* filename) {
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureID;
}


void create_gear(GLfloat vertices[], GLuint elements[], int poczatkowy_punkt_vertices, int poczatkowy_punkt_elements, float x, float y, float kolor_x, float kolor_y, float kolor_z, float radius, float rotation)
{
	int counter = poczatkowy_punkt_vertices;
	float radius_zebatek = radius * 1.3f;
	int poczatek = poczatkowy_punkt_elements;

	int nr_zebatki = 0;

	//PIERWSZA ZEBATKA

	vertices[counter] = x; counter++;
	vertices[counter] = y; counter++;
	vertices[counter] = -0.1; counter++; //wspolrzedna "z" ustawiona na sztywno
	vertices[counter] = kolor_x; counter++; //WPROWADZENIE KOORDYNATOW PUNKTU W SRODKU KOLA ABY LATWIEJ RYSOWALO SIE TROJKATY
	vertices[counter] = kolor_y; counter++;
	vertices[counter] = kolor_z; counter++;
	vertices[counter] = 1.0f; counter++;
	vertices[counter] = 0.0f; counter++; //textcord ktore nie sa potrzebne jezeli kolorujemy shaderami


	for (double i = 0; i < 2 * PI; i += ((2 * PI) / (N))) //PIERWSZE KO�O
	{
		vertices[counter] = cos(i + rotation) * radius + x; counter++;
		vertices[counter] = sin(i + rotation) * radius + y; counter++;
		vertices[counter] = -0.1; counter++;  //wspolrzedna "z" ustawiona na sztywno
		vertices[counter] = kolor_x; counter++;
		vertices[counter] = kolor_y; counter++;
		vertices[counter] = kolor_z; counter++;
		vertices[counter] = (cos(i + rotation) + 1) / 2; counter++;
		vertices[counter] = (sin(i + rotation) + 1) / 2; counter++;
	}

	int pom = 0;

	for (double i = 0; i < 2 * PI; i += ((2 * PI) / (N * 4.0f))) //ZEBATKI WIERZCHOLKI
	{
		if (pom % 8 == 1 || pom % 8 == 3)
		{
			vertices[counter] = (cos(i + rotation) * radius_zebatek) + x; counter++;
			vertices[counter] = (sin(i + rotation) * radius_zebatek) + y; counter++;
			vertices[counter] = -0.1; counter++;
			vertices[counter] = kolor_x; counter++;
			vertices[counter] = kolor_y; counter++;
			vertices[counter] = kolor_z; counter++;
			vertices[counter] = 0.0f; counter++;
			vertices[counter] = 0.0f; counter++; //textcord ktore nie sa potrzebne jezeli kolorujemy shaderami
		}

		pom++;

	}

	//DRUGA ZEBATKA


	vertices[counter] = x; counter++;
	vertices[counter] = y; counter++;
	vertices[counter] = 0.1; counter++; //wspolrzedna "z" ustawiona na sztywno
	vertices[counter] = kolor_x; counter++; //WPROWADZENIE KOORDYNATOW PUNKTU W SRODKU KOLA ABY LATWIEJ RYSOWALO SIE TROJKATY
	vertices[counter] = kolor_y; counter++;
	vertices[counter] = kolor_z; counter++;
	vertices[counter] = 0.0f; counter++;
	vertices[counter] = 0.0f; counter++; //textcord ktore nie sa potrzebne jezeli kolorujemy shaderami


	for (double i = 0; i < 2 * PI; i += ((2 * PI) / (N))) //PIERWSZE KO�O
	{
		vertices[counter] = cos(i + rotation) * radius + x; counter++;
		vertices[counter] = sin(i + rotation) * radius + y; counter++;
		vertices[counter] = 0.1; counter++;  //wspolrzedna "z" ustawiona na sztywno
		vertices[counter] = kolor_x; counter++;
		vertices[counter] = kolor_y; counter++;
		vertices[counter] = kolor_z; counter++;
		vertices[counter] = 0.0f; counter++;
		vertices[counter] = 0.0f; counter++; //textcord ktore nie sa potrzebne jezeli kolorujemy shaderami
	}

	pom = 0;

	for (double i = 0; i < 2 * PI; i += ((2 * PI) / (N * 4.0f))) //ZEBATKI WIERZCHOLKI
	{
		if (pom % 8 == 1 || pom % 8 == 3)
		{
			vertices[counter] = (cos(i + rotation) * radius_zebatek) + x; counter++;
			vertices[counter] = (sin(i + rotation) * radius_zebatek) + y; counter++;
			vertices[counter] = 0.1; counter++;
			vertices[counter] = kolor_x; counter++;
			vertices[counter] = kolor_y; counter++;
			vertices[counter] = kolor_z; counter++;
			vertices[counter] = 0.0f; counter++;
			vertices[counter] = 0.0f; counter++; //textcord ktore nie sa potrzebne jezeli kolorujemy shaderami
		}
		pom++;
	}


	//ELEMENTS

	counter = poczatkowy_punkt_elements;

	for (int i = 0; i < 100; i++)
	{
		if (poczatkowy_punkt_elements <= i * (N * 2 * 3 * 2 + 3 * N + 6 * N + 3 * N))
		{
			nr_zebatki = i;
			break;
		}
	}

	//PIERWSZA ZEBATKA (kolko)
	for (int i = 1; i < N; i++)
	{
		elements[counter] = ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + 1 + ((N * 4) + 2) * nr_zebatki; counter++;
	}
	elements[counter] = ((N * 4) + 2) * nr_zebatki; counter++;
	elements[counter] = 1 + ((N * 4) + 2) * nr_zebatki; counter++;
	elements[counter] = N + ((N * 4) + 2) * nr_zebatki; counter++;

	//PIERWSZA ZEBATKA (trapezy)
	for (int i = 1; i < N; i = i + 2)
	{
		elements[counter] = i + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + 1 + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + N + ((N * 4) + 2) * nr_zebatki; counter++;
	}
	for (int i = 2; i < N + 1; i = i + 2)
	{
		elements[counter] = i + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + N + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + N - 1 + ((N * 4) + 2) * nr_zebatki; counter++;
	}

	//DRUGA ZEBATKA (kolko) 
	for (int i = 1; i < N; i++)
	{
		elements[counter] = ((N * 2) + 1) + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + ((N * 2) + 1) + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + 1 + ((N * 2) + 1) + ((N * 4) + 2) * nr_zebatki; counter++;
	}
	elements[counter] = ((N * 2) + 1) + ((N * 4) + 2) * nr_zebatki; counter++;
	elements[counter] = ((N * 2) + 1) + 1 + ((N * 4) + 2) * nr_zebatki; counter++;
	elements[counter] = ((N * 2) + 1) + N + ((N * 4) + 2) * nr_zebatki; counter++;

	//DRUGA ZEBATKA (trapezy)
	for (int i = 1; i < N; i = i + 2)
	{
		elements[counter] = i + ((N * 2) + 1) + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + 1 + ((N * 2) + 1) + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + ((N * 2) + 1) + N + ((N * 4) + 2) * nr_zebatki; counter++;
	}
	for (int i = 2; i < N + 1; i = i + 2)
	{
		elements[counter] = i + ((N * 2) + 1) + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + N + ((N * 2) + 1) + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + N - 1 + ((N * 2) + 1) + ((N * 4) + 2) * nr_zebatki; counter++;
	}


	//LACZENIE 3D TROJKATOW
	for (int i = N + 1; i < N * 2; i = i + 2)
	{
		elements[counter] = i + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + 1 + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + 1 + (N * 2) + ((N * 4) + 2) * nr_zebatki; counter++;

		elements[counter] = i + 1 + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + 1 + (N * 2) + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + 2 + (N * 2) + ((N * 4) + 2) * nr_zebatki; counter++;
	}

	for (int i = 1; i < N + 1; i++)
	{
		elements[counter] = i + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + N + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + (N * 2) + 1 + ((N * 4) + 2) * nr_zebatki; counter++;

		elements[counter] = i + N + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + (N * 2) + 1 + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + (N * 3) + 1 + ((N * 4) + 2) * nr_zebatki; counter++;

	}

	for (int i = 2; i < N; i = i + 2)
	{
		elements[counter] = i + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + 1 + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + (N * 2) + 1 + ((N * 4) + 2) * nr_zebatki; counter++;

		elements[counter] = i + 1 + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + (N * 2) + 1 + ((N * 4) + 2) * nr_zebatki; counter++;
		elements[counter] = i + (N * 2) + 2 + ((N * 4) + 2) * nr_zebatki; counter++;
	}

	elements[counter] = N + ((N * 4) + 2) * nr_zebatki; counter++;
	elements[counter] = 1 + ((N * 4) + 2) * nr_zebatki; counter++;
	elements[counter] = (N * 3) + 1 + ((N * 4) + 2) * nr_zebatki; counter++;

	elements[counter] = 1 + ((N * 4) + 2) * nr_zebatki; counter++;
	elements[counter] = (N * 3) + 1 + ((N * 4) + 2) * nr_zebatki; counter++;
	elements[counter] = (N * 2) + 2 + ((N * 4) + 2) * nr_zebatki; counter++;
}

void create_bolt(float vertices[], GLuint elements[], int poczatkowy_punkt_vertices, int poczatkowy_punkt_elements, float x, float y, float kolor_x, float kolor_y, float kolor_z, float radius, int wierzcholki, float rotation, int ilosczebatek, int ilosckolek)
{
	int counter = poczatkowy_punkt_vertices;

	//PIERWSZE KO�O

	vertices[counter] = x; counter++;
	vertices[counter] = y; counter++;
	vertices[counter] = -0.1001; counter++; //wspolrzedna "z" ustawiona na sztywno
	vertices[counter] = kolor_x; counter++; //WPROWADZENIE KOORDYNATOW PUNKTU W SRODKU KOLA ABY LATWIEJ RYSOWALO SIE TROJKATY
	vertices[counter] = kolor_y; counter++;
	vertices[counter] = kolor_z; counter++;
	vertices[counter] = 0.0f; counter++;
	vertices[counter] = 0.0f; counter++; //textcord ktore nie sa potrzebne jezeli kolorujemy shaderami

	for (double i = 0; i < 2 * PI; i += ((2 * PI) / (wierzcholki))) //KO�O
	{
		vertices[counter] = cos(i + rotation) * radius + x; counter++;
		vertices[counter] = sin(i + rotation) * radius + y; counter++;
		vertices[counter] = -0.1001; counter++;
		vertices[counter] = kolor_x; counter++;
		vertices[counter] = kolor_y; counter++;
		vertices[counter] = kolor_z; counter++;
		vertices[counter] = 0.0f; counter++;
		vertices[counter] = 0.0f; counter++;
	}

	//DRUGIE KOLO

	vertices[counter] = x; counter++;
	vertices[counter] = y; counter++;
	vertices[counter] = 0.1001; counter++; //wspolrzedna "z" ustawiona na sztywno
	vertices[counter] = kolor_x; counter++; //WPROWADZENIE KOORDYNATOW PUNKTU W SRODKU KOLA ABY LATWIEJ RYSOWALO SIE TROJKATY
	vertices[counter] = kolor_y; counter++;
	vertices[counter] = kolor_z; counter++;
	vertices[counter] = 0.0f; counter++;
	vertices[counter] = 0.0f; counter++; //textcord ktore nie sa potrzebne jezeli kolorujemy shaderami

	for (double i = 0; i < 2 * PI; i += ((2 * PI) / (wierzcholki))) //KO�O
	{
		vertices[counter] = cos(i + rotation) * radius + x; counter++;
		vertices[counter] = sin(i + rotation) * radius + y; counter++;
		vertices[counter] = 0.1001; counter++;
		vertices[counter] = kolor_x; counter++;
		vertices[counter] = kolor_y; counter++;
		vertices[counter] = kolor_z; counter++;
		vertices[counter] = 0.0f; counter++;
		vertices[counter] = 0.0f; counter++;
	}

	counter = poczatkowy_punkt_elements;
	//LACZENIE PIERWSZEGO KOLKA

	int offset = (((N * 4) + 2) * ilosczebatek + ((N * 2) + 2) * ilosckolek); //WAZNE
	int offset2 = (((N * 4) + 2) * ilosczebatek + ((N * 2) + 2) * ilosckolek) + wierzcholki + 1;

	for (int i = 1; i < wierzcholki; i++)
	{
		elements[counter] = offset; counter++;
		elements[counter] = i + offset; counter++;
		elements[counter] = i + 1 + offset; counter++;
	}
	elements[counter] = offset; counter++;
	elements[counter] = wierzcholki + offset; counter++;
	elements[counter] = 1 + offset; counter++;

	//LACZENIE DRUGIEGO KOLKA
	for (int i = 1; i < wierzcholki; i++)
	{
		elements[counter] = offset2; counter++;
		elements[counter] = i + offset2; counter++;
		elements[counter] = i + 1 + offset2; counter++;
	}
	elements[counter] = offset2; counter++;
	elements[counter] = wierzcholki + offset2; counter++;
	elements[counter] = 1 + offset2; counter++;

	//LACZENIE K�LEK ZE SOBA
	for (int i = 1; i < wierzcholki; i++)
	{
		elements[counter] = i + offset; counter++;
		elements[counter] = i + 1 + offset; counter++;
		elements[counter] = i + offset2; counter++;
	}
	elements[counter] = wierzcholki + offset; counter++;
	elements[counter] = 1 + offset; counter++;
	elements[counter] = wierzcholki + offset2; counter++;

	for (int i = 2; i < wierzcholki + 1; i++)
	{
		elements[counter] = i + offset; counter++;
		elements[counter] = i - 1 + offset2; counter++;
		elements[counter] = i + offset2; counter++;
	}
	elements[counter] = 1 + offset; counter++;
	elements[counter] = 1 + offset2; counter++;
	elements[counter] = wierzcholki + offset2; counter++;
}