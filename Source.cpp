#include <math.h>
#include <iostream>
#include "Source.h"
#include <windows.h>
#include <string>
#include <cstdlib>
int WindowWidth;
int WindowHeight;

int main()   
{

    sf::RenderWindow windowUser(sf::VideoMode(600, 400), "Doom");
    sf::RectangleShape background, start, exitBut;
    sf::Sprite galka, block, full;
    MakeForm(&background, &start, &exitBut, &galka, &block, &full);
    bool fullScreen = false;

    sf::Clock clockBut;
    while (windowUser.isOpen())
    {
        sf::Event event;
        while (windowUser.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                 windowUser.close();
                 exit(0);
            }            

        }
        windowUser.clear(sf::Color::Black);
        
        if (isPress(windowUser, start.getPosition(), start.getSize()))
            windowUser.close();
        if (isPress(windowUser, exitBut.getPosition(), exitBut.getSize()))
            exit(0);
        if (isPress(windowUser, block.getPosition(), (sf::Vector2f)block.getTexture()->getSize()))
        {
            if (clockBut.getElapsedTime().asMilliseconds() > 200)
            {
                 if (!fullScreen)
                    fullScreen = true;            
                else
                    fullScreen = false;
                 clockBut.restart();
            }            
        }

        windowUser.draw(background);
        windowUser.draw(start);
        windowUser.draw(exitBut);
        windowUser.draw(block);
        if (fullScreen) windowUser.draw(galka);
        windowUser.draw(full);
        windowUser.display();
    }

    //-------------------------------------------------------

    bool isRunning = false;
    bool isShooting = false;
    bool oldShooting = false;
    bool isShifting = false;
    int currShotFrame = 0;
    int countKilled = 0;

    srand(time(NULL));
    sf::CircleShape player(0);
    player.setFillColor(sf::Color::Yellow);
    player.setPosition(4, 123);

    Camera camera;

    sf::Sound soundShoot = MakeSound();
    sf::Sound soundCheck = MakeSoundCheck();
    soundShoot.setVolume(80);
    soundCheck.setVolume(100);
    soundCheck.setLoop(false);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 10;

    sf::RenderWindow window;
    if (fullScreen)
    {
        WindowWidth = 1920;
        WindowHeight = 1080;
        window.create(sf::VideoMode(1920, 1080), "RayCasting", sf::Style::Fullscreen);
    }
    else
    {
        WindowWidth = 900;
        WindowHeight = 700;
        window.create(sf::VideoMode(WindowWidth, WindowHeight), "RayCasting");
    }
    
       
    window.setMouseCursorVisible(false);

    sf::Sprite aim,weapon,shoot,sight;
    aim = MakeAim();
    weapon = MakeWeapon();
    shoot = LoadShoot();
    sight = MakeSight();
    sf::Texture enemyTexture = LoadEnemy();

    std::vector<GameObject*> objects;
    CreateMap(objects);
    CreateEnemyes(objects,enemyTexture);

    sf::RectangleShape ground;
    ground.setSize({ WindowWidth*1.0f, 1.0f*WindowHeight / 2 });
    ground.setPosition(0, WindowHeight / 2);
    ground.setFillColor(sf::Color(127, 152, 0));

    sf::Text fps = LoadFps();

    sf::Clock clock,clockMoving;
    float dt,time;
    sf::Text timing = MakeTimeString();
    while (true)
    {
        if (IsExit())
            window.close();
            
        dt = clock.getElapsedTime().asMicroseconds();
        time = clockMoving.getElapsedTime().asSeconds();
        dt /= 1000;
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear({ 169, 222, 255 });
        window.draw(ground);

        Rotate(&player, 0.022, dt, window);
       
        isShifting = IsShift();
        if (isRunning && !isShifting)
        {
            weaponMoving(time, &weapon);
            weaponMoving(time, &sight);
        }
        Move(&player, 0.15 -0.07* isShifting, dt, player.getRotation(), objects, &isRunning);
        
        camera.setPosition({ player.getPosition().x + 5, player.getPosition().y + 5 });
        camera.setRotation(player.getRotation());
        camera.rayCasting(objects, window);
        camera.drawMap(window, objects, 0.4);
        isShooting = IsShootingFunc();
        if (isShooting && !oldShooting)
        {
            soundShoot.play();
        }
            
        if (!isShooting && oldShooting)
        {
            soundShoot.setLoop(false);
        }       
        if (isShooting)
        {
            if (camera.Kill(player.getRotation(), &objects))
            {
                ++countKilled;
                soundCheck.play();
                if (countKilled >= 10)
                {

                    sf::RenderWindow windowEnd(sf::VideoMode(500, 200), "End Game", sf::Style::Close);
                    sf::RectangleShape rectangle;
                    rectangle.setFillColor(sf::Color::White);
                    rectangle.setSize({ 500, 200 });
                    rectangle.setPosition({ 0,0 });
                    windowEnd.draw(rectangle);
                    EndGame(windowEnd, clockMoving.getElapsedTime().asSeconds());
                    window.close();
                    windowEnd.display();
                    window.setMouseCursorVisible(true);
                    while (true)
                    {
                        while (windowEnd.pollEvent(event))
                        {
                            if (event.type == sf::Event::Closed)
                            {
                                windowEnd.close();
                                exit(0);
                            }
                        }
                        
                    }
                    
                }

                CreateEnemyes(objects,enemyTexture);
                
            }
        }
            
        if (IsAming(window))
        {
            MakeShot(window, shoot, sight, &currShotFrame, &isShooting, soundShoot, { sight.getScale().x*sight.getTextureRect().width/5,sight.getScale().y * sight.getTextureRect().height /(10) });
            window.draw(sight);
        }            
        else
        {
            MakeShot(window, shoot, weapon,  &currShotFrame, &isShooting, soundShoot, { weapon.getScale().x * weapon.getTextureRect().width / (-15.0f), weapon.getTextureRect().height / (-6.0f) });
            window.draw(aim);
            window.draw(weapon);
        }
        oldShooting = isShooting;
        ShowTime(window, timing, clockMoving.getElapsedTime().asMilliseconds());;
        ShowFps(window,fps);
        camera.showPosition(window);
        camera.showCountKilled(window, countKilled);
        window.display(); 
    } 
}


bool isPress(sf::RenderWindow &window, sf::Vector2f coordinates,sf::Vector2f size)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
        if (sf::IntRect((sf::Vector2i)coordinates, (sf::Vector2i)size).contains(sf::Mouse::getPosition(window))) return true;

    return false;
}


void EndGame(sf::RenderWindow &window,float time)
{
        FILE* file;
        fopen_s(&file,"user/record.txt", "r+");
        float oldResult;
        sf::Font font;
        font.loadFromFile("fonts/BakbakOne-Regular.ttf");
        sf::Text text("", font, 20);
        text.setFillColor(sf::Color::Black);
        char temp;
        std::string result,tempString, str = std::to_string(time);
        int i = 0;
        while (str[i] != '.') ++i;
        result = str.substr(0, i);
        result = result + str.substr(i, 4);
        if (fscanf_s(file,"%c",&temp)!=EOF)
        {
            fseek(file,0, SEEK_SET);
            
            fscanf_s(file, "%f", &oldResult);
            str = std::to_string(oldResult);
            i = 0;
            while (str[i] != '.') ++i;
            tempString = str.substr(0, i);
            tempString = tempString + str.substr(i, 4);
            

            if (oldResult > time) 
            {                
                text.setString("New record!!! " + result + "\nOld record: " + tempString);
                fclose(file);
                fopen_s(&file, "user/record.txt", "w+");
                fprintf(file, "%f\n", time);
            }
            else
                text.setString("Your result: " + result + "\nRecord: " + tempString);
      
        }
        else
        {
            text.setString("Your new result: " + result + "\n");
            fprintf(file, "%f\n", time);
        }
            
        text.setPosition(160.0f, 60.0f);
        window.draw(text);
        fclose(file);
        
}


void MakeForm(sf::RectangleShape* background, sf::RectangleShape* start, sf::RectangleShape* exitBut, sf::Sprite* galka, sf::Sprite* block, sf::Sprite* full)
{
    static sf::Texture backgroundTexture, startTexture, exitTexture, blockTexture, galkaTexture, fullTexture;
    backgroundTexture.loadFromFile("images/floor2.jpg");

    background->setTexture(&backgroundTexture);
    background->setPosition(0.0f, 0.0f);
    background->setSize({600.0f,400.0f});

    startTexture.loadFromFile("images/start.png");
    start->setTexture(&startTexture);
    start->setPosition(124.0f, 100.0f);
    start->setSize((sf::Vector2f)startTexture.getSize());

    exitTexture.loadFromFile("images/exit.png");
    exitBut->setTexture(&exitTexture);
    exitBut->setPosition(354.0f, 297.0f);
    exitBut->setSize((sf::Vector2f)exitTexture.getSize());


    galkaTexture.loadFromFile("images/galka.png");
    blockTexture.loadFromFile("images/block.png");
    fullTexture.loadFromFile("images/fullScreen.png");
    galka->setTexture(galkaTexture);
    block->setTexture(blockTexture);
    full->setTexture(fullTexture);
    galka->setPosition(58.0f, 290.0f);
    block->setPosition(54.0f, 300.0f);
    full->setPosition(104.0f, 300.0f);
}


void ShowTime(sf::RenderWindow &window, sf::Text text,float time)
{
    time /= 1000;
    std::string str = std::to_string(time);
    std::string result;
    int i = 0;
    while (str[i] != '.') ++i;
    result = str.substr(0, i);
    result = result + str.substr(i, 4);
    text.setString(result);
    window.draw(text);
}


sf::Text MakeTimeString()
{
    static sf::Font font;//шрифт 
    font.loadFromFile("fonts/BakbakOne-Regular.ttf");//передаем нашему шрифту файл шрифта
    sf::Text text("", font, 50);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
    text.setPosition(WindowWidth * 1/2-70, 10);
    return text;
}


void CreateMap(std::vector<GameObject*>& objects)
{
    sf::Texture textureStone,textureWood;
    textureStone.loadFromFile("images/Wall.png");
    textureWood.loadFromFile("images/wood.png");
    //------------------ Bounders
    static sf::RectangleShape boundary;
    boundary.setSize({ 497, 497 });

    objects.push_back(new GameObject);
    objects[0]->setShape(boundary);
    objects[0]->setTexture(textureStone);
    
    
    //-------------   Square
    static sf::RectangleShape rectangle_1;
    rectangle_1.setFillColor(sf::Color::Blue);

    rectangle_1.setPosition({ 112, 0 });
    rectangle_1.setSize({ 73,  212 });

    objects.push_back(new GameObject);
    objects[objects.size() - 1]->setShape(rectangle_1);
    objects[objects.size() - 1]->setTexture(textureWood);
    
    //-------------   Circle
  /*  static sf::CircleShape circle;
    circle.setFillColor(sf::Color::Red);
    
    circle.setPosition({ 350, 150 });
    circle.setRadius(30);

    objects.push_back(new GameObject);
    objects[objects.size() - 1]->setShape(circle);
    objects[objects.size() - 1]->setTexture(textureWood);*/
    
    //--------------------- Ugol rectangle
    static sf::RectangleShape rectangle_2;
    rectangle_2.setFillColor(sf::Color::Blue);

    rectangle_2.setPosition({419, 0});
    rectangle_2.setSize    ({80,  112});

    objects.push_back(new GameObject);
    objects[objects.size() - 1]->setShape(rectangle_2);
    objects[objects.size() - 1]->setTexture(textureStone);
    //----------------------
    static sf::RectangleShape rectangle_3;
    rectangle_3.setFillColor(sf::Color::Blue);

    rectangle_3.setPosition({347, 256});
    rectangle_3.setSize    ({91,  48});

    objects.push_back(new GameObject);
    objects[objects.size() - 1]->setShape(rectangle_3);
    objects[objects.size() - 1]->setTexture(textureWood);
    //-----------------------Triangle
    static sf::ConvexShape triangle(3);
    triangle.setFillColor(sf::Color::Blue);

    triangle.setPoint(0, {177, 334});
    triangle.setPoint(1, {259, 443});
    triangle.setPoint(2, {98,  443});

    objects.push_back(new GameObject);
    objects[objects.size() - 1]->setShape(triangle);
    objects[objects.size() - 1]->setTexture(textureWood);
    
}


void CreateEnemyes(std::vector<GameObject*>& enemyes, sf::Texture texture)
{
    static sf::Vector2f positions[9][2] =
    {
        {{276, 142}, {310, 187}},
        {{431, 462} ,{466, 417}},
        {{304, 479} ,{364, 475}},
        {{41, 434}, {77, 479}},
        {{18, 38} ,{66, 18}},
        {{31, 337}, {54, 387}},
        {{406, 386} ,{450, 350}},
        {{452, 144}, {459, 201}},
        {{203, 53} ,{250, 16}}
    };
    
    static sf::ConvexShape enemy_1(2);
    enemy_1.setFillColor(sf::Color::Red);
    int currPosition = rand() % 9;
   // int currPosition = 8;
    enemy_1.setPoint(0, positions[currPosition][0]);
    enemy_1.setPoint(1, positions[currPosition][1]);

    enemyes.push_back(new GameObject);
    enemyes[enemyes.size() - 1]->setShape(enemy_1);
    enemyes[enemyes.size() - 1]->setTexture(texture);
}


sf::Text LoadFps()
{
    sf::Text text;
    static sf::Font font;
    font.loadFromFile("fonts/NotoSansJP-Regular.otf");
    text.setFont(font);
    text.setCharacterSize(24);
    text.setStyle(sf::Text::Bold);
    return text;
}


void ShowFps(sf::RenderWindow& window, sf::Text text)
{
    static sf::Clock clock;
    static float oldTime = clock.getElapsedTime().asMilliseconds();
    
    std:: string str;
    float test = clock.getElapsedTime().asMilliseconds();
    str = std::to_string(1000/(test - oldTime));
    if (str.size()>6)
    {
        str.erase(6, str.size()-6);
    }
    str += " fps";
    text.setString(str);
    text.setPosition(WindowWidth-90,WindowHeight-20);
    text.setCharacterSize(15);
    window.draw(text);
    
    clock.restart();
    oldTime = clock.getElapsedTime().asMilliseconds();
    
}


sf::Vector2f GetCursorPosition(sf::RenderWindow& window)
{
    sf::Vector2f cursorPosition;

    cursorPosition = (sf::Vector2f)sf::Mouse::getPosition(window);
    cursorPosition = window.mapPixelToCoords((sf::Vector2i)cursorPosition);

    return cursorPosition;
}


bool CrossingWay(std::vector<GameObject*> objects, sf::Vector2f position, sf::Vector2f moveVector, float angle)
{
    int countObjects = objects.size();
    for (int j = 0; j < countObjects; j++)
    {
        int countPoints = objects[j]->getPointCount();
        for (int p = 0; p < countPoints; p++)
        {
            sf::Vector2f pointCrossing = { 0, 0 };

            if (Camera::crossingLines({ position, position + moveVector },
                { objects[j]->getPoint(p) + objects[j]->getPosition(),
                  objects[j]->getPoint((p + 1) % countPoints) + objects[j]->getPosition() },
                &pointCrossing))
            {
                float distance = DistanceBetweenPoints(position, pointCrossing);

                distance *= cos(DegreesToRadians(angle));

                if (distance < 100) return true;
            }
        }
    }
    return false;

}


sf:: Vector2f AddSpace(sf::Vector2f prevVector)
{
    sf::Vector2f moveVector = prevVector;
    if (moveVector.x > 0) moveVector.x += 10;
    else if (moveVector.x < 0) moveVector.x -= 10;

    if (moveVector.y > 0) moveVector.y += 10;
    else if (moveVector.y < 0) moveVector.y -= 10;

    return moveVector;
}


bool IsExit()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        return true;
    else return false;
}


bool IsShootingFunc()
{
    return sf::Mouse::isButtonPressed(sf::Mouse::Left);
}


bool IsShift()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))  
        return true;
    else 
        return false;
    
}


bool IsAming(sf::RenderWindow& window)
{
    return sf::Mouse::isButtonPressed(sf::Mouse::Right);
}


void MakeShot(sf::RenderWindow& window, sf::Sprite shoot,sf::Sprite weapon,int* i,bool* isShooting,sf::Sound sound, sf::Vector2f temp)
{
    if (*isShooting || *i != 0)
    {
        //sf::Vector2f temp = { -20.0f,-35.0f*2 };//sf::Vector2f temp = { 65.0f,-17.0f };
        shoot.setPosition(weapon.getPosition() + temp );
        shoot.setTextureRect(sf::IntRect(*i * 256, 0, 256, 230));
        ++(*i);
        if (*i == 9)  // if (*i == 10)
        {
            *i = 0;
            *isShooting = false;
        }
        else
            *isShooting = true;
        window.draw(shoot);
    }         
}


sf::Sound MakeSound()
{
    static sf::SoundBuffer soundBuffer;
    soundBuffer.loadFromFile("sounds/shoot.ogg");
    sf::Sound soundShoot(soundBuffer);
    soundShoot.setLoop(true);
    return soundShoot;
}


sf::Texture LoadEnemy()
{
    static sf::Texture texture;
    texture.loadFromFile("images/enemy.png");
   // texture.loadFromFile("images/vask2.png");
    return texture;
}


sf::Sound MakeSoundCheck()
{
    static sf::SoundBuffer soundBuffer;
    soundBuffer.loadFromFile("sounds/check.ogg");
    sf::Sound sound(soundBuffer);
    sound.setLoop(true);
    return sound;
}


sf::Sprite MakeAim()
{
    static sf::Texture aim;
    aim.loadFromFile("images/aim3.png");
    sf::Sprite aimSprite;
    aimSprite.setTexture(aim);
    aimSprite.setColor(sf::Color::White);
    aimSprite.setPosition(WindowWidth / 2 - 288 * 0.1, WindowHeight / 2 - 288 * 0.1);
    aimSprite.setScale({ 0.1,  0.1 });
    return aimSprite;
}


void weaponMoving(float time, sf::Sprite* sprite)
{
    if (sprite->getPosition().y - 2 * sin(10 * time < WindowHeight + 20) && (sprite->getTexture()->getSize().y * sprite->getScale().y - 2 * sin(10 * time) + sprite->getPosition().y > WindowHeight))
        sprite->move({ -2 * cos(10 * time),-2 * sin(10 * time) });
}


sf::Sprite MakeSight()
{
    static sf::Texture texture;
    texture.loadFromFile("images/sight.png");
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(WindowWidth*0.002444f, WindowHeight*0.0031428f);
    sprite.setPosition((float)WindowWidth/2-sprite.getScale().x*sprite.getTextureRect().width/2, WindowHeight- sprite.getScale().y * sprite.getTextureRect().height);

    return sprite;
}


sf::Sprite MakeWeapon()
{
    static sf::Texture texture;
    texture.loadFromFile("images/m4a1.png");
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale({ WindowWidth * 0.0005f,WindowHeight * 0.00071428f });
    sprite.setPosition({ float(WindowWidth - sprite.getTextureRect().width * sprite.getScale().x - 90),float(WindowHeight - sprite.getTextureRect().height * sprite.getScale().y) });
    return sprite;
}


sf::Sprite LoadShoot()
{
    sf::Sprite sprite;
    static sf::Texture texture;
    texture.loadFromFile("images/shoot2.png");
    sprite.setTexture(texture);
    sprite.setScale(WindowWidth / 900, WindowHeight / 700);
    return sprite;
}


sf::Vector2f MoreAddSpace(sf::Vector2f prevVector)
{
    sf::Vector2f moveVector = prevVector;
    if (moveVector.x > 0) moveVector.x += 30;
    else if (moveVector.x < 0) moveVector.x -= 30;

    if (moveVector.y > 0) moveVector.y += 30;
    else if (moveVector.y < 0) moveVector.y -= 30;

    return moveVector;
}


void Move(sf::CircleShape* player, float velocity, float dt, float angle, std::vector <GameObject*> objects,bool* isRunning)
{
    
    float angleRadians = DegreesToRadians(angle);
    sf::Vector2f moveVector = { 0.0f,0.0f };
    sf::Vector2f spaceVector = { 0.0f,0.0f };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        moveVector = { velocity * dt * cos(angleRadians), velocity * dt * sin(angleRadians) };
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        moveVector = { -velocity * dt * cos(angleRadians), -velocity * dt * sin(angleRadians) };
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        moveVector = { (float)(moveVector.x + velocity * dt * cos(angleRadians + PI / 2)),(float)(moveVector.y + velocity * dt * sin(angleRadians + PI / 2) ) };
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        moveVector = { (float)(moveVector.x + velocity * dt * cos(angleRadians - PI / 2) ),(float)(moveVector.y + velocity * dt * sin(angleRadians - PI / 2)) };
    }
   
    if (moveVector.x == 0.0f && moveVector.y == 0.0f)
        *isRunning = false;
    else
        *isRunning = true;

    spaceVector = AddSpace(moveVector);

    if (!CrossingWay(objects, player->getPosition(), spaceVector, angle))
    {
        player->move(moveVector);
    }
        

}


void Rotate(sf::CircleShape* player, float velocity, float dt, sf::RenderWindow& window)
{

    static sf::Vector2f startPosition = GetCursorPosition(window);
    sf::Vector2f nowPosition = GetCursorPosition(window);

    if (nowPosition.x - startPosition.x > 0)
    {
        player->rotate((nowPosition.x - startPosition.x) * velocity);

        sf::Mouse::setPosition({WindowWidth/2, WindowHeight/2}, window);
        startPosition = GetCursorPosition(window);
    }
    else if (nowPosition.x - startPosition.x < 0)
    {
        player->rotate((nowPosition.x - startPosition.x) * velocity);

        sf::Mouse::setPosition({WindowWidth/2, WindowHeight/2}, window);
        startPosition = GetCursorPosition(window);
    }
}