#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>
#include <iomanip>
#include <algorithm>
#define QNS 10
using namespace std;

void cls() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//flushes buffer then waits for Enter
void waitEnter() {
    fflush(stdin);
    getchar();
}

struct Question {
    string text;
    string choices[4];
    char   answer;
};

const Question questions[QNS] = {
    { "What is the capital of Nepal?",
      {"Pokhara", "Kathmandu", "Butwal", "Dharan"}, 'B' },
    { "Which is the highest mountain in the world?",
      {"K2", "Lhotse", "Mount Everest", "Kangchenjunga"}, 'C' },
    { "How many districts are in Nepal?",
      {"55", "75", "77", "80"}, 'C' },
    { "What is the national animal of Nepal?",
      {"Tiger", "Elephant", "Cow", "Snow Leopard"}, 'C' },
    { "What is the national flower of Nepal?",
      {"Rose", "Lotus", "Rhododendron", "Marigold"}, 'C' },
    { "Which river is the longest in Nepal?",
      {"Koshi", "Gandaki", "Karnali", "Bagmati"}, 'C' },
    { "In which year did Nepal become a republic?",
      {"2006", "2007", "2008", "2010"}, 'C' },
    { "What is the currency of Nepal?",
      {"Rupee", "Taka", "Dollar", "Dinar"}, 'A' },
    { "What does 'Sagarmatha' mean?",
      {"High peak", "Forehead of the sky", "Snow mountain", "Sacred hill"}, 'B' },
    { "Which city is known as the 'City of Lakes' in Nepal?",
      {"Kathmandu", "Biratnagar", "Pokhara", "Chitwan"}, 'C' }
};

void printDivider()       { cout << "====================" << endl; }
void printDoubleDivider() { cout << "####################" << endl; }

void printScoreboard(const vector<string>& names,
                     const vector<int>&    scores,
                     int currentTurn) {
    printDoubleDivider();
    cout << "      SCOREBOARD" << endl;
    printDoubleDivider();
    for (int i = 0; i < (int)names.size(); i++) {
        cout << (i == currentTurn ? "> " : "  ");
        cout << left << setw(12) << names[i]
             << "Score: " << scores[i] << endl;
    }
    printDoubleDivider();
}

void printRemaining(const vector<bool>& answered) {
    cout << "Available questions: ";
    for (int i = 0; i < QNS; i++)
        if (!answered[i]) cout << (i + 1) << "  ";
    cout << endl;
}

void showRules() {
    cls();
    printDivider();
    cout << "          QUIZ RULES" << endl;
    printDivider();
    cout << "1. Players take turns picking questions." << endl;
    cout << "2. Each player picks ONE question per turn." << endl;
    cout << "3. Picked questions disappear for everyone." << endl;
    cout << "4. Correct answer = 1 point." << endl;
    cout << "5. No negative marking for wrong answers." << endl;
    cout << "6. Player with highest score wins!" << endl;
    printDivider();
    cout << "\nPress Enter to return to menu...";
    waitEnter();
}

void runQuiz() {
    cls();
    printDivider();
    cout << "       MULTIPLAYER SETUP" << endl;
    printDivider();

    int numPlayers = 0;
    do {
        cout << "Enter number of players (2-4): ";
        cin >> numPlayers;
        if (numPlayers < 2 || numPlayers > 4)
            cout << "Please enter between 2 and 4." << endl;
    } while (numPlayers < 2 || numPlayers > 4);

    vector<string> names(numPlayers);
    vector<int>    scores(numPlayers, 0);

    // Use cin >> instead of getline (safer in Dev-C++)
    for (int i = 0; i < numPlayers; i++) {
        cout << "Enter name for Player " << (i + 1) << ": ";
        cin >> names[i];
    }

    vector<bool> answered(QNS, false);
    int totalAnswered = 0;
    int currentTurn   = 0;

    while (totalAnswered < QNS) {
        cls();
        printScoreboard(names, scores, currentTurn);
        cout << endl;
        printRemaining(answered);
        cout << endl;
        printDivider();
        cout << "  " << names[currentTurn] << "'s turn!" << endl;
        printDivider();
        cout << "Pick a question number: ";
        fflush(stdin);
        int qno;
        cin >> qno;

        if (qno < 1 || qno > QNS) {
            cout << "Invalid! Choose from the available list." << endl;
            cout << "Press Enter to try again...";
            waitEnter();
            continue;
        }
        if (answered[qno - 1]) {
            cout << "Question " << qno << " is already taken! Pick another." << endl;
            cout << "Press Enter to try again...";
            waitEnter();
            continue;
        }

        cls();
        printScoreboard(names, scores, currentTurn);
        cout << endl;

        const Question& q = questions[qno - 1];
        printDivider();
        cout << "  " << names[currentTurn] << " -- Question " << qno << endl;
        printDivider();
        cout << q.text << endl;
        cout << "A. " << q.choices[0] << endl;
        cout << "B. " << q.choices[1] << endl;
        cout << "C. " << q.choices[2] << endl;
        cout << "D. " << q.choices[3] << endl;
        cout << "\nYour answer: ";
        fflush(stdin);
        char ans;
        cin >> ans;
        ans = toupper(ans);

        if (ans == q.answer) {
            cout << "\n>> Correct! +1 point for " << names[currentTurn] << "!" << endl;
            scores[currentTurn]++;
        } else {
            cout << "\n>> Wrong! Correct answer was " << q.answer << "." << endl;
        }

        answered[qno - 1] = true;
        totalAnswered++;
        currentTurn = (currentTurn + 1) % numPlayers;

        cout << "\nPress Enter to continue...";
        waitEnter();
    }

    cls();
    printDoubleDivider();
    cout << "        FINAL RESULTS" << endl;
    printDoubleDivider();

    int maxScore = *max_element(scores.begin(), scores.end());
    vector<string> winners;

    for (int i = 0; i < numPlayers; i++) {
        cout << "  " << left << setw(14) << names[i]
             << scores[i] << " / " << QNS << " pts" << endl;
        if (scores[i] == maxScore) winners.push_back(names[i]);
    }

    printDoubleDivider();
    if ((int)winners.size() == 1) {
        cout << "  WINNER: " << winners[0]
             << " with " << maxScore << " points!" << endl;
    } else {
        cout << "  TIE between: ";
        for (int i = 0; i < (int)winners.size(); i++) {
            cout << winners[i];
            if (i < (int)winners.size() - 1) cout << " & ";
        }
        cout << " (" << maxScore << " pts each)!" << endl;
    }
    printDoubleDivider();

    cout << "\nPress Enter to return to menu...";
    waitEnter();
}

class Menu {
public:
    void show() {
        int opt = 0;
        do {
            cls();
            printDivider();
            cout << "       QUIZ SYSTEM" << endl;
            printDivider();
            cout << "1. Start Quiz" << endl;
            cout << "2. View Rules" << endl;
            cout << "3. Exit" << endl;
            printDivider();
            cout << "Choose an option: ";
            cin >> opt;

            switch (opt) {
                case 1: runQuiz();   break;
                case 2: showRules(); break;
                case 3:
                    cls();
                    cout << "Thank you for playing! Goodbye!" << endl;
                    break;
                default:
                    cout << "Invalid option. Enter 1, 2, or 3." << endl;
                    cout << "Press Enter to try again...";
                    waitEnter();
            }
        } while (opt != 3);
    }
};

int main() {
    Menu m;
    m.show();
    return 0;
}
