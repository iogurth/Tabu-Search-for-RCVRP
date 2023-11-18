#ifndef INSTANCIAS_RCTVRP_H
#define INSTANCIAS_RCTVRP_H

#include <string>
#include <vector>
#include "../RCVRP.h"

RCVRPInstancia readRCVRPInstancia_O_and_S(std::string& set, std::string& instancia, std::string& nameArchivo);

RCVRPInstancia readRCVRPInstancia_V(std::string& set, std::string& instancia, std::string& risklvl, std::string& nameArchivo);

RCVRPInstancia readRCVRPInstancia_R(std::string& set, std::string& numNodes, std::string& dStandardDeviation, std::string& risklvl, std::string& nameArchivo);

void llenarMatrizAdyacencia(RCVRPInstancia& instancia);

#endif