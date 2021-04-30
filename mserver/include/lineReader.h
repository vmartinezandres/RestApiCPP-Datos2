/**
 *  Jose Julian Solano Quesada
 *  lineReader.h file
 */

#ifndef MONGOCXX-EXAMPLE_LINEREADER_H
#define MONGOCXX-EXAMPLE_LINEREADER_H

#include <string>

using namespace std;

/**
 * @class linereader
 * @brief clase donde se crean todas las variables asociadas a leer lo que se envia desde el IDE
 * @htmlinclude YES
 */

class lineReader {    
private:
    struct variable; /*!<caracter que se esta leyendo actualmente */

    variable *stackVar;
    variable *varTemp;
    
    void **memoria;
    
    int numVariables;
    int vueltaWhile;
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
    string separarAsignacion();
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