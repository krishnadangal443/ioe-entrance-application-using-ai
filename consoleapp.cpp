#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#define QNS 10
using namespace std;

void cls() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


struct Question {
    string text;
    string choices[4];   // A, B, C, D
    char   answer;       // correct letter (uppercase)
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

void printDivider() { cout << "====================" << endl; }

void showRules() {
    cls();
    printDivider();
    cout << "          QUIZ RULES" << endl;
    printDivider();
    cout << "1. There are " << QNS << " questions in total." << endl;
    cout << "2. Choose any unanswered question by its number." << endl;
    cout << "3. Each correct answer gives 1 point." << endl;
    cout << "4. No negative marking for wrong answers." << endl;
    cout << "5. Each question can only be answered once." << endl;
    printDivider();
}

void runQuiz() {
    vector<bool> answered(QNS, false); //to cheak wether the question is already answered or not
    int score      = 0;
    int answered_n = 0;

    while (answered_n < QNS) {
        cls();
        // Show remaining question numbers
        cout << "\nRemaining questions: \n";
        for (int i = 0; i < QNS; i++) {
            if (!answered[i]) cout << (i + 1) << "  ";
        }
        cout << "\nSelect a question number: ";

        int qno;
        cin >> qno;

        // Validate input
        if (qno < 1 || qno > QNS) {
            cout << "Invalid number! Please choose from the list above." << endl;
            continue;
        }
        if (answered[qno - 1]) {
            cout << "You already answered question " << qno << "! Pick another." << endl;
            continue;
        }

        // Display question
        const Question& q = questions[qno - 1];
        printDivider();
        cout << "Question " << qno << ": " << q.text << endl;
        cout << "A. " << q.choices[0] << endl;
        cout << "B. " << q.choices[1] << endl;
        cout << "C. " << q.choices[2] << endl;
        cout << "D. " << q.choices[3] << endl;
        cout << "Your answer: ";

        char ans;
        cin >> ans;
        ans = toupper(ans);

        if (ans == q.answer) {
            cout <<  " Correct!" << endl;
            score++;
        } else {
            cout << "Wrong! The correct answer was " << q.answer << "." << endl;
        }

        answered[qno - 1] = true;
        answered_n++;

        // Small pause so user can read result before screen clears
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    cls();
    // Final score
    printDivider();
    cout << "       QUIZ COMPLETE!" << endl;
    printDivider();
    cout << "Your Final Score: " << score << " / " << QNS << endl;

    if      (score == QNS)       cout << "Perfect score! Outstanding!" << endl;
    else if (score >= QNS * 0.8) cout << "Great job!" << endl;
    else if (score >= QNS * 0.5) cout << "Good effort!" << endl;
    else                         cout << "Better luck next time!" << endl;

    printDivider();
    cout << "\nPress Enter to return to menu...";
    cin.ignore();
    cin.get();
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
                case 1: runQuiz();  break;
                case 2:
                    showRules();
                    cout << "\nPress Enter to return to menu...";
                    cin.ignore();
                    cin.get();
                    break;
                case 3:
                    cout << "Thank you for using Quiz System. Goodbye!" << endl;
                    break;
                default:
                    cout << "Invalid option. Please enter 1, 2, or 3." << endl;
            }
        } while (opt != 3);
    }
};

int main() {
    Menu m;
    m.show();
    return 0;
}
