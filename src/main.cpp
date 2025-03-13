#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include "events.hpp"
#include "configuration.hpp"

sf::Vertex drawCurvePoint(sf::VertexArray points, float t) {
    if (points.getVertexCount() == 1) {
        return points[0];
    } else {
        sf::VertexArray newpoints;
        newpoints.setPrimitiveType(sf::Points);
        newpoints.resize(points.getVertexCount() - 1);
        for (std::size_t i = 0; i < newpoints.getVertexCount(); i++) {
            newpoints[i].position.x = (1 - t) * points[i].position.x + t * points[i + 1].position.x;
            newpoints[i].position.y = (1 - t) * points[i].position.y + t * points[i + 1].position.y;
        }
        return drawCurvePoint(newpoints, t);
    }
}

sf::VertexArray updateCurve(sf::VertexArray& controlPoints, std::vector<sf::CircleShape>& controlCircles, bool& update) {
  sf::VertexArray bezierCurve(sf::LineStrip);
  for(int i = 0; i < controlCircles.size(); i++) {
    controlPoints[i].position = controlCircles[i].getPosition();
  }
  const int sampleCount = 10000;
  for (int i = 0; i <= sampleCount; ++i) {
      float t = i / static_cast<float>(sampleCount);
      sf::Vertex point = drawCurvePoint(controlPoints, t);
      point.color = sf::Color::Yellow; 
      bezierCurve.append(point);
  }
  update = false;
  return bezierCurve;
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8; 
    auto window = sf::RenderWindow{ {conf::window_size.x, conf::window_size.y}, "Bezier Curve", sf::Style::Default, settings};
    window.setFramerateLimit(conf::max_framerate);

    sf::VertexArray controlPoints(sf::Points, 4);
    controlPoints[0].position = (sf::Vector2f(100.f, 500.f));   // Start point
    controlPoints[1].position = (sf::Vector2f(200.f, 100.f));   // Control point 1   
    controlPoints[2].position = (sf::Vector2f(600.f, 100.f)); 
    controlPoints[3].position = (sf::Vector2f(700.f, 500.f)); // End

    std::vector<sf::CircleShape> controlCircles;
    for (int i = 0; i < controlPoints.getVertexCount(); i++) {
        sf::CircleShape circle(conf::radius);
        circle.setOrigin(conf::radius, conf::radius);
        circle.setPosition(controlPoints[i].position);
        circle.setFillColor(sf::Color::Green);
        controlCircles.push_back(circle);
    }


    static bool update = false;
    while (window.isOpen())
    {
        processEvents(window, controlCircles, update);


        window.clear();

        for(const auto& circle : controlCircles) {
          window.draw(circle);
        }
        window.draw(updateCurve(controlPoints, controlCircles, update));

        window.display();
    }

    return 0;
}