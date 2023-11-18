#ifndef PRINTER_RCTVRP_H
#define PRINTER_RCTVRP_H

#include <string>
#include <vector>
#include "../RCVRP.h"

void printerInstancia(const RCVRPInstancia& instancia);

void printerMatrizAdyacencia(const RCVRPInstancia& instancia);

void printerSolucion(const RCVRPSolucion& solucion);

void guardarSolucionEnArchivo(const RCVRPSolucion& solucion, const std::string& nombreArchivo);

#endif