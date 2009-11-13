/*
 * NodoHoja.cpp
 *
 *  Created on: 30/09/2009
 *      Author: daniel
 */
#include "NodoHoja.h"
NodoHoja::NodoHoja(unsigned int cantElem,Referencia refSiguiente,ComparadorClave* comparador) : NodoHoja ::Nodo(cantElem,0,comparador){
	referenciaSiguiente = refSiguiente;
	pos = 0;
}

NodoHoja::NodoHoja(std::stringbuf* buf,unsigned int cantElem,ComparadorClave* comp,Clave* claveEstructural) : NodoHoja ::Nodo(cantElem,0,comp){
	pos = 0;
	buf->pubseekpos(0);
	buf->sgetn((char*)&numeroNivel,sizeof(numeroNivel));
	buf->sgetn((char*)&cantidadDeElementosLibre,sizeof(cantidadDeElementosLibre));
	buf->sgetn((char*)&referenciaSiguiente,sizeof(referenciaSiguiente));
	int Aux = cantElem - cantidadDeElementosLibre;
	Referencia ref;
	while(Aux>0){
		claveEstructural->deserializar(*buf);
		buf->sgetn((char*)&ref,sizeof(Referencia));
		ElementoNodo* elemento = new ElementoNodo(ref,claveEstructural);
		listaElementos.push_back(elemento);
		Aux--;
	}
}

Referencia NodoHoja::getReferenciaSiguiente(){
	return referenciaSiguiente;
}

void NodoHoja::setReferenciaSiguiente(Referencia ref){
	referenciaSiguiente = ref;
}

void NodoHoja::setPos(){
	pos=0;
}

void NodoHoja::avanzarPos(){
	pos++;
}

ElementoNodo* NodoHoja::getPos(){
	std::list<ElementoNodo*>::iterator it = listaElementos.begin();
	unsigned int aux=0;
	while(aux < pos){
		++it;
		aux++;
	}
	return *it;
}

unsigned int NodoHoja::numeroPos(){
	return pos;
}

void NodoHoja::serializate(stringbuf* buffer){
	buffer->pubseekpos(0);
	buffer->sputn ((char*)&numeroNivel,sizeof(numeroNivel));
	buffer->sputn ((char*)&cantidadDeElementosLibre,sizeof(cantidadDeElementosLibre));
	buffer->sputn ((char*)&referenciaSiguiente,sizeof(referenciaSiguiente));
	std::list<ElementoNodo*>::iterator it;
	it = listaElementos.begin();
	if(!listaElementos.empty()){
		while(it != listaElementos.end()){
			ElementoNodo* elemento = *it;
			Clave * clave = elemento->getClave();
			clave->serializar(*buffer);
			Referencia ref = elemento->getReferencia();
			buffer->sputn ((char*)&ref,sizeof(ref));
			++it;
		}
	}
}

int NodoHoja::agregarElemento(ElementoNodo* elemento){

	int retorno = 1;
	if(listaElementos.size() == cantidadMaximaDeElementos){
		retorno = 2;
	}
	if(listaElementos.empty()){
		listaElementos.push_back(elemento->clonarce());
		cantidadDeElementosLibre = cantidadDeElementosLibre - 1;
		return 1;
	}
	std::list<ElementoNodo*>::iterator it = listaElementos.begin();
	bool ubicado = false;
	while(!ubicado && it != listaElementos.end()){
		ElementoNodo* elementoAux = *it;
		if(comparador->Comparar(elemento->getClave(),elementoAux->getClave())==0){
			retorno = 0,
					ubicado=true;
		}else if(comparador->Comparar(elemento->getClave(),elementoAux->getClave())<0){
			listaElementos.insert(it,elemento->clonarce());
			if(retorno==1){
				cantidadDeElementosLibre=cantidadDeElementosLibre-1;
			}
			ubicado=true;
		}
		++it;
	}
	if(!ubicado){
		listaElementos.push_back(elemento->clonarce());
		if(retorno==1){
			cantidadDeElementosLibre=cantidadDeElementosLibre-1;
		}
	}
	return retorno;
}

void NodoHoja::balanceo(Nodo* nodoHermanoE,Nodo* nodoPadreE,bool izq){
	NodoHoja* nodoHermano = dynamic_cast<NodoHoja*>(nodoHermanoE);
	NodoIntermedio* nodoPadre= dynamic_cast<NodoIntermedio*>(nodoPadreE);
	bool encontrado = false;
	ElementoNodo* elem;
	if(izq){
		ElementoNodo * elemento  = listaElementos.front();
		std::list<ElementoNodo*>:: iterator itAux = nodoPadre->getListaElementos()->begin();
		while(!encontrado && itAux != nodoPadre->getListaElementos()->end()){
			elem = *itAux;
			if(comparador->Comparar(elemento->getClave(),elem->getClave())==0){
				encontrado = true;
			}else{
				++itAux;
			}
		}
		nodoHermano->getListaElementos()->push_back(elemento);
		nodoHermano->setEspacioLibre(nodoHermano->getEspacioLibre()-1);
		listaElementos.pop_front();
		elem->setClave(listaElementos.front()->getClave()->clonarce());
	}else{
		ElementoNodo * elemento  = listaElementos.back();
		std::list<ElementoNodo*>:: iterator itAux = nodoPadre->getListaElementos()->begin();
		while(!encontrado && itAux != nodoPadre->getListaElementos()->end()){
			elem = *itAux;
			if(comparador->Comparar(elem->getClave(),elemento->getClave())>0){
				encontrado = true;
			}else{
				++itAux;
			}
		}
		nodoHermano->getListaElementos()->push_front(elemento);
		nodoHermano->setEspacioLibre(nodoHermano->getEspacioLibre()-1);
		elem->setClave(elemento->getClave());
		listaElementos.pop_back();
	}
}

void NodoHoja::balanceoEspecial(Nodo* nodoPegado,Nodo* nodoAlejado,Nodo* padre,bool Izq){
	if(Izq){
		/*
		 * Los hermanos estan a izq siendo que el nodo this esta en subflujo, el nodo mas proximo tiene
		 * la minima cantidad de elementos y el mas alejado tiene al menos un elemento mas del minimo.
		 */
		std::list<ElementoNodo*>::reverse_iterator itPadre = padre->getListaElementos()->rbegin();
		ElementoNodo* elemento = *itPadre;
		listaElementos.push_front(nodoPegado->getListaElementos()->back());
		elemento->setClave(nodoPegado->getListaElementos()->back()->getClave()->clonarce());
		nodoPegado->getListaElementos()->pop_back();
		++itPadre;
		elemento = *itPadre;
		nodoPegado->getListaElementos()->push_front(nodoAlejado->getListaElementos()->back());
		elemento->setClave(nodoAlejado->getListaElementos()->back()->getClave()->clonarce());
		nodoAlejado->getListaElementos()->pop_back();
		cantidadDeElementosLibre = cantidadDeElementosLibre-1;
		nodoAlejado->setEspacioLibre(nodoAlejado->getEspacioLibre()+1);
	}else{
		/*
		 * Los hermanos estan a derecha siendo q el nodo this esta en subflujo, el nodo mas proximo tiene
		 * la minima cantidad de elementos y el mas alejado tiene al menos un elemento mas del minimo.
		 */
		std::list<ElementoNodo*>::iterator itPadre = padre->getListaElementos()->begin();
		++itPadre;
		ElementoNodo* elemento = *itPadre;
		nodoPegado->getListaElementos()->push_back(nodoAlejado->getListaElementos()->front());
		nodoAlejado->getListaElementos()->pop_front();
		elemento->setClave(nodoAlejado->getListaElementos()->front()->getClave()->clonarce());
		--itPadre;
		elemento = *itPadre;
		listaElementos.push_back(nodoPegado->getListaElementos()->front());
		nodoPegado->getListaElementos()->pop_front();
		elemento->setClave(nodoPegado->getListaElementos()->front()->getClave()->clonarce());
		cantidadDeElementosLibre = cantidadDeElementosLibre-1;
		nodoAlejado->setEspacioLibre(nodoAlejado->getEspacioLibre()+1);
	}
}

ElementoNodo* NodoHoja::dividirse(Nodo* nodoHermanoE,Nodo* nodoIzqE,Nodo* nodoMedioE,Nodo* nodoDerE,Nodo* nodoPadreE,Clave* clave){
	NodoHoja* nodoHermano = dynamic_cast<NodoHoja*>(nodoHermanoE);
	NodoHoja* nodoIzq = dynamic_cast<NodoHoja*>(nodoIzqE);
	NodoHoja* nodoMedio = dynamic_cast<NodoHoja*>(nodoMedioE);
	NodoHoja* nodoDer = dynamic_cast<NodoHoja*>(nodoDerE);
	NodoIntermedio* nodoPadre= dynamic_cast<NodoIntermedio*>(nodoPadreE);
	/*Seteo las referecnias siguienters q pueda, exepto la de nodo izq pq la de medio es indefinido*/
	nodoMedio->setReferenciaSiguiente(referenciaSiguiente);
	nodoDer->setReferenciaSiguiente(nodoHermano->getReferenciaSiguiente());
	int cantIzq = ((cantidadMaximaDeElementos*2)+1)/3;
	int cantMedio = ((cantidadMaximaDeElementos*2)+1)/3;
	ElementoNodo* elementoApadre,*elementoPadre;
	bool encontrado = false;
	/*busco en padre la clave, será el elemento seteado*/
	list<ElementoNodo*>::iterator itPadre = nodoPadre->getListaElementos()->begin();
	while(!encontrado && itPadre!=nodoPadre->getListaElementos()->end()){
		elementoPadre = *itPadre;
		if(comparador->Comparar(elementoPadre->getClave(),clave)==0){
			encontrado=true;
		}
		++itPadre;
	}
	list<ElementoNodo*>::iterator it = listaElementos.begin();
	list<ElementoNodo*>::iterator it2 = nodoHermano->getListaElementos()->begin();
	/*Cargo hijo izquierdo con 2/3 del padre*/
	while(it != listaElementos.end ()){
		ElementoNodo* elemento = *it;
		if(cantIzq==0) {
			elementoApadre = new ElementoNodo(0,elemento->getClave());
			nodoMedio->agregarElemento(elemento);
			cantMedio--;
			cantIzq=-1;
		}else{
			if(cantIzq>0){
				nodoIzq->agregarElemento(elemento);
				cantIzq--;
			}else{
				nodoMedio->agregarElemento(elemento);
				cantMedio--;
			}
		}
		it++;
	}
	while(it2 != nodoHermano->getListaElementos()->end()){
		ElementoNodo* elemento = *it2;
		if(cantMedio==0){
			delete elementoPadre->getClave();
			elementoPadre->setClave(elemento->getClave());
			nodoDer->agregarElemento(elemento);
			cantMedio=-1;
		}else{
			if(cantMedio>0){
				nodoMedio->agregarElemento(elemento);
				cantMedio--;
			}else{
				nodoDer->agregarElemento(elemento);
			}
		}
		it2++;
	}
	listaElementos.clear();
	nodoHermano->getListaElementos()->clear();
	return elementoApadre;
}

bool NodoHoja::buscar(const Clave* clave,ElementoNodo*&elemento){
	std::list<ElementoNodo*>::iterator it = listaElementos.begin();
	bool encontrado = false;
	bool existe = true;
	pos = 0;
	while(!encontrado && it!=listaElementos.end() && existe){
		elemento = *it;
		int comparacion = comparador->Comparar(elemento->getClave(),clave);
		if(comparacion>0){
			existe=false;
		}else if(comparacion==0){
			encontrado=true;
		}else{
			++it;
			pos++;
		}
	}
	if(!encontrado || !existe)return false;
	return true;;
}

bool NodoHoja::buscarReferenciaDeClaveX(const Clave* clave,Referencia* ref){
	ElementoNodo * elemento;
	bool encontro= buscar(clave,elemento);
	*ref = elemento->getReferencia();
	return encontro;
}

bool NodoHoja::setReferenciaDeClaveX(const Clave* clave,Referencia refNueva){
	ElementoNodo* elemento;
	bool encontro = buscar(clave,elemento);
	elemento->setReferencia(refNueva);
	return encontro;
}

int NodoHoja::busquedaSecuencial(const Clave* clave,ElementoNodo* &elemento){
	std::list<ElementoNodo*>::iterator it = listaElementos.begin();
	bool encontrado = false;
	int retorno  = -1;
	while(it != listaElementos.end()&& !encontrado){
		elemento = *it;
		if(comparador->Comparar(elemento->getClave(),clave)==0){
			encontrado = true;
			retorno = 0;
		}
		if(comparador->Comparar(elemento->getClave(),clave)>0){
			encontrado = true;
			retorno = 1;
		}
		++it;
	}
	return retorno;
}

int NodoHoja::unirse(Nodo* nodoHermanoIzq,Nodo* nodoHermanoDer,Nodo* Padre){
	std::list<ElementoNodo*>::reverse_iterator it = Padre->getListaElementos()->rbegin();
	ElementoNodo* elementoPadre;
	Clave* clave;
	bool encontrado = false;
	while(!encontrado && it!=Padre->getListaElementos()->rend()){
		elementoPadre = *it;
		if(comparador->Comparar(elementoPadre->getClave(),nodoHermanoDer->getListaElementos()->front()->getClave())<0){
			encontrado = true;
			/*elemento del padre q tengo q eliminar*/
			clave = elementoPadre->getClave();
		}

		if(encontrado){
			--it;
			elementoPadre = *it;
		}
		++it;
	}
	unsigned int cantIzq = cantidadMaximaDeElementos- (cantidadMaximaDeElementos - nodoHermanoIzq->getEspacioLibre()) - 1;
	std::list<ElementoNodo*>::iterator itMedio = listaElementos.begin();
	while(itMedio != listaElementos.end()){
		ElementoNodo* elem = *itMedio;
		if(cantIzq>0){
			nodoHermanoIzq->agregarElemento(elem);
			cantIzq--;
		}else {
			nodoHermanoDer->agregarElemento(elem);
		}
		++itMedio;
	}
	this->getListaElementos()->clear();
	elementoPadre->setClave(nodoHermanoDer->getListaElementos()->front()->getClave());
	return Padre->Eliminar(clave);
}
NodoHoja::~NodoHoja() {

}
