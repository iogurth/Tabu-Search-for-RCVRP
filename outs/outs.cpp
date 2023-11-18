#include "outs.h"
#include <fstream>
#include <iostream>
#include <cmath>

void printerInstancia(const RCVRPInstancia& instancia)
{
    std::cout << std::endl;
    if (instancia.numNodes != -1)
    {
        std::cout << "Número de nodos: " << instancia.numNodes << std::endl;
        std::cout << "Umbral de riesgo: " << instancia.riskThreshold << std::endl;
        std::cout << "Desviación estándar de la demanda: " << instancia.dStandardDeviation << std::endl;
        std::cout << "El risk level de la instancia: " << instancia.riskLevel << std::endl;

        for (int i = 0; i < instancia.numNodes; i++)
        {
            std::cout << "Nodo " << i << " - Demanda: " << instancia.nodes[i].demand
                      << ", Coordenadas: (" << instancia.nodes[i].x << ", " << instancia.nodes[i].y << ")" << std::endl;
        }
    }
    else
    {
        std::cerr << "Error al leer la instancia." << std::endl;
    }
    std::cout << std::endl;
}

void printerMatrizAdyacencia(const RCVRPInstancia& instancia) {
    std::cout << std::endl;
    for (int i = 0; i < instancia.numNodes; ++i) {
        for (int j = 0; j < instancia.numNodes; ++j) {
            std::cout << instancia.matrizAdyacencia[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void printerSolucion(const RCVRPSolucion& solucion) {
    std::cout << "Tiempo de Ejecución: " << solucion.tEjecucion << " segundos" << std::endl;
    std::cout << "Coste Total: " << solucion.cTotal << std::endl;
    std::cout << "Número de Vehículos: " << solucion.nVehiculos << std::endl;

    std::cout << "Nodos Atendidos: ";
    for (int nodo : solucion.nodesAtt) { //imprimir nodesAtt
        std::cout << nodo << " ";
    }
    std::cout << std::endl;

    std::cout << "Demanda de las Rutas: ";
    for (int demanda : solucion.rutasD) { //imprimir rutasD
        std::cout << demanda << " ";
    }
    std::cout << std::endl;

    std::cout << "Coste/Distancia de las Rutas: ";
    for (int costo : solucion.rutasC) { //imprimir rutasC
        std::cout << costo << " ";
    }
    std::cout << std::endl;

    std::cout << "Riesgo de las Rutas: ";
    for (int riesgo : solucion.rutasR) { //imprimir rutasR
        std::cout << riesgo << " ";
    }
    std::cout << std::endl;

    std::cout << "Rutas por Vehículo:" << std::endl;
    for (size_t i = 1; i <= solucion.rutas.size(); i++) { //imprimir rutas
        std::cout << "Vehículo " << i << ": ";
        for (int nodo : solucion.rutas[i-1]) {
            std::cout << nodo << " ";
        }
        std::cout << std::endl;
    }
}

void guardarSolucionEnArchivo(const RCVRPSolucion& solucion, const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo para escritura." << std::endl;
        return;
    }
    
    archivo << solucion.tEjecucion << std::endl;
    archivo << solucion.cTotal << std::endl;
    archivo << solucion.nVehiculos << std::endl;

    for (int node : solucion.nodesAtt) {
        archivo << node << " ";
    }
    archivo << std::endl;

    for (int i = 0; i < solucion.nVehiculos; i++) {
        archivo << solucion.rutasC[i] << " " << solucion.rutasR[i] << " ";
        for (long unsigned int j = 0; j < solucion.rutas[i].size(); j++) {
            if (j < solucion.rutas[i].size() - 1) {
                archivo << solucion.rutas[i][j] << " -> ";
            } else {
                archivo << solucion.rutas[i][j] << std::endl;
            }
        }
    }
    
    archivo.close();
    std::cout << "Archivo guardado exitosamente: " << nombreArchivo << std::endl;
}
