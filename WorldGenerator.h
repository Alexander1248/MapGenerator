#pragma once
#include <math.h>
#include <string>
#include "SFML/Graphics.hpp"
#include "Encryption.h"
#include "PerlinNoise.h"



using namespace std;
using namespace sf;


void ElisiumMapGen(float**& data, Vector2i mapSize, int maxHeight, string seed, string worldType, int maxT, int minT,float waterPercentage,int waterLevel, int mountianLevel,int kRivers,int riverLen,float riverRandomisationPercentage,float scale) {
	//Количество циклов генерации шума (влияет на скорость)
	const int octaves = 10;

	//Преобразование семени мира в приемлимое значение
	char* seedHash = new char[32];
	int len = seed.length();
	char* inpSeed = new char[len];
	for (int i = 0; i < len; i++) inpSeed[i] = seed[i];
	SHP_2(seedHash, inpSeed, len, 32);
	unsigned int sum = 0;
	for (int i = 0; i < 32; i++) sum += ((seedHash[i] - 33) % 2) * pow(2, i);

	//Инициализация массивов
	data = new float* [4];
	for (int i = 0; i < 4; i++) data[i] = new float[mapSize.x * mapSize.y];
	int* par = new int[mapSize.x * mapSize.y];

	//Генерация карты высот
	GenNoise(par, mapSize.x, mapSize.y, octaves, maxHeight, sum,scale);
	for (int i = 0; i < mapSize.x * mapSize.y; i++) {
		float dat = float(par[i]) / maxHeight;
		par[i] = pow(dat, 1.f + waterPercentage / 100) * maxHeight;
		data[0][i] = par[i];
	}


	//Генерация рек
	srand(sum);
	int kRiv = kRivers;
	int* water = new int[kRiv];
	int c1 = (mapSize.x * mapSize.y / 26214.4);
	for (int i = 0; i < kRiv; i++) {
		water[i] = c1 * rand() % (mapSize.x * mapSize.y);
		while (data[0][water[i]] > waterLevel || data[0][water[i]] < waterLevel - 100) water[i] = c1 * rand() % (mapSize.x * mapSize.y);
	}
	int* wbuff;
	for (int i = 0; i < kRiv; i++) {
		int k = 0;
		int past = water[i];
		while (k < riverLen) {
			int imin;
			if (data[0][water[i]] > waterLevel) data[3][water[i]] = 1;
			if (water[i] >= 1 && past != water[i] - 1) {
				imin = water[i] - 1;
				if (water[i] >= mapSize.x && data[0][water[i] - mapSize.x] > data[0][imin]) imin = water[i] - mapSize.x;
				if (water[i] <= (mapSize.x * mapSize.y) - mapSize.x && data[0][water[i] + mapSize.x] > data[0][imin]) imin = water[i] + mapSize.x;
				if (water[i] <= (mapSize.x * mapSize.y) - 1 && data[0][water[i] + 1] > data[0][imin]) imin = water[i] + 1;
			}
			else {
				if (water[i] >= mapSize.x && past != water[i] - mapSize.x) {
					imin = water[i] - mapSize.x;
					if (water[i] <= (mapSize.x * mapSize.y) - mapSize.x && data[0][water[i] + mapSize.x] > data[0][imin]) imin = water[i] + mapSize.x;
					if (water[i] >= 1 && data[0][water[i] - 1] > data[0][imin]) imin = water[i] - 1;
					if (water[i] <= (mapSize.x * mapSize.y) - 1 && data[0][water[i] + 1] > data[0][imin]) imin = water[i] + 1;
				}
				else {
					if (water[i] <= (mapSize.x * mapSize.y) - 1 && past != water[i] + 1) {
						imin = water[i] + 1;
						if (water[i] >= mapSize.x && data[0][water[i] - mapSize.x] > data[0][imin]) imin = water[i] - mapSize.x;
						if (water[i] <= (mapSize.x * mapSize.y) - mapSize.x && data[0][water[i] + mapSize.x] > data[0][imin]) imin = water[i] + mapSize.x;
						if (water[i] >= 1 && data[0][water[i] - 1] > data[0][imin]) imin = water[i] - 1;
					}
					else {
						if (water[i] <= (mapSize.x * mapSize.y) - mapSize.x && past != water[i] + mapSize.x) {
							imin = water[i] + mapSize.x;
							if (water[i] >= mapSize.x && data[0][water[i] - mapSize.x] > data[0][imin]) imin = water[i] - mapSize.x;
							if (water[i] >= 1 && data[0][water[i] - 1] > data[0][imin]) imin = water[i] - 1;
							if (water[i] <= (mapSize.x * mapSize.y) - 1 && data[0][water[i] + 1] > data[0][imin]) imin = water[i] + 1;
						}
					}
				}
			}
			if (rand() % 100 > riverRandomisationPercentage) water[i] = imin;
			else {
				int r = rand() % 4;
				if (r == 0 && water[i] >= 1) water[i] = water[i] - 1;
				if (r == 1 && water[i] >= mapSize.x) water[i] = water[i] - mapSize.x;
				if (r == 2 && water[i] <= (mapSize.x * mapSize.y) - 1) water[i] = water[i] + 1;
				if (r == 3 && water[i] <= (mapSize.x * mapSize.y) - mapSize.x) water[i] = water[i] + mapSize.x;
			}
			k++;
		}
	}


	//Генерация температурной карты
	float* par1 = new float[mapSize.x * mapSize.y];
	if (worldType == "Plane") {
		for (int i = 0; i < mapSize.x * mapSize.y; i++) {
			par1[i] = maxT / 4;


			float coef = 2;
			if (float(data[0][i]) / maxHeight < float(waterLevel + maxHeight / 10) / maxHeight and par1[i] < maxT - coef) par1[i] += coef;
			if (data[0][i] > mountianLevel and par1[i] >= data[0][i] * abs(0.5 * minT) / mountianLevel + minT) par1[i] -= data[0][i] * abs(0.5 * minT) / mountianLevel;
		}
	}
	if (worldType == "CoredPlane") {
		for (int i = 0; i < mapSize.x * mapSize.y; i++) {
			par1[i] = float(maxT - minT) * pow(float(cos(float(3.14 * sqrt(pow(i % mapSize.x - mapSize.x / 2, 2) + pow(float(i) / mapSize.x - mapSize.y / 2, 2))) / sqrt(pow(mapSize.x / 1.15, 2) + pow(mapSize.y / 1.15, 2)))), 2) + minT;


			float coef = 2;
			if (float(data[0][i]) / maxHeight < float(waterLevel + maxHeight / 10) / maxHeight and par1[i] < maxT - coef) par1[i] += coef;
			if (data[0][i] > mountianLevel and par1[i] >= data[0][i] * abs(0.5 * minT) / mountianLevel + minT) par1[i] -= data[0][i] * abs(0.5 * minT) / mountianLevel;
		}
	}
	if (worldType == "Sphere") {
		for (int i = 0; i < mapSize.x * mapSize.y; i++) {
			par1[i] = (maxT - minT) * pow(float(sin((3.14 * float(i) / mapSize.x) / mapSize.y)), 2) + minT;


			float coef = 2;
			if (float(data[0][i]) / maxHeight < float(waterLevel + maxHeight / 10) / maxHeight and par1[i] < maxT - coef) par1[i] += coef;
			if (data[0][i] > mountianLevel and par1[i] >= data[0][i] * abs(0.5 * minT) / mountianLevel + minT) par1[i] -= data[0][i] * abs(0.5 * minT) / mountianLevel;
		}
	}
	for (int i = 0; i < mapSize.x * mapSize.y; i++) data[2][i] = par1[i];


	//Реверсивное преобразование семени мира в приемлимое значение
	sum = 0;
	for (int i = 0; i < 32; i++) sum += ((seedHash[i] - 33) % 2) * pow(2, 31 - i);

	//Генерация карты влажности
	GenNoise(par, mapSize.x, mapSize.y, octaves, 1000, sum, scale);
	if (worldType == "Plane") {
		int coef = 10;
		for (int i = 0; i < mapSize.x * mapSize.y; i++) {
			if (data[0][i] < waterLevel + maxHeight / 10 and par[i] < 1000 - coef) par[i] += coef;
			if (data[0][i] > mountianLevel + maxHeight / 100 and par[i] > data[0][i] * coef * 10 / mountianLevel) par[i] -= data[0][i] * coef * 10 / mountianLevel;
		}
	}
	if (worldType == "CoredPlane") {
		for (int i = 0; i < mapSize.x * mapSize.y; i++) {
			int coef = 10;
			if (data[0][i] < waterLevel + maxHeight / 10 and par[i] < 1000 - coef) par[i] += coef;
			if (data[0][i] > mountianLevel + maxHeight / 100 and par[i] > data[0][i] * coef * 10 / mountianLevel) par[i] -= data[0][i] * coef * 10 / mountianLevel;
			par[i] = (par[i] + (1000 * pow(cos(3.14 * ((sqrt(pow(i % mapSize.x - mapSize.x / 2, 2) + pow(trunc(float(i) / mapSize.x - mapSize.y / 2), 2)) - sqrt(pow(mapSize.x / 8, 2) + pow(mapSize.y / 10, 2))) / sqrt(pow(mapSize.x / 1.5, 2) + pow(mapSize.y / 1.5, 2)))), 2)) * 6) / 7;
		}
	}
	if (worldType == "Sphere") {
		for (int i = 0; i < mapSize.x * mapSize.y; i++) {
			int coef = 10;
			if (data[0][i] < waterLevel + maxHeight / 10 and par[i] < 1000 - coef) par[i] += coef;
			if (data[0][i] > mountianLevel + maxHeight / 100 and par[i] > data[0][i] * coef * 10 / mountianLevel) par[i] -= data[0][i] * coef * 10 / mountianLevel;
			par[i] = (par[i] + (1000 * pow(sin(3.14 * trunc(float(i) / mapSize.x) / mapSize.y), 2))) / 2;
		}
	}
	for (int i = 0; i < mapSize.x * mapSize.y; i++) data[1][i] = par[i], par[i] = 0;
}