CXX = g++  # El compilador g++
CXXFLAGS = -std=c++11 -Wall  # Opciones de compilación de C++

# Lista de archivos fuente
SOURCES = outs/outs.cpp lectura-instancia/instancias.cpp greedy/greedy.cpp f_evaluacion/f_evaluacion.cpp tabu_search/tabu_search.cpp main.cpp

# Objetos generados a partir de los archivos fuente
OBJECTS = $(SOURCES:.cpp=.o)

# Nombre del archivo ejecutable
EXECUTABLE = rcvrp

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm

# Regla para compilar archivos fuente en objetos
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: all clean
