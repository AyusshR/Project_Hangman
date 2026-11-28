#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_WRONG 6

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
    "elephant", "giraffe", "dolphin", "penguin", "kangaroo",
    "cheetah", "leopard", "buffalo", "squirrel", "crocodile",
    "butterfly", "hedgehog", "panther", "peacock", "octopus",
    "tortoise", "gorilla", "sparrow", "parrot", "monkey",
    "india", "canada", "germany", "brazil", "australia",
    "japan", "mexico", "egypt", "nepal", "sweden",
    "mountain", "volcano", "desert", "island", "jungle",
    "waterfall", "glacier", "valley", "ocean", "forest",
    "pizza", "burger", "noodles", "sandwich", "chocolate",
    "biryani", "pancake", "mango", "banana", "pineapple",
    "strawberry", "avocado", "broccoli", "potato", "tomato",
    "umbrella", "bicycle", "backpack", "notebook", "pencil",
    "calendar", "elevator", "lantern", "mirror", "window",
    "science", "history", "teacher", "student",
    "adventure", "treasure", "journey", "rainbow", "thunder",
    "football", "cricket", "swimming", "guitar", "painting"
};
int wordCount = sizeof(words) / sizeof(words[0]);

const char *pickWord(void) {
    return words[rand() % wordCount];
}

void initDisplay(char *display, int length) {
    for (int i = 0; i < length; i++) {
        display[i] = '_';
    }
    display[length] = '\0';
}

int revealLetter(const char *secret, char *display, char letter) {
    int count = 0;
    for (int i = 0; secret[i] != '\0'; i++) {
        if (secret[i] == letter) {
            display[i] = letter;
            count++;
        }
    }
    return count;
}

int isAlreadyGuessed(const char *guessed, char letter) {
    return strchr(guessed, letter) != NULL;
}

void addGuess(char *guessed, char letter) {
    int count = strlen(guessed);
    guessed[count] = letter;
    guessed[count + 1] = '\0';
}

int isWordComplete(const char *display) {
    return strchr(display, '_') == NULL;
}

void drawHangman(int wrong) {
    printf("\n  +---+\n");
    printf("  |   %c\n", wrong >= 1 ? 'O' : ' ');
    printf("  |  %c%c%c\n", wrong >= 3 ? '/' : ' ', wrong >= 2 ? '|' : ' ', wrong >= 4 ? '\\' : ' ');
    printf("  |  %c %c\n", wrong >= 5 ? '/' : ' ', wrong >= 6 ? '\\' : ' ');
    printf("  |\n=====\n");
}

void printStatus(const char *display, const char *guessed, int wrong) {
    drawHangman(wrong);
    printf("\nWord: ");
    for (int i = 0; display[i] != '\0'; i++) {
        printf("%c ", display[i]);
    }
    printf("\nGuessed letters: %s", guessed);
    printf("\nChances left: %d\n", MAX_WRONG - wrong);
}

void showInstructions(void) {
    printf("\n------------- HOW TO PLAY -------------\n");
    printf("1. The computer picks a secret word.\n");
    printf("2. Each '_' is a hidden letter.\n");
    printf("3. Guess ONE letter at a time.\n");
    printf("4. Correct letters are revealed in the word.\n");
    printf("5. Wrong letters cost one chance (you have %d).\n", MAX_WRONG);
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

int readMenuChoice(void) {
    char input[50];
    if (scanf("%49s", input) != 1) {
        return -1;
    }
    if (strlen(input) == 1 && input[0] >= '1' && input[0] <= '3') {
        return input[0] - '0';
    }
    return 0;
}

int readLetter(char *letter) {
    char input[50];
    printf("\nEnter a letter: ");
    if (scanf("%49s", input) != 1) {
        return 0;
    }
    if (strlen(input) == 1 && isalpha((unsigned char)input[0])) {
        *letter = tolower((unsigned char)input[0]);
    } else {
        *letter = '\0';
    }
    return 1;
}

int playGame(void) {
    const char *secret = pickWord();
    int length = strlen(secret);
    char display[40];
    char guessed[27] = "";
    int wrong = 0;

    initDisplay(display, length);

    while (wrong < MAX_WRONG && !isWordComplete(display)) {
        printStatus(display, guessed, wrong);

        char letter;
        if (!readLetter(&letter)) {
            return -1;
        }
        if (letter == '\0') {
            printf("Invalid input! Enter a single letter (a-z).\n");
            continue;
        }
        if (isAlreadyGuessed(guessed, letter)) {
            printf("You already tried '%c'. Try another letter.\n", letter);
            continue;
        }

        addGuess(guessed, letter);

        int matches = revealLetter(secret, display, letter);
        if (matches > 0) {
            printf("Good guess! '%c' appears %d time%s.\n", letter, matches, matches == 1 ? "" : "s");
        } else {
            wrong++;
            printf("Sorry, '%c' is not in the word.\n", letter);
        }
    }

    printStatus(display, guessed, wrong);
    if (isWordComplete(display)) {
        printf("\n*** YOU WON! The word was: %s ***\n", secret);
        return 1;
    }
    printf("\n*** YOU LOST! The word was: %s ***\n", secret);
    return 0;
}

int main(void) {
    int wins = 0;
    int losses = 0;
    int choice;

    srand(time(NULL));

    do {
        showMenu();
        choice = readMenuChoice();

        switch (choice) {
            case 1: {
                int result = playGame();
                if (result == -1) {
                    choice = -1;
                    break;
                }
                if (result == 1) {
                    wins++;
                } else {
                    losses++;
                }
                printf("Score -> Wins: %d  Losses: %d\n", wins, losses);
                break;
            }
            case 2:
                showInstructions();
                break;
            case 0:
                printf("Invalid choice! Please enter 1, 2 or 3.\n");
                break;
        }
    } while (choice != 3 && choice != -1);

    printf("\nFinal score -> Wins: %d  Losses: %d\n", wins, losses);
    printf("Thanks for playing!\n");
    return 0;
}


/*⠤⢤⣤⣤⣤⣤⣤⣤⠀⠀⢰⣀⡆⠀⠀⢠⣤⣤⣤⣤⣤⣤⡤⠤
⠀⠀  ⠛⣿⣿⣿⣿⣿⣷⣤⣼⣿⣧⣤⣤⣾⣿⣿⣿⣿⣿⠛⠀⠀
⠀⠀⠀   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀   ⠉⠙⠿⣿⠿⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀   ⠿*/⠀⠀⠀⠀⠀⠀⠀⠀⠀

//JAI SHREE RAM  
