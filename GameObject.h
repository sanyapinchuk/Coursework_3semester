#pragma once
#include "Tools.h"
class GameObject
{
private:

    sf::Texture texture;

public:
    sf::Shape* shape = 0;
    GameObject();

    void setShape(sf::Shape& shape);
    void setTexture(sf::Texture& texture);
    void setPosition(sf::Vector2f position);
    void setScale(sf::Vector2f scale);

    sf::Vector2f getPosition();
    sf::Vector2f getPoint(int point);
    int          getPointCount();
    sf::Texture* getTexture();
    sf::Vector2f getScale();

    void         draw(sf::RenderWindow& window);
};
