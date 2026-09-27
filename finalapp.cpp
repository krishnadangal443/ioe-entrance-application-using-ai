#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>
#include <iomanip>
#include <algorithm>
#include <sstream>
using namespace std;

// Dev-C++ / MinGW safe int-to-string
string intToStr(int n) {
    ostringstream ss;
    ss << n;
    return ss.str();
}

// Safe integer input via getline - avoids infinite loop on bad input
// Returns true and sets out if valid integer, false otherwise
bool safeReadInt(int& out) {
    string line;
    if (!getline(cin, line)) return false;
    size_t start = line.find_first_not_of(" \t\r\n");
    if (start == string::npos) return false;
    line = line.substr(start);
    if (line.empty()) return false;
    for (size_t i = 0; i < line.size(); i++)
        if (!isdigit((unsigned char)line[i])) return false;
    istringstream iss(line);
    return (bool)(iss >> out);
}

// --- Constants ---
const string QUESTION_FILE  = "D:\\quiz_questions.txt";
const string SAVEGAME_FILE  = "D:\\quiz_savegame.txt";
const string ADMIN_PASSWORD = "labb";

// --- Utility ---
void cls() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitEnter() {
    cout << "\nPress Enter to continue...";
    string dummy;
    getline(cin, dummy);
}

void printDivider()       { cout << "==============================" << endl; }
void printDoubleDivider() { cout << "##############################" << endl; }

// --- Question struct ---
struct Question {
    string text;
    string choices[4];
    char   answer;
};

// --- Built-in fallback questions ---
#define BUILTIN_COUNT 10
const Question BUILTIN_ARRAY[BUILTIN_COUNT] = {
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

vector<Question> getBuiltinQuestions() {
    vector<Question> v;
    for (int i = 0; i < BUILTIN_COUNT; i++)
        v.push_back(BUILTIN_ARRAY[i]);
    return v;
}

// =============================================================================
// QUESTION FILE I/O
// File format per question (7 lines):
//   question text | choice A | choice B | choice C | choice D | answer | ---
// =============================================================================

vector<Question> loadQuestionsFromFile() {
    vector<Question> qs;
    ifstream fin(QUESTION_FILE.c_str());
    if (!fin.is_open()) return qs;
    string line;
    while (true) {
        Question q;
        if (!getline(fin, q.text))       break;
        if (q.text.empty())              continue;
        if (!getline(fin, q.choices[0])) break;
        if (!getline(fin, q.choices[1])) break;
        if (!getline(fin, q.choices[2])) break;
        if (!getline(fin, q.choices[3])) break;
        string ansLine;
        if (!getline(fin, ansLine))      break;
        q.answer = toupper(ansLine.empty() ? 'A' : ansLine[0]);
        getline(fin, line); // separator ---
        qs.push_back(q);
    }
    fin.close();
    return qs;
}

bool saveQuestionsToFile(const vector<Question>& qs) {
    ofstream fout(QUESTION_FILE.c_str());
    if (!fout.is_open()) return false;
    for (size_t i = 0; i < qs.size(); i++) {
        fout << qs[i].text       << "\n"
             << qs[i].choices[0] << "\n"
             << qs[i].choices[1] << "\n"
             << qs[i].choices[2] << "\n"
             << qs[i].choices[3] << "\n"
             << qs[i].answer     << "\n"
             << "---"            << "\n";
    }
    fout.close();
    return true;
}

vector<Question> getActiveQuestions() {
    vector<Question> fromFile = loadQuestionsFromFile();
    if (!fromFile.empty()) return fromFile;
    return getBuiltinQuestions();
}

// =============================================================================
// SAVE GAME I/O
// Format:
//   Line 1 : numPlayers
//   Line 2 : totalAnswered
//   Line 3 : currentTurn
//   Line 4 : QNS (total questions in this session)
//   Next numPlayers lines : name
//   Next numPlayers lines : score
//   Next QNS lines        : 0 or 1 (answered flags)
// =============================================================================

struct SavedGame {
    int              numPlayers;
    int              totalAnswered;
    int              currentTurn;
    int              QNS;
    vector<string>   names;
    vector<int>      scores;
    vector<bool>     answered;
};

bool hasSavedGame() {
    ifstream f(SAVEGAME_FILE.c_str());
    return f.is_open();
}

bool saveGame(const vector<string>& names,
              const vector<int>&    scores,
              const vector<bool>&   answered,
              int totalAnswered,
              int currentTurn,
              int QNS) {
    ofstream fout(SAVEGAME_FILE.c_str());
    if (!fout.is_open()) return false;
    int np = (int)names.size();
    fout << np            << "\n"
         << totalAnswered << "\n"
         << currentTurn   << "\n"
         << QNS           << "\n";
    for (int i = 0; i < np;  i++) fout << names[i]  << "\n";
    for (int i = 0; i < np;  i++) fout << scores[i] << "\n";
    for (int i = 0; i < QNS; i++) fout << (answered[i] ? 1 : 0) << "\n";
    fout.close();
    return true;
}

bool loadGame(SavedGame& sg) {
    ifstream fin(SAVEGAME_FILE.c_str());
    if (!fin.is_open()) return false;
    fin >> sg.numPlayers >> sg.totalAnswered >> sg.currentTurn >> sg.QNS;
    fin.ignore(1000, '\n');
    sg.names.resize(sg.numPlayers);
    sg.scores.resize(sg.numPlayers);
    sg.answered.resize(sg.QNS);
    for (int i = 0; i < sg.numPlayers; i++) getline(fin, sg.names[i]);
    for (int i = 0; i < sg.numPlayers; i++) { fin >> sg.scores[i]; fin.ignore(1000,'\n'); }
    for (int i = 0; i < sg.QNS;        i++) {
        int v; fin >> v; fin.ignore(1000,'\n');
        sg.answered[i] = (v == 1);
    }
    fin.close();
    return true;
}

void deleteSaveGame() {
    remove(SAVEGAME_FILE.c_str());
}

// DISPLAY HELPERS

void printScoreboard(const vector<string>& names,
                     const vector<int>&    scores,
                     int currentTurn) {
    printDoubleDivider();
    cout << "         SCOREBOARD" << endl;
    printDoubleDivider();
    for (int i = 0; i < (int)names.size(); i++) {
        cout << (i == currentTurn ? "> " : "  ");
        cout << left << setw(14) << names[i]
             << "Score: " << scores[i] << endl;
    }
    printDoubleDivider();
}

void printRemaining(const vector<bool>& answered, int total) {
    cout << "Available questions: ";
    for (int i = 0; i < total; i++)
        if (!answered[i]) cout << (i + 1) << "  ";
    cout << endl;
}

// Shows scores + current leader/tie - used on mid-game exit
void showIntermediateResults(const vector<string>& names,
                             const vector<int>&    scores,
                             int QNS,
                             int totalAnswered) {
    cls();
    printDoubleDivider();
    cout << "      CURRENT STANDINGS" << endl;
    printDoubleDivider();
    cout << "  Questions answered : " << totalAnswered
         << " / " << QNS << endl;
    cout << "  Questions remaining: " << (QNS - totalAnswered) << endl;
    printDoubleDivider();

    int maxScore = *max_element(scores.begin(), scores.end());
    vector<string> leaders;

    for (int i = 0; i < (int)names.size(); i++) {
        cout << "  " << left << setw(16) << names[i]
             << scores[i] << " pts" << endl;
        if (scores[i] == maxScore) leaders.push_back(names[i]);
    }

    printDoubleDivider();
    if (maxScore == 0) {
        cout << "  No points scored yet." << endl;
    } else if ((int)leaders.size() == 1) {
        cout << "  LEADING: " << leaders[0]
             << " with " << maxScore << " pts" << endl;
    } else {
        cout << "  TIE: ";
        for (int i = 0; i < (int)leaders.size(); i++) {
            cout << leaders[i];
            if (i < (int)leaders.size() - 1) cout << " & ";
        }
        cout << " (" << maxScore << " pts each)" << endl;
    }
    printDoubleDivider();
    cout << "\n  Game saved. Resume anytime from the main menu." << endl;
}


// RULES

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
    cout << "7. 2 to 10 players supported." << endl;
    cout << "8. Type 'exit' during your turn to save & quit." << endl;
    printDivider();
    waitEnter();
}

// =============================================================================
// ADMIN PANEL
// =============================================================================

void adminPanel() {
    cls();
    printDivider();
    cout << "         ADMIN PANEL" << endl;
    printDivider();
    cout << "Enter admin password: ";
    string pw;
    getline(cin, pw);

    if (pw != ADMIN_PASSWORD) {
        cout << "\n!! Wrong password. Access denied." << endl;
        waitEnter();
        return;
    }

    int opt = 0;
    do {
        cls();
        vector<Question> qs = getActiveQuestions();
        bool usingFile = !loadQuestionsFromFile().empty();

        printDivider();
        cout << "         ADMIN PANEL" << endl;
        printDivider();
        cout << "Questions source : "
             << (usingFile ? QUESTION_FILE : "Built-in (no file yet)") << endl;
        cout << "Total questions  : " << qs.size() << endl;
        printDivider();
        cout << "1. View all questions" << endl;
        cout << "2. Add a question"     << endl;
        cout << "3. Delete a question"  << endl;
        cout << "4. Reset to built-in questions" << endl;
        cout << "5. Back to main menu"  << endl;
        printDivider();
        cout << "Choose: ";
        if (!safeReadInt(opt)) opt = -1;

        if (opt == 1) {
            cls();
            printDivider();
            cout << "       ALL QUESTIONS" << endl;
            printDivider();
            for (int i = 0; i < (int)qs.size(); i++) {
                cout << "\nQ" << (i+1) << ": " << qs[i].text << endl;
                cout << "   A. " << qs[i].choices[0] << endl;
                cout << "   B. " << qs[i].choices[1] << endl;
                cout << "   C. " << qs[i].choices[2] << endl;
                cout << "   D. " << qs[i].choices[3] << endl;
                cout << "   Answer: " << qs[i].answer << endl;
                printDivider();
            }
            waitEnter();
        }
        else if (opt == 2) {
            cls();
            printDivider();
            cout << "       ADD QUESTION" << endl;
            printDivider();
            Question nq;
            cout << "Question text   : "; getline(cin, nq.text);
            cout << "Choice A        : "; getline(cin, nq.choices[0]);
            cout << "Choice B        : "; getline(cin, nq.choices[1]);
            cout << "Choice C        : "; getline(cin, nq.choices[2]);
            cout << "Choice D        : "; getline(cin, nq.choices[3]);
            char ans = 0;
            do {
                cout << "Correct answer (A/B/C/D): ";
                string tmp; getline(cin, tmp);
                if (!tmp.empty()) ans = toupper(tmp[0]);
                if (ans < 'A' || ans > 'D')
                    cout << "Please enter A, B, C, or D." << endl;
            } while (ans < 'A' || ans > 'D');
            nq.answer = ans;
            qs.push_back(nq);
            if (saveQuestionsToFile(qs))
                cout << "\n>> Question added and saved to " << QUESTION_FILE << endl;
            else {
                cout << "\n!! Could not write to " << QUESTION_FILE << endl;
                cout << "   Make sure the D:\\ drive exists and is writable." << endl;
            }
            waitEnter();
        }
        else if (opt == 3) {
            if (qs.empty()) {
                cout << "\nNo questions to delete." << endl;
                waitEnter();
                continue;
            }
            cls();
            printDivider();
            cout << "     DELETE A QUESTION" << endl;
            printDivider();
            for (int i = 0; i < (int)qs.size(); i++)
                cout << (i+1) << ". " << qs[i].text << endl;
            printDivider();
            cout << "Enter question number to delete (0 to cancel): ";
            int del = -1;
            if (!safeReadInt(del)) del = -1;
            if (del < 1 || del > (int)qs.size()) {
                cout << "Cancelled." << endl;
            } else {
                string deleted = qs[del-1].text;
                qs.erase(qs.begin() + (del-1));
                if (saveQuestionsToFile(qs)) {
                    cout << "\n>> Deleted: \"" << deleted << "\"" << endl;
                    cout << "   Saved to " << QUESTION_FILE << endl;
                } else {
                    cout << "\n!! Could not write to " << QUESTION_FILE << endl;
                }
            }
            waitEnter();
        }
        else if (opt == 4) {
            cout << "\nThis will overwrite " << QUESTION_FILE
                 << " with the 10 built-in questions." << endl;
            cout << "Are you sure? (Y/N): ";
            string confirmLine; getline(cin, confirmLine);
            char confirm = confirmLine.empty() ? 'N' : toupper(confirmLine[0]);
            if (toupper(confirm) == 'Y') {
                vector<Question> builtins = getBuiltinQuestions();
                if (saveQuestionsToFile(builtins))
                    cout << ">> Reset done. Built-in questions saved to file." << endl;
                else
                    cout << "!! Could not write to " << QUESTION_FILE << endl;
            } else {
                cout << "Reset cancelled." << endl;
            }
            waitEnter();
        }
        else if (opt != 5) {
            cout << "Invalid option." << endl;
            waitEnter();
        }
    } while (opt != 5);
}


// QUIZ CORE  (shared by fresh start and resume)


void playQuiz(vector<string>   names,
              vector<int>      scores,
              vector<bool>     answered,
              int              totalAnswered,
              int              currentTurn,
              const vector<Question>& questions) {

    int QNS = (int)questions.size();

    while (totalAnswered < QNS) {
        cls();
        printScoreboard(names, scores, currentTurn);
        cout << endl;
        printRemaining(answered, QNS);
        cout << endl;
        printDivider();
        cout << "  " << names[currentTurn] << "'s turn!" << endl;
        printDivider();
        cout << "Pick a question number (or type 0 to save & exit): ";

        string input;
        getline(cin, input);

        // trim whitespace
        size_t st = input.find_first_not_of(" \t\r\n");
        if (st != string::npos) input = input.substr(st);

        // --- handle exit ---
        string lowerInput = input;
        for (size_t i = 0; i < lowerInput.size(); i++)
            lowerInput[i] = tolower(lowerInput[i]);

        if (lowerInput == "exit" || lowerInput == "0") {
            if (saveGame(names, scores, answered, totalAnswered, currentTurn, QNS)) {
                showIntermediateResults(names, scores, QNS, totalAnswered);
            } else {
                cout << "\n!! Could not save to " << SAVEGAME_FILE << endl;
                cout << "   Make sure D:\\ drive exists." << endl;
            }
            waitEnter();
            return;
        }

        // --- parse number ---
        bool isAllDigits = !input.empty();
        for (size_t i = 0; i < input.size(); i++)
            if (!isdigit((unsigned char)input[i])) { isAllDigits = false; break; }

        int qno = 0;
        if (!isAllDigits) {
            cout << "!! Invalid input! Please enter a number or 0 to exit." << endl;
            waitEnter();
            continue;
        }
        istringstream iss(input);
        iss >> qno;

        if (qno < 1 || qno > QNS) {
            cout << "!! Invalid number! Choose from the available list." << endl;
            waitEnter();
            continue;
        }
        if (answered[qno - 1]) {
            cout << "!! Question " << qno << " is already taken! Pick another." << endl;
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

        // Answer input with validation
        char ans = 0;
        while (true) {
            cout << "\nYour answer (A/B/C/D): ";
            string ansLine;
            getline(cin, ansLine);
            size_t as = ansLine.find_first_not_of(" \t\r\n");
            if (as == string::npos || ansLine.empty()) {
                cout << "!! Invalid input! Please enter A, B, C, or D." << endl;
                continue;
            }
            ans = toupper(ansLine[as]);
            if (ans >= 'A' && ans <= 'D') break;
            cout << "!! Invalid input! Please enter A, B, C, or D." << endl;
        }

        if (ans == q.answer) {
            cout << "\n>> Correct! +1 point for " << names[currentTurn] << "!" << endl;
            scores[currentTurn]++;
        } else {
            cout << "\n>> Wrong! Correct answer was " << q.answer << "." << endl;
        }

        answered[qno - 1] = true;
        totalAnswered++;
        currentTurn = (currentTurn + 1) % (int)names.size();

        waitEnter();
    }

    // --- All questions done: final results ---
    deleteSaveGame();   // clean up save file

    cls();
    printDoubleDivider();
    cout << "         FINAL RESULTS" << endl;
    printDoubleDivider();

    int maxScore = *max_element(scores.begin(), scores.end());
    vector<string> winners;

    for (int i = 0; i < (int)names.size(); i++) {
        cout << "  " << left << setw(16) << names[i]
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
    waitEnter();
}


// RUN QUIZ  (entry point - handles fresh start vs. resume)


void runQuiz() {
    cls();
    printDivider();
    cout << "       MULTIPLAYER QUIZ" << endl;
    printDivider();

    // Check for a saved game first
    if (hasSavedGame()) {
        cout << "  A saved game was found!" << endl;
        printDivider();
        cout << "1. Resume saved game" << endl;
        cout << "2. Start a new game (saved game will be deleted)" << endl;
        printDivider();
        cout << "Choose: ";
        int choice = -1;
        if (!safeReadInt(choice)) choice = -1;

        if (choice == 1) {
            // --- RESUME ---
            SavedGame sg;
            if (!loadGame(sg)) {
                cout << "\n!! Could not load save file. Starting fresh." << endl;
                waitEnter();
            } else {
                vector<Question> questions = getActiveQuestions();
                if ((int)questions.size() != sg.QNS) {
                    cout << "\n!! Question set has changed since last save." << endl;
                    cout << "   Cannot resume. Starting a new game." << endl;
                    deleteSaveGame();
                    waitEnter();
                } else {
                    cls();
                    printDivider();
                    cout << "       RESUMING GAME" << endl;
                    printDivider();
                    cout << "Players   : ";
                    for (int i = 0; i < sg.numPlayers; i++) {
                        cout << sg.names[i];
                        if (i < sg.numPlayers - 1) cout << ", ";
                    }
                    cout << endl;
                    cout << "Progress  : " << sg.totalAnswered
                         << " / " << sg.QNS << " questions done" << endl;
                    cout << "Next turn : " << sg.names[sg.currentTurn] << endl;
                    printDivider();
                    cout << "\nPress Enter to continue...";
                    { string d; getline(cin, d); }

                    playQuiz(sg.names, sg.scores, sg.answered,
                             sg.totalAnswered, sg.currentTurn, questions);
                    return;
                }
            }
        } else {
            // Delete old save, fall through to fresh setup
            deleteSaveGame();
        }
    }

    // --- FRESH START ---
    vector<Question> questions = getActiveQuestions();
    int QNS = (int)questions.size();

    if (QNS == 0) {
        cout << "No questions available! Please add questions via Admin Panel." << endl;
        waitEnter();
        return;
    }

    cout << "Questions loaded : " << QNS << endl;
    printDivider();

    int numPlayers = 0;
    while (true) {
        cout << "Enter number of players (2-10): ";
        if (!safeReadInt(numPlayers) || numPlayers < 2 || numPlayers > 10) {
            cout << "!! Invalid input. Please enter a number between 2 and 10." << endl;
            numPlayers = 0;
        } else {
            break;
        }
    }

    vector<string> names(numPlayers);
    vector<int>    scores(numPlayers, 0);
    for (int i = 0; i < numPlayers; i++) {
        cout << "Enter name for Player " << (i+1) << ": ";
        getline(cin, names[i]);
        if (names[i].empty()) names[i] = "Player" + intToStr(i+1);
    }

    vector<bool> answered(QNS, false);
    playQuiz(names, scores, answered, 0, 0, questions);
}

// =============================================================================
// MENU
// =============================================================================

class Menu {
public:
    void show() {
        int opt = 0;
        do {
            cls();
            printDivider();
            cout << "         QUIZ SYSTEM" << endl;
            printDivider();
            // Show saved-game indicator on menu
            if (hasSavedGame()) {
                cout << "  [Saved game available]" << endl;
                printDivider();
            }
            cout << "1. Start Quiz" << endl;
            cout << "2. View Rules"  << endl;
            cout << "3. Admin Panel" << endl;
            cout << "4. Exit"        << endl;
            printDivider();
            cout << "Choose an option: ";
            if (!safeReadInt(opt)) opt = -1;

            switch (opt) {
                case 1: runQuiz();    break;
                case 2: showRules();  break;
                case 3: adminPanel(); break;
                case 4:
                    cls();
                    cout << "Thank you for playing! Goodbye!" << endl;
                    break;
                default:
                    cout << "!! Invalid input. Please enter 1, 2, 3, or 4." << endl;
                    cout << "\nPress Enter to try again...";
                    { string d; getline(cin, d); }
            }
        } while (opt != 4);
    }
};

// =============================================================================
// MAIN
// =============================================================================

int main() {
    Menu m;
    m.show();
    return 0;
}
