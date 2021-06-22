#include "global.h"

double SCREEN_HEIGHT;
double SCREEN_WIDTH;

std::vector<set> coefficients;
std::queue<sf::Vector2f> spots;

sf::Vector2f destination;

double angle = 0.0;


sf::Vertex a[2];

//std::complex<double> calculatePrimitive(double a, unsigned int n, long int k)
//{
//	std::complex<double> sum(0.0, 0.0);
//	std::complex<double> scale(0.0, 1.0);
//	std::complex<double> k_complex(k,0.0);
//	std::complex<double> x(M_2PI, 0.0);
//	std::complex<double> a_c(a, 0.0);
//	for (float i = 0.0; i < n; i++)
//	{
//		std::complex<double>multi(multiplier(n,i), 0.0);
//		sum += (pow(scale, 3.0 + i)/pow(k_complex,i+1.0)*pow(x,n- (unsigned int)i)*multi);
//	}
//	sum *= a_c;
//	sum.real(sum.real() / M_2PI);
//	sum.imag(sum.imag() / M_2PI);
//	return sum;
//}
//
//long int multiplier(unsigned int n, int i)
//{
//	return i==0?1: n * multiplier(n - 1, i - 1);
//}
//
//std::complex<double> coefficient(int n, double a)
//{
//	return n==0?std::complex<double>(0.0,0.0): calculatePrimitive(a, 3, -n);
//}

void init(const std::vector<sf::Vector2f>& points)
{
	if (!coefficients.empty())
	{
		coefficients.erase(coefficients.begin(), coefficients.end());
	}
	std::vector<std::complex<float>>input;
	/*for (int i = 0; i < 40; i++)
	{
		input.push_back(std::complex<float>(100.0, 100.0));
	}
	for (int i = 0; i < 40; i++)
	{
		input.push_back(std::complex<float>(100.0, -100.0));
	}*/
	for (size_t q = 0; q < points.size(); q++)
	{
		input.push_back(translate(points[q]));
	}
	coefficients = produce(input);
	sortCoefficients();
}

std::vector<set> produce(std::vector<std::complex<float>> inputs)
{
	//a+bi
	std::vector<set> products;
	set tmp;
	size_t N = inputs.size();
	std::complex<float> argument(0.0, 0.0);
	std::complex<float> sum(0.0, 0.0);
	for (size_t q = 0; q < N; q++)
	{
		sum.real(0.0);
		sum.imag(0.0);
		for (size_t r = 0; r < N; r++)
		{
			argument.real(cos(M_2PI / N * r * calculateFrequency(q)));
			argument.imag(-1.0*sin(M_2PI/N*r* calculateFrequency(q)));
			sum += inputs[r] * argument;
		}
		//frequency is index in vector
		
		sum.real(sum.real() / N);
		sum.imag(sum.imag() / N);
		tmp.x = sum.real();
		tmp.y = sum.imag();
		tmp.radius = std::abs(sum);
		tmp.frequency = calculateFrequency(q);
		tmp.phase = std::arg(sum);
		products.push_back(tmp);
	}

	return products;
}

const std::complex<float> translate(const sf::Vector2f& input)
{
	return std::complex<float>(input.x-SCREEN_WIDTH/2.0f,input.y-SCREEN_HEIGHT/2.0f);
}

void draw(sf::RenderTexture& texture, sf::RenderTexture& buffer)
{
	float x = SCALE*SCREEN_WIDTH/2.0f+ SCALE * (coefficients[0].radius * cos(coefficients[0].frequency * angle + coefficients[0].phase));
	float y = SCALE*SCREEN_HEIGHT/2.0f+ SCALE * (coefficients[0].radius * sin(coefficients[0].frequency * angle + coefficients[0].phase));
	sf::CircleShape circ;
	circ.setFillColor(sf::Color::Transparent/*sf::Color::Color(100,100,100,0xff)*/);
	circ.setOutlineColor(sf::Color::White);
	circ.setOutlineThickness(SCALE*2.5);
	a[0].color = sf::Color::White;
	a[1].color = sf::Color::White;
	float x2, y2;
	for (size_t t = 1; t < coefficients.size(); t++)
	{
		x2 = x;
		y2 = y;
		x += SCALE*(coefficients[t].radius * cos(coefficients[t].frequency *angle+ coefficients[t].phase));
		y += SCALE*(coefficients[t].radius * sin(coefficients[t].frequency *angle+ coefficients[t].phase));
		if (t <= 150)
		{
			circ.setRadius(SCALE * coefficients[t].radius);
			circ.setPointCount((size_t)round(SCALE * (circ.getRadius() + 20)));
			circ.setPosition(x2 - circ.getRadius(), y2 - circ.getRadius());
			buffer.draw(circ);
		}
		
		a[0] = sf::Vector2f(x2,y2);
		a[1] = sf::Vector2f(x,y);
		buffer.draw(a, 2, sf::Lines);
	}
	circ.setPosition(x - SCALE * 3.0f, y - SCALE * 3.0f);
	circ.setRadius(SCALE*3.0f);
	circ.setPointCount(circ.getRadius() * 3);
	circ.setFillColor(sf::Color::Red);
	spots.push(sf::Vector2f(x, y));
	if (circ.getRadius()>=3.0f*SCALE)
	{
		buffer.draw(circ);
	}
	angle += M_2PI / coefficients.size();
	if (angle >= M_2PI||(destination.x==x/SCALE&&destination.y==y/SCALE))
	{
		while (!spots.empty())
		{
			spots.pop();
		}
		texture.clear(sf::Color::Transparent);
		angle = 0;
	}
}

int calculateFrequency(int q)
{
	if (q != 0)
	{
		return q % 2 == 0 ? (-1 * calculateFrequency(q - 1)) : (q+1)/2;
	}
	return 0;
}

sf::Vector2f convert(sf::Vector2i input)
{
	return sf::Vector2f(static_cast<float>(input.x), static_cast<float>(input.y));
}

void sortCoefficients()
{
	std::sort(coefficients.begin()+1, coefficients.end(), compareSet);
}

bool compareSet(set a, set b)
{
	return (a.radius > b.radius);
}