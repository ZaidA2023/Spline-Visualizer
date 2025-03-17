#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include "events.hpp"
#include "configuration.hpp"
#//include "background.cpp"
#include <cmath>
#include <ranges>
#include "title.cpp"


int statey = state::TITLE;

using Matrix4 = std::array<std::array<float, 4>, 4>;
const Matrix4 bezier_coeff = {{
    {{ 1.f,  0.f,  0.f,  0.f}},
    {{-3.f,  3.f,  0.f,  0.f}},
    {{ 3.f, -6.f,  3.f,  0.f}},
    {{-1.f,  3.f, -3.f,  1.f}}
}};
const Matrix4 bspline_coeff = {{
    {{ 1.f,  4.f,  1.f,  0.f}},
    {{-3.f,  0.f,  3.f,  0.f}},
    {{ 3.f, -6.f,  3.f,  0.f}},
    {{-1.f,  3.f, -3.f,  1.f}}
}};
sf::VertexArray bezierCurve(sf::LineStrip);

sf::Vertex drawCurvePoint(sf::VertexArray points, float t, int offset) {
  const std::array<float, 4> poly = {{1, t, t*t, t*t*t}};
  std::array<float, 4> t_basis = {};

  for(int i = 0; i < 4; i++) {
    float sum = 0;
    for(int k = 0; k < 4; k++) {
      if(statey == state::BSPLINE) sum = sum + (poly[k] * (bspline_coeff[k][i]/6.f));
      if(statey == state::BEZIER) sum = sum + (poly[k] * (bezier_coeff[k][i]));
    }
    t_basis[i] = sum;
  }

  float x = 0.f, y = 0.f;
  for(int i = 0; i < 4; i++) {
    x += t_basis[i] * points[i + offset].position.x;
    y += t_basis[i] * points[i + offset].position.y;
  }
  return sf::Vertex(sf::Vector2f(x, y));
}

sf::VertexArray updateBezier(sf::VertexArray& controlPoints, std::vector<sf::CircleShape>& controlCircles, bool& update) {
  bezierCurve.clear();
  controlPoints.resize(controlCircles.size());
  for (std::size_t i = 0; i < controlCircles.size(); i++) {
    controlPoints[i].position = controlCircles[i].getPosition();
  }

  for(int i = 0; i < controlCircles.size(); i++) {
    controlPoints[i].position = controlCircles[i].getPosition();
  }
  const int sampleCount = 100;
  auto it = controlCircles.begin();
  for (int i = 0; i <= sampleCount; ++i) {
    float t = i / static_cast<float>(sampleCount);
    sf::Vertex point = drawCurvePoint(controlPoints, t, 0);
    point.color = sf::Color::Yellow; 
    bezierCurve.append(point);
  }
  for (int k = 3; k < controlCircles.size() - 1; k+=3) {
    for (int i = 0; i <= sampleCount; ++i) {
        float t = i / static_cast<float>(sampleCount);
        sf::Vertex point = drawCurvePoint(controlPoints, t, k);
        point.color = sf::Color::Yellow; 
        bezierCurve.append(point);
    }
  }
  update = false;
  return bezierCurve;
}

sf::VertexArray updateBspline(sf::VertexArray& controlPoints, std::vector<sf::CircleShape>& controlCircles, bool& update) {
  bezierCurve.clear();
  controlPoints.resize(controlCircles.size());
  for (std::size_t i = 0; i < controlCircles.size(); i++) {
    controlPoints[i].position = controlCircles[i].getPosition();
  }

  for(int i = 0; i < controlCircles.size(); i++) {
    controlPoints[i].position = controlCircles[i].getPosition();
  }
  const int sampleCount = 100;
  for (int k = 0; k < controlCircles.size() - 3; k++) {
    for (int i = 0; i <= sampleCount; ++i) {
        float t = i / static_cast<float>(sampleCount );
        sf::Vertex point = drawCurvePoint(controlPoints, t, k);
        point.color = sf::Color::Yellow; 
        bezierCurve.append(point);
    }
  }
  update = false;
  return bezierCurve;
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8; 
    auto window = sf::RenderWindow{ {conf::window_size.x, conf::window_size.y}, "Bezier Curve", sf::Style::Fullscreen, settings};
    window.setFramerateLimit(conf::max_framerate);

    sf::VertexArray controlPoints(sf::Points, 4);
    controlPoints[0].position = (sf::Vector2f(100.f, 500.f));   // Start point
    controlPoints[1].position = (sf::Vector2f(200.f, 100.f));   // Control point 1   
    controlPoints[2].position = (sf::Vector2f(600.f, 100.f)); 
    controlPoints[3].position = (sf::Vector2f(700.f, 500.f)); // End

    //Circles representing the control points
    std::vector<sf::CircleShape> controlCircles;
    for (int i = 0; i < controlPoints.getVertexCount(); i++) {
        sf::CircleShape circle(conf::radius);
        circle.setOrigin(conf::radius, conf::radius);
        circle.setPosition(controlPoints[i].position);
        circle.setFillColor(sf::Color::Green);
        if(i == 0 || i == 3) circle.setFillColor(sf::Color::Blue);
        controlCircles.push_back(circle);
    }

    const int sampleCount = 10000;
    for (int i = 0; i <= sampleCount; ++i) {
      float t = i / static_cast<float>(sampleCount);
      sf::Vertex point = drawCurvePoint(controlPoints, t, 0);
      point.color = sf::Color::Yellow; 
      bezierCurve.append(point);
    }

    //Performance boolean :D
    static bool update = false;

    //Button to add more control points
    sf::Texture texture;
    texture.loadFromFile("../src/plus.jpg");
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sf::Vector2u texSize = texture.getSize();
    float scaleX = 25 / static_cast<float>(texSize.x);
    float scaleY = 25 / static_cast<float>(texSize.y);
    sprite.setScale(scaleX, scaleY);
    sprite.setPosition(10, 10);

    sf::Font font;
    font.loadFromFile("../src/open-sans/OpenSans-Regular.ttf");
    sf::Text text("Bezier", font);

    std::vector<sf::RectangleShape> blah = drawBack();

    while (window.isOpen())
    {
      processEvents(window, controlCircles, update, statey);
      window.clear();
      if(statey == state::TITLE) {
        // draw title and then skip everything else
        drawTitle(&window);
      } else {
        for(const auto& tiles : blah) {
          window.draw(tiles);
        }
        window.draw(sprite);
        for(const auto& circle : controlCircles) {
          window.draw(circle);
        }
        if(update == true) {
          if(statey == state::BSPLINE) updateBspline(controlPoints, controlCircles, update);
          if (statey == state::BEZIER) updateBezier(controlPoints, controlCircles, update);
        }
        window.draw(bezierCurve);
      }
      window.display();
    }
    return 0;
}