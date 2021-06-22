#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <complex>
#include <math.h>
#include <queue>
#include <algorithm>
#include "sfLine.h"

#define SCALE 1.15f
extern double SCREEN_HEIGHT;
extern double SCREEN_WIDTH;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // !M_PI
#ifndef M_2PI
#define M_2PI 6.2831853071795864769
#endif // !M_2PI

struct set
{
	double x;
	double y;
	float radius;
	float phase;
	float frequency;
};

bool compareSet(set a, set b);

extern sf::Vertex a[2];

extern std::vector<set> coefficients;
extern std::queue<sf::Vector2f> spots;
extern sf::Vector2f destination;
extern double angle;


void  init(const std::vector<sf::Vector2f>& points);

std::vector<set>
produce(std::vector<std::complex<float>> inputs);

const std::complex<float> translate(const sf::Vector2f& input);

void draw(sf::RenderTexture& texture, sf::RenderTexture& buffer);

int calculateFrequency(int q);

sf::Vector2f convert(sf::Vector2i input);

void sortCoefficients();
