#ifndef PROJECT_START_H
#define PROJECT_START_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib>

class Game;

class Start {
public:
    explicit Start(Game &game);
    bool update();

private:
    sf::RenderWindow window;
    sf::Font fontTNR, fontBold, fontHorror, fontRoboto;
    sf::Text startText, settingsText, resultsText;
    sf::RectangleShape startButton, settingsButton, resultsButton;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundImage;
    Game &game;
    bool startGame = false;
    std::string selectedTopic;

    void handleClick(sf::Vector2f mousePos);
    void displaySettings();
    void updateChosenFontColor(int selectedFontType);
    void loadResources();
    void setupLayout();
    void updateElementPositions();
    void handleFontSelection(sf::Vector2f mousePos);
    void handleSizeSelection(sf::Vector2f mousePos);
    void handleTopicSelection(sf::Vector2f mousePos);

    sf::RectangleShape TNRButton, robotoButton, HorrorButton, boldButton, increaseButton, decreaseButton;
    sf::Text TNRText, robotoText, horrorText, boldText, increaseSizeButtonText, decreaseSizeButtonText, currentFontSizeText;
    std::vector<sf::Text> TopicMenuOptions;
    sf::RectangleShape TopicMenu;
    sf::Text TopicMenuText;
    int chosenFont;
};


#endif