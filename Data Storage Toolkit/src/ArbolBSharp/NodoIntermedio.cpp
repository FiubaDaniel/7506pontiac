/*
 * NodoIntermedio.cpp
 *
 *  Created on: 30/09/2009
 *      Author: daniel
 */

#include "NodoIntermedio.h"

NodoIntermedio::NodoIntermedio(unsigned int nivelNodo,unsigned int cantElem,ComparadorClave* comp): NodoIntermedio ::Nodo(cantElem,nivelNodo,comp){
	referenciaIzq = 0;
};
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
		ElementoNodo* elemento = new ElementoNodo(refD,claveEstructural->clonarce());
		listaElementos.push_back(elemento);
		Aux--;
	}
};
Referencia NodoIntermedio::getReferenciaIzq(){
	return referenciaIzq;
};
void NodoIntermedio::setRefereciaIzq(Referencia ref){
	referenciaIzq = ref;
};
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
};
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
};
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
		    /* Si es izq quiere decir q se balancea con un nodo a la izq, como concecuencia debo buscar en el
		     * padre a partir del primer elemento del nodo completo.
		     */
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
		    /*
		     * En referenciaDeBusqueda tengo al elemento a cambiar de nodo y en elemento padre
		     * el nodo q se modifica del padre.
		     */
		    Referencia refIzqNodoDer;
		    refIzqNodoDer = referenciaDeBusqueda->getReferencia();
		    Clave* claveAux = referenciaDeBusqueda->getClave();
			/*agrega en el nodo izq el elemento q corresponde*/
		    referenciaDeBusqueda->setClave(elementoPadre->getClave());//para elemento q cambia de nodo, y la clave será la del elemento del padre
		    referenciaDeBusqueda->setReferencia(referenciaIzq);
		    nodoHermano->getListaElementos()->push_back(referenciaDeBusqueda);
		    nodoHermano->setEspacioLibre(nodoHermano->getEspacioLibre()-1);
			/*Se modifica el elemento del padre,su clave*/
		    elementoPadre->setClave(claveAux);
			/* modifico la referencia izq del nodo q estaba sobrebordado*/
		    referenciaIzq = refIzqNodoDer;
		    /*elimino el elemento del nodo desbordado que paso al padre*/
		    listaElementos.pop_front();
		}else{
			/*El nodo con el q se balancea esta a la derecha*/
			encontrado = false;
			referenciaDeBusqueda = listaElementos.back();
			/*busco el elemento del padre afectado*/
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
			/*agrega en el nodo der el elemento q corresponde*/
			referenciaDeBusqueda->setClave(elementoPadre->getClave());
			referenciaDeBusqueda->setReferencia(nodoHermano->getReferenciaIzq());
			nodoHermano->getListaElementos()->push_front(referenciaDeBusqueda);
			nodoHermano->setEspacioLibre(nodoHermano->getEspacioLibre()-1);
			/*Se modifica el elemento del padre, su referencia y su clave*/
		    elementoPadre->setClave(clave);
		    /* modifico la referencia izq del nodo derecho*/
		    nodoHermano->setRefereciaIzq(refIzqNodoDer);
		    /*elimino el elemento del nodo desbordado que paso al padre*/
		    listaElementos.pop_back();
		}
};
void NodoIntermedio::balanceoEspecial(Nodo* nodoPegadoE,Nodo* nodoAlejadoE,Nodo* padre,bool Izq){
	NodoIntermedio* nodoPegado = dynamic_cast<NodoIntermedio*>(nodoPegadoE);
	NodoIntermedio* nodoAlejado = dynamic_cast<NodoIntermedio*>(nodoAlejadoE);
	if(Izq){
		std::list<ElementoNodo*>::iterator itPadre = padre->getListaElementos()->begin();
		ElementoNodo* elemPadre = *itPadre;
		balanceoEspecialIzq(nodoPegado,elemPadre,&listaElementos);
		++itPadre;
		elemPadre = *itPadre;
		balanceoEspecialIzq(nodoAlejado,elemPadre,nodoPegado->getListaElementos());
	}else{
		std::list<ElementoNodo*>::reverse_iterator itPadre = padre->getListaElementos()->rbegin();
		ElementoNodo* elemPadre = *itPadre;
		balanceoEspecialDer(nodoPegado,elemPadre,&listaElementos,referenciaIzq);
		++itPadre;
		elemPadre = *itPadre;
		Referencia refIzq=nodoPegado->getReferenciaIzq();
		balanceoEspecialDer(nodoAlejado,elemPadre,nodoPegado->getListaElementos(),refIzq);
		nodoPegado->setRefereciaIzq(refIzq);
	}
};
void NodoIntermedio::balanceoEspecialIzq(NodoIntermedio* nodoQcede,ElementoNodo* elemPadre,std::list<ElementoNodo*> *listaQrecibe){
	Referencia refElemHijo = nodoQcede->getListaElementos()->front()->getReferencia();
	Clave* claveElemHijo = nodoQcede->getListaElementos()->front()->getClave();
	listaQrecibe->push_back(nodoQcede->getListaElementos()->front());
	listaQrecibe->back()->setClave(elemPadre->getClave());
	listaQrecibe->back()->setReferencia(nodoQcede->getReferenciaIzq());
	nodoQcede->getListaElementos()->pop_front();
	nodoQcede->setRefereciaIzq(refElemHijo);
	elemPadre->setClave(claveElemHijo);
};
void NodoIntermedio::balanceoEspecialDer(NodoIntermedio* nodoQcede,ElementoNodo* elemPadre,std::list<ElementoNodo*> *listaQrecibe,Referencia& refIzq){
	Referencia refElemHijo = nodoQcede->getListaElementos()->back()->getReferencia();
	Clave* claveElemHijo = nodoQcede->getListaElementos()->back()->getClave();
	listaQrecibe->push_front(nodoQcede->getListaElementos()->back());
	listaQrecibe->front()->setClave(elemPadre->getClave());
	listaQrecibe->front()->setReferencia(refIzq);
	nodoQcede->getListaElementos()->pop_back();
    refIzq = refElemHijo;
    elemPadre->setClave(claveElemHijo);
};
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
ElementoNodo* NodoIntermedio::dividirse(Nodo* nodoHermanoE,Nodo* nodoIzqE,Nodo* nodoMedioE,Nodo* nodoDerE,Nodo* nodoPadreE,Clave clave){
	NodoIntermedio* nodoHermano = dynamic_cast<NodoIntermedio*>(nodoHermanoE);
	NodoIntermedio* nodoIzq = dynamic_cast<NodoIntermedio*>(nodoIzqE);
	NodoIntermedio* nodoMedio = dynamic_cast<NodoIntermedio*>(nodoMedioE);
	NodoIntermedio* nodoDer = dynamic_cast<NodoIntermedio*>(nodoDerE);
	NodoIntermedio* nodoPadre= dynamic_cast<NodoIntermedio*>(nodoPadreE);
	int cantElementosIzq = ((this->getCatidadMaximaDeElementos()*2)+1)/3;
	int cantElementosMedio = ((this->getCatidadMaximaDeElementos()*2)+1)/3;
	ElementoNodo* elem;
	ElementoNodo* retorno;
	/*Busco en el padre el elemento de la clave dada*/
	std::list<ElementoNodo*>::iterator itPadre = nodoPadre->getListaElementos()->begin();
	bool encontrado = false;
	while(!encontrado){
		elem = *itPadre;/*halias 320*/
		if(comparador->Comparar(elem->getClave(),&clave)){
			encontrado=true;
		}
	}
	nodoIzq->setRefereciaIzq(referenciaIzq);
	std::list<ElementoNodo*>::iterator itThis = listaElementos.begin();
	std::list<ElementoNodo*>::iterator itHermano = nodoHermano->getListaElementos()->begin();
	while(itThis != listaElementos.end ()){
		ElementoNodo* elemento = *itThis;
		if(cantElementosIzq==0) {//encontre a halias 309
			/*este elemento es el q debe agregarce a nodo padre. su referencia derecha es indefinida*/
			nodoMedio->setRefereciaIzq(elemento->getReferencia());
		    elemento->setReferencia(0);
			retorno = elemento;
			cantElementosIzq = -1;
			++itThis;
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
	/*
	 * Se debe relocalizar la clave padre, para hacerlo debo usar el elemento q separe
	 * al nodo intermedio del nodo derecho, entonces se guarda en un elemento auxiliar
	 * su clave hasta q se pueda setear el elemento padre y el elemento hijo. Para
	 * tenerlo en cuenta q habra un elementomas resto un valor a cantElementosMedio
	 */
	cantElementosMedio--;
	while(itHermano != nodoHermano->getListaElementos()->end()){
		ElementoNodo* elemento2 = *itHermano;
		if(cantElementosMedio==0){//halias 340
			/*este elemento es el q debe hacer cambio con padre. ademas la referencia de este elemento debe
			 * ser la referencia izq de nodo Derecho.
			 */
			nodoDer->setRefereciaIzq(elemento2->getReferencia());
			Clave* claveAux = elem->getClave();
			elem->setClave(elemento2->getClave());
			elemento2->setClave(claveAux);
			elemento2->setReferencia(nodoHermano->getReferenciaIzq());
			nodoMedio->agregarElemento(elemento2);
			cantElementosMedio=-1;
			++itHermano;
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
};

/*
 * Segun el orden establecido, al ser un nodo intermedio una clave puede ser igual
 * a la buscada, sin embargo la informacion solo se encuentra en Nodos Hoja, con lo cual
 * hay q segir la busqueda, por lo q la referencia entregada es la ref derecha de ese elemento.
 */
Referencia NodoIntermedio::bucarReferenciaAsiguienteNodo(const Clave* clave){
	    /*primero verifico q la referencia buscada no sea la referencia izq del nodo*/
	ElementoNodo* elemento;
	elemento = listaElementos.front();
	if(comparador->Comparar(clave,elemento->getClave())<0){
		return referenciaIzq;
	}
	std::list<ElementoNodo*>::iterator it = listaElementos.begin();
    Referencia ref;
	bool encontrado = false;
	while(!encontrado){
		elemento = *it;
		int comparacion = comparador->Comparar(elemento->getClave(),clave);
		if(comparacion<0 && it != listaElementos.end ()){
		      ref = elemento->getReferencia();
			  ++it;
		  }else if (comparacion == 0){
		      encontrado = true;
		      ref = elemento->getReferencia();
		  }else if(comparacion<0 && it == listaElementos.end()){
		    	encontrado = true;
		    	ref = elemento->getReferencia();
		  }else if(comparacion > 0){
		    	encontrado = true;
		    }
	}
		return ref;
};
int NodoIntermedio::unirse(Nodo* nodoHermanoIzq,Nodo* nodoHermanoDer,Nodo* Padre){
	std::list<ElementoNodo*>::reverse_iterator it = Padre->getListaElementos()->rbegin();
	ElementoNodo* elementoPadre;
	Clave* clave;
	bool encontrado = false;
	//Busco el primero menor al primero del nodo DERECHO
	while(!encontrado && it!=Padre->getListaElementos()->rend()){
		elementoPadre = *it;
		if(comparador->Comparar(elementoPadre->getClave(),nodoHermanoDer->getListaElementos()->front()->getClave())<0){
			encontrado = true;
		}
		++it;
		//Clave me indicara luego q elemento del padre debe ser eliminado
		ElementoNodo* aux = *it;
		clave = aux->getClave();
	}
	//Almaceno la clave del elemento del padre para poder setear al mismo sin problemas
	Clave* claveDeElementoPadre = elementoPadre->getClave();
	//Almaceno la referencia q aloja la ref izq del nodo derecho
	NodoIntermedio* nodoDerecho = dynamic_cast<NodoIntermedio*>(nodoHermanoDer);
	Referencia refIzqDeNodoDer  = nodoDerecho->getReferenciaIzq();
	unsigned int cantIzq = cantidadMaximaDeElementos- (cantidadMaximaDeElementos - nodoHermanoIzq->getEspacioLibre()) - 1;
	std::list<ElementoNodo*>::iterator itMedio = listaElementos.begin();
	while(itMedio != listaElementos.end()){
		if(cantIzq>0){
			nodoHermanoIzq->getListaElementos()->push_back(*itMedio);
		}else{
		if(cantIzq==0){
					ElementoNodo* elem = *itMedio;
					nodoDerecho->setRefereciaIzq(elem->getReferencia());
					elementoPadre->setClave(elem->getClave());
					elem->setReferencia(refIzqDeNodoDer);
					elem->setClave(claveDeElementoPadre);
					nodoHermanoDer->getListaElementos()->push_front(elem);
					cantIzq--;
		}else{nodoHermanoDer->getListaElementos()->push_front(*itMedio);}
	  }
		++itMedio;
	}
	listaElementos.clear();
	return Padre->Eliminar(clave);
};
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
};
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
};
void NodoIntermedio::limpiar(){
	std::list<ElementoNodo*>::iterator it;
		for(it = listaElementos.begin();it != listaElementos.end();++it){
			 ElementoNodo* elemento = *it;
			 delete elemento;
		}
};
NodoIntermedio::~NodoIntermedio() {

};

