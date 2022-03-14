#pragma once
#include "Camera.h"
#include "GameObject.h"
#include <SFML/Audio.hpp>

void Move(sf::CircleShape* player, float velocity, float dt, float angle, std::vector<GameObject*> objects,bool* isRunning);
void Rotate(sf::CircleShape* player, float velocity, float dt, sf::RenderWindow& window);

void CreateMap(std::vector<GameObject*>& objects);
void CreateEnemyes(std::vector<GameObject*>& enemyes,sf::Texture texture);
void ShowFps(sf::RenderWindow& window,sf::Text text);
sf::Text LoadFps();


sf::Sprite LoadShoot();
bool IsShootingFunc();
bool IsShift();
bool IsAming(sf::RenderWindow& window);
sf::Sound MakeSound();
sf::Sound MakeSoundCheck();
sf::Texture LoadEnemy();
void EndGame(sf::RenderWindow& window, float time);
sf::Sprite MakeAim();
sf::Sprite MakeWeapon();
void MakeForm(sf::RectangleShape* background, sf::RectangleShape* start, sf::RectangleShape* exitBut, sf::Sprite* galka, sf::Sprite* block, sf::Sprite* full);
bool isPress(sf::RenderWindow& window, sf::Vector2f coordinates, sf::Vector2f size);
bool IsExit();
sf::Text MakeTimeString();
void ShowTime(sf::RenderWindow& window, sf::Text text, float time);
void weaponMoving(float time, sf::Sprite* sprite);
sf::Sprite MakeSight();
void MakeShot(sf::RenderWindow& window,sf::Sprite shoot,sf::Sprite weapon, int* i, bool* isShooting,sf::Sound sound,sf::Vector2f temp);