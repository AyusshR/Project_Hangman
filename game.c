/*
 * Hangman - Word Guessing Game
 * FOCP-I (C Programming) Mini Project
 *
 * The computer picks a secret word from a predefined list and the player
 * guesses one letter at a time. The player wins by revealing the whole word
 * before using up MAX_WRONG_GUESSES wrong guesses.
 *
 * Team: Dragon Warriors
 *
 * C concepts used: strings, character arrays, pointers, functions,
 *                  random number generation, input validation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_WRONG_GUESSES  6    /* wrong guesses allowed per round        */
#define MAX_WORD_LENGTH    32   /* longest word + room for the '\0'       */
#define ALPHABET_SIZE      26   /* letters a-z, so at most 26 guesses     */
#define INPUT_BUFFER_SIZE  64   /* size of the line buffer for fgets()    */

/* Predefined words list (lowercase letters only; add more any time) */
static const char *words[] = {
    // dictionary of words 
    /* programming and computing */
    "computer", "pointer", "program", "keyboard", "function",
    "variable", "compiler", "software", "algorithm", "terminal",
    "memory", "array", "string", "integer", "loop",
    "debugger", "network", "database", "library", "syntax",
    "hardware", "internet", "browser", "protocol", "binary",
    "boolean", "recursion", "structure", "operator", "keyword",
    "compile", "execute", "console", "storage", "processor",
    "download", "firewall", "password",

    /* animals */
    "elephant", "giraffe", "dolphin", "penguin", "kangaroo",
    "cheetah", "leopard", "buffalo", "squirrel", "crocodile",
    "butterfly", "hedgehog", "panther", "peacock", "octopus",
    "tortoise", "gorilla", "sparrow", "parrot", "monkey",

    /* countries and places */
    "india", "canada", "germany", "brazil", "australia",
    "japan", "mexico", "egypt", "nepal", "sweden",
    "mountain", "volcano", "desert", "island", "jungle",
    "waterfall", "glacier", "valley", "ocean", "forest",

    /* food */
    "pizza", "burger", "noodles", "sandwich", "chocolate",
    "biryani", "pancake", "mango", "banana", "pineapple",
    "strawberry", "avocado", "broccoli", "potato", "tomato",

    /* everyday objects and general words */
    "umbrella", "bicycle", "backpack", "notebook", "pencil",
    "calendar", "elevator", "lantern", "mirror", "window",
    "science", "history", "teacher", "student",
    "adventure", "treasure", "journey", "rainbow", "thunder",
    "football", "cricket", "swimming", "guitar", "painting"
};
static const int WORD_COUNT = (int)(sizeof(words) / sizeof(words[0]));

/* ------------------------------------------------------------------ */
/* Word and display helpers                                           */
/* ------------------------------------------------------------------ */

/* Return a random word from the list. */
const char *pickRandomWord(void) {
    return words[rand() % WORD_COUNT];
}

/* Fill the display string with '_' (one per letter) and terminate it. */
void initDisplay(char *display, int wordLength) {
    for (int i = 0; i < wordLength; i++) {
        display[i] = '_';
    }
    display[wordLength] = '\0';
}

/*
 * Reveal every position of the secret word that matches the guessed letter.
 * Returns how many letters were revealed (0 means a wrong guess).
 */
int revealLetter(const char *secretWord, char *display, char guess) {
    int revealedCount = 0;

    for (int i = 0; secretWord[i] != '\0'; i++) {
        if (secretWord[i] == guess && display[i] == '_') {
            display[i] = guess;
            revealedCount++;
        }
    }
    return revealedCount;
}

/* Return 1 if the letter was already guessed, otherwise 0. */
int isAlreadyGuessed(const char *guessedLetters, char letter) {
    return strchr(guessedLetters, letter) != NULL;
}

/* Append a letter to the guessed-letters string. */
void recordGuess(char *guessedLetters, char letter) {
    size_t guessedCount = strlen(guessedLetters);

    guessedLetters[guessedCount] = letter;
    guessedLetters[guessedCount + 1] = '\0';   /* keep it a valid string */
}

/* The player has won when no '_' is left in the display. */
int isWordComplete(const char *display) {
    return strchr(display, '_') == NULL;
}

/* ------------------------------------------------------------------ */
/* Output                                                             */
/* ------------------------------------------------------------------ */

/*
 * Draw the hangman. Each body part appears once the number of wrong
 * guesses reaches its threshold: head 1, body 2, left arm 3,
 * right arm 4, left leg 5, right leg 6.
 */
void drawHangman(int wrongGuesses) {
    printf("\n  +---+\n");
    printf("  |   %c\n", wrongGuesses >= 1 ? 'O' : ' ');
    printf("  |  %c%c%c\n",
           wrongGuesses >= 3 ? '/'  : ' ',
           wrongGuesses >= 2 ? '|'  : ' ',
           wrongGuesses >= 4 ? '\\' : ' ');
    printf("  |  %c %c\n",
           wrongGuesses >= 5 ? '/'  : ' ',
           wrongGuesses >= 6 ? '\\' : ' ');
    printf("  |\n");
    printf("=====\n");
}

/* Show the hangman, the hidden word, tried letters and chances left. */
void printGameStatus(const char *display, const char *guessedLetters,
                     int wrongGuesses) {
    drawHangman(wrongGuesses);

    printf("\nWord: ");
    for (int i = 0; display[i] != '\0'; i++) {
        printf("%c ", display[i]);
    }
    printf("\nGuessed letters: %s", guessedLetters);
    printf("\nChances left: %d\n", MAX_WRONG_GUESSES - wrongGuesses);
}

void showInstructions(void) {
    printf("\n------------- HOW TO PLAY -------------\n");
    printf("1. The computer picks a secret word.\n");
    printf("2. Each '_' is a hidden letter.\n");
    printf("3. Guess ONE letter at a time.\n");
    printf("4. Correct letters are revealed in the word.\n");
    printf("5. Wrong letters cost one chance (you have %d).\n",
           MAX_WRONG_GUESSES);
    printf("6. Guess the whole word before chances run out to win!\n");
    printf("---------------------------------------\n");
}

void showMenu(void) {
    printf("\n=============================\n");
    printf("      HANGMAN WORD GAME\n");
    printf("=============================\n");
    printf("  1. Play\n");
    printf("  2. Instructions\n");
    printf("  3. Exit\n");
    printf("Enter your choice: ");
}

/* ------------------------------------------------------------------ */
/* Input (all keyboard input goes through fgets, never scanf, so that */
/* bad input cannot leave stray characters behind)                    */
/* ------------------------------------------------------------------ */

/* Read one line from the keyboard. Returns 0 on end of input, else 1. */
int readLine(char *buffer, int bufferSize) {
    if (fgets(buffer, bufferSize, stdin) == NULL) {
        return 0;
    }

    /* No newline means the line was longer than the buffer: throw away
       the rest so it is not mistaken for the next input. */
    if (strchr(buffer, '\n') == NULL) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            ;
        }
    }
    return 1;
}

/*
 * Read one guessed letter. Stores the lowercase letter in *letter, or '\0'
 * if the input was not exactly one letter. Returns 0 on end of input.
 */
int readGuess(char *letter) {
    char line[INPUT_BUFFER_SIZE];

    printf("\nEnter a letter: ");
    if (!readLine(line, sizeof(line))) {
        return 0;
    }

    const char *cursor = line;
    while (*cursor == ' ' || *cursor == '\t') {   /* skip leading spaces */
        cursor++;
    }

    /* Valid only if it is one letter followed by the end of the line.
       The cast to unsigned char keeps isalpha/tolower safe. */
    if (isalpha((unsigned char)cursor[0]) &&
        (cursor[1] == '\n' || cursor[1] == '\0')) {
        *letter = (char)tolower((unsigned char)cursor[0]);
    } else {
        *letter = '\0';
    }
    return 1;
}

/* Read a menu choice. Returns 1-3, 0 if invalid, or -1 on end of input. */
int readMenuChoice(void) {
    char line[INPUT_BUFFER_SIZE];

    if (!readLine(line, sizeof(line))) {
        return -1;
    }
    if ((line[0] == '1' || line[0] == '2' || line[0] == '3') &&
        (line[1] == '\n' || line[1] == '\0')) {
        return line[0] - '0';
    }
    return 0;
}

/* ------------------------------------------------------------------ */
/* Game logic                                                         */
/* ------------------------------------------------------------------ */

/* Play one round. Returns 1 if won, 0 if lost, -1 if input ended. */
int playRound(void) {
    const char *secretWord = pickRandomWord();
    int wordLength = (int)strlen(secretWord);

    char display[MAX_WORD_LENGTH];
    char guessedLetters[ALPHABET_SIZE + 1] = "";
    int wrongGuesses = 0;

    initDisplay(display, wordLength);

    while (wrongGuesses < MAX_WRONG_GUESSES && !isWordComplete(display)) {
        printGameStatus(display, guessedLetters, wrongGuesses);

        char guess;
        if (!readGuess(&guess)) {
            return -1;
        }

        /* Invalid and repeated guesses never cost a chance. */
        if (guess == '\0') {
            printf("Invalid input! Enter a single letter (a-z).\n");
            continue;
        }
        if (isAlreadyGuessed(guessedLetters, guess)) {
            printf("You already tried '%c'. Try another letter.\n", guess);
            continue;
        }

        recordGuess(guessedLetters, guess);

        int matchCount = revealLetter(secretWord, display, guess);
        if (matchCount > 0) {
            printf("Good guess! '%c' appears %d time%s.\n",
                   guess, matchCount, matchCount == 1 ? "" : "s");
        } else {
            wrongGuesses++;
            printf("Sorry, '%c' is not in the word.\n", guess);
        }
    }

    /* Round over: show the final state, then the result. */
    printGameStatus(display, guessedLetters, wrongGuesses);
    if (isWordComplete(display)) {
        printf("\n*** YOU WON! The word was: %s ***\n", secretWord);
        return 1;
    }
    printf("\n*** YOU LOST! The word was: %s ***\n", secretWord);
    return 0;
}

int main(void) {
    int winCount = 0;
    int lossCount = 0;
    int menuChoice;

    srand((unsigned)time(NULL));   /* seed so the word differs each run */

    do {
        showMenu();
        menuChoice = readMenuChoice();

        switch (menuChoice) {
            case 1: {
                int roundResult = playRound();

                if (roundResult == 1) {
                    winCount++;
                } else if (roundResult == 0) {
                    lossCount++;
                } else {
                    menuChoice = -1;   /* input ended: leave the loop */
                }
                if (roundResult != -1) {
                    printf("Score -> Wins: %d  Losses: %d\n",
                           winCount, lossCount);
                }
                break;
            }
            case 2:
                showInstructions();
                break;
            case 0:
                printf("Invalid choice! Please enter 1, 2 or 3.\n");
                break;
            default:   /* 3 = exit, -1 = input ended: loop condition stops */
                break;
        }
    } while (menuChoice != 3 && menuChoice != -1);

    printf("\nFinal score -> Wins: %d  Losses: %d\n", winCount, lossCount);
    printf("Thanks for playing!\n");
    return 0;
}
