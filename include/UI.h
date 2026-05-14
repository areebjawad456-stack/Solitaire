// #pragma once

// #include <iostream>
// #include <windows.h>
// #include <string>
// #include <algorithm>
// #include <queue>
// #include "GameLogic.h"
// #include "Card.h"
// using namespace std;

// #define printSelectedCard_X 100
// #define printSelectedCard_Y 17


// void setConsoleSettings() {
//     SetConsoleOutputCP(CP_UTF8);

//     HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//     CONSOLE_SCREEN_BUFFER_INFO csbi;
//     if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
//         COORD newSize;
//         newSize.X = csbi.dwSize.X;
//         newSize.Y = 1500;

//         if (SetConsoleScreenBufferSize(hConsole, newSize)) {
//             cout << "Console buffer size set to 1500 rows (scrollable)." << endl;
//         }
//         else {
//             cerr << "Failed to set console buffer size." << endl;
//         }

//         SMALL_RECT newWindow;
//         newWindow.Top = csbi.srWindow.Top;
//         newWindow.Left = csbi.srWindow.Left;
//         newWindow.Right = csbi.srWindow.Left + 200;
//         newWindow.Bottom = csbi.srWindow.Top + 3000;

//         if (SetConsoleWindowInfo(hConsole, TRUE, &newWindow)) {
//             cout << "Console window size set to 80x20 and is fixed." << endl;
//         }
//         else {
//             cerr << "Failed to set console window size." << endl;
//         }
//     }
//     else {
//         cerr << "Error getting console screen buffer info!" << endl;
//     }
// }

// void hideCursor() {
//     try {
//         CONSOLE_CURSOR_INFO cursorInfo;
//         cursorInfo.bVisible = false;
//         cursorInfo.dwSize = 1;
//         SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
//     } catch (const exception& e) {
//         cerr << "Exception in hideCursor: " << e.what() << endl;
//     }
// }

// void printDataAtXY(int x, int y, const string& data) {
//     setCursorPosition(x, y);
//     cout << string(60, ' ');
//     setCursorPosition(x, y);
//     cout << "=> " << data;
// }

// void clearOutputArea(int x, int y, int lines) {
//     for (int i = 0; i < lines; ++i) {
//         setCursorPosition(x, y + i);
//         cout << string(55, ' ');
//     }
// }

// void updatePositions(GameLogic& game, vector<vector<vector<int>>>& TableauPositions, vector<vector<int>>& FoundationPositions, vector<vector<int>>& AdditionalPilesPositions) {
//     int Y_START = 245;
//     int X_START = 12;
//     for (int i = 0; i < 7; ++i) {
//         Y_START = 245;
//         X_START = 12 + i * (58 + 38);
//         int X_END = X_START + 58;
//         TableauPositions[i].clear();
//         if (game.getTableauPiles()[i].getCards().isEmpty()) {
//             TableauPositions[i].push_back({X_START, X_END, Y_START, Y_START + 160});
//             continue;
//         }
//         for (int j = 0; j < game.getTableauPiles()[i].getCards().getSize(); ++j) {
//             if (j == game.getTableauPiles()[i].getCards().getSize() - 1) {
//                 TableauPositions[i].push_back({X_START, X_END, Y_START, Y_START + 80});
//                 Y_START += 80;
//             } else {
//                 TableauPositions[i].push_back({X_START, X_END, Y_START, Y_START + 38});
//                 Y_START += 38;
//             }
//         }
//     }
//     AdditionalPilesPositions[0] = {145, 205, 78, 160};
//     AdditionalPilesPositions[1] = {222, 280, 78, 160};

//     int Start_X = 492;
//     int Common_X_DIFF = 58;
//     int COMMON_PILE_TO_PILE_X_DIFF = 38;
//     int Start_Y = 78;
//     int END_Y = 160;
//     for (int i = 0; i < 4; ++i) {
//         FoundationPositions[i] = {Start_X, Start_X + Common_X_DIFF, Start_Y, END_Y};
//         Start_X += Common_X_DIFF + COMMON_PILE_TO_PILE_X_DIFF;
//     }
// }

// void handleMouseClick(GameLogic& game, POINT pt, vector<vector<vector<int>>>& TableauPositions, vector<vector<int>>& FoundationPositions, vector<vector<int>>& AdditionalPilesPositions, bool& isCardSelected, Card& selectedCard, int& selectedTableau, int& selectedCardIndex, int& selectedCardCount, bool& isWasteSelected, bool& isFoundationSelected, int& selectedFoundation) {
//     if (!isCardSelected && !isWasteSelected && !isFoundationSelected) {
//         for (int i = 0; i < 7; ++i) {
//             Stack cards = game.getTableauPiles()[i].getCards();
//             int cardCount = cards.getSize();
//             vector<Card> tempCards;

//             for (int j = 0; j < cardCount; ++j) {
//                 if (!cards.isEmpty()) {
//                     tempCards.push_back(cards.pop());
//                 } else {
//                     cerr << "Error: Attempted to pop from an empty stack." << endl;
//                     break;
//                 }
//             }

//             for (int j = cardCount - 1; j >= 0; --j) {
//                 const auto& card = tempCards[j];
//                 int posIndex = cardCount - 1 - j;
//                 if (posIndex < TableauPositions[i].size() && pt.x >= TableauPositions[i][posIndex][0] && pt.x <= TableauPositions[i][posIndex][1] && pt.y >= TableauPositions[i][posIndex][2] && pt.y <= TableauPositions[i][posIndex][3] && card.isFaceUp()) {
//                     isCardSelected = true;
//                     selectedCard = card;
//                     selectedTableau = i;
//                     selectedCardIndex = posIndex;
//                     selectedCardCount = cardCount - posIndex;
//                     clearOutputArea(80, 15, 15);
//                     printDataAtXY(80, 20, "Selected Card: ");
//                     PrintCardBox(card.getSuit(), card.getRank(), printSelectedCard_X, printSelectedCard_Y);
//                     break;
//                 }
//             }
//             if (isCardSelected) break;
//         }

//         if (!isCardSelected) {
//             const auto& wastePile = game.getWastePile();
//             if (!wastePile.isEmpty() && pt.x >= AdditionalPilesPositions[1][0] && pt.x <= AdditionalPilesPositions[1][1] && pt.y >= AdditionalPilesPositions[1][2] && pt.y <= AdditionalPilesPositions[1][3]) {
//                 selectedCard = wastePile.topCard();
//                 isWasteSelected = true;
//                 clearOutputArea(80, 15, 15);
//                 printDataAtXY(80, 20, "Selected Card: ");
//                 PrintCardBox(selectedCard.getSuit(), selectedCard.getRank(), printSelectedCard_X, printSelectedCard_Y);
//             }
//         }

//         if (!isCardSelected && !isWasteSelected) {
//             for (int i = 0; i < 4; ++i) {
//                 if (pt.x >= FoundationPositions[i][0] && pt.x <= FoundationPositions[i][1] && pt.y >= FoundationPositions[i][2] && pt.y <= FoundationPositions[i][3]) {
//                     const auto& foundationPile = game.getFoundationPiles()[i];
//                     if (!foundationPile.isEmpty()) {
//                         selectedCard = foundationPile.topCard();
//                         isFoundationSelected = true;
//                         selectedFoundation = i;
//                         clearOutputArea(80, 15, 15);
//                         printDataAtXY(80, 20, "Selected Card: ");
//                         PrintCardBox(selectedCard.getSuit(), selectedCard.getRank(), printSelectedCard_X, printSelectedCard_Y);
//                     }
//                 }
//             }
//         }

//         // Check for stock pile click
//         if (!isCardSelected && !isWasteSelected && !isFoundationSelected) {
//             if (pt.x >= AdditionalPilesPositions[0][0] && pt.x <= AdditionalPilesPositions[0][1] && pt.y >= AdditionalPilesPositions[0][2] && pt.y <= AdditionalPilesPositions[0][3]) {
//                 game.drawCard();
//                 updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
//                 system("cls");
//                 game.displayGame();
//             }
//         }
//     }
// }

// void handleCardMove(GameLogic& game, POINT pt, vector<vector<vector<int>>>& TableauPositions, vector<vector<int>>& FoundationPositions, vector<vector<int>>& AdditionalPilesPositions, bool& isCardSelected, Card& selectedCard, int& selectedTableau, int& selectedCardCount, bool& isWasteSelected, bool& isFoundationSelected, int& selectedFoundation) {
//     if (isCardSelected) {
//         for (int i = 0; i < 7; ++i) {
//             if (i != selectedTableau) {
//                 int lastIndex = TableauPositions[i].size() - 1;
//                 if (pt.x >= TableauPositions[i][lastIndex][0] && pt.x <= TableauPositions[i][lastIndex][1] && pt.y >= TableauPositions[i][lastIndex][2] && pt.y <= TableauPositions[i][lastIndex][3]) {
//                     bool move = game.moveCard(selectedTableau + 2, i + 2, selectedCardCount);
//                     if (!move) {
//                         clearOutputArea(80, 15, 15);
//                         printDataAtXY(80, 20, "Invalid Move!");
//                         isCardSelected = false;
//                     } else {
//                         isCardSelected = false;
//                         updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
//                         system("cls");
//                         game.displayGame();
//                         break;
//                     }
//                 }
//             }
//         }
//         for (int i = 0; i < 4; ++i) {
//             if (pt.x >= FoundationPositions[i][0] && pt.x <= FoundationPositions[i][1] && pt.y >= FoundationPositions[i][2] && pt.y <= FoundationPositions[i][3]) {
//                 bool move = game.moveCard(selectedTableau + 2, i + 9, selectedCardCount);
//                 if (!move) {
//                     clearOutputArea(80, 15, 15);
//                     printDataAtXY(80, 20, "Invalid Move!");
//                     isCardSelected = false;
//                 } else {
//                     isCardSelected = false;
//                     updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
//                     system("cls");
//                     game.displayGame();
//                     break;
//                 }
//             }
//         }
//     } else if (isWasteSelected) {
//         for (int i = 0; i < 7; ++i) {
//             int lastIndex = TableauPositions[i].size() - 1;
//             if (pt.x >= TableauPositions[i][lastIndex][0] && pt.x <= TableauPositions[i][lastIndex][1] && pt.y >= TableauPositions[i][lastIndex][2] && pt.y <= TableauPositions[i][lastIndex][3]) {
//                 bool move = game.moveCard(1, i + 2);
//                 if (!move) {
//                     clearOutputArea(80, 15, 15);
//                     printDataAtXY(80, 20, "Invalid Move!");
//                     isWasteSelected = false;
//                 } else {
//                     isWasteSelected = false;
//                     updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
//                     system("cls");
//                     game.displayGame();
//                     break;
//                 }
//             }
//         }
//         for (int i = 0; i < 4; ++i) {
//             if (pt.x >= FoundationPositions[i][0] && pt.x <= FoundationPositions[i][1] && pt.y >= FoundationPositions[i][2] && pt.y <= FoundationPositions[i][3]) {
//                 bool move = game.moveCard(1, i + 9);
//                 if (!move) {
//                     clearOutputArea(80, 15, 15);
//                     printDataAtXY(80, 20, "Invalid Move!");
//                     isWasteSelected = false;
//                 } else {
//                     isWasteSelected = false;
//                     updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
//                     system("cls");
//                     game.displayGame();
//                     break;
//                 }
//             }
//         }
//     } else if (isFoundationSelected) {
//         for (int i = 0; i < 7; ++i) {
//             int lastIndex = TableauPositions[i].size() - 1;
//             if (pt.x >= TableauPositions[i][lastIndex][0] && pt.x <= TableauPositions[i][lastIndex][1] && pt.y >= TableauPositions[i][lastIndex][2] && pt.y <= TableauPositions[i][lastIndex][3]) {
//                 bool move = game.moveCard(selectedFoundation + 9, i + 2);
//                 if (!move) {
//                     clearOutputArea(80, 15, 15);
//                     printDataAtXY(80, 20, "Invalid Move!");
//                     isFoundationSelected = false;
//                 } else {
//                     isFoundationSelected = false;
//                     updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
//                     system("cls");
//                     game.displayGame();
//                     break;
//                 }
//             }
//         }
//     }
// }

// void handleDeselection(bool& isCardSelected, bool& isWasteSelected, bool& isFoundationSelected, int selectedTableau, int selectedFoundation) {
//     if (isCardSelected) {
//         isCardSelected = false;
//         clearOutputArea(80, 15, 15);
//         printDataAtXY(80, 20, "Card Deselected");
//     } else if (isWasteSelected) {
//         isWasteSelected = false;
//         clearOutputArea(80, 15, 15);
//         printDataAtXY(80, 20, "Card Deselected");
//     } else if (isFoundationSelected) {
//         isFoundationSelected = false;
//         clearOutputArea(80, 15, 15);
//         printDataAtXY(80, 20, "Card Deselected");
//     }
// }

// void handleModeSwitch(bool& isDarkMode, GameLogic& game) {
//     isDarkMode = !isDarkMode;
//     if (isDarkMode) {
//         system("color 0F"); // Black background, white text
//     } else {
//         system("color F0"); // White background, black text
//     }
//     system("cls");
//     game.displayGame();
//     clearOutputArea(80, 25, 1);
//     printDataAtXY(80, 25, "Switched to " + string(isDarkMode ? "Dark" : "Light") + " Mode");
//     Sleep(200);
// }

// void GameLoop() {
//     setConsoleSettings();
//     system("color F0");
//     system("cls");
//     fullsc();
    
//     bool isDarkMode = false;
//     try {
//         GameLogic game;
//         vector<vector<vector<int>>> TableauPositions(7);
//         vector<vector<int>> FoundationPositions(4);
//         vector<vector<int>> AdditionalPilesPositions(2);
//         chrono::steady_clock::time_point startTime = chrono::steady_clock::now();
//         hideCursor();

//         system("cls");
//         game.displayGame();

//         updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
//         while (true) {
//             try {
//                 int elapsedTime = chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - startTime).count();
//                 setCursorPosition(95, 4);
//                 cout << "Time Elapsed: " << elapsedTime << "s";

//                 SHORT leftButton = GetAsyncKeyState(VK_LBUTTON);
//                 SHORT rightButton = GetAsyncKeyState(VK_RBUTTON);

//                 POINT pt;
//                 GetCursorPos(&pt);

//                 static bool isCardSelected = false;
//                 static Card selectedCard;
//                 static int selectedTableau = -1;
//                 static int selectedCardIndex = -1;
//                 static int selectedCardCount = 1;
//                 static bool isWasteSelected = false;
//                 static bool isFoundationSelected = false;
//                 static int selectedFoundation = -1;

//                 if (leftButton & 0x8000) {
//                     handleMouseClick(game, pt, TableauPositions, FoundationPositions, AdditionalPilesPositions, isCardSelected, selectedCard, selectedTableau, selectedCardIndex, selectedCardCount, isWasteSelected, isFoundationSelected, selectedFoundation);
//                     handleCardMove(game, pt, TableauPositions, FoundationPositions, AdditionalPilesPositions, isCardSelected, selectedCard, selectedTableau, selectedCardCount, isWasteSelected, isFoundationSelected, selectedFoundation);
//                     Sleep(200);
//                 }

//                 if (GetAsyncKeyState('D') & 0x8000) {
//                     handleDeselection(isCardSelected, isWasteSelected, isFoundationSelected, selectedTableau, selectedFoundation);
//                 }

//                 if (GetAsyncKeyState('S') & 0x8000) {
//                     handleModeSwitch(isDarkMode, game);
//                 }

//                 if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
//                     break;
//                 }

//                 Sleep(100);
//             } catch (const exception& e) {
//                 cerr << "Exception in game loop: " << e.what() << endl;
//             }
//         }
//         system("cls");
//         game.displayGame();
//     } catch (const exception& e) {
//         cerr << "Exception in gameLoop: " << e.what() << endl;
//     }
// }













#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <algorithm>
#include <queue>
#include "GameLogic.h"
#include "Card.h"
using namespace std;

#define printSelectedCard_X 100
#define printSelectedCard_Y 17


void setConsoleSettings() {
    SetConsoleOutputCP(CP_UTF8);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        COORD newSize;
        newSize.X = csbi.dwSize.X;
        newSize.Y = 1500;

        if (SetConsoleScreenBufferSize(hConsole, newSize)) {
            cout << "Console buffer size set to 1500 rows (scrollable)." << endl;
        }
        else {
            cerr << "Failed to set console buffer size." << endl;
        }

        SMALL_RECT newWindow;
        newWindow.Top = csbi.srWindow.Top;
        newWindow.Left = csbi.srWindow.Left;
        newWindow.Right = csbi.srWindow.Left + 200;
        newWindow.Bottom = csbi.srWindow.Top + 3000;

        if (SetConsoleWindowInfo(hConsole, TRUE, &newWindow)) {
            cout << "Console window size set to 80x20 and is fixed." << endl;
        }
        else {
            cerr << "Failed to set console window size." << endl;
        }
    }
    else {
        cerr << "Error getting console screen buffer info!" << endl;
    }
}

void hideCursor() {
    try {
        CONSOLE_CURSOR_INFO cursorInfo;
        cursorInfo.bVisible = false;
        cursorInfo.dwSize = 1;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    } catch (const exception& e) {
        cerr << "Exception in hideCursor: " << e.what() << endl;
    }
}

void showLoader() {
    system("cls");
    hideCursor();
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Get console dimensions
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    
    int centerX = consoleWidth / 2 - 20;
    int centerY = consoleHeight / 2;
    
    // Title
    setCursorPosition(centerX - 5, centerY - 4);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "╔════════════════════════════════════════╗";
    setCursorPosition(centerX - 5, centerY - 3);
    cout << "║                                        ║";
    setCursorPosition(centerX - 5, centerY - 2);
    cout << "║          KLONDIKE SOLITAIRE           ║";
    setCursorPosition(centerX - 5, centerY - 1);
    cout << "║                                        ║";
    setCursorPosition(centerX - 5, centerY);
    cout << "╚════════════════════════════════════════╝";
    
    // Loading animation
    setCursorPosition(centerX + 5, centerY + 3);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "Loading";
    
    for (int i = 0; i < 6; ++i) {
        cout << ".";
        Sleep(300);
    }
    
    setCursorPosition(centerX + 2, centerY + 5);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "Game Starting...";
    Sleep(500);
    
    // Reset color
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void showWinMessage(int elapsedTime) {
    system("cls");
    hideCursor();
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    int centerX = 35;
    int centerY = 10;
    
    SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    
    setCursorPosition(centerX, centerY);
    cout << "╔════════════════════════════════════════════╗";
    setCursorPosition(centerX, centerY + 1);
    cout << "║                                            ║";
    setCursorPosition(centerX, centerY + 2);
    cout << "║         CONGRATULATIONS! YOU WON! 🎉      ║";
    setCursorPosition(centerX, centerY + 3);
    cout << "║                                            ║";
    setCursorPosition(centerX, centerY + 4);
    cout << "║   You have successfully completed the     ║";
    setCursorPosition(centerX, centerY + 5);
    cout << "║          Klondike Solitaire game!         ║";
    setCursorPosition(centerX, centerY + 6);
    cout << "║                                            ║";
    
    // Show completion time
    setCursorPosition(centerX, centerY + 7);
    string timeStr = "║      Time: " + to_string(elapsedTime) + " seconds";
    while (timeStr.length() < 47) timeStr += " ";
    timeStr += "║";
    cout << timeStr;
    
    setCursorPosition(centerX, centerY + 8);
    cout << "║                                            ║";
    setCursorPosition(centerX, centerY + 9);
    cout << "╚════════════════════════════════════════════╝";
    
    // Reset color
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    SetConsoleTextAttribute(hConsole, consoleInfo.wAttributes);
    
    setCursorPosition(centerX + 5, centerY + 12);
    cout << "Press any key to exit...";
    
    // Wait for any key press
    while (true) {
        for (int key = 0; key < 256; ++key) {
            if (GetAsyncKeyState(key) & 0x8000) {
                Sleep(200);
                return;
            }
        }
        Sleep(100);
    }
}

void showRules() {
    system("cls");
    hideCursor();
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    
    setCursorPosition(30, 2);
    cout << "╔══════════════════════════════════════════════════════════╗";
    setCursorPosition(30, 3);
    cout << "║              KLONDIKE SOLITAIRE - GAME RULES            ║";
    setCursorPosition(30, 4);
    cout << "╚══════════════════════════════════════════════════════════╝";
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    
    setCursorPosition(5, 6);
    cout << "OBJECTIVE:";
    setCursorPosition(5, 7);
    cout << "  • Move all cards to the four Foundation piles (top-right)";
    setCursorPosition(5, 8);
    cout << "  • Each Foundation must be built from Ace to King of the same suit";
    
    setCursorPosition(5, 10);
    cout << "TABLEAU RULES (Main Playing Area):";
    setCursorPosition(5, 11);
    cout << "  • Cards must be placed in descending order (K, Q, J, 10... 2, A)";
    setCursorPosition(5, 12);
    cout << "  • Cards must alternate colors (Red on Black, Black on Red)";
    setCursorPosition(5, 13);
    cout << "  • Only Kings can be placed on empty Tableau spaces";
    setCursorPosition(5, 14);
    cout << "  • You can move sequences of cards together";
    
    setCursorPosition(5, 16);
    cout << "STOCK AND WASTE PILE:";
    setCursorPosition(5, 17);
    cout << "  • Click Stock pile to draw cards to Waste pile";
    setCursorPosition(5, 18);
    cout << "  • When Stock is empty, cards from Waste are reshuffled back";
    setCursorPosition(5, 19);
    cout << "  • Hidden cards from Tableau are included in reshuffle";
    
    setCursorPosition(5, 21);
    cout << "FOUNDATION RULES:";
    setCursorPosition(5, 22);
    cout << "  • Must start with an Ace";
    setCursorPosition(5, 23);
    cout << "  • Build up in sequence (A, 2, 3, 4... Q, K)";
    setCursorPosition(5, 24);
    cout << "  • Only cards of the same suit can be added";
    
    setCursorPosition(5, 26);
    cout << "CONTROLS:";
    setCursorPosition(5, 27);
    cout << "  • LEFT CLICK - Select/Move cards";
    setCursorPosition(5, 28);
    cout << "  • D - Deselect card";
    setCursorPosition(5, 29);
    cout << "  • S - Switch between Dark and Light mode";
    setCursorPosition(5, 30);
    cout << "  • F - View/Hide game rules";
    setCursorPosition(5, 31);
    cout << "  • ESC - Exit game";
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    setCursorPosition(30, 34);
    cout << "Press F again to return to the game...";
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void printDataAtXY(int x, int y, const string& data) {
    setCursorPosition(x, y);
    cout << string(60, ' ');
    setCursorPosition(x, y);
    cout << "=> " << data;
}

void clearOutputArea(int x, int y, int lines) {
    for (int i = 0; i < lines; ++i) {
        setCursorPosition(x, y + i);
        cout << string(55, ' ');
    }
}

void updatePositions(GameLogic& game, vector<vector<vector<int>>>& TableauPositions, vector<vector<int>>& FoundationPositions, vector<vector<int>>& AdditionalPilesPositions) {
    int Y_START = 245;
    int X_START = 12;
    for (int i = 0; i < 7; ++i) {
        Y_START = 245;
        X_START = 12 + i * (58 + 38);
        int X_END = X_START + 58;
        TableauPositions[i].clear();
        if (game.getTableauPiles()[i].getCards().isEmpty()) {
            TableauPositions[i].push_back({X_START, X_END, Y_START, Y_START + 160});
            continue;
        }
        for (int j = 0; j < game.getTableauPiles()[i].getCards().getSize(); ++j) {
            if (j == game.getTableauPiles()[i].getCards().getSize() - 1) {
                TableauPositions[i].push_back({X_START, X_END, Y_START, Y_START + 80});
                Y_START += 80;
            } else {
                TableauPositions[i].push_back({X_START, X_END, Y_START, Y_START + 38});
                Y_START += 38;
            }
        }
    }
    AdditionalPilesPositions[0] = {145, 205, 78, 160};
    AdditionalPilesPositions[1] = {222, 280, 78, 160};

    int Start_X = 492;
    int Common_X_DIFF = 58;
    int COMMON_PILE_TO_PILE_X_DIFF = 38;
    int Start_Y = 78;
    int END_Y = 160;
    for (int i = 0; i < 4; ++i) {
        FoundationPositions[i] = {Start_X, Start_X + Common_X_DIFF, Start_Y, END_Y};
        Start_X += Common_X_DIFF + COMMON_PILE_TO_PILE_X_DIFF;
    }
}

void handleMouseClick(GameLogic& game, POINT pt, vector<vector<vector<int>>>& TableauPositions, vector<vector<int>>& FoundationPositions, vector<vector<int>>& AdditionalPilesPositions, bool& isCardSelected, Card& selectedCard, int& selectedTableau, int& selectedCardIndex, int& selectedCardCount, bool& isWasteSelected, bool& isFoundationSelected, int& selectedFoundation) {
    if (!isCardSelected && !isWasteSelected && !isFoundationSelected) {
        for (int i = 0; i < 7; ++i) {
            Stack cards = game.getTableauPiles()[i].getCards();
            int cardCount = cards.getSize();
            vector<Card> tempCards;

            for (int j = 0; j < cardCount; ++j) {
                if (!cards.isEmpty()) {
                    tempCards.push_back(cards.pop());
                } else {
                    cerr << "Error: Attempted to pop from an empty stack." << endl;
                    break;
                }
            }

            for (int j = cardCount - 1; j >= 0; --j) {
                const auto& card = tempCards[j];
                int posIndex = cardCount - 1 - j;
                if (posIndex < TableauPositions[i].size() && pt.x >= TableauPositions[i][posIndex][0] && pt.x <= TableauPositions[i][posIndex][1] && pt.y >= TableauPositions[i][posIndex][2] && pt.y <= TableauPositions[i][posIndex][3] && card.isFaceUp()) {
                    isCardSelected = true;
                    selectedCard = card;
                    selectedTableau = i;
                    selectedCardIndex = posIndex;
                    selectedCardCount = cardCount - posIndex;
                    clearOutputArea(80, 15, 15);
                    printDataAtXY(80, 20, "Selected Card: ");
                    PrintCardBox(card.getSuit(), card.getRank(), printSelectedCard_X, printSelectedCard_Y);
                    break;
                }
            }
            if (isCardSelected) break;
        }

        if (!isCardSelected) {
            const auto& wastePile = game.getWastePile();
            if (!wastePile.isEmpty() && pt.x >= AdditionalPilesPositions[1][0] && pt.x <= AdditionalPilesPositions[1][1] && pt.y >= AdditionalPilesPositions[1][2] && pt.y <= AdditionalPilesPositions[1][3]) {
                selectedCard = wastePile.topCard();
                isWasteSelected = true;
                clearOutputArea(80, 15, 15);
                printDataAtXY(80, 20, "Selected Card: ");
                PrintCardBox(selectedCard.getSuit(), selectedCard.getRank(), printSelectedCard_X, printSelectedCard_Y);
            }
        }

        if (!isCardSelected && !isWasteSelected) {
            for (int i = 0; i < 4; ++i) {
                if (pt.x >= FoundationPositions[i][0] && pt.x <= FoundationPositions[i][1] && pt.y >= FoundationPositions[i][2] && pt.y <= FoundationPositions[i][3]) {
                    const auto& foundationPile = game.getFoundationPiles()[i];
                    if (!foundationPile.isEmpty()) {
                        selectedCard = foundationPile.topCard();
                        isFoundationSelected = true;
                        selectedFoundation = i;
                        clearOutputArea(80, 15, 15);
                        printDataAtXY(80, 20, "Selected Card: ");
                        PrintCardBox(selectedCard.getSuit(), selectedCard.getRank(), printSelectedCard_X, printSelectedCard_Y);
                    }
                }
            }
        }

        // Check for stock pile click
        if (!isCardSelected && !isWasteSelected && !isFoundationSelected) {
            if (pt.x >= AdditionalPilesPositions[0][0] && pt.x <= AdditionalPilesPositions[0][1] && pt.y >= AdditionalPilesPositions[0][2] && pt.y <= AdditionalPilesPositions[0][3]) {
                game.drawCard();
                updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
                system("cls");
                game.displayGame();
            }
        }
    }
}

void handleCardMove(GameLogic& game, POINT pt, vector<vector<vector<int>>>& TableauPositions, vector<vector<int>>& FoundationPositions, vector<vector<int>>& AdditionalPilesPositions, bool& isCardSelected, Card& selectedCard, int& selectedTableau, int& selectedCardCount, bool& isWasteSelected, bool& isFoundationSelected, int& selectedFoundation) {
    if (isCardSelected) {
        for (int i = 0; i < 7; ++i) {
            if (i != selectedTableau) {
                int lastIndex = TableauPositions[i].size() - 1;
                if (pt.x >= TableauPositions[i][lastIndex][0] && pt.x <= TableauPositions[i][lastIndex][1] && pt.y >= TableauPositions[i][lastIndex][2] && pt.y <= TableauPositions[i][lastIndex][3]) {
                    bool move = game.moveCard(selectedTableau + 2, i + 2, selectedCardCount);
                    if (!move) {
                        clearOutputArea(80, 15, 15);
                        printDataAtXY(80, 20, "Invalid Move!");
                        isCardSelected = false;
                    } else {
                        isCardSelected = false;
                        updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
                        system("cls");
                        game.displayGame();
                        break;
                    }
                }
            }
        }
        for (int i = 0; i < 4; ++i) {
            if (pt.x >= FoundationPositions[i][0] && pt.x <= FoundationPositions[i][1] && pt.y >= FoundationPositions[i][2] && pt.y <= FoundationPositions[i][3]) {
                bool move = game.moveCard(selectedTableau + 2, i + 9, selectedCardCount);
                if (!move) {
                    clearOutputArea(80, 15, 15);
                    printDataAtXY(80, 20, "Invalid Move!");
                    isCardSelected = false;
                } else {
                    isCardSelected = false;
                    updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
                    system("cls");
                    game.displayGame();
                    break;
                }
            }
        }
    } else if (isWasteSelected) {
        for (int i = 0; i < 7; ++i) {
            int lastIndex = TableauPositions[i].size() - 1;
            if (pt.x >= TableauPositions[i][lastIndex][0] && pt.x <= TableauPositions[i][lastIndex][1] && pt.y >= TableauPositions[i][lastIndex][2] && pt.y <= TableauPositions[i][lastIndex][3]) {
                bool move = game.moveCard(1, i + 2);
                if (!move) {
                    clearOutputArea(80, 15, 15);
                    printDataAtXY(80, 20, "Invalid Move!");
                    isWasteSelected = false;
                } else {
                    isWasteSelected = false;
                    updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
                    system("cls");
                    game.displayGame();
                    break;
                }
            }
        }
        for (int i = 0; i < 4; ++i) {
            if (pt.x >= FoundationPositions[i][0] && pt.x <= FoundationPositions[i][1] && pt.y >= FoundationPositions[i][2] && pt.y <= FoundationPositions[i][3]) {
                bool move = game.moveCard(1, i + 9);
                if (!move) {
                    clearOutputArea(80, 15, 15);
                    printDataAtXY(80, 20, "Invalid Move!");
                    isWasteSelected = false;
                } else {
                    isWasteSelected = false;
                    updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
                    system("cls");
                    game.displayGame();
                    break;
                }
            }
        }
    } else if (isFoundationSelected) {
        for (int i = 0; i < 7; ++i) {
            int lastIndex = TableauPositions[i].size() - 1;
            if (pt.x >= TableauPositions[i][lastIndex][0] && pt.x <= TableauPositions[i][lastIndex][1] && pt.y >= TableauPositions[i][lastIndex][2] && pt.y <= TableauPositions[i][lastIndex][3]) {
                bool move = game.moveCard(selectedFoundation + 9, i + 2);
                if (!move) {
                    clearOutputArea(80, 15, 15);
                    printDataAtXY(80, 20, "Invalid Move!");
                    isFoundationSelected = false;
                } else {
                    isFoundationSelected = false;
                    updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
                    system("cls");
                    game.displayGame();
                    break;
                }
            }
        }
    }
}

void handleDeselection(bool& isCardSelected, bool& isWasteSelected, bool& isFoundationSelected, int selectedTableau, int selectedFoundation) {
    if (isCardSelected) {
        isCardSelected = false;
        clearOutputArea(80, 15, 15);
        printDataAtXY(80, 20, "Card Deselected");
    } else if (isWasteSelected) {
        isWasteSelected = false;
        clearOutputArea(80, 15, 15);
        printDataAtXY(80, 20, "Card Deselected");
    } else if (isFoundationSelected) {
        isFoundationSelected = false;
        clearOutputArea(80, 15, 15);
        printDataAtXY(80, 20, "Card Deselected");
    }
}

void handleModeSwitch(bool& isDarkMode, GameLogic& game) {
    isDarkMode = !isDarkMode;
    if (isDarkMode) {
        system("color 0F"); // Black background, white text
    } else {
        system("color F0"); // White background, black text
    }
    system("cls");
    game.displayGame();
    clearOutputArea(80, 25, 1);
    printDataAtXY(80, 25, "Switched to " + string(isDarkMode ? "Dark" : "Light") + " Mode");
    Sleep(200);
}

void GameLoop() {
    setConsoleSettings();
    system("color F0");
    fullsc();
    
    // Show loader for 2 seconds
    showLoader();
    
    system("cls");
    
    bool isDarkMode = false;
    bool showingRules = false;
    try {
        GameLogic game;
        vector<vector<vector<int>>> TableauPositions(7);
        vector<vector<int>> FoundationPositions(4);
        vector<vector<int>> AdditionalPilesPositions(2);
        chrono::steady_clock::time_point startTime = chrono::steady_clock::now();
        chrono::steady_clock::duration pausedDuration = chrono::steady_clock::duration::zero();
        chrono::steady_clock::time_point pauseStartTime;
        
        hideCursor();

        system("cls");
        game.displayGame();

        updatePositions(game, TableauPositions, FoundationPositions, AdditionalPilesPositions);
        
        while (true) {
            try {
                // Check for win condition
                if (game.isGameWon()) {
                    int elapsedTime = chrono::duration_cast<chrono::seconds>(
                        chrono::steady_clock::now() - startTime - pausedDuration).count();
                    showWinMessage(elapsedTime);
                    break;
                }

                // Calculate elapsed time (excluding paused time)
                int elapsedTime = chrono::duration_cast<chrono::seconds>(
                    chrono::steady_clock::now() - startTime - pausedDuration).count();
                
                if (!showingRules) {
                    setCursorPosition(95, 4);
                    cout << "Time Elapsed: " << elapsedTime << "s   ";
                }

                SHORT leftButton = GetAsyncKeyState(VK_LBUTTON);
                SHORT rightButton = GetAsyncKeyState(VK_RBUTTON);

                POINT pt;
                GetCursorPos(&pt);

                static bool isCardSelected = false;
                static Card selectedCard;
                static int selectedTableau = -1;
                static int selectedCardIndex = -1;
                static int selectedCardCount = 1;
                static bool isWasteSelected = false;
                static bool isFoundationSelected = false;
                static int selectedFoundation = -1;

                // Handle F key for rules
                if (GetAsyncKeyState('F') & 0x8000) {
                    if (!showingRules) {
                        // Entering rules view - pause timer
                        showingRules = true;
                        pauseStartTime = chrono::steady_clock::now();
                        showRules();
                    } else {
                        // Exiting rules view - resume timer
                        showingRules = false;
                        pausedDuration += chrono::steady_clock::now() - pauseStartTime;
                        system("cls");
                        game.displayGame();
                    }
                    Sleep(300); // Debounce
                }

                if (!showingRules) {
                    if (leftButton & 0x8000) {
                        handleMouseClick(game, pt, TableauPositions, FoundationPositions, AdditionalPilesPositions, isCardSelected, selectedCard, selectedTableau, selectedCardIndex, selectedCardCount, isWasteSelected, isFoundationSelected, selectedFoundation);
                        handleCardMove(game, pt, TableauPositions, FoundationPositions, AdditionalPilesPositions, isCardSelected, selectedCard, selectedTableau, selectedCardCount, isWasteSelected, isFoundationSelected, selectedFoundation);
                        Sleep(200);
                    }

                    if (GetAsyncKeyState('D') & 0x8000) {
                        handleDeselection(isCardSelected, isWasteSelected, isFoundationSelected, selectedTableau, selectedFoundation);
                    }

                    if (GetAsyncKeyState('S') & 0x8000) {
                        handleModeSwitch(isDarkMode, game);
                    }

                    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                        break;
                    }
                }

                Sleep(100);
            } catch (const exception& e) {
                cerr << "Exception in game loop: " << e.what() << endl;
            }
        }
        system("cls");
    } catch (const exception& e) {
        cerr << "Exception in gameLoop: " << e.what() << endl;
    }
}
