#include "instancias.h"
#include <fstream>
#include <iostream>
#include <cmath>

RCVRPInstancia readRCVRPInstancia_O_and_S(std::string& set, std::string& instancia, std::string& nameArchivo)
{
    RCVRPInstancia instance; //inicializa la instancia
    std::string path = "instancias-RCVRP/"; //define el path de la carpeta de instancias

    std::string filename = path + set + "/" + instancia + ".txt"; //crea el filename con los componentes
    std::ifstream file(filename); //abre el archivo .txt de la instancia
    nameArchivo = instancia;

    if (!file) //verificador de errores de apertura de archivo
    {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        instance.numNodes = -1;
        return instance;
    }

    file >> instance.numNodes; //lee la primera linea de la instancia
    file >> instance.riskThreshold; //lee la segunda linea de la instancia
    instance.nodes.resize(instance.numNodes); //redimensiona el vector de nodos de la instancia con la cantidad obtenida

    double suma = 0.0;
    for (int i = 0; i < instance.numNodes; i++)
    {
        file >> instance.nodes[i].demand; //agrega a cada nodo su demanda correspondiente leyendo elementos de file
        suma += instance.nodes[i].demand; //suma las demandas agregadas a suma
    }
    double mean = suma / ( instance.numNodes-1); //calcula la media de las demandas

    for (int i = 0; i < instance.numNodes; i++)
    {
        file >> instance.nodes[i].x >> instance.nodes[i].y; //agrega las coordenadas correspondientes a cada nodo desde file
    }

    double sumaDiferenciasCuadradas = 0.0; //inicializa las diferencias cuadradas para obtener la desvest
    for (int i = 1; i < instance.numNodes; i++) { //itera sobre la cantidad de nodos
        double diferencia = instance.nodes[i].demand - mean; //calcula la diferencia del elemento - media
        sumaDiferenciasCuadradas += diferencia * diferencia; //suma constantemente a la variable instanciada
    }
    instance.dStandardDeviation = std::sqrt( sumaDiferenciasCuadradas / (instance.numNodes-1) ); //obtiene raiz para sacar la desviación estándar

    file.close(); //cierra el archivo de instancia

    return instance; //retorna la instancia en cuestión
}

RCVRPInstancia readRCVRPInstancia_V(std::string& set, std::string& instancia, std::string& risklvl, std::string& nameArchivo)
{
    RCVRPInstancia instance; //inicializa la instancia
    std::string path = "instancias-RCVRP/"; //define el path de la carpeta de instancias

    std::string filename = path + set + "/RISK-LEVEL-" + risklvl + "/" + instancia + ".txt"; //crea el filename con los componentes
    std::ifstream file(filename); //abre el archivo .txt de la instancia
    nameArchivo = instancia;

    instance.riskLevel = std::stod(risklvl);

    if (!file) //verificador de errores de apertura de archivo
    {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        instance.numNodes = -1;
        return instance;
    }
    file >> instance.numNodes; //lee la primera linea de la instancia
    file >> instance.riskThreshold; //lee la segunda linea de la instancia
    instance.nodes.resize(instance.numNodes); //redimensiona el vector de nodos de la instancia con la cantidad obtenida

    double suma = 0.0;
    for (int i = 0; i < instance.numNodes; i++)
    {
        file >> instance.nodes[i].demand; //agrega a cada nodo su demanda correspondiente leyendo elementos de file
        suma += instance.nodes[i].demand; //suma las demandas agregadas a suma
    }
    double mean = suma / ( instance.numNodes-1); //calcula la media de las demandas

    for (int i = 0; i < instance.numNodes; i++)
    {
        file >> instance.nodes[i].x >> instance.nodes[i].y; //agrega las coordenadas correspondientes a cada nodo desde file
    }
    double sumaDiferenciasCuadradas = 0.0; //inicializa las diferencias cuadradas para obtener la desvest
    for (int i = 1; i < instance.numNodes; i++) { //itera sobre la cantidad de nodos
        double diferencia = instance.nodes[i].demand - mean; //calcula la diferencia del elemento - media
        sumaDiferenciasCuadradas += diferencia * diferencia; //suma constantemente a la variable instanciada
    }
    instance.dStandardDeviation = std::sqrt( sumaDiferenciasCuadradas / (instance.numNodes-1) ); //obtiene raiz para sacar la desviación estándar

    file.close(); //cierra el archivo de instancia

    return instance; //retorna la instancia en cuestión
}

RCVRPInstancia readRCVRPInstancia_R(std::string& set, std::string& numNodes, std::string& dStandardDeviation, std::string& risklvl, std::string& nameArchivo)
{
    RCVRPInstancia instance; //inicializa una instancia
    std::string path = "instancias-RCVRP/"; //define el path de la carpeta de instancias

    std::string filename = path + set + "/" + numNodes + "_" + dStandardDeviation + "_" + risklvl + ".txt"; //crea el filename con los componentes
    std::ifstream file(filename); //abre el archivo .txt de la instancia
    nameArchivo = numNodes + "_" + dStandardDeviation + "_" + risklvl;

    if (!file) //verificador de errores de apertura de archivo
    {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        instance.numNodes = -1;
        return instance;
    }

    file >> instance.numNodes; //lee la primera linea de la instancia
    file >> instance.riskThreshold; //lee la segunda linea de la instancia
    instance.nodes.resize(instance.numNodes); //redimensiona el vector de nodos de la instancia con la cantidad obtenida

    for (int i = 0; i < instance.numNodes; i++)
    {
        file >> instance.nodes[i].demand; //agrega a cada nodo su demanda correspondiente leyendo elementos de file
    }

    for (int i = 0; i < instance.numNodes; i++)
    {
        file >> instance.nodes[i].x >> instance.nodes[i].y; //agrega las coordenadas correspondientes a cada nodo desde file
    }

    instance.riskLevel = std::stod(risklvl); //obtiene el nivel de riesgo desde los parámetros
    instance.dStandardDeviation = std::stod(dStandardDeviation); //obtiene la desviación estándar desde los parámetros

    file.close(); //cierra el archivo de instancia

    return instance; //retorna la instancia en cuestión
}

void llenarMatrizAdyacencia(RCVRPInstancia& instancia) {
    int numNodos = instancia.numNodes; //inicializa el número de nodos
    instancia.matrizAdyacencia.resize(numNodos, std::vector<double>(numNodos, 0.0)); //reajusta la matriz a una de tamaño numNodosxnumNodos
    double distancia; //inicializa distancia
    for (int i = 0; i < numNodos; ++i) { //recorre por columnas
        for (int j = 0; j < numNodos; ++j) { //recorre por filas
            if (i != j) { //mientras no sea la diagonal, dado que es la distancia del nodo a sí mismo, es decir, 0
                //obtiene la distancia entre el punto fijado de la columna y el punto iterado de la fila
                distancia = sqrt(pow(instancia.nodes[i].x - instancia.nodes[j].x, 2) + pow(instancia.nodes[i].y - instancia.nodes[j].y, 2));
                instancia.matrizAdyacencia[i][j] = distancia; //se agrega el valor a la matriz
            }
        }
    }
}
