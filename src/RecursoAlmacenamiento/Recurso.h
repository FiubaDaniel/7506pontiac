
#ifndef RECURSOALMACENAMIENTO_H
#define RECURSOALMACENAMIENTO_H
#include "EstrategiaRecursos.h"
#include "../Almacenamiento/Almacenamiento.h"
#include "../Almacenamiento/EstrategiaAlmacenamiento.h"
#include "../Indices/EstrategiaIndice.h"
#include "../Compuesto/Registro.h"
#include "../ArbolBSharp/ComparadorClave.h"
#include "../ArbolBSharp/Clave.h"
class EstrategiaRecursos;

class Recurso{
public:
   Recurso(Almacenamiento* archivo,EstrategiaIndice* indice,EstrategiaRecursos* estrategiaUsada,EstrategiaAlmacenamiento*estretegiaAlmacenamiento);
   virtual ~Recurso ( );
   /*
    * Agrega un Registro.
    * return :
    * true, si pudo insertar, false sino si el registro ya existe.
    */
   bool insertar(Registro* registro);
   /*
    * Elimina un Registro con la clave proporcionada.
    * return :
    * true, si pudo eliminar, false sino el registro no existe.
    */
   bool eliminar(Clave* unaClave);
   /*
    * Modifica el Registro que coincide con la clave proporcionada.
    * return:
    * true, si pudo modificar, false sino si el registro no existe.
    */
   bool modificar(Clave *unaClave,Registro* registro);
   /*
    * Busca el registro de acuerdo a la clave.
    * return:
    * true, si pudo eliminar, false sino el registro no existe.
    */
   bool obtener(Clave*unaClave,Registro*registro);
   void mostrarIndice();
   void mostrarDatos();
   void cerrarArchivo();
private:
   Clave *clave;
   ComparadorClave*comparador;
   EstrategiaAlmacenamiento*estrategiaAlmacenamiento;
   Almacenamiento* archivo;
   EstrategiaIndice* indice;
   EstrategiaRecursos* estrategiaUsada;
};

#endif // RECURSOALMACENAMIENTO_H
