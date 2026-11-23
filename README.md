# Project_Hangman

**Hangman: Word Guessing Game in C**
FOCP-I (C Programming) Mini Project

## Team Members

| S.No | Name | Roll No. | GitHub Username |
|------|------|----------|-----------------|
| 1 | Ayush Raj | 26CSU383 | AyusshR |
| 2 | Manas Manchanda | 26CSU037 | manasmanchanda |
| 3 | Devansh | 26CSU032 | Devansh1908-eq |
| 4 | Shubham Saini| 26CSU015 | Shubham-Saini015 |
| 5 | Yuganshu Soni | 26CSU036 | yuganshusoni55 |

## Description

Hangman is a console-based word-guessing game written in C. The computer secretly picks a word from a predefined list and shows it as underscores. The player guesses one letter at a time. Correct letters are revealed in their positions, and each wrong letter adds a part to an ASCII hangman. The player wins by uncovering the whole word before 6 wrong guesses are used. The correct word is shown at the end of every round.

## Features

- Menu-driven interface: Play, Instructions, Exit
- Random secret word chosen from a list of 100+ words (tech, animals, places, food, everyday objects)
- Hidden word displayed with underscores; matching positions revealed on a correct guess
- 6 chances, with a live count of remaining chances
- ASCII hangman drawing that grows with every wrong guess
- Input validation: only single letters accepted, case-insensitive, repeated guesses do not cost a chance
- Win/loss message showing the correct word
- Running win/loss score across rounds

## C Concepts Used

Strings and character arrays, pointers, user-defined functions, random number generation (`rand()` / `srand()`), loops, `switch`, and input validation with `fgets`.

## How to Compile and Run

Make sure `gcc` is installed, then run:

```
gcc -Wall hangman.c -o hangman
./hangman
```

On Windows (MinGW):

```
gcc -Wall hangman.c -o hangman.exe
hangman.exe
```

## How to Play

1. Choose **1. Play** from the menu.
2. Type one letter and press Enter.
3. Reveal the whole word before you run out of the 6 chances.
4. Choose **2. Instructions** from the menu at any time to see the rules.

## Project Structure

```
Project_Hangman/
├── hangman.c        # source code
├── README.md        # this file
├── screenshots/     # output screenshots (menu, gameplay, result)
└── docs/            # completed project documentation (DOCX/PDF)
```

## Customising the Word List

Open `hangman.c` and add words to the `words[]` array. Each word must be lowercase letters only (no spaces, hyphens or digits).
