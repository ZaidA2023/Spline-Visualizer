#include <SFML/Graphics.hpp>
#include "configuration.hpp"
#include <vector>
#include <SFML/Window.hpp>
#include "background.hpp"
#include <stdio.h>


void drawTitle(sf::RenderWindow* window) {
  //Bezier title and bounding square
  sf::Vector2f middle(conf::window_size.x / 2, conf::window_size.y / 2);
  sf::Font font;
  font.loadFromFile("../src/open-sans/OpenSans-Regular.ttf");
  sf::Text text("Bezier", font);
  text.setFillColor(sf::Color::Black);
  text.setOrigin(text.getGlobalBounds().getSize() / 2.f);
  text.setPosition(middle.x - 60.f, middle.y);

  sf::RectangleShape rectangle;
  sf::FloatRect textBounds = text.getGlobalBounds();
  rectangle.setPosition(textBounds.left - 5, textBounds.top - 5);
  sf::Vector2f recSize(textBounds.width + 10, textBounds.height + 10);
  rectangle.setSize(recSize);
  rectangle.setFillColor(sf::Color::White);
  rectangle.setOutlineThickness(2);

  window->draw(rectangle);
  window->draw(text);

  //B-spline title and bounding square
  text.setString("B-spline");
  text.setPosition(middle.x + 60.f, middle.y);
  textBounds = text.getGlobalBounds();
  rectangle.setPosition(textBounds.left - 5, textBounds.top - 5);
  sf::Vector2f recSize2(textBounds.width + 10, textBounds.height + 10);
  rectangle.setSize(recSize2);
  window->draw(rectangle);
  window->draw(text);
}
