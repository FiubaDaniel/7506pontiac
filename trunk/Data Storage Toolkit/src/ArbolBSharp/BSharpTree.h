/*
 * BSharpTree.h
 *
 *  Created on: 09/10/2009
 *      Author: daniel
 */

#ifndef BSHARPTREE_H_
#define BSHARPTREE_H_
#include "Clave.h"
#include "ElementoNodoIntermedio.h"
#include "ElementoNodoHoja.h"
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
	void crear(char* nombreArch,char* nomEspaciosLibres,unsigned int cantElemNodo,ComparadorClave* comp);
	void abrir(char* nombreArch,char* nomEspaciosLibres,ComparadorClave* comp);
	BSharpTree(char* nombreArch,char* nomEspaciosLibres,unsigned int cantElemNodo,int tamanioSerializadoClave,ComparadorClave* comp);
	BSharpTree(char* nombreArch,char* nomEspaciosLibres,ComparadorClave* comp);
	bool Buscar(Clave clave,Referencia referencia);
	Referencia BuscarSecuencial(Clave clave);
	Referencia Siguiente();
	int setReferenciaClaveX(Clave clave);
	int insertar(Referencia ref,Clave* clave);
	int eliminar(Clave* clave);
	int modificar(Clave* clave,Referencia refNueva);
	ElementoNodoIntermedio* ObtenerRaiz();
	/*Falta el listar*/
	virtual ~BSharpTree();
private:
	NodoHoja* buscarPrimerNodoHoja();
	int calcularTamanioSerializado(int tamanioSerializadoClave);
	int bucarIterativo(Nodo nodo, Clave* clave);
	unsigned int utimoNodo,numeroDeElementosXnodo,posicionRaiz,tamanioNodo;
	std::fstream archivoArbol;
	std::fstream archivoEspaciosLibres;
	char* nombreArchivo;
	char* nombreEspaciosLibres;
	ComparadorClave* comparador;
	NodoIntermedio* Raiz;
};

#endif /* BSHARPTREE_H_ */
