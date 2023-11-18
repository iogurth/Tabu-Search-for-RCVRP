#ifndef RCTVRP_INSTANCE_H
#define RCTVRP_INSTANCE_H

#include <vector>
#include <numeric>

/*
*******************************************************
Header RCVRP 
*******************************************************
Componentes:
    -struct Node: Nodo, cuenta con sus coordenadas y su demanda correspondiente.
    -struct RCVRPInstancia: Instancia leída desde archivo, cubre todos los campos posibles de lectura.
    -struct RCVRPSolucion: Solución creada, cubre todos los campos posibles de la solución tanto para Greedy como para Tabu Search.
*/


struct Node {
    double demand;
    double x, y;
};

struct RCVRPInstancia {
    int numNodes; //canidad total de nodos
    double riskThreshold; //umbral de riesgo
    double dStandardDeviation; //desviacion estandar
    double riskLevel; //nivel de riesgo (en caso de existir)
    std::vector<Node> nodes; //vector de nodos
    std::vector<std::vector<double>> matrizAdyacencia; //matriz de adyacencia o distancias
};

struct RCVRPSolucion {
    double tEjecucion; //tiempo de ejecucion
    double cTotal; //costo/distancia total
    int nVehiculos; //numero de vehiculos
    std::vector<int> nodesAtt; //nodos atendidos
    std::vector<double> rutasD; //demanda de la ruta i
    std::vector<double> rutasC; //costo/distancia recorrida de la ruta i
    std::vector<double> rutasR; //riesgo de la ruta i
    std::vector<std::vector<int>> rutas; //conjuntos de rutas, ruta i es la ruta del vehículo i
};

#endif
