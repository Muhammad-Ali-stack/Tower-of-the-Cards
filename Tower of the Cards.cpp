#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define DECK_SIZE 52
#define MAX_HAND_SIZE 100 // Arbitrarily large to allow multiple draws

struct Card {
    char value[3]; // Value of the card (e.g., "2", "10", "A", "J", "Q", "K")
    char suit[9];  // Suit of the card (e.g., "Hearts", "Diamonds")
};

// Function to set terminal text color
void setColor(const char* color) {
    std::cout << color;
}

// Reset terminal color
void resetColor() {
    std::cout << "\033[0m";
}

// Play a beep sound
void playSound() {
#ifdef _WIN32
    Beep(750, 300);
#else
    std::cout << "\a";
#endif
}

// Function to create a deck of 52 cards
void createDeck(Card deck[]) {
    const char* values[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    const char* suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    int index = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            snprintf(deck[index].value, sizeof(deck[index].value), "%s", values[j]);
            snprintf(deck[index].suit, sizeof(deck[index].suit), "%s", suits[i]);
            index++;
        }
    }
}

// Function to shuffle the deck
void shuffleDeck(Card deck[]) {
    srand(time(0));
    for (int i = 0; i < DECK_SIZE; i++) {
        int j = rand() % DECK_SIZE;
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

// Function to get the color based on suit or if it's a power card
const char* getCardColor(const Card* card) {
    if (strcmp(card->value, "J") == 0 || strcmp(card->value, "Q") == 0 || strcmp(card->value, "K") == 0 || strcmp(card->value, "A") == 0) {
        return "\033[1;35m"; // Purple for power cards
    }

    if (strcmp(card->suit, "Hearts") == 0) {
        return "\033[1;31m"; // Red
    } else if (strcmp(card->suit, "Diamonds") == 0) {
        return "\033[1;36m"; // Light Blue
    } else if (strcmp(card->suit, "Clubs") == 0) {
        return "\033[1;33m"; // Yellow
    } else if (strcmp(card->suit, "Spades") == 0) {
        return "\033[1;32m"; // Light Green
    }

    return "\033[0m"; // Default color
}

// Function to draw a card from the deck
Card drawCard(Card deck[], int* deckIndex) {
    return deck[(*deckIndex)--];
}

// Function to display a card with color
void displayCard(const Card* card) {
    setColor(getCardColor(card));
    std::cout << card->value << " of " << card->suit << std::endl;
    resetColor();
}

// Function to display a hand of cards
void displayHand(Card hand[], int handSize) {
    for (int i = 0; i < handSize; i++) {
        std::cout << i + 1 << ": ";
        displayCard(&hand[i]);
    }
}

// Function to remove a card from the hand
void removeCardFromHand(Card hand[], int* handSize, int index) {
    for (int i = index; i < *handSize - 1; i++) {
        hand[i] = hand[i + 1];
    }
    (*handSize)--;
}

// Function to check if a card is a power card
bool isPowerCard(const Card* card) {
    return (strcmp(card->value, "J") == 0 || strcmp(card->value, "K") == 0 || strcmp(card->value, "Q") == 0 || strcmp(card->value, "A") == 0);
}

// Function to calculate the points of a card
int getCardPoints(const Card* card) {
    if (strcmp(card->value, "A") == 0) return 15;
    if (strcmp(card->value, "K") == 0) return 13;
    if (strcmp(card->value, "Q") == 0) return 12;
    if (strcmp(card->value, "J") == 0) return 11;
    return atoi(card->value); // For numerical cards, return the integer value
}

// Function to apply the effect of power cards
void applyPowerCard(const Card* card, Card hand[], int* handSize, Card deck[], int* deckIndex) {
    playSound(); // Play sound when a power card is applied
    if (strcmp(card->value, "J") == 0) {
        std::cout << "Power Card (Joker) played! Drawing two extra cards.\n";
        if (*deckIndex >= 1) {
            hand[(*handSize)++] = drawCard(deck, deckIndex);
            hand[(*handSize)++] = drawCard(deck, deckIndex);
        }
    } else if (strcmp(card->value, "K") == 0) {
        std::cout << "Power Card (King) played! Removing two points from the opponent's score.\n";
    } else if (strcmp(card->value, "Q") == 0) {
        std::cout << "Power Card (Queen) played! Protecting tower for the next three turns.\n";
    } else if (strcmp(card->value, "A") == 0) {
        std::cout << "Power Card (Ace) played! Doubling points for the next three cards.\n";
    }
}

// Function to display power card effects
void displayPowerCardEffects() {
    setColor("\033[1;35m"); // Purple color for power cards
    std::cout << "\nPower Card Effects:\n";
    std::cout << "J (Joker): Draw two extra cards.\n";
    std::cout << "K (King): Remove two points from the opponent's score.\n";
    std::cout << "Q (Queen): Protect tower for the next three turns.\n";
    std::cout << "A (Ace): Double points for the next three cards drawn.\n";
    resetColor();
}

// Main game function
int main() {
    Card deck[DECK_SIZE];
    createDeck(deck);
    shuffleDeck(deck);

    int deckIndex = DECK_SIZE - 1;
    Card playerHand[MAX_HAND_SIZE];
    int playerHandSize = 5;

    for (int i = 0; i < playerHandSize; i++) {
        playerHand[i] = drawCard(deck, &deckIndex);
    }

    int playerPoints = 0, computerPoints = 0;

    // Display rules and power card effects
    setColor("\033[1;32m"); // Green for rules
    std::cout << "Welcome to the Card Game!" << std::endl;
    std::cout << "Rules:\n";
    std::cout << "1. Each turn, you can play a card from your hand.\n";
    std::cout << "2. Power cards (J, Q, K, A) have special effects.\n";
    std::cout << "3. The game ends when the deck is empty or after a set number of rounds.\n";
    std::cout << "4. Highest points win!\n";
    resetColor();

    displayPowerCardEffects();

    int numRounds;
    std::cout << "\nEnter the number of rounds to play: ";
    std::cin >> numRounds;

    for (int round = 1; round <= numRounds; round++) {
        std::cout << "\n--- Round " << round << " ---" << std::endl;

        // Player's turn
        std::cout << "Your hand:\n";
        displayHand(playerHand, playerHandSize);

        int choice;
        std::cout << "Choose a card to play (1-" << playerHandSize << "): ";
        std::cin >> choice;

        Card chosenCard = playerHand[choice - 1];
        std::cout << "You played: ";
        displayCard(&chosenCard);

        removeCardFromHand(playerHand, &playerHandSize, choice - 1);
        playerPoints += getCardPoints(&chosenCard);

        if (isPowerCard(&chosenCard)) {
            applyPowerCard(&chosenCard, playerHand, &playerHandSize, deck, &deckIndex);
        }

        // Computer's turn (simulated)
        std::cout << "Computer is playing...\n";
        Card computerCard = drawCard(deck, &deckIndex);
        std::cout << "Computer played: ";
        displayCard(&computerCard);

        computerPoints += getCardPoints(&computerCard);

        if (isPowerCard(&computerCard)) {
            applyPowerCard(&computerCard, playerHand, &playerHandSize, deck, &deckIndex);
        }
    }

    // Display final scores and winner
    std::cout << "\nGame Over!\n";
    std::cout << "Your Score: " << playerPoints << std::endl;
    std::cout << "Computer's Score: " << computerPoints << std::endl;

    if (playerPoints > computerPoints) {
        std::cout << "You win!\n";
    } else if (computerPoints > playerPoints) {
        std::cout << "Computer wins!\n";
    } else {
        std::cout << "It's a tie!\n";
    }

    return 0;
}