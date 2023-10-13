#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>
#include <vector>
#include <bits/stdc++.h>
#include <uC++.h>
#include <cmath>

using namespace std;

#define light_speed 299792458

_Mutex _Coroutine FileReader{
public:
    FileReader(ifstream& archivo, int chunkSize, int numberOfTasks): archivo(archivo), chunkSize(chunkSize), numberOfTasks(numberOfTasks){};
    ~FileReader(){};

    vector<string> siguiente(int id){
        if (completado_final){
            lineas.clear();
            return lineas;
        }
        else{
            resume();
            cout << "Entro: " << id << endl;

            return lineas;
        }
    };

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

    int getNumberOfTasks(){
        return numberOfTasks;
    }

    int getIdTarea(){
        return id_tarea;
    }
    int contador_lineas = 0;

    int getContador_lineas(){
        return contador_lineas;
    }
    /*
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
    */

private:
    ifstream& archivo;
    
    vector<string> lineas;

    string linea;
    bool completado = false;
    bool completado_final = false;
    
    int chunkSize;
    int numberOfTasks;
    int tareas = numberOfTasks;
    int id_tarea = 0;
    int contador = 0;
    void main(){
        int i = 0;
        while (getline(archivo, linea)) { 
            lineas.push_back(linea);
            contador_lineas = contador_lineas + 1;
            cout << "Procesada: " << contador_lineas << endl;
            if (archivo.eof()) {
                completado_final = true;
                archivo.close();
                break;
            }
            i = i + 1;
            if (i >= chunkSize) {
                i = 0;
                cout << "suspend" << endl;
                if (archivo.eof()) {
                    completado_final = true;
                     archivo.close();
                     break;
                }
                suspend();
                lineas.clear();
            }
            //cout << linea << endl;
        }
        suspend();

    }
};

_Task Procesador{
public:
    Procesador(int procesadorID, FileReader* f, double delta_x, int N): id(procesadorID), f(f), delta_x(delta_x), N(N){};
    ~Procesador(){};
private:
    int id;
    FileReader* f;
    double delta_x;
    int N;
    vector <string> lineas;
    bool completado = false;
    void main(){
        double delta_u = 0.0;
        double delta_v = 0.0;
        double i_k, j_k;
        string linea_actual;
        vector<string> elementos;
        double u_k, v_k, vr, vi, w, frec, ce;
        //cout << "Soy la tarea: " << id << endl;
        delta_u = 1/(N*delta_x);
        delta_v = delta_u;
        while(!completado){
            //cout << "Soy la tarea: " << id << endl;

            //cout << "procesador: " << id << endl;
            lineas = f->siguiente(id);
            if(lineas.empty()){
                break;
            }
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
                u_k = u_k * (frec / light_speed);
                v_k = v_k * (frec / light_speed);
                i_k = round(u_k / delta_u) + (N/2);
                cout << "i_k: " << i_k << endl;
                j_k = round(v_k / delta_v) + (N/2);
                elementos.clear();
                
            }
           // cout << "proceso terminado: " << id << endl;    
            for (int i = 0; i < lineas.size(); i++) {
                cout << "linea: "<< i << lineas[i] << endl;
                }
            lineas.clear();
                //cout << "atascada: " << id << endl;
            }
       // cout << "Me voy. Soy la tarea: " << id << endl;

        }
};

int main(int argc, char *argv[]) {
    string input_file = "";
    char *output_directory = NULL;
    double delta_x = 0.0;
    int N = 0;
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
                delta_x = atof(optarg);
                break;
            case 'N':
                N = atoi(optarg);
                break;
            case 'c':
                chunk_size = atoi(optarg);
                break;
            case 't':
                num_tasks = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Uso: %s -i datosuv.raw -o datosgrideados -d delta_x -N tamañoimagen -c chunklectura -t numerotareas\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }


    std::cout << input_file << std::endl;
    printf("Output directory: %s\n", output_directory);
    printf("Delta x: %lf\n", delta_x);
    printf("Image size: %d\n", N);
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
    delta_x = ((M_PI)/(3600 * 180)) * delta_x;
    
    Procesador** procesadores = new Procesador*[num_tasks];
    for (int i = 0; i < num_tasks; i++) {
        procesadores[i] = new Procesador(i, FileReaderPtr, delta_x, N);
        cout << "Tarea creada: " << i << endl;
    }
    for (int i = 0; i < num_tasks; i++) {
        delete procesadores[i];
    }
    delete procesadores;
    delete FileReaderPtr;
    
    return 0;
}
