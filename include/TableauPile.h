#pragma once

#include "Pile.h"
#include "Stack.h"
#include "Card.h"
#include <vector>
using namespace std;

class TableauPile : public Pile {
private:
    Stack cards;

public:
    bool addCard(const Card& card) override {
        cards.push(card);
        return true;
    }

    Card removeCard() override {
        Card cardToReturn = cards.pop();
        if (!cards.isEmpty()) {
            if (!cards.peek().isFaceUp()) {
                Card temp = cards.pop();
                temp.flip();
                cards.push(temp);
            }
        }
        return cardToReturn;
    }

    Card topCard() const override {
        return cards.peek();
    }

    bool isEmpty() const override {
        return cards.isEmpty();
    }

    int getSize() const {
        return cards.getSize();
    }

    vector<Card> removeCards(int count) {
        if (count > cards.getSize()) {
            throw runtime_error("Not enough cards");
        }

        vector<Card> removedCards;
        Stack tempStack;
        // Remove cards and store them
        for (int i = 0; i < count; i++) {
            Card card = cards.pop();
            removedCards.push_back(card);
            tempStack.push(card);
        }

        // Flip the next card in the tableau
        if (!cards.isEmpty()) {
            if (!cards.peek().isFaceUp()) {
                Card temp = cards.pop();
                temp.flip();
                cards.push(temp);
            }
        }

        // Reverse order for proper sequence
        reverse(removedCards.begin(), removedCards.end());
        return removedCards;
    }

    void addCards(const vector<Card>& newCards) {
        for (const Card& card : newCards) {
            addCard(card);
        }
    }

    vector<Card> peekCards(int count) const {
        if (count > cards.getSize()) {
            throw runtime_error("Not enough cards");
        }

        vector<Card> visibleCards;
        Stack tempStack = cards;
        for (int i = 0; i < count; i++) {
            visibleCards.push_back(tempStack.pop());
        }
        reverse(visibleCards.begin(), visibleCards.end());
        return visibleCards;
    }

    void displayTopCard(int x,int y) const {
        if (!cards.isEmpty()) {
            Stack tempStack = cards;
            vector<Card> cardList;

            while (!tempStack.isEmpty()) {
                cardList.push_back(tempStack.pop());
            }

            bool foundFaceUp = false;

            // Display the cards in the tableau, handling face-down cards   
            const int originalSize = cardList.size();
            WORD oldColor=0;
            for (size_t i = cardList.size(); i > 0; --i) {
                if (cardList[i - 1].isFaceUp()) {
                oldColor = PrintCardBox(cardList[i - 1].getSuit(), cardList[i - 1].getRank(),x,y+((originalSize-i)*2), originalSize - i,oldColor);
                    foundFaceUp = true;
                } else {
                oldColor = PrintCardBox(Card::Suit::Club, Card::Rank::Flipped, x, y + ((originalSize - i)*2 ),originalSize-i,oldColor);
                }
            }
        }
    }

    const Stack& getCards() const {
        return cards;
    }
};