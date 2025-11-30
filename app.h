
#ifndef APP_H
#define APP_H

#include <SFML/Graphics.hpp>

// Initialize SFML window and load level
bool initApp(const char* filename);

// Main loop
void runApp();

// Clean up resources
void cleanupApp();

// Event handling
void handleInput(sf::RenderWindow& window);

// Rendering functions
void renderFrame(sf::RenderWindow& window);
void drawGrid(sf::RenderWindow& window);
void drawTrains(sf::RenderWindow& window);
void drawUI(sf::RenderWindow& window);

// Helper functions
sf::Color getTileColor(char tile);
sf::Color getTrainColor(int colorIndex);
#endif

