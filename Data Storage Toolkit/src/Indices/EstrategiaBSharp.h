
#ifndef ESTRATEGIABSHARP_H
#define ESTRATEGIABSHARP_H
#include "EstrategiaIndice.h"
#include "../ArbolBSharp/BSharpTree.h"
#include "../ArbolBSharp/Clave.h"
#include "../ArbolBSharp/ComparadorClave.h"
#include "../ArbolBSharp/ComparadorRegistroVariable.h"
#include "../ArbolBSharp/ComparadorRegistroFijo.h"
#include "../ArbolBSharp/Clave.h"
#include "../ArbolBSharp/ElementoNodo.h"
#include "../ArbolBSharp/Nodo.h"
#include "../ArbolBSharp/NodoIntermedio.h"
#include "../ArbolBSharp/NodoHoja.h"
#include "../ArbolBSharp/TamanioInsuficienteException.h"

#include <string>

class EstrategiaBSharp : virtual public EstrategiaIndice{
public:

  EstrategiaBSharp (Clave* clave);

  virtual ~EstrategiaBSharp ( );

  void crear(std::string nombreArch,unsigned int tamanioBloque,Clave* clave,ComparadorClave* comp);
  bool abrir(std::string nombreArch,ComparadorClave* comp);
  bool BuscarReferencia(const Clave* clave,Referencia* referencia);
  Referencia Siguiente(bool ultimo);
  bool insertar(Referencia ref,Clave* clave);
  bool eliminar(const Clave* clave);
  bool modificar(const Clave* clave,Referencia refNueva);
  void cerrar();
  int tamanioBloque();
  void mostrarEstado();
  BSharpTree* obtenerArbol();

private:
  BSharpTree* arbol;
};

#endif // ESTRATEGIABSHARP_H
