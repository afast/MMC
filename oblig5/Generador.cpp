#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define DEBUG 0

using namespace std;

class Generador {
  ifstream archivo;
  istringstream lineaActual;
  char * nombreArchivo;
  
  public:
    int a, ancho; //extremos del intervalo
    bool descartarPrimero, lecturaCompleta;
    void iniciar(char * nombre, bool descartarPrimerNumero) {
      nombreArchivo = nombre;
      descartarPrimero = descartarPrimerNumero;
      archivo.open(nombreArchivo);
      readLineFromFile();
    }
    void iniciar(char * nombre, bool descartarPrimerNumero, int extremo_a, int extremo_b) {
      nombreArchivo = nombre;
      descartarPrimero = descartarPrimerNumero;
      a = extremo_a;
      ancho = extremo_b - a;
      archivo.open(nombreArchivo);
      readLineFromFile();
    }
    void readLineFromFile() {
      string line;
      int descartado;
      if (!archivo.is_open())
        archivo.open(nombreArchivo);
      if (archivo.eof()) {
        if (DEBUG)
        cout << "End of file detected, start anew" << endl;
        archivo.clear();
        archivo.seekg(0);
        if (DEBUG)
        cout << "File state: " << archivo.good() << endl;
      }
      if (archivo.good()) {
        getline(archivo, line);
        if (DEBUG)
        cout << "Linea leida: " << line << endl;
        lineaActual.str(line);
        lineaActual.clear();
        lineaActual.seekg(0);
        if (descartarPrimero) {
          lineaActual >> descartado;
          if (DEBUG)
          cout << "Descartado: " << descartado << endl;
        }
      }
    }
    double getNextRand() {
      return (double)(getNextNum()-a)/ancho;
    }
    int getNextNum() {
      int result;
      if (DEBUG)
        cout << "Linea Actual: " << lineaActual.eof() << endl;
      if (lineaActual.eof())
        readLineFromFile();
      lineaActual >> result;
      if (lineaActual.eof() && archivo.eof())
        lecturaCompleta = true;
      return result;
    }
    void cerrar() {
      archivo.close();
    }
    void normalizar() {
      int max, min, aux;

      cout << "Normalizando el generador para generar entre [0,1]... ";
      max = min = getNextNum();
      
      while (!lecturaCompleta) {
        aux = getNextNum();
        if (aux > max)
          max = aux;
        if (aux < min)
          min = aux;
      }
      a = min;
      ancho = max-min;
      cout << "normalizado" << endl;
      archivo.clear();
      archivo.seekg(0);
      lineaActual.clear();
      lineaActual.seekg(0);
      readLineFromFile();
    }
};
