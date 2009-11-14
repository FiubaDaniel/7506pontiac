/*
 * NodoHoja.h
 *
 *  Created on: 30/09/2009
 *      Author: daniel
 */

#ifndef NODOHOJA_H_
#define NODOHOJA_H_
#include "Nodo.h"
#include "NodoIntermedio.h"
#include "ElementoNodo.h"

using namespace std;

class NodoHoja: public Nodo{

private:
	    unsigned int pos;
		Referencia referenciaSiguiente;
		/*
		 * Retorna un elemento buscado.
		 */
		bool buscar(const Clave* clave,ElementoNodo*& elemento);
public:
	/*
	 * Los nodos hojas deben poder crearce de dos formas, una mediante una tira de bytes
	 * que se obtiene del archivo y otra mediante nuevos datos.
	 */
	/*
	* Se crea el  nodo hoja a partir de un buffer, la cantidad de elementos, el tamaño de
    * la hoja y el tamaño de la clave  son parametros ya q se guardan solo una vez.
	*/
		NodoHoja(std::stringbuf* buf,int unsigned cantElem,ComparadorClave* comp,Clave* claveEstructural);

		NodoHoja(unsigned int cantElem,Referencia refSiguiente,ComparadorClave* comp);

		virtual ~NodoHoja();

		void setReferenciaSiguiente(Referencia ref);
    /*
     * Setea la referencia de una clave determinada
     */
		bool setReferenciaDeClaveX(const Clave* clave,Referencia refNueva);
	/*
	* Devuelve 1 si ubico el elemento o devuelve 0 si ya existia
	* En caso de estar lleno devuelve 2, indicando q el nodo ya tiene todos
	* sus elementos, como concecuencia debe reorganizarce colocando un elemento
	* en un nodo hermano o producir la generacion de otro nodo. el elemento se agregara
    * en la posicion correcta enviando el mensaje de arreglar esa situacion
	*/
		int agregarElemento(ElementoNodo* elemento);
	/*
     * El tamaño de la clave ya fue serializado al igual q la cantidad de elementos por nodo
	 * al princio del archivo.
	 */
		void serializate(stringbuf* buffer);
    /*
	 * El elemento ya se elimino antes, se llega a esta funcion por un subflujo sin posibilidad de balanceo.
	 */
		int unirse(Nodo* nodoHermanoIzq,Nodo* nodoHermanoDer,Nodo* Padre);
	/*
	* Se reacomodan los elementos y las referencias q los unen, el nuevo nodo seria el medio, como concecuencia las referencias a es seran
	* indefinidas y se estableceran fuera del metodo.
	* Clave indica la clave del padre q separa los nodos completos.
	* El elemento q se agregue al padre (q apuntara a nodo medio) tmb tiene su referencia indefinida y debe ser agregado fuera de la funcion.
	*/
	/*
	* Precondicion: el nodo que llame a la funcion dividirce debe ser el izq.
	*/
		ElementoNodo* dividirse(Nodo* nodoHermanoE,Nodo* nodoIzqE,Nodo* nodoMedioE,Nodo* nodoDerE,Nodo* nodoPadreE,Clave* clave);
	/*
	* Situacion de desborde: El nodo this contiene un elemento mas del permitido, siendo q el nodoHermano q
	* pasa por parametro tiene al menos una posicion libre para poder alojar ese elemento
	* izq indica si es el nodo izq con el q se valancea o si el false con el derecho.
	* Situacion de subflujo: el nodo this es quien posee al menos un elemento mas del minimo, siendo el nodo
	* q tiene subflujo el nodoHermano.
	*/
		void balanceo(Nodo* nodoIncompletoE,Nodo* nodoPadreE,bool izq);
	/*
	 * Izq indica si los hemanos con que se balancea estan a izq si es verdadero o flaso si estan a derecha
	*/
		void balanceoEspecial(Nodo* nodoPegado,Nodo* nodoAlejado,Nodo* padre,bool Izq);
	/*
	 * Devuelve en elemento el elemento buscado o en su defecto si no existe devuelve el sigueite elemento.
	 * retorna un valor entero. -1 si no encontro. 0 si es igual al q se buscaba. 1 si es el siguiente.
	*/
		int busquedaSecuencial(const Clave* clave,ElementoNodo*& elemento);

		Referencia getReferenciaSiguiente();
	/*
	* Bucas la clave dentro del nodo Hoja, y si la encuentra la retorna la referencia
	* al archivo de esa clave.En caso de no existir retorna el valor -1.
	*/
		bool buscarReferenciaDeClaveX(const Clave* clave,Referencia* ref);

		void setPos();

		void avanzarPos();

		ElementoNodo* getPos();

		unsigned int numeroPos();
};
#endif /* NODOHOJA_H_ */
