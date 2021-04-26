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
    int posicionVar;
    int llavesDisponibles;
    
    string linea;
    string primeraPalabra;
    string tipoVar;
    string nombreVar;
    string valorVarStr1;
    string valorVarStr2;
    string temporal;
 
    char igualdad;
    char operador;
    char valorVarChar;

    double valorVarNum1;   
    double valorVarNum2;
    double valorVarNumT;

    bool isllaveAbierta;
    
public:
    lineReader();
    string leerLinea(string);
private:
    string definirOperacion();
    string abrirLlaves();
    string cerrarLlaves();
    string imprimir();
    string separarDeclaracion();
    string declarar();
    string error();
    
    void guardarInt(int);
    void guardarFloat(float);
    void guardarLong(long);
    void guardarDouble(double);
    void guardarChar(char);

    void popVariable();
    void pushVariable(string, string, int, bool);
    double encontrarNumVar(int);
    char encontrarCharVar(int);
    int encontrarPosVar(string);

};

#endif