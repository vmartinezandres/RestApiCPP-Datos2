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
    if (linea == "") {
        return "{\"tipoMensaje\" : \"Aviso\", \"mensaje\" : \"Espacio en blanco\"}";
    }
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
        return separarAsignacion('D');
    }

    varTemp = encontrarVar(primeraPalabra);
    if (varTemp != NULL) {
        // Asignacion
        linea = varTemp->tipo + " " + linea;
        tipoVar = varTemp->tipo;
        return separarAsignacion('A');

    }
    else {
       // Error
        return error();
    }
    
}

string lineReader::abrirLlaves(){
    isllaveAbierta = true;
    llavesDisponibles ++;
    return "{\"tipoMensaje\" : \"Aviso\", \"mensaje\" : \"Se abrieron llaves\"}";

}

string lineReader::cerrarLlaves(){
    if (llavesDisponibles > 0) {
        while (!stackVar->comienzoLlaves) {
            popVariable();
        }
        popVariable();
        llavesDisponibles --;
        return "{\"tipoMensaje\" : \"Aviso\", \"mensaje\" : \"Se cerraron llaves\"}";
    }
    else {
        return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"No se han abierto llaves\"}";
    }
    

}

string lineReader::imprimir() {
    istringstream isstream(linea);

    vueltaWhile = 1;
    while(!isstream.eof()){
        
        isstream >> temporal;
        switch (vueltaWhile) {
            case 2:
                // Comprobando sintaxis del cout
                if (temporal != "<<") {
                    return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Sintaxis incorrecta en el cout\"}";
                }
                break;

            case 3:
                if (temporal == ";") {
                    return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Sintaxis incorrecta en el cout\"}";
                }
                nombreVar = temporal.substr(0, temporal.length() - 1);
                break;
            case 4:
                // Comprobando existe palabras sobrantes del segundo valor
                return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Solo se puede imprimir una varibale\"}";
                break;

        }
        vueltaWhile ++;
    }
    varTemp = encontrarVar(nombreVar);
    if (varTemp == NULL) {
        return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Variable no encontrada\"}";
    }
    else {
        if (varTemp->tipo == "int") {
            return "{\"tipoMensaje\" : \"Cout\", \"mensaje\" : \"" + to_string(*((int*)memoria[varTemp->posicion])) + "\"}";
        }
        else if (varTemp->tipo == "float") {
            return "{\"tipoMensaje\" : \"Cout\", \"mensaje\" : \"" + to_string(*((float*)memoria[varTemp->posicion])) + "\"}";
        }
        else if (varTemp->tipo == "long") {
            return "{\"tipoMensaje\" : \"Cout\", \"mensaje\" : \"" + to_string(*((long*)memoria[varTemp->posicion])) + "\"}";
        }
        else if (varTemp->tipo == "double") {
            return "{\"tipoMensaje\" : \"Cout\", \"mensaje\" : \"" + to_string(*((double*)memoria[varTemp->posicion])) + "\"}";
        }
        else {
            return "{\"tipoMensaje\" : \"Cout\", \"mensaje\" : \"" + nada + *((char*)memoria[varTemp->posicion]) + "\"}";
        }
    }
}

string lineReader::separarAsignacion(char opcionAsignacion) {
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
                    return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Sintaxis incorrecta en la declaracion\"}";
                }
                
                nombreVar = temporal;
                break;

            case 3:
                // Comprobando sintaxis del igual
                if (temporal != "=") {
                    return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Sintaxis incorrecta en la declaracion\"}";
                }
               
                igualdad = temporal[0];
                break;

            case 4:
                // Comprobando sintaxis del primer valor
                if (temporal == ";") {
                    return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Sintaxis incorrecta en la declaracion del primer valor\"}";
                }
                
                valorVarStr1 = temporal;
                break;

            case 5:
                // Comprobando sintaxis del operador
                if (temporal != "+" && temporal != "-" && temporal != "*" && temporal != "-" && temporal != "/") {
                    return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Operador desconocido\"}";
                }
                
                operador = temporal[0];
                break;

            case 6:
                // Comprobando sintaxis del segundo valor
                if (temporal == ";") {
                    return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Sintaxis incorrecta en la declaracion del segundo valor\"}";
                }
                valorVarStr2 = temporal;
                break;
            case 7:
                // Comprobando existe palabras sobrantes del segundo valor
                return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Sintaxis incorrecta en la declaracion\"}";
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
            return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Debe asignarse un valor\"}";
        }
        else {
            // Existe al menos un valor

            //Quitando los ;
            if (operador == NULL) {
                valorVarStr1 = valorVarStr1.substr(0, valorVarStr1.length() -1 );
            }
            else {
                if (valorVarStr2 == "") {
                    return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Falta un segundo valor\"}";
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
                        return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Asignacion no valida\"}";
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
                            return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Asignacion no valida\"}";
                        }
                        valorVarNum2 = valorVarNum(varTemp->tipo, varTemp->posicion);
                    }
                }   
            }
            else {
                // Tipo char
                if (operador != NULL) {
                    return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Asignacion no valida, char no se pueden operar)\"}";
                }
                else {
                    varTemp = encontrarVar(valorVarStr1);
                    if (varTemp == NULL) {
                        if (valorVarStr1.length() != 3) {
                            return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Asignacion incorrecta de char\"}";
                            
                        }
                        else if (valorVarStr1[0] != '\'' || valorVarStr1[2] != '\'') {
                            return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Asignacion incorrecta de char, debe ir intre comillas simples\"}";
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
    switch (opcionAsignacion) {
    case 'D':
        return declarar();
        break;
    
    case 'A':
        return asignar();
        break;
    }
     
}

string lineReader::declarar(){

    numVariables ++;
    ostringstream oss;

    // Casos posibles
    if (tipoVar == "int") {

        memoria[numVariables] = (int*) malloc(sizeof(int));
        *((int*) memoria[numVariables]) = valorVarNumT; 
        pushVariable(tipoVar, nombreVar, numVariables, isllaveAbierta);

        oss << memoria[numVariables];
        address = oss.str();
        isllaveAbierta = false;

        
        return "{\"tipoMensaje\" : \"Declaracion\", \"mensaje\" : \"" + stackVar->tipo + " " + stackVar->nombre + " = " 
                + to_string(*((int*)memoria[stackVar->posicion])) + " (" + address + ")\"}";

    }

    else if (tipoVar == "float") {

        memoria[numVariables] = (float*) malloc(sizeof(float));
        *((float*) memoria[numVariables]) = valorVarNumT;
        pushVariable(tipoVar, nombreVar, numVariables, isllaveAbierta);

        oss << memoria[numVariables];
        address = oss.str();
        isllaveAbierta = false; 
        
        return "{\"tipoMensaje\" : \"Declaracion\", \"mensaje\" : \"" + stackVar->tipo + " " + stackVar->nombre + " = " 
                + to_string(*((float*)memoria[stackVar->posicion])) + " (" + address + ")\"}";

    }

    else if (tipoVar == "long") {

        memoria[numVariables] = (long*) malloc(sizeof(long));
        *((long*) memoria[numVariables]) = valorVarNumT;
        pushVariable(tipoVar, nombreVar, numVariables, isllaveAbierta);

        oss << memoria[numVariables];
        address = oss.str();
        isllaveAbierta = false;

        return "{\"tipoMensaje\" : \"Declaracion\", \"mensaje\" : \"" + stackVar->tipo + " " + stackVar->nombre + " = " 
                + to_string(*((long*)memoria[stackVar->posicion])) + " (" + address + ")\"}";

    }

    else if (tipoVar == "double") {
        
        memoria[numVariables] = (double*) malloc(sizeof(double));
        *((double*) memoria[numVariables]) = valorVarNumT;
        pushVariable(tipoVar, nombreVar, numVariables, isllaveAbierta);

        oss << memoria[numVariables];
        address = oss.str();
        isllaveAbierta = false;

        return "{\"tipoMensaje\" : \"Declaracion\", \"mensaje\" : \"" + stackVar->tipo + " " + stackVar->nombre + " = " 
                + to_string(*((double*)memoria[stackVar->posicion])) + " (" + address + ")\"}";
    }

    else {

        memoria[numVariables] = (char*) malloc(sizeof(char));
        *((char*) memoria[numVariables]) = valorVarChar;
        pushVariable(tipoVar, nombreVar, numVariables, isllaveAbierta);

        oss << memoria[numVariables];
        address = oss.str();
        isllaveAbierta = false;

        return "{\"tipoMensaje\" : \"Declaracion\", \"mensaje\" : \"" + stackVar->tipo + " " + stackVar->nombre + " = " 
                + *((char*)memoria[stackVar->posicion]) + " (" + address + ")\"}";
    }

}

string lineReader::asignar() {

    varTemp = encontrarVar(primeraPalabra);

    ostringstream oss;
    oss << memoria[varTemp->posicion];
    address = oss.str();

    // Casos posibles
    if (tipoVar == "int") {

        *((int*) memoria[varTemp->posicion]) = valorVarNumT; 

        return "{\"tipoMensaje\" : \"Asignacion\", \"mensaje\" : \"" + varTemp->tipo + " " + varTemp->nombre + " = " 
                + to_string(*((int*)memoria[varTemp->posicion])) + " (" + address + ")\"}";

    }

    else if (tipoVar == "float") {

        *((float*) memoria[varTemp->posicion]) = valorVarNumT;
        
        return "{\"tipoMensaje\" : \"Asignacion\", \"mensaje\" : \"" + varTemp->tipo + " " + varTemp->nombre + " = " 
                + to_string(*((float*)memoria[varTemp->posicion])) + " (" + address + ")\"}";

    }

    else if (tipoVar == "long") {

        *((long*) memoria[varTemp->posicion]) = valorVarNumT;
        
        return "{\"tipoMensaje\" : \"Asignacion\", \"mensaje\" : \"" + varTemp->tipo + " " + varTemp->nombre + " = " 
                + to_string(*((long*)memoria[varTemp->posicion])) + " (" + address + ")\"}";

    }

    else if (tipoVar == "double") {
        
        *((double*) memoria[varTemp->posicion]) = valorVarNumT;
        
        return "{\"tipoMensaje\" : \"Asignacion\", \"mensaje\" : \"" + varTemp->tipo + " " + varTemp->nombre + " = " 
                + to_string(*((double*)memoria[varTemp->posicion])) + " (" + address + ")\"}";
    }

    else {

        *((char*) memoria[numVariables]) = valorVarChar;
        
        return "{\"tipoMensaje\" : \"Asignacion\", \"mensaje\" : \"" + varTemp->tipo + " " + varTemp->nombre + " = " 
                + *((char*)memoria[varTemp->posicion]) + " (" + address + ")\"}";
    }
}

string lineReader::error() {
    return "{\"tipoMensaje\" : \"Error\", \"mensaje\" : \"Sintaxis desconocida\"}";

}

string lineReader::clear() {
    while (stackVar != NULL) {
        popVariable();
    }
    numVariables = -1;
    isllaveAbierta = true;
    llavesDisponibles = 0;

    return "{\"tipoMensaje\" : \"Aviso\", \"mensaje\" : \"Se limpio la memoria\"}";


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



