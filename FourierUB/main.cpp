#include "global.h"

int main()
{
    int state = -1;
    const int DRAWING = 0;
    const int USER = 1;

    bool leave = 0;
    sf::RenderWindow window;
    sf::VideoMode desktop = sf::VideoMode().getDesktopMode();
    SCREEN_HEIGHT = desktop.height;
    SCREEN_WIDTH = desktop.width;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8.0;
    window.create(desktop, "Discrete Fourier Transform",sf::Style::Default,settings);
    window.setFramerateLimit(144);
    window.setVerticalSyncEnabled(1);
    sf::RenderTexture texture;
    sf::RenderTexture buffer;
    texture.create(SCALE * SCREEN_WIDTH, SCALE * SCREEN_HEIGHT);
    texture.clear(sf::Color::Transparent);
    
    buffer.create(SCALE*SCREEN_WIDTH, SCALE*SCREEN_HEIGHT);
    buffer.clear(sf::Color::Transparent);


    sf::Sprite usrDrawing;
    sf::RenderTexture usrDrawingT;
    usrDrawingT.create(SCREEN_WIDTH, SCREEN_HEIGHT);
    usrDrawingT.clear(sf::Color::Transparent);
    usrDrawing.setTexture(usrDrawingT.getTexture(), true);
    usrDrawingT.display();

    sf::Sprite sprite;
    sf::Sprite buffer_sprite;
    buffer.setSmooth(true);
    texture.setSmooth(true);
    sprite.setTexture(texture.getTexture(), true);
    buffer_sprite.setTexture(buffer.getTexture(), true);
    buffer_sprite.setScale(1.0f / SCALE, 1.0f / SCALE);
    buffer.display();
    texture.display();

    sf::Event event;
    bool userDrawing = 1;
    bool drawing = 0;
    unsigned int iterator=0;
    std::vector<sf::Vector2f>points;
    while (!leave)
    {
        //events
        {
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    leave = 1;
                    window.close();
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                leave = 1;
                window.close();
                break;
            }
            if (window.hasFocus()&&sf::Mouse::isButtonPressed(sf::Mouse::Left)&&state!=USER)
            {
                state = USER;
                usrDrawingT.clear(sf::Color::Transparent);
                texture.clear(sf::Color::Transparent);
                points.erase(points.begin(), points.end());
                angle = 0;
            }
            else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)&&state==USER)
            {
                state = DRAWING;
                usrDrawingT.clear(sf::Color::Transparent);
                texture.clear(sf::Color::Transparent);
                destination = points.back();
                while (!spots.empty())
                {
                    spots.pop();
                }
                init(points);
            }
        }

        //render

        {
            window.clear(sf::Color::Black);
            if (state == DRAWING)
            {
                buffer.clear(sf::Color::Black);
                draw(texture, buffer);
                {
                    while (spots.size() >= 2)
                    {
                        sf::Vector2f origin = spots.front();
                        spots.pop();
                        sfLine line(origin, spots.front(), sf::Color::Red, 2.5 * SCALE);
                        line.draw(texture, sf::RenderStates::Default);
                    }

                }
                buffer.draw(sprite);
                window.draw(buffer_sprite);
            }
            else if (state == USER)
            {
                points.push_back(convert(sf::Mouse::getPosition(window)));
                if (points.size() >= 2)
                {
                    sfLine line(
                        sf::Vector2f(static_cast<float>(points[points.size() - 2].x), static_cast<float>(points[points.size() - 2].y)),
                        sf::Vector2f(static_cast<float>(points.back().x), static_cast<float>((float)points.back().y)),
                        sf::Color::Red, 3.0);
                    line.draw(usrDrawingT, sf::RenderStates::Default);
                }
                window.draw(usrDrawing);
            }
            window.display();
        }
    }
    return 0;
}
