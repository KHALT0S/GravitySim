#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

//using namespace std;

const float G = 6.6743e-11; // Scaled gravitational constant for demonstration
const float TIME_STEP = 0.5f; // Time step for the simulation

struct CelestialBody {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float mass;
    sf::Color color;
    float radius; // Visual representation size
};

sf::Vector2f calculateGravitationalForce(const CelestialBody& body1, const CelestialBody& body2) {
    sf::Vector2f direction = body2.position - body1.position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance == 0) return sf::Vector2f(0.f, 0.f);
    float forceMagnitude = (G * body1.mass * body2.mass) / (distance * distance);
    direction /= distance; // Normalize the direction vector
    return direction * forceMagnitude;
}

int main() {
    // Create the window with resizable style
    sf::RenderWindow window(sf::VideoMode(800, 600), "Gravity Simulation", sf::Style::Default | sf::Style::Resize);
    window.setFramerateLimit(144);

    // Initialize stars with properties similar to Alpha Centauri A and B
    float mass1 = 1.1e11f; // Scaled mass for demonstration
    float mass2 = 0.9e11f; // Scaled mass for demonstration
    float distance = 200.f; // Distance between the stars
    float velocityMagnitude = std::sqrt(G * (mass1 + mass2) / distance);

    CelestialBody star1 = { sf::Vector2f(400.f - distance / 2, 300.f), sf::Vector2f(0.f, -velocityMagnitude * (mass2 / (mass1 + mass2))), mass1, sf::Color::Red, 10.f };
    CelestialBody star2 = { sf::Vector2f(400.f + distance / 2, 300.f), sf::Vector2f(0.f, velocityMagnitude * (mass1 / (mass1 + mass2))), mass2, sf::Color::Blue, 10.f };

    // Initialize planets orbiting around star1
    std::vector<CelestialBody> planets = {
        { sf::Vector2f(400.f - distance / 2 + 100.f, 300.f), sf::Vector2f(0.f, 0.5f), 1e9f, sf::Color::Green, 5.f },
        { sf::Vector2f(400.f - distance / 2 + 150.f, 300.f), sf::Vector2f(0.f, 0.4f), 1e9f, sf::Color::Yellow, 5.f },
        { sf::Vector2f(400.f - distance / 2 + 200.f, 300.f), sf::Vector2f(0.f, 0.3f), 1e9f, sf::Color::Cyan, 5.f }
    };

    // Store stars separately for easy manipulation
    std::vector<CelestialBody> stars = { star1, star2 };

    // Initial view setup
    sf::View view = window.getDefaultView();

    // Run the simulation
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized) {
                // Adjust the view when the window is resized
                view.setSize(event.size.width, event.size.height);
                window.setView(view);
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (event.mouseButton.button == sf::Mouse::Right) {
                    // Add a star at the mouse position
                    float newStarMass = 1e11f; // Example mass
                    sf::Color newStarColor = sf::Color::White; // Example color
                    stars.push_back({ mousePos, sf::Vector2f(0.f, 0.f), newStarMass, newStarColor, 10.f });
                }
                else if (event.mouseButton.button == sf::Mouse::Left) {
                    // Add a planet at the mouse position
                    float newPlanetMass = 1e9f; // Example mass
                    sf::Color newPlanetColor = sf::Color::Magenta; // Example color
                    planets.push_back({ mousePos, sf::Vector2f(0.f, 0.f), newPlanetMass, newPlanetColor, 5.f });
                }
            }
        }

        // Calculate gravitational forces and update positions and velocities
        for (auto& planet1 : planets) {
            sf::Vector2f totalForce(0.f, 0.f);
            for (const auto& star : stars) {
                totalForce += calculateGravitationalForce(planet1, star);
            }
            for (const auto& planet2 : planets) {
                if(&planet1 != &planet2){
                    totalForce += calculateGravitationalForce(planet1, planet2);
                }
            }
            planet1.velocity += totalForce / planet1.mass * TIME_STEP;
            planet1.position += planet1.velocity * TIME_STEP;
        }

        for (auto& star1 : stars) {
            sf::Vector2f totalForce(0.f, 0.f);
            for (const auto& star2 : stars) {
                if (&star1 != &star2) {
                    totalForce += calculateGravitationalForce(star1, star2);
                }
            }
            star1.velocity += totalForce / star1.mass * TIME_STEP;
            star1.position += star1.velocity * TIME_STEP;
        }

        // Calculate center of mass
        sf::Vector2f centerOfMass(0.f, 0.f);
        float totalMass = 0.f;
        for (const auto& star : stars) {
            centerOfMass += star.position * star.mass;
            totalMass += star.mass;
        }
        centerOfMass /= totalMass;

        // Clear the window
        window.clear(sf::Color::Black);

        // Create and draw stars
        for (const auto& star : stars) {
            sf::CircleShape shape(star.radius);
            shape.setFillColor(star.color);
            shape.setPosition(star.position - sf::Vector2f(star.radius, star.radius));
            window.draw(shape);
        }

        // Create and draw planets
        for (const auto& planet : planets) {
            sf::CircleShape shape(planet.radius);
            shape.setFillColor(planet.color);
            shape.setPosition(planet.position - sf::Vector2f(planet.radius, planet.radius));
            window.draw(shape);
        }

        // Move the camera to follow the center of mass
        view.setCenter(centerOfMass);
        window.setView(view);

        // Display the window contents
        window.display();
    }

    return 0;
}
