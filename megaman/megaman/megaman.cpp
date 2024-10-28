#include "game.h"

int main(){
    Game game;

    while (!game.GetWindow()->IsDone()) {
        //Game loop
        
        game.Handling();
        game.Update(); //we update twice for some reason, else it will be slow
        game.Render();
        game.RestartClock();
    }
   
    return 0;
}

//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <vector>
//
//class Platform {
//public:
//    sf::RectangleShape shape;
//
//    Platform(float x, float y, float width, float height) {
//        shape.setSize(sf::Vector2f(width, height));
//        shape.setPosition(x, y);
//    }
//};
//
//class Character1 {
//public:
//    sf::RectangleShape shape;
//    float velocityY = 0.0f;
//    bool isJumping = false;
//
//    const float gravity = 980.0f;    // Gravity force (pixels per second^2)
//    const float jumpStrength = 350.0f;
//
//    Character1(float width, float height) {
//        shape.setSize(sf::Vector2f(width, height));
//        shape.setPosition(100, 400); // Start at an initial position
//    }
//
//    void update(float deltaTime, const std::vector<Platform>& platforms) {
//        if (isJumping) {
//            velocityY += gravity * deltaTime; // Apply gravity
//            shape.move(0, velocityY * deltaTime); // Move character
//
//            // Check collision with platforms
//            for (const auto& platform : platforms) {
//                if (checkCollision(platform)) {
//                    velocityY = 0.0f;
//                    isJumping = false;
//                    shape.setPosition(shape.getPosition().x, platform.shape.getPosition().y - shape.getSize().y);
//                    break;
//                }
//            }
//
//            // Check if the character is off-screen at the bottom (falling down)
//            if (shape.getPosition().y >= 600 - shape.getSize().y) {
//                shape.setPosition(shape.getPosition().x, 600 - shape.getSize().y);
//                isJumping = false;
//                velocityY = 0.0f;
//            }
//        }
//    }
//
//    void jump() {
//        if (!isJumping) { // Only jump if not already jumping
//            std::cout << "Jump!" << '\n';
//            isJumping = true;
//            velocityY = -jumpStrength;
//        }
//    }
//
//private:
//    bool checkCollision(const Platform& platform) {
//        // Check if the character is landing on top of the platform
//        return shape.getGlobalBounds().intersects(platform.shape.getGlobalBounds()) &&
//            (shape.getPosition().y + shape.getSize().y - velocityY * 0.1f <= platform.shape.getPosition().y);
//    }
//};
//
//int main() {
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Platform Jumping Example");
//
//    // Create platforms at different heights
//    std::vector<Platform> platforms;
//    platforms.emplace_back(100, 500, 300, 20); // Platform 1
//    platforms.emplace_back(400, 350, 250, 20); // Platform 2
//    platforms.emplace_back(150, 200, 200, 20); // Platform 3
//
//    Character1 character(50, 50);
//    sf::Clock clock;
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        // Handle input
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
//            character.jump();
//        }
//
//        // Calculate deltaTime
//        float deltaTime = clock.restart().asSeconds();
//
//        // Update character movement
//        character.update(deltaTime, platforms);
//
//        // Render
//        window.clear();
//        for (const auto& platform : platforms) {
//            window.draw(platform.shape);
//        }
//        window.draw(character.shape);
//        window.display();
//    }
//
//    return 0;
//}
