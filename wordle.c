#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

// --- DEFINED CONSTANTS ---
// Easy maintenance.
#define MAX_GUESSES 6
#define LOSS_SENTINEL 100
#define MAX_WORD_LENGTH 20 // A safe maximum length for any word

// --- GLOBAL CONSTANTS ---
const char * const GREEN = "\x1b[32m";
const char * const YELLOW = "\x1b[33m";
const char * const WHITE = "\x1b[0m";
const char * current = WHITE;

// --- HELPER FUNCTIONS ---

// Sets the color of the console output.
void setColour(const char *colour) {
    if (current == colour) return;
    printf("%s", colour);
    current = colour;
}

// --- CORE GAME LOGIC ---

// Runs the main game loop, including input validation and coloring.
int wordlFunction(const char *answer) {
    int length = strlen(answer);
    int number = 1;
    
    while (1) {
        setColour(WHITE);

        // --- INPUT HANDLING AND VALIDATION ---
        char guess[100]; // A safe buffer for long inputs.

        printf("Enter guess: ");

        // Read the line safely using fgets.
        if (fgets(guess, sizeof(guess), stdin) == NULL) {
            printf("\n"); // Handle Ctrl+D or input error.
            break;
        }

        // Remove the trailing newline character.
        guess[strcspn(guess, "\n")] = 0;

        // Validate the length of the input.
        if (strlen(guess) != length) {
            printf("Your guess must be exactly %d letters long. Try again.\n", length);
            continue; // Skip to the next loop iteration without penalty.
        }

        // Convert guess to lowercase for fair comparison.
        for(int i = 0; guess[i]; i++){
          guess[i] = tolower(guess[i]);
        }

        // Check for a win.
        if (strcmp(guess, answer) == 0) {
            setColour(GREEN);
            printf("%s\n", guess);
            setColour(WHITE);
            break; 
        }

        // --- TWO-PASS COLORING ALGORITHM ---
        char feedback[length + 1];
        memset(feedback, 'W', length); 
        feedback[length] = '\0';

        char answerCopy[length + 1];
        strcpy(answerCopy, answer);

        // Find all GREENS.
        for (int i = 0; i < length; i++) {
            if (guess[i] == answerCopy[i]) {
                feedback[i] = 'G';
                answerCopy[i] = '_'; 
            }
        }

        // Find all YELLOWS from remaining letters.
        for (int i = 0; i < length; i++) {
            if (feedback[i] != 'G') {
                char* match = strchr(answerCopy, guess[i]);
                if (match != NULL) {
                    feedback[i] = 'Y';
                    *match = '_';
                }
            }
        }

        // RENDER THE FEEDBACK
        for (int i = 0; i < length; i++) {
            switch (feedback[i]) {
                case 'G': setColour(GREEN); break;
                case 'Y': setColour(YELLOW); break;
                default:  setColour(WHITE); break;
            }
            printf("%c", guess[i]);
        }
        
        printf("\n");
        ++number; // Increment guess count only for valid guesses.

        if (number > MAX_GUESSES) {
            return LOSS_SENTINEL;
        }
    }

    return number;
}


// --- MAIN FUNCTION ---

// Entry point of the program. Handles file reading and starts the game.
int main() {
    char answer[MAX_WORD_LENGTH];
    char buffer[MAX_WORD_LENGTH];

    // Open the word list file.
    FILE *file = fopen("wordleList.txt", "r");
    if (file == NULL) {
        perror("Error: Cannot open wordleList.txt");
        return 1;
    }

    // Count the words in the file.
    int word_count = 0;
    while (fscanf(file, "%s", buffer) == 1) {
        word_count++;
    }

    if (word_count == 0) {
        printf("Error: No words found in wordleList.txt\n");
        fclose(file);
        return 1;
    }

    // Select a random word.
    srand(time(NULL));
    int random_index = rand() % word_count;

    // Go to the chosen word and read it into 'answer'.
    rewind(file);
    for (int i = 0; i <= random_index; i++) {
        fscanf(file, "%s", answer);
    }
    fclose(file);

    // Convert the chosen answer to lowercase.
    for(int i = 0; answer[i]; i++){
      answer[i] = tolower(answer[i]);
    }

    // Start the game and get the result.
    int number = wordlFunction(answer);
    setColour(WHITE); // Reset color after the game.

    // Print final message.
    if (number == LOSS_SENTINEL) {
        printf("You lost! The word was: %s", answer);
        printf("\n");
    } else {
        printf("Finished in %d guesses", number);
        printf("\n");
    }
    
    return 0;
}