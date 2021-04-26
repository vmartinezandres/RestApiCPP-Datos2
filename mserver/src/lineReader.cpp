#include <iostream>
#include <sstream>
#include "lineReader.h"


using namespace std;

// Structs
struct lineReader::variable {
    string tipo;
    string nombre;
    int posicion;
    bool comienzoLlaves;
    variable *next;
};

// Constructor
lineReader::lineReader() {
    memoria = (void**) malloc(120);
    numVariables = -1;
    variable *stackVar = NULL;
    isllaveAbierta = true;
    llavesDisponibles = 0;

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

    // Declaracion
    else if (primeraPalabra == "int" || primeraPalabra == "float" || primeraPalabra == "long" ||
            primeraPalabra == "double" || primeraPalabra == "char") {
        tipoVar = primeraPalabra;
        return separarDeclaracion();
    }

    // Error
    else {
        return error();
    }
    
}

string lineReader::abrirLlaves(){
    isllaveAbierta = true;
    llavesDisponibles ++;
    return "{\"tipo\" : \"Aviso\", \"respuesta\" : \"Se abrieron llaves\"}";

}

string lineReader::cerrarLlaves(){
    if (llavesDisponibles > 0) {
        while (!stackVar->comienzoLlaves) {
            popVariable();
        }
        popVariable();
        llavesDisponibles --;
        return "{\"tipo\" : \"Aviso\", \"respuesta\" : \"Se cerraron llaves\"}";

    }
    else {
        return "{\"tipo\" : \"Error\", \"respuesta\" : \"No se han abierto llaves\"}";
    }
    

}

string lineReader::imprimir(){
    return "se imprimio algo";

}

string lineReader::separarDeclaracion() {
    istringstream isstream(linea);
 
    vueltaWhile = 1;
    igualdad = NULL;
    operador = NULL;
    valorVarChar = NULL;
    valorVarStr1 = "";
    valorVarStr2 = "";
    valorVarNum1 = 0;
    valorVarNum2 = 0;
    valorVarNumT = 0;
    while(!isstream.eof()){
        
        isstream >> temporal;
        switch (vueltaWhile) {
            case 2:
                // Comprobando sintaxis del nombre
                if (temporal == ";") {
                    return "{\"tipo\" : \"Error\", \"respuesta\" : \"Sintaxis incorrecta en la declaracion\"}";
                }
                
                nombreVar = temporal;
                break;

            case 3:
                // Comprobando sintaxis del igual
                if (temporal != "=") {
                    return "{\"tipo\" : \"Error\", \"respuesta\" : \"Sintaxis incorrecta en la declaracion\"}";
                }
               
                igualdad = temporal[0];
                break;

            case 4:
                // Comprobando sintaxis del primer valor
                if (temporal == ";") {
                    return "{\"tipo\" : \"Error\", \"respuesta\" : \"Sintaxis incorrecta en la declaracion del primer valor\"}";
                }
                
                valorVarStr1 = temporal;
                break;

            case 5:
                // Comprobando sintaxis del operador
                if (temporal != "+" && temporal != "-" && temporal != "*" && temporal != "-" && temporal != "/") {
                    return "{\"tipo\" : \"Error\", \"respuesta\" : \"Operador desconocido\"}";
                }
                
                operador = temporal[0];
                break;

            case 6:
                // Comprobando sintaxis del segundo valor
                if (temporal == ";") {
                    return "{\"tipo\" : \"Error\", \"respuesta\" : \"Sintaxis incorrecta en la declaracion del segundo valor\"}";
                }
                valorVarStr2 = temporal;
                break;
            case 7:
                // Comprobando existe palabras sobrantes del segundo valor
                return "{\"tipo\" : \"Error\", \"respuesta\" : \"Sintaxis incorrecta en la declaracion\"}";
                break;
        }
        vueltaWhile ++;
    }
    
    // El nombre esta bien
    if (igualdad == NULL) {
        // No hay igual (=)
        nombreVar = nombreVar.substr(0, nombreVar.length() - 1);
    }
    else {
        // Se encuentra el igual (=)  
        // No hay operador o el operador es +, -, * o /

        if (valorVarStr1 == "") {
            return "{\"tipo\" : \"Error\", \"respuesta\" : \"Debe asignarse un valor\"}";
        }
        else {
            // Existe al menos un valor

            //Quitando los ;
            if (operador == NULL) {
                valorVarStr1 = valorVarStr1.substr(0, valorVarStr1.length() -1 );
            }
            else {
                if (valorVarStr2 == "") {
                    return "{\"tipo\" : \"Error\", \"respuesta\" : \"Falta un segundo valor\"}";
                }
                valorVarStr2 = valorVarStr2.substr(0, valorVarStr2.length() -1 );
            }

            // Comprobando si realmente son el dato que dicen ser
            
            // Tipo numerico
            if (tipoVar != "char") {
                try{
                    valorVarNum1 = stod(valorVarStr1);
                } 
                catch(exception) {
                    posicionVar = encontrarPosVar(valorVarStr1);
                    if (posicionVar == -1) {
                        return "{\"tipo\" : \"Error\", \"respuesta\" : \"Asignacion no valida\"}";
                    }
                    valorVarNum1 = encontrarNumVar(posicionVar);
                }
                if (operador != NULL) {
                    try{
                        valorVarNum2 = stod(valorVarStr2);
                    } 
                    catch(exception) {
                        posicionVar = encontrarPosVar(valorVarStr2);
                        if (posicionVar == -1) {
                            return "{\"tipo\" : \"Error\", \"respuesta\" : \"Asignacion no valida\"}";
                        }
                        valorVarNum2 = encontrarNumVar(posicionVar);
                    }
                }   
            }
            else {
                // Tipo char
                if (operador != NULL) {
                    return "{\"tipo\" : \"Error\", \"respuesta\" : \"Declaracion incorrecta, el tipo char no se pueden operar)\"}";
                }
                else {
                    posicionVar = encontrarPosVar(valorVarStr1);
                    if (posicionVar == -1) {
                        if (valorVarStr1.length() != 3) {
                            return "{\"tipo\" : \"Error\", \"respuesta\" : \"Declaracion incorrecta del char\"}";
                            
                        }
                        else if (valorVarStr1[0] != '\'' || valorVarStr1[2] != '\'') {
                            return "{\"tipo\" : \"Error\", \"respuesta\" : \"Declaracion incorrecta del char, debe ir intre comillas simples\"}";
                        }
                        else {
                            valorVarChar = valorVarStr1[1];

                        }
                    }
                    else {
                        valorVarChar = encontrarCharVar(posicionVar);
                    }
                }
            }

            switch (operador)
            {
            case '+':
                valorVarNumT = valorVarNum1 + valorVarNum2;
                break;
            case '-':
                valorVarNumT = valorVarNum1 - valorVarNum2;
                break;
            case '*':
                valorVarNumT = valorVarNum1 * valorVarNum2;
                break;
            case '/':
                valorVarNumT = valorVarNum1 / valorVarNum2;
                break;
            
            default:
                valorVarNumT = valorVarNum1;
                break;
            }            
        }
    }
    return declarar(); 
}

string lineReader::declarar(){

    numVariables ++;

    // Casos posibles
    if (tipoVar == "int") {

        memoria[numVariables] = (int*) malloc(sizeof(int));
        *((int*) memoria[numVariables]) = valorVarNumT; 

        pushVariable(tipoVar, nombreVar, numVariables, isllaveAbierta);
        isllaveAbierta = false;

        return "{\"tipo\" : \"Declaracion\", \"respuesta\" : \"" + stackVar->tipo + " " + stackVar->nombre + " = " 
                + to_string(*((int*)memoria[stackVar->posicion])) + " (0x??????????)\"}";

    }

    else if (tipoVar == "float") {

        memoria[numVariables] = (float*) malloc(sizeof(float));
        *((float*) memoria[numVariables]) = valorVarNumT;

        pushVariable(tipoVar, nombreVar, numVariables, isllaveAbierta);
        isllaveAbierta = false; 
        
        return "{\"tipo\" : \"Declaracion\", \"respuesta\" : \"" + stackVar->tipo + " " + stackVar->nombre + " = " 
                + to_string(*((float*)memoria[stackVar->posicion])) + " (0x??????????)\"}";

    }

    else if (tipoVar == "long") {

        memoria[numVariables] = (long*) malloc(sizeof(long));
        *((long*) memoria[numVariables]) = valorVarNumT;

        pushVariable(tipoVar, nombreVar, numVariables, isllaveAbierta);
        isllaveAbierta = false;

        return "{\"tipo\" : \"Declaracion\", \"respuesta\" : \"" + stackVar->tipo + " " + stackVar->nombre + " = " 
                + to_string(*((long*)memoria[stackVar->posicion])) + " (0x??????????)\"}";

    }

    else if (tipoVar == "double") {
        
        memoria[numVariables] = (double*) malloc(sizeof(double));
        *((double*) memoria[numVariables]) = valorVarNumT;
        
        pushVariable(tipoVar, nombreVar, numVariables, isllaveAbierta);
        isllaveAbierta = false;

        return "{\"tipo\" : \"Declaracion\", \"respuesta\" : \"" + stackVar->tipo + " " + stackVar->nombre + " = " 
                + to_string(*((double*)memoria[stackVar->posicion])) + " (0x??????????)\"}";
    }

    else {

        memoria[numVariables] = (char*) malloc(sizeof(char));
        *((char*) memoria[numVariables]) = valorVarChar;
        
        pushVariable(tipoVar, nombreVar, numVariables, isllaveAbierta);
        isllaveAbierta = false;

        return "{\"tipo\" : \"Declaracion\", \"respuesta\" : \"" + stackVar->tipo + " " + stackVar->nombre + " = " 
                + to_string(*((char*)memoria[stackVar->posicion])) + " (0x??????????)\"}";
    }

}

string lineReader::error() {
    return "hubo un error";

}

double lineReader::encontrarNumVar(int pos) {
    variable *var = stackVar;
    for (int i = 0; i < pos; i++) {
        var = var->next;
    }
    if (var->tipo == "int") {
        return *((int*) memoria[pos]);
    }
    else if (var->tipo == "float") {
        return *((float*) memoria[pos]);
    }
    else if (var->tipo == "long") {
        return *((long*) memoria[pos]);
    }
    else if (var->tipo == "double") {
        return *((double*) memoria[pos]);
    }
    return NULL;
    
}

char lineReader::encontrarCharVar(int pos) {
    variable *var = stackVar;
    for (int i = 0; i < pos; i++) {
        var = var->next;
    }
    if (var->tipo == "char") {
        return *((char*) memoria[pos]);
    }
    return ';';
}

int lineReader::encontrarPosVar(string name) {
    variable *var = stackVar;
    while (var != nullptr) {
        if (var->nombre == name) {
            return var->posicion;
        }  
        var = var->next; 
    }
    return -1;
}

void lineReader::popVariable() {

    variable *aux = stackVar;
    stackVar = stackVar->next;
    free(memoria[aux->posicion]);
    memoria[aux->posicion] = nullptr;
    delete aux;
    numVariables --;
}

void lineReader::pushVariable(string tipo, string nombre, int posicion, bool llaves) {

    variable *nuevoVar = new variable();

    nuevoVar->tipo = tipo;
    nuevoVar->nombre = nombre;
    nuevoVar->posicion = posicion;
    nuevoVar->comienzoLlaves = llaves;

    nuevoVar->next = stackVar;
    stackVar = nuevoVar;
}

