#ifndef TABUSEARCH_RCTVRP_H
#define TABUSEARCH_RCTVRP_H

#include <vector>
#include <cmath>
#include <tuple>
using Tupla = std::tuple<int, int>;
#include "../f_evaluacion/f_evaluacion.h"
#include "../RCVRP.h"

std::vector<std::vector<int>> two_opt(const std::vector<int>& ruta);

std::vector<int> two_opt_one(const std::vector<int>& ruta, const int& k, const int& j);

RCVRPSolucion tabu_search(const RCVRPInstancia& instancia, const RCVRPSolucion& solucionC, const int MAX_ITER);

void push_back_tabu(std::vector<Tupla>& tabulist, Tupla mov, std::size_t fixed_size);

double riesgo(const std::vector<std::vector<double>>& matrizAdyaciencia, const std::vector<Node>& nodes, const std::vector<int>& ruta);

#endif