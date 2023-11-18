#include "lectura-instancia/instancias.h"
#include "outs/outs.h"
#include "greedy/greedy.h"
#include "f_evaluacion/f_evaluacion.h"
#include "tabu_search/tabu_search.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <iomanip>


int main()
{
    RCVRPInstancia instance;

    std::string set; //se inicializa set para lectura por consola
    std::string instxt; //se inicializa instxt para lectura por consola
    std::string risklvl; //se inicializa risklvl para lectura por consola
    std::string numnodes; //se inicializa numnodes para lectura por consola
    std::string desvest; //se inicializa desvest para lectura por consola
    std::string nameArchivo;

    std::cout << "Ingrese el SET (O, R, S, V): ";
    std::cin >> set;
    if (set == "O" || set == "S")
    {
        set = "SET-" + set;
        std::cout << "(10, 13, 16, 19, 21, 25, 28, 31, 37, 40, 41, 49, 52, 53, 55, 64, 69, 73, 79, " << std::endl;
        std::cout << "81, 85, 94, 103,105, 125, 127, 137, 145, 169, 187, 209, 249, 273, 337)" << std::endl;
        std::cout << "Ingrese el número de la instancia: ";
        std::cin >> instxt;
        instance = readRCVRPInstancia_O_and_S(set, instxt, nameArchivo); //se lee la instancia en cuestión
        if (set == "O") {nameArchivo = set + "_" + nameArchivo;}
        else {nameArchivo = set + "_" + nameArchivo;}
    }
    else if (set == "V")
    {
        set = "SET-" + set;
        std::cout << "Ingrese el Risk Level (1.0, 1.5, 2.0, 2.5, 3.0): ";
        std::cin >> risklvl;
        std::cout << "(22, 26, 30, 36, 45, 51, 72, 101, 121, 135, 151, 200, 256, 301)" << std::endl;
        std::cout << "Ingrese el número de la instancia: ";
        std::cin >> instxt;
        instance = readRCVRPInstancia_V(set, instxt, risklvl, nameArchivo); //se lee la instancia en cuestión
        nameArchivo = set + "_" + "RISKLVL-" + risklvl + "_" + nameArchivo;
    }
    else if (set == "R")
    {
        set = "SET-" + set;
        std::cout << "Ingrese el número de nodos (4, 6, 8, 10, 12, 14, 16, 18, 20): ";
        std::cin >> numnodes;
        std::cout << "Ingrese la desviacion estándar (1, 3, 5, 7): ";
        std::cin >> desvest;
        std::cout << "Ingrese el Risk Level (1.0, 1.5, 2.0, 2.5, 3.0): ";
        std::cin >> risklvl;
        instance = readRCVRPInstancia_R(set, numnodes, desvest, risklvl, nameArchivo); //se lee la instancia en cuestión
        nameArchivo = set + "_" + nameArchivo;
    }

    //printerInstancia(instance); //comentar/descomentar para mostrar o no la instancia y sus componentes por pantalla

    llenarMatrizAdyacencia(instance);

    //printerMatrizAdyacencia(instance); //comentar/descomentar para mostrar o no la matriz de adyacencia por pantalla

    RCVRPSolucion solucion;
    
    auto inicioG = std::chrono::high_resolution_clock::now(); //se inicializa el cronómetro
    solucion = greedy(instance); //se obtiene la solución Greedy de la instancia
    auto finG = std::chrono::high_resolution_clock::now(); //se detiene el cronómetro
    std::chrono::duration<double> duracionG = finG - inicioG; //se obtiene la diferencia de tiempos para obtener el tiempo de Greedy
    solucion.tEjecucion = duracionG.count(); //se obtiene el tiempo de ejecución en segundos para asignarlo a la solución

    //printerSolucion(solucion); //comentar/descomentar para mostrar o no la solución y sus componentes por pantalla

    std::string nameArchivoG = "GREEDY;" + nameArchivo;
    guardarSolucionEnArchivo(solucion, nameArchivoG);

    std::cout << "Ingrese la cantidad de ejecuciones para el Tabu Search: ";
    int ITER; //variable para el numero de iteraciones
    std::cin >> ITER; //se almacena el numero de iteraciones

    RCVRPSolucion solucionFinal;

    auto inicioTS = std::chrono::high_resolution_clock::now(); //se inicializa el cronómetro
    solucionFinal = tabu_search(instance, solucion, ITER); //se obtiene la solución Tabu Search de la instancia
    auto finTS = std::chrono::high_resolution_clock::now(); //se detiene el cronómetro
    std::chrono::duration<double> duracionTS = finTS - inicioTS; //se obtiene la diferencia de tiempos para obtener el tiempo de Tabu Search
    solucionFinal.tEjecucion = duracionTS.count() + solucion.tEjecucion; //se obtiene el tiempo de ejecución en segundos para asignarlo a la solución

    //printerSolucion(solucionFinal); //comentar/descomentar para mostrar o no la solución y sus componentes por pantalla

    std::string nameArchivoTS = "TABU-SEARCH;" + nameArchivo;
    guardarSolucionEnArchivo(solucionFinal, nameArchivoTS);

    std::cout << "C total: " << solucionFinal.cTotal << std::endl;
    std::cout << "tiempo: " << solucionFinal.tEjecucion << std::endl;
    

    return 0;
}
