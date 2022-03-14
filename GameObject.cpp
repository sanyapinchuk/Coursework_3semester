#include "GameObject.h"


GameObject::GameObject()
{

}
  

void GameObject::setShape(sf::Shape& shape)
{
    this->shape = &shape;
}


void GameObject::setTexture(sf::Texture& texture)
{
    this->texture = texture;
}


void GameObject::setPosition(sf::Vector2f position)
{
    shape->setPosition(position);
}


void GameObject::setScale(sf::Vector2f scale)
{
    shape->setScale(scale);
}


sf::Vector2f GameObject::getPosition()
{
    return shape->getPosition();
}


sf::Vector2f GameObject::getPoint(int point)
{
    sf::Vector2f temp = shape->getPoint(point);
    return temp;
}


int GameObject::getPointCount()
{
    return shape->getPointCount();
}


sf::Texture* GameObject::getTexture()
{
    return &texture;
}


sf::Vector2f  GameObject::getScale()
{
    return shape->getScale();
}


void GameObject::draw(sf::RenderWindow& window)
{
    window.RenderWindow::draw(*shape);
}

