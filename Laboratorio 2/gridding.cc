#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>
#include <vector>
#include <bits/stdc++.h>


using namespace std;



_Cormonitor FileReader{
public:
    FileReader(ifstream& archivo, int chunkSize, int numberOfTasks): archivo(archivo), chunkSize(chunkSize), numberOfTasks(numberOfTasks){};
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
    bool isCompletado(){
        return completado;
    }
    bool isCompletadoFinal(){
        return completado_final;
    }
    vector<bool> getPase(){
        return pase;
    }
    int getNumberOfTasks(){
        return numberOfTasks;
    }

    int getIdTarea(){
        return id_tarea;
    }

    bool esMiTurno(int id, vector<bool> pase, int tareas){
        int contador = 0;
        for(int i = 0; i < tareas; i++){
            if(pase[i] == false){
                contador = contador + 1;
            }
        }
        if((contador == (tareas - 1)) && (pase[id] == true)){
            cout << "Es el turno de: " << id << endl;
            return true;
        }
        return false;
    }

private:
    ifstream& archivo;
    
    vector<string> lineas;

    string linea;
    bool completado = false;
    bool completado_final = false;
    
    int chunkSize;
    int numberOfTasks;
    int tareas = numberOfTasks;
    vector<bool> pase;
    int id_tarea = 0;
    int contador = 0;
    void main(){
        int i = 0;
        for (int j = 0; j < numberOfTasks; j++) {
            pase.push_back(false);
        }
        while (getline(archivo, linea)) {
            pase[contador] = true;
            
            lineas.push_back(linea);
            if (archivo.eof()) {
                completado_final = true;
                 archivo.close();
                suspend();
                break;
            }
            i = i + 1;
            if (i >= chunkSize) {
                id_tarea = id_tarea + 1;
                i = 0;
                pase[contador] = false;
                contador = contador + 1;
                if(contador >= numberOfTasks){
                    contador = 0;
                    id_tarea = 0;
                    pase[contador] = true;
                }
                else{
                    pase[contador] = true;
                }
                cout << "suspend" << endl;

                if (archivo.eof()) {
                    completado_final = true;
                     archivo.close();
                     suspend();
                    break;
                }
                suspend();
                lineas.clear();
            }
            //cout << linea << endl;

        }

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
    bool completado = false;
    void main(){
        string linea_actual;
        vector<string> elementos;
        double u_k, v_k, vr, vi, w, frec, ce;
        while(f->getIdTarea() != id){
                //cout << "atascada: " << id << endl;
        }
        while(!completado){
            if(f->isCompletadoFinal()){
                break;
            }
            cout << "procesador: " << id << endl;
            f->siguiente();
            completado = true;
            lineas = f->getLineas();
            for(int i = 0; i < lineas.size(); i++){
                linea_actual = lineas[i];
                stringstream ss(linea_actual);
                while(ss.good()){
                    string substr;
                    getline(ss, substr, ',');
                    elementos.push_back(substr);
                }
                u_k = stod(elementos[0]);
                v_k = stod(elementos[1]);
                vr = stod(elementos[3]);
                vi = stod(elementos[4]);
                w = stod(elementos[5]);
                frec = stod(elementos[6]);
                ce = stod(elementos[7]);
                //cout << "linea: " << i << lineas[i] << endl;
            }
            cout << "proceso terminado: " << id << endl;    
            for (int i = 0; i < lineas.size(); i++) {
                cout << "linea: "<< i << lineas[i] << endl;
                }
            lineas.clear();
            while(completado){
                if(f->esMiTurno(id, f->getPase(), f->getNumberOfTasks())){
                    completado = false;
                }
                if(f->isCompletadoFinal()){
                    break;
            }
                
                //cout << "atascada: " << id << endl;
            }

        }
    }
};

int main(int argc, char *argv[]) {
    string input_file = "";
    char *output_directory = NULL;
    double deltax = 0.0;
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
                deltax = atof(optarg);
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
                fprintf(stderr, "Uso: %s -i datosuv.raw -o datosgrideados -d deltax -N tamañoimagen -c chunklectura -t numerotareas\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }


    std::cout << input_file << std::endl;
    printf("Output directory: %s\n", output_directory);
    printf("Deltau: %lf\n", deltax);
    printf("Image size: %d\n", image_size);
    printf("Chunk size: %d\n", chunk_size);
    printf("Number of tasks: %d\n", num_tasks);

    ifstream archivo(input_file.c_str());
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo" << endl;
    }
    /*
    string test = "hola, esto es, una prueba";
    stringstream ss(test);
    string subtr;
    getline(ss, subtr, ',');
    cout << "resultado: " << subtr << endl;
    */
    
    FileReader* FileReaderPtr = new FileReader(archivo, chunk_size, num_tasks);
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
