#ifndef ESTRATEGIARECURSOS_H
#define ESTRATEGIARECURSOS_H
#include "Recurso.h"
#include "../Compuesto/Registro.h"
#include "../ArbolBSharp/Clave.h"
#include "../Almacenamiento/EstrategiaAlmacenamiento.h"
#include "../Indices/EstrategiaIndice.h"
class Recurso;

class EstrategiaRecursos{
public:
	/*getters de los elementos que la estrategia esta utilizando*/
	virtual  EstrategiaAlmacenamiento *getEstrategiaAlmacenamiento()=0;
	/**/
	virtual  EstrategiaIndice *getIndice()=0;
	/*estrategia= es una EstrategiaAlmacenamiento estrategia lista para ser usada*/
	virtual void setEstrategiAlmacenamiento(EstrategiaAlmacenamiento *estrategia)=0;
	/*indice=es una EstrategiaIndice lista para se usada*/
	virtual void setIndice(EstrategiaIndice *indice)=0;
	/* inserta el registro,coordinando el indice y la estrategia seteados.
	 * si el registro ya se encuentra almacenado no realiza ninguna operacion y devuelve false.
	 */
	virtual bool insertar(Registro* registro)=0;
	/*elimina el registro,coordinando el indice y la estrategia seteados.
	 * de no existir el registro devuelve false y no realiza ninguna accion.
	 */
	virtual bool eliminar(Clave* unaClave)=0;
	/*modifica el registro,coordinando el indice y la estrategia seteados
	 * de no existir el registro devuelve false y no realiza ninguna accion.
	 */
	virtual bool modificar(Clave* unaClave,Registro* registro)=0;
	/*obtiene el registro,coordinando el indice y la estrategia seteados.
	 *de no existir el registro devuelve false y no realiza ninguna accion.
	 */
	virtual bool obtener(Clave* unaClave,Registro*registro)=0;
};
#endif // ESTRAGIARECURSOS_H
