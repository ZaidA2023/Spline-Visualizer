#include "background.hpp"
#include <SFML/Graphics.hpp>
#include "configuration.hpp"
#include <vector>


std::vector<sf::RectangleShape> drawBack() {
  int x_tiles = (conf::window_size.x / conf::tileSize) + 1;
  int y_tiles = (conf::window_size.y / conf::tileSize) + 1;

  std::vector<sf::RectangleShape> grid;
  for(int i = 0; i < x_tiles; i++) {
    for(int k = 0; k < y_tiles; k++) {
      sf::RectangleShape rectangle({conf::tileSize, conf::tileSize});
      rectangle.setOrigin(conf::tileSize/2, conf::tileSize/2);
      rectangle.setPosition((i * conf::tileSize) + (conf::tileSize/2) , (k * conf::tileSize) + (conf::tileSize/2));
      rectangle.setFillColor(sf::Color::Black);
      rectangle.setOutlineColor(sf::Color::White);
      rectangle.setOutlineThickness(1.f);
      grid.push_back(rectangle);
    }
  }
  return grid;
}