#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include<vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Game
{
private:



    sf::RenderWindow* window;
    sf::VideoMode videomode;
    sf::Event ev;

    //mouse posi
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;



    //resources
    sf::Font font;
    sf::Texture worldBackgroundTex;
    sf::Sprite worldBackground;



    //Text
    sf::Text uiText;

    //game logic
    bool endGame;
    unsigned points;
    int health;
    float enemySpawnTimer;
    float enemySpawnTimerMax;
    int maxEnemies;
    bool mouseHeld;

    //game object
    std::vector<sf::CircleShape> enemies;
    sf::CircleShape enemy;





    //private fucntions
    void initVariables();
    void initWindow();
    void initworld();
    void initFonts();
    void initText();
    void initEnemies();

public:
    //Constructor -- Destructor
    Game();
    virtual ~Game();

    //accessors
    const bool running() const;
    const bool getEndGame() const;



    //Functions
    void pollEvents();
    void updateMousePositions();
    void updateText();
    void update();
    void renderText(sf::RenderTarget& target);
    void renderWorld();
    void render();
    void spawnEnemy();
    void updateEnemies();
    void renderEnemies(sf::RenderTarget& target);


};





void Game::initVariables()
{
    endGame = false;
    points = 0;
    health = 10;
    enemySpawnTimerMax = 15.f;
    enemySpawnTimer = enemySpawnTimerMax;
    maxEnemies = 3;
    mouseHeld = false;
}

void Game::initWindow()
{
    this->videomode.height = 600;
    this->videomode.width = 600;
    this->window = new sf::RenderWindow(this->videomode, "Game Snow", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(60);
}



void Game::initworld()
{
    if (!worldBackgroundTex.loadFromFile("Fonts/background.jpg"))
    {
        //error
    }

    worldBackground.setTexture(worldBackgroundTex);


}

void Game::initFonts()
{

    if (font.loadFromFile("Fonts/Dosis-Light.ttf"))
    {
        // error...
    }

}




void Game::initText()
{

    uiText.setFont(font);
    uiText.setCharacterSize(24);
    uiText.setFillColor(sf::Color::White);
    uiText.setString("RENDER");
}

void Game::initEnemies()
{
    enemy.setPosition(10.f, 10.f);
    enemy.setRadius(10.f);
    enemy.setFillColor(sf::Color::Cyan);

}
Game::Game() //Void mischein
{
    initVariables();
    initWindow();
    initFonts();
    initworld();
    initText();
    initEnemies();

}

Game::~Game()
{
    delete this->window;

}

//accessors
const bool Game::running() const
{
     return window->isOpen();
}

const bool Game::getEndGame() const
{
    return endGame;
}
//Functions

void Game::spawnEnemy()
{

    enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(window->getSize().x - this->enemy.getPosition().x)),
        0.f

    );

    //random enemy type
    int type = rand() % 5;
    switch (type)
    {
    case 0:
       enemy.setRadius(13.f);
       enemy.setFillColor(sf::Color::Magenta);
        break;
    case 1:
        enemy.setRadius(16.f);
        enemy.setFillColor(sf::Color::Blue);
        break;
    case 2:
        enemy.setRadius(20.f);
        enemy.setFillColor(sf::Color::Cyan);
        break;
    case 3:
        enemy.setRadius(25.f);
       enemy.setFillColor(sf::Color::Green);
        break;
    default:
        enemy.setRadius(30.f);
       enemy.setFillColor(sf::Color::Yellow);
        break;
    }


    //spawn enemy
    this->enemies.push_back(this->enemy);

    //remove enemies at end of screen

}

void Game::updateEnemies()
{
    //updating timer enemy spawning
    if (this->enemies.size() < this->maxEnemies)
    {
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
        {
            //spawn the enemy and reset timer
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        }
        else
            this->enemySpawnTimer += 1.f;
    }

    //move and update enemies

    for (int i = 0; i < this->enemies.size(); i++)
    {
        bool deleted = false;
        this->enemies[i].move(0.f, 5.f);
        if (this->enemies[i].getPosition().y > this->window->getSize().y)
        {
            this->enemies.erase(this->enemies.begin() + i);
            this->health -= 1;
            std::cout << "health:" << this->health << "\n";
        }
    }


    //check if clicked

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (this->mouseHeld == false)
        {
            this->mouseHeld = true;
            bool deleted = false;
            for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
            {
                if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
                {

                    //gain points
                    if (enemies[i].getFillColor() == sf::Color::Magenta)
                        points += 10;

                    else if (enemies[i].getFillColor() == sf::Color::Blue)
                        points += 7;

                    else if (enemies[i].getFillColor() == sf::Color::Cyan)
                        this->points += 5;


                    else if (this->enemies[i].getFillColor() == sf::Color::Green)
                        this->points += 3;


                    else if (this->enemies[i].getFillColor() == sf::Color::Yellow)
                        this->points += 1;



                    points += 1;
                    std::cout << "points:" << this->points << "\n";

                    //delete enemy
                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);
                }
            }
        }
    }

    else
    {
        this->mouseHeld = false;
    }

}



void Game::renderEnemies(sf::RenderTarget& target)
{

    //render enemies
    for (auto& e : this->enemies)
    {
        target.draw(e);
    }
}


void Game::pollEvents()
{

    //Event Polling
    while (this->window->pollEvent(this->ev)) {

        switch (this->ev.type) {

        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (ev.key.code == sf::Keyboard::Escape)
                this->window->close();
        }
    }
}

void Game::updateMousePositions()
{
    //return void 

    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);

}

void Game::updateText()
{
    std::stringstream ss;

    ss << "Points:" << this->points << "\n"
        << "Health:" << this->health << "\n";
    this->uiText.setString(ss.str());


}

void Game::update()
{

    this->pollEvents();

    if (this->endGame == false)
    {
        this->updateMousePositions();
        this->updateText();
        this->updateEnemies();

    }
    //endgame condition
    if (this->health <= 0)
        this->endGame = true;
}

void Game::renderText(sf::RenderTarget& target)
{

    target.draw(this->uiText);

}

void Game::renderWorld()
{
    this->window->draw(this->worldBackground);
}


void Game::render()
{

    //render game objects
    this->window->clear();
    this->renderWorld();


    //draw game objects
    this->renderEnemies(*this->window);

    this->renderText(*this->window);

    this->window->display();
}

int main()
{

    
    std::srand(static_cast<unsigned>(time(NULL)));



    //init game engine
    Game game;




    //Game Loop
    while (game.running() && !game.getEndGame())
    {


        //Update
        game.update();


        //Render
        game.render();
    }


    return 0;
    

    
}