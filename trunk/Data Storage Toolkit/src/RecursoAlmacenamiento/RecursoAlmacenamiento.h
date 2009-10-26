
#ifndef RECURSOALMACENAMIENTO_H
#define RECURSOALMACENAMIENTO_H
#include "EstrategiaRecursos.h"
#include "Almacenamiento.h"
#include "EstrategiaIndice.h"

class EstrategiaRecursos;

class RecursoAlmacenamiento{
	EstrategiaRecursos* estrategiaUsada;
public:
  RecursoAlmacenamiento ();
  virtual ~RecursoAlmacenamiento ( );
  bool inicializar();
  bool agregar(Componente unComponente);
  bool eliminar(std::string unaClave);
  bool modificar(std::string unaClave, Componente unComponente);
  Componente obtener(std::string unaClave);
  void mostrarContenidoBuffer();
  void mostrarIndice();
  void mostrarDatos();
  void cerrarArchivo();
private:
  Almacenamiento* buffer=null;
  Almacenamiento* archivo=null;
  EstrategiaIndice* indice=null;
};

#endif // RECURSOALMACENAMIENTO_H
