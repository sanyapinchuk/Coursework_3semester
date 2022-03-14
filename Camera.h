#pragma once
#include <vector>
#include "GameObject.h"

class Camera
{
private:
    sf::Vector2f cameraPosition;
    float rotationAngle;
    float viewRange;
    float viewAngle;
    float distanceRays;
    std::vector<sf::Vector2f> pointsRays;

    static void getLineCoefficients(Line line, double* k, float* b);
public:                                                                       // 0.06 min
    Camera(float _viewRange = 1000, float _viewAngle = 75, float _distanceRays = 0.1,float _rotationAngle=0);
    
    static bool crossingLines(Line line_1, Line   line_2, sf::Vector2f* pCrossing);
    void setPosition(sf::Vector2f position);
    void setRotation(float angle);
    void rayCasting(std::vector<GameObject*> objects, sf::RenderWindow& window);
    bool Kill(float angle, std::vector<GameObject*> *objects);
    void drawMap(sf::RenderWindow& window, std::vector<GameObject*> objects, float scale);
    void showPosition(sf::RenderWindow& window);
    void showCountKilled(sf::RenderWindow& window,int count);
};
