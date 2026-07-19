#include <iostream>
#include <vector>
#include <random>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <deque>
#include <windows.h>
#include <sstream>
#include <iomanip>
#include <fstream>

using namespace std;

string GameName = "Default";
int savecount = 0;
int code_len = 4;
int digit_range = 6;
int totalround = 6;
int round_left = totalround;
bool hascheat = false;
vector<vector<char>> inputed_code;
vector<int> code;
vector<int> corr_num(totalround, 0);
vector<int> corr_pos(totalround, 0);
deque<int> prev_inputs;
bool allowrepeat = false;
bool cheat = false;
bool pvp = false;
long long int starttime = 0;
long long int gametime = 0;
const string slotdir = "slots.txt";

struct SaveData {
    bool hascheat;
    bool cheat;
    long long int starttime;
    long long int gametime;
    int code_len;
    int digit_range;
    int totalround;
    int round_left;
    bool allowrepeat;
    vector<int> code;
    vector<vector<char>> inputed_code;
    vector<int> corr_num;
    vector<int> corr_pos;
    //to Binary
    void serialize(ofstream& ofs) {
            ofs.write(reinterpret_cast<const char*>(&hascheat), sizeof(hascheat));
            ofs.write(reinterpret_cast<const char*>(&cheat), sizeof(cheat));
            ofs.write(reinterpret_cast<const char*>(&starttime), sizeof(starttime));
            ofs.write(reinterpret_cast<const char*>(&gametime), sizeof(gametime));
            ofs.write(reinterpret_cast<const char*>(&code_len), sizeof(code_len));
            ofs.write(reinterpret_cast<const char*>(&digit_range), sizeof(digit_range));
            ofs.write(reinterpret_cast<const char*>(&totalround), sizeof(totalround));
            ofs.write(reinterpret_cast<const char*>(&round_left), sizeof(round_left));
            ofs.write(reinterpret_cast<const char*>(&allowrepeat), sizeof(allowrepeat));

            size_t code_len = code.size();
            ofs.write(reinterpret_cast<const char*>(&code_len), sizeof(code_len));
            if (code_len > 0) {
                ofs.write(reinterpret_cast<const char*>(code.data()), code_len * sizeof(int));
            }

            size_t y = inputed_code.size(); //2D matrix
            ofs.write(reinterpret_cast<const char*>(&y), sizeof(y));
            for (const auto& y : inputed_code) {
                size_t x = y.size();
                ofs.write(reinterpret_cast<const char*>(&x), sizeof(x));
                if (x > 0) {
                    ofs.write(reinterpret_cast<const char*>(y.data()), x * sizeof(char));
                }
            }

            size_t nsize = corr_num.size();
            ofs.write(reinterpret_cast<const char*>(&nsize), sizeof(nsize));
            if (nsize > 0) {
                ofs.write(reinterpret_cast<const char*>(corr_num.data()),
                        nsize * sizeof(int));
            }

            size_t psize = corr_pos.size();
            ofs.write(reinterpret_cast<const char*>(&psize), sizeof(psize));
            if (psize > 0) {
                ofs.write(reinterpret_cast<const char*>(corr_pos.data()),
                        psize * sizeof(int));
            }
        }

    //from Binary
    void deserialize(ifstream& ifs) {
        ifs.read(reinterpret_cast<char*>(&hascheat), sizeof(hascheat));
        ifs.read(reinterpret_cast<char*>(&cheat), sizeof(cheat));
        ifs.read(reinterpret_cast<char*>(&starttime), sizeof(starttime));
        ifs.read(reinterpret_cast<char*>(&gametime), sizeof(gametime));
        ifs.read(reinterpret_cast<char*>(&code_len), sizeof(code_len));
        ifs.read(reinterpret_cast<char*>(&digit_range), sizeof(digit_range));
        ifs.read(reinterpret_cast<char*>(&totalround), sizeof(totalround));
        ifs.read(reinterpret_cast<char*>(&round_left), sizeof(round_left));
        ifs.read(reinterpret_cast<char*>(&allowrepeat), sizeof(allowrepeat));

        size_t code_len = 0;
        ifs.read(reinterpret_cast<char*>(&code_len), sizeof(code_len));
        code.resize(code_len);
        if (code_len > 0) {
            ifs.read(reinterpret_cast<char*>(code.data()), code_len * sizeof(int));
        }

        size_t y = 0; // 2D matrix
        ifs.read(reinterpret_cast<char*>(&y), sizeof(y));
        inputed_code.resize(y);
        for (size_t i = 0; i < y; ++i) {
            size_t x = 0;
            ifs.read(reinterpret_cast<char*>(&x), sizeof(x));
            inputed_code[i].resize(x);
            if (x > 0) {
                ifs.read(reinterpret_cast<char*>(inputed_code[i].data()), x * sizeof(char));
            }
        }
        size_t nsize = 0;
        ifs.read(reinterpret_cast<char*>(&nsize), sizeof(nsize));
        corr_num.resize(nsize);
        if (nsize > 0) {
            ifs.read(reinterpret_cast<char*>(corr_num.data()),
                    nsize * sizeof(int));
            }

        size_t psize = 0;
        ifs.read(reinterpret_cast<char*>(&psize), sizeof(psize));
        corr_pos.resize(psize);
        if (psize > 0) {
            ifs.read(reinterpret_cast<char*>(corr_pos.data()),
                    psize * sizeof(int));
        }
    }

} Default = {false,false,0,0,4,6,8,8,false,{0,0,0,0},vector<vector<char>>(8, vector<char>(4,'_')),vector<int>(8, 0),vector<int>(8, 0)};

struct SaveSlot {
    string SaveName;
    string FileName;
    time_t lastopen;
    bool occupied;
} DefaultSlot = {"Default", "slot1.bin", 0, false};


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

string SAVES = R"(
  __   __   _   _  ___  __  
/' _/ /  \ | \ / || __/' _/ 
`._`.| /\ |`\ V /'| _|`._`. 
|___/|_||_|  \_/  |___|___/ 
)";

void menu(); 
void gameprocess();
void settings();
void startingsettings();
void reset();
void gamemenu();


void saveSlots(const vector<SaveSlot>& slots) {
    ofstream ofs(slotdir, ios::trunc);
    for (const auto& slot : slots) {
        ofs << (slot.occupied ? "1" : "0") << ';'
            << slot.SaveName << ';'
            << slot.FileName << ';'
            << slot.lastopen
            << '\n';
    }
}

vector<SaveSlot> loadSlots() {
    vector<SaveSlot> slots;
    string line;
    ifstream ifs(slotdir);
    if (!ifs) {
        for (int i = 1; i <= 3; ++i) {
            slots.push_back({
                "Empty Slot",
                "Slot" + to_string(i) + ".bin",
                0,
                false
            });
        }
        saveSlots(slots);
        return slots;
    }

    while (getline(ifs, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string temp;
        SaveSlot slot;
        getline(ss, temp, ';'); slot.occupied   = (temp == "1");
        getline(ss, temp, ';'); slot.SaveName   = temp;
        getline(ss, temp, ';'); slot.FileName   = temp;
        getline(ss, temp, ';'); slot.lastopen   = static_cast<time_t>(stoll(temp));
        slots.push_back(slot);
    }
    return slots;
}

void SaveToSlot(SaveSlot& slot) {
    SaveData data; 
    data.hascheat   = hascheat;
    data.cheat      = cheat;
    data.starttime  = starttime;
    data.gametime   = gametime;
    data.code_len   = code_len;
    data.digit_range= digit_range;
    data.totalround = totalround;
    data.round_left = round_left;
    data.allowrepeat= allowrepeat;
    data.code       = code;
    data.inputed_code = inputed_code;
    data.corr_num   = corr_num;
    data.corr_pos   = corr_pos;

    ofstream ofs(slot.FileName, ios::binary);
    if (!ofs) {
        cout << "Error" << endl;
        _getch();
        return;
    }
    data.serialize(ofs);
    ofs.close();
    slot.occupied = true;
    slot.lastopen = time(NULL);
    cout << "Game saved to: " << slot.FileName << endl;
    _getch();
}

bool loadGameFromSlot(const SaveSlot& slot) {
    if (!slot.occupied) {
        cout << "Empty Slot!\n";
        _getch();
        return false;
    }

    ifstream ifs(slot.FileName, ios::binary);
    if (!ifs) {
        cout << "No Save Data Found!\n";
        _getch();
        return false;
    }

    SaveData data;
    data.deserialize(ifs);
    ifs.close();

    hascheat    = data.hascheat;
    cheat       = data.cheat;
    starttime   = data.starttime;
    gametime    = data.gametime;
    code_len    = data.code_len;
    digit_range = data.digit_range;
    totalround  = data.totalround;
    round_left  = data.round_left;
    allowrepeat = data.allowrepeat;
    code        = data.code;
    inputed_code= data.inputed_code;
    corr_num    = data.corr_num;
    corr_pos    = data.corr_pos;

    inputed_code.resize(totalround, vector<char>(code_len, '_'));
    corr_num.resize(totalround, 0);
    corr_pos.resize(totalround, 0);
    GameName = slot.SaveName;

    return true;
}

void Savemenu() {
    auto slots = loadSlots();
    int cursor = 0;
    enum Mode { LOAD_MODE, SAVE_MODE } mode = LOAD_MODE;

    while (true) {
        system("cls");
        cout << SAVES << '\n';
        cout << "[←/→ Mode | ↑/↓ Saves]\n\n";
        cout << (mode == LOAD_MODE ? "> Load Mode <\n" : "  Load Mode\n");
        cout << (mode == SAVE_MODE ? "> Save Mode <\n\n" : "  Save Mode\n\n");

        for (size_t i = 0; i < slots.size(); ++i) {
            const auto& slot = slots[i];
            cout << (i == cursor ? ">> " : "   ");
            cout << "Slot " << (i + 1) << ": ";
            if (slot.occupied) {
                tm* t = localtime(&slot.lastopen);
                char buf[32];
                strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", t);
                cout << slot.SaveName << "  (" << buf << ")";
            } else {
                cout << "Empty Slot";
            }
            cout << '\n';
        }

        int key = _getch();
        if (key == 27) return;         // Esc
        if (key == 224 || key == 0) {
            key = _getch();
            if (key == 72 && cursor > 0) cursor--;
            if (key == 80 && cursor < (int)slots.size() - 1) cursor++;
            if (key == 75 || key == 77) {
                mode = (mode == LOAD_MODE ? SAVE_MODE : LOAD_MODE);
            }
            continue;
        }
        if (key == 13) { // Enter
            auto& slot = slots[cursor];
            if (mode == LOAD_MODE) {
                if (loadGameFromSlot(slot)) {
                    slot.lastopen = time(nullptr);
                    saveSlots(slots);
                    gameprocess();
                }
            } else {
                cout << "Enter Save Name: ";
                string newName;
                getline(cin, newName);
                if (newName.empty()) newName = "Save Slot " + to_string(cursor + 1);
                slot.SaveName = newName;
                SaveToSlot(slot);
                saveSlots(slots);
            }
        }
    }
}


void rand_code(vector<int> &code, mt19937 &gen) {
    if (pvp == true) {
        pvp = false;
        return;
    }
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
    random_device rd;
    mt19937 gen(rd());
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
                cout << "> New Game <" << endl << "  Saves" << endl << "  Game Settings" << endl << "  Quit Game" << endl;
                break;
            case 1:
                cout << "  New Game" << endl << "> Saves <" << endl << "  Game Settings" << endl << "  Quit Game" << endl;
                break;
            case 2:
                cout << "  New Game" << endl << "  Saves" << endl << "> Game Settings < " << endl << "  Quit Game" << endl;
                break;
            case 3:
                cout << "  New Game" << endl << "  Saves" << endl << "  Game Settings" << endl << "> Quit Game <" << endl;
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
                    startingsettings();
                    reset();
                    gameprocess();
                    break;
                case 1:
                    Savemenu();
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

void inputPvpCode() {
    vector<int> pvpcode;
    cout << "Input your code. (" << code_len << " Digits): ";

    while (pvpcode.size() < code_len) {
        int ch = _getch();
        if (ch >= '1' && ch <= '9') {
            int d = ch - '0';
            if (d > digit_range) continue;
            if (!allowrepeat && find(pvpcode.begin(), pvpcode.end(), d) != pvpcode.end()) continue;

            pvpcode.push_back(d);
            cout << d;
        }
        else if ((ch == 8 || ch == 127) && !pvpcode.empty()) { 
            pvpcode.pop_back();
            cout << "\b";
        }
    }

    code = pvpcode;
    cout << "\n\nPVP Code Set!" << endl;
    _getch();
    system("cls");
}

void inputSaveName() {
    cout << "Enter Save Name: ";
    cin >> GameName;
}

void startingsettings(){
    int pter_pos = 0;
    while (true)
    {
        system("cls");
        cout << "Mode Selection:\n";
        cout << (pter_pos == 0 ? "> Standard Mode <\n" : "  Standard Mode\n");
        cout << (pter_pos == 1 ? "> PVP Mode <\n" : "  PVP Mode\n");

        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == 72 && pter_pos > 0){
                pter_pos--;
            }else if (key == 80 && pter_pos < 1){
                pter_pos++;
            }
        }

        if (key == 13){
            switch (pter_pos){
                case 0:
                    inputSaveName();
                    return;
                case 1:
                    inputPvpCode();
                    pvp = true;
                    inputSaveName();
                    return;
            }
        }
    }
    
}

void settings(){
    int pter_pos = 0;
    while (true) {
        system("cls");
        cout << SETTINGS << '\n';
        if (!allowrepeat && digit_range < code_len){
            digit_range = code_len;
        }
        switch (pter_pos){
            case 0:
                cout << "> Allow Repeat Digits: " << (allowrepeat ? "Yes <" : "No <") << endl
                     << "  Code Length: " << renderbar(code_len , 9) << '[' << code_len << ']' << endl
                     << "  Code Range: " << renderbar(digit_range , 9) << '[' << digit_range << ']' << endl
                     << "  Round Limit: " << renderbar(totalround , -1) << '[' << totalround << ']' << endl
                     << "  Continue" << endl;
                break;
            case 1:
                cout << "  Allow Repeat Digits: " << (allowrepeat ? "Yes" : "No") << endl
                     << "> Code Length: " << renderbar(code_len , 9) << '[' << code_len << ']' << " <" << endl
                     << "  Code Range: " << renderbar(digit_range , 9) << '[' << digit_range << ']' << endl
                     << "  Round Limit: " << renderbar(totalround , -1) << '[' << totalround << ']' << endl
                     << "  Continue" << endl;
                break;
            case 2:
                cout << "  Allow Repeat Digits: " << (allowrepeat ? "Yes" : "No") << endl
                     << "  Code Length: " << renderbar(code_len , 9) << '[' << code_len << ']' << endl
                     << "> Code Range: " << renderbar(digit_range , 9) << '[' << digit_range << ']' << " <" << endl
                     << "  Round Limit: " << renderbar(totalround , -1) << '[' << totalround << ']' << endl
                     << "  Continue" << endl;
                break;
            case 3:
                cout << "  Allow Repeat Digits: " << (allowrepeat ? "Yes" : "No") << endl
                     << "  Code Length: " << renderbar(code_len , 9) << '[' << code_len << ']' << endl
                     << "  Code Range: " << renderbar(digit_range , 9) << '[' << digit_range << ']' << endl
                     << "> Round Limit: " << renderbar(totalround , -1) << '[' << totalround << ']' << " <" << endl
                     << "  Continue" << endl;
                break;
            case 4:
                cout << "  Allow Repeat Digits: " << (allowrepeat ? "Yes" : "No") << endl
                     << "  Code Length: " << renderbar(code_len , 9) << '[' << code_len << ']' << endl
                     << "  Code Range: " << renderbar(digit_range , 9) << '[' << digit_range << ']' << endl
                     << "  Round Limit: " << renderbar(totalround ,-1) << '[' << totalround << ']' << endl
                     << "> Continue <" << endl;
                break;
            default:
                break;
        }
        
        int key = _getch();

        if (key == 27) return;     

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

void gamemenu(){
    int pter_pos = 0;
    while (true) {
        system("cls");
        cout << Title << '\n';
        switch (pter_pos){
            case 0:
                cout << "> Resume Game <" << endl << "  Save " << endl << "  Exit" << endl;
                break;
            case 1:
                cout << "  Resume Game" << endl << "> Save <" << endl << "  Exit" << endl;
                break;
            case 2:
                cout << "  Resume Game" << endl << "  Save  " << endl << "> Exit <" << endl;
                break;
            default:
            break;
        }

        int key = _getch();

        if (key == 27) return;     

        if (key == 0 || key == 224) {
            key = _getch();
            if (key == 72 && pter_pos > 0){
                pter_pos--;
            }else if (key == 80 && pter_pos < 2){
                pter_pos++;
            }
            
        }

        if (key == 13){
            switch (pter_pos){
                case 0:
                    return;
                    break;

                case 1:
                    Savemenu();
                    break;

                case 2:
                    reset();
                    menu();
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
    cout << endl << GameName << endl << "Code Length: " << code_len << "    Remains: " << round_left << "/" << totalround << "    Game Time: " << gametime << " seconds" << endl << endl << "  " ;
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
            cout << "|" << "           " << corr_pos[i] << "                    " << corr_num[i] << endl ;
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
                        cout << endl;
    cout << "You Lost, The correct code is：";
    for (int i = 0; i < code_len; i++) {
        cout << code[i] << " ";
    }
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
                hascheat = true;
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
    y = totalround - round_left;
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


            if (input == 27) {
                gamemenu();
                system("cls");
            }
            
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