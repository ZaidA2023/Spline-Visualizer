#pragma once

#include <SFML/Graphics.hpp>

namespace conf
{
// Window configuration
const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
sf::Vector2u const window_size = {desktopMode.width, desktopMode.height};
sf::Vector2f const window_size_f = static_cast<sf::Vector2f>(window_size); //Tutorial dude said this would be useful?? idk
uint32_t const max_framerate = 144;

float const radius = 7.5f;

constexpr int tileSize = 50;

sf::Vector2f const p1 = {(conf::window_size.x/2) - 300.f, (conf::window_size.y/2) + 200.f};
sf::Vector2f const p2 = {(conf::window_size.x/2) - 200.f, (conf::window_size.y/2) - 200.f};
sf::Vector2f const p3 = {(conf::window_size.x/2) + 200.f, (conf::window_size.y/2) - 200.f};
sf::Vector2f const p4 = {(conf::window_size.x/2) + 300.f, (conf::window_size.y/2) + 200.f};

}

enum state 
{
  TITLE,
  BEZIER,
  BSPLINE
};