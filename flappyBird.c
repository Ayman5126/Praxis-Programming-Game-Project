//Flappy Bird

//Headers and Variables being declared
#include <string.h>                                 
#include <stdlib.h>                                 
#include <windows.h>                                
#include <time.h>                                   
#include <stdio.h>                                  

#define screenWidth  32                              // Game board width
#define screenHieght 16                              // Game board height
#define pipeCount    4                               // Number of pipes
#define cKey         0x43                            // Windows virtual key code for the letter 'c'

// colors variable declaration
#define Red         "\e[31m"                        
#define Green       "\e[32m"                        
#define yellow      "\e[33m"                        
#define blue        "\e[34m"                        
#define magenta     "\e[35m"                        
#define white       "\e[37m"                        
#define noColor     "\e[0m"                           // No color (reset)

// creating coordinates
typedef struct {
    int x;
    int y;
} COORDS;

COORDS bird;
COORDS pipes[4];

//variables
int score = 0;     
int customSpeed;
int customDistance;
int choice;
char customColor[10];
int gameSpeed = 100;                                 // Default game speed in milliseconds
int pipeDistance = 15;                               // Default distance between pipes
char pipeColorCode[] = Green;                        // Default pipe color: green
char birdColorCode[] = yellow;                       // Default bird color: yellow

// Function declarations
void drawBoard();                                         // draws the game board
void Pipes();                                    
void collisionCheck();                                      // test for collisions
void ShowMenu();                                  
void ShowEndScreen();                             



//function to set difficulty
void setDifficulty() {
    int difficultyChoice;
    printf("\nSelect Difficulty: \n" 
        "  1. Easy (200ms, 20 blocks) \n "
        " 2. Medium (150ms, 15 blocks) \n "
        " 3. Hard (100ms, 10 blocks) \n "
        " 4. Custom \n "
        "Enter choice: ");
    scanf("%d", &difficultyChoice);

    switch (difficultyChoice) {                          //depending on user's input in chooses between the cases which chooses the difficulty
        case 1: gameSpeed = 200; pipeDistance = 20; break;
        case 2: gameSpeed = 150; pipeDistance = 15; break;
        case 3: gameSpeed = 100; pipeDistance = 10; break;
        case 4: {

            printf("Enter custom game speed (ms): ");
            scanf("%d", &customSpeed);
            printf("Enter custom pipe distance: ");
            scanf("%d", &customDistance);
            gameSpeed = (customSpeed > 0) ? customSpeed : 100;
            pipeDistance = (customDistance > 0) ? customDistance : 15;
            break;
        }
        default: printf("Invalid choice. Using default settings.\n");
    }
}

//functiom to set pipe color
void selectPipeColor() { 
    int colorChoice;
    printf("\nSelect Pipe Color: \n "
        " 1. Green \n "
        " 2. Red \n "
        " 3. Blue \n "
        " 4. White \n "
        " 5. yellow \n "
        " 6. magenta \n "
        " 7. Custom \n"
        "Enter choice: ");

    scanf("%d", &colorChoice);
    switch (colorChoice) {     //depending on user's input in chooses between the cases which chooses the color of the pipe
        case 1: strcpy(pipeColorCode, Green); break;
        case 2: strcpy(pipeColorCode, Red); break;
        case 3: strcpy(pipeColorCode, blue); break;
        case 4: strcpy(pipeColorCode, white); break;
        case 5: strcpy(pipeColorCode, yellow); break;
        case 6: strcpy(pipeColorCode, magenta); break;
        case 7: {

            printf("Enter custom ANSI color code: ");
            scanf("%s", customColor);
            strcpy(pipeColorCode, customColor);
            break;
        }
        default: printf("Invalid choice. Using default (Green).\n"); strcpy(pipeColorCode, Green);
    }
}

//function to set bird color
void selectBirdColor() {
    int colorChoice;
    printf("\nSelect Bird Color: \n "
        " 1. Green \n "
        " 2. Red \n "
        " 3. Blue \n "
        " 4. White \n "
        " 5. Yellow \n "
        " 6. Magenta \n "
        " 7. Custom \n"
        "Enter choice: ");

    scanf("%d", &colorChoice);
    switch (colorChoice) {                 //depending on user's input in chooses between the cases which chooses the color of the bird
        case 1: strcpy(birdColorCode, Green); break;
        case 2: strcpy(birdColorCode, Red); break;
        case 3: strcpy(birdColorCode, blue); break;
        case 4: strcpy(birdColorCode, white); break;
        case 5: strcpy(birdColorCode, yellow); break;
        case 6: strcpy(birdColorCode, magenta); break;
        case 7: {
            char customColor[10];
            printf("Enter custom ANSI color code: ");
            scanf("%s", customColor);
            strcpy(birdColorCode, customColor);
            break;
        }
        default: printf("Invalid choice. Using default (Yellow).\n"); strcpy(birdColorCode, yellow);
    }
}

//function to display menu
void ShowMenu() {
    while (1) {
        printf("\n=== Flappy Bird Menu ===\n");
        printf(" 1. Set Difficulty \n"
            " 2. Select Pipe Color \n"
            " 3. Select Bird Color \n"
            " 4. Start Game \n"
            " 5. Exit Game \n"
            "Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {                     //user chooses weather it wants to start game directly with deafault settings or customize
            case 1: setDifficulty(); break;
            case 2: selectPipeColor(); break;
            case 3: selectBirdColor(); break;
            case 4:
                printf("Starting game — Speed: %dms, Distance: %d blocks.\n", gameSpeed, pipeDistance);
                return;
            case 5: exit(0);    
            default:
                printf("Invalid choice. Please try again.\n");
        }

        system("pause>nul");
    }
}

// Function to draw the game board
void drawBoard() {
    char buff[5000];                                // Buffer to hold the game board
    strcpy(buff, "\e[17A");                         // Move cursor up 17 lines

    // Display the score at the top of the board
    char scoreBuff[50];
    sprintf(scoreBuff, "Score: %d\n", score);
    strcat(buff, scoreBuff);

    for (int y = 0; y <= screenHieght; y++) {               // Loop over each row
        for (int x = 0; x <= screenWidth; x++) {           // Loop over each column
            if (y == 0 || y == screenHieght || x == 0 || x == screenWidth) {
                strcat(buff, noColor "[]");              // to Draw screen border
                continue;
            }

            for (int i = 0; i < pipeCount; i++) {    // Loop over each pipe
                if (
                    pipes[i].x >= x - 1 && 
                    pipes[i].x <= x + 1 && 
                    (
                        pipes[i].y == y + 3 ||
                        pipes[i].y == y - 3
                    )
                ) {    //this is to construct the pipes in the selected color
                    strcat(buff, pipeColorCode);    // Add selected pipe color 
                    strcat(buff, "■■");                    
                    strcat(buff, noColor);               // Reset color
                    goto bottom;
                } else if (
                    pipes[i].x == x - 1 && 
                    pipes[i].y == y - 4
                ) {
                    strcat(buff, pipeColorCode);    
                    strcat(buff, "]/");
                    strcat(buff, noColor);           
                    goto bottom;
                } else if (
                    pipes[i].x == x && 
                    (
                        pipes[i].y <= y - 4 || 
                        pipes[i].y >= y + 4
                    )
                ) {
                    strcat(buff, pipeColorCode);    
                    strcat(buff, "■■");
                    strcat(buff, noColor);            
                    goto bottom;
                } else if (
                    pipes[i].x == x + 1 && 
                    pipes[i].y == y - 4
                ) {
                    strcat(buff, pipeColorCode); 
                    strcat(buff, "\\[");
                    strcat(buff, noColor);         
                    goto bottom;
                } else if (
                    pipes[i].x == x - 1 && 
                    pipes[i].y == y + 4
                ) {
                    strcat(buff, pipeColorCode);  
                    strcat(buff, "]\\");
                    strcat(buff, noColor);      
                    goto bottom;
                } else if (
                    pipes[i].x == x + 1 && 
                    pipes[i].y == y + 4
                ) {
                    strcat(buff, pipeColorCode);
                    strcat(buff, "/[");
                    strcat(buff, noColor);           
                    goto bottom;
                } else if (
                    pipes[i].x == x + 1 && 
                    (
                        pipes[i].y <= y - 5 || 
                        pipes[i].y >= y + 5
                    )
                ) {
                    strcat(buff, pipeColorCode);    
                    strcat(buff, " [");
                    strcat(buff, noColor);             
                    goto bottom;
                } else if (
                    pipes[i].x == x - 1 && 
                    (
                        pipes[i].y <= y - 5 || 
                        pipes[i].y >= y + 5
                    )
                ) {
                    strcat(buff, pipeColorCode);    
                    strcat(buff, "] ");
                    strcat(buff, noColor);         
                    goto bottom;
                }
            }

            // Draw the bird in the selected color
            if (bird.y == y && bird.x == x) {
                strcat(buff, birdColorCode);        // Add selected bird color
                strcat(buff, ")>");
                strcat(buff, noColor);                  
            } else if (bird.y == y && bird.x == x + 1) {
                strcat(buff, birdColorCode);       
                strcat(buff, "_(");
                strcat(buff, noColor);               
            } else if (bird.y == y && bird.x == x + 2) {
                strcat(buff, birdColorCode);  
                strcat(buff, " _");
                strcat(buff, noColor);                
            } else if (bird.y == y - 1 && bird.x == x) {
                strcat(buff, birdColorCode);       
                strcat(buff, ") ");
                strcat(buff, noColor);                  
            } else if (bird.y == y - 1 && bird.x == x + 1) {
                strcat(buff, birdColorCode);    
                strcat(buff, "__");
                strcat(buff, noColor);                  
            } else if (bird.y == y - 1 && bird.x == x + 2) {
                strcat(buff, birdColorCode);      
                strcat(buff, " \\");
                strcat(buff, noColor);                  
            } else {
                strcat(buff, noColor "  ");            // Empty space in default color
            }
            bottom:;
        }
        strcat(buff, "\n");
    }
    printf("\n%s", buff);                         // to Print the game board
}

// Function to get rid of the pipes when they hit the end of the board
void Pipes() {
    for (int i = 0; i < pipeCount; i++) {
        if (pipes[i].x == -1) {                  // If the pipe moves off-screen
            (i == 0) ? (pipes[i].x = pipes[2].x + pipeDistance) : (pipes[i].x = pipes[i - 1].x + pipeDistance);
            pipes[i].y = (rand() % 7) + 5;      // Set random height for the pipe
        }
    }
}

// Function to test for collisions with the floor or the pipes
void collisionCheck() {
    if (bird.y == screenHieght) {                      // If the bird hits the ground
        ShowEndScreen();
    }

    for (int i = 0; i < pipeCount; i++) {
        if (
            (bird.x - 2 < pipes[i].x + 2) &&   
            (bird.x > pipes[i].x - 2) &&        
            (
                (bird.y < pipes[i].y - 2) ||    
                (bird.y > pipes[i].y + 1)      
            )
        ) {
            ShowEndScreen();                           // sends to end screen on collision
        }
    }
}

// Function to update the score when the bird passes a pipe
void UpdateScore() {
    for (int i = 0; i < pipeCount; i++) {
        if (pipes[i].x == bird.x - 1) {
            score++;
            Beep(1200, 100);
        }
    }
}

// Function to load the High Score
int loadHighScore() {
    FILE *file = fopen("highscore.txt", "r");
    int highscore = 0;

    if (file == NULL) {
        // No highscore file exists yet
        return 0;
    }

    fscanf(file, "%d", &highscore);
    fclose(file);
    return highscore;
}

//Function to save the high score
void saveHighScore(int score) {
    FILE *file = fopen("highscore.txt", "w");
    if (file == NULL) {
        printf("Error saving high score!\n");
        return;
    }
    fprintf(file, "%d", score);
    fclose(file);
}


// Function to show the end screen
void ShowEndScreen() {

    Beep(880, 300);  
    Beep(659, 300);  
    Beep(880, 300);  
    Beep(659, 300);  
    Beep(440, 600);  

    system("cls");                           
    printf("\n");
    printf(" _______  __   __  _______     _______   ___      __   _______\n");
    printf("|__   __||  | |  ||       |   |       | |    |   |  | |   ___  |\n");
    printf("   | |   |  |_|  ||  _____|   |  _____| |     |  |  | |  |   |  |\n");
    printf("   | |   |   _   || |_____    | |_____  |  ||  | |  | |  |    | |\n");
    printf("   | |   |  | |  ||  _____|   |  _____| |  | |  ||  | |  |    | |\n");
    printf("   | |   |  | |  || |_____    | |_____  |  |  |     | |  |___|  |\n");
    printf("   |_|   |__| |__||_______|   |_______| |__|   |____| |________|\n");
    printf("\n");
    printf("Thank you for playing!\n");
    printf("Your Score: %d\n", score);
    int highScore = loadHighScore();
    if (score > highScore) {                        //checks score vs highscore
        printf("New High Score! 🎉 %d\n", score);
        saveHighScore(score);
    } else {
        printf("High Score: %d\n", highScore);
    }

    exit(0);                                      // Exit the game
}

// Main function
int main() {
    SetConsoleOutputCP(65001);
    srand(time(NULL));                         
    system("title \" Flappy Bird\"");

    // Showing menu in the start of the gamr
    ShowMenu();

    bird.x = 10;                              //bird's start position
    bird.y = 10;

    for (int i = 0; i < pipeCount; i++) {
        pipes[i].x = 25 + pipeDistance * i;    // pipe spacing according to the user
        pipes[i].y = (rand() % 7) + 5;         // Set random height for the pipes
    }

    int frame = 0;                            // Variable to keep passed frames
    printf("Press space bar to jump and c to Close.\n");

    while (1) {
        if (GetAsyncKeyState(VK_SPACE)) {         // If the user presses the space bar
            Beep(500, 50);
            bird.y -= 2;                      // Move the bird up 2 pixels
            
        }
        if (GetAsyncKeyState(cKey)) {        // If the user presses c
            break;
        }

        if (frame == 2) {                    // If it's the third frame
            bird.y++;                        // Drop the bird by 1 pixel
            for (int i = 0; i < 3; i++) {    // Move the pipes forward
                pipes[i].x--;
            }
            UpdateScore();                  // Update the score if the bird passes a pipe
            frame = 0;                      // Reset the frame counter
        }

        collisionCheck();                  // Test for collisions
        drawBoard();                       // Draw the game
        Pipes();                           // Update the pipes
        frame++;
        Sleep(gameSpeed);                  // Wait based on user-defined speed
    }

    return 0;
}
