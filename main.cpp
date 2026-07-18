#include <iostream>
#include <vector>
#include <random>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <deque>
using namespace std;
int code_len = 4;
int digit_range = 4;
int totalround = 6;
int time_limit = 0;
int round_left = totalround;
vector<vector<char>> inputed_code;
vector<int> code;
vector<int> corr_num(totalround, 0);
vector<int> corr_pos(totalround, 0);
deque<int> prev_inputs;
bool allowrepeat = false;
bool cheat = false;
long long int starttime = 0;
long long int gametime = 0;
string SaveName = "Default";
string Title = R"(
 __ __  __    __ _____ ___ ___ __ __ _ __  _ __   
|  V  |/  \ /' _/_   _| __| _ \  V  | |  \| | _\  
| \_/ | /\ |`._`. | | | _|| v / \_/ | | | ' | v | 
|_| |_|_||_||___/ |_| |___|_|_\_| |_|_|_|\__|__/  
)";
string WIN = R"(
 __   __ __  _  _   _   _  _ __  _   _  
 \ `v' //__\| || | | | | || |  \| | / \ 
  `. .'| \/ | \/ | | 'V' || | | ' | \_/ 
   !_!  \__/ \__/  !_/ \_!|_|_|\__| (_) 
)";
string LOSE = R"(
 __   __ __  _  _   _   __    __  ___   _  
 \ `v' //__\| || | | | /__\ /' _/| __| / \ 
  `. .'| \/ | \/ | | || \/ |`._`.| _|  \_/ 
   !_!  \__/ \__/  |___\__/ |___/|___| (_) 
)";
string SETTINGS = R"(
  __  ___ _____ _____ _ __  _  __   __  
/' _/| __|_   _|_   _| |  \| |/ _]/' _/ 
`._`.| _|  | |   | | | | | ' | [/\`._`. 
|___/|___| |_|   |_| |_|_|\__|\__/|___/ 
)";
string CHEATMODE = R"(
   ____  _ ___  __ _____ __ __  __  __  ___  
  / _/ || | __|/  \_   _|  V  |/__\| _\| __| 
 | \_| >< | _|| /\ || | | \_/ | \/ | v | _|  
  \__/_||_|___|_||_||_| |_| |_|\__/|__/|___| 
)";
string ACTIVATE = R"(
  __   ________ _  _   _   __ _____ ___ __   
 /  \ / _/_   _| || \ / | /  \_   _| __| _\  
| /\ | \__ | | | |`\ V /'| /\ || | | _|| v | 
|_||_|\__/ |_| |_|  \_/  |_||_||_| |___|__/  
)";
string DEACTIVATE = R"(
 __  ___  __   ________ _  _   _   __ _____ ___ __   
| _\| __|/  \ / _/_   _| || \ / | /  \_   _| __| _\  
| v | _|| /\ | \__ | | | |`\ V /'| /\ || | | _|| v | 
|__/|___|_||_|\__/ |_| |_|  \_/  |_||_||_| |___|__/  
)";

void menu(); 
void gameprocess();
void settings();
void reset();


void rand_code(vector<int> &code, mt19937 &gen) {
    for (int i = 0; i < code_len; i++) {
        int temp_gen = uniform_int_distribution<int>(1, digit_range)(gen);
        if (!allowrepeat){
            while (find(code.begin(), code.end(), temp_gen) != code.end()) {
                temp_gen = uniform_int_distribution<int>(1, digit_range)(gen);
            }
        }
        code.push_back(temp_gen);
    }
}


void reset(){
    code.clear();
    round_left = totalround;
    mt19937 gen(time(NULL));
    inputed_code.assign(totalround, vector<char>(code_len, '_'));
    corr_num.assign(totalround, 0);
    corr_pos.assign(totalround, 0);
    rand_code(code, gen);
    fill(corr_num.begin(), corr_num.end(), 0);
    fill(corr_pos.begin(), corr_pos.end(), 0);
}


void menu() {
    int pter_pos = 0;
    while (true) {
        system("cls");
        cout << Title << '\n';
        switch (pter_pos){
            case 0:
                cout << "> Start Game <" << endl << "  Load Game" << endl << "  Game Settings" << endl << "  Quit Game" << endl;
                break;
            case 1:
                cout << "  Start Game" << endl << "> Load Game <" << endl << "  Game Settings" << endl << "  Quit Game" << endl;
                break;
            case 2:
                cout << "  Start Game" << endl << "  Load Game" << endl << "> Game Settings < " << endl << "  Quit Game" << endl;
                break;
            case 3:
                cout << "  Start Game" << endl << "  Load Game" << endl << "  Game Settings" << endl << "> Quit Game <" << endl;
            default:
            break;
        }

        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == 72 && pter_pos > 0){
                pter_pos--;
            }else if (key == 80 && pter_pos < 3){
                pter_pos++;
            }
            
        }

        if (key == 13){
            switch (pter_pos){
                case 0:
                    reset();
                    gameprocess();
                    break;
                case 1:
                    //loadgame();
                    break;
                case 2:
                    settings();
                    break;
                case 3:
                    exit(0);
                    break;
                default:
                    break;
                }
        }

    }
}



string renderbar(int now, int max){
    string bar;
    bar += "<";
    for (int i = 0; i < now; i++){
        bar += "▪";
    }
    for (int i = now; i < max && max > 0; i++){
        bar += "-";
    }
    bar += ">";
    return bar;
}

void settings(){
    int pter_pos = 0;
    while (true) {
        system("cls");
        cout << SETTINGS << '\n';
        switch (pter_pos){
            case 0:
                cout << "> Allow Repeat Digits: " << (allowrepeat ? "Yes <" : "No <") << endl
                     << "  Code Length: " << renderbar(code_len , 9) << '[' << code_len << ']' << endl
                     << "  Code Range: " << renderbar(digit_range , 9) << '[' << digit_range << ']' << endl
                     << "  Round Limit: " << renderbar(totalround , -1) << '[' << totalround << ']' << endl
                     << "  Back to Menu" << endl;
                break;
            case 1:
                cout << "  Allow Repeat Digits: " << (allowrepeat ? "Yes" : "No") << endl
                     << "> Code Length: " << renderbar(code_len , 9) << '[' << code_len << ']' << " <" << endl
                     << "  Code Range: " << renderbar(digit_range , 9) << '[' << digit_range << ']' << endl
                     << "  Round Limit: " << renderbar(totalround , -1) << '[' << totalround << ']' << endl
                     << "  Back to Menu" << endl;
                break;
            case 2:
                cout << "  Allow Repeat Digits: " << (allowrepeat ? "Yes" : "No") << endl
                     << "  Code Length: " << renderbar(code_len , 9) << '[' << code_len << ']' << endl
                     << "> Code Range: " << renderbar(digit_range , 9) << '[' << digit_range << ']' << " <" << endl
                     << "  Round Limit: " << renderbar(totalround , -1) << '[' << totalround << ']' << endl
                     << "  Back to Menu" << endl;
                break;
            case 3:
                cout << "  Allow Repeat Digits: " << (allowrepeat ? "Yes" : "No") << endl
                     << "  Code Length: " << renderbar(code_len , 9) << '[' << code_len << ']' << endl
                     << "  Code Range: " << renderbar(digit_range , 9) << '[' << digit_range << ']' << endl
                     << "> Round Limit: " << renderbar(totalround , -1) << '[' << totalround << ']' << " <" << endl
                     << "  Back to Menu" << endl;
                break;
            case 4:
                cout << "  Allow Repeat Digits: " << (allowrepeat ? "Yes" : "No") << endl
                     << "  Code Length: " << renderbar(code_len , 9) << '[' << code_len << ']' << endl
                     << "  Code Range: " << renderbar(digit_range , 9) << '[' << digit_range << ']' << endl
                     << "  Round Limit: " << renderbar(totalround ,-1) << '[' << totalround << ']' << endl
                     << "> Back to Menu <" << endl;
                break;
            default:
                break;
        }
        
        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == 72 && pter_pos > 0){
                pter_pos--;
            }else if (key == 80 && pter_pos < 4){
                pter_pos++;
            }
            
        }

        if ( (key == 75 || key == 77) || key == 13){
            switch (pter_pos){
                case 0:
                    if (key == 13){
                        allowrepeat = !allowrepeat;
                    }
                    break;
                case 1:
                    if (key == 75 && code_len > 1){
                        code_len--;
                    } else if (key == 77 && code_len < 9){
                        code_len++;
                    }
                    break;
                case 2:
                    if (key == 75 && digit_range > 1){
                        digit_range--;
                    } else if (key == 77 && digit_range < 9){
                        digit_range++;
                    }
                    break;
                case 3:
                    if (key == 75 && totalround > 1){
                        totalround--;
                    } else if (key == 77){
                        totalround++;
                    }
                    break;
                case 4:
                    if (key == 13){
                        return;
                    }
                    break;
                default:
                    break;
                }
        }

    }
}


bool checkcode(int y){
    corr_pos[y] = 0;
    corr_num[y] = 0;
    vector<bool> code_checked(code_len, false);
    vector<bool> input_checked(code_len, false);

    for (int i = 0; i < code_len; i++) {
        if (inputed_code[y][i] - '0' == code[i]) {
            corr_pos[y]++;
            code_checked[i] = true;
            input_checked[i] = true;
        }
    }
    
    for (int i = 0; i < code_len; i++) {
        if (!input_checked[i]) {
            for (int j = 0; j < code_len; j++) {
                if (!code_checked[j] && inputed_code[y][i] - '0' == code[j]) {
                    corr_num[y]++;
                    code_checked[j] = true;
                    break;
                }
            }
        }
    }
    if (corr_pos[y] == code_len) {
        return true;
    }
    else {
        return false;
    }
}


void rendergui(){
    cout << endl << SaveName << endl << "Code Length: " << code_len << "    Remains: " << round_left << "/" << totalround << "    Game Time: " << gametime << " seconds" << endl << endl << "  " ;
        for (int i = 0; i < code_len; i++) {
            cout << " _";
        }
}


void rendermatrix(int y){
    cout << "     " << "Position Correct" << "      " << "Code Correct" << endl;
            for (int i = 0; i < totalround; ++i) {
                if (i == y){
                    printf(">>");
                }else{
                    printf("  ");
                }
            for (int j = 0; j < code_len; j++){
                cout << "|" << inputed_code[i][j];
            }
            cout << "|" << "          " << corr_pos[i] << "          " << corr_num[i] << endl ;
            }
            cout << endl;
}

void win(){
    cout << WIN << endl;
    _getch();
    reset();
    menu();
}

void lose(){
    cout << LOSE << endl;
    _getch();
    reset();
    menu();
}

void checkcheat(int input){
    bool konami = false;
    int cheat_code[10] = {72,72,80,80,75,77,75,77,98,97};

    if (prev_inputs.size() > 9){
        prev_inputs.pop_front();
    }

    prev_inputs.push_back(input);

    if (prev_inputs.size() == 10){
        konami = true;
        for (int i = 0; i < 10; i++){
            if (prev_inputs[i] != cheat_code[i]){
                konami = false;
                break;
            }
        }

        if (konami){
            if (!cheat){
                cout << CHEATMODE << endl << ACTIVATE << endl;
                _getch();
            }else{
                cout << CHEATMODE << endl << DEACTIVATE << endl;
                _getch();
            }
            cheat = !cheat;
        }
    }
    //cout << prev_inputs.size() << endl;
    //cout << input << endl;
}



void gameprocess(){
    system("cls");
    cheat = false;
    starttime = time(NULL);
    bool HasCheat = false;
    int x = 0, y = 0;
    rendergui();
    rendermatrix(y);

    while (true) {
        if (_kbhit()) {
            system("cls");
            int input = _getch();

                if (input == 0 || input == 224) {
                input = _getch();
                }

                checkcheat(input);

                if (cheat) {
                    HasCheat = true;
                    cout << "CHEATMODE ACTIVATED" << endl;
                    cout << "The code is: ";
                    for (int i = 0; i < code_len; i++) {
                    cout << code[i] << " ";
                    }
                }


            gametime = time(NULL)-starttime;

            if (input == 8) {
                if (x > 0) {
                    --x;
                    inputed_code[y][x] = '_';
                }
            }

            else if (input == 13) {
                if (x == code_len) {
                    if (!checkcode(y)) {
                        y++;
                        round_left--;
                        x = 0;
                    }
                    else {
                        win();
                    }
                }

                if (round_left <= 0) {
                    lose();
                    cout << endl;
                    cout << "You Lost, The correct code is：";
                    for (int i = 0; i < code_len; i++) {
                        cout << code[i] << " ";
                    }
                }
            }

            else if ((input >= '0' && input <= '9') && x < code_len) {
                inputed_code[y][x] = input;
                x++;
            }

            rendergui();

            rendermatrix(y);
        }
    }
}


int main() {
    reset();
    menu();
    return 0;
}