#include "Tools.h"


bool IsInsideRect(sf::Vector2f cursorPosition, sf::RectangleShape* rectangle)
{
    sf::Vector2f position = rectangle->getPosition();
    sf::Vector2f size = rectangle->getSize();
    sf::Vector2f scale = rectangle->getScale();

    size = { size.x * scale.x, size.y * scale.y };

    if (cursorPosition.x >= position.x && cursorPosition.x <= position.x + size.x &&
        cursorPosition.y >= position.y && cursorPosition.y <= position.y + size.y)
    {
        return 1;
    }

    return 0;
}

float DistanceBetweenPoints(sf::Vector2f p1, sf::Vector2f p2)
{
    float distance;
    distance = sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));

    return distance;
}

float DegreesToRadians(float degrees)
{
    return degrees * PI / 180;
}

sf::Color CorrectLightness(sf::Color color, int delta)
{
    int r = std::min(std::max(color.r + delta * 20 / 100, 0), 255);
    int g = std::min(std::max(color.g + delta * 20 / 100, 0), 255);
    int b = std::min(std::max(color.b + delta * 20 / 100, 0), 255);

    return sf::Color(r, g, b);
}
