#include <iostream>
#include <uC++.h>

using namespace std;

_Coroutine FormatInput {
    char ch;
    int g, b;
    void main() {
        for (;;) {
            for (g=0; g<5; g++) {
                for (b=0; b<4; b++) {
                    suspend();
                    cout << ch;
                }
                cout << " ";
            }
            cout << endl;
        }
    }
    
    public:
        FormatInput() { resume(); }
        ~FormatInput() {
            if (g != 0 || b != 0)
            cout << endl;
        }
        void prt(char ch) {
            FormatInput::ch = ch;
            resume();
        }
};

int main() {
    FormatInput fmt;
    char ch;
    for (;;) {
        cin >> ch;
        if (cin.fail()) break;
        fmt.prt(ch);
    }
    return 0;
}