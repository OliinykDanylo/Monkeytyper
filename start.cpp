#include "start.h"
#include "game.h"
#include <iostream>

Start::Start(Game &game) : window(sf::VideoMode(1200, 800), "MonkeyTyper Start"), fontTNR(), startText(), startButton(), startGame(false), game(game), chosenFont(0) {
    loadResources();
    setupLayout();
}
bool Start::update() {
    selectedTopic = "Mix";
    game.setCategory(selectedTopic);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    handleClick(mousePos);
                    if (startGame) {
                        game.start();
                        startGame = false;
                        return true;
                    }
                }
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal) {
                    game.changeFontSize(game.getFontSize() + 1);
                } else if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Hyphen) {
                    game.changeFontSize(game.getFontSize() - 1);
                }
            } else if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                setupLayout();
            }
        }
        window.clear();
        window.draw(backgroundImage);
        window.draw(startButton);
        window.draw(startText);
        window.draw(settingsText);
        window.draw(resultsButton);
        window.draw(resultsText);

        displaySettings();
        window.display();
    }
    return false;
}

void Start::loadResources() {
    if (!fontTNR.loadFromFile("../assets/TimesNewRoman.ttf") ||
        !fontBold.loadFromFile("../assets/Bold.ttf") ||
        !fontHorror.loadFromFile("../assets/Horror.ttf") ||
        !fontRoboto.loadFromFile("../assets/Roboto.ttf") ||
        !backgroundTexture.loadFromFile("../assets/forest.png")) {
        std::cerr << "Failed to load resources." << std::endl;
    }
    backgroundImage.setTexture(backgroundTexture);
    startButton.setSize(sf::Vector2f(250, 50));
    startButton.setFillColor(sf::Color::Black);
    startText.setFont(fontTNR);
    startText.setString("Start");
    startText.setFillColor(sf::Color::White);
    settingsText.setFont(fontHorror);
    settingsText.setString("Settings");
    settingsText.setCharacterSize(35);
    settingsText.setFillColor(sf::Color::Black);
    resultsButton.setSize(sf::Vector2f(200, 50));
    resultsButton.setFillColor(sf::Color::Black);
    resultsText.setFont(fontTNR);
    resultsText.setString("Results");
    resultsText.setFillColor(sf::Color::White);
    resultsText.setCharacterSize(24);
}
void Start::setupLayout() {
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    backgroundImage.setScale(scaleX, scaleY);

    updateElementPositions();
}

void Start::updateElementPositions() {
    sf::Vector2u windowSize = window.getSize();

    startButton.setPosition(windowSize.x - 300, windowSize.y / 2);
    startText.setPosition(startButton.getPosition().x + startButton.getSize().x / 2.0f - startText.getLocalBounds().width / 2.0f, startButton.getPosition().y + 10);

    settingsText.setPosition(50, 200);

    resultsButton.setPosition(windowSize.x - 250, windowSize.y - 100);
    resultsText.setPosition(resultsButton.getPosition().x + resultsButton.getSize().x / 2.0f - resultsText.getLocalBounds().width / 2.0f, resultsButton.getPosition().y + 10);

    updateChosenFontColor(chosenFont);
}

void Start::handleClick(sf::Vector2f mousePos) {
    if (startButton.getGlobalBounds().contains(mousePos)) {
        startGame = true;
        window.close();
    } else if (resultsButton.getGlobalBounds().contains(mousePos)) {
        std::system("open \"../assets/gameResults.txt\"");
    } else {
        handleFontSelection(mousePos);
        handleSizeSelection(mousePos);
        handleTopicSelection(mousePos);
    }
}

void Start::handleFontSelection(sf::Vector2f mousePos) {
    if (TNRButton.getGlobalBounds().contains(mousePos)) {
        game.changeFont(fontTNR);
        chosenFont = 0;
        updateChosenFontColor(chosenFont);
    } else if (robotoButton.getGlobalBounds().contains(mousePos)) {
        game.changeFont(fontRoboto);
        chosenFont = 1;
        updateChosenFontColor(chosenFont);
    } else if (HorrorButton.getGlobalBounds().contains(mousePos)) {
        game.changeFont(fontHorror);
        chosenFont = 2;
        updateChosenFontColor(chosenFont);
    } else if (boldButton.getGlobalBounds().contains(mousePos)) {
        game.changeFont(fontBold);
        chosenFont = 3;
        updateChosenFontColor(chosenFont);
    }
}
void Start::handleSizeSelection(sf::Vector2f mousePos) {
    if (increaseButton.getGlobalBounds().contains(mousePos)) {
        game.changeFontSize(game.getFontSize() + 1);
    } else if (decreaseButton.getGlobalBounds().contains(mousePos)) {
        game.changeFontSize(game.getFontSize() - 1);
    }
}
void Start::handleTopicSelection(sf::Vector2f mousePos) {
    for (size_t i = 0; i < TopicMenuOptions.size(); ++i) {
        if (TopicMenuOptions[i].getGlobalBounds().contains(mousePos)) {
            selectedTopic = TopicMenuOptions[i].getString().substring(2);
            TopicMenuText.setString("Select Topic: " + selectedTopic);
            game.setCategory(selectedTopic);
            break;
        }
    }
}

void Start::displaySettings() {
    const float buttonSpacing = 25.0f;
    float yPos = settingsText.getPosition().y + settingsText.getLocalBounds().height + buttonSpacing * 2;

    sf::Text titleText("Choose a font:", fontTNR, 24);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(settingsText.getPosition().x, yPos - titleText.getLocalBounds().height - buttonSpacing + 10);

    TNRButton.setSize(sf::Vector2f(250, 25));
    TNRButton.setFillColor(sf::Color(255, 255, 255, 0)); // Transparent white color
    TNRButton.setPosition(settingsText.getPosition().x, yPos);

    TNRText.setString(" - Times New Roman (default font)");
    TNRText.setFont(fontTNR);
    TNRText.setCharacterSize(24);
    TNRText.setPosition(settingsText.getPosition().x, yPos);
    yPos += TNRText.getLocalBounds().height + buttonSpacing;

    robotoButton.setSize(sf::Vector2f(100, robotoText.getLocalBounds().height + robotoText.getLocalBounds().height / 2));
    robotoButton.setFillColor(sf::Color(255, 255, 255, 0));
    robotoButton.setPosition(settingsText.getPosition().x, yPos);

    robotoText.setString(" - Roboto");
    robotoText.setFont(fontRoboto);
    robotoText.setCharacterSize(24);
    robotoText.setPosition(settingsText.getPosition().x, yPos);
    yPos += robotoText.getLocalBounds().height + buttonSpacing;

    HorrorButton.setSize(sf::Vector2f(100, horrorText.getLocalBounds().height + horrorText.getLocalBounds().height / 2));
    HorrorButton.setFillColor(sf::Color(255, 255, 255, 0));
    HorrorButton.setPosition(settingsText.getPosition().x, yPos);

    horrorText.setString(" - Horror");
    horrorText.setFont(fontHorror);
    horrorText.setCharacterSize(24);
    horrorText.setPosition(settingsText.getPosition().x, yPos);
    yPos += horrorText.getLocalBounds().height + buttonSpacing;

    boldButton.setSize(sf::Vector2f(100, boldText.getLocalBounds().height + boldText.getLocalBounds().height / 2));
    boldButton.setFillColor(sf::Color(255, 255, 255, 0));
    boldButton.setPosition(settingsText.getPosition().x, yPos);

    boldText.setString(" - Bold");
    boldText.setFont(fontBold);
    boldText.setCharacterSize(24);
    boldText.setPosition(settingsText.getPosition().x, yPos);
    yPos += boldText.getLocalBounds().height + buttonSpacing;

    sf::Text chooseSizeOfFontText("Choose a size of font:", fontTNR, 24);
    chooseSizeOfFontText.setFillColor(sf::Color::White);
    chooseSizeOfFontText.setPosition(settingsText.getPosition().x, yPos);

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
    currentFontSizeText.setString(std::to_string(game.getFontSize()));
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
    TopicMenuText.setPosition(settingsText.getPosition().x, TopicMenu.getPosition().y);

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
}
void Start::updateChosenFontColor(int selectedFont) {
    TNRText.setFillColor(selectedFont == 0 ? sf::Color::Black : sf::Color::White);
    robotoText.setFillColor(selectedFont == 1 ? sf::Color::Black : sf::Color::White);
    horrorText.setFillColor(selectedFont == 2 ? sf::Color::Black : sf::Color::White);
    boldText.setFillColor(selectedFont == 3 ? sf::Color::Black : sf::Color::White);
}