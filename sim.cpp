#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>

enum class GameState {
    Menu,
    Window1,
    Window2
};

class Planet {
public:
    Planet(const std::string& name, float radius, float mass, float gravity, sf::Color color)
        : name(name), radius(radius), mass(mass), gravity(gravity), color(color) {}

    std::string getName() const { return name; }
    float getRadius() const { return radius; }
    float getMass() const { return mass; }
    float getGravity() const { return gravity; }
    sf::Color getColor() const { return color; }

private:
    std::string name;
    float radius;
    float mass;
    float gravity;
    sf::Color color;
};

class Projectile {
public:
    Projectile(float launchAngle, float initialVelocity, const Planet& planet)
        : launchAngle(launchAngle), initialVelocity(initialVelocity), planet(planet) {}

    float getLaunchAngle() const { return launchAngle; }
    float getInitialVelocity() const { return initialVelocity; }
    const Planet& getPlanet() const { return planet; }

private:
    float launchAngle;
    float initialVelocity;
    const Planet& planet;
};

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode(800, 600), "Planet Selection");

    // Define planets and create the menu
    std::vector<Planet> planets;
    planets.push_back(Planet("Earth", 6371.f, 5.972e24f, 9.8f, sf::Color::Blue));
    planets.push_back(Planet("Moon(Bonus)", 1737.1f, 7.34767309e22f, 1.62f, sf::Color::White));
    planets.push_back(Planet("Mars", 3389.5f, 6.4171e23f, 3.711f, sf::Color::Red));
    planets.push_back(Planet("Jupiter", 69911.f, 1.8982e27f, 24.79f, sf::Color::Yellow));
    planets.push_back(Planet("Saturn", 58232.f, 5.6834e26f, 10.44f, sf::Color::Magenta));
    planets.push_back(Planet("Uranus", 25362.f, 8.6810e25f, 8.87f, sf::Color::Cyan));
    planets.push_back(Planet("Neptune", 24622.f, 1.02413e26f, 11.15f, sf::Color::Green));
    planets.push_back(Planet("Mercury", 2439.7f, 3.3011e23f, 3.7f, sf::Color::Black));
    planets.push_back(Planet("Venus", 6051.8f, 4.8675e24f, 8.87f, sf::Color::White));


    sf::Font font;
    font.loadFromFile("Roboto-Black.ttf"); 

    sf::Text menuText("Select a planet:", font, 24);
    menuText.setFillColor(sf::Color::White);
    menuText.setPosition(20.f, 20.f);

    std::vector<sf::Text> planetLabels;
    for (size_t i = 0; i < planets.size(); ++i) {
        sf::Text label(planets[i].getName(), font, 20);
        label.setFillColor(sf::Color::White);
        label.setPosition(20.f, 60.f + static_cast<float>(i) * 30.f);
        planetLabels.push_back(label);
    }

    int selectedPlanetIndex = -1; 
    float velocity = 0.f;
    float angle = 0.f;
    sf::Texture moonTexture;
    moonTexture.loadFromFile("moon.png");
    sf::Sprite moonSprite(moonTexture);
    moonSprite.setScale(2.f, 2.f);
    moonSprite.setPosition(0.f, 0.f);
    bool isWindow1 = false;
    bool showMenu = true;
    sf::Text ProjNumber("Number of projectiles: ", font, 24);
    ProjNumber.setFillColor(sf::Color::White);
    ProjNumber.setPosition(20.f, 50.f);
    std::string projNumberStr = "";
    sf::Text UserHelp("Type to put in values in order. Press enter after you finish typing to lock in the value for each.", font, 16);
    UserHelp.setFillColor(sf::Color::White);
    UserHelp.setPosition(20.f, 20.f);
    sf::Text velocityInput("Velocity: ", font, 24);
    velocityInput.setFillColor(sf::Color::White);
    velocityInput.setPosition(20.f, 50.f);

    sf::Text angleInput("Angle: ", font, 24);
    angleInput.setFillColor(sf::Color::White);
    angleInput.setPosition(20.f, 100.f);

    std::string velocityStr = "";
    std::string angleStr = "";

    bool velocityInputActive = true;
    bool angleInputActive = false;
    bool projNumberInputActive = false;
    GameState gameState = GameState::Menu;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                switch(gameState){
                    case GameState::Menu:
                        for (size_t i = 0; i < planetLabels.size(); ++i) {
                            if (planetLabels[i].getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                                selectedPlanetIndex = static_cast<int>(i);
                                isWindow1 = true;
                                showMenu = false;
                                break;
                            }
                        }
                        gameState = GameState::Window1;
                        break;
                    // case GameState::NumberInput:
                    //     projNumberInputActive = true;
                    //     break;
                    case GameState::Window1:
                        if (velocityInput.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                            velocityInputActive = true;
                            angleInputActive = false;
                        }
                        else if (angleInput.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                            angleInputActive = true;
                            velocityInputActive = false;
                        }
                        break;
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= 0 && event.text.unicode <= 126) {
                    if (event.text.unicode == 13) {
                        if (velocityInputActive) {
                            velocityInputActive = false;
                            angleInputActive = true;
                            velocity = std::stof(velocityStr);
                        }
                        else if (angleInputActive) {
                            angleInputActive = false;
                            angle = std::stof(angleStr);
                            gameState = GameState::Window2;
                            break;
                        }
                    }
                    if (velocityInputActive) {
                        if (event.text.unicode == 8 && velocityStr.size() > 0) { 
                                velocityStr.pop_back();
                            }
                        else{
                            velocityStr += static_cast<char>(event.text.unicode);}
                        velocityInput.setString("Velocity: " + velocityStr);}
                    else if (angleInputActive) {
                        if (event.text.unicode == 8 && angleStr.size() > 0) {
                                angleStr.pop_back();
                        }   
                        else{
                                angleStr += static_cast<char>(event.text.unicode);}
                        angleInput.setString("Angle: " + angleStr);}
            }
            }
            
            
            if (event.type == sf::Event::Resized) {
                // Fixed SIZE
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }
        window.clear();
        switch (gameState){
        case GameState::Menu:
            window.draw(moonSprite);
            window.draw(menuText);
            for (const sf::Text& label : planetLabels) {
                window.draw(label);
            }
            break;
        
        case GameState::Window1:
            window.draw(UserHelp);  
            window.draw(velocityInput);
            window.draw(angleInput);
            break;

        case GameState::Window2:
            sf::RectangleShape ground(sf::Vector2f(1000000000.f, 2500000000.f));
            ground.setFillColor(planets[selectedPlanetIndex].getColor());
            ground.setPosition(0.f, 550.f);
            sf::CircleShape projectile(5.f);
            projectile.setFillColor(sf::Color::Red);

            float initialX = 400.f;
            float initialY = ground.getPosition().y - projectile.getRadius();
            projectile.setPosition(initialX, initialY);      
            float gravity = planets[selectedPlanetIndex].getGravity(); 
            float initialVelocityX = velocity* std::cos(angle * 3.14159265f / 180.f);
            float initialVelocityY = -velocity * std::sin(angle * 3.14159265f / 180.f);
            float deltaTime = 0.01f;
            float newY = 0.f;
            float newX = 0.f;
            sf::Clock clock;
            // float range = (velocity * velocity * std::sin(2 * angle * 3.14159265f / 180.f)) / gravity;
            // float scalingFactor = 1.0f;
            // if (range > 0.0f) {
            //     scalingFactor = window.getSize().x / range;
            // }
            sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
            bool isProjectileInMotion = true;
            window.draw(ground);
            while (isProjectileInMotion) {
                sf::Time elapsed = clock.restart();
                float deltaTime = elapsed.asSeconds();

                newX = projectile.getPosition().x + initialVelocityX * deltaTime;
                newY = projectile.getPosition().y + initialVelocityY * deltaTime + 0.5f * gravity * deltaTime * deltaTime;
                initialVelocityY += gravity * deltaTime;
                projectile.setPosition(newX, newY);
                view.setCenter(projectile.getPosition());

                window.setView(view);  // Apply the view to the window
                // window.clear();
                // window.draw(ground);
                window.draw(projectile);
                window.display();

                if (newY >= ground.getPosition().y - projectile.getRadius()) {
                    isProjectileInMotion = false;
                }
            }

            // Reset the view to the default view after the animation
            window.setView(window.getDefaultView());
            window.close();


            break;
        }

        window.display();
    }
    return 0;

}
