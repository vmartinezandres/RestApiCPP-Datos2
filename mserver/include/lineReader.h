#ifndef MONGOCXX-EXAMPLE_LINEREADER_H
#define MONGOCXX-EXAMPLE_LINEREADER_H

#include <string>

using namespace std;

class lineReader {    
private:
    struct variable;

    variable *stackVar;
    variable *varTemp;
    
    void **memoria;
    
    int numVariables;
    int vueltaWhile;
    int llavesDisponibles;
    int posParentesis;
    
    string linea;
    string primeraPalabra;
    string tipoVar;
    string nombreVar;
    string valorVarStr1;
    string valorVarStr2;
    string temporal;
    string nada = "";
    string address;
 
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
    string clear();
    string memory();

private:
    string definirOperacion();
    string abrirLlaves();
    string cerrarLlaves();
    string imprimir();
    string separarAsignacion(char);
    string declarar();
    string asignar();
    string error();
    
    void guardarInt(int);
    void guardarFloat(float);
    void guardarLong(long);
    void guardarDouble(double);
    void guardarChar(char);
    
    variable* encontrarVar(string);
    double valorVarNum(string, int);

    void pushVariable(string, string, int, bool);
    void popVariable();
};

#endif