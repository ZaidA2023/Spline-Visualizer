#pragma once

namespace conf
{
// Window configuration
sf::Vector2u const window_size = {2560, 1440};
sf::Vector2f const window_size_f = static_cast<sf::Vector2f>(window_size); //Tutorial dude said this would be useful?? idk
uint32_t const max_framerate = 144;

float const radius = 10.f;

constexpr int tileSize = 50;

}