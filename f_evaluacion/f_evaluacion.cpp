#include "f_evaluacion.h"
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <cmath>

double f_evaluacion(const std::vector<std::vector<double>>& matrizAdyacencia, const std::vector<int>& ruta)
{
    int n = ruta.size(); //obtiene el largo de la ruta
    double c = 0; //inicializa c

    for (int i = 0; i < n-1; ++i)
    {
        c += matrizAdyacencia[ruta[i]][ruta[i+1]]; //obtiene la distancia de los nodos actual al siguiente
    }
    return c; //retorna la distancia total recorrida de la ruta
}
