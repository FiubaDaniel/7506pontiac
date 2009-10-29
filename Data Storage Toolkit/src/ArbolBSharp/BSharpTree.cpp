/*
 * BSharpTree.cpp
 *
 *  Created on: 09/10/2009
 *      Author: daniel
 */

#include "BSharpTree.h"
using namespace std;

BSharpTree::BSharpTree(char* nombreArch,char* nomEspaciosLibres,unsigned int cantElemNodo, int tamanioSerializadoClave,ComparadorClave* comp) {
	comparador = comp;
	tamanioNodo = BSharpTree::calcularTamanioSerializado(tamanioSerializadoClave);
	numeroDeElementosXnodo = cantElemNodo;
	strcpy(nomEspaciosLibres,nombreEspaciosLibres);
	archivoEspaciosLibres.open(nombreEspaciosLibres,std::fstream::out);
	strcpy(nombreArch, nombreArchivo);
	archivoArbol.open(nombreArchivo,std::fstream::out);
	if(archivoArbol.is_open()&&archivoEspaciosLibres.is_open()){
		archivoArbol.seekp(0);
		std:: stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
		buffer.pubseekpos(0);
		numeroDeElementosXnodo = cantElemNodo;
		buffer.sputn ((char*)&cantElemNodo,sizeof(cantElemNodo));
		utimoNodo = 0;
		buffer.sputn ((char*)&utimoNodo,sizeof(utimoNodo));
		buffer.sputn ((char*)&tamanioNodo,sizeof(tamanioNodo));
		posicionRaiz = 4*sizeof(int);
		buffer.sputn ((char*)&posicionRaiz,sizeof(posicionRaiz));
		archivoArbol.write((char*)&buffer,4*sizeof(int));
		/*Establezco la raiz*/ //chequear lo de tamaño intermedio
		Raiz = new NodoIntermedio(0,0,numeroDeElementosXnodo,comparador);
		Raiz->serializate(&buffer);
		archivoArbol.write((char*)&buffer,tamanioNodo);
		archivoEspaciosLibres.seekp(0);
		archivoEspaciosLibres.write(0,sizeof(int));
	}
	archivoArbol.close();
	archivoEspaciosLibres.close();
};
BSharpTree::BSharpTree(char* nombreArch,char* nomEspaciosLibres,ComparadorClave* comp){
	comparador = comp;
	strcpy(nomEspaciosLibres,nombreEspaciosLibres);
	strcpy(nombreArch, nombreArchivo);
	archivoEspaciosLibres.open(nombreEspaciosLibres,std::fstream::in);
	archivoArbol.open(nombreArchivo,std::fstream::in);
	archivoArbol.seekg(0);
	std:: stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	buf.pubseekpos(0);
	archivoArbol.read((char*)&buf,4*sizeof(int));
	buf.pubseekpos(0);
	buf.sgetn((char*)&numeroDeElementosXnodo,sizeof(unsigned int));
	buf.sgetn((char*)&utimoNodo,sizeof(int));
	buf.sgetn((char*)&tamanioNodo,sizeof(int));
	buf.sgetn((char*)&posicionRaiz,sizeof(int));
	buf.pubseekpos(0);
	archivoArbol.read((char*)&buf,tamanioNodo);
	Raiz = new NodoIntermedio(&buf,numeroDeElementosXnodo,comparador);
    archivoArbol.close();
    archivoEspaciosLibres.close();
};
int BSharpTree::calcularTamanioSerializado(int tamanioSerializadoClave){
	return tamanioSerializadoClave + 3*sizeof(unsigned int) + 2*sizeof(Referencia);
};
/*
 * Devuelve 0 en caso de no existir la clave buscada.
 */
bool BSharpTree::Buscar(Clave clave,Referencia referencia){
	archivoArbol.open(nombreArchivo,std::fstream::in);
	int ref = bucarIterativo(*Raiz, clave);
	archivoArbol.close();
	if(ref<0)return false;
	else{
		referencia = (unsigned int)ref;
		return true;
	}
};
int BSharpTree::bucarIterativo(Nodo nodo, Clave clave){
	int  noExiste = -1;
	if(nodo.getNumeroNivel()!=0){
		NodoIntermedio& nodoAux = (NodoIntermedio&) nodo;
		Referencia refAux = nodoAux.bucarReferenciaAsiguienteNodo(clave);
		nodoAux.getNumeroDeNodo();
		refAux+=0;
	}
	return noExiste;
};
BSharpTree::~BSharpTree() {

};

