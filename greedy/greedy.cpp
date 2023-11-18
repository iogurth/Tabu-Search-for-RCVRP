#include "greedy.h"
#include <fstream>
#include <iostream>
#include <cmath>

int funcionMiope(const std::vector<std::vector<double>>& matrizAdyacencia, const std::vector<int>& ruta, const double& T,
                const std::vector<int>& nodosRestantes, double& rutaR, double& rutaD, double& rutaC, const std::vector<Node>& nodes)
{
    int index = -1; //inicializa index
    int lenRestantes = nodosRestantes.size();
    int lenRuta = ruta.size();

    bool first = true; //flag que hace agregar el primer elemento que encuentre (factible) al comienzo ya que será lo mejor hasta allí 
    double Rmin = 0; //R minimo actualizable, inicia en 0
    double Dmin = 0; //D minimo actualizable, inicia en 0
    double Cmin = 0; //C minimo actualizable, inicia en 0
    double R, D, C;

    for (int i = 0; i < lenRestantes; i++) //for que itera sobre los nodos restantes
    {
        
        R = rutaR - rutaD * matrizAdyacencia[ruta[lenRuta-1]][0]; //se elimina el arco final de R hacia el deposito
        R = R + rutaD * matrizAdyacencia[nodosRestantes[i]][ruta[lenRuta-1]]; //se calcula R hacia el nuevo nodo agregado
        D = rutaD + nodes[nodosRestantes[i]].demand; //se calcula D con el nuevo nodo agregado
        R = R + D * matrizAdyacencia[nodosRestantes[i]][0]; //se termina de calcular R al volver con el nodo agregado

        C = rutaC - matrizAdyacencia[ruta[lenRuta-1]][0]; //resta la distancia de volver al deposito
        C = C + matrizAdyacencia[nodosRestantes[i]][ruta[lenRuta-1]]; //suma la distancia de moverse a otro arco
        C = C + matrizAdyacencia[nodosRestantes[i]][0]; //suma la distancia de regresar al deposito

        if (first == true && R <= T)
        {
            Rmin = R; //actualiza Rmin con el nuevo R
            Dmin = D; //actualiza Dmin con el nuevo D
            Cmin = C; //actualiza Cmin con el nuevo C
            index = i; //actualiza el indice del nodo para retornar
            first = false; //desactiva el flag para las proximas iteraciones
        }
        else if (R <= T && C < Cmin) //verifica la restriccion del risk threshold maximo
        {
            Rmin = R; //actualiza Rmin con el nuevo R
            Dmin = D; //actualiza Dmin con el nuevo D
            Cmin = C; //actualiza Cmin con el nuevo C
            index = i; //actualiza el indice del nodo para retornar
        }
    }
    if (index != -1) 
    {
        rutaC = Cmin; //actualiza el valor de rutaC, es decir, solucion.rutasC[j] en la funcin Greedy
        rutaR = Rmin; //actualiza el valor de rutaR, es decir, solucion.rutasR[j] en la funcin Greedy
        rutaD = Dmin; //actualiza el valor de rutaD, es decir, solucion.rutasD[j] en la funcin Greedy
    }
    return index; //retorna el indice del nodo restante que se puede agregar a la ruta en la funcion Greedy, es -1 si no se puede ninguno
}

RCVRPSolucion greedy(const RCVRPInstancia& instancia)
{
    RCVRPSolucion solucion;
    
    int n = instancia.numNodes - 1;
    //int nNodosSinVicitar = n;
    std::vector<int> nodosRestantes(n);
    std::iota(nodosRestantes.begin(), nodosRestantes.end(), 1); //crea vector nodosRestantes, con elementos desde el 1 hasta instancia.numNodes - 1

    int j, popIndx, cter;
    for (int i = 1; i <= n; i++) //for que limita el número de vehículos a n (restriccion)
    {
        //solucion.nVehiculos = i * floor(std::sqrt( n / nNodosSinVicitar ) ); //inicializa el número de vehículos
        solucion.nVehiculos = i; //inicializa el número de vehículos
        solucion.rutasD.assign(solucion.nVehiculos, 0);
        solucion.rutasC.assign(solucion.nVehiculos, 0);
        solucion.rutasR.assign(solucion.nVehiculos, 0);
        solucion.rutas.resize(solucion.nVehiculos); //inicializa el tamaño del vector de las rutas en funcion de los vehículos

        for (int k = 0; k < solucion.nVehiculos; k++) { solucion.rutas[k].assign(1, 0); } //inicializa las rutas con el nodo salida

        j = 0; //establece j en 0 cada iteración de vehículos
        cter = 0; //cter se inicia en 0 cada iteración de vehículos

        while(nodosRestantes.size() > 0)
        {
            
            popIndx = funcionMiope(instancia.matrizAdyacencia, solucion.rutas[j], instancia.riskThreshold, nodosRestantes, solucion.rutasR[j], 
                                    solucion.rutasD[j], solucion.rutasC[j], instancia.nodes); //llamada a la función
            
            if (popIndx == -1)
            {
                cter++; //se aumenta cter en 1 indicando que el vehículo en cuestion no pudo agregar ningún nodo
                if (cter == solucion.nVehiculos) //si cter es igual a la cantidad de vehículos, se entiende que ninguno pudo agregar nodos
                {
                    nodosRestantes.resize(n); //se reajusta el tamaño de nodosRestantes
                    std::iota(nodosRestantes.begin(), nodosRestantes.end(), 1); //se rellena nuevamente con elementos de 1 a n
                    break; //como no pudo agregar nodos ningún vehículo, rompe el ciclo while
                }
                j = (j + 1) % solucion.nVehiculos; //si cter no es igual a la cantidad de vehículos, solo crece j
                continue; //posteriormente, ignora el resto del código e itera nuevamente con un nuevo j
            }
            solucion.rutas[j].push_back(nodosRestantes[popIndx]); //se agrega el nodo del índice obtenido desde la función Miope

            std::vector<int>::iterator iter = nodosRestantes.begin() + popIndx; //se crea un iterador para nodosRestantes
            nodosRestantes.erase(iter); //se elimina el elemento del iterador, es decir, el elemento agregado a la ruta del vehículo j
            j = (j + 1) % solucion.nVehiculos; //j crece

            cter = 0; //como se logró agregar un elemento a un vehículo, las demás rutas pueden cambiar por lo que cter vuelve a 0
        }
        if (nodosRestantes.size() == 0) //si no quedan nodos restantes, rompe el for indicando que se encontró una solución de rutas Greedy
        {
            break;
        }
    }
    solucion.cTotal = 0;
    //solucion.rutasC.assign(solucion.nVehiculos, 0);
    for (int i = 0; i < solucion.nVehiculos; i++) //se itera sobre los vehículos utilizados para la solución
    {
        solucion.rutas[i].push_back(0); //se agrega el nodo final depósito a las rutas,
        //solucion.rutasC[i] = f_evaluacion(instancia.matrizAdyaciencia, solucion.rutas[i]);
        solucion.cTotal += solucion.rutasC[i]; 
    }
    solucion.nodesAtt.resize(instancia.numNodes-1);
    std::iota(solucion.nodesAtt.begin(), solucion.nodesAtt.end(), 1); //se asignan los nodos recorridos a solución, que son todos para este punto
    
    return solucion; //la función retorna la solución Greedy encontrada
}
