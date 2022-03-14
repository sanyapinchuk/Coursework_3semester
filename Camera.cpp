#include "Camera.h"


Camera::Camera(float _viewRange, float _viewAngle, float _distanceRays,float _rotationAngle) : viewRange(_viewRange),
viewAngle(_viewAngle),
distanceRays(_distanceRays),
rotationAngle(_rotationAngle)
{
    if (distanceRays < 1)
    {
        pointsRays.resize(ceil(viewAngle / distanceRays));
    }
    else
    {
        pointsRays.resize(ceil(viewAngle / distanceRays) + 1);
    }
}


bool Camera::crossingLines(Line line_1, Line line_2, sf::Vector2f* pCrossing)
{
    float error = 0.5;

    bool isParallelY_1 = true;
    double k1 = DBL_MAX; float b1 = FLT_MAX;
    if (abs(line_1.p1.x - line_1.p2.x) >= error)
    {
        getLineCoefficients(line_1, &k1, &b1);
        isParallelY_1 = false;
    }

    bool isParallelY_2 = true;
    double k2 = DBL_MAX; float b2 = FLT_MAX;
    if (abs(line_2.p1.x - line_2.p2.x) >= error)
    {
        getLineCoefficients(line_2, &k2, &b2);
        isParallelY_2 = false;
    }

    if (k1 == k2)
    {
        return false;
    }

    if (isParallelY_1)
    {
        pCrossing->x = line_1.p1.x;
        pCrossing->y = k2 * (pCrossing->x) + b2;
    }
    else if (isParallelY_2)
    {
        pCrossing->x = line_2.p1.x;
        pCrossing->y = k1 * (pCrossing->x) + b1;
    }
    else
    {
        pCrossing->x = (b2 - b1) / (k1 - k2);
        pCrossing->y = k1 * (pCrossing->x) + b1;
    }

    if (!BelongingInterval(line_1.p1.x, line_1.p2.x, pCrossing->x, error) ||
        !BelongingInterval(line_1.p1.y, line_1.p2.y, pCrossing->y, error) ||
        !BelongingInterval(line_2.p1.x, line_2.p2.x, pCrossing->x, error) ||
        !BelongingInterval(line_2.p1.y, line_2.p2.y, pCrossing->y, error))
    {
        return false;
    }

    return true;
}


void Camera::getLineCoefficients(Line line, double* k, float* b)
{
    if (line.p1.x == line.p2.x)
    {
        (*k) = DBL_MAX;
        (*b) = line.p1.y;

        return;
    }
    (*k) = (line.p2.y - line.p1.y) / (line.p2.x - line.p1.x);

    (*b) = line.p1.y - (*k) * line.p1.x;
}


void Camera::setPosition(sf::Vector2f position)
{
    cameraPosition = position;
}


void Camera::setRotation(float angle)
{
    rotationAngle = angle;
}


bool Camera::Kill(float angle, std::vector<GameObject*> *objects)
{
    float angleRadians = DegreesToRadians( angle);
    sf::Vector2f pos = { 0, 0 };     // check line 
    sf::Vector2f pointCrossing = { 0, 0 };
    pos.x = cameraPosition.x + cos(angleRadians) * viewRange;
    pos.y = cameraPosition.y + sin(angleRadians) * viewRange;

    if (crossingLines({ cameraPosition, pos },
        { (*objects)[objects->size() - 1]->getPoint(0) + (*objects)[objects->size() - 1]->getPosition(),
          (*objects)[objects->size() - 1]->getPoint(1) + (*objects)[objects->size() - 1]->getPosition() },
        &pointCrossing))
    {
        float distanceAimPlr = DistanceBetweenPoints(cameraPosition, pointCrossing);
        distanceAimPlr *= cos(DegreesToRadians(angle-rotationAngle));

        int countObjects = objects->size()-1;
        float minDistance = distanceAimPlr;
        for (int j = 0; j < countObjects; j++)
        {
            int countPoints = (*objects)[j]->getPointCount();
            for (int p = 0; p < countPoints; p++)
            {
                sf::Vector2f p2Crossing = { 0, 0 };

                if (crossingLines({ cameraPosition, pos },
                    { (*objects)[j]->getPoint(p) + (*objects)[j]->getPosition(),
                      (*objects)[j]->getPoint((p + 1) % countPoints) + (*objects)[j]->getPosition() },
                    &p2Crossing))
                {
                    float distance = DistanceBetweenPoints(cameraPosition, p2Crossing);
                    distance *= cos(DegreesToRadians(angle - rotationAngle)); //////////////???????????
                    if (distance< minDistance)
                    return false;
                }
            }
        }
         objects->pop_back();
         return true;
    }
    return false;
       
}


void Camera::rayCasting(std::vector<GameObject*> objects, sf::RenderWindow& window)
{
    int countObjects = objects.size();
    int countTextures = 0;
    int oldobj = -1;
    float oldWidth = 0;
    int oldP = 0;
    float angleNow = -(viewAngle / 2);
    for (int i = 1; angleNow <= viewAngle / 2; i++)
    {
        float angleRadians = DegreesToRadians(rotationAngle + angleNow);

        sf::Vector2f pos = { 0, 0 };     // check line 
        pos.x = cameraPosition.x + cos(angleRadians) * viewRange;
        pos.y = cameraPosition.y + sin(angleRadians) * viewRange;

        int obj = 0;
        int point = 0;

        sf::Vector2f pCrossing;
        float minDistance = FLT_MAX;
        for (int j = 0; j < countObjects; j++)
        {
            int countPoints = objects[j]->getPointCount();
            for (int p = 0; p < countPoints; p++)
            {
                sf::Vector2f pointCrossing = { 0, 0 };

                if (crossingLines({ cameraPosition, pos },
                    { objects[j]->getPoint(p) + objects[j]->getPosition(),
                      objects[j]->getPoint((p + 1) % countPoints) + objects[j]->getPosition() },
                    &pointCrossing))
                {
                    float distance = DistanceBetweenPoints(cameraPosition, pointCrossing);
                    distance *= cos(DegreesToRadians(angleNow));

                    if (distance < minDistance) 
                    {
                        pCrossing = pointCrossing;
                        minDistance = distance;
                        obj = j;
                        point = p;
                    }
                }
            }
        }

        if (minDistance == FLT_MAX)
        {
            pointsRays[i - 1] = pos;
        }
        else
        {
            sf::RectangleShape rectangle;
            pointsRays[i - 1] = pCrossing;

            int countPoints = objects[obj]->getPointCount();

            float width = WindowWidth / (viewAngle / distanceRays);
            float height = (WindowWidth/10) / minDistance * 400;

            rectangle.setSize({ width, height });
            rectangle.setPosition({ (i - 1) * width, (WindowHeight - height) / 2 });

            sf::Texture* texture = objects[obj]->getTexture();
            sf::Vector2f sizeTexture = (sf::Vector2f)texture->getSize();
            rectangle.setTexture(texture);
            float k = fmod(DistanceBetweenPoints(objects[obj]->getPoint(point) + objects[obj]->getPosition(), pCrossing), sizeTexture.x) / sizeTexture.x;
            rectangle.setTextureRect(sf::IntRect((int)(sizeTexture.x * k), 0, (int)(sizeTexture.x / (viewAngle / distanceRays)), (int)sizeTexture.y));

            sf::Color color(114, 114, 114);
            color = CorrectLightness(color, std::min(0, -(int)(minDistance / 1.1)));
            rectangle.setFillColor(color);
            window.draw(rectangle);
        }
        angleNow += distanceRays;
    }
}


void Camera::drawMap(sf::RenderWindow& window, std::vector<GameObject*> objects, float scale)
{
    int countObjects = objects.size();
    sf::RectangleShape rectangle(sf::Vector2f(497 * scale, 497 * scale));
    rectangle.setFillColor(sf::Color(120, 120, 120));
    window.draw(rectangle);
    for (int i = 1; i < countObjects; i++)
    {
        sf::Vector2f position = objects[i]->getPosition();
        sf::Vector2f oldPosition = objects[i]->getPosition();
        sf::Vector2f oldscale = objects[i]->getScale();
        objects[i]->setPosition(position * scale);
        objects[i]->setScale({ scale, scale });
        if (i == countObjects - 1)
        {
            sf::RectangleShape enemy;
            enemy.setPosition(objects[i]->getPoint(0)*scale);
            enemy.setSize({ DistanceBetweenPoints(objects[i]->getPoint(0), objects[i]->getPoint(1)),5.0f });
            enemy.setScale(scale, scale);
            float grad = atan(((objects[i]->getPoint(0).y - objects[i]->getPoint(1).y) / ((objects[i]->getPoint(1).x - objects[i]->getPoint(0).x))));
            grad *= 180 / PI;
            enemy.rotate(-grad);
            enemy.setFillColor(sf::Color::Red);
            window.draw(enemy);
        }
        else
        {
            objects[i]->draw(window);
        }        
        objects[i]->setPosition(oldPosition);
        objects[i]->setScale(oldscale);
    }
    sf::Vertex line[2];
    /* sf::RectangleShape rectangle(sf::Vector2f(497 * scale, 497 * scale));
     rectangle.setFillColor(sf::Color::Black);
     window.draw(rectangle);*/
    line[0] = sf::Vertex(cameraPosition * scale);
    line[0].color = sf::Color::Green;

    int countPoints = pointsRays.size();
    for (int i = 0; i < countPoints; i += 10)
    {
        line[1] = sf::Vertex(pointsRays[i] * scale);
        line[1].color = sf::Color::Green;
        window.draw(line, 2, sf::Lines);
    }
}


void Camera::showPosition(sf::RenderWindow& window)
{
    std::string str;
    str = "x:" + std::to_string((int)cameraPosition.x) + "  y:" + std::to_string((int)cameraPosition.y);
    sf::Text text;
    sf::Font font;
    font.loadFromFile("fonts/NotoSansJP-Regular.otf");
    text.setFont(font);
    text.setCharacterSize(24);
    text.setStyle(sf::Text::Bold);
    text.setString(str);
    text.setPosition(20, WindowHeight - 20);
    text.setCharacterSize(15);
    window.draw(text);
}


void Camera::showCountKilled(sf::RenderWindow& window,int count)
{
    std::string str;
    str = std::to_string(count);
    sf::Text text;
    sf::Font font;
    font.loadFromFile("fonts/NotoSansJP-Regular.otf");
    text.setFont(font);
    text.setCharacterSize(24);
    text.setStyle(sf::Text::Regular);
    text.setString(str);
    text.setFillColor(sf::Color::Red);
    text.setPosition(WindowWidth-100,20);
    text.setCharacterSize(45);
    window.draw(text);
}
