#pragma once
#include <SFML/Window.hpp>
#include "events.hpp"
#include <SFML/Graphics.hpp>
#include "configuration.hpp"

void processEvents(sf::Window& window, std::vector<sf::CircleShape>& controlCircles, bool& update);