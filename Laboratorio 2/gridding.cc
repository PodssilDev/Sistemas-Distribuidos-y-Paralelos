#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>
#include <vector>


using namespace std;



_Cormonitor FileReader{
public:
    FileReader(ifstream& archivo, int chunkSize): archivo(archivo), chunkSize(chunkSize){};
    ~FileReader(){};
    void siguiente(){
        resume();
    }
    vector<string> getLineas(){
        return lineas;
    }
    string getLinea(){
        return linea;
    }


private:
    ifstream& archivo;
    
    vector<string> lineas;

    string linea;
    
    int chunkSize;
    void main(){
        cout << "test2" << endl;
        int i = 0;
        while (getline(archivo, linea)) {
            lineas.push_back(linea);
            i = i + 1;
            if (i == chunkSize) {
                i = 0;
                suspend();
                lineas.clear();
            }
            //cout << linea << endl;

        }
        archivo.close();
    }
};

_Task Procesador{
public:
    Procesador(int procesadorID, FileReader* f): id(procesadorID), f(f){};
    ~Procesador(){};
private:
    int id;
    FileReader* f;
    vector <string> lineas;
    void main(){
        cout << "Procesando: " << id << endl;
        f->siguiente();
        cout << "test" << endl;
        lineas = f->getLineas();
        for (int i = 0; i < lineas.size(); i++) {
            cout << "linea: "<< i << lineas[i] << endl;
        }
    }
};

int main(int argc, char *argv[]) {
    string input_file = "";
    char *output_directory = NULL;
    double deltau = 0.0;
    int image_size = 0;
    int chunk_size = 0;
    int num_tasks = 0;

    int opt;
    while ((opt = getopt(argc, argv, "i:o:d:N:c:t:")) != -1) {
        switch (opt) {
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_directory = optarg;
                break;
            case 'd':
                deltau = atof(optarg);
                break;
            case 'N':
                image_size = atoi(optarg);
                break;
            case 'c':
                chunk_size = atoi(optarg);
                break;
            case 't':
                num_tasks = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Uso: %s -i datosuv.raw -o datosgrideados -d deltau -N tamañoimagen -c chunklectura -t numerotareas\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }


    std::cout << input_file << std::endl;
    printf("Output directory: %s\n", output_directory);
    printf("Deltau: %lf\n", deltau);
    printf("Image size: %d\n", image_size);
    printf("Chunk size: %d\n", chunk_size);
    printf("Number of tasks: %d\n", num_tasks);

    ifstream archivo(input_file.c_str());
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo" << endl;
    }
    
    FileReader* FileReaderPtr = new FileReader(archivo, chunk_size);
    //FileReaderPtr->siguiente();
    //vector <string> lineas = FileReaderPtr->getLinea();
    //FileReaderPtr->siguiente();
    //string linea = FileReaderPtr->getLinea();
    //FileReaderPtr->siguiente();
    
    Procesador** procesadores = new Procesador*[num_tasks];
    for (int i = 0; i < num_tasks; i++) {
        procesadores[i] = new Procesador(i, FileReaderPtr);
    }
    for (int i = 0; i < num_tasks; i++) {
        procesadores[i]->~Procesador();
    }
    delete procesadores;
    delete FileReaderPtr;

    return 0;
}
