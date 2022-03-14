#ifndef TOOLS
#define TOOLS
#include <SFML\Graphics.hpp>
#define PI 3.14159265


    extern int WindowWidth;  // 900*700
    extern int WindowHeight;

struct Line
{
    sf::Vector2f p1;
    sf::Vector2f p2;
};


bool IsInsideRect(sf::Vector2f cursorPosition, sf::RectangleShape* rectangle);

float DistanceBetweenPoints(sf::Vector2f p1, sf::Vector2f p2);

float DegreesToRadians(float degrees);

sf::Color CorrectLightness(sf::Color color, int delta);

template <typename type>
void Swap(type* a, type* b)
{
    type c = (*a);
    (*a) = (*b);
    (*b) = c;
}

template <typename type>
bool BelongingInterval(type min, type max, type num, float error)
{
    if (min > max) Swap(&min, &max);

    if (num >= min - error && num <= max + error) return true;
    else                                     return false;
}

#endif