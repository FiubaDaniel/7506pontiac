/*
 * NodoIntermedio.cpp
 *
 *  Created on: 30/09/2009
 *      Author: daniel
 */

#include "NodoIntermedio.h"

NodoIntermedio::NodoIntermedio(unsigned int nivelNodo,unsigned int cantElem,ComparadorClave* comp): NodoIntermedio ::Nodo(cantElem,nivelNodo,comp){
	referenciaIzq = 0;
}
NodoIntermedio::NodoIntermedio(std::stringbuf* buf,unsigned int cantElem,ComparadorClave* comp,Clave* claveEstructural): NodoIntermedio ::Nodo(cantElem,0,comp){
	buf->pubseekpos(0);
	buf->sgetn((char*)&numeroNivel,sizeof(numeroNivel));
	buf->sgetn((char*)&cantidadDeElementosLibre,sizeof(cantidadDeElementosLibre));
	int Aux = cantElem - cantidadDeElementosLibre;
	buf->sgetn((char*)&referenciaIzq,sizeof(Referencia));
	Referencia refD;
	while(Aux>0){
		claveEstructural->deserializar(*buf);
		buf->sgetn((char*)&refD,sizeof(Referencia));
		ElementoNodo* elemento = new ElementoNodo(refD,claveEstructural);
		listaElementos.push_back(elemento);
		Aux--;
	}
}
Referencia NodoIntermedio::getReferenciaIzq(){
	return referenciaIzq;
}
void NodoIntermedio::setRefereciaIzq(Referencia ref){
	referenciaIzq = ref;
}
void NodoIntermedio::serializate(std::stringbuf* buffer){
	buffer->pubseekpos(0);
	buffer->sputn ((char*)&numeroNivel,sizeof(numeroNivel));
	buffer->sputn ((char*)&cantidadDeElementosLibre,sizeof(cantidadDeElementosLibre));
	buffer->sputn ((char*)&referenciaIzq,sizeof(referenciaIzq));
	std::list<ElementoNodo*>::iterator it = listaElementos.begin();
	if(!listaElementos.empty()){
		while(it != listaElementos.end()){
			ElementoNodo* elemento = *it;
			Clave * clave = elemento->getClave();
			clave->serializar(*buffer);
			Referencia refD = elemento->getReferencia();
			buffer->sputn ((char*)&refD,sizeof(Referencia));
			++it;
		}
	}
}
/*
 * Devuelve 1 si agrego el elemento o devuelve 0 si ya existia
 * En caso de estar lleno devuelve 2, indicando q el nodo ya tiene todos
 * sus elementos, como concecuencia debe reorganizarce colocando un elemento
 * en un nodo hermano o producir la generacion de otro nodo. el elemento se agregara
 * en la posicion correcta enviando el mensaje de arreglar esa situacion.
 * El elemento viene con sus referencia derecha seteada.
 */
int NodoIntermedio::agregarElemento(ElementoNodo* elemento){
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
				cantidadDeElementosLibre = cantidadDeElementosLibre - 1;
			}
			ubicado=true;
		}else{
			++it;
		}
	}
	if(!ubicado){
		listaElementos.push_back(elemento->clonarce());
		if(retorno==1){
			cantidadDeElementosLibre = cantidadDeElementosLibre - 1;
		}
	}
	return retorno;
}
/*
 * Izq determina si el hermano con el q se valancea esta a la izq o esta a la derecha.
 * Primero se busca el elemento del padre q será modificado, que será el elemento mas grande
 * de los elementos mas chicos al primer elemento del nodo derecho.
 * Luego se obtiene el cambio de referencias y por ultimo se modifican los elementos.
 */
void NodoIntermedio::balanceo(Nodo* nodoHermanoE,Nodo* nodoPadreE, bool izq){
	NodoIntermedio* nodoHermano = dynamic_cast<NodoIntermedio*>(nodoHermanoE);
	NodoIntermedio* nodoPadre= dynamic_cast<NodoIntermedio*>(nodoPadreE);
	ElementoNodo* elementoPadre;
	ElementoNodo* referenciaDeBusqueda;
	bool encontrado;
	if(izq){
		encontrado = false;
		referenciaDeBusqueda = listaElementos.front();
		std::list<ElementoNodo*>::reverse_iterator itReversa = nodoPadre->getListaElementos()->rbegin();
		while(!encontrado && itReversa != nodoPadre->getListaElementos()->rend()){
			elementoPadre = *itReversa;
			if(comparador->Comparar(referenciaDeBusqueda->getClave(),elementoPadre->getClave())<0){
				++itReversa;
			}else{
				encontrado = true;
			}
		}
		Referencia refIzqNodoDer;
		refIzqNodoDer = referenciaDeBusqueda->getReferencia();
		Clave* claveAux = referenciaDeBusqueda->getClave();
		referenciaDeBusqueda->setClave(elementoPadre->getClave());//para elemento q cambia de nodo, y la clave será la del elemento del padre
		referenciaDeBusqueda->setReferencia(referenciaIzq);
		nodoHermano->getListaElementos()->push_back(referenciaDeBusqueda);
		nodoHermano->setEspacioLibre(nodoHermano->getEspacioLibre()-1);
		elementoPadre->setClave(claveAux);
		referenciaIzq = refIzqNodoDer;
		listaElementos.pop_front();
	}else{
		encontrado = false;
		referenciaDeBusqueda = listaElementos.back();
		std::list<ElementoNodo*>::iterator it = nodoPadre->getListaElementos()->begin();
		while(!encontrado && it != nodoPadre->getListaElementos()->end()){
			elementoPadre = *it;
			if(comparador->Comparar(referenciaDeBusqueda->getClave(),elementoPadre->getClave())>0){
				++it;
			}else{
				encontrado = true;
			}
		}
		Referencia refIzqNodoDer;
		refIzqNodoDer = referenciaDeBusqueda->getReferencia();
		Clave* clave = referenciaDeBusqueda->getClave();
		referenciaDeBusqueda->setClave(elementoPadre->getClave());
		referenciaDeBusqueda->setReferencia(nodoHermano->getReferenciaIzq());
		nodoHermano->getListaElementos()->push_front(referenciaDeBusqueda);
		nodoHermano->setEspacioLibre(nodoHermano->getEspacioLibre()-1);
		elementoPadre->setClave(clave);
		nodoHermano->setRefereciaIzq(refIzqNodoDer);
		listaElementos.pop_back();
	}
}
/*
 * Izq debe determinar que los hermanos estan a izq
 */
void NodoIntermedio::balanceoEspecial(Nodo* nodoPegadoE,Nodo* nodoAlejadoE,Nodo* padre,bool Izq){
	NodoIntermedio* nodoPegado = dynamic_cast<NodoIntermedio*>(nodoPegadoE);
	NodoIntermedio* nodoAlejado = dynamic_cast<NodoIntermedio*>(nodoAlejadoE);
	if(Izq){
		std::list<ElementoNodo*>::reverse_iterator itPadre = padre->getListaElementos()->rbegin();
		ElementoNodo* elemPadre = *itPadre;
		balanceoEspecialHermanosIzq(nodoPegado,elemPadre,&listaElementos,referenciaIzq);
		++itPadre;
		elemPadre = *itPadre;
		Referencia refIzq=nodoPegado->getReferenciaIzq();
		balanceoEspecialHermanosIzq(nodoAlejado,elemPadre,nodoPegado->getListaElementos(),refIzq);
		nodoPegado->setRefereciaIzq(refIzq);
		nodoAlejado->setEspacioLibre(nodoAlejado->getEspacioLibre()+1);
		cantidadDeElementosLibre = cantidadDeElementosLibre-1;
	}else{
		std::list<ElementoNodo*>::iterator itPadre = padre->getListaElementos()->begin();
		ElementoNodo* elemPadre = *itPadre;
		balanceoEspecialHermanosDer(nodoPegado,elemPadre,&listaElementos);
		++itPadre;
		elemPadre = *itPadre;
		Referencia refIzq=nodoPegado->getReferenciaIzq();
		balanceoEspecialHermanosDer(nodoAlejado,elemPadre,nodoPegado->getListaElementos());
		nodoPegado->setRefereciaIzq(refIzq);
		nodoAlejado->setEspacioLibre(nodoAlejado->getEspacioLibre()+1);
		cantidadDeElementosLibre = cantidadDeElementosLibre-1;
	}
}
void NodoIntermedio::balanceoEspecialHermanosDer(NodoIntermedio* nodoQcede,ElementoNodo* elemPadre,std::list<ElementoNodo*> *listaQrecibe){
	Referencia refElemHijo = nodoQcede->getListaElementos()->front()->getReferencia();
	Clave* claveElemHijo = nodoQcede->getListaElementos()->front()->getClave();
	listaQrecibe->push_back(nodoQcede->getListaElementos()->front());
	listaQrecibe->back()->setClave(elemPadre->getClave());
	listaQrecibe->back()->setReferencia(nodoQcede->getReferenciaIzq());
	nodoQcede->getListaElementos()->pop_front();
	nodoQcede->setRefereciaIzq(refElemHijo);
	elemPadre->setClave(claveElemHijo);
}
void NodoIntermedio::balanceoEspecialHermanosIzq(NodoIntermedio* nodoQcede,ElementoNodo* elemPadre,std::list<ElementoNodo*> *listaQrecibe,Referencia& refIzq){
	Referencia refElemHijo = nodoQcede->getListaElementos()->back()->getReferencia();
	Clave* claveElemHijo = nodoQcede->getListaElementos()->back()->getClave();
	listaQrecibe->push_front(nodoQcede->getListaElementos()->back());
	listaQrecibe->front()->setClave(elemPadre->getClave());
	listaQrecibe->front()->setReferencia(refIzq);
	nodoQcede->getListaElementos()->pop_back();
	refIzq = refElemHijo;
	elemPadre->setClave(claveElemHijo);
}
/*
 * Dos nodos uno sobrebordado y el otro completo, entonces se dividen en nodos en tres nodos nuevos,
 * dos de ellos se almacenaran el la posicion de los nodos existentes y otro en una nueva posicion
 * como consecuencia el elemento agregado tendra su referencia derecha nula hasta q fuera se le encuentre un lugar.
 * Clave indica el elemento del padre q separa ambos nodos.
 * retorna el elemento q debe agregarce al padre
 */
/*
 * PreCondicion: el nodo this q llame a la funcion es el nodo Izquierdo, o sea el de claves mas chicas.
 */
ElementoNodo* NodoIntermedio::dividirse(Nodo* nodoHermanoE,Nodo* nodoIzqE,Nodo* nodoMedioE,Nodo* nodoDerE,Nodo* nodoPadreE,Clave* clave){
	NodoIntermedio* nodoHermano = dynamic_cast<NodoIntermedio*>(nodoHermanoE);
	NodoIntermedio* nodoIzq = dynamic_cast<NodoIntermedio*>(nodoIzqE);
	NodoIntermedio* nodoMedio = dynamic_cast<NodoIntermedio*>(nodoMedioE);
	NodoIntermedio* nodoDer = dynamic_cast<NodoIntermedio*>(nodoDerE);
	NodoIntermedio* nodoPadre= dynamic_cast<NodoIntermedio*>(nodoPadreE);
	int cantElementosIzq = ((cantidadMaximaDeElementos*2)+1)/3;
	int cantElementosMedio = ((cantidadMaximaDeElementos*2)+1)/3;
	ElementoNodo* elem;
	ElementoNodo* retorno;
	std::list<ElementoNodo*>::iterator itPadre = nodoPadre->getListaElementos()->begin();
	bool encontrado = false;
	while(!encontrado && itPadre!= nodoPadre->getListaElementos()->end()){
		elem = *itPadre;
		if(comparador->Comparar(elem->getClave(),clave)==0){
			encontrado=true;
		}
		++itPadre;
	}
	nodoIzq->setRefereciaIzq(referenciaIzq);
	std::list<ElementoNodo*>::iterator itThis = listaElementos.begin();
	std::list<ElementoNodo*>::iterator itHermano = nodoHermano->getListaElementos()->begin();
	while(itThis != listaElementos.end ()){
		ElementoNodo* elemento = *itThis;
		if(cantElementosIzq==0) {
			nodoMedio->setRefereciaIzq(elemento->getReferencia());
			elemento->setReferencia(0);
			retorno = elemento;
			cantElementosIzq = -1;
		}else{
			if(cantElementosIzq>0){
				nodoIzq->agregarElemento(elemento);
				cantElementosIzq--;
			}else{
				nodoMedio->agregarElemento(elemento);
				cantElementosMedio--;
			}
		}
		++itThis;
	}
	cantElementosMedio--;
	while(itHermano != nodoHermano->getListaElementos()->end()){
		ElementoNodo* elemento2 = *itHermano;
		if(cantElementosMedio==0){
			nodoDer->setRefereciaIzq(elemento2->getReferencia());
			Clave* claveAux = elem->getClave();
			elem->setClave(elemento2->getClave());
			elemento2->setClave(claveAux);
			elemento2->setReferencia(nodoHermano->getReferenciaIzq());
			nodoMedio->agregarElemento(elemento2);
			cantElementosMedio=-1;
		}else{
			if(cantElementosMedio>0){
				nodoMedio->agregarElemento(elemento2);
				cantElementosMedio--;
			}else{
				nodoDer->agregarElemento(elemento2);
			}
		}
		++itHermano;
	}
	listaElementos.clear();
	nodoHermano->getListaElementos()->clear();
	return retorno;
}

/*
 * Segun el orden establecido, al ser un nodo intermedio una clave puede ser igual
 * a la buscada, sin embargo la informacion solo se encuentra en Nodos Hoja, con lo cual
 * hay q segir la busqueda, por lo q la referencia entregada es la ref derecha de ese elemento.
 */
Referencia NodoIntermedio::bucarReferenciaAsiguienteNodo(Clave* clave){
	ElementoNodo* elemento;
	if (comparador->Comparar(clave,listaElementos.front()->getClave())<0){
		return referenciaIzq;
	}
	std::list<ElementoNodo*>::reverse_iterator it = listaElementos.rbegin();
	bool encontrado = false;
	while(!encontrado && it != listaElementos.rend ()){
		elemento = *it;
		if(comparador->Comparar(elemento->getClave(),clave)==0 || comparador->Comparar(elemento->getClave(),clave)<0){
			return elemento->getReferencia();
		}else{
			++it;
		}
	}
	return 0;
}
int NodoIntermedio::unirse(Nodo* nodoHermanoIzq,Nodo* nodoHermanoDer,Nodo* Padre){
	std::list<ElementoNodo*>::reverse_iterator it = Padre->getListaElementos()->rbegin();
	ElementoNodo* elementoPadre;
	ElementoNodo* auxiliarPadre;
	bool encontrado = false;
	while(!encontrado && it!=Padre->getListaElementos()->rend()){
		elementoPadre = *it;
		if(comparador->Comparar(elementoPadre->getClave(),nodoHermanoDer->getListaElementos()->front()->getClave())<0){
			encontrado = true;
		}
		++it;
		if(encontrado){
			auxiliarPadre = *it;
			bool esEste = false;
			std::list<ElementoNodo*>::iterator itAux=  Padre->getListaElementos()->begin();
			while(!esEste){
				ElementoNodo* elem = *itAux;
				if(comparador->Comparar(elem->getClave(),auxiliarPadre->getClave())==0){
					esEste = true;
					Padre->getListaElementos()->erase(itAux);
					Padre->setEspacioLibre(Padre->getEspacioLibre()+1);
				}
				++itAux;
			}
		}
	}
	Clave* claveDeElementoPadre = elementoPadre->getClave();
	NodoIntermedio* nodoDerecho = dynamic_cast<NodoIntermedio*>(nodoHermanoDer);
	auxiliarPadre->setReferencia(referenciaIzq);
	nodoHermanoIzq->agregarElemento(auxiliarPadre);
	Referencia refIzqDeNodoDer  = nodoDerecho->getReferenciaIzq();
    int cantIzq = cantidadMaximaDeElementos- (cantidadMaximaDeElementos - nodoHermanoIzq->getEspacioLibre()) - 1;
	std::list<ElementoNodo*>::iterator itMedio = listaElementos.begin();
	while(itMedio != listaElementos.end()){
		ElementoNodo* elem = *itMedio;
		if(cantIzq>0 ){
			nodoHermanoIzq->agregarElemento(elem);
		}else{
			if(cantIzq == 0){
				nodoDerecho->setRefereciaIzq(elem->getReferencia());
				elementoPadre->setClave(elem->getClave());
				elem->setReferencia(refIzqDeNodoDer);
				elem->setClave(claveDeElementoPadre);
				nodoHermanoDer->agregarElemento(elem);
			}else{nodoHermanoDer->agregarElemento(elem);}
		}
		++itMedio;
		cantIzq--;
	}
	listaElementos.clear();
	if(Padre->getListaElementos()->size() < ((cantidadMaximaDeElementos*2)/3)){
		return 2;
	}
	return 1;
}
void NodoIntermedio::setearClave(const Clave* claveAeliminar,Clave* claveSetear){
	std::list<ElementoNodo*>::iterator it = listaElementos.begin();
	bool encontrado = false;
	while(!encontrado){
		ElementoNodo* elemento = *it;
		if(comparador->Comparar(claveAeliminar,elemento->getClave())==0){
			elemento->setClave(claveSetear);
			encontrado = true;
		}
		++it;
	}
}
bool NodoIntermedio::estaClave(const Clave* clave){
	std::list<ElementoNodo*>::iterator it = listaElementos.begin();
	while(it != listaElementos.end()){
		ElementoNodo* elemento = *it;
		if(comparador->Comparar(clave,elemento->getClave())==0){
			return true;
		}
		++it;
	}
	return false;
}
void NodoIntermedio::limpiar(){
	std::list<ElementoNodo*>::iterator it;
	for(it = listaElementos.begin();it != listaElementos.end();++it){
		ElementoNodo* elemento = *it;
		delete elemento;
	}
}
NodoIntermedio::~NodoIntermedio() {}





