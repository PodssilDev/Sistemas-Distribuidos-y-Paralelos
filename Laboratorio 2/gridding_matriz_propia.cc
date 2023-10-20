#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>
#include <vector>
#include <bits/stdc++.h>
#include <uC++.h>
#include <cmath>
#include <uBarrier.h>
#include <time.h>

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
            //cout << "Entro: " << id << endl;

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
        while(true){
        while (getline(archivo, linea)) { 
            lineas.push_back(linea);
            contador_lineas = contador_lineas + 1;
            //cout << "Procesada: " << contador_lineas << endl;
            if (archivo.eof()) {
                completado_final = true;
                archivo.close();
                break;
            }
            i = i + 1;
            if (i >= chunkSize) {
                i = 0;
                //cout << "suspend" << endl;
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
        lineas.clear();
        }

    }
};

_Task Procesador_Matriz{
public:
    Procesador_Matriz(int procesadorID, FileReader* f, double delta_x, int N): id(procesadorID), f(f), delta_x(delta_x), N(N){
        matrix_real = new double*[N];
        for(int i = 0; i < N; i++){
            matrix_real[i] = new double[N];
            for(int j = 0; j < N; j++){
                matrix_real[i][j] = 0.0;
            }
        }
        matrix_imag = new double*[N];
        for(int i = 0; i < N; i++){
            matrix_imag[i] = new double[N];
            for(int j = 0; j < N; j++){
                matrix_imag[i][j] = 0.0;
            }
        }
        matrix_peso = new double*[N];
        for(int i = 0; i < N; i++){
            matrix_peso[i] = new double[N];
            for(int j = 0; j < N; j++){
                matrix_peso[i][j] = 0.0;
            }
        }
    };
    ~Procesador_Matriz(){
        for(int i = 0; i < N; i++){
            delete matrix_real[i];
        }
        delete matrix_real;
        for(int i = 0; i < N; i++){
            delete matrix_imag[i];
        }
        delete matrix_imag;
        for(int i = 0; i < N; i++){
            delete matrix_peso[i];
        }
        delete matrix_peso;
    };
    double** matrix_real;
    double** matrix_imag;
    double** matrix_peso;
    void setReal(int i, int j, double value){
        matrix_real[i][j] = value;
    };
    void setImag(int i, int j, double value){
        matrix_imag[i][j] = value;
    };
    void setPeso(int i, int j, double value){
        matrix_peso[i][j] = value;
    };

    double getReal(int i, int j){
        return matrix_real[i][j];
    };
    double getImag(int i, int j){
        return matrix_imag[i][j];
    };
    double getPeso(int i, int j){
        return matrix_peso[i][j];
    };
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
        double i_k, j_k, calculo_real, calculo_imag, calculo_peso;
        string linea_actual;
        vector<string> elementos;
        double u_k, v_k, vr, vi, w, frec, ce;
        //cout << "Soy la tarea: " << id << endl;
        delta_u = 1/(N*delta_x);
        delta_v = delta_u;
        while(true){
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
                //cout << "i_k: " << i_k << endl;
                j_k = round(v_k / delta_v) + (N/2);
                calculo_real = getReal(i_k, j_k) + (vr * w);
                //cout << "calculo real: " << calculo_real << endl;
                calculo_imag = getImag(i_k, j_k) + (vi * w);
                calculo_peso = getPeso(i_k, j_k) + w;
                setReal(i_k, j_k, calculo_real);
                setImag(i_k, j_k, calculo_imag);
                setPeso(i_k, j_k, calculo_peso);
                elementos.clear();
                
            }
           // cout << "proceso terminado: " << id << endl;    
           /*
            for (int i = 0; i < lineas.size(); i++) {
                //cout << "linea: "<< i << lineas[i] << endl;
                }
            lineas.clear();
            */
                //cout << "atascada: " << id << endl;
            }
       // cout << "Me voy. Soy la tarea: " << id << endl;

        }
};

int main(int argc, char *argv[]) {
    string input_file = "";
    string output_directory = "";
    string output_directory_r = "";
    string output_directory_i = "";
    double delta_x = 0.0;
    int N = 0;
    int chunk_size = 0;
    int num_tasks = 0;
    unsigned t0, t1;
    double time2;


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


    cout << input_file << endl;
    cout << output_directory << endl;
    cout << delta_x << endl;
    cout << N << endl;
    cout << chunk_size << endl;
    cout << num_tasks << endl;
    //b = new uBarrier(num_tasks);

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
    Procesador_Matriz** procesadores_matriz = new Procesador_Matriz*[num_tasks];
    t0 = clock(); 
    for (int i = 0; i < num_tasks; i++) {
        procesadores_matriz[i] = new Procesador_Matriz(i, FileReaderPtr, delta_x, N);
        cout << "Tarea creada: " << i << endl;
    }

    //matrix->setReal(0, 0, 1.0);
    //matrix->printMatrix();
    double** matrix_real;
    double** matrix_imag;
    double** matrix_peso;
    matrix_real = new double*[N];
        for(int i = 0; i < N; i++){
            matrix_real[i] = new double[N];
            for(int j = 0; j < N; j++){
                matrix_real[i][j] = 0.0;
            }
        }
    matrix_imag = new double*[N];
        for(int i = 0; i < N; i++){
            matrix_imag[i] = new double[N];
            for(int j = 0; j < N; j++){
                matrix_imag[i][j] = 0.0;
            }
        }
    matrix_peso = new double*[N];
        for(int i = 0; i < N; i++){
            matrix_peso[i] = new double[N];
            for(int j = 0; j < N; j++){
                matrix_peso[i][j] = 0.0;
            }
        }
    for(int k = 0; k < num_tasks; k++){
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                    matrix_real[i][j] = matrix_real[i][j] + procesadores_matriz[k]->getReal(i, j);
                    matrix_imag[i][j] = matrix_imag[i][j] + procesadores_matriz[k]->getImag(i, j);
                    matrix_peso[i][j] = matrix_peso[i][j] + procesadores_matriz[k]->getPeso(i, j);
            }
        }
    }

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(matrix_peso[i][j] != 0.0){
            matrix_real[i][j] = matrix_real[i][j] / matrix_peso[i][j];
            matrix_imag[i][j] = matrix_imag[i][j] / matrix_peso[i][j];
        }
    }

    }   
    t1 = clock();
    time2 = (double)(t1 - t0) / CLOCKS_PER_SEC; // Cálculo del tiempo de ejecución
    cout << "Terminado: " << time2 << "[s]" << endl;

    output_directory_r = output_directory + "rm.raw";
    output_directory_i = output_directory + "im.raw";
    ofstream archivo_out(output_directory_r.c_str(), ios::out | ios::binary);

    if (!archivo_out) {
        std::cerr << "No se pudo abrir el archivo " << std::endl;
        return 1;
    }

    // Escribir los datos de la matriz en el archivo .raw
    for (int i = 0; i < N; i++) {
        archivo_out.write(reinterpret_cast<char*>(matrix_real[i]), N * sizeof(double));
    }

    // Cerrar el archivo
    archivo_out.close();

    ofstream archivo_out2(output_directory_i.c_str(), ios::out | ios::binary);
    if(!archivo_out2){
        std::cerr << "No se pudo abrir el archivo " << std::endl;
        return 1;
    }
    for (int i = 0; i < N; i++) {
        archivo_out2.write(reinterpret_cast<char*>(matrix_imag[i]), N * sizeof(double));
    }
    archivo_out2.close();

    for (int i = 0; i < num_tasks; i++) {
        delete procesadores_matriz[i];
    }
    delete procesadores_matriz;
    delete FileReaderPtr;
    for(int i = 0; i < N; i++){
            delete matrix_real[i];
        }
        delete matrix_real;
        for(int i = 0; i < N; i++){
            delete matrix_imag[i];
        }
        delete matrix_imag;
        for(int i = 0; i < N; i++){
            delete matrix_peso[i];
        }
        delete matrix_peso;
    return 0;
}