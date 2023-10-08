#include <iostream>


using namespace std;

_Coroutine lectura{
    FILE *archivo;

    void main(){
        archivo = fopen("hltau_completo_uv.csv", "r");
        char c;
        while((c = fgetc(archivo)) != EOF){
            cout << c;
            suspend();
        }
        fclose(archivo);
    }

    public:
        void siguiente(){
            resume();
        }
};

int main(){
    lectura l;
    l.siguiente();
    return 0;
}