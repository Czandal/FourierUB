#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
class sfLine : public sf::Drawable
{
public:
    sfLine(const sf::Vector2f& point1, const sf::Vector2f& point2,const sf::Color LineColor,float LineThickness) :
        color(LineColor), thickness(LineThickness)
    {
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;

        for (int i = 0; i < 4; ++i)
            vertices[i].color = color;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(vertices, 4, sf::Quads);
    }

    void setColor(sf::Color color)
    {
        this->color = color;
        for (int i = 0; i < 4; ++i)
            vertices[i].color = color;
    }


private:
    sf::Vertex vertices[4];
    float thickness;
    sf::Color color;
};