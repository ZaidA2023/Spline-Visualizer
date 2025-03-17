#include "events.hpp"
#include <SFML/Graphics.hpp>
#include "configuration.hpp"
#include <SFML/Window.hpp>

void processEvents(sf::Window& window, std::vector<sf::CircleShape>& controlCircles, bool& update, int& statey)
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
    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
    if (event.mouseButton.button == sf::Mouse::Left) {
      if(statey != state::TITLE) {
        //Not a title screen click
        for (std::size_t i = 0; i < controlCircles.size(); ++i) {
            float distance = std::hypot(mousePos.x - controlCircles[i].getPosition().x,
                                        mousePos.y - controlCircles[i].getPosition().y);
            if (distance <= conf::radius) {
                isDragging = true;
                selectedCircleIndex = i;
                break;
            }
        }
        
      if(mousePos.x > 50 && mousePos.x < 75 &&
          mousePos.y > 10 && mousePos.y < 35 && !clicked) 
        {
          if(statey == state::BSPLINE) {
          //Generate new Main and Control Points
            sf::CircleShape circle(conf::radius);
            circle.setOrigin(conf::radius, conf::radius);
            circle.setPosition((13*50), (5*50));
            circle.setFillColor(sf::Color::Green);
            controlCircles.insert(controlCircles.end() - 1, circle);
            clicked = true;
            update = true;
          } else if(statey == state::BEZIER) {
            for(int i = 0; i < 3; i++) {
              sf::CircleShape circle(conf::radius);
              circle.setOrigin(conf::radius, conf::radius);
              circle.setPosition((13*50) + i*50, (5*50));
              circle.setFillColor(sf::Color::Blue);
              if(i == 0 || i == 1) circle.setFillColor(sf::Color::Green);
              controlCircles.push_back(circle);
              clicked = true;
              update = true;
            }
          }
        }
        if(mousePos.x > 20 && mousePos.x < 45 && mousePos.y > 10 && mousePos.y < 35) {
          statey = state::TITLE;
        }
        if(mousePos.x > 80 && mousePos.x < 105 && mousePos.y > 10 && mousePos.y < 35) {
          std::vector<sf::CircleShape> newCircles;
          for (int i = 0; i < 3; i++) {
            newCircles.push_back(controlCircles[i]);
          }
          newCircles.push_back(controlCircles[controlCircles.size()-1]);
          newCircles[0].setPosition(conf::p1);  
          newCircles[1].setPosition(conf::p2);  
          newCircles[2].setPosition(conf::p3); 
          newCircles[3].setPosition(conf::p4); 
          controlCircles = newCircles;
          update = true;
        }
      } else {
        //Title screen click
        sf::Vector2f middle(conf::window_size.x / 2, conf::window_size.y / 2);
        if(statey == state::TITLE && 
          mousePos.x < 652+99 && mousePos.x > 652-5 && mousePos.y < 484.5f+36 && mousePos.y > 484.5f-5) 
        {
          statey = state::BEZIER;
          update = true;
        } else if (statey == state::TITLE && 
          mousePos.x < 772+124 && mousePos.x > 772-5 && mousePos.y < 483.5f+44 && mousePos.y > 483.5f-5) {
          statey = state::BSPLINE;
          update = true;
        }
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
    sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
    if (isDragging && selectedCircleIndex != -1) {
        update = true;
        controlCircles[selectedCircleIndex].setPosition(mousePos);
    } 
    sf::Vector2f middle(conf::window_size.x / 2, conf::window_size.y / 2);

    if(statey == state::TITLE &&
       mousePos.x < 652+99 && mousePos.x > 652-5 && mousePos.y < 484.5f+36 && mousePos.y > 484.5f-5 ||
       mousePos.x < 772+124 && mousePos.x > 772-5 && mousePos.y < 483.5f+44 && mousePos.y > 483.5f-5) 
    {
      sf::Cursor cursor;
      if (cursor.loadFromSystem(sf::Cursor::Hand))
      {
        window.setMouseCursor(cursor);
      }

    } else if(mousePos.x > 10 && mousePos.x < 105 && mousePos.y > 10 && mousePos.y < 35 ) {
      sf::Cursor cursor;
      if (cursor.loadFromSystem(sf::Cursor::Hand))
      {
        window.setMouseCursor(cursor);
      }
    } else {
      sf::Cursor cursor;
      if (cursor.loadFromSystem(sf::Cursor::Arrow))
      {
        window.setMouseCursor(cursor);
      }
    }

  }
  }
}
