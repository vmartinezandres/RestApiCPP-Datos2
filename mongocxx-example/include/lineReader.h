#ifndef MONGOCXX-EXAMPLE_LINEREADER_H
#define MONGOCXX-EXAMPLE_LINEREADER_H

#include <string>

using namespace std;

class lineReader {    
private:
    struct variable;
    variable *stackVar;
    void **memoria;
    int numVariables;
    int vueltaWhile;
    string linea;
    string primeraPalabra;
    string nombreVar;
    string valorVar;
    string temporal;
public:
    lineReader();
    string leerLinea(string);
private:
    string definirOperacion();
    string abrirLlaves();
    string cerrarLlaves();
    string imprimir();
    string asignar(string);
    string error();
    
    void guardarInt(int);
    void guardarFloat(float);
    void guardarLong(long);
    void guardarDouble(double);
    void guardarChar(char);

    void popVariable();
    void pushVariable(string tipo, string nombre, void *&puntero);

};

#endif