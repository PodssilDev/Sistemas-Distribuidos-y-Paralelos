#include <iostream>
#include <fstream>
#include <string>


using namespace std;


_Coroutine FileReader{
public:
        // constructor
        FileReader(){};
        ~FileReader(){};
        void siguiente(){
            resume();
        }
        
private:
  FILE *archivo;

    void main(){
    ifstream archivo("hltau_completo_uv.csv");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo" << endl;
    }

    string linea;
    while (getline(archivo, linea)) {
        cout << linea << endl;
        suspend();
    }

    archivo.close();

    }
};


_Task Procesador{
public:
    Procesador(FileReader* f): f(f){};
    ~Procesador(){};
private:
    FileReader* f;
    void main(){
            cout << "Procesando: " << 1 << endl;
            f->siguiente();
        }
};

int main(){
    cout << "Hola" << endl;
    FileReader* FileReaderPtr = new FileReader();
    Procesador** procesadores = new Procesador*[1];
    procesadores[0] = new Procesador(FileReaderPtr);
    
    return 0;
}
