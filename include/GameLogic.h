// #pragma once

// #include <chrono>
// #include <algorithm>
// #include <vector>
// #include <iostream>
// #include <random>
// #include <array>
// #include "Deck.h"
// #include "StockPile.h"
// #include "WastePile.h"
// #include "FoundationPile.h"
// #include "TableauPile.h"


// class GameLogic {
// private:
//     Deck deck;
//     StockPile stockPile;
//     WastePile wastePile;
//     array<FoundationPile, 4> foundationPiles;
//     array<TableauPile, 7> tableauPiles;
//     int moveCount;

//     bool isValidTableauMove(const Card& card, const Card& target) {
//         // Different color (red/black) and one rank lower
//         bool isDifferentColor = ((card.getSuit() == Card::Heart || card.getSuit() == Card::Diamond) !=
//                                (target.getSuit() == Card::Heart || target.getSuit() == Card::Diamond));
//         return isDifferentColor && (card.getRank() == target.getRank() - 1);
//     }

//     bool isValidTableauSequence(const vector<Card>& cards) {
//         for (size_t i = 0; i < cards.size() - 1; i++) {
//             if (!isValidTableauMove(cards[i+1], cards[i])) {
//                 return false;
//             }
//         }
//         return true;
//     }

//     bool moveMultipleCards(int sourceIndex, int destIndex, int cardCount) {
//         try {
//             if (cardCount > tableauPiles[sourceIndex].getSize()) {
//                 return false;
//             }

//             vector<Card> cardsToMove = tableauPiles[sourceIndex].peekCards(cardCount);

//             if (!isValidTableauSequence(cardsToMove)) {
//                 return false;
//             }

//             if (!tableauPiles[destIndex].isEmpty()) {
//                 if (!isValidTableauMove(cardsToMove[0], tableauPiles[destIndex].topCard())) {
//                     return false;
//                 }
//             } else if (cardsToMove[0].getRank() != Card::King) {
//                 return false;
//             }

//             cardsToMove = tableauPiles[sourceIndex].removeCards(cardCount);
//             tableauPiles[destIndex].addCards(cardsToMove);
//             moveCount++;
//             return true;
//         }
//         catch (const exception&) {
//             cerr << "Invalid move" << endl;
//             return false;
//         }
//     }

// public:
//     GameLogic() : foundationPiles{
//         FoundationPile(),
//         FoundationPile(),
//         FoundationPile(),
//         FoundationPile()
//     }, moveCount(0) {
//         initializeGame();
//     }

//     void initializeGame() {
//         deck.initializeDeck();
//         deck.shuffleDeck();

//         // Initialize 7 Tableaus
//         for (int i = 0; i < 7; i++) {
//             for (int j = i; j < 7; j++) {
//                 Card card = deck.dealCard();
//                 // Flip Up the top card of each tableau
//                 if (i == j) card.flip();
//                 tableauPiles[j].addCard(card);
//             }
//         }

//         // Move remaining deck cards to stockPile
//         while(!deck.isEmpty()) {
//             stockPile.addCard(deck.dealCard());
//         }
//     }

//     // Add this helper inside the GameLogic class (private or public as you prefer)
// void recycleWasteToStock() {
//     // Collect all cards from waste (top -> bottom)
//     vector<Card> temp;
//     while (!wastePile.isEmpty()) {
//         Card c = wastePile.removeCard(); // removes top card from waste
//         c.flip();                         // make it face-down for stock
//         temp.push_back(c);
//     }

//     // Move cards back into stock in reversed order so the stock's draw order
//     // repeats correctly when the player starts drawing again.
//     // temp currently holds: [top_of_waste, ..., bottom_of_waste]
//     // We add from temp.rbegin() -> temp.rend() so the bottom_of_waste becomes
//     // the card on top of the stock as expected after flipping whole pile.
//     for (auto it = temp.rbegin(); it != temp.rend(); ++it) {
//         stockPile.addCard(*it);
//     }

//     cout << "Stock recycled from Waste." << endl;
// }


//     // Replace your existing drawCard() with this version
// void drawCard() {
//     if (!stockPile.isEmpty()) {
//         Card card = stockPile.removeCard();
//         card.flip();  // Ensure the card is face up
//         wastePile.addCard(card);
//         moveCount++;
//     } 
//     else if (!wastePile.isEmpty()) {
//         // Recycle the whole waste back into stock (face-down) — no shuffling
//         recycleWasteToStock();
//         // note: not incrementing moveCount for recycling; adjust if you prefer counting it
//     } 
//     else {
//         cerr << "No cards left to draw!" << endl;
//     }
// }


//     // indexes => StockPile: 0, WastePile: 1, Tableaus: 2 to 8, Foundations: 9 to 12
//     bool moveCard(int sourcePile, int destinationPile, int cardCount = 1) {
//         try {
//             // Stock to waste (draw)
//             if (sourcePile == 0 && destinationPile == 1) {
//                 drawCard();
//                 return true;
//             }

//             // Tableau to tableau (Mutiple cards)
//             if (sourcePile >= 2 && sourcePile <= 8 && 
//                 destinationPile >= 2 && destinationPile <= 8 && 
//                 cardCount > 1) {
//                 return moveMultipleCards(sourcePile - 2, destinationPile - 2, cardCount);
//             }

//             // Waste to tableau
//             if (sourcePile == 1 && destinationPile >= 2 && destinationPile <= 8) {
//                 if (wastePile.isEmpty()) return false;
//                 Card card = wastePile.topCard();
//                 int tableauIndex = destinationPile - 2;
                
//                 if (tableauPiles[tableauIndex].isEmpty() && card.getRank() == Card::King) {
//                     tableauPiles[tableauIndex].addCard(wastePile.removeCard());
//                     moveCount++;
//                     return true;
//                 }
                
//                 if (!tableauPiles[tableauIndex].isEmpty() && 
//                     isValidTableauMove(card, tableauPiles[tableauIndex].topCard())) {
//                     tableauPiles[tableauIndex].addCard(wastePile.removeCard());
//                     moveCount++;
//                     return true;
//                 }
//             }

//             // Waste to foundation
//             if (sourcePile == 1 && destinationPile >= 9 && destinationPile <= 12) {
//                 if (wastePile.isEmpty()) return false;
//                 Card card = wastePile.topCard();
//                 if(foundationPiles[destinationPile - 9].addCard(card)) {
//                     wastePile.removeCard();
//                     moveCount++;
//                     return true;
//                 } else {
//                     return false;
//                 }
//             }

//             // Foundation to tableau
//             if (sourcePile >= 9 && sourcePile <= 12 && destinationPile >= 2 && destinationPile <= 8) {
//                 int foundationIndex = sourcePile - 9;
//                 if (foundationPiles[foundationIndex].isEmpty()) return false;
//                 Card card = foundationPiles[foundationIndex].topCard();
//                 int tableauIndex = destinationPile - 2;
//                 if (tableauPiles[tableauIndex].isEmpty() && card.getRank() == Card::King) {
//                     tableauPiles[tableauIndex].addCard(foundationPiles[foundationIndex].removeCard());
//                     moveCount++;
//                     return true;
//                 }
//                 if (!tableauPiles[tableauIndex].isEmpty() && 
//                     isValidTableauMove(card, tableauPiles[tableauIndex].topCard())) {
//                     tableauPiles[tableauIndex].addCard(foundationPiles[foundationIndex].removeCard());
//                     moveCount++;
//                     return true;
//                 }
//             }

//             // Tableau to tableau
//             if (sourcePile >= 2 && sourcePile <= 8 && destinationPile >= 2 && destinationPile <= 8) {
//                 int sourceIndex = sourcePile - 2;
//                 int destIndex = destinationPile - 2;
                
//                 if (tableauPiles[sourceIndex].isEmpty()) return false;
//                 Card card = tableauPiles[sourceIndex].topCard();
                
//                 if (tableauPiles[destIndex].isEmpty() && card.getRank() == Card::King) {
//                     tableauPiles[destIndex].addCard(tableauPiles[sourceIndex].removeCard());
//                     moveCount++;
//                     return true;
//                 }
                
//                 if (!tableauPiles[destIndex].isEmpty() && 
//                     isValidTableauMove(card, tableauPiles[destIndex].topCard())) {
//                     tableauPiles[destIndex].addCard(tableauPiles[sourceIndex].removeCard());
//                     moveCount++;
//                     return true;
//                 }
//             }

//             // Tableau to foundation
//             if (sourcePile >= 2 && sourcePile <= 8 && destinationPile >= 9 && destinationPile <= 12) {
//                 int tableauIndex = sourcePile - 2;
//                 if (tableauPiles[tableauIndex].isEmpty()) return false;
//                 Card card = tableauPiles[tableauIndex].topCard();
//                 if (foundationPiles[destinationPile - 9].addCard(card)) {
//                     tableauPiles[tableauIndex].removeCard();
//                     moveCount++;
//                     return true;
//                 } else {
//                     return false;
//                 }
//                 return true;
//             }

//             return false;
//         }
//         catch (const exception& e) {
//             return false;
//         }
//     }
//     bool isGameWon() const {
//         // Check if all foundation piles have 13 cards
//         for (const auto& pile : foundationPiles) {
//             if (pile.isEmpty() || pile.getSize() != 13) return false;
//         }
//         return true;
//     }

//     void printPersistentInstructions(int x, int y) const {
//         setCursorPosition(x, y);
//         cout << "=> Press D to deselect the card";
//         setCursorPosition(x, y + 1);
//         cout << "=> Press S to switch between Dark and Light mode";
//         setCursorPosition(x, y + 2);
//         cout << "=> Press ESC to exit the game";
//     }

//     void displayGame() const {
//         setCursorPosition(0, 3);
//         cout << "Stock Pile =>";
//         stockPile.displayTopCard();
//         setCursorPosition(30, 3);
//         cout << "<= Waste Pile\n";
//         wastePile.displayTopCard();
    
//         setCursorPosition(65, 0);
//         cout << "Foundations" << endl;
//         for (int i = 0; i < foundationPiles.size(); ++i) {
//             foundationPiles[i].displayTopCard(50+(10*i),1);
//             cout << "\n";
//         }

//         setCursorPosition(30, 8);
//         cout << "Tableaus" << endl;
//         int currentX = -7;
//         int currentY = 9;
//         for (int i = 0; i < tableauPiles.size(); ++i) {
//             setCursorPosition(currentX+=10, currentY);
//             cout << i + 1;
//             tableauPiles[i].displayTopCard(10*i,10);
//         }

//         setCursorPosition(95, 2);
//         cout << "Move Count: " << moveCount;
//         printPersistentInstructions(80, 12);
//     }

//     string displayHint() {
//         vector<string> hints;
//         try {
//             if (!getWastePile().isEmpty()) {
//                 const Card wasteCard = getWastePile().topCard();
//                 for (int i = 0; i < 4; ++i) {
//                     if (foundationPiles[i].isValidMove(wasteCard)) {
//                         hints.push_back("Hint: Move card from Waste to Foundation " + to_string(i + 1));
//                     }
//                 }
//             }

//             if (!getWastePile().isEmpty()) {
//                 const Card wasteCard = getWastePile().topCard();
//                 for (int i = 0; i < 7; ++i) {
//                     const Card destCard = tableauPiles[i].topCard();
//                     if (isValidTableauMove(wasteCard, destCard)) {
//                         hints.push_back("Hint: Move card from Waste to Tableau " + to_string(i + 1));
//                     }
//                 }
//             }

//             for (int i = 0; i < 7; ++i) {
//                 if (!tableauPiles[i].isEmpty()) {
//                     const Card sourceCard = tableauPiles[i].topCard();
//                     for (int j = 0; j < 7; ++j) {
//                         const Card destCard = tableauPiles[j].topCard();
//                         if (isValidTableauMove(sourceCard, destCard)) {
//                             hints.push_back("Hint: Move card from Tableau " + to_string(i + 1) + " to Tableau " + to_string(j + 1));
//                         }
//                     }
//                 }
//             }

//             if (!getStockPile().isEmpty()) {
//                 hints.push_back("Hint: Move card from Stock to Waste");
//             }

//             // return random hint from hint vector
//             if (hints.empty()) {
//                 return "No hints available";
//             }

//             random_device rd;
//             mt19937 gen(rd());
//             uniform_int_distribution<> distr(0, hints.size() - 1);
//             return hints[distr(gen)];


//         } catch (const exception& e) { 
//             return "Hint: Move card from Stock to Waste";
//         }
//     }

//     const StockPile& getStockPile() const { return stockPile; }

//     const WastePile& getWastePile() const { return wastePile; }

//     const array<FoundationPile, 4>& getFoundationPiles() const { return foundationPiles; }

//     const array<TableauPile, 7>& getTableauPiles() const { return tableauPiles; }
// };











// #pragma once

// #include <chrono>
// #include <algorithm>
// #include <vector>
// #include <iostream>
// #include <random>
// #include <array>
// #include "Deck.h"
// #include "StockPile.h"
// #include "WastePile.h"
// #include "FoundationPile.h"
// #include "TableauPile.h"


// class GameLogic {
// private:
//     Deck deck;
//     StockPile stockPile;
//     WastePile wastePile;
//     array<FoundationPile, 4> foundationPiles;
//     array<TableauPile, 7> tableauPiles;
//     int moveCount;

//     bool isValidTableauMove(const Card& card, const Card& target) {
//         // Different color (red/black) and one rank lower
//         bool isDifferentColor = ((card.getSuit() == Card::Heart || card.getSuit() == Card::Diamond) !=
//                                (target.getSuit() == Card::Heart || target.getSuit() == Card::Diamond));
//         return isDifferentColor && (card.getRank() == target.getRank() - 1);
//     }

//     bool isValidTableauSequence(const vector<Card>& cards) {
//         for (size_t i = 0; i < cards.size() - 1; i++) {
//             if (!isValidTableauMove(cards[i+1], cards[i])) {
//                 return false;
//             }
//         }
//         return true;
//     }

//     bool moveMultipleCards(int sourceIndex, int destIndex, int cardCount) {
//         try {
//             if (cardCount > tableauPiles[sourceIndex].getSize()) {
//                 return false;
//             }

//             vector<Card> cardsToMove = tableauPiles[sourceIndex].peekCards(cardCount);

//             if (!isValidTableauSequence(cardsToMove)) {
//                 return false;
//             }

//             if (!tableauPiles[destIndex].isEmpty()) {
//                 if (!isValidTableauMove(cardsToMove[0], tableauPiles[destIndex].topCard())) {
//                     return false;
//                 }
//             } else if (cardsToMove[0].getRank() != Card::King) {
//                 return false;
//             }

//             cardsToMove = tableauPiles[sourceIndex].removeCards(cardCount);
//             tableauPiles[destIndex].addCards(cardsToMove);
//             moveCount++;
//             return true;
//         }
//         catch (const exception&) {
//             cerr << "Invalid move" << endl;
//             return false;
//         }
//     }

//     void recycleWasteToStock() {
//         vector<Card> allHiddenCards;
//         vector<vector<Card>> tableauFaceUpCards(7);
        
//         // Step 1: Collect face-up and face-down cards from each tableau
//         for (int i = 0; i < 7; ++i) {
//             Stack tempStack = tableauPiles[i].getCards();
//             vector<Card> allCards;
            
//             // Pop all cards
//             while (!tempStack.isEmpty()) {
//                 allCards.push_back(tempStack.pop());
//             }
            
//             // Reverse to get bottom-to-top order
//             reverse(allCards.begin(), allCards.end());
            
//             // Separate face-down (hidden) from face-up
//             bool foundFaceUp = false;
//             for (const auto& card : allCards) {
//                 if (card.isFaceUp()) {
//                     foundFaceUp = true;
//                     tableauFaceUpCards[i].push_back(card);
//                 } else if (!foundFaceUp) {
//                     allHiddenCards.push_back(card);
//                 }
//             }
//         }
        
//         // Step 2: Collect all cards from waste pile
//         while (!wastePile.isEmpty()) {
//             Card c = wastePile.removeCard();
//             c.flip(); // Make face-down
//             allHiddenCards.push_back(c);
//         }
        
//         // Step 3: Shuffle all hidden cards
//         auto seed = chrono::system_clock::now().time_since_epoch().count();
//         shuffle(allHiddenCards.begin(), allHiddenCards.end(), 
//                     default_random_engine(seed));
        
//         // Step 4: Calculate how many cards to put in stock (same as waste pile had)
//         int stockCount = min(24, (int)allHiddenCards.size());
        
//         // Step 5: Put cards back into stock
//         for (int i = 0; i < stockCount; ++i) {
//             stockPile.addCard(allHiddenCards[i]);
//         }
        
//         // Step 6: Rebuild tableaus with face-up cards and remaining hidden cards
//         int hiddenIndex = stockCount;
//         for (int i = 0; i < 7; ++i) {
//             // Clear the tableau
//             while (!tableauPiles[i].isEmpty()) {
//                 tableauPiles[i].removeCard();
//             }
            
//             // Add hidden cards back (distribute remaining hidden cards)
//             int hiddenCardsNeeded = 0;
//             if (hiddenIndex < allHiddenCards.size()) {
//                 // Distribute remaining hidden cards across tableaus
//                 int remainingHidden = allHiddenCards.size() - hiddenIndex;
//                 int remainingTableaus = 7 - i;
//                 hiddenCardsNeeded = (remainingHidden + remainingTableaus - 1) / remainingTableaus;
//                 hiddenCardsNeeded = min(hiddenCardsNeeded, remainingHidden);
//             }
            
//             for (int j = 0; j < hiddenCardsNeeded; ++j) {
//                 if (hiddenIndex < allHiddenCards.size()) {
//                     tableauPiles[i].addCard(allHiddenCards[hiddenIndex++]);
//                 }
//             }
            
//             // Add face-up cards back
//             for (const auto& card : tableauFaceUpCards[i]) {
//                 tableauPiles[i].addCard(card);
//             }
//         }
        
//         cout << "Stock recycled with " << stockCount 
//                   << " cards. Remaining hidden cards redistributed in tableaus." << endl;
//     }

// public:
//     GameLogic() : foundationPiles{
//         FoundationPile(),
//         FoundationPile(),
//         FoundationPile(),
//         FoundationPile()
//     }, moveCount(0) {
//         initializeGame();
//     }

//     void initializeGame() {
//         deck.initializeDeck();
//         deck.shuffleDeck();

//         // Initialize 7 Tableaus
//         for (int i = 0; i < 7; i++) {
//             for (int j = i; j < 7; j++) {
//                 Card card = deck.dealCard();
//                 // Flip Up the top card of each tableau
//                 if (i == j) card.flip();
//                 tableauPiles[j].addCard(card);
//             }
//         }

//         // Move remaining deck cards to stockPile
//         while(!deck.isEmpty()) {
//             stockPile.addCard(deck.dealCard());
//         }
//     }

//     void drawCard() {
//         if (!stockPile.isEmpty()) {
//             Card card = stockPile.removeCard();
//             card.flip();  // Ensure the card is face up
//             wastePile.addCard(card);
//             moveCount++;
//         } 
//         else if (!wastePile.isEmpty()) {
//             // Recycle and shuffle
//             recycleWasteToStock();
//         } 
//         else {
//             cerr << "No cards left to draw!" << endl;
//         }
//     }

//     // indexes => StockPile: 0, WastePile: 1, Tableaus: 2 to 8, Foundations: 9 to 12
//     bool moveCard(int sourcePile, int destinationPile, int cardCount = 1) {
//         try {
//             // Stock to waste (draw)
//             if (sourcePile == 0 && destinationPile == 1) {
//                 drawCard();
//                 return true;
//             }

//             // Tableau to tableau (Multiple cards)
//             if (sourcePile >= 2 && sourcePile <= 8 && 
//                 destinationPile >= 2 && destinationPile <= 8 && 
//                 cardCount > 1) {
//                 return moveMultipleCards(sourcePile - 2, destinationPile - 2, cardCount);
//             }

//             // Waste to tableau
//             if (sourcePile == 1 && destinationPile >= 2 && destinationPile <= 8) {
//                 if (wastePile.isEmpty()) return false;
//                 Card card = wastePile.topCard();
//                 int tableauIndex = destinationPile - 2;
                
//                 if (tableauPiles[tableauIndex].isEmpty() && card.getRank() == Card::King) {
//                     tableauPiles[tableauIndex].addCard(wastePile.removeCard());
//                     moveCount++;
//                     return true;
//                 }
                
//                 if (!tableauPiles[tableauIndex].isEmpty() && 
//                     isValidTableauMove(card, tableauPiles[tableauIndex].topCard())) {
//                     tableauPiles[tableauIndex].addCard(wastePile.removeCard());
//                     moveCount++;
//                     return true;
//                 }
//             }

//             // Waste to foundation
//             if (sourcePile == 1 && destinationPile >= 9 && destinationPile <= 12) {
//                 if (wastePile.isEmpty()) return false;
//                 Card card = wastePile.topCard();
//                 if(foundationPiles[destinationPile - 9].addCard(card)) {
//                     wastePile.removeCard();
//                     moveCount++;
//                     return true;
//                 } else {
//                     return false;
//                 }
//             }

//             // Foundation to tableau
//             if (sourcePile >= 9 && sourcePile <= 12 && destinationPile >= 2 && destinationPile <= 8) {
//                 int foundationIndex = sourcePile - 9;
//                 if (foundationPiles[foundationIndex].isEmpty()) return false;
//                 Card card = foundationPiles[foundationIndex].topCard();
//                 int tableauIndex = destinationPile - 2;
//                 if (tableauPiles[tableauIndex].isEmpty() && card.getRank() == Card::King) {
//                     tableauPiles[tableauIndex].addCard(foundationPiles[foundationIndex].removeCard());
//                     moveCount++;
//                     return true;
//                 }
//                 if (!tableauPiles[tableauIndex].isEmpty() && 
//                     isValidTableauMove(card, tableauPiles[tableauIndex].topCard())) {
//                     tableauPiles[tableauIndex].addCard(foundationPiles[foundationIndex].removeCard());
//                     moveCount++;
//                     return true;
//                 }
//             }

//             // Tableau to tableau
//             if (sourcePile >= 2 && sourcePile <= 8 && destinationPile >= 2 && destinationPile <= 8) {
//                 int sourceIndex = sourcePile - 2;
//                 int destIndex = destinationPile - 2;
                
//                 if (tableauPiles[sourceIndex].isEmpty()) return false;
//                 Card card = tableauPiles[sourceIndex].topCard();
                
//                 if (tableauPiles[destIndex].isEmpty() && card.getRank() == Card::King) {
//                     tableauPiles[destIndex].addCard(tableauPiles[sourceIndex].removeCard());
//                     moveCount++;
//                     return true;
//                 }
                
//                 if (!tableauPiles[destIndex].isEmpty() && 
//                     isValidTableauMove(card, tableauPiles[destIndex].topCard())) {
//                     tableauPiles[destIndex].addCard(tableauPiles[sourceIndex].removeCard());
//                     moveCount++;
//                     return true;
//                 }
//             }

//             // Tableau to foundation
//             if (sourcePile >= 2 && sourcePile <= 8 && destinationPile >= 9 && destinationPile <= 12) {
//                 int tableauIndex = sourcePile - 2;
//                 if (tableauPiles[tableauIndex].isEmpty()) return false;
//                 Card card = tableauPiles[tableauIndex].topCard();
//                 if (foundationPiles[destinationPile - 9].addCard(card)) {
//                     tableauPiles[tableauIndex].removeCard();
//                     moveCount++;
//                     return true;
//                 } else {
//                     return false;
//                 }
//             }

//             return false;
//         }
//         catch (const exception& e) {
//             return false;
//         }
//     }

//     bool isGameWon() const {
//         // Check if all foundation piles have 13 cards
//         for (const auto& pile : foundationPiles) {
//             if (pile.isEmpty() || pile.getSize() != 13) return false;
//         }
//         return true;
//     }

//     void printPersistentInstructions(int x, int y) const {
//         setCursorPosition(x, y);
//         cout << "=> Press D to deselect the card";
//         setCursorPosition(x, y + 1);
//         cout << "=> Press S to switch between Dark and Light mode";
//         setCursorPosition(x, y + 2);
//         cout << "=> Press ESC to exit the game";
//     }

//     void displayGame() const {
//         setCursorPosition(0, 3);
//         cout << "Stock Pile =>";
//         stockPile.displayTopCard();
//         setCursorPosition(30, 3);
//         cout << "<= Waste Pile\n";
//         wastePile.displayTopCard();
    
//         setCursorPosition(65, 0);
//         cout << "Foundations" << endl;
//         for (int i = 0; i < foundationPiles.size(); ++i) {
//             foundationPiles[i].displayTopCard(50+(10*i),1);
//             cout << "\n";
//         }

//         setCursorPosition(30, 8);
//         cout << "Tableaus" << endl;
//         int currentX = -7;
//         int currentY = 9;
//         for (int i = 0; i < tableauPiles.size(); ++i) {
//             setCursorPosition(currentX+=10, currentY);
//             cout << i + 1;
//             tableauPiles[i].displayTopCard(10*i,10);
//         }

//         setCursorPosition(95, 2);
//         cout << "Move Count: " << moveCount;
//         printPersistentInstructions(80, 12);
//     }

//     string displayHint() {
//         vector<string> hints;
//         try {
//             if (!getWastePile().isEmpty()) {
//                 const Card wasteCard = getWastePile().topCard();
//                 for (int i = 0; i < 4; ++i) {
//                     if (foundationPiles[i].isValidMove(wasteCard)) {
//                         hints.push_back("Hint: Move card from Waste to Foundation " + to_string(i + 1));
//                     }
//                 }
//             }

//             if (!getWastePile().isEmpty()) {
//                 const Card wasteCard = getWastePile().topCard();
//                 for (int i = 0; i < 7; ++i) {
//                     const Card destCard = tableauPiles[i].topCard();
//                     if (isValidTableauMove(wasteCard, destCard)) {
//                         hints.push_back("Hint: Move card from Waste to Tableau " + to_string(i + 1));
//                     }
//                 }
//             }

//             for (int i = 0; i < 7; ++i) {
//                 if (!tableauPiles[i].isEmpty()) {
//                     const Card sourceCard = tableauPiles[i].topCard();
//                     for (int j = 0; j < 7; ++j) {
//                         const Card destCard = tableauPiles[j].topCard();
//                         if (isValidTableauMove(sourceCard, destCard)) {
//                             hints.push_back("Hint: Move card from Tableau " + to_string(i + 1) + " to Tableau " + to_string(j + 1));
//                         }
//                     }
//                 }
//             }

//             if (!getStockPile().isEmpty()) {
//                 hints.push_back("Hint: Move card from Stock to Waste");
//             }

//             // return random hint from hint vector
//             if (hints.empty()) {
//                 return "No hints available";
//             }

//             random_device rd;
//             mt19937 gen(rd());
//             uniform_int_distribution<> distr(0, hints.size() - 1);
//             return hints[distr(gen)];

//         } catch (const exception& e) { 
//             return "Hint: Move card from Stock to Waste";
//         }
//     }

//     const StockPile& getStockPile() const { return stockPile; }

//     const WastePile& getWastePile() const { return wastePile; }

//     const array<FoundationPile, 4>& getFoundationPiles() const { return foundationPiles; }

//     const array<TableauPile, 7>& getTableauPiles() const { return tableauPiles; }
// };


















#pragma once

#include <chrono>
#include <algorithm>
#include <vector>
#include <iostream>
#include <random>
#include <array>
#include "Deck.h"
#include "StockPile.h"
#include "WastePile.h"
#include "FoundationPile.h"
#include "TableauPile.h"
using namespace std;


class GameLogic {
private:
    Deck deck;
    StockPile stockPile;
    WastePile wastePile;
    array<FoundationPile, 4> foundationPiles;
    array<TableauPile, 7> tableauPiles;
    int moveCount;

    bool isValidTableauMove(const Card& card, const Card& target) {
        // Different color (red/black) and one rank lower
        bool isDifferentColor = ((card.getSuit() == Card::Heart || card.getSuit() == Card::Diamond) !=
                               (target.getSuit() == Card::Heart || target.getSuit() == Card::Diamond));
        return isDifferentColor && (card.getRank() == target.getRank() - 1);
    }

    bool isValidTableauSequence(const vector<Card>& cards) {
        for (size_t i = 0; i < cards.size() - 1; i++) {
            if (!isValidTableauMove(cards[i+1], cards[i])) {
                return false;
            }
        }
        return true;
    }

    bool moveMultipleCards(int sourceIndex, int destIndex, int cardCount) {
        try {
            if (cardCount > tableauPiles[sourceIndex].getSize()) {
                return false;
            }

            vector<Card> cardsToMove = tableauPiles[sourceIndex].peekCards(cardCount);

            if (!isValidTableauSequence(cardsToMove)) {
                return false;
            }

            if (!tableauPiles[destIndex].isEmpty()) {
                if (!isValidTableauMove(cardsToMove[0], tableauPiles[destIndex].topCard())) {
                    return false;
                }
            } else if (cardsToMove[0].getRank() != Card::King) {
                return false;
            }

            cardsToMove = tableauPiles[sourceIndex].removeCards(cardCount);
            tableauPiles[destIndex].addCards(cardsToMove);
            moveCount++;
            return true;
        }
        catch (const exception&) {
            cerr << "Invalid move" << endl;
            return false;
        }
    }

    void recycleWasteToStock() {
        vector<Card> allHiddenCards;
        vector<vector<Card>> tableauStructure(7);
        vector<int> hiddenCountPerTableau(7, 0);
        
        // Step 1: Extract structure of each tableau (preserve positions)
        for (int i = 0; i < 7; ++i) {
            Stack tempStack = tableauPiles[i].getCards();
            vector<Card> allCards;
            
            while (!tempStack.isEmpty()) {
                allCards.push_back(tempStack.pop());
            }
            
            reverse(allCards.begin(), allCards.end());
            tableauStructure[i] = allCards;
            
            // Count hidden cards in this tableau
            for (const auto& card : allCards) {
                if (!card.isFaceUp()) {
                    hiddenCountPerTableau[i]++;
                    allHiddenCards.push_back(card);
                } else {
                    break;
                }
            }
        }
        
        // Step 2: Collect waste pile cards
        int wasteCount = wastePile.getSize();
        while (!wastePile.isEmpty()) {
            Card c = wastePile.removeCard();
            c.flip();
            allHiddenCards.push_back(c);
        }
        
        // Step 3: Shuffle all hidden cards
        auto seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(allHiddenCards.begin(), allHiddenCards.end(), 
                    default_random_engine(seed));
        
        // Step 4: Put same number of cards back to stock
        int cardIndex = 0;
        for (int i = 0; i < wasteCount && cardIndex < allHiddenCards.size(); ++i) {
            stockPile.addCard(allHiddenCards[cardIndex++]);
        }
        
        // Step 5: Rebuild tableaus with same structure
        for (int i = 0; i < 7; ++i) {
            while (!tableauPiles[i].isEmpty()) {
                tableauPiles[i].removeCard();
            }
            
            // Add hidden cards (same count as before)
            for (int j = 0; j < hiddenCountPerTableau[i] && cardIndex < allHiddenCards.size(); ++j) {
                tableauPiles[i].addCard(allHiddenCards[cardIndex++]);
            }
            
            // Add face-up cards back (same as before)
            for (size_t j = hiddenCountPerTableau[i]; j < tableauStructure[i].size(); ++j) {
                tableauPiles[i].addCard(tableauStructure[i][j]);
            }
        }
        
        cout << "Stock recycled and shuffled!" << endl;
    }

public:
    GameLogic() : foundationPiles{
        FoundationPile(),
        FoundationPile(),
        FoundationPile(),
        FoundationPile()
    }, moveCount(0) {
        initializeGame();
    }

    void initializeGame() {
        deck.initializeDeck();
        deck.shuffleDeck();

        // Initialize 7 Tableaus
        for (int i = 0; i < 7; i++) {
            for (int j = i; j < 7; j++) {
                Card card = deck.dealCard();
                // Flip Up the top card of each tableau
                if (i == j) card.flip();
                tableauPiles[j].addCard(card);
            }
        }

        // Move remaining deck cards to stockPile
        while(!deck.isEmpty()) {
            stockPile.addCard(deck.dealCard());
        }
    }

    void drawCard() {
        if (!stockPile.isEmpty()) {
            Card card = stockPile.removeCard();
            card.flip();
            wastePile.addCard(card);
            moveCount++;
        } 
        else if (!wastePile.isEmpty()) {
            recycleWasteToStock();
        } 
        else {
            cerr << "No cards left to draw!" << endl;
        }
    }

    // indexes => StockPile: 0, WastePile: 1, Tableaus: 2 to 8, Foundations: 9 to 12
    bool moveCard(int sourcePile, int destinationPile, int cardCount = 1) {
        try {
            // Stock to waste (draw)
            if (sourcePile == 0 && destinationPile == 1) {
                drawCard();
                return true;
            }

            // Tableau to tableau (Multiple cards)
            if (sourcePile >= 2 && sourcePile <= 8 && 
                destinationPile >= 2 && destinationPile <= 8 && 
                cardCount > 1) {
                return moveMultipleCards(sourcePile - 2, destinationPile - 2, cardCount);
            }

            // Waste to tableau
            if (sourcePile == 1 && destinationPile >= 2 && destinationPile <= 8) {
                if (wastePile.isEmpty()) return false;
                Card card = wastePile.topCard();
                int tableauIndex = destinationPile - 2;
                
                if (tableauPiles[tableauIndex].isEmpty() && card.getRank() == Card::King) {
                    tableauPiles[tableauIndex].addCard(wastePile.removeCard());
                    moveCount++;
                    return true;
                }
                
                if (!tableauPiles[tableauIndex].isEmpty() && 
                    isValidTableauMove(card, tableauPiles[tableauIndex].topCard())) {
                    tableauPiles[tableauIndex].addCard(wastePile.removeCard());
                    moveCount++;
                    return true;
                }
            }

            // Waste to foundation
            if (sourcePile == 1 && destinationPile >= 9 && destinationPile <= 12) {
                if (wastePile.isEmpty()) return false;
                Card card = wastePile.topCard();
                if(foundationPiles[destinationPile - 9].addCard(card)) {
                    wastePile.removeCard();
                    moveCount++;
                    return true;
                } else {
                    return false;
                }
            }

            // Foundation to tableau
            if (sourcePile >= 9 && sourcePile <= 12 && destinationPile >= 2 && destinationPile <= 8) {
                int foundationIndex = sourcePile - 9;
                if (foundationPiles[foundationIndex].isEmpty()) return false;
                Card card = foundationPiles[foundationIndex].topCard();
                int tableauIndex = destinationPile - 2;
                if (tableauPiles[tableauIndex].isEmpty() && card.getRank() == Card::King) {
                    tableauPiles[tableauIndex].addCard(foundationPiles[foundationIndex].removeCard());
                    moveCount++;
                    return true;
                }
                if (!tableauPiles[tableauIndex].isEmpty() && 
                    isValidTableauMove(card, tableauPiles[tableauIndex].topCard())) {
                    tableauPiles[tableauIndex].addCard(foundationPiles[foundationIndex].removeCard());
                    moveCount++;
                    return true;
                }
            }

            // Tableau to tableau
            if (sourcePile >= 2 && sourcePile <= 8 && destinationPile >= 2 && destinationPile <= 8) {
                int sourceIndex = sourcePile - 2;
                int destIndex = destinationPile - 2;
                
                if (tableauPiles[sourceIndex].isEmpty()) return false;
                Card card = tableauPiles[sourceIndex].topCard();
                
                if (tableauPiles[destIndex].isEmpty() && card.getRank() == Card::King) {
                    tableauPiles[destIndex].addCard(tableauPiles[sourceIndex].removeCard());
                    moveCount++;
                    return true;
                }
                
                if (!tableauPiles[destIndex].isEmpty() && 
                    isValidTableauMove(card, tableauPiles[destIndex].topCard())) {
                    tableauPiles[destIndex].addCard(tableauPiles[sourceIndex].removeCard());
                    moveCount++;
                    return true;
                }
            }

            // Tableau to foundation
            if (sourcePile >= 2 && sourcePile <= 8 && destinationPile >= 9 && destinationPile <= 12) {
                int tableauIndex = sourcePile - 2;
                if (tableauPiles[tableauIndex].isEmpty()) return false;
                Card card = tableauPiles[tableauIndex].topCard();
                if (foundationPiles[destinationPile - 9].addCard(card)) {
                    tableauPiles[tableauIndex].removeCard();
                    moveCount++;
                    return true;
                } else {
                    return false;
                }
            }

            return false;
        }
        catch (const exception& e) {
            return false;
        }
    }

    bool isGameWon() const {
        // Check if all 4 foundation piles have 13 cards each (A to K)
        for (const auto& pile : foundationPiles) {
            if (pile.isEmpty() || pile.getSize() != 13) return false;
        }
        return true;
    }

    void printPersistentInstructions(int x, int y) const {
        setCursorPosition(x, y);
        cout << "=> Press D to deselect the card";
        setCursorPosition(x, y + 1);
        cout << "=> Press S to switch between Dark and Light mode";
        setCursorPosition(x, y + 2);
        cout << "=> Press ESC to exit the game";
    }

    void displayGame() const {
        setCursorPosition(0, 3);
        cout << "Stock Pile =>";
        stockPile.displayTopCard();
        setCursorPosition(30, 3);
        cout << "<= Waste Pile\n";
        wastePile.displayTopCard();
    
        setCursorPosition(65, 0);
        cout << "Foundations" << endl;
        for (int i = 0; i < foundationPiles.size(); ++i) {
            foundationPiles[i].displayTopCard(50+(10*i),1);
            cout << "\n";
        }

        setCursorPosition(30, 8);
        cout << "Tableaus" << endl;
        int currentX = -7;
        int currentY = 9;
        for (int i = 0; i < tableauPiles.size(); ++i) {
            setCursorPosition(currentX+=10, currentY);
            cout << i + 1;
            tableauPiles[i].displayTopCard(10*i,10);
        }

        setCursorPosition(95, 2);
        cout << "Move Count: " << moveCount;
        printPersistentInstructions(80, 12);
    }

    string displayHint() {
        vector<string> hints;
        try {
            if (!getWastePile().isEmpty()) {
                const Card wasteCard = getWastePile().topCard();
                for (int i = 0; i < 4; ++i) {
                    if (foundationPiles[i].isValidMove(wasteCard)) {
                        hints.push_back("Hint: Move card from Waste to Foundation " + to_string(i + 1));
                    }
                }
            }

            if (!getWastePile().isEmpty()) {
                const Card wasteCard = getWastePile().topCard();
                for (int i = 0; i < 7; ++i) {
                    const Card destCard = tableauPiles[i].topCard();
                    if (isValidTableauMove(wasteCard, destCard)) {
                        hints.push_back("Hint: Move card from Waste to Tableau " + to_string(i + 1));
                    }
                }
            }

            for (int i = 0; i < 7; ++i) {
                if (!tableauPiles[i].isEmpty()) {
                    const Card sourceCard = tableauPiles[i].topCard();
                    for (int j = 0; j < 7; ++j) {
                        const Card destCard = tableauPiles[j].topCard();
                        if (isValidTableauMove(sourceCard, destCard)) {
                            hints.push_back("Hint: Move card from Tableau " + to_string(i + 1) + " to Tableau " + to_string(j + 1));
                        }
                    }
                }
            }

            if (!getStockPile().isEmpty()) {
                hints.push_back("Hint: Move card from Stock to Waste");
            }

            // return random hint from hint vector
            if (hints.empty()) {
                return "No hints available";
            }

            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> distr(0, hints.size() - 1);
            return hints[distr(gen)];

        } catch (const exception& e) { 
            return "Hint: Move card from Stock to Waste";
        }
    }

    const StockPile& getStockPile() const { return stockPile; }

    const WastePile& getWastePile() const { return wastePile; }

    const array<FoundationPile, 4>& getFoundationPiles() const { return foundationPiles; }

    const array<TableauPile, 7>& getTableauPiles() const { return tableauPiles; }
};