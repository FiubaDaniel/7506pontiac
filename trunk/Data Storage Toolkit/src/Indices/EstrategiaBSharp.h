
#ifndef ESTRATEGIABSHARP_H
#define ESTRATEGIABSHARP_H
#include "EstrategiaIndice.h"
#include "../ArbolBSharp/Clave.h"
#include "../ArbolBSharp/ComparadorClave.h"
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

  void crear(std::string nombreArch,unsigned int tamanioBloque,int tamanioSerializadoClave,ComparadorClave* comp);
  void abrir(std::string nombreArch,ComparadorClave* comp);
  bool BuscarReferencia(const Clave* clave,Referencia* referencia);
  Referencia Siguiente(bool ultimo);
  bool insertar(Referencia ref,Clave* clave);
  bool eliminar(const Clave* clave);
  bool modificar(const Clave* clave,Referencia refNueva);

private:
  BSharpTree* arbol;
};

#endif // ESTRATEGIABSHARP_H
