#ifndef GREEDY_RCTVRP_H
#define GREEDY_RCTVRP_H

#include "../RCVRP.h"
#include "../f_evaluacion/f_evaluacion.h"

int funcionMiope(const std::vector<std::vector<double>>& matrizAdyacencia, const std::vector<int>& ruta, const double& T,
                const std::vector<int>& nodosRestantes, double& rutaR, double& rutaD, double& rutaC, const std::vector<Node>& nodes);
RCVRPSolucion greedy(const RCVRPInstancia& instancia);

#endif