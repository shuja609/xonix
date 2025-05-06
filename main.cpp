#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <string>
#include <cmath>
using namespace sf;
using namespace std;

const int M = 25;
const int N = 40;
const int ts = 18; // tile size

// Grid representation
int grid[M][N] = {0};

// Game states
enum GameState { MENU, DIFFICULTY_SELECT, PLAYING, GAME_OVER };
GameState gameState = MENU;

struct MotionPattern {
    // This is a function pointer that calculates offsets of x and y based on when the time since the last pattern switch
    void (*calculateMotion)(float time, float speedInitial, float &dx, float &dy);
    const char *patternName; // Name of current pattern for display or to debug
};

// Enemy related function prototypes
void zigzagMotion(float time, float speedInitial, float &dx, float &dy);
void circularMotion(float time, float speedInitial, float &dx, float &dy);

// This array stores the types of enemy motion patterns
const MotionPattern patterns[] = {{zigzagMotion, "Zig-zag"}, {circularMotion, "Circular"}};

const int numOfPatterns = sizeof(patterns) / sizeof(patterns[0]);   // Determine number of different patterns for later use

struct Enemy
{
    float x, y, dx, dy;       // Int was changed to float to enable smoother movement during speed adjustments
    float speedInitial;
    int patternType;              // Decides which pattern to use
    float timeOfPattern;    // Tracks how long it has been since the last patetrn change
    bool patternActive;   // A flag that determines whether the enemy should switch to a pattern
    const MotionPattern* pattern;    // This is a pointer that points to the pattern function

    Enemy()
    {
        x = y = 300;
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;
        
        // We need to store the initial speed's magnitude
        speedInitial = sqrt(dx * dx + dy * dy);
        
        // We adjust the speed of the enemy if they are 'stationary'
        if (dx == 0 && dy == 0) {
          dx = 1;
          speedInitial = 1;
        }
        
        // Variables controlling the pattern of enemy movement need to be initialised here
        patternActive = false;
        patternType = rand() % numOfPatterns;    // This initialises the active pattern to a random one
        timeOfPattern = 0.0f;
        pattern = &patterns[patternType];
    }

    void move(float speedMultiplier)
    {
        float currentdx, currentdy;
        
        if(patternActive) {
        // Apply movement of pattern
          timeOfPattern += 0.016f;    // This approximates to 60 fps (?)
          pattern->calculateMotion(timeOfPattern, speedInitial, currentdx, currentdy);
        
          // The current speed multiplier has to be applied to enemy movement
          currentdx *= speedMultiplier;
          currentdy *= speedMultiplier;
        }
        else {
          // If the flag isnt active, use regular linear movement
          currentdx = speedMultiplier * dx;
          currentdy = speedMultiplier * dy;
        }
        
        // Apply change on the x-axis and check for collisions
        x += currentdx;
        if (grid[int(y / ts)][int(x / ts)] == 1) {
          currentdx = -currentdx;
          dx = -dx;
          x += currentdx;
          
          if(patternActive) {   // If a pattern is in use, bounce off the walls without changing pattern
            timeOfPattern+= 0.25f;    // Small step in pattern time to avoid getting stuck
          }
        }
        
        y += currentdy;
        if (grid[int(y / ts)][int(x / ts)] == 1) {
          currentdy = -currentdy;
          dy = -dy;
          y += currentdy;
          
          if(patternActive) {   // If a pattern is in use, bounce off the walls without changing pattern
            timeOfPattern += 0.25f;    // Small step in pattern time to avoid getting stuck
          }
        }
    }
};

// Function prototypes
void drop(int y, int x);
void initializeGrid();
string formatTime(float timeInSeconds);
void resetElapsedTimer();
void updateElapsedTimer();
void switchEnemyPattern(Enemy enemies[], int enemyCount, float presentTime);

// Global variables
int difficultyLevel = 1; // Default to easy
int enemyCount = 2; // Default to easy (2 enemies)
float elapsedTime = 0.0f;     // Default starting time is set to 0
Clock gameClock;    // A clock object is made to track elapsed game time
bool timerActive = false;  // We need a flag that indicates whether the timer is running or not
// Speed increase variables
const float speedFactor = 0.5f;    // Enemy speed will increase every 20s by a factor of 0.5
const float speedIncreaseInterval = 20.0f;
float timeSinceLastIncrease = 0.0f;
float speedMultiplier = 1.0f;
const float maxSpeedMultiplier = 4.0f;
// Variables for enemy pattern switching
const float patternSwitchInterval = 30.0f;    // The threshold time interval to switch the pattern
bool switchedPattern = false;    // This flag tracks whether the enemy's movement pattern has changed

// Global text declarations
Text timerText;
Text speedText;

/**
 * Recursive flood fill algorithm to mark cells
 */
void drop(int y, int x)
{
    if (grid[y][x] == 0)
        grid[y][x] = -1;
    if (grid[y - 1][x] == 0)
        drop(y - 1, x);
    if (grid[y + 1][x] == 0)
        drop(y + 1, x);
    if (grid[y][x - 1] == 0)
        drop(y, x - 1);
    if (grid[y][x + 1] == 0)
        drop(y, x + 1);
}

/**
 * Initialize the game grid with borders
 */
void initializeGrid()
{
    // Clear grid
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            grid[i][j] = 0;
            // Set borders
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
                grid[i][j] = 1;
        }
    }
}

// Functions controlling elapsed time and display
string formatTime(float timeInSeconds) {
  int mins = int(timeInSeconds) / 60;
  int seconds = int (timeInSeconds) % 60;
  int millisecs = int((timeInSeconds - int(timeInSeconds)) * 10);
  
  // Formatting time elapsed in MM:SS.millisecs form
  string formattedTime = (mins < 10 ? "0" : "") +to_string(mins) + ":" + (seconds < 10 ? "0" : "") + 
                        to_string(seconds) + "." + to_string(millisecs);
                          
  return formattedTime;
}

void resetElapsedTimer() {
  elapsedTime = 0.0f;
  gameClock.restart();    // The restart() method resets the game clock to zero and keeps it running
  timerActive = true;
  
  // Reset speed adjustment variables as well
  timeSinceLastIncrease = 0.0f;
  speedMultiplier = 1.0f;
  
  // Reset the flag that tracks pattern change of enemies
  switchedPattern = false;
  
}

void updateElapsedTimer() {
  if (timerActive) {
    // This updates the elapsed timer since the last frame
    float timeFromLastFrame = gameClock.restart().asSeconds();
    
    elapsedTime += timeFromLastFrame;
    
    // Also update the time since the last increase of speed
    timeSinceLastIncrease += timeFromLastFrame;
    
    // If the time since last speed increase reaches 20, then we have to update enemy speed
    if (timeSinceLastIncrease >= speedIncreaseInterval) {
      timeSinceLastIncrease -= speedIncreaseInterval; // This resets the speed timer
      speedMultiplier += speedFactor;   // This increases the multiplier by which enemy speed increases
    }
    
    // Keeping an upper limit so that speed does not increase infinitely (?) - is this necessary
    if (speedMultiplier > maxSpeedMultiplier) {
      speedMultiplier = maxSpeedMultiplier;
    }
    
    // This displays the updated elapsed time on the display
    timerText.setString("Time: " + formatTime(elapsedTime));
    
    // Shows the current multiplier that controls enemy speed
    speedText.setString("Speed: x" + to_string(int(speedMultiplier * 100) / 100.0).substr(0,4));
  }
}

// Functions that control enemy movement patterns
void zigzagMotion(float time, float speedInitial, float &dx, float &dy) {
  int phase = int((time * 2)) % 2;    // Direction is changed every 0.5s (?)
  
  // The basis of this zigzag pattern is that the y direction of the enemy sprite alternates
  // while x direction is kept the same
  if (phase == 0) {
    dx = speedInitial;
    dy = speedInitial * 0.5f;
  }
  else {
    dx = speedInitial;
    dy = -speedInitial * 0.5f;
  }
}

void circularMotion(float time, float speedInitial, float &dx, float &dy) {
  // We define arrays that with already calculated values around the circumference of a circle
  // These values are approximate values of sin/cos values from 8 different points on the circle's circumference (?)
  const float circumferenceX[8] = {1.0f, 0.7f, 0.0f, -0.7f, -1.0f, -0.7f, 0.0f, 0.7f};
  const float circumferenceY[8] = {0.0f, 0.7f, 1.0f, 0.7f, 0.0f, -0.7f, -1.0f, -0.7f};
  
  // The position around the circle will change at 4 points per second (every 2 seconds)
  int i = int(time * 4) % 8;
  
  // The movement of the enemy is adjusted based on the direction around the circular pattern
  dx = circumferenceX[i] * speedInitial;
  dy = circumferenceY[i] * speedInitial;
}

// This function switches the motion pattern of half the enemy sprites
void switchEnemyPattern(Enemy enemies[], int enemyCount, float presentTime) {
  // The switch must occur every 30s and if the switch hasnt occurred already
  if (presentTime >= patternSwitchInterval && !switchedPattern) {
    // Calculate half the enemy count to switch
    int halfOfEnemies = enemyCount / 2;
    
    // Switch the movement of the calculated half
    for (int i = 0; i < halfOfEnemies; i++) {
      enemies[i].patternActive = true;
      
      // The pattern assigned to each enemy will alternate
      enemies[i].patternType = i % numOfPatterns;
      enemies[i].pattern = &patterns[enemies[i].patternType];
      
      // After switching, reset the timer
      enemies[i].timeOfPattern= 0;
    }
    
    switchedPattern = true;
    
    cout << "Switched" << endl;  // For debugging in console
  }
}

int main()
{
    srand(time(0));

    // Initialize game window
    RenderWindow window(VideoMode(N * ts, M * ts), "Xonix Game!");
    window.setFramerateLimit(60);

    // Load font
    Font gameFont;
    if (!gameFont.loadFromFile("fonts/PressStart2P.ttf")) {
        cout << "Error: Unable to load font from fonts/PressStart2P.ttf" << endl;
        return -1;
    }

    // Load textures
    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");

    // Create sprites
    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);

    // Initialize enemies
    Enemy enemies[10];
    for (int i = 0; i < 10; i++) {
        enemies[i] = Enemy();
    }

    // Game variables
    bool gameRunning = false; // Start in menu mode
    int playerX = 10, playerY = 0;
    int moveX = 0, moveY = 0;
    float timer = 0, delay = 0.07;
    int moveCounter = 0;
    Clock clock;

    // Initialize game grid
    initializeGrid();

    // Main menu text elements
    Text titleText("XONIX", gameFont, 40);
    titleText.setFillColor(Color::White);
    titleText.setPosition(N * ts / 2 - 80, 60);

    Text startGameText("1. START GAME", gameFont, 16);
    startGameText.setFillColor(Color::White);
    startGameText.setPosition(N * ts / 2 - 90, 170);

    Text difficultyText("2. SELECT DIFFICULTY", gameFont, 16);
    difficultyText.setFillColor(Color::White);
    difficultyText.setPosition(N * ts / 2 - 90, 210);

    Text exitText("3. EXIT GAME", gameFont, 16);
    exitText.setFillColor(Color::White);
    exitText.setPosition(N * ts / 2 - 90, 250);

    // Difficulty menu text elements
    Text difficultyTitleText("SELECT DIFFICULTY:", gameFont, 20);
    difficultyTitleText.setFillColor(Color::Yellow);
    difficultyTitleText.setPosition(N * ts / 2 - 120, 120);

    Text easyText("1. EASY (2 ENEMIES)", gameFont, 16);
    easyText.setFillColor(Color::White);
    easyText.setPosition(N * ts / 2 - 90, 170);

    Text mediumText("2. MEDIUM (4 ENEMIES)", gameFont, 16);
    mediumText.setFillColor(Color::White);
    mediumText.setPosition(N * ts / 2 - 90, 210);

    Text hardText("3. HARD (6 ENEMIES)", gameFont, 16);
    hardText.setFillColor(Color::White);
    hardText.setPosition(N * ts / 2 - 90, 250);

    Text backText("4. BACK TO MAIN MENU", gameFont, 16);
    backText.setFillColor(Color::White);
    backText.setPosition(N * ts / 2 - 90, 320);

    // Game over text
    Text restartText("PRESS R TO RESTART", gameFont, 16);
    restartText.setFillColor(Color::White);
    restartText.setPosition(N * ts / 2 - 120, M * ts / 2 + 50);
    
    Text menuText("PRESS ESC FOR MENU", gameFont, 16);
    menuText.setFillColor(Color::White);
    menuText.setPosition(N * ts / 2 - 120, M * ts / 2 + 80);
    
    // Number of moves text
    Text moves("Moves = " + to_string(moveCounter), gameFont, 10);
    moves.setFillColor(Color::White);
    moves.setPosition(10, 8);
    
    // Game elapsed text
    timerText.setString("Timer: " + to_string(elapsedTime));
    timerText.setFont(gameFont);
    timerText.setCharacterSize(10);
    timerText.setFillColor(Color::White);
    timerText.setPosition(10, 23);
    
    // Speed multiplier text
    speedText.setString("Speed: x1.0");
    speedText.setFont(gameFont);
    speedText.setCharacterSize(10);
    speedText.setFillColor(Color::White);
    speedText.setPosition(10, 38);

    // Main game loop
    while (window.isOpen())
    {
        // Handle time
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        // Process events
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
               
            if (e.type == Event::KeyPressed)
            {
                // Use nested switch statements for menu navigation
                switch (gameState)
                {
                    case MENU:
                        switch (e.key.code)
                        {
                            case Keyboard::Num1:
                            case Keyboard::Numpad1:
                                // Start game with current difficulty
                                gameState = PLAYING;
                                gameRunning = true;
                                playerX = 10;
                                playerY = 0;
                                moveX = moveY = 0;
                                moveCounter = 0;
                                resetElapsedTimer();
                                
                                // Reset enemies
                                for (int i = 0; i < enemyCount; i++) {
                                    enemies[i] = Enemy();
                                }
                                
                                // Reset grid
                                initializeGrid();
                                break;
                                
                                switchedPattern = false;
                            
                            case Keyboard::Num2:
                            case Keyboard::Numpad2:
                                // Go to difficulty selection
                                gameState = DIFFICULTY_SELECT;
                                break;
                            
                            case Keyboard::Num3:
                            case Keyboard::Numpad3:
                                // Exit game
                                window.close();
                                break;
                        }
                        break;
                    
                    case DIFFICULTY_SELECT:
                        switch (e.key.code)
                        {
                            case Keyboard::Num1:
                            case Keyboard::Numpad1:
                                // Easy difficulty
                                difficultyLevel = 1;
                                enemyCount = 2;
                                
                                // Start game immediately
                                gameState = PLAYING;
                                gameRunning = true;
                                playerX = 10;
                                playerY = 0;
                                moveX = moveY = 0;
                                moveCounter = 0;
                                resetElapsedTimer();
                                
                                // Reset enemies
                                for (int i = 0; i < enemyCount; i++) {
                                    enemies[i] = Enemy();
                                }
                                
                                // Reset grid
                                initializeGrid();
                                break;
                                
                                switchedPattern = false;
                            
                            case Keyboard::Num2:
                            case Keyboard::Numpad2:
                                // Medium difficulty
                                difficultyLevel = 2;
                                enemyCount = 4;
                                
                                // Start game immediately
                                gameState = PLAYING;
                                gameRunning = true;
                                playerX = 10;
                                playerY = 0;
                                moveX = moveY = 0;
                                moveCounter = 0;
                                resetElapsedTimer();
                                
                                // Reset enemies
                                for (int i = 0; i < enemyCount; i++) {
                                    enemies[i] = Enemy();
                                }
                                
                                // Reset grid
                                initializeGrid();
                                break;
                            
                            case Keyboard::Num3:
                            case Keyboard::Numpad3:
                                // Hard difficulty
                                difficultyLevel = 3;
                                enemyCount = 6;
                                
                                // Start game immediately
                                gameState = PLAYING;
                                gameRunning = true;
                                playerX = 10;
                                playerY = 0;
                                moveX = moveY = 0;
                                moveCounter = 0;
                                resetElapsedTimer();
                                
                                // Reset enemies
                                for (int i = 0; i < enemyCount; i++) {
                                    enemies[i] = Enemy();
                                }
                                
                                // Reset grid
                                initializeGrid();
                                break;
                                
                                switchedPattern = false;
                            
                            case Keyboard::Num4:
                            case Keyboard::Numpad4:
                            case Keyboard::Escape:
                                // Back to main menu
                                gameState = MENU;
                                break;
                        }
                        break;
                    
                    case PLAYING:
                        if (e.key.code == Keyboard::Escape)
                        {
                            gameState = MENU;
                        }
                        break;
                    
                    case GAME_OVER:
                        moveCounter = 0;
                        resetElapsedTimer();
                        if (e.key.code == Keyboard::R)
                        {
                            gameState = PLAYING;
                            gameRunning = true;
                            playerX = 10;
                            playerY = 0;
                            moveX = moveY = 0;
                            
                            
                            initializeGrid();
                            
                            // Reset enemies
                            for (int i = 0; i < enemyCount; i++) {
                                enemies[i] = Enemy();
                            }
                            
                            switchedPattern = false;
                        }
                        else if (e.key.code == Keyboard::Escape)
                        {
                            gameState = MENU;
                        }
                        break;
                }
            }
        }

        // Game logic only runs in PLAYING state
        if (gameState == PLAYING)
        {
            moves.setString("Moves = " + to_string(moveCounter));
            updateElapsedTimer();
            
            // Handle keyboard input for movement
            if (Keyboard::isKeyPressed(Keyboard::Left))  { moveX = -1; moveY = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Right)) { moveX = 1; moveY = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Up))    { moveX = 0; moveY = -1; }
            if (Keyboard::isKeyPressed(Keyboard::Down))  { moveX = 0; moveY = 1; }
            
            if (!gameRunning) 
            {
                timerActive = false;
                gameState = GAME_OVER;
                continue;
            }

            // Update player position
            if (timer > delay)
            {
                // We create variables that store the player's previous to ensure that it has actually moved to count moves
                int previousX = playerX;
                int previousY = playerY;
                
                // Moving the player
                playerX += moveX;
                playerY += moveY;
                
                // Handle boundaries
                if (playerX < 0) playerX = 0;
                if (playerX > N - 1) playerX = N - 1;
                if (playerY < 0) playerY = 0;
                if (playerY > M - 1) playerY = M - 1;
                
                // MOVEMENT TRACKING - Starts when player moves from border to unmarked space 
                // Start by assuming player is on border - needs to be static variable because it shouldnt reset between frames
                static bool prevOnBorder = true;
                
                // Check if player is currently on marked territory
                bool nowOnBorder = (grid[playerY][playerX] == 1);
                
                // If the player was on a marked grid and now on unmarked one, this counts as a move)
                bool moved = (previousX != playerX || previousY != playerY);
                if (moved && prevOnBorder && grid[playerY][playerX] == 0) {
                  moveCounter++;
                  moves.setString("Moves = " + to_string(moveCounter));  // Displays updated number of moves
                }
                
                // The variable that tracks whether a player is on a marked cell should be updated for the next frame
                prevOnBorder = nowOnBorder;

                // Check collisions
                if (grid[playerY][playerX] == 2) 
                    gameRunning = false;
                if (grid[playerY][playerX] == 0) 
                    grid[playerY][playerX] = 2;
                
                timer = 0;
            }

            // Move enemies by the current speed multiplier
            for (int i = 0; i < enemyCount; i++) 
                enemies[i].move(speedMultiplier);

            // Check if player completed a section
            if (grid[playerY][playerX] == 1)
            {
                moveX = moveY = 0;

                // Fill areas
                for (int i = 0; i < enemyCount; i++)
                    drop(enemies[i].y / ts, enemies[i].x / ts);

                // Update grid
                for (int i = 0; i < M; i++)
                    for (int j = 0; j < N; j++)
                        if (grid[i][j] == -1) 
                            grid[i][j] = 0;
                        else 
                            grid[i][j] = 1;
            }

            // Check player-enemy collisions
            for (int i = 0; i < enemyCount; i++)
                if (grid[int(enemies[i].y / ts)][int(enemies[i].x / ts)] == 2) 
                    gameRunning = false;
        }

        // Draw everything
        window.clear(Color(0, 0, 50)); // Dark blue background

        switch (gameState)
        {
            case MENU:
                // Draw main menu
                window.draw(titleText);
                window.draw(startGameText);
                window.draw(difficultyText);
                window.draw(exitText);
                break;
            
            case DIFFICULTY_SELECT:
                // Draw difficulty menu
                window.draw(titleText);
                window.draw(difficultyTitleText);
                window.draw(easyText);
                window.draw(mediumText);
                window.draw(hardText);
                window.draw(backText);
                break;
            
            case PLAYING:
                
            case GAME_OVER:
                
                // Draw grid
                for (int i = 0; i < M; i++)
                {
                    for (int j = 0; j < N; j++)
                    {
                        if (grid[i][j] == 0) continue;
                        if (grid[i][j] == 1) sTile.setTextureRect(IntRect(0, 0, ts, ts));
                        if (grid[i][j] == 2) sTile.setTextureRect(IntRect(54, 0, ts, ts));
                        sTile.setPosition(j * ts, i * ts);
                        window.draw(sTile);
                    }
                }

                // Draw player
                sTile.setTextureRect(IntRect(36, 0, ts, ts));
                sTile.setPosition(playerX * ts, playerY * ts);
                window.draw(sTile);
                window.draw(moves);
                window.draw(timerText);
                window.draw(speedText);

                // Draw enemies
                // Apply different colours to the trails of enemies on different patterns for better discernability
                int rotationIndex;   // Apply different rotation speeds for each pattern
                for (int i = 0; i < enemyCount; i++)
                {
                    sEnemy.setPosition(enemies[i].x, enemies[i].y);
                    
                    // Applying different rotation and colours to different patterns of movmement
                    if (enemies[i].patternActive) {
                      if (enemies[i].patternType == 0) {   // Pattern 0 refers to zigzag
                        sEnemy.rotate(10);
                        sEnemy.setColor(Color::Magenta);
                      }
                      else {// referring to circular pattern
                        sEnemy.rotate(15);
                        sEnemy.setColor(Color::Cyan);
                      }
                    }
                    else {
                      sEnemy.rotate(5);
                    }
                    window.draw(sEnemy);
                }

                // Draw game over screen
                if (gameState == GAME_OVER)
                {
                    window.draw(sGameover);
                    window.draw(restartText);
                    window.draw(menuText);
                }
                break;
        }

        // Display the window
        window.display();
    }

    return 0;
}