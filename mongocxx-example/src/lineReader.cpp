#include <iostream>
#include <sstream>
#include "lineReader.h"


using namespace std;

// Structs
struct lineReader::variable {
    string tipo;
    string nombre;
    void *puntero;
    variable *next;
};

// Constructor
lineReader::lineReader() {
    memoria = (void**) malloc(120);
    numVariables = -1;
    variable *stackVar = NULL;
}

// Funciones
string lineReader::leerLinea(string _linea) {
    linea = _linea;
    return definirOperacion();
}

string lineReader::definirOperacion() {
    istringstream isstream(linea);
    isstream >> primeraPalabra;
    // Abrir llaves
    if (primeraPalabra == "{") {
        return abrirLlaves();
    }
    
    // Cerrar llaves
    else if (primeraPalabra == "}") {
        return cerrarLlaves();
    }
    
    // Cout
    else if (primeraPalabra == "cout") {
        return imprimir();
    }

    // Asignacion
    else if (primeraPalabra == "int" || primeraPalabra == "float" || primeraPalabra == "long" ||
            primeraPalabra == "double" || primeraPalabra == "char") {
        return asignar(primeraPalabra);
    }

    // Error
    else {
        return error();
    }
    
}

string lineReader::abrirLlaves(){
    return "se abrieron llaves";

}

string lineReader::cerrarLlaves(){
    return "se cerraron llaves";

}

string lineReader::imprimir(){
    return "se imprimio algo";

}

string lineReader::asignar(string tipoVar){

    // Separar las partes de la asignacion
    istringstream isstream(linea);
    vueltaWhile = 1;
    while (vueltaWhile != 5) {
        isstream >> temporal;

        switch (vueltaWhile) {
            case 2:
                nombreVar = temporal;
                break;
            case 4:
                valorVar = temporal.substr(0, temporal.length() - 1);
                break;

        }
        vueltaWhile ++;
    }
    
    cout << "Tipo: " << tipoVar << endl;
    cout << "Nombre: " << nombreVar << endl;
    cout << "valor: " << valorVar << endl;
    cout << "----------------------------" << endl;

    numVariables ++;

    // Casos posibles
    if (tipoVar == "int") {

        cout << "Antes de utilizar la memoria: " << endl;
        cout << "La Memoria principal esta en " << memoria << endl;
        cout << "El primer campo esta en " << memoria[0] << endl;
        cout << "----------------------------" << endl;

        memoria[numVariables] = (int*) malloc(sizeof(int));
        *((int*) memoria[numVariables]) = stoi(valorVar); 

        cout << "Una vez usada la memoria: " << endl;
        cout << "La Memoria principal esta en " << memoria << endl;
        cout << "El primer campo esta en " << memoria[0] << " y tiene un valor de " << *((int*) memoria[0]) << endl; 
        cout << "----------------------------" << endl;
        pushVariable(tipoVar, nombreVar, memoria[numVariables]);

        cout << "Una vez ingresada en el stack: " << endl;
        cout << "La Memoria principal esta en " << memoria << endl;
        cout << "El primer campo esta en " << stackVar->puntero << " y tiene un valor de " << *((int*) stackVar->puntero) << endl; 
        cout << "Su nombre es " << stackVar->nombre << " y es un " << stackVar->tipo << endl; 
        cout << "----------------------------" << endl;

        return "todo bien :)";
    }

    else if (tipoVar == "float") {

        memoria[numVariables] = (float*) malloc(sizeof(float));
        *((float*) memoria[numVariables]) = stof(valorVar);

        return "falta";

    }

    else if (tipoVar == "long") {

        memoria[numVariables] = (long*) malloc(sizeof(long));
        *((long*) memoria[numVariables]) = stol(valorVar);

        return "falta";

    }

    else if (tipoVar == "double") {
        memoria[numVariables] = (double*) malloc(sizeof(double));
        *((double*) memoria[numVariables]) = stod(valorVar);
        return "falta";
    }

    else {

        memoria[numVariables] = (char*) malloc(sizeof(char));
        *((char*) memoria[numVariables]) = valorVar[1];
        return "falta";
    }

}

string lineReader::error() {
    return "hubo un error";

}

// Funciones extra
void lineReader::popVariable() {
    variable *aux = stackVar;
    stackVar = stackVar->next;
    free(stackVar->puntero);
    stackVar->puntero = nullptr;
    delete aux;
}

void lineReader::pushVariable(string tipo, string nombre, void *&puntero) {
    variable *nuevoVar = new variable();

    nuevoVar->tipo = tipo;
    nuevoVar->nombre = nombre;
    nuevoVar->puntero = puntero;

    nuevoVar->next = stackVar;
    stackVar = nuevoVar;
}

// void encontrarVariable() {

// }
