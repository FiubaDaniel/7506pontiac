/*
 * NodoHoja.cpp
 *
 *  Created on: 30/09/2009
 *      Author: daniel
 */
#include "NodoHoja.h"
NodoHoja::NodoHoja(unsigned int numNodo,unsigned int cantElem,Referencia refIzq,Referencia refDer,ComparadorClave* comparador) : NodoHoja ::Nodo(numNodo,cantElem,0){
	referenciaIzq = refIzq;
	referenciaDer = refDer;
};
/*
 * Se cre el  nodo hoja a partir de un buffer, la cantidad de elementos, el tamaño de
 * la hoja y el tamaño de la clave  son parametros ya q se guardan solo una vez.
 */
NodoHoja::NodoHoja(std::stringbuf buf,unsigned int cantElem,ComparadorClave* comp) : NodoHoja ::Nodo(0,cantElem,0){
	comparador = comp;
	buf.pubseekpos(0);
	unsigned int Nnodo,cantElemLibres,nivel;
	buf.sgetn((char*)&nivel,sizeof(nivel));
	Nodo::setNumeroNivel(Nnodo);
	buf.sgetn((char*)&Nnodo,sizeof(Nnodo));
	Nodo::setNumeroDeNodo(Nnodo);
	buf.sgetn((char*)&cantElemLibres,sizeof(cantElemLibres));
	Nodo::setEspacioLibre(cantElemLibres);
	buf.sgetn((char*)&referenciaIzq,sizeof(referenciaIzq));
	buf.sgetn((char*)&referenciaDer,sizeof(referenciaDer));
	int Aux = cantElem - cantElemLibres;
	Referencia ref;
	while(Aux>0){
		Clave* clave = new Clave();
		clave->deserializar(buf);
		buf.sgetn((char*)&ref,sizeof(Referencia));
		ElementoNodo* elemento = new ElementoNodo(ref,clave);
		listaElementos.push_back(elemento);
		Aux--;
	}
};
std::list<ElementoNodo*> NodoHoja:: getListaElementos(){
	return listaElementos;
};
Referencia NodoHoja::getReferenciaIzq(){
	return referenciaIzq;
};
Referencia NodoHoja::getReferenciaDer(){
	return referenciaDer;
};
void NodoHoja::setReferenciaIzq(Referencia ref){
	referenciaIzq =ref;
};
void NodoHoja::setReferenciaDer(Referencia ref){
	referenciaDer = ref;
};
/*
 * El tamaño de la clave ya fue serializado al igual q la cantidad de elementos por nodo
 * al princio del archivo.
 */
void NodoHoja::serializate(stringbuf* buffer){

	unsigned int Nnodo,cantElemLibres,nivel;
	nivel = Nodo::getNumeroNivel();
	Nnodo = Nodo::getNumeroDeNodo();
	cantElemLibres = Nodo::getEspacioLibre();
	buffer->sputn ((char*)&nivel,sizeof(nivel));
	buffer->sputn ((char*)&Nnodo,sizeof(Nnodo));
	buffer->sputn ((char*)&cantElemLibres,sizeof(cantElemLibres));
	buffer->sputn ((char*)&referenciaIzq,sizeof(referenciaIzq));
	buffer->sputn ((char*)&referenciaDer,sizeof(referenciaDer));
	std::list<ElementoNodo*>::iterator it;
	it = listaElementos.begin();
	if(!listaElementos.empty()){
	while(it != listaElementos.end()){
		ElementoNodo* elemento = *it;
		Clave * clave = elemento->getClave();
		clave->serializar(*buffer);
		Referencia ref = elemento->getReferencia();
		buffer->sputn ((char*)ref,sizeof(ref));
		++it;
	   }
	}
};
/*
 * Devuelve 1 si ubico el elemento o devuelve 0 si ya existia
 * En caso de estar lleno devuelve 2, indicando q el nodo ya tiene todos
 * sus elementos, como concecuencia debe reorganizarce colocando un elemento
 * en un nodo hermano o producir la generacion de otro nodo. el elemento se agregara
 * en la posicion correcta enviando el mensaje de arreglar esa situacion
 */
int NodoHoja::agregarElemento(ElementoNodo* elemento){

	int retorno = 1;
	if(listaElementos.size() == Nodo::getCatidadMaximaDeElementos()){
		retorno = 2;
	}
	if(listaElementos.empty()){
		listaElementos.push_back(elemento);
		Nodo::setEspacioLibre(Nodo::getEspacioLibre()-1);
		return 1;
	}
	std::list<ElementoNodo*>::iterator it = listaElementos.begin();
	bool ubicado = false;
	while(!ubicado || it != listaElementos.end()){
		ElementoNodo* elementoAux = *it;
		if(comparador->Comparar(elemento->getClave(),elementoAux->getClave())==0){
			retorno = 0,
			ubicado=true;
		}else if(comparador->Comparar(elemento->getClave(),elementoAux->getClave())<0){
			listaElementos.push_back(elemento);
			if(retorno==1){
			Nodo:: setEspacioLibre(Nodo::getEspacioLibre()-1);
			}
			ubicado=true;
		}else{
			it++;
		}
	}
    return retorno;
};
int NodoHoja::calcularDosTercios(){
    return ((int)((Nodo::getCatidadMaximaDeElementos()*2)/3));
};
/*El nodo this contiene un elemento mas del permitido, siendo q el nodoIncompleto q
 * pasa por parametro tiene al menos una posicion libre para poder alojar ese elemento
 * izq indica si es el nodo izq con el q se valancea o si el false con el derecho
 */
void NodoHoja::balanceo(NodoHoja* nodoIncompleto,NodoIntermedio* nodoPadre,bool izq){
	bool encontrado = false;
	ElementoNodoIntermedio* elem;

	if(izq){
		ElementoNodo * elemento  = listaElementos.front();
		std::list<ElementoNodoIntermedio*>:: iterator itAux = nodoPadre->getListaElementos().begin();
		while(!encontrado && itAux != nodoPadre->getListaElementos().end()){
			elem = *itAux;
			if(comparador->Comparar(elemento->getClave(),elem->getClave())==0){
			     encontrado = true;
			}else{
				++itAux;
			}
		}
		if(!encontrado){
			//lanzar excepcion
		}
		nodoIncompleto->getListaElementos().push_back(elemento);
		nodoIncompleto->setEspacioLibre(nodoIncompleto->getEspacioLibre()-1);
		listaElementos.pop_front();
		elem->setClave(listaElementos.front()->getClave()->clonarce());
	}else{
		ElementoNodo * elemento  = listaElementos.back();
		std::list<ElementoNodoIntermedio*>:: iterator itAux = nodoPadre->getListaElementos().begin();
		while(!encontrado && itAux != nodoPadre->getListaElementos().end()){
			elem = *itAux;
			if(comparador->Comparar(elemento->getClave(),elem->getClave())<0){
				encontrado = true;
			}else{
				++itAux;
			}
		}
		if(!encontrado){
			//excepcion
		}
		nodoIncompleto->getListaElementos().push_front(elemento);
		nodoIncompleto->setEspacioLibre(nodoIncompleto->getEspacioLibre()-1);
		elem->setClave(elemento->getClave());
		listaElementos.pop_back();
	}
};
/*
 * Se reacomodan los elementos y las referencias q los unen, el nuevo nodo seria el medio, como concecuencia las referencias a es seran
 * indefinidas y se estableceran fuera del metodo.
 * Clave indica la clave del padre q separa los nodos completos.
 * El elemento q se agregue al padre (q apuntara a nodo medio) tmb tiene su referencia indefinida y debe ser agregado fuera de la funcion.
 */
/*
 * Precondicion: el nodo que llame a la funcion dividirce debe ser el izq.
 */
ElementoNodoIntermedio* NodoHoja::dividirse(NodoHoja* nodoHermano,NodoHoja* nodoIzq,NodoHoja* nodoMedio,NodoHoja* nodoDer,NodoIntermedio* nodoPadre,Clave clave){

	int cantElem = Nodo::getCatidadMaximaDeElementos();
	int cantIzq = ((cantElem*2)+1)/3;
	int cantMedio = ((cantElem*2)+1)/3;
	ElementoNodoIntermedio * elementoApadre,*elementoPadre;
	bool encontrado = false;
	/*busco en padre la clave, será el elemento seteado*/
	list<ElementoNodoIntermedio*>::iterator itPadre = nodoPadre->getListaElementos().begin();
	while(!encontrado){
		elementoPadre = *itPadre;
		if(comparador->Comparar(elementoPadre->getClave(),&clave)==0){
			encontrado=true;
		}else{
			++itPadre;
		}
	}
	list<ElementoNodo*>::iterator it = listaElementos.begin();
	list<ElementoNodo*>::iterator it2 = nodoHermano->getListaElementos().begin();
	/*Cargo hijo izquierdo con 2/3 del padre*/
	while(it != listaElementos.end ()){
		ElementoNodo* elemento = *it;
		if(cantIzq==0) {
		    elementoApadre = new ElementoNodoIntermedio(0,elemento->getClave()->clonarce());
		    nodoMedio->agregarElemento(elemento);
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
	while(it2 != nodoHermano->getListaElementos().end ()){
		ElementoNodo* elemento = *it2;
		if(cantMedio==0){
			elementoPadre->setClave(elemento->getClave()->clonarce());
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
	nodoHermano->getListaElementos().clear();
	return elementoApadre;
};
/*
 * Retorna un elemento buscado.
 */
ElementoNodo* NodoHoja::buscar(Clave* clave){
	std::list<ElementoNodo*>::iterator it = listaElementos.begin();
	ElementoNodo* elemento;
	bool encontrado = false;
	bool existe = true;
	while(!encontrado && it!=listaElementos.end() && existe){
		elemento = *it;
	    int comparacion = comparador->Comparar(elemento->getClave(),clave);
	    if(comparacion>0){
	    	existe=false;
	        elemento=NULL;
	 	   }else if(comparacion==0){
	 		   encontrado=true;
	 	   }else{
	 		   ++it;
	 	     }
	}
	    if(!encontrado)elemento=NULL;
	    return elemento;
};
/*
 * Bucas la clave dentro del nodo Hoja, y si la encuentra la retorna la referencia
 * al archivo de esa clave.En caso de no existir retorna el valor -1.
 */
Referencia NodoHoja::buscarReferenciaDeClaveX(Clave* clave){
   ElementoNodo* elemento = NodoHoja::buscar(clave);
   if(elemento==NULL)return -1;
   return elemento->getReferencia();
};
/*
 * Busca una clave determinada y modifica la referencia al archivo que le corresponde
 * devuelve -1 si no encontro la clave o 1 si la encontro y modifico.
 */
int NodoHoja::setReferenciaDeClaveX(Clave* clave,Referencia refNueva){
	ElementoNodo* elemento = NodoHoja::buscar(clave);
	if(elemento==NULL)return -1;
	elemento->setReferencia(refNueva);
	return 1;
};
/*
 * Devuelve en elemento el elemento buscado o en su defecto si no existe devuelve el sigueite elemento.
 * retorna un valor entero. -1 si no encontro. 0 si es igual al q se buscaba. 1 si es el siguiente.
 */
int NodoHoja::busquedaSecuencial(Clave* clave,ElementoNodo* elemento){
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
};
NodoHoja::~NodoHoja() {

    list<ElementoNodo*>::iterator it = listaElementos.begin();
    while(it!=listaElementos.end()){
    	ElementoNodo* elemento = *it;
    	delete elemento;
    	++it;
    }
    listaElementos.clear();
};
