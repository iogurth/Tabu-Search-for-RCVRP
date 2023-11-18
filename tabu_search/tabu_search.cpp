#include "tabu_search.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <tuple>
#include <limits>
#include <random>
#include <iomanip>

using Tupla = std::tuple<int, int>;

std::vector<std::vector<int>> two_opt(const std::vector<int>& ruta) 
{
    int n = ruta.size();
    std::vector<std::vector<int>> vecindario; //se crea el vector de vecotores para retornar el vecindario completo del movimiento

    for (int k = 1; k < n - 2; ++k) { //limita el rango para no movier los arcos de los elementos extremos (depositos)
        for (int j = k + 1; j < n - 1; ++j) { //itera para hacer los cambios de ruta
            std::vector<int> nuevaRuta = ruta; //genera un vector igual a la ruta
            std::reverse(nuevaRuta.begin() + k, nuevaRuta.begin() + j + 1); //realiza cambio de arcos
            vecindario.push_back(nuevaRuta); //lo agrega al retorno
        }
    }

    return vecindario; //retorna el vecindario generado desde la ruta
}

std::vector<int> two_opt_one(const std::vector<int>& ruta, const int& k, const int& j)
{  
    std::vector<int> nuevaRuta = ruta; //genera un vector igual a la ruta
    std::reverse(nuevaRuta.begin() + k, nuevaRuta.begin() + j + 1); //realiza cambio de arcos

    return nuevaRuta; //retorna el vecindario generado desde la ruta
}

double riesgo(const std::vector<std::vector<double>>& matrizAdyaciencia, const std::vector<Node>& nodes, const std::vector<int>& ruta)
{
    double Rid = 0.; //inicializa el riesgo en 0.
    double D = 0.; //inicializa la demanda en 0.
    int n = ruta.size(); //obtiene el largo de la ruta
    double c; //inicializa c

    for (int i = 0; i < n-1; ++i)
    {
        D += nodes[ruta[i]].demand; //suma demanda
        c = matrizAdyaciencia[ruta[i]][ruta[i+1]]; //obtiene la distancia de los nodos actual al siguiente
        Rid += c * D; //obtiene el riesgo con c y D
    }
    return Rid; //retorna el riesgo de la ruta
}

void push_back_tabu(std::vector<Tupla>& tabulist, Tupla mov, std::size_t fixed_size) {

    if (tabulist.size() >= fixed_size) //si el vector ya ha alcanzado su tamaño máximo, elimina el primer elemento
    {
        tabulist.erase(tabulist.begin()); //elimina el primer elemento de la lista
    }

    tabulist.push_back(mov); //añade el nuevo movimiento al final de la lista
}

RCVRPSolucion tabu_search(const RCVRPInstancia& instancia, const RCVRPSolucion& solucionC, const int MAX_ITER) 
{
    RCVRPSolucion solucion; //la solucion a retornar (no se reemplaza la actual para conservar los datos de c inical)
    solucion.rutas.resize(solucionC.nVehiculos); //arregla el tamaño de las rutas a retornar

    std::vector<std::vector<int>> vecindario; //variable que guarda los vecindarios generados por 2-opt

    std::vector<std::vector<Tupla>> listasTabu(solucionC.nVehiculos); //el vector de listas tabu, una por vehiculo
    std::vector<int> listasTabuLEN(solucionC.nVehiculos); //inicializa el largo del vector de largos de las listas tabu
    for (int i = 0; i < solucionC.nVehiculos; i++) 
    {
        //se define el largo de las listas tabú por vehículo, el 11% del vecindario posible generado, techo.
        listasTabuLEN[i] = ceil( 0.20*((solucionC.rutas[i].size()-3)*(solucionC.rutas[i].size()-4)/2) );
    }

    std::vector<std::vector<int>> rutasActuales(solucionC.rutas.size());

    solucion.nVehiculos = solucionC.nVehiculos;
    solucion.rutas.resize(solucionC.rutas.size());
    solucion.rutasC.resize(solucionC.rutas.size());
    solucion.rutasD.resize(solucionC.rutas.size());
    solucion.rutasR.resize(solucionC.rutas.size());

    for (int i = 0; i < solucionC.nVehiculos; i++) 
    {
        rutasActuales[i] = solucionC.rutas[i]; //inicaliza las rutas actuales con las rutas actuales iniciales de la solucionC
        solucion.rutas[i] = solucionC.rutas[i]; //inicaliza las rutas mejores con las rutas actuales iniciales de la solucionC
        solucion.rutasC[i] = solucionC.rutasC[i]; //inicaliza los costos de las rutas con las rutas actuales iniciales de la solucionC
        solucion.rutasD[i] = solucionC.rutasD[i]; //agrega los valores de la demanda de la ruta, no cambian con los movs porque son los mismos nodos
        solucion.rutasR[i] = solucionC.rutasR[i]; //inicaliza los riesgos de las rutas con las rutas actuales iniciales de la solucionC
    }

    int iteraciones = 0; //contador de iteraciones
    int indexCmin; //indice de la solucion de f_eval minima generada en el vecindario
    double Cmin;
    double Rnew;
    double Cf_value;
    Tupla mov;
    Tupla movToPush;
    int k, j; //indices de la tupla para la lista tabu
    int ktabu, jtabu; //indices para extraer el movimiento de la lista tabu
    double Ctabu, Rtabu; //Costo y Riesgo de la ruta tabu que se esta testeando
    double vecTRiesgo; //variable para el riesgo de ruta del vecindario

    std::random_device rd;  //dispositivo de generación de numeros aleatorios
    std::mt19937 gen(rd()); //generador de numeros aleatorios Mersenne Twister 19937
    std::uniform_real_distribution<> dis(0.0, 1.0); //distribucion uniforme entre 0.0 y 1.0
    double PICK_P = 0.95;
    double random; //variable para las generaciones de numeros random

    std::vector<int> puntos = {MAX_ITER / 2, MAX_ITER / 4, MAX_ITER / 8};
    std::vector<bool> indicadores = {false, false, false};

    std::vector<int> rutaTabu; //ruta tabu solo usada en casos particulares

    int rutasPasadas = 0;
    unsigned long long int n_feval = 0;

    while (iteraciones < MAX_ITER) //ciclo de iteraciones de la lista tabu
    {
        if (rutasPasadas >= solucionC.nVehiculos) { break; } //en caso de que no se pueda mejorar ninguna ruta, simplemente termina la ejecucion
        for (size_t i = 0; i < rutasActuales.size(); i++) //realiza "una iteracion" para la solucion, for de rutas
        {
            if (rutasActuales[i].size() < 5) { rutasPasadas++; continue; } //no puede generar vecindario ya que no hay arcos para iterar, para instancias pequeñas
            
            rutasPasadas = 0;
            vecindario = two_opt(rutasActuales[i]); //vecindario de movimientos para la ruta i
            indexCmin = -1; //comienza en -1 para poder diferenciar si es tomada alguna solucion o no
            Cmin = std::numeric_limits<double>::max(); //el numero double mas grande posible inicializa Cmin
            for (size_t t = 0; t < vecindario.size(); t++) //for de vecindario de la ruta i
            {
                vecTRiesgo = riesgo(instancia.matrizAdyacencia, instancia.nodes, vecindario[t]); //obtiene riesgo de ruta del vecindario
                Cf_value = f_evaluacion(instancia.matrizAdyacencia, vecindario[t]); //se evalua la ruta en la funcion de evaluacion
                n_feval++;
                if ( Cf_value < Cmin && vecTRiesgo <= instancia.riskThreshold) //verifica restricciones y mejora
                {
                    auto p1 = std::mismatch(rutasActuales[i].begin(), rutasActuales[i].end(), vecindario[t].begin()); //diferencias del inicio
                    auto p2 = std::mismatch(rutasActuales[i].rbegin(), rutasActuales[i].rend(), vecindario[t].rbegin()); //diferencias del final
                    k = std::distance(rutasActuales[i].begin(), p1.first); //k, el primer index diferente
                    j = rutasActuales[i].size() - 1 - std::distance(rutasActuales[i].rbegin(), p2.first); //j, el final de lo diferente entre las rutas
                    mov = std::make_tuple(k, j); //la tupla del movimiento

                    if (std::find(listasTabu[i].begin(), listasTabu[i].end(), mov) != listasTabu[i].end())
                    { //verifica el movimiento en la lista tabu
                        continue; //el movimiento ya esta en la lista tabu, no se puede tomar
                    }
                    else 
                    {//el movimiento no esta en la lista tabu, asi que si se puede tomar, actualiza indexCmin y Cmin
                        Cmin = Cf_value; //se guarda el costo minimo actual de la ruta factible
                        indexCmin = t; //tambien el indice de la ruta del costo minimo factible
                        Rnew = vecTRiesgo; //se guarda el riesgo de la rtua para no volver a calcularlo
                        movToPush = mov; //se guarda el movimiento que genera a la nueva solucion tomada
                    }
                }
            }

            push_back_tabu(listasTabu[i], movToPush, listasTabuLEN[i]); //se hace push del movimiento finalmente utilizado

            if (Cmin < solucion.rutasC[i]) //la ruta elegida del vecindario es de mejor calidad que la ruta actual o no
            {
                solucion.rutas[i] = vecindario[indexCmin];
                solucion.rutasC[i] = Cmin;
                solucion.rutasR[i] = Rnew;
            }

            if (indexCmin != -1) //en caso de que no pueda tomar una, ver que hacer, definir una probabilidad de tomar una ruta tabu
            {
                rutasActuales[i] = vecindario[indexCmin]; //pudo tomar algun valor, cosa que no siempre es segura por la lista tabu y los infactibles
            }
            else
            {
                random = dis(gen); //genera un numero random entre 0 y 1
                if (random <= PICK_P) //si es menor a la probabilidad de tomar una solucion tabu, se hace
                {
                    for (size_t v = 0; v < listasTabu[i].size()-1; v++)
                    {
                        Tupla movTabu = listasTabu[i][v]; //se obtiene la tupla del movimiento tabu
                        ktabu = std::get<0>(movTabu); //se obtiene el elemento k tabu de la lista tabu, de cada movimiento
                        jtabu = std::get<1>(movTabu); //se obtiene el elemento j tabu de la lista tabu, de cada movimiento

                        rutaTabu = two_opt_one(rutasActuales[i], ktabu, jtabu); //genera el movimiento para obtener una de las rutas tabu

                        Ctabu = f_evaluacion(instancia.matrizAdyacencia, rutaTabu);
                        n_feval++;
                        Rtabu = riesgo(instancia.matrizAdyacencia, instancia.nodes, rutaTabu);
                        if (Rtabu <= instancia.riskThreshold) //verifica si la ruta tabu es factible
                        {
                            if (Ctabu < solucion.rutasC[i]) //la ruta tabu es mejor que la ruta mejor actual
                            {
                                solucion.rutas[i] = vecindario[indexCmin]; //se toma la ruta de la solucion mejor actual
                                solucion.rutasC[i] = Cmin; //se toma el costo de la ruta de la solucion mejor actual
                                solucion.rutasR[i] = Rnew; //se toma el riesgo de la ruta de la solucion mejor actual
                            }
                            rutasActuales[i] = rutaTabu; //se actualza la ruta i actual
                            break; //sale del for dado que ya agregó una solución tabú
                        }
                    }
                    PICK_P -= 0.1; //disminuye la probabilidad para hacer mas dificil que continue explorando demasiado conforme sucedan estas excepciones
                }
            }
            vecindario.clear();
        }

        for (size_t p = 0; p < puntos.size(); ++p) //itera sobre los puntos posibles de reduccion de listas tabu
        {
            if (iteraciones >= puntos[p] && !indicadores[p]) //verifica si no se hizo el cambio ya y si las iteraciones pasan los puntos
            {
                indicadores[p] = true; //cambia el indicador a true
                for (size_t i = 0; i < listasTabu.size(); i++) //hace el cambio de tamaño en las listas tabu
                {
                    if (listasTabu[i].size() >  static_cast<size_t>(listasTabuLEN[i] / 2) ) //solo se acorta a al mitad si la lista es de largo mayor a la mitad de su len fijo
                    {
                        listasTabu[i].erase(listasTabu[i].begin(), listasTabu[i].begin() + listasTabu[i].size() / 2); //se borra la primera mitad de la lista tabu
                        listasTabuLEN[i] = listasTabu[i].size(); //se reasigna el nuevo largo de la lista tabu
                    }
                }
            }
        }

        iteraciones++; //aumenta en 1 el contador de iteraciones
        std::cout << "Iteración: " << std::setw(3) << iteraciones << '\r'; //muestra el contador en consola para saber que se avanza
        std::cout.flush();
    }

    solucion.nodesAtt = solucionC.nodesAtt; //se asisgnan los nodos atendidos

    solucion.cTotal = 0.;
    for (int i = 0; i < solucion.nVehiculos; i++) //se itera sobre los vehículos utilizados para la solución
    {
        solucion.cTotal += solucion.rutasC[i]; 
    }
    
    std::cout << std::endl;
    std::cout << "feval: " << n_feval << std::endl;
    std::cout << std::endl;
    return solucion; //retorna la nueva solucion
}
