#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>
#include <vector>
#include <bits/stdc++.h>
#include <uC++.h>
#include <cmath>
#include <time.h>

using namespace std;

/*
Laboratorio 2 - Sistemas Distribuidos y paralelos 13329 2-2023
Autor: John Serrano Carrasco
Solucion: Matriz propia de cada tarea
*/

// Definciion de constantes
#define light_speed 299792458

/*
La clase Reader es la encargada de leer el archivo de entrada y entregar las lineas de este a las tareas
para su procesamiento. Esta clase es de tipo Mutex Coroutine y cuenta con un atributo ifstream que es el archivo
de entrada. Ademas, cuenta con un atributo chunkSize que indica la cantidad de lineas que se le entregaran
a cada tarea y un atributo numberOfTasks que indica la cantidad de tareas que se creadas. La clase cuenta
con un metodo siguiente que es el encargado de entregar activar el main de la corrutina y un metodo main que es
el encargado de leer el archivo y entregar las lineas a las tareas
*/
_Mutex _Coroutine Reader{
public:
    /* 
    Entradas: Un ifstream que es el archivo de entrada, un entero chunkSize que indica la cantidad de lineas
    que se le entregaran a cada tarea y un entero numberOfTasks que indica la cantidad de tareas que se crearan
    Salida: Ninguna
    Descripcion: Constructor de la clase, recibe el archivo de entrada, el tamaño del chunk y la cantidad de tareas
    */
    Reader(ifstream& archivo, int chunkSize, int numberOfTasks): archivo(archivo), chunkSize(chunkSize), numberOfTasks(numberOfTasks){};

    /*
    Entradas: Ninguna
    Salida: Ninguna
    Descripcion: Destructor de la clase
    */
    ~Reader(){};

    /*
    Entrada: Ninguna
    Salida: Un vector de strings que contiene las lineas que se le entregaran a la tarea
    Descripcion: Metodo que se encarga de entregar activar el main de la corrutina si es que no se ha leido
    todo el archivo y entregar un vector que contiene las lineas leidas o de entregar un vector vacio si es 
    que ya se leyo todo el archivo
    */
    vector<string> siguiente(){
        if (completado_final){
            lineas.clear();
            return lineas;
        }
        else{
            resume();
            return lineas;
        }
    };
    

private:
    ifstream& archivo; // Archivo de entrada
    vector<string> lineas; // Vector que contiene las lineas leidas
    string linea; // Linea leida
    bool completado_final = false; // Booleano que indica si se leyo todo el archivo
    int chunkSize; // Tamaño del chunk
    int numberOfTasks; // Cantidad de tareas
    void main(){
        int i = 0; // Contador de lineas leidas
        while(true){
            while (getline(archivo, linea)) {
                lineas.push_back(linea); // Se agrega la linea al vector
                if (archivo.eof()) { // Si se llego al final del archivo se sale del ciclo
                    completado_final = true;
                    archivo.close(); // Se cierra el archivo
                    break;
                }
                i = i + 1;
                if (i >= chunkSize) { // Si se llego al tamaño del chunk 
                    i = 0; // Se reinicia el contador
                    if (archivo.eof()) { // Si se llego al final del archivo se sale del ciclo
                        completado_final = true;
                        archivo.close();  // Se cierra el archivo
                        break;
                    }
                    suspend(); // La tarea actual sale de la corrutina para procesar el chunk
                    lineas.clear(); // Se limpia el vector de lineas para que la proxima tarea que entre a la corrutina pueda leer el chunk
                }
            }
            suspend(); // Si se llgo aqui es porque se leyo todo el archivo y se sale de la corrutina
            lineas.clear(); // Se entrega un vector vacio si ya se leyo todo el archivo
        }
    }
};

/*
La clase Gridder es la encargada de procesar los datos entregados por la clase Reader. Esta clase es de tipo
_Task y cuenta con un atributo id que indica el id de la tarea, un atributo r que es un puntero a la clase Reader,
un atributo delta_x que indica el tamaño de la celda y un atributo N que indica el tamaño de la matriz. Ademas,
cuenta con un atributo matrix_real que es un puntero a la matriz real de la clase Matrix, un atributo matrix_imag
que es un puntero a la matriz imaginaria de la clase Matrix y un atributo matrix_peso que es un puntero a la matriz
peso de la clase Matrix. La clase cuenta con un metodo main que es el encargado de procesar los datos
entregados por la Corrutina Reader
*/
_Task Gridder{
public:
    /*
    Entradas: Un entero id que indica el id de la tarea, un puntero a la corrutina Reader, un double delta_x
    que indica el tamaño de la celda, un entero N que indica el tamaño de la matriz
    Salida: Ninguna
    Descripcion: Constructor de la clase, recibe el id de la tarea, un puntero a la corrutina Reader, el tamaño de la celda y
    el tamaño de la matriz. Crea las matrices y las inicializa en 0.0
    */
    Gridder(int id, Reader* r, double delta_x, int N): id(id), r(r), delta_x(delta_x), N(N){
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

    /* 
    Entradas: Ninguna
    Salida: Ninguna
    Descripcion: Destructor de la clase. Destruye las matrices
    */
    ~Gridder(){
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

    /*
    Metodos set
    Entradas: Un entero i y un entero j que indican la posicion de la matriz a modificar y un valor de tipo double
    que indica el valor a modificar
    Salida: Ninguna
    Descripcion: Metodos que modifican el valor de la matriz en la posicion i, j por el valor value
    */
    void setReal(int i, int j, double value){
        matrix_real[i][j] = value;
    };
    void setImag(int i, int j, double value){
        matrix_imag[i][j] = value;
    };
    void setPeso(int i, int j, double value){
        matrix_peso[i][j] = value;
    };
    
    /*
    Metodos get
    Entradas: Un entero i y un entero j que indican la posicion de la matriz a consultar
    Salida: Un valor de tipo double que indica el valor de la matriz en la posicion i, j
    Descripcion: Metodos que retornan el valor de la matriz en la posicion i, j
    */
    double getReal(int i, int j){
        return matrix_real[i][j];
    };
    double getImag(int i, int j){
        return matrix_imag[i][j];
    };
    double getPeso(int i, int j){
        return matrix_peso[i][j];
    }

private:
    int id; // Id de la tarea
    Reader* r; // Puntero a la corrutina
    double delta_x; // Tamaño de la celda
    int N; // Tamaño de la matriz
    vector <string> lineas; // Vector que contiene las lineas leidas

    void main(){
        // Inicializacion de variables
        double delta_u = 0.0; 
        double delta_v = 0.0;
        double i_k, j_k, calculo_real, calculo_imag, calculo_peso;
        string linea_actual;
        vector<string> elementos; // Vector que contiene los elementos de una linea
        double u_k, v_k, vr, vi, w, frec, ce; // Variables que se obtienen de la linea
        delta_u = 1/(N*delta_x); // Se calcula el delta_u
        delta_v = delta_u; // Del enunciado se asume delta_u = delta_v

        while(true){
            lineas = r->siguiente(); // La tarea entra a la corrutina y obtiene sus lineas
            if(lineas.empty()){ // Si el vector esta vacio es porque ya se leyo todo el archivo
                break;
            }
            for(int i = 0; i < lineas.size(); i++){
                linea_actual = lineas[i]; // Se obtiene una linea de las lineas leidas
                stringstream substring(linea_actual); // Se crea un stringstream para separar los elementos de la linea
                while(substring.good()){ // Se separan los elementos de la linea y se guardan en el vector elementos
                    string substr;
                    getline(substring, substr, ',');
                    elementos.push_back(substr);
                }
                // Se obtienen los valores de la linea y se guardan en las variables correspondientes
                u_k = stod(elementos[0]);
                v_k = stod(elementos[1]);
                vr = stod(elementos[3]);
                vi = stod(elementos[4]);
                w = stod(elementos[5]);
                frec = stod(elementos[6]);

                // Se calcula u_k y v_k de acuerdo al enunciado
                u_k = u_k * (frec / light_speed);
                v_k = v_k * (frec / light_speed);

                // Se calcula i_k y j_k de acuerdo al enunciado
                i_k = round(u_k / delta_u) + (N/2);
                j_k = round(v_k / delta_v) + (N/2);
                
                // Se calculan los valores de la matriz real, imaginaria y peso de acuerdo al enunciado
                calculo_real = getReal(i_k, j_k) + (vr * w);
                calculo_imag = getImag(i_k, j_k) + (vi * w);
                calculo_peso = getPeso(i_k, j_k) + w;

                // Se guardan los valores calculados en las matrices en las posiciones correspondientes
                setReal(i_k, j_k, calculo_real);
                setImag(i_k, j_k, calculo_imag);
                setPeso(i_k, j_k, calculo_peso);

                // Se limpia el vector elementos para la proxima linea
                elementos.clear();
            }
        }
    }
};

// Main principal del programa
int main(int argc, char *argv[]) {
    // Inicializacion de variables
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
                input_file = optarg; // Nombre del archivo de entrada
                break;
            case 'o':
                output_directory = optarg; // Nombre del archivo de salida
                break;
            case 'd':
                delta_x = atof(optarg); // Tamaño de la celda
                break;
            case 'N':
                N = atoi(optarg); // Tamaño de la matriz
                break;
            case 'c':
                chunk_size = atoi(optarg); // Tamaño del chunk
                break;
            case 't':
                num_tasks = atoi(optarg); // Cantidad de tareas
                break;
            default:
                fprintf(stderr, "Uso: %s -i datosuv.raw -o datosgrideados -d delta_x -N tamañoimagen -c chunklectura -t numerotareas\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    // Se abre el archivo de entrada
    ifstream archivo(input_file.c_str());
    if (!archivo.is_open()) { //
        cout << "No se pudo abrir el archivo" << endl;
        exit(EXIT_FAILURE);
    }
    
    // Se crean las matrices de la parte real, imaginaria y peso. Estas matrices se van a ir actualizando cuando las tareas terminen
    // de realizar todos sus calculos
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
    
    Reader* ReaderPtr = new Reader(archivo, chunk_size, num_tasks); // Se crea la corrutina Reader
    delta_x = ((M_PI)/(3600 * 180)) * delta_x; // Se calcula el valor de delta_x en radianes
    Gridder** Gridders = new Gridder*[num_tasks]; // Se crea un arreglo de tareas Gridder

    t0 = clock(); // Se comienza a contar el tiempo de ejecucion
    for (int i = 0; i < num_tasks; i++) { // Se crean las tareas Gridder
        Gridders[i] = new Gridder(i, ReaderPtr, delta_x, N);
        cout << "Tarea creada: " << i << endl;
    }
    
    // Se procede a sumar las matrices de cada tarea para obtener la matriz final
    for(int k = 0; k < num_tasks; k++){
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                    matrix_real[i][j] = matrix_real[i][j] + Gridders[k]->getReal(i, j);
                    matrix_imag[i][j] = matrix_imag[i][j] + Gridders[k]->getImag(i, j);
                    matrix_peso[i][j] = matrix_peso[i][j] + Gridders[k]->getPeso(i, j);
            }
        }
    }

    // Se realiza la normalizacion de la matriz real e imaginaria
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(matrix_peso[i][j] != 0.0){
            matrix_real[i][j] = matrix_real[i][j] / matrix_peso[i][j];
            matrix_imag[i][j] = matrix_imag[i][j] / matrix_peso[i][j];
            }
        }
    }

    t1 = clock(); // Se termina de contar el tiempo de ejecucion
    time2 = (double)(t1 - t0) / CLOCKS_PER_SEC; // Cálculo del tiempo de ejecución
    cout << "Tiempo del proceso de gridding con matriz propia: " << time2 << "[s]" << endl; // Se imprime el tiempo de ejecucion

    output_directory_r = output_directory + "r_mp.raw"; // Se crea el nombre del archivo de salida de la parte real
    ofstream archivo_r(output_directory_r.c_str(), ios::out | ios::binary); // Se abre el archivo de salida de la parte real
    if (!archivo_r) { // Si no se pudo abrir el archivo se termina el programa
        std::cerr << "No se pudo abrir el archivo " << std::endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N; i++) { // Escribir los datos de la matriz real en el archivo .raw
        archivo_r.write(reinterpret_cast<char*>(matrix_real[i]), N * sizeof(double));
    }
    archivo_r.close();   // Se cierra el archivo de salida
    cout << "Archivo de salida de la parte real creado" << endl;

    output_directory_i = output_directory + "i_mp.raw"; // Se crea el nombre del archivo de salida de la parte imaginaria
    ofstream archivo_i(output_directory_i.c_str(), ios::out | ios::binary); // Se abre el archivo de salida de la parte imaginaria
    if(!archivo_i){ // Si no se pudo abrir el archivo se termina el programa
        std::cerr << "No se pudo abrir el archivo " << std::endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N; i++) { // Escribir los datos de la matriz imaginaria en el archivo .raw
        archivo_i.write(reinterpret_cast<char*>(matrix_imag[i]), N * sizeof(double));
    }
    archivo_i.close(); // Se cierra el archivo de salida
    cout << "Archivo de salida de la parte imaginaria creado" << endl;

    // Se elimina toda la memoria reservada del programa   
    for (int i = 0; i < num_tasks; i++) {
        delete Gridders[i];
    }
    delete Gridders;
    delete ReaderPtr;
    
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