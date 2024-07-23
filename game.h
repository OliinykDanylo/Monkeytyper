#ifndef PROJECT_GAME_H
#define PROJECT_GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib>

class Game {
public:
    Game(sf::RenderWindow &);
    void start();
    void setCategory(const std::string &category);
    void changeFont(const sf::Font &newFont);
    void changeFontSize(int newSize);
    int getFontSize() const;

    enum GameState { Active, Paused, Ended, RestartMenu } gameStatus = Active;
private:
    sf::RenderWindow &window;
    sf::Font gameFont;
    sf::Color color;
    float dt;
    int fontSize;
    float speed;
    int points;
    int wordCount;
    float spawnInterval;
    float timeElapsed;
    std::string currentCategory;
    std::string categoryFilePath;
    std::vector<std::string> wordList;
    std::vector<sf::Text> textItems;
    bool paused = false;
    sf::Clock gameClock;
    sf::Time pausedTime;
    sf::Texture bgTexture;
    sf::Sprite bgImage;

    struct ActiveWord {
        std::string fullWord;
        std::string typedPart;
        sf::Text text;
    };

    std::vector<ActiveWord> wordsOnScreen;
    std::string typedWord;
    int lives = 5;

    void updateElementPositions();
    void loadResources();
    void setupLayout();
    void displayRestartMenu();
    void uploadWordsFromFile(const std::string &filename);
    void handleInput();
    void render();
    void displayWords();
    void updateWords();
    void removeOutOfBoundsWords();
    void saveResult() const;
    void spawnWord();
    void displayScorePanel();
    void displayGameOver();
    void displayPauseBtn();
    void displayResumeBtn();
    void displayExitBtn();
    void displayRestartBtn();
    void displayResultsBtn();
    void handleMouseClick(sf::Vector2f mousePos);
    void handleGameOverScreenMouseClick(sf::Vector2f mousePos);
    void reset();
    void updateGame();
    void updateChosenFontColor(int selectedFontType);

    sf::RectangleShape scoreBg, pauseBtn, resumeBtn, exitBtn, restartBtn, resultsBtn;
    sf::Text scoreText, typedText, livesText, pauseBtnText, resumeBtnText, exitBtnText, restartBtnText, resultsText, homeTxt;

    //for restart
    sf::Font fontTNR, fontBold, fontHorror, fontRoboto;
    sf::RectangleShape TNRButton, robotoButton, HorrorButton, boldButton, increaseButton, decreaseButton, confirmBtn;
    sf::Text TNRText, robotoText, horrorText, boldText, increaseSizeButtonText, decreaseSizeButtonText, currentFontSizeText, confirmBtnText;
    std::vector<sf::Text> TopicMenuOptions;
    sf::RectangleShape TopicMenu;
    sf::Text TopicMenuText;
    std::string selectedTopic;
    int chosenFont;
};

#endif