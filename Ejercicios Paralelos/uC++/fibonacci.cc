#include <iostream>
#include <uC++.h>
using namespace std;

_Coroutine fibonacci {
    int fn;
    void main() {
        int fn1, fn2;
        fn = 1; fn1 = fn;
        suspend();//return to last resume
        fn = 1; fn2 = fn1; fn1 = fn;
        suspend();//return to last resume
        for ( ;; ) {
            fn = fn1 + fn2;
            suspend();//return to last resume
            fn2 = fn1; fn1 = fn;
        }
    }
    public:
        int next() {
            resume();//transfer to last suspend
            return fn;
        }
};


int main() {
    fibonacci f1, f2;
    for (int i=1; i <= 6; i++) {
        cout << f1.next() << " " << f2.next() << endl;
    }
    return 0;
}