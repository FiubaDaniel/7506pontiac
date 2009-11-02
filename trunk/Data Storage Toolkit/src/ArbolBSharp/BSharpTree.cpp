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
}
void BSharpTree::crear(string nombreArch,unsigned int tamanioDeBloque, int tamanioSerializadoClave,ComparadorClave* comp) {
	 comparador = comp;
	 tamanioNodo = tamanioDeBloque;
	numeroDeElementosXnodo = calcularCantidadElementosPorNodo(tamanioSerializadoClave);
	cantidadMinimaDeElementos = (unsigned int) ((numeroDeElementosXnodo)*2)/3;
	nombreArchivo = nombreArch+"_Arbol";
	nombreEspaciosLibres = nombreArch+"_EspaciosLibre";
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
		posicionRaiz = 3*sizeof(int);
		buffer.sputn ((char*)&posicionRaiz,sizeof(posicionRaiz));
		archivoArbol.write(array,tamanio);
		/*Establezco la raiz y su referencia izq creando la hoja izq vacia*/
		Raiz = new NodoIntermedio((unsigned int) 1,numeroDeElementosXnodo,comparador);
		char array2[tamanioNodo];
		buffer.pubsetbuf(array2,tamanioNodo);
		buffer.pubseekpos(0);
		Raiz->serializate(&buffer);
		archivoArbol.write(array2,tamanioNodo);
		Raiz->setRefereciaIzq(archivoArbol.tellp());
		NodoHoja* hoja = new NodoHoja(numeroDeElementosXnodo,0,comparador);
		buffer.pubseekpos(0);
		hoja->serializate(&buffer);
		archivoArbol.write(array2,tamanioNodo);
		archivoArbol.seekp(posicionRaiz);
		buffer.pubseekpos(0);
		Raiz->serializate(&buffer);
		archivoArbol.write(array2,tamanioNodo);
		delete hoja;
		archivoEspaciosLibres.seekp(0);
		char array3[sizeof(int)];
		buffer.pubsetbuf(array3,sizeof(int));
		buffer.pubseekpos(0);
		int cero = 0;
		buffer.sputn ((char*)&cero,sizeof(cero));
		buffer.pubseekpos(0);
		archivoEspaciosLibres.write(array3,sizeof(cero));
	}
};
void BSharpTree::abrir(string nombreArch,ComparadorClave* comp){
	comparador = comp;
	nombreArchivo = nombreArch+"_Arbol";
    nombreEspaciosLibres = nombreArch+"_EspaciosLibre";
	archivoArbol.seekg(0);
	archivoEspaciosLibres.open(nombreEspaciosLibres.c_str(),std::fstream::out|std::fstream::in|std::fstream::binary);
	archivoArbol.open(nombreArchivo.c_str(),std::fstream::out|std::fstream::in|std::fstream::binary);
	std:: stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	int tamanio = sizeof(int)*3;
	char array[tamanio];
	buf.pubsetbuf(array,tamanio);
	archivoArbol.read(array,tamanio);
	buf.pubseekpos(0);
	buf.sgetn((char*)&numeroDeElementosXnodo,sizeof(numeroDeElementosXnodo));
	buf.sgetn((char*)&tamanioNodo,sizeof(tamanioNodo));
	buf.sgetn((char*)&posicionRaiz,sizeof(int));
	char array2[tamanioNodo];
	buf.pubsetbuf(array2,tamanioNodo);
	archivoArbol.read(array2,tamanio);
	Raiz = new NodoIntermedio(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
    cantidadMinimaDeElementos = (unsigned int) ((numeroDeElementosXnodo)*2)/3;
};
int BSharpTree::calcularCantidadElementosPorNodo(unsigned int tamSerializadoClave){
	 unsigned int resultado = tamanioNodo - 2 * sizeof(int) - sizeof(Referencia);
	 int tamElemento = sizeof(Referencia)+tamSerializadoClave;
	 resultado = (int)resultado/tamElemento;
	 if (resultado<1){
		throw  TamanioInsuficienteException();
	 }else return resultado;
};
/*
 * Devuelve false en caso de no existir la clave buscada.
 * Almacena el nodo Hoja de donde obtuvo el elemento para una posible busqueda secuencial.
 */
bool BSharpTree::Buscar(const Clave* clave,Referencia* referencia){
	delete ultimoNodo;
	bool encontrado =  buscarIterativo(*Raiz,clave,referencia,ultimoNodo);
	archivoArbol.close();
    return encontrado;
};
bool BSharpTree::buscarIterativo(NodoIntermedio nodo,const Clave* clave,Referencia*ref,NodoHoja* ultimo){
	int nivel;
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	Referencia refAux = nodo.bucarReferenciaAsiguienteNodo(clave);
    buf.pubseekpos(0);
    char array2[tamanioNodo];
    archivoArbol.seekg(refAux);
    buf.pubsetbuf(array2,tamanioNodo);
    archivoArbol.read(array2,tamanioNodo);
    buf.pubseekpos(0);
    buf.sgetn((char*)&nivel,sizeof(int));
    if(nivel!=0){
       NodoIntermedio nodoAux(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
       return buscarIterativo(nodoAux,clave,ref,ultimo);
     }else{
    	 /*ver si esta bien lo de destruir el nodo hoja q tenia antes*/
       delete ultimo;
       ultimo = new NodoHoja(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
       bool retorno = ultimo->buscarReferenciaDeClaveX(clave,ref);
       return retorno;
     }
};
/*
 * Pre condicion: archivo abierto para lectura.
 * Pre condicion: cuando se llama por primera vez a la funcion se debe entregar la raiz
 * para comenzar la busqueda correctamente.
 */
NodoHoja* BSharpTree::buscarPrimerNodoHoja(NodoIntermedio nodo){
    Referencia ref;
    int nivel;
    std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	if(nodo.getReferenciaIzq()!=0){
    	 ref = nodo.getReferenciaIzq();
     }else{
    	 ElementoNodo* elemento = nodo.getListaElementos()->front();
    	 ref = elemento->getReferencia();
     }
	char array2[tamanioNodo];
	buf.pubseekpos(0);
	buf.pubsetbuf(array2,tamanioNodo);
	archivoArbol.read(array2,tamanioNodo);
	buf.pubseekpos(0);
	buf.sgetn((char*)&nivel,sizeof(int));
	if(nivel!=0){
	NodoIntermedio nodoAux(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
	return buscarPrimerNodoHoja(nodoAux);
	     }else{
	    	 NodoHoja* nodoAux = new NodoHoja(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
	    	 nodoAux->setPos();
	    	 return nodoAux;
	     }
   return NULL;
};
/*
 * El parametro indica si la busqueda secuencial q se realizará es
 * a partir del ultimo buscado o desde el primer elemento.
 */
//Si quieren referencia y clave o ambas
Referencia BSharpTree::Siguiente(bool ultimo){
	if(!ultimo){
		ultimoNodo = buscarPrimerNodoHoja(*Raiz);
	}else{
		ultimoNodo->avanzarPos();
	}
	ElementoNodo* elemento = ultimoNodo->getPos();
	return elemento->getReferencia();
};
/*
 * true si encontro y modifico, en caso contrario false.
 */
bool BSharpTree::modificar(const Clave* clave,Referencia refNueva){
	/*primero debo buscar el nodo hoja*/
	Referencia referenciaDeNodoHoja;//es la referencia donde se debe guardar el nodo en el archivo una vez modificado.
	NodoHoja* nodo = buscarHoja(*Raiz,clave,referenciaDeNodoHoja);
	bool modificado = nodo->setReferenciaDeClaveX(clave,refNueva);
	if(modificado){
		std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
		char array2[tamanioNodo];
		buf.pubseekpos(0);
		archivoArbol.seekp(referenciaDeNodoHoja);
		buf.pubsetbuf(array2,tamanioNodo);
		buf.pubseekpos(0);
		nodo->serializate(&buf);
		archivoArbol.write(array2,tamanioNodo);;
	}
	return modificado;
};
NodoHoja* BSharpTree::buscarHoja(NodoIntermedio nodo,const Clave* clave,Referencia referenciaDeNodoHoja){
	int nivel;
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	Referencia refAux = nodo.bucarReferenciaAsiguienteNodo(clave);
	char array2[tamanioNodo];
	buf.pubseekpos(0);
	archivoArbol.seekg(refAux);
	buf.pubsetbuf(array2,tamanioNodo);
	archivoArbol.read(array2,tamanioNodo);
	buf.pubseekpos(0);
	buf.sgetn((char*)&nivel,sizeof(int));
	if(nivel!=0){
		NodoIntermedio nodoAux(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
		return buscarHoja(nodoAux,clave,referenciaDeNodoHoja);
	}else{
		NodoHoja* nodoAux = new NodoHoja(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
		return nodoAux;
	}
	return NULL;
};
bool BSharpTree::insertar(Referencia ref,Clave* clave){
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	ElementoNodo* elemento = new ElementoNodo(ref,clave);
	if(Raiz->getEspacioLibre()==numeroDeElementosXnodo){
		/*creo el nodo derecho hoja e inserto el elemento en ambos, raiz e hijo*/
		char array2[tamanioNodo];
		archivoArbol.seekp(0,fstream::end);
		Referencia refAux  = (Referencia)archivoArbol.tellp();
		NodoHoja* hoja = new NodoHoja(numeroDeElementosXnodo,0,comparador);
		hoja->agregarElemento(elemento);
		grabarUnitario(hoja,refAux);
		delete hoja;
		elemento->setReferencia(refAux);
		Raiz->agregarElemento(elemento);
		grabarUnitario(Raiz,posicionRaiz);
		archivoArbol.seekg(Raiz->getReferenciaIzq());
		buf.pubseekpos(0);
		/*Ahora toma la referencia izquierda q ya existia y le seteo su referencia izq hacia la hoja agregada*/
		buf.pubsetbuf(array2,tamanioNodo);
		archivoArbol.read(array2,tamanioNodo);
		hoja = new NodoHoja(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
		hoja->setReferenciaSiguiente(refAux);
	    grabarUnitario(hoja,Raiz->getReferenciaIzq());
	    delete elemento;
	    delete hoja;
		return true;
	}
	Referencia refHoja;
	Nodo* hoja;
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
		char arrayRaiz[tamanioNodo];
		buf.pubsetbuf(arrayRaiz,tamanioNodo);
		Raiz->serializate(&buf);
		archivoArbol.seekp(posicionRaiz);
		archivoArbol.write(arrayRaiz,tamanioNodo);
	}
	listaDePadres.clear();
	delete hoja;
	delete elemento;
	return true;
};
void BSharpTree::BuscarInsertarOEliminar(Nodo* &hoja,std::list<Referencia>&listaDePadres,NodoIntermedio* nodo,const Clave* clave,Referencia& refHoja,bool& esRaiz,bool esInsertar){
	int nivel;
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	Referencia refAux = nodo->bucarReferenciaAsiguienteNodo(clave);
	char array2[tamanioNodo];
	archivoArbol.seekg(refAux);
	buf.pubsetbuf(array2,tamanioNodo);
	archivoArbol.read(array2,tamanioNodo);
	buf.pubseekpos(0);
	buf.sgetn((char*)&nivel,sizeof(int));
	if(!esRaiz){
		delete nodo;
	}else{ esRaiz = false;}
	if(nivel!=0){//es nodo intermedio
		NodoIntermedio* aux = new NodoIntermedio(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
		modificarLista(listaDePadres,esInsertar,aux);
		listaDePadres.push_front(refAux);/*el ultimo lo pongo delante siempre*/
		BuscarInsertarOEliminar(hoja,listaDePadres,aux,clave,refHoja,esRaiz,esInsertar);
	}else{
		hoja =  new NodoHoja(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
		refHoja = refAux;
		return;
	}
	return;
};
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
};
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
};
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
};
void BSharpTree::grabarUnitario(Nodo* nodo,Referencia ref){
	std::stringbuf buff(ios_base :: in | ios_base :: out | ios_base :: binary);
	char array2[tamanioNodo];
	archivoArbol.seekp(ref);
	buff.pubsetbuf(array2,tamanioNodo);
	buff.pubseekpos(0);
	nodo->serializate(&buff);
	archivoArbol.write(array2,tamanioNodo);
};
/*
 * Nodo se encuentra en sobre flujo.
 */
void BSharpTree::resolverDesborde(Nodo* nodo,std::list<Referencia>&listaDePadres,Referencia refHijo){
	std::stringbuf buff(ios_base :: in | ios_base :: out | ios_base :: binary);
	bool izq;//indica si el hermano es el Izq
	bool balancear;//indica si se debe balancear o si se debe dividir
	Nodo* hermano;
	Referencia refHermano;
	ElementoNodo* elementoPadre;//es el elemento del padre q separa los nodos hermanos q se dividen o valancean
	//Obtengo el padre del nodo desbordado
	archivoArbol.seekg(listaDePadres.front());
	char array2[tamanioNodo];
	buff.pubsetbuf(array2,tamanioNodo);
	archivoArbol.read(array2,tamanioNodo);
	NodoIntermedio* padre = new NodoIntermedio(&buff,numeroDeElementosXnodo,comparador,claveEstructural);
	//primero obtengo si se balancea y si es con hermano derecho o izq
	buscarNodoBalancearODividir(padre,nodo,hermano,refHijo,izq,balancear,refHermano,elementoPadre);
	if(balancear){
		nodo->balanceo(hermano,padre,izq);
		grabado(nodo,hermano,padre,refHijo,refHermano,listaDePadres.front());
		return;
	}else{
		ElementoNodo* elementoAagregarEnPadre;
		/*Busco posicion nuevo nodo*/
		Referencia refMedio = buscarEspacioLibre();
		if(refMedio==0){
			archivoArbol.seekp(0,fstream::end);
			refMedio = archivoArbol.tellp();
		}
		Nodo* nodoIzq;
		Nodo* nodoMedio;
		Nodo* nodoDer;
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
		 int desborde = padre->agregarElemento(elementoAagregarEnPadre);
		grabarUnitario(padre,listaDePadres.front());
		    if(desborde==2){
		 	   refHijo = listaDePadres.front();
		 	   listaDePadres.pop_front();
		 		if(!listaDePadres.empty()){
		 			resolverDesborde(padre,listaDePadres,refHijo);
		 		}else{
		 			desbordeRaiz();
		 			return;
		 			}
		    }
		 }

};
void BSharpTree::buscarNodoBalancearODividir(NodoIntermedio* padre,Nodo* hijo,Nodo*& hermano,Referencia refHijo,bool& izq,bool& balancear,Referencia& refHermano,ElementoNodo*& elemPadre){
	bool encontrado = false;
	//Busco si el nodo hijo esta en alguno de los extremos
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
		//busco hermano izq
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
        obtenerHermano(refHermano,hermano,hijo->getNumeroNivel(),balancear);
        if(!balancear){//busco hermano derecho
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
};
void BSharpTree::obtenerHermano(Referencia ref,Nodo*& hermano,unsigned int nivel,bool&balancear){
	std::stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
	char array2[tamanioNodo];
	archivoArbol.seekg(ref);
	buffer.pubsetbuf(array2,tamanioNodo);
	archivoArbol.read(array2,tamanioNodo);
	if(nivel==0){
	   hermano = new NodoHoja(&buffer,numeroDeElementosXnodo,comparador,claveEstructural);
	}else{ hermano = new NodoIntermedio(&buffer,numeroDeElementosXnodo,comparador,claveEstructural);}
	if(hermano->getEspacioLibre()==0){balancear = false;
	   }else{
		   balancear=true;
     }
};
void BSharpTree::desbordeRaiz(){
	std::stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
	int cantIzq = (numeroDeElementosXnodo*2)/3;
    ElementoNodo* elementoRaiz;
	NodoIntermedio* HijoIzq = new NodoIntermedio(Raiz->getNumeroNivel(),numeroDeElementosXnodo,comparador);
	NodoIntermedio* HijoDer = new NodoIntermedio(Raiz->getNumeroNivel(),numeroDeElementosXnodo,comparador);
	std::list<ElementoNodo*>::iterator it = Raiz->getListaElementos()->begin();
	while(cantIzq>0&&it!=Raiz->getListaElementos()->end()){
		ElementoNodo* elem = *it;
		HijoIzq->agregarElemento(elem);
		cantIzq=cantIzq-1;
		++it;
	}
	elementoRaiz = *it;
	++it;
	while(it != Raiz->getListaElementos()->end()){
		ElementoNodo* elem2 = *it;
		HijoDer->agregarElemento(elem2);
		++it;
	}
	HijoIzq->setRefereciaIzq(Raiz->getReferenciaIzq());
	HijoDer->setRefereciaIzq(elementoRaiz->getReferencia());
	archivoArbol.seekp(0,fstream::end);
    Raiz->setRefereciaIzq((Referencia)archivoArbol.tellp());
    grabarUnitario(HijoIzq,archivoArbol.tellp());
    archivoArbol.seekp(0,fstream::end);
    elementoRaiz->setReferencia((Referencia)archivoArbol.tellp());
    grabarUnitario(HijoDer,archivoArbol.tellp());
    Raiz->getListaElementos()->clear();
    Raiz->agregarElemento(elementoRaiz);
    Raiz->setNumeroNivel(Raiz->getNumeroNivel()+1);
    grabarUnitario(Raiz,posicionRaiz);
};
bool BSharpTree::eliminar(const Clave* clave){
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	Referencia refHoja;
	Nodo* hoja;
	bool esRaiz=true;
	NodoIntermedio* nodo = Raiz;
	/*guarda las referencias de los padres de la hoja, pero no todas sino hasta el padre q al menos
	 * tiene mas de un nodo del minimo. (esto no quita q pueda ser la Raiz o ni siquiera la raiz)*/
	std::list<Referencia>listaDePadres;
	listaDePadres.push_back(posicionRaiz);
	BuscarInsertarOEliminar(hoja,listaDePadres,nodo,clave,refHoja,esRaiz,false);
	int subFlujo = hoja->Eliminar(clave);
	eliminarClaveEnIntermedio(clave,hoja->getListaElementos()->front()->getClave());
	if(subFlujo==0){return false;}
	if(subFlujo==1){
		grabarUnitario(hoja,refHoja);
	    }else{
	    	resolverSubflujo(hoja,listaDePadres,refHoja);
	}
	listaDePadres.clear();
	return true;
};

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
	Nodo* hermanoIzq;
	Nodo* hermanoDer;
	Referencia refHermanoIzq;
	Referencia refHermanoDer;
	if(refHijo == posicionRaiz){return;}//La raiz tiene permitido estar en subflujo, es mas tene solo un elemento
	archivoArbol.seekg(listaDePadres.front());//listaDePadre.front() tiene la ref al padre
	char array2[tamanioNodo];
	buff.pubsetbuf(array2,tamanioNodo);
	archivoArbol.read(array2,tamanioNodo);
	NodoIntermedio* padre = new NodoIntermedio(&buff,numeroDeElementosXnodo,comparador,claveEstructural);
	buscarHermanos(nodo,padre,hermanoIzq,hermanoDer,refHermanoIzq,refHermanoDer,refHijo,booleanoInformacion);
	if(booleanoInformacion[0]){
		hermanoIzq->balanceo(nodo,padre,false);
		grabado(nodo,hermanoIzq,padre,refHijo,refHermanoIzq,listaDePadres.front());
	}else if(booleanoInformacion[1]){
	    hermanoDer->balanceo(nodo,padre,true);
	    grabado(nodo,hermanoDer,padre,refHijo,refHermanoDer,listaDePadres.front());
	}else if(booleanoInformacion[2]){
		int subflujo = nodo->unirse(hermanoIzq,hermanoDer,padre);
		resolverReferenciaSiguiente(hermanoIzq,refHermanoDer);
		nuevoEspacioLibre(refHijo);
		if(subflujo==1){
		grabado(hermanoIzq,hermanoDer,padre,refHermanoIzq,refHermanoDer,listaDePadres.front());
		}else{
			grabar(hermanoIzq,hermanoDer,refHermanoIzq,refHermanoDer);
			refHijo = listaDePadres.front();
			listaDePadres.pop_front();
			destruirNodos(nodo,hermanoDer,hermanoIzq);
			resolverSubflujo(padre,listaDePadres,refHijo);
		}
	}else if(booleanoInformacion[3]){
		if(hermanoIzq->getEspacioLibre()>cantidadMinimaDeElementos){
			nodo->balanceo(hermanoIzq,padre,false);
			grabado(nodo,hermanoIzq,padre,refHijo,refHermanoIzq,listaDePadres.front());
		}
		/*en esta situacion extrema el nodo izq no es tal, sino q es el nodo medio*/
		if(hermanoDer->getEspacioLibre()>cantidadMinimaDeElementos){
			nodo->balanceoEspecial(hermanoIzq,hermanoDer,padre,false);
			grabado(nodo,hermanoIzq,hermanoDer,refHijo,refHermanoIzq,refHermanoDer);
			grabarUnitario(padre,listaDePadres.front());
		}else{
			int subflujo = hermanoIzq->unirse(nodo,hermanoDer,padre);
			resolverReferenciaSiguiente(nodo,refHermanoDer);
			nuevoEspacioLibre(refHermanoIzq);
			if(subflujo==1){
				grabado(nodo,hermanoDer,padre,refHijo,refHermanoDer,listaDePadres.front());
				}else{
					grabar(nodo,hermanoDer,refHijo,refHermanoDer);
					refHijo = listaDePadres.front();
					listaDePadres.pop_front();
					destruirNodos(nodo,hermanoDer,hermanoIzq);
					resolverSubflujo(padre,listaDePadres,refHijo);}
		}
	}else if(booleanoInformacion[4]){
		/*en esta situacion extrema el nodo izq no es tal, sino q es el nodo medio*/
		if(hermanoIzq->getEspacioLibre()>cantidadMinimaDeElementos){
				nodo->balanceo(hermanoIzq,padre,true);
				grabado(nodo,hermanoIzq,padre,refHijo,refHermanoIzq,listaDePadres.front());
		}else if(hermanoIzq->getEspacioLibre()>cantidadMinimaDeElementos){
			nodo->balanceoEspecial(hermanoDer,hermanoIzq,padre,true);
			grabado(nodo,hermanoIzq,hermanoDer,refHijo,refHermanoIzq,refHermanoDer);
			grabarUnitario(padre,listaDePadres.front());
		}else{
			int subflujo = hermanoDer->unirse(hermanoIzq,nodo,padre);
			resolverReferenciaSiguiente(hermanoIzq,refHijo);
			nuevoEspacioLibre(refHermanoDer);
			if(subflujo==1){
			   grabado(nodo,hermanoIzq,padre,refHijo,refHermanoIzq,listaDePadres.front());
			}else{
				grabar(nodo,hermanoIzq,refHijo,refHermanoIzq);
			    refHijo = listaDePadres.front();
				listaDePadres.pop_front();
				destruirNodos(nodo,hermanoDer,hermanoIzq);
				resolverSubflujo(padre,listaDePadres,refHijo);}
			}
	}else{/*en esta situacion el hermano sea derecho o izq se carga en hermano izq*/
		if(nodo->getNumeroNivel()>0){subflujoHijosRaiz(nodo,hermanoIzq,refHijo,refHermanoIzq);}
	}
};
void BSharpTree::destruirNodos(Nodo* nodo,Nodo* hermanoDer,Nodo* hermanoIzq){
	delete nodo;
	delete hermanoDer;
	delete hermanoIzq;
};

void BSharpTree::buscarHermanos(Nodo* nodoActual,NodoIntermedio* padre,Nodo* hermanoIzq,Nodo* hermanoDer,Referencia& refHermanoIzq,Referencia& refHermanoDer,Referencia refHijo,vector<bool> &informacion){
 bool extremo = false;/*extremo solo se usa para 3 y 4.*/
	if(refHijo==padre->getReferenciaIzq()){
		/*esto indica q es hermano extrema izq, como concecuencia hermano Izq tendra el nodo intermedio*/
			informacion[3]=true;
			/*refHermanoIzq seria el hermano derecho pegado (nodo medio)*/
		}
	if(refHijo == padre->getListaElementos()->back()->getReferencia()){
		informacion[4]=true;
		extremo = true;
	}
	if(informacion[3]||informacion[4]){
		refHermanoIzq = obtenerReferenciaHermano(padre,*(nodoActual->getListaElementos()->front()->getClave()),extremo);
		hermanoIzq =  obtenerHermanoXsuBflujo(nodoActual->getNumeroNivel(),refHermanoIzq);
		refHermanoDer = obtenerReferenciaHermano(padre,*(hermanoIzq->getListaElementos()->front()->getClave()),extremo);
		hermanoDer = obtenerHermanoXsuBflujo(nodoActual->getNumeroNivel(),refHermanoDer);
		return;
	}
	if((Raiz->getListaElementos()->size()==1)&& ((Raiz->getReferenciaIzq()==refHijo)||(Raiz->getListaElementos()->front()->getReferencia()==refHijo))){
		informacion[5]=true;
		/*solo tiene un hermano*/
		if((Raiz->getReferenciaIzq()==refHijo)){refHermanoIzq = padre->getListaElementos()->front()->getReferencia();}
		else{refHermanoIzq = padre->getReferenciaIzq();}
		hermanoIzq = obtenerHermanoXsuBflujo(nodoActual->getNumeroNivel(),refHermanoIzq);
	}
	if(!informacion[3]&&!informacion[4]&&!informacion[5]){
		refHermanoIzq = obtenerReferenciaHermano(padre,*(nodoActual->getListaElementos()->front()->getClave()),true);
		hermanoIzq = obtenerHermanoXsuBflujo(nodoActual->getNumeroNivel(),refHermanoIzq);
		if((numeroDeElementosXnodo-hermanoIzq->getEspacioLibre()) > cantidadMinimaDeElementos){
			informacion[0]=true;
			return;
		}else{
		refHermanoDer = obtenerReferenciaHermano(padre,*(nodoActual->getListaElementos()->front()->getClave()),false);
		hermanoDer = obtenerHermanoXsuBflujo(nodoActual->getNumeroNivel(),refHermanoIzq);
		if((numeroDeElementosXnodo-hermanoDer->getEspacioLibre()) > cantidadMinimaDeElementos){
			informacion[1]=true;
			return;
		}else{
			informacion[2]=true;
			return;
		  }
		}
	}
};
Referencia BSharpTree::obtenerReferenciaHermano(Nodo* padre,Clave clave,bool Izq){
	std::list<ElementoNodo*>::iterator itPadre = padre->getListaElementos()->begin();
		bool encontrado=false;
		while(!encontrado && itPadre != padre->getListaElementos()->end()){
			ElementoNodo* elemPadre = *itPadre;
			if((comparador->Comparar(elemPadre->getClave(),&clave)==0)||(elemPadre->getClave(),&clave)<0){
				encontrado  = true;
				if(Izq){--itPadre;
				}else{++itPadre;}
				ElementoNodo* elemHermano = *itPadre;
				return elemHermano->getReferencia();
			}
		  ++itPadre;
	}
    return NULL;
};
Nodo* BSharpTree::obtenerHermanoXsuBflujo(int nivel,Referencia ref){
      archivoArbol.seekg(ref);
      std::stringbuf buff(ios_base :: in | ios_base :: out | ios_base :: binary);
      buff.pubseekpos(0);
      char array2[tamanioNodo];
      buff.pubsetbuf(array2,tamanioNodo);
      archivoArbol.read(array2,tamanioNodo);
      if(nivel==0){
    	  NodoHoja* nodo = new NodoHoja(&buff,numeroDeElementosXnodo,comparador,claveEstructural);
    	  return nodo;
      }
      NodoIntermedio* nodo = new NodoIntermedio(&buff,numeroDeElementosXnodo,comparador,claveEstructural);
      return nodo;
};
void BSharpTree::resolverReferenciaSiguiente(Nodo* nodoIzq,Referencia refAHermanoNuevo){
	if(nodoIzq->getNumeroNivel()==0){
		NodoHoja* nodoIzquierdo = dynamic_cast<NodoHoja*>(nodoIzq);
		nodoIzquierdo->setReferenciaSiguiente(refAHermanoNuevo);
	}
};
void BSharpTree::eliminarClaveEnIntermedio(const Clave* claveAeliminar,Clave* claveSetear){
	NodoIntermedio* nodoConClave = buscarIntermedio(claveAeliminar,Raiz);
	if(nodoConClave==NULL){return;}
	nodoConClave->setearClave(claveAeliminar,claveSetear);
};
NodoIntermedio* BSharpTree::buscarIntermedio(const Clave* clave,NodoIntermedio* nodo){
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	if(nodo->estaClave(clave)){return nodo;
	}else{
		delete nodo;
		Referencia refAux = nodo->bucarReferenciaAsiguienteNodo(clave);
		buf.pubseekpos(0);
		archivoArbol.seekg(refAux);
		char array2[tamanioNodo];
		buf.pubsetbuf(array2,tamanioNodo);
		archivoArbol.read(array2,tamanioNodo);
		buf.pubseekpos(0);
		int nivel;
		buf.sgetn((char*)&nivel,sizeof(int));
		if(nivel==0){return NULL;}
		else{
			nodo = new NodoIntermedio(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
			return buscarIntermedio(clave,nodo);
		}
	}
	return NULL;
};
/*
 * Si se llama a este metodo es pq la Raiz solo tiene dos hijos (deben ser intermedio), como concecuecia un solo elemento,
 *  y uno de sus hijos esta en subflujo sin poder balancearce. en caso de q el subflujo
 * sea justo igual al minimo menos 1 no se hace nada, peo si es inferior se genera entre la raiz y los hijos
 * un solo nodo q pasa a ser la nueva raiz.
 */
void BSharpTree::subflujoHijosRaiz(Nodo* nodo,Nodo* hermano,Referencia refNodo,Referencia refHermano){
	if((nodo->getEspacioLibre()<(cantidadMinimaDeElementos-2))||(hermano->getEspacioLibre()<(cantidadMinimaDeElementos-2))||((nodo->getEspacioLibre()<cantidadMinimaDeElementos)&&(hermano->getEspacioLibre()<cantidadMinimaDeElementos))){
		if(comparador->Comparar(nodo->getListaElementos()->front()->getClave(),hermano->getListaElementos()->back()->getClave())>0){
			armarNuevaRaiz(hermano,nodo);
		}else{
			armarNuevaRaiz(nodo,hermano);
		}
	  nuevoEspacioLibre(refNodo);
	  nuevoEspacioLibre(refHermano);
	}
};
void BSharpTree::armarNuevaRaiz(Nodo* nodoIzq,Nodo* nodoDer){
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	NodoIntermedio* nodoIzquierdo = dynamic_cast<NodoIntermedio*>(nodoIzq);
	NodoIntermedio* nodoDerecho = dynamic_cast<NodoIntermedio*>(nodoDerecho);
	Raiz->getListaElementos()->front()->setReferencia(nodoDerecho->getReferenciaIzq());
	Raiz->setRefereciaIzq(nodoIzquierdo->getReferenciaIzq());
	for(std::list<ElementoNodo*>::reverse_iterator it = nodoIzquierdo->getListaElementos()->rbegin();it != nodoIzquierdo->getListaElementos()->rend();++it){
		Raiz->agregarElemento(*it);
	}
	for(std::list<ElementoNodo*>::iterator it = nodoIzquierdo->getListaElementos()->begin();it != nodoIzquierdo->getListaElementos()->end();++it){
		Raiz->agregarElemento(*it);
	}
	Raiz->setNumeroNivel(Raiz->getNumeroNivel()-1);
	grabarUnitario(Raiz,posicionRaiz);
	delete nodoIzquierdo;
	delete nodoDerecho;
}
void BSharpTree::nuevoEspacioLibre(Referencia ref){
	std::stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
	buffer.pubseekpos(0);
	archivoEspaciosLibres.seekg(0);
	int cantElem;
	int tamanio = sizeof(Referencia);
	char array[tamanio];
	buffer.pubsetbuf(array,tamanio);
	archivoArbol.read(array,tamanio);
	buffer.pubseekpos(0);
	buffer.sgetn((char*)&cantElem,sizeof(int));
	int pos = sizeof(int)+ (cantElem*sizeof(Referencia));
	archivoEspaciosLibres.seekp(pos);
	buffer.pubseekpos(0);
	buffer.sputn((char*)&ref,sizeof(Referencia));
	archivoArbol.write(array,tamanio);
	cantElem++;
	buffer.pubseekpos(0);
	buffer.sputn((char*)&cantElem,sizeof(int));
	archivoEspaciosLibres.seekp(0);
	archivoArbol.write(array,tamanio);
}
Referencia BSharpTree::buscarEspacioLibre(){
	std::stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
	buffer.pubseekpos(0);
	archivoEspaciosLibres.seekg(0);
	int cantElem;
	int tamanio = sizeof(int);
	char array[tamanio];
	buffer.pubsetbuf(array,tamanio);
	archivoArbol.read(array,tamanio);
	buffer.pubseekpos(0);
	buffer.sgetn((char*)&cantElem,sizeof(int));
	if(cantElem == 0)return 0;
	cantElem--;
	int pos = sizeof(int)+ (sizeof(Referencia)*(cantElem));
	Referencia retorno;
	char array3[sizeof(Referencia)];
	buffer.pubsetbuf(array3,sizeof(Referencia));
	archivoEspaciosLibres.seekg(pos);
	archivoArbol.read(array3,sizeof(Referencia));
	buffer.pubseekpos(0);
	buffer.sputn((char*)&retorno,sizeof(Referencia));
	buffer.pubsetbuf(array,tamanio);
	buffer.pubseekpos(0);
	archivoEspaciosLibres.seekp(0);
	archivoArbol.write(array,tamanio);
	return retorno;
};
Nodo* BSharpTree::obtenerNodoPorPosiciones(int posInicial){
	std::stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
	buffer.pubseekpos(0);
	archivoArbol.seekp(posInicial);
	char array2[tamanioNodo];
	buffer.pubsetbuf(array2,tamanioNodo);
	archivoArbol.read(array2,tamanioNodo);
	return NULL;
};
BSharpTree::~BSharpTree() {
  delete Raiz;
  archivoArbol.close();
  archivoEspaciosLibres.close();
};

