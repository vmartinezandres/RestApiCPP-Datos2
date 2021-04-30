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
/**
 * @struct linereader
 * @brief Estructura que se encarga de leer la linea que se envia desde el IDE
 */

// Constructor
lineReader::lineReader() {
    memoria = (void**) malloc(120);
    numVariables = -1;
    variable *stackVar = NULL;
    isllaveAbierta = true;
    llavesDisponibles = 0;

}

// Funciones
/**
 * @brief leerLinea
 * @param linea que se esta leyendo
 * @return operacion que se va a ejecutar
 * @htmlinclude YES
 */
string lineReader::leerLinea(string _linea) {
    linea = _linea;
    return definirOperacion();
}
/**
 * @brief  define que tiene que hacerse con el caracter en el que se encuentra
 * @return el caso a ejecutar
 *
 */
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
        return separarAsignacion();
    }

    varTemp = encontrarVar(primeraPalabra);
    if (varTemp != NULL) {
        // Asignacion
        return asignar();

    }
    else {
       // Error
        return error();
    }
    
}
/**
 * @brief abre llaves si se encuentra una {
 */
string lineReader::abrirLlaves(){
    isllaveAbierta = true;
    llavesDisponibles ++;
    return "{\"tipo\" : \"Aviso\", \"respuesta\" : \"Se abrieron llaves\"}";

}
/**
 * @brief cierra llaves si se encuentra una }
 */
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
/**
 * @brief confirma que se imprimio algo
 */
string lineReader::imprimir(){
    return "se imprimio algo";

}
/**
 * @brief se encarga de imprimir la razon del error que encuentra en la linea
 */
string lineReader::separarAsignacion() {
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
                    varTemp = encontrarVar(valorVarStr1);
                    if (varTemp == NULL) {
                        return "{\"tipo\" : \"Error\", \"respuesta\" : \"Asignacion no valida\"}";
                    }
                    valorVarNum1 = valorVarNum(varTemp->tipo, varTemp->posicion);
                    
                }
                if (operador != NULL) {
                    try{
                        valorVarNum2 = stod(valorVarStr2);
                    } 
                    catch(exception) {
                        varTemp = encontrarVar(valorVarStr2);
                        if (varTemp == NULL) {
                            return "{\"tipo\" : \"Error\", \"respuesta\" : \"Asignacion no valida\"}";
                        }
                        valorVarNum2 = valorVarNum(varTemp->tipo, varTemp->posicion);
                    }
                }   
            }
            else {
                // Tipo char
                if (operador != NULL) {
                    return "{\"tipo\" : \"Error\", \"respuesta\" : \"Asignacion no valida, char no se pueden operar)\"}";
                }
                else {
                    varTemp = encontrarVar(valorVarStr1);
                    if (varTemp == NULL) {
                        if (valorVarStr1.length() != 3) {
                            return "{\"tipo\" : \"Error\", \"respuesta\" : \"Asignacion incorrecta de char\"}";
                            
                        }
                        else if (valorVarStr1[0] != '\'' || valorVarStr1[2] != '\'') {
                            return "{\"tipo\" : \"Error\", \"respuesta\" : \"Asignacion incorrecta de char, debe ir intre comillas simples\"}";
                        }
                        else {
                            valorVarChar = valorVarStr1[1];

                        }
                    }
                    else {
                        valorVarChar =  *((char*) memoria[varTemp->posicion]);
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
/**
 * @brief
 */
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
                + *((char*)memoria[stackVar->posicion]) + " (0x??????????)\"}";
    }

}

string lineReader::asignar() {
    return "Se asigno algo";
}

string lineReader::error() {
    return "hubo un error";

}

lineReader::variable* lineReader::encontrarVar(string varName) {
    variable *var = stackVar;
    while (var != nullptr) {
        if (var->nombre == varName) {
            return var;
        }
        var = var->next;
    }
    return NULL;
    
}
/**
 *
 * @param tipo
 * @param pos
 * @return
 */
double lineReader::valorVarNum(string tipo, int pos) {
    if (tipo == "int") {
        return *((int*) memoria[pos]);
    }
    else if (tipo == "float") {
        return *((float*) memoria[pos]);
    }
    else if (tipo == "long") {
        return *((long*) memoria[pos]);
    }
    else {
        return *((double*) memoria[pos]);
    }
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

void lineReader::popVariable() {

    variable *aux = stackVar;
    stackVar = stackVar->next;
    free(memoria[aux->posicion]);
    memoria[aux->posicion] = nullptr;
    delete aux;
    numVariables --;
}



