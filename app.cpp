#include "app.h"
#include "../core/simulation_state.h"
#include "../core/io.h"
#include "../core/grid.h"
#include "../core/trains.h"
#include "../core/switches.h"
#include <iostream>
#include <cmath>

using namespace std;


sf::RenderWindow* appWindow = nullptr;
sf::Font appFont;
bool fontLoaded = false;
bool isPaused = false;
bool stepMode = false;
int tickDelay = 100;
sf::Clock tickClock;
int tileSize = 32;

bool initApp(const char* filename) {
    // Initialize simulation state
    initializeSimulationState();
    
    // Load level file
    if (!loadLevelFile(filename)) {
        cout << "ERROR: Failed to load level file!" << endl;
        return false;
    }
    
    // Initialize log files
    initializeLogFiles();
    
    // Create window
    int windowWidth = numCols * tileSize + 200;
    int windowHeight = numRows * tileSize + 100;
    
    appWindow = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), "Switchback Rails");
    appWindow->setFramerateLimit(60);
    
    // Try to load font (optional)
    fontLoaded = appFont.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    
    simulationRunning = true;
    
    return true;
}

void handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                isPaused = !isPaused;
            }
            else if (event.key.code == sf::Keyboard::Period) {
                stepMode = true;
            }
            else if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            else if (event.key.code == sf::Keyboard::Up) {
                tickDelay = max(10, tickDelay - 50);
            }
            else if (event.key.code == sf::Keyboard::Down) {
                tickDelay = min(1000, tickDelay + 50);
            }
        }
    }
}

void updateSimulation() {
    // Execute one simulation tick
    spawnTrainsForTick();
    determineAllRoutes();
    updateSwitchCounters();
    queueSwitchFlips();
    moveAllTrains();
    checkArrivals();
    applyDeferredFlips();
    updateSignalLights();
    
    // Log everything
    logTrainTrace();
    logSwitchState();
    logSignalState();
    
    // Check if all trains are done
    bool allDone = true;
    for (int i = 0; i < trainCount; i++) {
        if (trains[i].active && 
            trains[i].state != TRAIN_ARRIVED && 
            trains[i].state != TRAIN_CRASHED) {
            allDone = false;
            break;
        }
    }
    
    if (allDone && currentTick > 0) {
        writeMetrics();
        simulationRunning = false;
    }
    
    currentTick++;
}

sf::Color getTileColor(char tile) {
    if (tile == '-' || tile == '|' || tile == '/' || tile == '\\') {
        return sf::Color(80, 80, 80);
    }
    if (tile == '+') {
        return sf::Color(120, 120, 120);
    }
    if (tile == 'S') {
        return sf::Color(50, 150, 50);
    }
    if (tile == 'D') {
        return sf::Color(150, 50, 50);
    }
    if (tile == '=') {
        return sf::Color(200, 200, 50);
    }
    if (tile == '.') {
        return sf::Color(30, 30, 40);
    }
    if (tile >= 'A' && tile <= 'Z') {
        return sf::Color(100, 100, 200);
    }
    return sf::Color(50, 50, 50);
}

sf::Color getTrainColor(int colorIndex) {
    sf::Color colors[8];
    colors[0] = sf::Color(255, 100, 100);  // Red
    colors[1] = sf::Color(100, 255, 100);  // Green
    colors[2] = sf::Color(100, 100, 255);  // Blue
    colors[3] = sf::Color(255, 255, 100);  // Yellow
    colors[4] = sf::Color(255, 100, 255);  // Magenta
    colors[5] = sf::Color(100, 255, 255);  // Cyan
    colors[6] = sf::Color(255, 150, 100);  // Orange
    colors[7] = sf::Color(200, 100, 255);  // Purple
    
    return colors[colorIndex % 8];
}

void drawGrid(sf::RenderWindow& window) {
    for (int y = 0; y < numRows; y++) {
        for (int x = 0; x < numCols; x++) {
            sf::RectangleShape tile(sf::Vector2f(tileSize - 2, tileSize - 2));
            tile.setPosition(x * tileSize, y * tileSize);
            
            char tileChar = grid[y][x];
            tile.setFillColor(getTileColor(tileChar));
            
            if (isTrackTile(tileChar)) {
                tile.setOutlineThickness(1);
                tile.setOutlineColor(sf::Color(100, 100, 100));
            }
            
            window.draw(tile);
            
            // Draw switch letters
            if (fontLoaded && isSwitchTile(tileChar)) {
                sf::Text text;
                text.setFont(appFont);
                char str[2];
                str[0] = tileChar;
                str[1] = '\0';
                text.setString(str);
                text.setCharacterSize(16);
                text.setFillColor(sf::Color::White);
                text.setPosition(x * tileSize + 8, y * tileSize + 4);
                window.draw(text);
            }
        }
    }
}

void drawTrains(sf::RenderWindow& window) {
    for (int i = 0; i < trainCount; i++) {
        if (trains[i].active && trains[i].state != TRAIN_ARRIVED) {
            float radius = tileSize / 3.0f;
            sf::CircleShape trainShape(radius);
            trainShape.setPosition(
                trains[i].x * tileSize + tileSize / 6.0f,
                trains[i].y * tileSize + tileSize / 6.0f
            );
            
            trainShape.setFillColor(getTrainColor(trains[i].color));
            trainShape.setOutlineThickness(2);
            trainShape.setOutlineColor(sf::Color::White);
            
            window.draw(trainShape);
            
            // Draw train ID
            if (fontLoaded) {
                sf::Text idText;
                idText.setFont(appFont);
                char idStr[10];
                sprintf(idStr, "%d", trains[i].id);
                idText.setString(idStr);
                idText.setCharacterSize(12);
                idText.setFillColor(sf::Color::White);
                idText.setPosition(
                    trains[i].x * tileSize + tileSize / 3.0f,
                    trains[i].y * tileSize + tileSize / 3.0f
                );
                window.draw(idText);
            }
        }
    }
}

void drawUI(sf::RenderWindow& window) {
    if (!fontLoaded) return;
    
    int panelX = numCols * tileSize + 10;
    
    // Tick counter
    sf::Text tickText;
    tickText.setFont(appFont);
    char tickStr[50];
    sprintf(tickStr, "Tick: %d", currentTick);
    tickText.setString(tickStr);
    tickText.setCharacterSize(18);
    tickText.setFillColor(sf::Color::White);
    tickText.setPosition(panelX, 20);
    window.draw(tickText);
    
    // Delivered count
    sf::Text deliveredText;
    deliveredText.setFont(appFont);
    char delStr[50];
    sprintf(delStr, "Delivered: %d", trainsDelivered);
    deliveredText.setString(delStr);
    deliveredText.setCharacterSize(18);
    deliveredText.setFillColor(sf::Color::Green);
    deliveredText.setPosition(panelX, 50);
    window.draw(deliveredText);
    
    // Crashed count
    sf::Text crashedText;
    crashedText.setFont(appFont);
    char crashStr[50];
    sprintf(crashStr, "Crashed: %d", trainsCrashed);
    crashedText.setString(crashStr);
    crashedText.setCharacterSize(18);
    crashedText.setFillColor(sf::Color::Red);
    crashedText.setPosition(panelX, 80);
    window.draw(crashedText);
    
    // Status
    sf::Text statusText;
    statusText.setFont(appFont);
    statusText.setString(isPaused ? "PAUSED" : "RUNNING");
    statusText.setCharacterSize(18);
    statusText.setFillColor(isPaused ? sf::Color::Yellow : sf::Color::Green);
    statusText.setPosition(panelX, 110);
    window.draw(statusText);
    
    // Controls help
    sf::Text helpText;
    helpText.setFont(appFont);
    helpText.setString(
        "Controls:\n"
        "SPACE - Pause\n"
        ". - Step\n"
        "UP/DOWN - Speed\n"
        "ESC - Exit"
    );
    helpText.setCharacterSize(14);
    helpText.setFillColor(sf::Color(150, 150, 150));
    helpText.setPosition(panelX, 150);
    window.draw(helpText);
}

void renderFrame(sf::RenderWindow& window) {
    window.clear(sf::Color(20, 20, 30));
    
    drawGrid(window);
    drawTrains(window);
    drawUI(window);
    
    window.display();
}

void runApp() {
    if (appWindow == nullptr) {
        cout << "ERROR: Window not initialized!" << endl;
        return;
    }
    
    while (appWindow->isOpen() && simulationRunning) {
        handleInput(*appWindow);
        
        if (!isPaused || stepMode) {
            if (tickClock.getElapsedTime().asMilliseconds() >= tickDelay) {
                updateSimulation();
                tickClock.restart();
                stepMode = false;
            }
        }
        
        renderFrame(*appWindow);
    }
}

void cleanupApp() {
    if (appWindow != nullptr) {
        appWindow->close();
        delete appWindow;
        appWindow = nullptr;
    }
}
