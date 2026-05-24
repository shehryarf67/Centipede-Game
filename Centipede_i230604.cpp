#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cstdlib>

using namespace std;

// Initializing Dimensions.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;
const int gameColumns = resolutionY / boxPixelsY;

const int x = 0;
const int y = 1;
const int exists = 2;

void drawCentipede(sf::RenderWindow &window, int &score, float centipede[][6], sf::Sprite &centipedeBodySprite, sf::Sprite &centipedeHeadSprite, sf::Sprite &centipedeRightSprite, sf::Sprite playerSprite, sf::Texture playerTexture, float player[2], float bullet[], float mushroom[][4], int& numberMushrooms);
void moveCentipede(float centipede[][6], sf::Clock &centipedeClock, const int movingLeft, int numCentipedeParts, int resolutionX, int resolutionY, int boxPixelsX, int boxPixelsY, int numberMushrooms, float mushroom[][4]);
void drawPlayer(sf::RenderWindow &window, float player[], sf::Sprite &playerSprite);
void movePlayer(float player[], sf::Clock &playerClock);
void moveBullet(float bullet[], sf::Clock &bulletClock);
void generateNewCentipedeHead(float centipede[][6], float centipedeHeads[][6], int& numCentipedeParts, int& numCentipedeHeads, const int maxNumCentipedeHeads, int resolutionX, int resolutionY, int boxPixelsX, int boxPixelsY);
void drawBullet(sf::RenderWindow &window, float bullet[], sf::Sprite &bulletSprite);
void handleNewCentipedeHeadGeneration(float centipede[][6], int& numCentipedeParts, int resolutionX, int resolutionY, int boxPixelsX, int boxPixelsY);
void drawMushrooms(sf::RenderWindow &window, int &score, float mushroom[][4], sf::Sprite &mushroomSprite, sf::Sprite &mushroomSprite2, sf::Sprite &poisMushroomSprite, sf::Sprite &poisMushroomSprite2, float bullet[], int numberMushrooms);

int main()
{
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);
    window.setSize(sf::Vector2u(1000, 1000));
    window.setPosition(sf::Vector2i(100, 0));

    sf::Music bgMusic;
    bgMusic.openFromFile("Music/field_of_hopes.ogg");
    bgMusic.play();
    bgMusic.setVolume(50);

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    backgroundTexture.loadFromFile("Textures/background.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.20));

    float player[2] = {};
    player[x] = (gameColumns / 2) * boxPixelsX;
    player[y] = resolutionY - (5 * boxPixelsY);
    sf::Clock playerClock;
    sf::Texture playerTexture;
    sf::Sprite playerSprite;
    playerTexture.loadFromFile("Textures/player.png");
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    int numCentipedeParts = 12; //Centipede Initialization 
    const int maxNumCentipedeHeads = 20; 
    int numCentipedeHeads = 0;
    float centipede[50][6] = {};
    float centipedeHeads[maxNumCentipedeHeads][6];
    int headIndex = numCentipedeParts - 1;
    const int movingLeft = 1;
    const int movingDown = 1;
    const int exists1 = 1;

    centipede[headIndex][x] = rand() % (resolutionX - (12 * boxPixelsX)); //Generate the head of the centipede on the top row
    centipede[headIndex][y] = 0;
    centipede[headIndex][2] = movingLeft;
    centipede[headIndex][3] = movingDown;
    centipede[headIndex][4] = exists1;
    centipede[headIndex][5] = 1;
    
    if (centipede[headIndex][x] < boxPixelsX) //Make sure the head is not too close to the edges
        centipede[headIndex][x] = boxPixelsX;
    if (centipede[headIndex][x] > resolutionX - boxPixelsX)
        centipede[headIndex][x] = resolutionX - boxPixelsX;

    for (int i = headIndex - 1; i >= 0; i--) //Generate body segments on the top row in a coherent way
    {
        centipede[i][x] = centipede[i + 1][x] + boxPixelsX; //Generate the body next to the previous body segment
        centipede[i][y] = 0;
        centipede[i][2] = movingLeft;
        centipede[i][3] = movingDown;
        centipede[i][4] = exists1;
        centipede[i][5] = 0;

        if (centipede[i][x] < boxPixelsX) //Make sure body segment is not too close to the edges
            centipede[i][x] = boxPixelsX;
        if (centipede[i][x] > resolutionX - boxPixelsX)
            centipede[i][x] = resolutionX - boxPixelsX;
    }

    sf::Clock centipedeClock;
    sf::Texture centipedeBodyTexture;
    sf::Sprite centipedeBodySprite;
    centipedeBodyTexture.loadFromFile("Textures/c_body_left_walk.png");
    centipedeBodySprite.setTexture(centipedeBodyTexture);
    centipedeBodySprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    sf::Texture centipedeHeadTexture;
    sf::Sprite centipedeHeadSprite;
    centipedeHeadTexture.loadFromFile("Textures/c_head_left_walk.png");
    centipedeHeadSprite.setTexture(centipedeHeadTexture);
    centipedeHeadSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    sf::Texture centipedeRightTexture;
    sf::Sprite centipedeRightSprite;
    centipedeRightTexture.loadFromFile("Textures/right_head.png");
    centipedeRightSprite.setTexture(centipedeRightTexture);
    centipedeRightSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    float mushroom[50][4];
    const int hitCount = 2;
    int numberMushrooms = rand() % 11 + 20; //Generate between 20 and 30 mushrooms

    for (int i = 0; i < numberMushrooms; i++) //Generate mushrooms at random positions, excludint topmost row and player area
    {
        bool overlap = true;
        while (overlap)
        {

            mushroom[i][x] = rand() % (gameColumns - 2) * boxPixelsX + boxPixelsX; 
            mushroom[i][y] = rand() % (gameRows - 6) * boxPixelsY + boxPixelsY;
            mushroom[i][2] = 0;
            mushroom[i][2] = 0;
            mushroom[i][3] = 0;

            overlap = false;
            for (int j = 0; j < i; j++) //Check and prevent overlap with other mushrooms
            {
                if (mushroom[i][x] < mushroom[j][x] + boxPixelsX &&
                    mushroom[i][x] + boxPixelsX > mushroom[j][x] &&
                    mushroom[i][y] < mushroom[j][y] + boxPixelsY &&
                    mushroom[i][y] + boxPixelsY > mushroom[j][y])
                {
                    overlap = true;
                    break; 
                }
            }
        }
    }

    sf::Texture mushroomTexture;
    sf::Sprite mushroomSprite;
    mushroomTexture.loadFromFile("Textures/mushroom.png");
    mushroomSprite.setTexture(mushroomTexture);
    mushroomSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    sf::Texture mushroomTexture2;
    sf::Sprite mushroomSprite2;
    mushroomTexture2.loadFromFile("Textures/mushroom.png");
    mushroomSprite2.setTexture(mushroomTexture2);
    mushroomSprite2.setTextureRect(sf::IntRect(3 * boxPixelsX, 0, boxPixelsX, boxPixelsY));

    sf::Texture poisMushroomTexture;
    sf::Sprite poisMushroomSprite;
    poisMushroomTexture.loadFromFile("Textures/mushroom.png");
    poisMushroomSprite.setTexture(poisMushroomTexture);
    poisMushroomSprite.setTextureRect(sf::IntRect(0, boxPixelsY, boxPixelsX, boxPixelsY));

    sf::Texture poisMushroomTexture2;
    sf::Sprite poisMushroomSprite2;
    poisMushroomTexture2.loadFromFile("Textures/mushroom.png");
    poisMushroomSprite2.setTexture(poisMushroomTexture2);
    poisMushroomSprite2.setTextureRect(sf::IntRect(3 * boxPixelsX, boxPixelsY, boxPixelsX, boxPixelsY));

    float bullet[3] = {};
    bullet[x] = player[x];
    bullet[y] = player[y] - boxPixelsY;
    bullet[exists] = false;
    sf::Clock bulletClock;
    sf::Texture bulletTexture;
    sf::Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    sf::Font font;
    font.loadFromFile("Textures/fontcenti.ttf"); //Font selection for score display
    int score = 0;

    sf::SoundBuffer bulletBuffer;
    bulletBuffer.loadFromFile("Sound Effects/fire1.wav");
    sf::Sound bulletSound;
    bulletSound.setBuffer(bulletBuffer);
    bool bulletSoundPlayed = false;
    
    while (window.isOpen())
    {
        window.draw(backgroundSprite);
        drawCentipede(window, score, centipede, centipedeBodySprite, centipedeHeadSprite, centipedeRightSprite, playerSprite, playerTexture, player, bullet, mushroom, numberMushrooms);
        drawCentipede(window, score, centipedeHeads, centipedeBodySprite, centipedeHeadSprite, centipedeRightSprite, playerSprite, playerTexture, player, bullet, mushroom, numberMushrooms);
        generateNewCentipedeHead(centipede, centipedeHeads, numCentipedeParts, numCentipedeHeads, maxNumCentipedeHeads, resolutionX, resolutionY, boxPixelsX, boxPixelsY);
        drawMushrooms(window, score, mushroom, mushroomSprite, mushroomSprite2, poisMushroomSprite, poisMushroomSprite2, bullet, numberMushrooms);
        moveCentipede(centipede, centipedeClock, movingLeft, numCentipedeParts, resolutionX, resolutionY, boxPixelsX, boxPixelsY, numberMushrooms, mushroom);
        moveCentipede(centipedeHeads, centipedeClock, movingLeft, numCentipedeHeads, resolutionX, resolutionY, boxPixelsX, boxPixelsY, numberMushrooms, mushroom);
        drawPlayer(window, player, playerSprite);

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(32);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10, 10); 

        window.draw(scoreText);

        for (int i = 0; i < numCentipedeParts; i++)
        {
            if ((centipede[i][4] == 1) && player[x] < centipede[i][x] + boxPixelsX &&
                player[x] + boxPixelsX > centipede[i][x] &&
                player[y] < centipede[i][y] + boxPixelsY &&
                player[y] + boxPixelsY > centipede[i][y])
            {
                window.close();  //End game on collision
            }
        }
        for (int i = 0; i < numCentipedeHeads; i++)
        {
            if ((centipedeHeads[i][4] == 1) && player[x] < centipedeHeads[i][x] + boxPixelsX &&
                player[x] + boxPixelsX > centipedeHeads[i][x] &&
                player[y] < centipedeHeads[i][y] + boxPixelsY &&
                player[y] + boxPixelsY > centipedeHeads[i][y])
            {
                window.close();
            }
        }
        
        for (int i = 0; i < numberMushrooms; i++)
        {
            if (mushroom[i][2] < hitCount)
            {
                if (player[x] < mushroom[i][x] + boxPixelsX &&
                    player[x] + boxPixelsX > mushroom[i][x] &&
                    player[y] < mushroom[i][y] + boxPixelsY &&
                    player[y] + boxPixelsY > mushroom[i][y])
                {
                    if (mushroom[i][3] == 1)
                    {
                        window.close(); //End game when player collides with poisonous mushroom
                    }
                }
            }
        }

        if (bullet[exists] == true)
        {
            moveBullet(bullet, bulletClock);
            drawBullet(window, bullet, bulletSprite);
        }

        movePlayer(player, playerClock);

        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                return 0;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
            {
                if (bullet[exists] == false)
                {
                    bullet[exists] = true;
                    bullet[x] = player[x];
                    bullet[y] = player[y] - 32;

                    if (!bulletSoundPlayed)
                    {
                        bulletSound.play();
                        bulletSoundPlayed = true; //Play sound once 
                    }
                    bulletSoundPlayed = false;
                }
            }
            
        }

        window.display();
        window.clear();
    }
}

void drawMushrooms(sf::RenderWindow &window, int &score, float mushroom[][4], sf::Sprite &mushroomSprite, sf::Sprite &mushroomSprite2, sf::Sprite &poisMushroomSprite, sf::Sprite &poisMushroomSprite2, float bullet[], int numberMushrooms) //Draws the mushrooms
{
    const int hitCount = 2;

    for (int i = 0; i < numberMushrooms; i++)
    {
        if (mushroom[i][2] < hitCount)
        {
            if (mushroom[i][2] == 0 && mushroom[i][3] == 0) //Draw full mushroom at no hit
            {
                mushroomSprite.setPosition(mushroom[i][x], mushroom[i][y]);
                window.draw(mushroomSprite);
            }
            else if (mushroom[i][2] == 1 && mushroom[i][3] == 0) //Draw half mushroom at 1 hit
            {
                mushroomSprite2.setPosition(mushroom[i][x], mushroom[i][y]);
                window.draw(mushroomSprite2);
            }
            else if (mushroom[i][2] == 0 && mushroom[i][3] == 1) //Same logic for poisonous mushroom
            {
                poisMushroomSprite.setPosition(mushroom[i][x], mushroom[i][y]);
                window.draw(poisMushroomSprite);
            }
            else if (mushroom[i][2] == 1 && mushroom[i][3] == 1)
            {
                poisMushroomSprite2.setPosition(mushroom[i][x], mushroom[i][y]);
                window.draw(poisMushroomSprite2);
            }

            if (bullet[exists]) //Detect collision with mushroom and bullet
            {
                if (mushroom[i][2] < hitCount &&
                    bullet[x] + 20 > mushroom[i][x] &&
                    bullet[x] < mushroom[i][x] + 20 &&
                    bullet[y] + 32 > mushroom[i][y] &&
                    bullet[y] < mushroom[i][y] + 32)
                {
                    mushroom[i][2]++;

                    if (mushroom[i][2] >= hitCount)
                    {
                        mushroom[i][2] = hitCount;
                        score++; //Increment score
                    }

                    bullet[exists] = false; //Remove bullet from screen
                }
            }
        }
    }
}


void drawCentipede(sf::RenderWindow &window, int &score, float centipede[][6], sf::Sprite &centipedeBodySprite, sf::Sprite &centipedeHeadSprite, sf::Sprite &centipedeRightSprite, sf::Sprite playerSprite, sf::Texture playerTexture, float player[2], float bullet[], float mushroom[][4], int& numberMushrooms) //Displaying the centipede
{
    const int numCentipedeParts = 12;
    int headIndex = numCentipedeParts - 1;

    for (int i = numCentipedeParts - 1; i >= 0; i--)
    {
        if (centipede[i][4] == 1) //Draw if exists
        {
            if (centipede[i][5] == 1) //Draw head
            {
                if (centipede[i][2] == 0) 
                {
                    centipedeRightSprite.setPosition(centipede[i][x], centipede[i][y]);
                    window.draw(centipedeRightSprite);
                }
                else
                {
                    centipedeHeadSprite.setPosition(centipede[i][x], centipede[i][y]);
                    window.draw(centipedeHeadSprite);
                }
            }
            else
            {
                centipedeBodySprite.setPosition(centipede[i][x], centipede[i][y]);
                window.draw(centipedeBodySprite);
            }
        }
        if (bullet[exists]) //Detect hit with head
        {
            if (centipede[i][4] == 1 && bullet[x] + 32 > centipede[i][x] &&
                bullet[x] < centipede[i][x] + 32 &&
                bullet[y] + 32 > centipede[i][y] &&
                bullet[y] < centipede[i][y] + 32)
            {
                int segmentCount = i;
                if (centipede[i][5] == 1)
                {         
                    playerSprite.setTexture(playerTexture);
                    playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
                    playerSprite.setPosition(player[x], player[y]);

                    score += 20;
                    
                    do //Removing all previous segments
                    {
                        centipede[segmentCount][4] = 0;
                        segmentCount--;
                    }
                    while (centipede[segmentCount][4] != 0);
                }

                centipede[i][4] = 0; //Turning exist to false
                bullet[exists] = false;

                if (centipede[i][5] != 1) //Checking collision with body
                {
                    score += 10;
                    if (i - 1 >= 0 || i == 0) 
                    {
                        if (centipede[i][y] >= resolutionY - (5 * boxPixelsY))
                        {
                            centipede[i - 1][5] = 1;
                            numberMushrooms++;
                            mushroom[numberMushrooms - 1][x] = centipede[i][x]; //Generating poisonous mushroom on shot index
                            mushroom[numberMushrooms - 1][y] = centipede[i][y];
                            mushroom[numberMushrooms - 1][2] = 0;
                            mushroom[numberMushrooms - 1][3] = 1;
                        }
                        else
                        {
                            centipede[i - 1][5] = 1;
                            numberMushrooms++;
                            mushroom[numberMushrooms - 1][x] = centipede[i][x]; //Generating normal mushroom on shot index
                            mushroom[numberMushrooms - 1][y] = centipede[i][y];
                            mushroom[numberMushrooms - 1][2] = 0;
                            mushroom[numberMushrooms - 1][3] = 0;
                        }
                    }
                }
            }
        }
    }
}

void collisionMushroom(float centipede[][6], float mushroom[][4], int numCentipedeParts, int numberMushrooms) //Detect centipede collision with mushroom 
{
    for (int i = numCentipedeParts - 1; i >= 0; i--)
    {
        for (int j = 0; j < numberMushrooms; j++)
        {
            if (mushroom[j][2] < 2) //Check if mushroom exists 
            {
                if (centipede[i][3] == 1) //Check if moving down
                {
                    if (centipede[i][2] == 1) //Check if moving left
                    {
                        if (centipede[i][x] > mushroom[j][x] && //Checking collision 
                            centipede[i][x] < mushroom[j][x] + boxPixelsX &&
                            centipede[i][y] > mushroom[j][y] - 16 &&
                            centipede[i][y] < mushroom[j][y] + 16)
                        {
                            centipede[i][x] = mushroom[j][x] + boxPixelsX;
                            centipede[i][y] += boxPixelsY;
                            centipede[i][2] = 0;
                        }
                    }
                    else
                    {
                        if (centipede[i][x] + boxPixelsX > mushroom[j][x] &&
                            centipede[i][x] + boxPixelsX < mushroom[j][x] + boxPixelsX &&
                            centipede[i][y] > mushroom[j][y] - 16 &&
                            centipede[i][y] < mushroom[j][y] + 16)
                        {
                            centipede[i][x] = mushroom[j][x] - boxPixelsX;
                            centipede[i][y] += boxPixelsY;
                            centipede[i][2] = 1;
                        }
                    }
                }
                else
                {
                    if (centipede[i][2] == 1)
                    {
                        if (centipede[i][x] > mushroom[j][x] &&
                            centipede[i][x] < mushroom[j][x] + boxPixelsX &&
                            centipede[i][y] > mushroom[j][y] - 16 &&
                            centipede[i][y] < mushroom[j][y] + 16)
                        {
                            centipede[i][x] = mushroom[j][x] + boxPixelsX;
                            centipede[i][y] -= boxPixelsY;
                            centipede[i][2] = 0;
                        }
                    }
                    else
                    {
                        if (centipede[i][x] + boxPixelsX > mushroom[j][x] &&
                            centipede[i][x] + boxPixelsX < mushroom[j][x] + boxPixelsX &&
                            centipede[i][y] > mushroom[j][y] - 16 &&
                            centipede[i][y] < mushroom[j][y] + 16)
                        {
                            centipede[i][x] = mushroom[j][x] - boxPixelsX;
                            centipede[i][y] -= boxPixelsY;
                            centipede[i][2] = 1;
                        }
                    }
                }
            }
        }
    }
}

void moveCentipede(float centipede[][6], sf::Clock &centipedeClock, const int movingLeft, int numCentipedeParts, int resolutionX, int resolutionY, int boxPixelsX, int boxPixelsY, int numberMushrooms, float mushroom[][4]) //Moving centipede
{
    const float speedX = 2.5;
    const float speedY = 32.0; 

    collisionMushroom(centipede, mushroom, numCentipedeParts, numberMushrooms); //Call the function to detect collision 
    
    for (int i = numCentipedeParts - 1; i >= 0; i--)
    {
        if (centipede[i][3] == 1)
        {
            if (centipede[i][2] == movingLeft)
            {   
                centipede[i][x] -= speedX; //Decrement x values to move left
                if (centipede[i][x] < 0)
                {
                    centipede[i][x] = 0; //Adjust the position to stay inside the screen
                    centipede[i][y] += speedY;
                    centipede[i][2] = 0;
                }
            }
            else
            {   
                centipede[i][x] += speedX;
                if (centipede[i][x] > resolutionX - 32)
                {
                    centipede[i][x] = resolutionX - 32; //Adjust the position to stay inside the screen
                    centipede[i][y] += speedY;
                    centipede[i][2] = 1;
                }
            }
        }
        else
        {
            if (centipede[i][2] == movingLeft)
            {   
                centipede[i][x] -= speedX;
                if (centipede[i][x] < 0)
                {
                    centipede[i][x] = 0; 
                    centipede[i][y] -= speedY;
                    centipede[i][2] = 0;
                }
            }
            else
            {   
                centipede[i][x] += speedX;
                if (centipede[i][x] > resolutionX - 32)
                {
                    centipede[i][x] = resolutionX - 32; 
                    centipede[i][y] -= speedY;
                    centipede[i][2] = 1;
                }
            }
            if (centipede[i][y] == resolutionY - (5 * 32)) //Restrict in player area 
            {
                centipede[i][3] = 1;
            }
        }
        if (centipede[i][y] == resolutionY - (2 * 32)) //Prevent the centipede from moving to the last row
        centipede[i][3] = 0;
    }
}

void generateNewCentipedeHead(float centipede[][6], float centipedeHeads[][6], int& numCentipedeParts, int& numCentipedeHeads, const int maxNumCentipedeHeads, int resolutionX, int resolutionY, int boxPixelsX, int boxPixelsY) //Periodically generate a new centipede head
{
    const int movingLeft = 1;

    static sf::Clock headGenerationClock;
    if (headGenerationClock.getElapsedTime().asSeconds() > 10.0) //Generate a head every 10 seconds 
    {
        for (int i = 0; i < numCentipedeParts; ++i) //Check if any centipede enters the player area 
        {
            if (centipede[i][y] >= resolutionY - (5 * boxPixelsY) && centipede[i][4] == 1) //Also checks if centipede exists 
            {
                if (numCentipedeHeads < maxNumCentipedeHeads)
                {
                    centipedeHeads[numCentipedeHeads][x] = resolutionX - boxPixelsX;  //Add a new centipede head at the right side of the screen
                    centipedeHeads[numCentipedeHeads][y] = resolutionY - (5 * boxPixelsY);
                    centipedeHeads[numCentipedeHeads][2] = movingLeft;
                    centipedeHeads[numCentipedeHeads][3] = 0;
                    centipedeHeads[numCentipedeHeads][4] = 1;
                    centipedeHeads[numCentipedeHeads][5] = 1;  

                    numCentipedeHeads++;
                    break; 
                }
            }
        }

        headGenerationClock.restart();
    }
}


void movePlayer(float player[], sf::Clock &playerClock) //Moves player according to keys pressed
{
    const float speed = 15.0;

    if (playerClock.getElapsedTime().asMilliseconds() < 20)
        return;

    playerClock.restart();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player[y] > resolutionY - (5 * boxPixelsY))
    {
        player[y] -= speed;
        if (player[y] < (gameColumns * 3 / 4) * boxPixelsY)
            player[y] = (gameColumns * 3 / 4) * boxPixelsY;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player[x] >= 0)
    {
        player[x] -= speed;
        if (player[x] < 0)
            player[x] = 0;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player[y] < resolutionY - 32)
    {
        player[y] += speed;
        if (player[y] > resolutionY - 32)
            player[y] = resolutionY - 32;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player[x] < resolutionX - 32)
    {
        player[x] += speed;
        if (player[x] > resolutionX - 32)
            player[x] = resolutionX - 32;
    }
}

void drawPlayer(sf::RenderWindow &window, float player[], sf::Sprite &playerSprite) //Displays player 
{
    playerSprite.setPosition(player[x], player[y]);
    window.draw(playerSprite);
}

void moveBullet(float bullet[], sf::Clock &bulletClock) //Moves bullet
{
    const int hitCount = 2;

    if (bulletClock.getElapsedTime().asMilliseconds() < 20)
        return;

    bulletClock.restart();

    if (bullet[exists])
    {
        bullet[y] -= 15;

        if (bullet[y] < -32)
        {
            bullet[exists] = false;
        }
    }
}

void drawBullet(sf::RenderWindow &window, float bullet[], sf::Sprite &bulletSprite)
{
    bulletSprite.setPosition(bullet[x], bullet[y]);
    window.draw(bulletSprite);
}
