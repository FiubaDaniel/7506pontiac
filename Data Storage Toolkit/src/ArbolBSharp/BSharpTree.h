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
	//ver los dos primeros q tengo q meter el constructor adentro
	void crear(char* nombreArch,char* nomEspaciosLibres,unsigned int tamanioBloque,int tamanioSerializadoClave,ComparadorClave* comp);
	void abrir(char* nombreArch,char* nomEspaciosLibres,ComparadorClave* comp);
	BSharpTree(char* nombreArch,char* nomEspaciosLibres,unsigned int tamanioBloque,int tamanioSerializadoClave,ComparadorClave* comp);
	BSharpTree(char* nombreArch,char* nomEspaciosLibres,ComparadorClave* comp);
	bool Buscar(Clave clave,Referencia* referencia);
	Referencia Siguiente(bool ultimo);
	bool insertar(Referencia ref,Clave* clave);
	bool eliminar(Clave clave);
	bool modificar(Clave clave,Referencia refNueva);
	/*Falta el listar*/
	virtual ~BSharpTree();
private:
	NodoHoja* buscarPrimerNodoHoja(NodoIntermedio nodo);
	NodoHoja* buscarHoja(NodoIntermedio nodo,Clave clave,Referencia referenciaDeNodoHoja);
	bool buscarIterativo(NodoIntermedio nodo,Clave clave,Referencia* ref,NodoHoja* ultimo);
	int calcularCantidadElementosPorNodo(int tamSerializadoClave);
	void BuscarInsertarOEliminar(Nodo* hoja,std::list<Referencia>&listaDePadres,NodoIntermedio* nodo,Clave clave,Referencia refHoja,bool& esRaiz,bool esInsertar);
	Referencia buscarEspacioLibre();
	void grabado(Nodo* original,Nodo* hermano,Nodo* padre,Referencia refOriginal,Referencia refPadre,Referencia refHermano);
	void grabar(Nodo* nodoOriginal,Nodo* nodoHermano,Referencia refOriginal,Referencia refHermano);
	void grabarUnitario(Nodo* nodo,Referencia ref);
	void destrozarLista(std::list<NodoIntermedio*>);
	void resolverDesborde(Nodo* nodo,list<Referencia>&listaDePadres,Referencia refHijo);
	void desbordeRaiz();
	void buscarNodoBalancearODividir(NodoIntermedio padre,Nodo* hijo,Nodo* hermano,Referencia& refHijo,bool& izq,bool& balancear,Referencia refHermano,ElementoNodo* elemPadre);
	void obtenerHermano(Referencia ref,Nodo* Hermano,unsigned int nivel,bool& balancear);
	ElementoNodo* ObtenerRaiz();
	void modificarLista(std::list<Referencia>&listaDePadres,bool esInsertar,Nodo* nodo);
	void resolverSubflujo(Nodo* nodo,std::list<Referencia>&listaDePadres,Referencia refHijo);
	void resolverReferenciaSiguiente(Nodo* nodoIzq,Referencia refAHermanoNuevo);
	void subflujoHijosRaiz(Nodo* nodo,Nodo* hermano,Referencia refNodo,Referencia refHermano);
	void buscarHermanos(Nodo* nodoActual,NodoIntermedio* padre,Nodo* hermanoIzq,Nodo* hermanoDer,Referencia& refHermanoIzq,Referencia& refHermanoDer,Referencia refPadre,vector<bool> &informacion);
	void armarNuevaRaiz(Nodo* nodoIzq,Nodo* nodoDer);
	void nuevoEspacioLibre(Referencia);
	void destruirNodos(Nodo* nodo,Nodo* hermanoDer,Nodo* hermanoIzq);
	void eliminarClaveEnIntermedio(Clave claveAeliminar,Clave* claveSetear);
	NodoIntermedio* buscarIntermedio(Clave clave,NodoIntermedio* nodo);
	unsigned int numeroDeElementosXnodo,posicionRaiz,tamanioNodo,cantidadMinimaDeElementos;
	std::fstream archivoArbol;
	std::fstream archivoEspaciosLibres;
	char* nombreArchivo;
	char* nombreEspaciosLibres;
	ComparadorClave* comparador;
	NodoIntermedio* Raiz;
	NodoHoja* ultimoNodo;
};

#endif /* BSHARPTREE_H_ */
