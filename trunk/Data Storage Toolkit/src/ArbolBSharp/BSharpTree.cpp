/*
 * BSharpTree.cpp
 *
 *  Created on: 09/10/2009
 *      Author: daniel
 */

#include "BSharpTree.h"
#include <cstdlib>
#include <vector>
#include "TamanioInsuficienteException.h"
using namespace std;

BSharpTree::BSharpTree(Clave* clave){
	claveEstructural = clave->clonarce();
	this->ultimoNodo=NULL;
}

void BSharpTree::crear(string nombreArch,unsigned int tamanioDeBloque, Clave* clave,ComparadorClave* comp) {
	comparador = comp;
	tamanioNodo = tamanioDeBloque;
	numeroDeElementosXnodo = calcularCantidadElementosPorNodo(clave->tamanioSerializado());
	cantidadMinimaDeElementos = (unsigned int) ((numeroDeElementosXnodo)*2)/3;
	nombreArchivo = nombreArch+"_Arbol";
	nombreEspaciosLibres = nombreArch+"_EspaciosLibre";
	archivoEspaciosLibres.open(nombreEspaciosLibres.c_str(),std::fstream::out |std::fstream::binary|std::fstream::trunc);
	archivoArbol.open(nombreArchivo.c_str(),std::fstream::out |std::fstream::binary|std::fstream::trunc);
	archivoEspaciosLibres.close();
	archivoArbol.close();
	archivoEspaciosLibres.open(nombreEspaciosLibres.c_str(),std::fstream::out | std::fstream::in | std::fstream::binary|std::fstream::trunc);
	archivoArbol.open(nombreArchivo.c_str(),std::fstream::out | std::fstream::in |std::fstream::binary|std::fstream::trunc);
	if(archivoArbol.is_open()&&archivoEspaciosLibres.is_open()){
		std:: stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
		archivoArbol.seekp(0);
		int tamanio = sizeof(int)*3;
		char array[tamanio];
		buffer.pubsetbuf(array,tamanio);
		buffer.pubseekpos(0);
		buffer.sputn ((char*)&numeroDeElementosXnodo,sizeof(numeroDeElementosXnodo));
		buffer.sputn ((char*)&tamanioNodo,sizeof(tamanioNodo));
		posicionRaiz = 3*sizeof(unsigned int);
		buffer.sputn ((char*)&posicionRaiz,sizeof(posicionRaiz));
		archivoArbol.write(array,tamanio);
		//Todo cambio la creacion de la raiz a una hoja
		Raiz = new NodoHoja(numeroDeElementosXnodo,0,comparador);
		char array2[tamanioNodo];
		buffer.pubsetbuf(array2,tamanioNodo);
		buffer.pubseekpos(0);
		Raiz->serializate(&buffer);
		archivoArbol.write(array2,tamanioNodo);
		archivoEspaciosLibres.seekp(0);
		int Siguiente = 0;
		archivoEspaciosLibres.write((char*)&Siguiente,sizeof(Siguiente));
	}
}
/*Abre un arbol ya existente*/
bool BSharpTree::abrir(string nombreArch,ComparadorClave* comp){
	comparador = comp;
	nombreArchivo = nombreArch+"_Arbol";
	nombreEspaciosLibres = nombreArch+"_EspaciosLibre";
	archivoEspaciosLibres.open(nombreEspaciosLibres.c_str(),std::fstream::out|std::fstream::in|std::fstream::binary);
	archivoArbol.open(nombreArchivo.c_str(),std::fstream::out|std::fstream::in|std::fstream::binary);
	if(!archivoArbol.is_open()||!archivoEspaciosLibres.is_open()){
		return false;
	}
	archivoArbol.seekg(0);
	std:: stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	int tamanio = sizeof(int)*3;
	char array[tamanio];
	archivoArbol.read(array,tamanio);
	buf.pubsetbuf(array,tamanio);
	buf.pubseekpos(0);
	buf.sgetn((char*)&numeroDeElementosXnodo,sizeof(unsigned int));
	buf.sgetn((char*)&tamanioNodo,sizeof(unsigned int));
	buf.sgetn((char*)&posicionRaiz,sizeof(unsigned int));
	char array2[tamanioNodo];
	archivoArbol.read(array2,tamanioNodo);
	buf.pubsetbuf(array2,tamanioNodo);
	buf.pubseekpos(0);
	int nivel;
	buf.sgetn((char*)&nivel,sizeof(int));
	if(nivel!=0){
		Raiz = new NodoIntermedio(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
		this->imprimirNodo(Raiz);
	}else{
		Raiz = new NodoHoja(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
	}
	cantidadMinimaDeElementos = (unsigned int) ((numeroDeElementosXnodo)*2)/3;
	return true;
}

int BSharpTree::calcularCantidadElementosPorNodo(unsigned int tamSerializadoClave){
	unsigned int resultado = tamanioNodo - 2 * sizeof(int) - sizeof(Referencia);
	int tamElemento = sizeof(Referencia)+tamSerializadoClave;
	resultado = (int)(resultado/tamElemento);
	if (resultado<1){
		throw  TamanioInsuficienteException();
	}else return resultado;
}

bool BSharpTree::Buscar(const Clave* clave,Referencia* referencia){
	Referencia referenciaDeNodoHoja;
	Clave* aux = const_cast<Clave*>(clave);
	if(this->estaVacio())return false;
	if(Raiz->getNumeroNivel()==0){
		NodoHoja* nodoRaiz = dynamic_cast<NodoHoja*>(Raiz);
		return nodoRaiz->buscarReferenciaDeClaveX(clave,referencia);
	}
	NodoHoja* hoja = buscarHoja(Raiz,aux,referenciaDeNodoHoja);
	return hoja->buscarReferenciaDeClaveX(clave,referencia);
}

NodoHoja* BSharpTree::buscarPrimerNodoHoja(Nodo* nodoE){
	Referencia ref;
	int nivel;
	NodoIntermedio* nodo = dynamic_cast<NodoIntermedio*>(nodoE);
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	if(nodo->getReferenciaIzq()!=0){
		ref = nodo->getReferenciaIzq();
	}else{
		ElementoNodo* elemento = nodo->getListaElementos()->front();
		ref = elemento->getReferencia();
	}
	char array2[tamanioNodo];
	buf.pubseekpos(0);
	archivoArbol.seekg(ref);
	archivoArbol.read(array2,tamanioNodo);
	buf.pubsetbuf(array2,tamanioNodo);
	buf.pubseekpos(0);
	buf.sgetn((char*)&nivel,sizeof(int));
	if(nivel!=0){
		NodoIntermedio nodoAux(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
		return buscarPrimerNodoHoja(&nodoAux);
	}else{
		NodoHoja* nodoAux = new NodoHoja(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
		nodoAux->setPos();
		return nodoAux;
	}
	return NULL;
}

NodoHoja* BSharpTree::buscarHoja(Nodo* nodoE,Clave* clave,Referencia& referenciaDeNodoHoja){
	int nivel;
	NodoIntermedio* nodo = dynamic_cast<NodoIntermedio*>(nodoE);
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	Referencia refAux = nodo->bucarReferenciaAsiguienteNodo(clave);
	char array2[tamanioNodo];
	buf.pubseekpos(0);
	archivoArbol.seekg(refAux);
	archivoArbol.read(array2,tamanioNodo);
	buf.pubsetbuf(array2,tamanioNodo);
	buf.pubseekpos(0);
	buf.sgetn((char*)&nivel,sizeof(int));
	if(nivel!=0){
		NodoIntermedio nodoAux(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
		return buscarHoja(&nodoAux,clave,referenciaDeNodoHoja);
	}else{
		NodoHoja* nodoAux = new NodoHoja(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
		delete ultimoNodo;
		ultimoNodo = nodoAux;
		referenciaDeNodoHoja =refAux;
		return nodoAux;
	}
	return NULL;
}

Referencia BSharpTree::Siguiente(bool ultimo){
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	if(!ultimo){
		ultimoNodo = buscarPrimerNodoHoja(Raiz);
	}else{
		ultimoNodo->avanzarPos();
		if(ultimoNodo->numeroPos() == (numeroDeElementosXnodo-ultimoNodo->getEspacioLibre())){
			char array2[tamanioNodo];
			buf.pubseekpos(0);
			archivoArbol.seekg(ultimoNodo->getReferenciaSiguiente());
			archivoArbol.read(array2,tamanioNodo);
			buf.pubsetbuf(array2,tamanioNodo);
			delete ultimoNodo;
			ultimoNodo = new NodoHoja(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
		}
	}
	ElementoNodo* elemento = ultimoNodo->getPos();
	return elemento->getReferencia();
}

bool BSharpTree::modificar(const Clave* clave,Referencia refNueva){
	Referencia referenciaDeNodoHoja;
	Clave* aux = const_cast<Clave*>(clave);
	NodoHoja* hoja = buscarHoja(Raiz,aux,referenciaDeNodoHoja);
	bool modificado = hoja->setReferenciaDeClaveX(clave,refNueva);
	if(modificado){
		std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
		char array2[tamanioNodo];
		buf.pubseekpos(0);
		archivoArbol.seekp(referenciaDeNodoHoja);
		buf.pubsetbuf(array2,tamanioNodo);
		buf.pubseekpos(0);
		hoja->serializate(&buf);
		archivoArbol.write(array2,tamanioNodo);;
	}
	return modificado;
}

bool BSharpTree::insertar(Referencia ref,Clave* clave){
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	ElementoNodo* elemento = new ElementoNodo(ref,clave);

	if(Raiz->getNumeroNivel()==0){
		int retorno = Raiz->agregarElemento(elemento);
		this->grabarUnitario(Raiz,posicionRaiz);
		if(retorno==0){return false;}
		if(retorno==2){
			NodoHoja* hojaIzq = new NodoHoja(numeroDeElementosXnodo,0,comparador);
			NodoHoja* hojaDer = new NodoHoja(numeroDeElementosXnodo,0,comparador);
			NodoIntermedio* nuevaRaiz = new NodoIntermedio((unsigned int)1,numeroDeElementosXnodo,comparador);
			reacomodarArbol(hojaIzq,hojaDer,nuevaRaiz);
			delete hojaIzq;
			delete hojaDer;
		}
		return true;
	}
	Referencia refHoja;
	Nodo* hoja=NULL;
	bool esRaiz=true;
	std::list<Referencia>listaDePadres;
	listaDePadres.push_back(posicionRaiz);
	BuscarInsertarOEliminar(hoja,listaDePadres,Raiz,clave,refHoja,esRaiz,true);
	int desborde = hoja->agregarElemento(elemento);
	if(desborde==0)return false;
	if(desborde==1){
		grabarUnitario(hoja,refHoja);
	}else{
		resolverDesborde(hoja,listaDePadres,refHoja);
		delete Raiz;
		recomponerRaiz();
	}
	listaDePadres.clear();
	delete hoja;
	delete elemento;
	return true;
}

void BSharpTree::reacomodarArbol(NodoHoja*& hojaIzq,NodoHoja*& hojaDer,NodoIntermedio*&nuevaRaiz){
	int cant = (numeroDeElementosXnodo/2)+1;
	std::list<ElementoNodo*>::iterator it = Raiz->getListaElementos()->begin();
	while(it!=Raiz->getListaElementos()->end()){
		if(cant>0){
			hojaIzq->agregarElemento(*it);
			cant--;
		}else{
			hojaDer->agregarElemento(*it);
		}
		++it;
	}
	delete Raiz;
	nuevaRaiz->agregarElemento(hojaDer->getListaElementos()->front());
	archivoArbol.seekp(0,fstream::end);
	Referencia refIzq  = (Referencia)archivoArbol.tellp();
	nuevaRaiz->setRefereciaIzq(refIzq);
	grabarUnitario(hojaIzq,refIzq);
	Referencia refDer  = (Referencia)archivoArbol.tellp();
	nuevaRaiz->getListaElementos()->front()->setReferencia(refDer);
	hojaIzq->setReferenciaSiguiente(refDer);
	grabado(hojaIzq,hojaDer,nuevaRaiz,refIzq,refDer,posicionRaiz);
	Raiz = nuevaRaiz;
}

void BSharpTree::BuscarInsertarOEliminar(Nodo* &hoja,std::list<Referencia>&listaDePadres,Nodo* nodoE,Clave* clave,Referencia& refHoja,bool& esRaiz,bool esInsertar){
	int nivel;
	NodoIntermedio* nodo = dynamic_cast<NodoIntermedio*>(nodoE);
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	Referencia refAux = nodo->bucarReferenciaAsiguienteNodo(clave);
	char array2[tamanioNodo];
	archivoArbol.seekg(refAux);
	archivoArbol.read(array2,tamanioNodo);
	buf.pubsetbuf(array2,tamanioNodo);
	buf.pubseekpos(0);
	buf.sgetn((char*)&nivel,sizeof(int));
	if(!esRaiz){
		delete nodo;
	}else{ esRaiz = false;}
	if(nivel!=0){
		NodoIntermedio* aux = new NodoIntermedio(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
		modificarLista(listaDePadres,esInsertar,aux);
		listaDePadres.push_front(refAux);
		BuscarInsertarOEliminar(hoja,listaDePadres,aux,clave,refHoja,esRaiz,esInsertar);
	}else{
		hoja =  new NodoHoja(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
		refHoja = refAux;
		return;
	}
	return;
}

void BSharpTree::modificarLista(std::list<Referencia>&listaDePadres,bool esInsertar,Nodo* nodo){
	if(esInsertar){
		if(nodo->getEspacioLibre()>0){
			listaDePadres.clear();
		}
	}else{
		if(nodo->getListaElementos()->size()>cantidadMinimaDeElementos){
			listaDePadres.clear();
		}
	}
}

void BSharpTree::grabado(Nodo* original,Nodo* hermano,Nodo* padre,Referencia refOriginal,Referencia refHermano,Referencia refPadre){
	std::stringbuf buff(ios_base :: in | ios_base :: out | ios_base :: binary);
	char array2[tamanioNodo];
	buff.pubsetbuf(array2,tamanioNodo);
	archivoArbol.seekp(refOriginal);
	original->serializate(&buff);
	archivoArbol.write(array2,tamanioNodo);
	buff.pubseekpos(0);
	padre->serializate(&buff);
	archivoArbol.seekp(refPadre);
	archivoArbol.write(array2,tamanioNodo);
	buff.pubseekpos(0);
	hermano->serializate(&buff);
	archivoArbol.seekp(refHermano);
	archivoArbol.write(array2,tamanioNodo);
}

void BSharpTree::grabar(Nodo* nodoOriginal,Nodo* nodoHermano,Referencia refOriginal,Referencia refHermano){
	std::stringbuf buff(ios_base :: in | ios_base :: out | ios_base :: binary);
	char array2[tamanioNodo];
	buff.pubsetbuf(array2,tamanioNodo);
	nodoOriginal->serializate(&buff);
	archivoArbol.seekp(refOriginal);
	archivoArbol.write(array2,tamanioNodo);
	buff.pubseekpos(0);
	nodoHermano->serializate(&buff);
	archivoArbol.seekp(refHermano);
	archivoArbol.write(array2,tamanioNodo);
}

void BSharpTree::grabarUnitario(Nodo* nodo,Referencia ref){
	std::stringbuf buff(ios_base :: in | ios_base :: out | ios_base :: binary);
	char array2[tamanioNodo];
	archivoArbol.seekp(ref);
	buff.pubsetbuf(array2,tamanioNodo);
	buff.pubseekpos(0);
	nodo->serializate(&buff);
	archivoArbol.write(array2,tamanioNodo);
}

void BSharpTree::resolverDesborde(Nodo* nodo,std::list<Referencia>&listaDePadres,Referencia refHijo){
	std::stringbuf buff(ios_base :: in | ios_base :: out | ios_base :: binary);
	bool izq;//indica si el hermano es el Izq
	bool balancear;
	Nodo* hermano=NULL;
	Referencia refHermano;
	/*es el elemento del padre q separa los nodos hermanos q se dividen o valancea*/
	ElementoNodo* elementoPadre=NULL;
	archivoArbol.seekg(listaDePadres.front());
	char array2[tamanioNodo];
	archivoArbol.read(array2,tamanioNodo);
	buff.pubsetbuf(array2,tamanioNodo);
	NodoIntermedio* padre = new NodoIntermedio(&buff,numeroDeElementosXnodo,comparador,claveEstructural);
	/*primero obtengo si se balancea y si es con hermano derecho o izq*/
	buscarNodoBalancearODividir(padre,nodo,hermano,refHijo,izq,balancear,refHermano,elementoPadre);
	if(balancear){
		nodo->balanceo(hermano,padre,izq);
		grabado(nodo,hermano,padre,refHijo,refHermano,listaDePadres.front());
		return;
	}else{
		ElementoNodo* elementoAagregarEnPadre=NULL;
		/*Busco posicion nuevo nodo*/
		Referencia refMedio = buscarEspacioLibre();
		if(refMedio==0){
			archivoArbol.seekp(0,fstream::end);
			refMedio = archivoArbol.tellp();
		}
		Nodo* nodoIzq=NULL;
		Nodo* nodoMedio=NULL;
		Nodo* nodoDer=NULL;
		if(nodo->getNumeroNivel()==0){
			nodoIzq = new NodoHoja(numeroDeElementosXnodo,refMedio,comparador);
			nodoMedio = new NodoHoja(numeroDeElementosXnodo,0,comparador);
			nodoDer = new NodoHoja(numeroDeElementosXnodo,0,comparador);
		}else{
			nodoIzq =  new NodoIntermedio(nodo->getNumeroNivel(),numeroDeElementosXnodo,comparador);
			nodoMedio = new NodoIntermedio(nodo->getNumeroNivel(),numeroDeElementosXnodo,comparador);
			nodoDer =new NodoIntermedio(nodo->getNumeroNivel(),numeroDeElementosXnodo,comparador);
		}
		if(izq){//
			elementoAagregarEnPadre =  hermano->dividirse(nodo,nodoIzq,nodoMedio,nodoDer,padre,elementoPadre->getClave());
			elementoAagregarEnPadre->setReferencia(refMedio);
			grabado(nodoIzq,nodoMedio,nodoDer,refHermano,refMedio,refHijo);
		}else{
			elementoAagregarEnPadre = nodo->dividirse(hermano,nodoIzq,nodoMedio,nodoDer,padre,elementoPadre->getClave());
			elementoAagregarEnPadre->setReferencia(refMedio);
			grabado(nodoIzq,nodoMedio,nodoDer,refHijo,refMedio,refHermano);
		}
		delete nodoIzq;
		delete nodoMedio;
		delete nodoDer;
		int desborde = padre->agregarElemento(elementoAagregarEnPadre);
		grabarUnitario(padre,listaDePadres.front());
		if(desborde==2){
			refHijo = listaDePadres.front();
			listaDePadres.pop_front();
			if(!listaDePadres.empty()){
				resolverDesborde(padre,listaDePadres,refHijo);
			}else{
				desbordeRaiz(padre);
			}
		}
	}
	delete padre;
}

void BSharpTree::buscarNodoBalancearODividir(NodoIntermedio* padre,Nodo* hijo,Nodo*& hermano,Referencia refHijo,bool& izq,bool& balancear,Referencia& refHermano,ElementoNodo*& elemPadre){
	bool encontrado = false;
	if(refHijo == padre->getReferenciaIzq() || padre->getListaElementos()->back()->getReferencia() == refHijo){
		if(refHijo == padre->getReferenciaIzq()){
			elemPadre = padre->getListaElementos()->front();
			izq = false;
			refHermano = elemPadre->getReferencia();
		}else{
			elemPadre = padre->getListaElementos()->back();
			if(padre->getCatidadMaximaDeElementos()-padre->getEspacioLibre()==1){
				refHermano = padre->getReferenciaIzq();
			}else{
				std::list<ElementoNodo*>:: reverse_iterator it = padre->getListaElementos()->rbegin();
				++it;
				ElementoNodo* elem= *it;
				refHermano = elem->getReferencia();
			}
			izq=true;
		}
		obtenerHermano(refHermano,hermano,hijo->getNumeroNivel(),balancear);
		return;
	}else{
		izq = true;
		if(padre->getListaElementos()->front()->getReferencia()==refHijo){
			refHermano = padre->getReferenciaIzq();
		}else{
			std::list<ElementoNodo*>:: reverse_iterator it = padre->getListaElementos()->rbegin();
			ElementoNodo* elem = hijo->getListaElementos()->front();
			while(!encontrado && it!=padre->getListaElementos()->rend()){
				elemPadre = *it;
				if(comparador->Comparar(elemPadre->getClave(),elem->getClave())<0){
					encontrado=true;
					refHermano=elemPadre->getReferencia();
				}
				++it;
			}
		}
		obtenerHermano(refHermano,hermano,hijo->getNumeroNivel(),balancear);
		if(!balancear){
			izq = false;
			delete hermano;
			encontrado = false;
			std::list<ElementoNodo*>:: iterator it = padre->getListaElementos()->begin();
			ElementoNodo* elem = hijo->getListaElementos()->back();
			while(!encontrado){
				elemPadre = *it;
				if(comparador->Comparar(elemPadre->getClave(),elem->getClave())>0){
					encontrado=true;
					refHermano=elemPadre->getReferencia();
				}
				++it;
			}
			obtenerHermano(refHermano,hermano,hijo->getNumeroNivel(),balancear);
		}
	}
}

void BSharpTree::obtenerHermano(Referencia ref,Nodo*& hermano,unsigned int nivel,bool&balancear){
	std::stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
	char array2[tamanioNodo];
	archivoArbol.seekg(ref);
	archivoArbol.read(array2,tamanioNodo);
	buffer.pubsetbuf(array2,tamanioNodo);
	if(nivel==0){
		hermano = new NodoHoja(&buffer,numeroDeElementosXnodo,comparador,claveEstructural);
	}else{ hermano = new NodoIntermedio(&buffer,numeroDeElementosXnodo,comparador,claveEstructural);}
	if(hermano->getEspacioLibre()==0){balancear = false;
	}else{
		balancear=true;
	}
}

void BSharpTree::desbordeRaiz(Nodo* RaizE){
	NodoIntermedio* RaizNueva = dynamic_cast<NodoIntermedio*>(RaizE);
	std::stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
	int cantIzq = (numeroDeElementosXnodo*2)/3;
	ElementoNodo* elementoRaiz=NULL;
	NodoIntermedio* HijoIzq = new NodoIntermedio(RaizNueva->getNumeroNivel(),numeroDeElementosXnodo,comparador);
	NodoIntermedio* HijoDer = new NodoIntermedio(RaizNueva->getNumeroNivel(),numeroDeElementosXnodo,comparador);
	std::list<ElementoNodo*>::iterator it = RaizNueva->getListaElementos()->begin();
	while(cantIzq>0&&it!=RaizNueva->getListaElementos()->end()){
		ElementoNodo* elem = *it;
		HijoIzq->agregarElemento(elem);
		cantIzq=cantIzq-1;
		++it;
	}
	elementoRaiz = *it;
	++it;
	while(it != RaizNueva->getListaElementos()->end()){
		ElementoNodo* elem2 = *it;
		HijoDer->agregarElemento(elem2);
		++it;
	}
	HijoIzq->setRefereciaIzq(RaizNueva->getReferenciaIzq());
	HijoDer->setRefereciaIzq(elementoRaiz->getReferencia());
	archivoArbol.seekp(0,fstream::end);
	RaizNueva->setRefereciaIzq((Referencia)archivoArbol.tellp());
	grabarUnitario(HijoIzq,archivoArbol.tellp());
	archivoArbol.seekp(0,fstream::end);
	elementoRaiz->setReferencia((Referencia)archivoArbol.tellp());
	grabarUnitario(HijoDer,archivoArbol.tellp());
	RaizNueva->getListaElementos()->clear();
	RaizNueva->agregarElemento(elementoRaiz);
	RaizNueva->setNumeroNivel(RaizNueva->getNumeroNivel()+1);
	RaizNueva->setEspacioLibre(RaizNueva->getCatidadMaximaDeElementos()-1);
	grabarUnitario(RaizNueva,posicionRaiz);
	delete HijoIzq;
	delete HijoDer;
}

bool BSharpTree::eliminar(const Clave* claveE){
	Clave* clave = const_cast<Clave*>(claveE);
	if(Raiz->getNumeroNivel()==0){
		NodoHoja* RaizCast = dynamic_cast<NodoHoja*>(Raiz);
		return RaizCast->Eliminar(clave);
	}
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	Referencia refHoja;
	Nodo* hoja=NULL;
	bool esRaiz=true;
	std::list<Referencia>listaDePadres;
	listaDePadres.push_back(posicionRaiz);
	BuscarInsertarOEliminar(hoja,listaDePadres,Raiz,clave,refHoja,esRaiz,false);
	int subFlujo = hoja->Eliminar(clave);
	if(hoja->getCatidadMaximaDeElementos()==2){
		eliminarHoja(hoja,listaDePadres,refHoja);
	}else{
		eliminarClaveEnIntermedio(clave,hoja->getListaElementos()->front()->getClave());
		if(subFlujo==0){return false;}
		if(subFlujo==1){
			grabarUnitario(hoja,refHoja);
		}else{
			resolverSubflujo(hoja,listaDePadres,refHoja);
		}
	}
	recomponerRaiz();
	listaDePadres.clear();
	return true;
}

/*El vector boleano contendra la informacion necesaria sobre los hermanos q se obtuvieron indicando:
 *  posicion 0: si se puede balancear con hermano izquierdo.
 *  posicion 1: si se puede balancear con hermano derecho.
 *  posicion 2: si se une sin caso especial 3, 4 o 5.
 *  posicion 3: si nodo extremo izquierdo. tratara de balancear de forma distinta y sino puede unir
 *  posicion 4: si nodo extremo derecho.
 *  posicion 5: si son hijos de la Raiz y solo hay dos hijos
 */
void BSharpTree::resolverSubflujo(Nodo* nodo,std::list<Referencia>&listaDePadres,Referencia refHijo){
	std::stringbuf buff(ios_base :: in | ios_base :: out | ios_base :: binary);
	vector<bool> booleanoInformacion (5,false);
	Nodo* hermanoIzq=NULL;
	Nodo* hermanoDer=NULL;
	Referencia refHermanoIzq;
	Referencia refHermanoDer;
	if(refHijo == posicionRaiz){return;}//La raiz tiene permitido estar en subflujo, es mas tener solo un elemento
	archivoArbol.seekg(listaDePadres.front());//listaDePadre.front() tiene la ref al padre
	char array2[tamanioNodo];
	archivoArbol.read(array2,tamanioNodo);
	buff.pubsetbuf(array2,tamanioNodo);
	NodoIntermedio* padre = new NodoIntermedio(&buff,numeroDeElementosXnodo,comparador,claveEstructural);
	buscarHermanos(nodo,padre,hermanoIzq,hermanoDer,refHermanoIzq,refHermanoDer,refHijo,booleanoInformacion);
	if(booleanoInformacion[0]){
		hermanoIzq->balanceo(nodo,padre,false);
		hermanoIzq->setEspacioLibre(hermanoIzq->getEspacioLibre()+1);
		grabado(nodo,hermanoIzq,padre,refHijo,refHermanoIzq,listaDePadres.front());
		delete hermanoIzq;
		delete padre;
	}else if(booleanoInformacion[1]){
		hermanoDer->balanceo(nodo,padre,true);
		hermanoDer->setEspacioLibre(hermanoDer->getEspacioLibre()+1);
		grabado(nodo,hermanoDer,padre,refHijo,refHermanoDer,listaDePadres.front());
		destruirNodos(nodo,hermanoDer,hermanoIzq);
		delete padre;
	}else if(booleanoInformacion[2]){
		int subflujo = nodo->unirse(hermanoIzq,hermanoDer,padre);
		resolverReferenciaSiguiente(hermanoIzq,refHermanoDer);
		grabar(hermanoIzq,hermanoDer,refHermanoIzq,refHermanoDer);
		nuevoEspacioLibre(refHijo);
		grabarUnitario(padre,listaDePadres.front());
		destruirNodos(nodo,hermanoDer,hermanoIzq);
		if(subflujo==2){
			refHijo = listaDePadres.front();
			listaDePadres.pop_front();
			resolverSubflujo(padre,listaDePadres,refHijo);
		}
	}else if(booleanoInformacion[3]&&!booleanoInformacion[5]){
		if((numeroDeElementosXnodo - hermanoIzq->getEspacioLibre())>cantidadMinimaDeElementos){
			hermanoIzq->balanceo(nodo,padre,true);
			hermanoIzq->setEspacioLibre(hermanoIzq->getEspacioLibre()+1);
			grabado(nodo,hermanoIzq,padre,refHijo,refHermanoIzq,listaDePadres.front());
			destruirNodos(nodo,hermanoDer,hermanoIzq);
			delete padre;
		}else if((numeroDeElementosXnodo-hermanoDer->getEspacioLibre())>cantidadMinimaDeElementos){
			nodo->balanceoEspecial(hermanoIzq,hermanoDer,padre,false);
			grabado(nodo,hermanoIzq,hermanoDer,refHijo,refHermanoIzq,refHermanoDer);
			grabarUnitario(padre,listaDePadres.front());
			destruirNodos(nodo,hermanoDer,hermanoIzq);
			delete padre;
		}else{
			int subflujo = hermanoIzq->unirse(nodo,hermanoDer,padre);
			resolverReferenciaSiguiente(nodo,refHermanoDer);
			grabar(nodo,hermanoDer,refHijo,refHermanoDer);
			nuevoEspacioLibre(refHermanoIzq);
			grabarUnitario(padre,listaDePadres.front());
			destruirNodos(nodo,hermanoDer,hermanoIzq);
			if(subflujo==2){
				refHijo = listaDePadres.front();
				listaDePadres.pop_front();
				resolverSubflujo(padre,listaDePadres,refHijo);}
		}
	}else if(booleanoInformacion[4]&& !booleanoInformacion[5]){
		if((numeroDeElementosXnodo - hermanoIzq->getEspacioLibre())>cantidadMinimaDeElementos){
			hermanoIzq->balanceo(nodo,padre,false);
			hermanoIzq->setEspacioLibre(hermanoIzq->getEspacioLibre()+1);
			grabado(nodo,hermanoIzq,padre,refHijo,refHermanoIzq,listaDePadres.front());
			destruirNodos(nodo,hermanoDer,hermanoIzq);
			delete padre;
		}else if((numeroDeElementosXnodo - hermanoDer->getEspacioLibre())>cantidadMinimaDeElementos){
			nodo->balanceoEspecial(hermanoIzq,hermanoDer,padre,true);
			grabado(nodo,hermanoIzq,hermanoDer,refHijo,refHermanoIzq,refHermanoDer);
			grabarUnitario(padre,listaDePadres.front());
			destruirNodos(nodo,hermanoDer,hermanoIzq);
			delete padre;
		}else{
			int subflujo = hermanoIzq->unirse(hermanoDer,nodo,padre);
			resolverReferenciaSiguiente(hermanoDer,refHijo);
			grabar(nodo,hermanoDer,refHijo,refHermanoDer);
			nuevoEspacioLibre(refHermanoIzq);
			grabarUnitario(padre,listaDePadres.front());
			destruirNodos(nodo,hermanoDer,hermanoIzq);
			if(subflujo==2){
				refHijo = listaDePadres.front();
				listaDePadres.pop_front();
				resolverSubflujo(padre,listaDePadres,refHijo);}
		}
	}else{
		subflujoHijosRaiz(nodo,hermanoIzq,padre,refHijo,refHermanoIzq);
	}
}

void BSharpTree::destruirNodos(Nodo* nodo,Nodo* hermanoDer,Nodo* hermanoIzq){
	delete nodo;
	delete hermanoDer;
	delete hermanoIzq;
}

void BSharpTree::buscarHermanos(Nodo* nodoActual,NodoIntermedio* padre,Nodo* &hermanoIzq,Nodo* &hermanoDer,Referencia& refHermanoIzq,Referencia& refHermanoDer,Referencia refHijo,vector<bool> &informacion){
	bool extremo = false;
	NodoIntermedio* RaizCast = dynamic_cast<NodoIntermedio*>(Raiz);
	if(refHijo==padre->getReferenciaIzq()){
		informacion[3]=true;
	}
	if(refHijo == padre->getListaElementos()->back()->getReferencia()){
		informacion[4]=true;
		extremo = true;
	}
	if((informacion[3]||informacion[4])&&(Raiz->getListaElementos()->size()!=1||(Raiz->getListaElementos()->size()==1 &&refHijo!=RaizCast->getReferenciaIzq()&&refHijo!=Raiz->getListaElementos()->back()->getReferencia()))){
		refHermanoIzq = obtenerReferenciaHermano(padre,nodoActual->getListaElementos()->front()->getClave(),extremo);
		hermanoIzq =  obtenerHermanoXsuBflujo(nodoActual->getNumeroNivel(),refHermanoIzq);
		refHermanoDer = obtenerReferenciaHermano(padre,hermanoIzq->getListaElementos()->front()->getClave(),extremo);
		hermanoDer = obtenerHermanoXsuBflujo(nodoActual->getNumeroNivel(),refHermanoDer);
		return;
	}
	if((RaizCast->getListaElementos()->size()==1)&& ((RaizCast->getReferenciaIzq()==refHijo)||(RaizCast->getListaElementos()->front()->getReferencia()==refHijo))){
		informacion[5]=true;
		if((RaizCast->getReferenciaIzq()==refHijo)){refHermanoIzq = padre->getListaElementos()->front()->getReferencia();}
		else{refHermanoIzq = padre->getReferenciaIzq();}
		hermanoIzq = obtenerHermanoXsuBflujo(nodoActual->getNumeroNivel(),refHermanoIzq);
	}
	if(!informacion[3]&&!informacion[4]&&!informacion[5]){
		refHermanoIzq = obtenerReferenciaHermano(padre,nodoActual->getListaElementos()->front()->getClave(),true);
		hermanoIzq = obtenerHermanoXsuBflujo(nodoActual->getNumeroNivel(),refHermanoIzq);
		if((numeroDeElementosXnodo-hermanoIzq->getEspacioLibre()) > cantidadMinimaDeElementos){
			informacion[0]=true;
			return;
		}else{
			refHermanoDer = obtenerReferenciaHermano(padre,nodoActual->getListaElementos()->front()->getClave(),false);
			hermanoDer = obtenerHermanoXsuBflujo(nodoActual->getNumeroNivel(),refHermanoDer);
			if((numeroDeElementosXnodo-hermanoDer->getEspacioLibre()) > cantidadMinimaDeElementos){
				informacion[1]=true;
				return;
			}else{
				informacion[2]=true;
				return;
			}
		}
	}
}

Referencia BSharpTree::obtenerReferenciaHermano(Nodo* padreE,Clave* clave,bool Izq){
	NodoIntermedio* padre = dynamic_cast<NodoIntermedio*>(padreE);
	ElementoNodo* elemPadre=NULL;
	std::list<ElementoNodo*>::iterator it = padre->getListaElementos()->begin();
	++it;
	elemPadre = *it;
	if(Izq && (comparador->Comparar(clave,elemPadre->getClave())<0)){
		return padre->getReferenciaIzq();
	}
	if(!Izq && (comparador->Comparar(clave,padre->getListaElementos()->front()->getClave())<0)){
		return padre->getListaElementos()->front()->getReferencia();
	}
	std::list<ElementoNodo*>::reverse_iterator itPadre = padre->getListaElementos()->rbegin();
	while(itPadre != padre->getListaElementos()->rend()){
		elemPadre = *itPadre;
		if((comparador->Comparar(elemPadre->getClave(),clave)==0)||(comparador->Comparar(elemPadre->getClave(),clave)<0)){
			if(Izq){
				++itPadre;
			}else{ --itPadre;}
			elemPadre = *itPadre;
			return elemPadre->getReferencia();
		}
		++itPadre;
	}
	return NULL;
}

void BSharpTree::eliminarHoja (Nodo* nodo,std::list<Referencia>&listaDePadres,Referencia refHijo){

	grabarUnitario(nodo,refHijo);
	if(nodo->getEspacioLibre()==2){
		this->nuevoEspacioLibre(refHijo);
	}
	Nodo* padreE = this->obtenerNodoPorPosiciones(listaDePadres.front());
	NodoIntermedio* padre = dynamic_cast<NodoIntermedio*>(padreE);
	bool vacio = true;
	Nodo* nodoAux = this->obtenerNodoPorPosiciones(padre->getReferenciaIzq());
	if(nodoAux->getEspacioLibre()!=2){
		vacio=false;
	}
	delete nodoAux;
	std::list<ElementoNodo*>::iterator itPadre = padre->getListaElementos()->begin();
	while(itPadre != padre->getListaElementos()->end()&&vacio){
		ElementoNodo* elemPadre = *itPadre;
		nodoAux = this->obtenerNodoPorPosiciones(elemPadre->getReferencia());
		if(nodoAux->getCatidadMaximaDeElementos()!=nodoAux->getEspacioLibre()){
			vacio=false;
		}
		delete nodoAux;
		++itPadre;
	}
	if(vacio){
		while(!padre->getListaElementos()->empty()){
			padre->getListaElementos()->pop_front();
		}
		this->nuevoEspacioLibre(refHijo);
	}
	/*if(nodo->getEspacioLibre()<2){
		grabarUnitario(nodo,refHijo);
	}else{
		archivoArbol.seekg(listaDePadres.front());
		char array2[tamanioNodo];
		archivoArbol.read(array2,tamanioNodo);
		buff.pubsetbuf(array2,tamanioNodo);
		NodoIntermedio* padre = new NodoIntermedio(&buff,numeroDeElementosXnodo,comparador,claveEstructural);
		//Padre dos elementos
		if(padre->getEspacioLibre()==0){

		}
	}*/
}

Nodo* BSharpTree::obtenerHermanoXsuBflujo(int nivel,Referencia ref){
	archivoArbol.seekg(ref);
	std::stringbuf buff(ios_base :: in | ios_base :: out | ios_base :: binary);
	buff.pubseekpos(0);
	char array2[tamanioNodo];
	archivoArbol.seekg(ref);
	archivoArbol.read(array2,tamanioNodo);
	buff.pubsetbuf(array2,tamanioNodo);
	if(nivel==0){
		Nodo* nodo = new NodoHoja(&buff,numeroDeElementosXnodo,comparador,claveEstructural);
		return nodo;
	}else{
		Nodo* nodo = new NodoIntermedio(&buff,numeroDeElementosXnodo,comparador,claveEstructural);
		return nodo;
	}
}

void BSharpTree::resolverReferenciaSiguiente(Nodo* nodoIzq,Referencia refAHermanoNuevo){
	if(nodoIzq->getNumeroNivel()==0){
		NodoHoja* nodoIzquierdo = dynamic_cast<NodoHoja*>(nodoIzq);
		nodoIzquierdo->setReferenciaSiguiente(refAHermanoNuevo);
	}
}

void BSharpTree::eliminarClaveEnIntermedio(Clave* claveAeliminar,Clave* claveSetear){
	Referencia refNodoModificado = posicionRaiz;
	NodoIntermedio* nodoConClave = buscarIntermedio(claveAeliminar,Raiz,true,refNodoModificado);
	if(nodoConClave==NULL){return;}
	nodoConClave->setearClave(claveAeliminar,claveSetear);
	grabarUnitario(nodoConClave,refNodoModificado);
}

NodoIntermedio* BSharpTree::buscarIntermedio(Clave* clave,Nodo* nodoE,bool esRaiz,Referencia& refAux){
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	NodoIntermedio* nodo = dynamic_cast<NodoIntermedio*>(nodoE);
	if(nodo->estaClave(clave)){return nodo;
	}else{
		refAux = nodo->bucarReferenciaAsiguienteNodo(clave);
		if(!esRaiz){
			delete nodo;}else{esRaiz = false;}
		buf.pubseekpos(0);
		archivoArbol.seekg(refAux);
		char array2[tamanioNodo];
		archivoArbol.read(array2,tamanioNodo);
		buf.pubsetbuf(array2,tamanioNodo);
		buf.pubseekpos(0);
		int nivel;
		buf.sgetn((char*)&nivel,sizeof(int));
		if(nivel==0){return NULL;}
		else{
			nodo = new NodoIntermedio(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
			return buscarIntermedio(clave,nodo,esRaiz,refAux);
		}
	}
	return NULL;
}
/*
 * Si se llama a este metodo es pq la Raiz solo tiene dos hijos (deben ser intermedio), como concecuecia un solo elemento,
 *  y uno de sus hijos esta en subflujo sin poder balancearce. en caso de q el subflujo
 * sea justo igual al minimo menos 1 no se hace nada, peo si es inferior se genera entre la raiz y los hijos
 * un solo nodo q pasa a ser la nueva raiz.
 */
void BSharpTree::subflujoHijosRaiz(Nodo* nodo,Nodo* hermano,Nodo* padre,Referencia refNodo,Referencia refHermano){
	if((hermano->getListaElementos()->size()>cantidadMinimaDeElementos)){//se puede balancear
		if(comparador->Comparar(nodo->getListaElementos()->back()->getClave(),hermano->getListaElementos()->front()->getClave())<0){//hermano esta a la derecha de nodo
			hermano->balanceo(nodo,padre,true);
		}else{//hermano esta a la izquierda de nodo
			hermano->balanceo(nodo,padre,false);
		}
		hermano->setEspacioLibre(hermano->getEspacioLibre()+1);
		grabado(nodo,hermano,padre,refNodo,refHermano,posicionRaiz);
		delete nodo;
		delete hermano;
	}else if(nodo->getNumeroNivel()==0){//debe unirce pasando la Raiz a una hoja
		if(comparador->Comparar(nodo->getListaElementos()->back()->getClave(),hermano->getListaElementos()->front()->getClave())<0){
			armarRaizHoja(nodo,hermano,refNodo,refHermano);
		}else{
			armarRaizHoja(hermano,nodo,refNodo,refHermano);
		}
	}else{//Debe unirce siendo que la Raiz sigue siendo nodo Intermedio
		if(comparador->Comparar(nodo->getListaElementos()->back()->getClave(),hermano->getListaElementos()->front()->getClave())<0){//Hermano esta a derecha
			armarRaizIntermedia(nodo,hermano,padre,refNodo,refHermano);
		}else{
			armarRaizIntermedia(hermano,nodo,padre,refHermano,refNodo);
		}
	}
}

void BSharpTree::armarRaizIntermedia(Nodo* nodoIzq,Nodo* nodoDer,Nodo* padre,Referencia refNodoIzq,Referencia refNodoDer){
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	NodoIntermedio* nodoIzquierdo = dynamic_cast<NodoIntermedio*>(nodoIzq);
	NodoIntermedio* nodoDerecho = dynamic_cast<NodoIntermedio*>(nodoDer);
	NodoIntermedio* RaizCast = dynamic_cast<NodoIntermedio*>(padre);
	RaizCast->getListaElementos()->front()->setReferencia(nodoDerecho->getReferenciaIzq());
	RaizCast->setRefereciaIzq(nodoIzquierdo->getReferenciaIzq());
	for(std::list<ElementoNodo*>::reverse_iterator it = nodoIzquierdo->getListaElementos()->rbegin();it != nodoIzquierdo->getListaElementos()->rend();++it){
		RaizCast->agregarElemento(*it);
	}
	for(std::list<ElementoNodo*>::iterator it = nodoDerecho->getListaElementos()->begin();it != nodoDerecho->getListaElementos()->end();++it){
		RaizCast->agregarElemento(*it);
	}
	RaizCast->setNumeroNivel(RaizCast->getNumeroNivel()-1);
	grabarUnitario(RaizCast,posicionRaiz);
	nuevoEspacioLibre(refNodoIzq);
	nuevoEspacioLibre(refNodoDer);
	delete nodoIzq;
	delete nodoDer;
}

void BSharpTree::armarRaizHoja(Nodo* nodoIzq,Nodo* nodoDer,Referencia refIzq,Referencia refDer){
	for(std::list<ElementoNodo*>::iterator it = nodoDer->getListaElementos()->begin();it != nodoDer->getListaElementos()->end();++it){
		nodoIzq->agregarElemento(*it);
	}
	delete Raiz;
	NodoHoja* nodoI = dynamic_cast<NodoHoja*>(nodoIzq);
	nodoI->setReferenciaSiguiente(0);
	Raiz = nodoIzq;
	grabarUnitario(nodoIzq,posicionRaiz);
	delete nodoDer;
	nuevoEspacioLibre(refIzq);
	nuevoEspacioLibre(refDer);
}

void BSharpTree::recomponerRaiz(){
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	char arrayRaiz[tamanioNodo];
	archivoArbol.seekg(posicionRaiz);
	archivoArbol.read(arrayRaiz,tamanioNodo);
	buf.pubsetbuf(arrayRaiz,tamanioNodo);
	buf.pubseekpos(0);
	int nivel;
	buf.sgetn((char*)&nivel,sizeof(int));
	if(nivel==0){
		Raiz = new NodoHoja(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
	}else{
		Raiz = new NodoIntermedio(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
	}
}

void BSharpTree::nuevoEspacioLibre(Referencia ref){
	archivoEspaciosLibres.seekg(0);
	int cant;
	archivoEspaciosLibres.read((char*)&cant,sizeof(int));
	archivoEspaciosLibres.seekp(sizeof(cant)+sizeof(Referencia)*cant);
	archivoEspaciosLibres.write((char*)&ref,sizeof(Referencia));
	cant++;
	archivoEspaciosLibres.seekg(0);
	archivoEspaciosLibres.write((char*)&cant,sizeof(int));
}

Referencia BSharpTree::buscarEspacioLibre(){
	int Siguiente;
	Referencia retorno;
	archivoEspaciosLibres.seekg(0);
	archivoEspaciosLibres.read((char*)&Siguiente,sizeof(Siguiente));
	if(Siguiente==0){
		return 0;
	}else{
		archivoEspaciosLibres.seekg(sizeof(Siguiente)+(Siguiente-1)*sizeof(Referencia));
		archivoEspaciosLibres.read((char*)&retorno,sizeof(Referencia));
		Siguiente--;
		archivoEspaciosLibres.seekp(0);
		archivoEspaciosLibres.write((char*)&Siguiente,sizeof(Siguiente));
	}
	return retorno;
}

Nodo* BSharpTree::obtenerNodoPorPosiciones(Referencia posInicial){
	std::stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);

	buffer.pubseekpos(0);
	Nodo* nodo;
	archivoArbol.seekg(posInicial);
	char array2[tamanioNodo];
	archivoArbol.read(array2,tamanioNodo);
	buffer.pubsetbuf(array2,tamanioNodo);
	int nivel;
	buffer.sgetn((char*)&nivel,sizeof(int));
	if(nivel==0){
		nodo = new NodoHoja(&buffer,numeroDeElementosXnodo,comparador,claveEstructural);
	}else{
		nodo = new NodoIntermedio(&buffer,numeroDeElementosXnodo,comparador,claveEstructural);
	}
	return nodo;
}

void BSharpTree::imprimir(){
	cout<<"Tamanio de los nodos: "<< tamanioNodo <<endl;
	cout<<"posicion de la raiz: "<<posicionRaiz<<endl;
	imprimirIterativo(Raiz);
}

void BSharpTree::imprimirIterativo(Nodo* nodoE){

	if(nodoE->getEspacioLibre()!=nodoE->getCatidadMaximaDeElementos()){
		cout<<" "<<endl;
		if(nodoE->getNumeroNivel()!=0){
			NodoIntermedio* nodo = dynamic_cast<NodoIntermedio*>(nodoE);
			cout <<"Nodo Intermedio ";
			cout <<"Nivel: " << nodo->getNumeroNivel()<<" ";
			cout <<"Referencia Izquierda: " << nodo->getReferenciaIzq()<<endl;
			cout <<"Elementos: ";
			std::list<ElementoNodo*>::iterator it = nodo->getListaElementos()->begin();
			while(it!=nodo->getListaElementos()->end()){
				ElementoNodo* elem = *it;
				for(unsigned int i = 0; i<elem->getClave()->getCantidadAtributos();i++){
					cout<<" ";
					elem->getClave()->getAtributo(i)->imprimir(cout);
					cout<<" ";
				}
				cout << elem->getReferencia();
				cout<<"   ";
				++it;
			}
			cout<<" "<<endl;
			Nodo* nodoSiguiente = obtenerNodoPorPosiciones(nodo->getReferenciaIzq());
			imprimirIterativo(nodoSiguiente);
			delete nodoSiguiente;
			it = nodo->getListaElementos()->begin();
			while(it!=nodo->getListaElementos()->end()){
				ElementoNodo* elem = *it;
				nodoSiguiente = obtenerNodoPorPosiciones(elem->getReferencia());
				imprimirIterativo(nodoSiguiente);
				delete nodoSiguiente;
				++it;
			}
		}else{
			NodoHoja* nodo = dynamic_cast<NodoHoja*>(nodoE);
			cout <<"Nodo Hoja ";
			cout <<"Nivel: " << nodo->getNumeroNivel()<<" ";
			cout <<"Referencia Siguente: " << nodo->getReferenciaSiguiente()<<endl;;
			cout <<"Elementos: ";
			std::list<ElementoNodo*>::iterator it = nodo->getListaElementos()->begin();
			while(it!=nodo->getListaElementos()->end()){
				ElementoNodo* elem = *it;
				for(unsigned int i = 0; i<elem->getClave()->getCantidadAtributos();i++){
					cout<<" ";
					elem->getClave()->getAtributo(i)->imprimir(cout);
					cout<<" ";
				}
				cout << elem->getReferencia();
				cout<<"   ";
				++it;
			}
			cout<<" "<<endl;
		}
	}
}


void BSharpTree::posicionarArchivo(){
	archivoArbol.seekg(posicionRaiz);
	archivoArbol.seekp(posicionRaiz);
}

bool BSharpTree::siguienteAlmacenado(Nodo*& nodo){
	std::stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
	if(archivoArbol.peek()==EOF){
		archivoArbol.clear();// saca el flag de eof
		return false;
	}
	//cout<<archivoArbol.tellg()<<endl;//todo sacar
	char array2[tamanioNodo];
	archivoArbol.read(array2,tamanioNodo);
	buffer.pubsetbuf(array2,tamanioNodo);
	int nivel;
	buffer.sgetn((char*)&nivel,sizeof(int));
	if(nivel==0){
		nodo =new  NodoHoja(&buffer,numeroDeElementosXnodo,comparador,claveEstructural);
		//this->imprimirNodo(nodo);//todo sacar
	}else{
		nodo = new NodoIntermedio(&buffer,numeroDeElementosXnodo,comparador,claveEstructural);
		//this->imprimirNodo(nodo);//todo sacar
	}
	return true;
}

void BSharpTree::escribir(std::stringbuf& buffer){
	/*archivoArbol.write(buffer.str().data(),tamanioNodo);
	delete Raiz;
	this->recomponerRaiz();*/
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	buffer.pubseekpos(0,ios::out|ios::binary|ios::in);
	int nivel;
	Nodo* nodo;
	buffer.sgetn((char*)&nivel,sizeof(int));
	if(nivel==0){
		nodo =new  NodoHoja(&buffer,numeroDeElementosXnodo,comparador,claveEstructural);
		//this->imprimirNodo(nodo);//todo sacar
	}else{
		nodo = new NodoIntermedio(&buffer,numeroDeElementosXnodo,comparador,claveEstructural);
		//this->imprimirNodo(nodo);//todo sacar
	}
	//cout<<archivoArbol.tellp()<<endl;//todo sacar
	char array[tamanioNodo];
	buf.pubsetbuf(array,tamanioNodo);
	buf.pubseekpos(0);
	nodo->serializate(&buf);
	archivoArbol.write(array,tamanioNodo);
	delete nodo;
}

void BSharpTree::observarNodo(std::stringbuf* buffer){
	int nivel;
	Nodo* nodo;

	buffer->sgetn((char*)&nivel,sizeof(int));
	if(nivel==0){
		nodo =new  NodoHoja(buffer,numeroDeElementosXnodo,comparador,claveEstructural);
	}else{
		nodo = new NodoIntermedio(buffer,numeroDeElementosXnodo,comparador,claveEstructural);
	}
	this->imprimirNodo(nodo);
	delete nodo;
}

std::string BSharpTree::getMetadata(){
	std::string resultado;
	char*p=(char*)&numeroDeElementosXnodo;
	for(unsigned i=0;i<sizeof(numeroDeElementosXnodo);i++){
		resultado.push_back(*p);
		p++;
	}
	p=(char*)&tamanioNodo;
	for(unsigned i=0;i<sizeof(tamanioNodo);i++){
		resultado.push_back(*p);
		p++;
	}
	p=(char*)&posicionRaiz;
	for(unsigned i=0;i<sizeof(posicionRaiz);i++){
		resultado.push_back(*p);
		p++;
	}
	return resultado;
};

void BSharpTree::setMetadata(char* metadata){
	numeroDeElementosXnodo=*(unsigned int *)metadata;
	metadata+=sizeof(unsigned int);
	tamanioNodo=*(unsigned int*)metadata;
	metadata+=sizeof(unsigned int);
	posicionRaiz=*(unsigned int*)metadata;
	cantidadMinimaDeElementos = (unsigned int) ((numeroDeElementosXnodo)*2)/3;
	archivoArbol.seekp(0);
	archivoArbol.write((char*)&numeroDeElementosXnodo,sizeof(numeroDeElementosXnodo));
	archivoArbol.write((char*)&tamanioNodo,sizeof(tamanioNodo));
	archivoArbol.write((char*)&posicionRaiz,sizeof(posicionRaiz));
}

void BSharpTree::imprimirNodo(Nodo* nodoE){
	cout<<" "<<endl;
	if(nodoE->getNumeroNivel()!=0){
		NodoIntermedio* nodo = dynamic_cast<NodoIntermedio*>(nodoE);
		cout <<"Nodo Intermedio ";
		cout <<"Nivel: " << nodo->getNumeroNivel()<<" ";
		cout <<"Referencia Izquierda: " << nodo->getReferenciaIzq()<<endl;
		cout <<"Elementos: ";
		std::list<ElementoNodo*>::iterator it = nodo->getListaElementos()->begin();
		while(it!=nodo->getListaElementos()->end()){
			ElementoNodo* elem = *it;
			for(unsigned int i = 0; i<elem->getClave()->getCantidadAtributos();i++){
				cout<<" ";
				elem->getClave()->getAtributo(i)->imprimir(cout);
				cout<<" ";
			}
			cout << elem->getReferencia();
			cout<<"   ";
			++it;
		}
		cout<<" "<<endl;
	}else{
		NodoHoja* nodo = dynamic_cast<NodoHoja*>(nodoE);
		cout <<"Nodo Hoja ";
		cout <<"Nivel: " << nodo->getNumeroNivel()<<" ";
		cout <<"Referencia Siguente: " << nodo->getReferenciaSiguiente()<<endl;;
		cout <<"Elementos: ";
		std::list<ElementoNodo*>::iterator it = nodo->getListaElementos()->begin();
		while(it!=nodo->getListaElementos()->end()){
			ElementoNodo* elem = *it;
			for(unsigned int i = 0; i<elem->getClave()->getCantidadAtributos();i++){
				cout<<" ";
				elem->getClave()->getAtributo(i)->imprimir(cout);
				cout<<" ";
			}
			cout << elem->getReferencia();
			cout<<"   ";
			++it;
		}
		cout<<" "<<endl;
	}
}

bool BSharpTree::abrir(string nombreArch,ComparadorClave* comp,bool comprimido){
	nombreArchivo = nombreArch+"_Arbol";
	nombreEspaciosLibres = nombreArch+"_EspaciosLibre";
	comparador = comp;
	archivoArbol.open(nombreArchivo.c_str(),std::fstream::out |std::fstream::binary|std::fstream::trunc);
	if(!archivoArbol.is_open()){
		return false;
	}
	archivoArbol.close();
	archivoArbol.open(nombreArchivo.c_str(),std::fstream::out | std::fstream::in |std::fstream::binary|std::fstream::trunc);
	if(!archivoArbol.is_open()){
		return false;
	}
	archivoEspaciosLibres.open(nombreEspaciosLibres.c_str(),std::fstream::out|std::fstream::in|std::fstream::binary);
	if(!archivoEspaciosLibres.is_open()){
		return false;
	}
	return true;
}
std::string BSharpTree::getNombreArchivo(){
	return this->nombreArchivo;
}
bool BSharpTree::estaVacio(){
	if(Raiz->getListaElementos()->empty())
		return true;
	return false;
}

int BSharpTree::tamanioBloque(){
	return tamanioNodo;
}
void BSharpTree::cerrar(){
	grabarUnitario(Raiz,posicionRaiz);
	archivoArbol.close();
	archivoEspaciosLibres.close();
}
BSharpTree::~BSharpTree() {
	grabarUnitario(Raiz,posicionRaiz);
	delete Raiz;
	archivoArbol.close();
	archivoEspaciosLibres.close();
}

