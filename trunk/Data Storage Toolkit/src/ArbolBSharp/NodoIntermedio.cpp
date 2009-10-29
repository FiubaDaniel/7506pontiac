/*
 * NodoIntermedio.cpp
 *
 *  Created on: 30/09/2009
 *      Author: daniel
 */

#include "NodoIntermedio.h"

NodoIntermedio::NodoIntermedio(unsigned int nivelNodo,unsigned int numNodo,unsigned int cantElem,ComparadorClave* comp): NodoIntermedio ::Nodo(numNodo,cantElem,nivelNodo){

	comparador = comp;
	referenciaIzq = 0;
};
NodoIntermedio::NodoIntermedio(std::stringbuf* buf,unsigned int cantElem,ComparadorClave* comp): NodoIntermedio ::Nodo(0,cantElem,0){
	comparador = comp;
	buf->pubseekpos(0);
	unsigned int Nnodo,cantElemLibres,Nnivel;
	buf->sgetn((char*)&Nnivel,sizeof(Nnivel));
	Nodo::setNumeroNivel(Nnivel);
	buf->sgetn((char*)&Nnodo,sizeof(Nnodo));
	Nodo::setNumeroDeNodo(Nnodo);
	buf->sgetn((char*)&cantElemLibres,sizeof(cantElemLibres));
	Nodo::setEspacioLibre(cantElemLibres);
	int Aux = cantElem - cantElemLibres;
	buf->sgetn((char*)&referenciaIzq,sizeof(Referencia));
	Referencia refD;
	while(Aux>0){
		Clave* clave = new Clave();
		clave->deserializar(*buf);
		buf->sgetn((char*)&refD,sizeof(Referencia));
		ElementoNodoIntermedio* elemento = new ElementoNodoIntermedio(refD,clave);
		listaElementos.push_back(elemento);
		Aux--;
	}
};
std::list<ElementoNodoIntermedio*>NodoIntermedio::getListaElementos(){
	return listaElementos;
};
Referencia NodoIntermedio::getReferenciaIzq(){
	return referenciaIzq;
};
void NodoIntermedio::setRefereciaIzq(Referencia ref){
	referenciaIzq = ref;
};
void NodoIntermedio::serializate(std::stringbuf* buffer){
	    buffer->pubseekpos(0);
		unsigned int Nnodo,cantElemLibres,nivel;
		Nnodo = Nodo::getNumeroDeNodo();
		nivel = Nodo::getNumeroNivel();
		cantElemLibres = Nodo::getEspacioLibre();
		buffer->sputn ((char*)&nivel,sizeof(nivel));
		buffer->sputn ((char*)&Nnodo,sizeof(Nnodo));
		buffer->sputn ((char*)&cantElemLibres,sizeof(cantElemLibres));
		buffer->sputn ((char*)&referenciaIzq,sizeof(referenciaIzq));
		std::list<ElementoNodoIntermedio*>::iterator it = listaElementos.begin();
		if(!listaElementos.empty()){
			while(it != listaElementos.end()){
				ElementoNodoIntermedio* elemento = *it;
			    Clave * clave = elemento->getClave();
			    clave->serializar(*buffer);
			    Referencia refD = elemento->getReferenciaDer();
			    buffer->sputn ((char*)refD,sizeof(Referencia));
			    ++it;
			}
		}
};
/*
 * Devuelve 1 si ubico el elemento o devuelve 0 si ya existia
 * En caso de estar lleno devuelve 2, indicando q el nodo ya tiene todos
 * sus elementos, como concecuencia debe reorganizarce colocando un elemento
 * en un nodo hermano o producir la generacion de otro nodo. el elemento se agregara
 * en la posicion correcta enviando el mensaje de arreglar esa situacion.
 * El elemento viene con sus referencia derecha seteada.
 */
int NodoIntermedio::agregarElemento(ElementoNodoIntermedio* elemento){
	int retorno = 1;
	if(listaElementos.size() == Nodo::getCatidadMaximaDeElementos()){
		retorno = 2;
	}
	if(listaElementos.empty()){
			listaElementos.push_back(elemento);
			Nodo::setEspacioLibre(Nodo::getEspacioLibre()-1);
			return 1;
		}
	std::list<ElementoNodoIntermedio*>::iterator it = listaElementos.begin();
	bool ubicado = false;
	while(!ubicado || it != listaElementos.end()){
		ElementoNodoIntermedio* elementoAux = *it;
		if(comparador->Comparar(elemento->getClave(),elementoAux->getClave())==0){
			retorno = 0,
			ubicado=true;
		}else if(comparador->Comparar(elemento->getClave(),elementoAux->getClave())<0){
			listaElementos.push_back(elemento);
			Nodo:: setEspacioLibre(Nodo::getEspacioLibre()-1);
			ubicado=true;
		}else{
			++it;
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
void NodoIntermedio::balanceo(NodoIntermedio* nodoHermano,NodoIntermedio* nodoPadre, bool izq){

	ElementoNodoIntermedio* elementoPadre;
	ElementoNodoIntermedio* referenciaDeBusqueda;
	bool encontrado;
	if(izq){
		    /* Si es izq quiere decir q se balancea con un nodo a la izq, como concecuencia debo buscar en el
		     * padre a partir del primer elemento del nodo completo.
		     */
		    encontrado = false;
		    referenciaDeBusqueda = listaElementos.front();
		    std::list<ElementoNodoIntermedio*>::reverse_iterator itReversa = nodoPadre->getListaElementos().rbegin();
		    while(!encontrado && itReversa != nodoPadre->getListaElementos().rend()){
		    	ElementoNodoIntermedio* elementoPadre = *itReversa;
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
		    refIzqNodoDer = referenciaDeBusqueda->getReferenciaDer();
		    Clave* claveAux = referenciaDeBusqueda->getClave();
			/*agrega en el nodo izq el elemento q corresponde*/
		    referenciaDeBusqueda->setClave(elementoPadre->getClave());//para elemento q cambia de nodo, y la clave será la del elemento del padre
		    referenciaDeBusqueda->setReferenciaDer(referenciaIzq);
		    nodoHermano->getListaElementos().push_back(referenciaDeBusqueda);
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
			std::list<ElementoNodoIntermedio*>::iterator it = nodoPadre->getListaElementos().begin();
			while(!encontrado && it != nodoPadre->getListaElementos().end()){
		         ElementoNodoIntermedio* elementoPadre = *it;
				 if(comparador->Comparar(referenciaDeBusqueda->getClave(),elementoPadre->getClave())>0){
					    ++it;
				}else{
					   encontrado = true;
				    }
			 }
			Referencia refIzqNodoDer;
			refIzqNodoDer = referenciaDeBusqueda->getReferenciaDer();
			Clave* clave = referenciaDeBusqueda->getClave();
			/*agrega en el nodo der el elemento q corresponde*/
			referenciaDeBusqueda->setClave(elementoPadre->getClave());
			referenciaDeBusqueda->setReferenciaDer(referenciaIzq);
			nodoHermano->getListaElementos().push_front(referenciaDeBusqueda);
			/*Se modifica el elemento del padre, su referencia y su clave*/
		    elementoPadre->setClave(clave);
		    /* modifico la referencia izq del nodo derecho*/
		    nodoHermano->setRefereciaIzq(refIzqNodoDer);
		    /*elimino el elemento del nodo desbordado que paso al padre*/
		    listaElementos.pop_back();
		}
};
/*
 * Dos nodos uno sobrebordado y el otro completo, entonces se dividen en nodos en tres nodos nuevos,
 * dos de ellos se almacenaran el la posicion de los nodos existentes y otro en una nueva posicion
 * como consecuencia el elemento agregado tendra su referencia derecha nula hasta q fuera se le encuentre un lugar.
 * Clave indica el elemento del padre q separa ambos nodos.
 * retorna el un valor q indica si el padre quedo en sobreflujo.
 * claveRefNull indica la clave del elemento q tiene refeencia indefinida en nodo padre.
 */
 /*
  * PreCondicion: el nodo this q llame a la funcion es el nodo Izquierdo, o sea el de claves mas chicas.
  */
int NodoIntermedio::dividirse(NodoIntermedio* nodoHermano,NodoIntermedio* nodoIzq,NodoIntermedio* nodoMedio,NodoIntermedio* nodoDer,NodoIntermedio* nodoPadre,Clave clave,Clave* claveRefNull){
	int retorno;
	int maximoDeElementos = Nodo::getCatidadMaximaDeElementos();
	int cantElementosIzq = ((maximoDeElementos*2)+1)/3;
	int cantElementosMedio = ((maximoDeElementos*2)+1)/3;
	ElementoNodoIntermedio *elem;
	/*Busco en el padre el elemento de la clave dada*/
	std::list<ElementoNodoIntermedio*>::iterator itPadre = nodoPadre->getListaElementos().begin();
	bool encontrado = false;
	while(!encontrado){
		elem = *itPadre;/*halias 320*/
		if(comparador->Comparar(elem->getClave(),&clave)){
			encontrado=true;
		}
	}
	nodoIzq->setRefereciaIzq(referenciaIzq);
	std::list<ElementoNodoIntermedio*>::iterator itThis = listaElementos.begin();
	std::list<ElementoNodoIntermedio*>::iterator itHermano = nodoHermano->getListaElementos().begin();
	while(itThis != listaElementos.end ()){
		ElementoNodoIntermedio* elemento = *itThis;
		if(cantElementosIzq==0) {//encontre a halias 309
			/*este elemento es el q debe agregarce a nodo padre. su referencia derecha es indefinida*/
			nodoMedio->setRefereciaIzq(elemento->getReferenciaDer());
			claveRefNull = elemento->getClave();
		    elemento->setReferenciaDer(0);
			retorno = nodoPadre->agregarElemento(elemento);
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
	while(itHermano != nodoHermano->getListaElementos().end()){
		ElementoNodoIntermedio* elemento2 = *itHermano;
		if(cantElementosMedio==0){//halias 340
			/*este elemento es el q debe hacer cambio con padre. ademas la referencia de este elemento debe
			 * ser mla referencia izq de nodo Derecho.
			 */
			nodoDer->setRefereciaIzq(elemento2->getReferenciaDer());
			Clave* claveAux = elem->getClave();
			elem->setClave(elemento2->getClave());
			elemento2->setClave(claveAux);
			elemento2->setReferenciaDer(nodoHermano->getReferenciaIzq());
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
	nodoHermano->getListaElementos().clear();
	return retorno;
};

/*
 * Segun el orden establecido, al ser un nodo intermedio una clave puede ser igual
 * a la buscada, sin embargo la informacion solo se encuentra en Nodos Hoja, con lo cual
 * hay q segir la busqueda, por lo q la referencia entregada es la ref derecha de ese elemento.
 */
Referencia NodoIntermedio::bucarReferenciaAsiguienteNodo(Clave clave){
	    /*primero verifico q la referencia buscada no sea la referencia izq del nodo*/
	ElementoNodoIntermedio* elemento;
	elemento = listaElementos.front();
	if(comparador->Comparar(&clave,elemento->getClave())<0){
		return referenciaIzq;
	}
	std::list<ElementoNodoIntermedio*>::iterator it = listaElementos.begin();
    Referencia ref;
	bool encontrado = false;
	while(!encontrado){
		elemento = *it;
		int comparacion = comparador->Comparar(elemento->getClave(),&clave);
		if(comparacion<0 && it != listaElementos.end ()){
		      ref = elemento->getReferenciaDer();
			  ++it;
		  }else if (comparacion == 0){
		      encontrado = true;
		      ref = elemento->getReferenciaDer();
		  }else if(comparacion<0 && it == listaElementos.end()){
		    	encontrado = true;
		    	ref = elemento->getReferenciaDer();
		  }else if(comparacion > 0){
		    	encontrado = true;
		    }
	}
		return ref;
};
NodoIntermedio::~NodoIntermedio() {
	std::list<ElementoNodoIntermedio*>::iterator it;
	for(it = listaElementos.begin();it != listaElementos.end();++it){
		 ElementoNodoIntermedio* elemento = *it;
		 delete elemento;
	}
	listaElementos.clear();
};

