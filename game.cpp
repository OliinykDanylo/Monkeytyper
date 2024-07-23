#include "Game.h"
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>

Game::Game(sf::RenderWindow &win) : window(win), color(sf::Color::White), fontSize(30), speed(100), points(0), wordCount(0), spawnInterval(2.5), timeElapsed(0), lives(1), gameStatus(Active), paused(false), chosenFont(0) {
    loadResources();
    setupLayout();
}

void Game::start() {
    setCategory(currentCategory);
    if (wordList.empty()) {
        std::cerr << "No words loaded from file." << std::endl;
        return;
    }
    while (window.isOpen()) {
        window.clear();
        handleInput();
        if (gameStatus == Active) {
            updateGame();
        }
        render();
        window.display();
    }
}

void Game::loadResources() {
    if (!gameFont.loadFromFile("../assets/TimesNewRoman.ttf") ||
        !fontTNR.loadFromFile("../assets/TimesNewRoman.ttf") ||
        !fontBold.loadFromFile("../assets/Bold.ttf") ||
        !fontHorror.loadFromFile("../assets/Horror.ttf") ||
        !fontRoboto.loadFromFile("../assets/Roboto.ttf") ||
        !bgTexture.loadFromFile("../assets/forest.png")) {
        std::cerr << "Failed to load resources." << std::endl;
    }
    bgImage.setTexture(bgTexture);
}
void Game::setupLayout() {
    sf::Vector2u winSize = window.getSize();
    sf::Vector2u texSize = bgTexture.getSize();
    float scaleX = static_cast<float>(winSize.x) / texSize.x;
    float scaleY = static_cast<float>(winSize.y) / texSize.y;
    bgImage.setScale(scaleX, scaleY);

    updateElementPositions();
}
void Game::updateElementPositions() {
    sf::Vector2u windowSize = window.getSize();

    scoreBg.setSize(sf::Vector2f(windowSize.x, 100));
    scoreBg.setPosition(0, windowSize.y - 100);

    scoreText.setPosition(500, windowSize.y - 80);
    typedText.setPosition(10, windowSize.y - 80);
    livesText.setPosition(windowSize.x - 200, windowSize.y - 80);

    const float toTheLeft = 50.0f;

    pauseBtn.setPosition(windowSize.x - 30, 10);
    resumeBtn.setPosition((windowSize.x - resumeBtn.getSize().x) / 3 - toTheLeft, (windowSize.y - resumeBtn.getSize().y) / 2 + 100);
    exitBtn.setPosition((windowSize.x - exitBtn.getSize().x) / 1.5 - toTheLeft, (windowSize.y - exitBtn.getSize().y) / 2 + 100);
    restartBtn.setPosition((windowSize.x - restartBtn.getSize().x) / 3 - toTheLeft, (windowSize.y - restartBtn.getSize().y) / 2 + 100);
    resultsBtn.setPosition(windowSize.x - 250, windowSize.y - 100);
    resultsText.setPosition(resultsBtn.getPosition().x + resultsBtn.getSize().x / 2.0f - resultsText.getLocalBounds().width / 2.0f, resultsBtn.getPosition().y + 10);
}

void Game::uploadWordsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string word;
        while (file >> word) {
            wordList.push_back(word);
        }
        file.close();
    } else {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }

    if (wordList.empty()) {
        std::cerr << "No words loaded from file " << filename << std::endl;
    }
}
void Game::setCategory(const std::string &category) {
    currentCategory = category;
    categoryFilePath = "../assets/" + category + ".txt";
    uploadWordsFromFile(categoryFilePath);
}
void Game::changeFont(const sf::Font &newFont) {
    gameFont = newFont;
    for (auto& text : textItems) {
        text.setFont(gameFont);
    }
}
void Game::changeFontSize(int newSize) {
    fontSize = newSize;
    for (auto& text : textItems) {
        text.setCharacterSize(fontSize);
    }
}
int Game::getFontSize() const {
    return fontSize;
}

void Game::render() {
    window.draw(bgImage);

    if (gameStatus == Active) {
        displayWords();
        displayScorePanel();
        displayPauseBtn();
    }
    if (gameStatus == Paused) {
        displayResumeBtn();
        displayExitBtn();
        displayScorePanel();
    }

    if (gameStatus == Ended) {
        displayGameOver();
        displayExitBtn();
        displayRestartBtn();
        displayResultsBtn();
    }

    if (gameStatus == RestartMenu) {
        displayRestartMenu();
        displayExitBtn();
        displayResultsBtn();
    }
}
void Game::displayScorePanel() {
    window.draw(scoreBg);

    scoreText.setFont(gameFont);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setString("Your Score: " + std::to_string(points));
    window.draw(scoreText);

    typedText.setFont(gameFont);
    typedText.setFillColor(sf::Color::Black);
    typedText.setString("Typed Word: " + typedWord);
    window.draw(typedText);

    livesText.setFont(gameFont);
    livesText.setFillColor(sf::Color::Black);
    livesText.setString("Lives: " + std::to_string(lives));
    window.draw(livesText);
}
void Game::displayGameOver() {
    sf::Text gameOverText;
    gameOverText.setFont(gameFont);
    gameOverText.setCharacterSize(fontSize);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("Game Over. Your final score: " + std::to_string(points));
    gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getLocalBounds().width / 2, window.getSize().y / 2 - gameOverText.getLocalBounds().height / 2);
    window.draw(gameOverText);
}
void Game::displayPauseBtn() {
    pauseBtn.setSize(sf::Vector2f(20, 20));
    pauseBtnText.setFont(gameFont);
    pauseBtnText.setCharacterSize(20);
    pauseBtnText.setFillColor(sf::Color::Black);
    pauseBtnText.setString("||");

    sf::FloatRect buttonBounds = pauseBtn.getGlobalBounds();
    sf::FloatRect textBounds = pauseBtnText.getLocalBounds();
    float textX = buttonBounds.left + (buttonBounds.width - textBounds.width) / 2;
    float textY = buttonBounds.top + (buttonBounds.height - textBounds.height) / 2 - textBounds.top;
    pauseBtnText.setPosition(textX, textY);

    window.draw(pauseBtn);
    window.draw(pauseBtnText);
}
void Game::displayResumeBtn() {
    resumeBtn.setSize(sf::Vector2f(200, 50));
    resumeBtn.setFillColor(sf::Color::Black);

    resumeBtnText.setFont(gameFont);
    resumeBtnText.setCharacterSize(40);
    resumeBtnText.setFillColor(sf::Color::White);
    resumeBtnText.setString("RESUME");

    sf::FloatRect buttonBounds = resumeBtn.getGlobalBounds();
    sf::FloatRect textBounds = resumeBtnText.getLocalBounds();
    float textX = buttonBounds.left + (buttonBounds.width - textBounds.width) / 2;
    float textY = buttonBounds.top + (buttonBounds.height - textBounds.height) / 2 - textBounds.top;
    resumeBtnText.setPosition(textX, textY);

    window.draw(resumeBtn);
    window.draw(resumeBtnText);
}
void Game::displayExitBtn() {
    exitBtn.setSize(sf::Vector2f(200, 50));
    exitBtn.setFillColor(sf::Color::Black);

    exitBtnText.setFont(gameFont);
    exitBtnText.setCharacterSize(40);
    exitBtnText.setFillColor(sf::Color::White);
    exitBtnText.setString("EXIT");

    sf::FloatRect buttonBounds = exitBtn.getGlobalBounds();
    sf::FloatRect textBounds = exitBtnText.getLocalBounds();
    float textX = buttonBounds.left + (buttonBounds.width - textBounds.width) / 2;
    float textY = buttonBounds.top + (buttonBounds.height - textBounds.height) / 2 - textBounds.top;
    exitBtnText.setPosition(textX, textY);

    window.draw(exitBtn);
    window.draw(exitBtnText);
}
void Game::displayRestartBtn() {
    restartBtn.setSize(sf::Vector2f(200, 50));
    restartBtn.setFillColor(sf::Color::Black);

    restartBtnText.setFont(gameFont);
    restartBtnText.setCharacterSize(40);
    restartBtnText.setFillColor(sf::Color::White);
    restartBtnText.setString("RESTART");

    sf::FloatRect buttonBounds = restartBtn.getGlobalBounds();
    sf::FloatRect textBounds = restartBtnText.getLocalBounds();
    float textX = buttonBounds.left + (buttonBounds.width - textBounds.width) / 2;
    float textY = buttonBounds.top + (buttonBounds.height - textBounds.height) / 2 - textBounds.top;
    restartBtnText.setPosition(textX, textY);

    window.draw(restartBtn);
    window.draw(restartBtnText);
}
void Game::displayResultsBtn() {
    resultsBtn.setSize(sf::Vector2f(200, 50));
    resultsBtn.setFillColor(sf::Color::Black);

    resultsText.setFont(gameFont);
    resultsText.setString("Results");
    resultsText.setFillColor(sf::Color::White);
    resultsText.setCharacterSize(24);
    resultsText.setPosition(resultsBtn.getPosition().x + resultsBtn.getSize().x / 2.0f - resultsText.getLocalBounds().width / 2.0f, resultsBtn.getPosition().y + 10);

    window.draw(resultsBtn);
    window.draw(resultsText);
}
void Game::displayWords() {
    for (const auto& word : wordsOnScreen) {
        if (!word.typedPart.empty()) {
            sf::Text highlightedText = word.text;
            highlightedText.setString(word.typedPart);
            highlightedText.setFillColor(sf::Color(211, 211, 211));

            sf::Text remainingText = word.text;
            remainingText.setString(word.fullWord.substr(word.typedPart.length()));
            float offsetX = highlightedText.getLocalBounds().width;
            remainingText.setPosition(word.text.getPosition().x + offsetX, word.text.getPosition().y);

            window.draw(highlightedText);
            window.draw(remainingText);
        } else {
            window.draw(word.text);
        }
    }
}
void Game::displayRestartMenu() {
    const float buttonSpacing = 25.0f;
    float yPos = 100 + buttonSpacing * 2;

    sf::Text titleText("Choose a font:", fontTNR, 24);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(25, yPos - titleText.getLocalBounds().height - buttonSpacing + 10);

    TNRButton.setSize(sf::Vector2f(250, 25));
    TNRButton.setFillColor(sf::Color(255, 255, 255, 0)); // Transparent white color
    TNRButton.setPosition(25, yPos);

    TNRText.setString(" - Times New Roman (default font)");
    TNRText.setFont(fontTNR);
    TNRText.setCharacterSize(24);
    TNRText.setPosition(50, yPos);
    yPos += TNRText.getLocalBounds().height + buttonSpacing;

    robotoButton.setSize(sf::Vector2f(100, robotoText.getLocalBounds().height + robotoText.getLocalBounds().height / 2));
    robotoButton.setFillColor(sf::Color(255, 255, 255, 0));
    robotoButton.setPosition(50, yPos);

    robotoText.setString(" - Roboto");
    robotoText.setFont(fontRoboto);
    robotoText.setCharacterSize(24);
    robotoText.setPosition(50, yPos);
    yPos += robotoText.getLocalBounds().height + buttonSpacing;

    HorrorButton.setSize(sf::Vector2f(100, horrorText.getLocalBounds().height + horrorText.getLocalBounds().height / 2));
    HorrorButton.setFillColor(sf::Color(255, 255, 255, 0));
    HorrorButton.setPosition(50, yPos);

    horrorText.setString(" - Horror");
    horrorText.setFont(fontHorror);
    horrorText.setCharacterSize(24);
    horrorText.setPosition(50, yPos);
    yPos += horrorText.getLocalBounds().height + buttonSpacing;

    boldButton.setSize(sf::Vector2f(100, boldText.getLocalBounds().height + boldText.getLocalBounds().height / 2));
    boldButton.setFillColor(sf::Color(255, 255, 255, 0));
    boldButton.setPosition(50, yPos);

    boldText.setString(" - Bold");
    boldText.setFont(fontBold);
    boldText.setCharacterSize(24);
    boldText.setPosition(50, yPos);
    yPos += boldText.getLocalBounds().height + buttonSpacing;

    sf::Text chooseSizeOfFontText("Choose a size of font:", fontTNR, 24);
    chooseSizeOfFontText.setFillColor(sf::Color::White);
    chooseSizeOfFontText.setPosition(50, yPos);

    decreaseButton.setSize(sf::Vector2f(25, 25));
    decreaseButton.setFillColor(sf::Color(255, 255, 255, 0));
    decreaseButton.setPosition(chooseSizeOfFontText.getPosition().x + chooseSizeOfFontText.getLocalBounds().width + 20, yPos);

    decreaseSizeButtonText.setString("-");
    decreaseSizeButtonText.setFont(fontTNR);
    decreaseSizeButtonText.setCharacterSize(24);
    decreaseSizeButtonText.setPosition(chooseSizeOfFontText.getPosition().x + chooseSizeOfFontText.getLocalBounds().width + 20, yPos);

    increaseButton.setSize(sf::Vector2f(25, 25));
    increaseButton.setFillColor(sf::Color(255, 255, 255, 0));
    increaseButton.setPosition(decreaseButton.getPosition().x + increaseButton.getSize().x + 30, yPos);

    increaseSizeButtonText.setString("+");
    increaseSizeButtonText.setFont(fontTNR);
    increaseSizeButtonText.setCharacterSize(24);
    increaseSizeButtonText.setPosition(decreaseButton.getPosition().x + increaseButton.getSize().x + 30, yPos);

    currentFontSizeText.setFont(fontTNR);
    currentFontSizeText.setCharacterSize(24);
    currentFontSizeText.setFillColor(sf::Color::White);
    currentFontSizeText.setString(std::to_string(getFontSize()));
    currentFontSizeText.setPosition(decreaseButton.getPosition().x + 25, yPos);

    window.draw(titleText);
    window.draw(TNRButton);
    window.draw(TNRText);
    window.draw(robotoButton);
    window.draw(robotoText);
    window.draw(HorrorButton);
    window.draw(horrorText);
    window.draw(boldButton);
    window.draw(boldText);

    window.draw(chooseSizeOfFontText);
    window.draw(increaseButton);
    window.draw(increaseSizeButtonText);
    window.draw(decreaseButton);
    window.draw(decreaseSizeButtonText);
    window.draw(currentFontSizeText);

    TopicMenu.setSize(sf::Vector2f(200, 50));
    TopicMenu.setFillColor(sf::Color(255, 255, 255, 0));
    TopicMenu.setPosition(chooseSizeOfFontText.getPosition().x, currentFontSizeText.getPosition().y + buttonSpacing);

    TopicMenuText.setFont(fontTNR);
    TopicMenuText.setString("Select Topic: " + selectedTopic);
    TopicMenuText.setFillColor(sf::Color::White);
    TopicMenuText.setCharacterSize(24);
    TopicMenuText.setPosition(50, TopicMenu.getPosition().y);

    std::vector<std::string> topics = {"Mix", "Technology", "Food", "Entertainment"};
    TopicMenuOptions.clear();
    for (size_t i = 0; i < topics.size(); ++i) {
        sf::Text option;
        option.setFont(fontTNR);
        std::string optionText = "- " + topics[i];
        option.setString(optionText);
        option.setFillColor(topics[i] == selectedTopic ? sf::Color::Black : sf::Color::White);
        option.setCharacterSize(24);
        option.setPosition(TopicMenu.getPosition().x, TopicMenu.getPosition().y + TopicMenu.getSize().y + 5 + i * 30 - 10);
        TopicMenuOptions.push_back(option);
    }

    window.draw(TopicMenu);
    window.draw(TopicMenuText);

    for (const auto &option : TopicMenuOptions) {
        window.draw(option);
    }

    confirmBtn.setSize(sf::Vector2f(200, 50));
    confirmBtn.setFillColor(sf::Color::Black);
    confirmBtn.setPosition(restartBtn.getPosition().x, restartBtn.getPosition().y);
    window.draw(confirmBtn);

    confirmBtnText.setString("CONFIRM");
    confirmBtnText.setFont(gameFont);
    confirmBtnText.setFillColor(sf::Color::White);
    confirmBtnText.setPosition(confirmBtn.getPosition().x + (confirmBtn.getSize().x - confirmBtnText.getLocalBounds().width) / 2, confirmBtn.getPosition().y + (confirmBtn.getSize().y - confirmBtnText.getLocalBounds().height) / 2 - confirmBtnText.getLocalBounds().top);
    window.draw(confirmBtnText);
}
void Game::updateChosenFontColor(int selectedFont) {
    TNRText.setFillColor(selectedFont == 0 ? sf::Color::Black : sf::Color::White);
    robotoText.setFillColor(selectedFont == 1 ? sf::Color::Black : sf::Color::White);
    horrorText.setFillColor(selectedFont == 2 ? sf::Color::Black : sf::Color::White);
    boldText.setFillColor(selectedFont == 3 ? sf::Color::Black : sf::Color::White);
}

void Game::handleMouseClick(sf::Vector2f mousePos) {
    if (gameStatus == Active || gameStatus == Paused) {
        if (pauseBtn.getGlobalBounds().contains(mousePos)) {
            gameStatus = Paused;
            paused = true;
            pausedTime = gameClock.getElapsedTime();
        } else if (resumeBtnText.getGlobalBounds().contains(mousePos)) {
            gameStatus = Active;
            paused = false;
            gameClock.restart();
            timeElapsed -= pausedTime.asSeconds();
        } else if (exitBtn.getGlobalBounds().contains(mousePos)) {
            saveResult();
            window.close();
        } else if (restartBtn.getGlobalBounds().contains(mousePos)) {
            reset();
        }
    } else if (gameStatus == RestartMenu) {
        if (TNRButton.getGlobalBounds().contains(mousePos)) {
            changeFont(fontTNR);
            chosenFont = 0;
            updateChosenFontColor(chosenFont);
        } else if (robotoButton.getGlobalBounds().contains(mousePos)) {
            changeFont(fontRoboto);
            chosenFont = 1;
            updateChosenFontColor(chosenFont);
        } else if (HorrorButton.getGlobalBounds().contains(mousePos)) {
            changeFont(fontHorror);
            chosenFont = 2;
            updateChosenFontColor(chosenFont);
        } else if (boldButton.getGlobalBounds().contains(mousePos)) {
            changeFont(fontBold);
            chosenFont = 3;
            updateChosenFontColor(chosenFont);
        } else if (increaseButton.getGlobalBounds().contains(mousePos)) {
            changeFontSize(getFontSize() + 1);
        } else if (decreaseButton.getGlobalBounds().contains(mousePos)) {
            changeFontSize(getFontSize() - 1);
        } else {
            for (size_t i = 0; i < TopicMenuOptions.size(); ++i) {
                if (TopicMenuOptions[i].getGlobalBounds().contains(mousePos)) {
                    selectedTopic = TopicMenuOptions[i].getString().substring(2);
                    TopicMenuText.setString("Select Topic: " + selectedTopic);
                    setCategory(selectedTopic);
                    break;
                }
            }
        }
        if (exitBtn.getGlobalBounds().contains(mousePos)) {
            window.close();
        }
        if (confirmBtn.getGlobalBounds().contains(mousePos)) {
            changeFont(gameFont);
            changeFontSize(fontSize);
            setCategory(currentCategory);
            points = 0;
            wordCount = 0;
            timeElapsed = 0;
            speed = 100;
            lives = 5;

            wordsOnScreen.clear();
            typedWord.clear();
            gameStatus = Active;

            wordList.clear();
            setCategory(currentCategory);

            gameClock.restart();
        }
    }
}
void Game::handleGameOverScreenMouseClick(sf::Vector2f mousePos) {
    if (exitBtn.getGlobalBounds().contains(mousePos)) {
        window.close();
    } else if (restartBtn.getGlobalBounds().contains(mousePos)) {
        reset();
    } else if (resultsBtn.getGlobalBounds().contains(mousePos)) {
        std::system("open \"../assets/gameResults.txt\"");
    }
}
void Game::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        else if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode < 128) {
                char typedChar = static_cast<char>(event.text.unicode);
                if (typedChar == '\b') {
                    if (!typedWord.empty()) {
                        typedWord.pop_back();
                    }
                } else if (typedChar == '\r' || typedChar == '\n') {
                    auto wordIter = std::find_if(wordsOnScreen.begin(), wordsOnScreen.end(),
                                                 [this](const ActiveWord& word) {
                                                     return word.fullWord == typedWord;
                                                 });
                    if (wordIter != wordsOnScreen.end()) {
                        wordsOnScreen.erase(wordIter);
                        typedWord.clear();
                        points++;
                    }
                } else {
                    typedWord += typedChar;
                }

                ActiveWord* closestWord = nullptr;
                float maxPositionX = -1.0f;
                for (auto& word : wordsOnScreen) {
                    if (word.fullWord.substr(0, typedWord.length()) == typedWord) {
                        if (word.text.getPosition().x > maxPositionX) {
                            closestWord = &word;
                            maxPositionX = word.text.getPosition().x;
                        }
                    }
                }

                for (auto& word : wordsOnScreen) {
                    if (&word == closestWord) {
                        word.typedPart = typedWord;
                    } else {
                        word.typedPart = "";
                    }
                }
            }
        } else if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (gameStatus == Ended) {
                handleGameOverScreenMouseClick(mousePos);
            } else {
                handleMouseClick(mousePos);
            }
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                if (gameStatus == Active) {
                    gameStatus = Paused;
                    paused = true;
                    pausedTime = gameClock.getElapsedTime();
                } else if (gameStatus == Paused) {
                    gameStatus = Active;
                    paused = false;
                    gameClock.restart();
                    timeElapsed -= pausedTime.asSeconds();
                }
            } else if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                setupLayout();
            }
        }
    }
}

void Game::reset() {
    gameStatus = RestartMenu;
}
void Game::updateGame() {
    if (gameStatus == Active && !paused) {
        dt = gameClock.restart().asSeconds();
        updateWords();

        if (timeElapsed >= spawnInterval) {
            if (lives > 0) {
                spawnWord();
                ++wordCount;
                timeElapsed = 0.0;
                if (wordCount % 15 == 0) {
                    speed += 10.0;
                    spawnInterval = std::max(static_cast<float>(spawnInterval - 0.5f), 0.5f);
                }
            }
        }
        timeElapsed += dt;
        removeOutOfBoundsWords();
        if (lives <= 0) {
            std::cerr << "Game Over: No lives left." << std::endl;
            gameStatus = Ended;
            wordsOnScreen.clear();
            saveResult();
        }
    }
}
void Game::spawnWord() {
    ActiveWord newWord;
    newWord.fullWord = wordList[rand() % wordList.size()];
    newWord.typedPart = "";
    newWord.text.setString(newWord.fullWord);
    newWord.text.setFont(gameFont);
    newWord.text.setCharacterSize(fontSize);
    newWord.text.setFillColor(color);

    float x = -newWord.text.getLocalBounds().width;
    float maxY = static_cast<float>(window.getSize().y - newWord.text.getLocalBounds().height - 130);
    float minY = 0.0f;
    float scorePanelHeight = 100.0f;
    float y = static_cast<float>(std::rand() % static_cast<int>(maxY - minY - scorePanelHeight)) + minY;
    newWord.text.setPosition(x, y);

    wordsOnScreen.push_back(newWord);
}
void Game::updateWords() {
    for (auto& word : wordsOnScreen) {
        word.text.move(speed * dt, 0);
    }
}
void Game::removeOutOfBoundsWords() {
    wordsOnScreen.erase(std::remove_if(wordsOnScreen.begin(), wordsOnScreen.end(),
                                       [this](const ActiveWord &word) {
                                           if (word.text.getPosition().x > window.getSize().x) {
                                               --lives;
                                               return true;
                                           }
                                           return false;
                                       }), wordsOnScreen.end());
}

void Game::saveResult() const {
    std::ofstream file("../assets/gameResults.txt", std::ios::app);
    if (file.is_open()) {
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);

        file << "Date: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << ", Score: " << points << "\n";
        file.close();
    } else {
        std::cerr << "Failed to open game_results.txt for writing." << std::endl;
    }
}