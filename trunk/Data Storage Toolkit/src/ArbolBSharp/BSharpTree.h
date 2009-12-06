/*
 * BSharpTree.h
 *
 *  Created on: 09/10/2009
 *      Author: daniel
 */

#ifndef BSHARPTREE_H_
#define BSHARPTREE_H_
#include "Clave.h"
#include "ElementoNodo.h"
#include "Nodo.h"
#include "NodoIntermedio.h"
#include "NodoHoja.h"
#include "ComparadorClave.h"
#include <fstream>
#include <sstream>
#include <string.h>

class BSharpTree {
public:

	BSharpTree(Clave* clave);
	/*
	 * Crea el archivo donde se alojara el arbol y el archivo donde se alojan los espacios que quedan libres
	 * al eliminarce nodos ya no utilizados.
	 * Setea todos los valores Iniciales necesarios para el funcionamiento del arbol como el tamaño de los nodos,
	 * la cantidad de registros que contienen los nodos y la posicion de la raiz.
	 */
	void crear(string nombreArch,unsigned int tamanioBloque,Clave* clave,ComparadorClave* comp);
	/*
	 * Abre el archivo del arbol y de espacios libres. setea la raiz que siempre se encuentra disponible en
	 * memoria.
	 */
	bool abrir(string nombreArch,ComparadorClave* comp);
	/*
	 * Busca la referencia al archivo de un registro segun la clave determinada
	 * y la almacena en referencia.
	 * Devuelve verdadero en caso de que exista la clave y falso si la clave buscada no se
	 * encuentra en el arbol.
	 */
	bool Buscar(const Clave* clave,Referencia* referencia);
	/*
	 * Devuelve la referencia al archivo siguiente al registro buscado por ultima vez en caso de
	 * que el valor parametro sea verdadero, en caso contrario devuelve el primer elemento del arbol.
	 */
	Referencia Siguiente(bool ultimo);
	/*
	 * Inserta una clave con su referencia en el arbol en forma ordenada.
	 */
	bool insertar(Referencia ref,Clave* clave);
	/*
	 * Elimina el registro del arbol de la clave dada. devuelve falso si la clave no existe
	 * en el arbol.
	 */
	bool eliminar(const Clave* clave);
	/*
	 * Modifica el valor de referencia de la clave dada.
	 */
	bool modificar(const Clave* clave,Referencia refNueva);
	/*
	 * Imprime el arbol en panatalla
	 */
	void imprimir();
	/*
	 * deuelve verdadero si esta vacio.
	 */
	bool estaVacio();

	void cerrar();

	int tamanioBloque();

	bool siguienteAlmacenado(Nodo* &nodo);

	void escribir(std::stringbuf* buffer);

	void posicionarArchivo();

	std::string getMetadata();

	void setMetadata(char* metadata);

	void recomponerRaiz();

	void imprimirNodo(Nodo* nodoE);

	void observarNodo(std::stringbuf* buffer);

	std::string getNombreArchivo();

	virtual ~BSharpTree();

private:

	NodoHoja* buscarPrimerNodoHoja(Nodo* nodoE);

	NodoHoja* buscarHoja(Nodo* nodoE,Clave* clave,Referencia& referenciaDeNodoHoja);

	bool buscarIterativo(NodoIntermedio* nodo, Clave* clave,Referencia* ref,NodoHoja* ultimo);

	int calcularCantidadElementosPorNodo(unsigned int tamSerializadoClave);

	void BuscarInsertarOEliminar(Nodo* &hoja,std::list<Referencia>&listaDePadres,Nodo* nodo, Clave* clave,Referencia& refHoja,bool& esRaiz,bool esInsertar);

	Referencia buscarEspacioLibre();

	void grabado(Nodo* original,Nodo* hermano,Nodo* padre,Referencia refOriginal,Referencia refPadre,Referencia refHermano);

	void grabar(Nodo* nodoOriginal,Nodo* nodoHermano,Referencia refOriginal,Referencia refHermano);

	void grabarUnitario(Nodo* nodo,Referencia ref);

	void destrozarLista(std::list<NodoIntermedio*>);

	void resolverDesborde(Nodo* nodo,list<Referencia>&listaDePadres,Referencia refHijo);

	void desbordeRaiz(Nodo* RaizE);

	void buscarNodoBalancearODividir(NodoIntermedio* padre,Nodo* hijo,Nodo*& hermano,Referencia refHijo,bool& izq,bool& balancear,Referencia& refHermano,ElementoNodo*& elemPadre);

	void obtenerHermano(Referencia ref,Nodo*& Hermano,unsigned int nivel,bool& balancear);

	ElementoNodo* ObtenerRaiz();

	void modificarLista(std::list<Referencia>&listaDePadres,bool esInsertar,Nodo* nodo);

	void resolverSubflujo(Nodo* nodo,std::list<Referencia>&listaDePadres,Referencia refHijo);

	void resolverReferenciaSiguiente(Nodo* nodoIzq,Referencia refAHermanoNuevo);

	void subflujoHijosRaiz(Nodo* nodo,Nodo* hermano,Nodo* padre,Referencia refNodo,Referencia refHermano);

	void armarRaizIntermedia(Nodo* nodoIzq,Nodo* nodoDer,Nodo* padre,Referencia refNodoIzq,Referencia refNodoDer);

	void armarRaizHoja(Nodo* nodoIzq,Nodo* nodoDer,Referencia refIzq,Referencia refDer);

	void nuevoEspacioLibre(Referencia);

	void destruirNodos(Nodo* nodo,Nodo* hermanoDer,Nodo* hermanoIzq);

	void eliminarClaveEnIntermedio(Clave* claveAeliminar,Clave* claveSetear);

	NodoIntermedio* buscarIntermedio(Clave* clave,Nodo* nodoE,bool esRaiz,Referencia& refAux);

	void buscarHermanos(Nodo* nodoActual,NodoIntermedio* padre,Nodo* &hermanoIzq,Nodo* &hermanoDer,Referencia& refHermanoIzq,Referencia& refHermanoDer,Referencia refHijo,vector<bool> &informacion);

	Referencia obtenerReferenciaHermano(Nodo* padre,Clave* clave,bool Izq);

	Nodo* obtenerHermanoXsuBflujo(int nivel,Referencia ref);

	Nodo* obtenerNodoPorPosiciones(Referencia posInicial);

	void imprimirIterativo(Nodo* nodo);

	void reacomodarArbol(NodoHoja*& hojaIzq,NodoHoja*& hojaDer,NodoIntermedio*&nuevaRaiz);

	unsigned int numeroDeElementosXnodo,posicionRaiz,cantidadMinimaDeElementos;

	unsigned int tamanioNodo;

	std::fstream archivoArbol;
	std::fstream archivoEspaciosLibres;
	string nombreArchivo;
	string nombreEspaciosLibres;
	ComparadorClave* comparador;
	Nodo* Raiz;
	NodoHoja* ultimoNodo;
	Clave * claveEstructural;
};

#endif /* BSHARPTREE_H_ */
