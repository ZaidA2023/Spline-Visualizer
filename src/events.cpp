#include "events.hpp"
#include <SFML/Graphics.hpp>
#include "configuration.hpp"
#include <SFML/Window.hpp>

void processEvents(sf::Window& window, std::vector<sf::CircleShape>& controlCircles, bool& update)
{
    static bool isDragging = false;
    bool clicked = false;
    static std::size_t selectedCircleIndex = -1;
    for (auto event = sf::Event{}; window.pollEvent(event);)
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }


        } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    for (std::size_t i = 0; i < controlCircles.size(); ++i) {
                        float distance = std::hypot(mousePos.x - controlCircles[i].getPosition().x,
                                                    mousePos.y - controlCircles[i].getPosition().y);
                        if (distance <= conf::radius) {
                            isDragging = true;
                            selectedCircleIndex = i;
                            break;
                        }
                    }
                  if(mousePos.x > 10 && mousePos.x < 35 &&
                     mousePos.y > 10 && mousePos.y < 35 && !clicked) 
                     {
                        //Generate new Control Point
                        sf::CircleShape circle(conf::radius);
                        circle.setOrigin(conf::radius, conf::radius);
                        circle.setPosition(conf::window_size.x / 2, conf::window_size.y / 2);
                        circle.setFillColor(sf::Color::Green);
                        controlCircles.push_back(circle);
                        clicked = true;
                        update = true;
                     }
                }
            } else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if(isDragging == true) {
                      update = true;
                    }
                    isDragging = false;
                    clicked = false;
                    selectedCircleIndex = -1;
                }
            } else if (event.type == sf::Event::MouseMoved) {
                if (isDragging && selectedCircleIndex != -1) {
                    update = true;
                    sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
                    controlCircles[selectedCircleIndex].setPosition(mousePos);
                }
            }
        }
    }
