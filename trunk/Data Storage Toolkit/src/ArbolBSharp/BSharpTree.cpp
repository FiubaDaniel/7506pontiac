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
		posicionRaiz = 3*sizeof(int);
		buffer.sputn ((char*)&posicionRaiz,sizeof(posicionRaiz));
		archivoArbol.write(array,tamanio);
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
		buffer.sputn((char*)&cero,sizeof(cero));
		buffer.pubseekpos(0);
		archivoEspaciosLibres.write(array3,sizeof(cero));
	}
}

bool BSharpTree::abrir(string nombreArch,ComparadorClave* comp){
	comparador = comp;
	nombreArchivo = nombreArch+"_Arbol";
    nombreEspaciosLibres = nombreArch+"_EspaciosLibre";
	archivoArbol.seekg(0);
	archivoEspaciosLibres.open(nombreEspaciosLibres.c_str(),std::fstream::out|std::fstream::in|std::fstream::binary);
	archivoArbol.open(nombreArchivo.c_str(),std::fstream::out|std::fstream::in|std::fstream::binary);
	if(!archivoArbol.is_open()||!archivoEspaciosLibres.is_open()){
		return false;
	}
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
    return true;
}

int BSharpTree::calcularCantidadElementosPorNodo(unsigned int tamSerializadoClave){
	 unsigned int resultado = tamanioNodo - 2 * sizeof(int) - sizeof(Referencia);
	 int tamElemento = sizeof(Referencia)+tamSerializadoClave;
	 resultado = (int)resultado/tamElemento;
	 if (resultado<1){
		throw  TamanioInsuficienteException();
	 }else return resultado;
}

bool BSharpTree::Buscar(const Clave* clave,Referencia* referencia){
	Referencia referenciaDeNodoHoja;
	Clave* aux = const_cast<Clave*>(clave);
	NodoHoja* hoja = buscarHoja(Raiz,aux,referenciaDeNodoHoja);
	return hoja->buscarReferenciaDeClaveX(clave,referencia);
}

NodoHoja* BSharpTree::buscarPrimerNodoHoja(NodoIntermedio* nodo){
    Referencia ref;
    int nivel;
    std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	if(nodo->getReferenciaIzq()!=0){
    	 ref = nodo->getReferenciaIzq();
     }else{
    	 ElementoNodo* elemento = nodo->getListaElementos()->front();
    	 ref = elemento->getReferencia();
     }
	char array2[tamanioNodo];
	buf.pubseekpos(0);
	buf.pubsetbuf(array2,tamanioNodo);
	archivoArbol.seekg(ref);
	archivoArbol.read(array2,tamanioNodo);
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

NodoHoja* BSharpTree::buscarHoja(NodoIntermedio* nodo,Clave* clave,Referencia& referenciaDeNodoHoja){
	int nivel;
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	Referencia refAux = nodo->bucarReferenciaAsiguienteNodo(clave);
	char array2[tamanioNodo];
	buf.pubseekpos(0);
	archivoArbol.seekg(refAux);
	buf.pubsetbuf(array2,tamanioNodo);
	archivoArbol.read(array2,tamanioNodo);
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
			buf.pubsetbuf(array2,tamanioNodo);
			archivoArbol.seekg(ultimoNodo->getReferenciaSiguiente());
			archivoArbol.read(array2,tamanioNodo);
			delete ultimoNodo;
			ultimoNodo = new NodoHoja(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
		}
	}
	ElementoNodo* elemento = ultimoNodo->getPos();
	return elemento->getReferencia();
}

bool BSharpTree::modificar(const Clave* clave,Referencia refNueva){
    Referencia referenciaDeNodoHoja;//
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
		delete Raiz;
		recomponerRaiz();
	}
	listaDePadres.clear();
	delete hoja;
	delete elemento;
	return true;
}

void BSharpTree::BuscarInsertarOEliminar(Nodo* &hoja,std::list<Referencia>&listaDePadres,NodoIntermedio* nodo,Clave* clave,Referencia& refHoja,bool& esRaiz,bool esInsertar){
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
	buffer.pubsetbuf(array2,tamanioNodo);
	archivoArbol.read(array2,tamanioNodo);
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
    ElementoNodo* elementoRaiz;
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
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	Referencia refHoja;
	Nodo* hoja;
	bool esRaiz=true;
	Clave* clave = const_cast<Clave*>(claveE);
	std::list<Referencia>listaDePadres;
	listaDePadres.push_back(posicionRaiz);
	BuscarInsertarOEliminar(hoja,listaDePadres,Raiz,clave,refHoja,esRaiz,false);
	int subFlujo = hoja->Eliminar(clave);
	eliminarClaveEnIntermedio(clave,hoja->getListaElementos()->front()->getClave());
	if(subFlujo==0){return false;}
	if(subFlujo==1){
		grabarUnitario(hoja,refHoja);
	    }else{
	    	resolverSubflujo(hoja,listaDePadres,refHoja);
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
	Nodo* hermanoIzq;
	Nodo* hermanoDer;
	Referencia refHermanoIzq;
	Referencia refHermanoDer;
	if(refHijo == posicionRaiz){return;}//La raiz tiene permitido estar en subflujo, es mas tener solo un elemento
	archivoArbol.seekg(listaDePadres.front());//listaDePadre.front() tiene la ref al padre
	char array2[tamanioNodo];
	buff.pubsetbuf(array2,tamanioNodo);
	archivoArbol.read(array2,tamanioNodo);
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
		if(nodo->getNumeroNivel()>0){subflujoHijosRaiz(nodo,hermanoIzq,refHijo,refHermanoIzq);}
	}
}

void BSharpTree::destruirNodos(Nodo* nodo,Nodo* hermanoDer,Nodo* hermanoIzq){
	delete nodo;
	delete hermanoDer;
	delete hermanoIzq;
}

void BSharpTree::buscarHermanos(Nodo* nodoActual,NodoIntermedio* padre,Nodo* &hermanoIzq,Nodo* &hermanoDer,Referencia& refHermanoIzq,Referencia& refHermanoDer,Referencia refHijo,vector<bool> &informacion){
 bool extremo = false;
	if(refHijo==padre->getReferenciaIzq()){
			informacion[3]=true;
		}
	if(refHijo == padre->getListaElementos()->back()->getReferencia()){
		informacion[4]=true;
		extremo = true;
	}
	if((informacion[3]||informacion[4])&&Raiz->getListaElementos()->size()!=1){
		refHermanoIzq = obtenerReferenciaHermano(padre,nodoActual->getListaElementos()->front()->getClave(),extremo);
		hermanoIzq =  obtenerHermanoXsuBflujo(nodoActual->getNumeroNivel(),refHermanoIzq);
		refHermanoDer = obtenerReferenciaHermano(padre,hermanoIzq->getListaElementos()->front()->getClave(),extremo);
		hermanoDer = obtenerHermanoXsuBflujo(nodoActual->getNumeroNivel(),refHermanoDer);
		return;
	}
	if((Raiz->getListaElementos()->size()==1)&& ((Raiz->getReferenciaIzq()==refHijo)||(Raiz->getListaElementos()->front()->getReferencia()==refHijo))){
		informacion[5]=true;
		if((Raiz->getReferenciaIzq()==refHijo)){refHermanoIzq = padre->getListaElementos()->front()->getReferencia();}
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
	ElementoNodo* elemPadre;
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

Nodo* BSharpTree::obtenerHermanoXsuBflujo(int nivel,Referencia ref){
      archivoArbol.seekg(ref);
      std::stringbuf buff(ios_base :: in | ios_base :: out | ios_base :: binary);
      buff.pubseekpos(0);
      char array2[tamanioNodo];
      buff.pubsetbuf(array2,tamanioNodo);
      archivoArbol.seekg(ref);
      archivoArbol.read(array2,tamanioNodo);
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
	Referencia refNodoModificado;
	NodoIntermedio* nodoConClave = buscarIntermedio(claveAeliminar,Raiz,true,refNodoModificado);
	if(nodoConClave==NULL){return;}
	nodoConClave->setearClave(claveAeliminar,claveSetear);
	grabarUnitario(nodoConClave,refNodoModificado);
}

NodoIntermedio* BSharpTree::buscarIntermedio(Clave* clave,NodoIntermedio* nodo,bool esRaiz,Referencia& refAux){
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	if(nodo->estaClave(clave)){return nodo;
	}else{
		refAux = nodo->bucarReferenciaAsiguienteNodo(clave);
		if(!esRaiz){
		delete nodo;}else{esRaiz = false;}
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
}

void BSharpTree::armarNuevaRaiz(Nodo* nodoIzq,Nodo* nodoDer){
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	NodoIntermedio* nodoIzquierdo = dynamic_cast<NodoIntermedio*>(nodoIzq);
	NodoIntermedio* nodoDerecho = dynamic_cast<NodoIntermedio*>(nodoDer);
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

void BSharpTree::recomponerRaiz(){
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	char arrayRaiz[tamanioNodo];
	buf.pubsetbuf(arrayRaiz,tamanioNodo);
	archivoArbol.seekg(posicionRaiz);
	archivoArbol.read(arrayRaiz,tamanioNodo);
	Raiz = new NodoIntermedio(&buf,numeroDeElementosXnodo,comparador,claveEstructural);
}

void BSharpTree::nuevoEspacioLibre(Referencia ref){
	std::stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
	buffer.pubseekpos(0);
	archivoEspaciosLibres.seekg(0);
	int cantElem;
	int tamanio = sizeof(Referencia);
	char array[tamanio];
	buffer.pubsetbuf(array,tamanio);
	archivoEspaciosLibres.read(array,tamanio);
	buffer.pubseekpos(0);
	buffer.sgetn((char*)&cantElem,sizeof(int));
	int pos = sizeof(int)+ (cantElem*sizeof(Referencia));
	archivoEspaciosLibres.seekp(pos);
	buffer.pubseekpos(0);
	buffer.sputn((char*)&ref,sizeof(Referencia));
	archivoEspaciosLibres.write(array,tamanio);
	cantElem++;
	buffer.pubseekpos(0);
	buffer.sputn((char*)&cantElem,sizeof(int));
	archivoEspaciosLibres.seekp(0);
	archivoEspaciosLibres.write(array,tamanio);
}

Referencia BSharpTree::buscarEspacioLibre(){
	std::stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
	buffer.pubseekpos(0);
	archivoEspaciosLibres.seekg(0);
	int cantElem;
	int tamanio = sizeof(int);
	char array[tamanio];
	buffer.pubsetbuf(array,tamanio);
	archivoEspaciosLibres.read(array,tamanio);
	buffer.pubseekpos(0);
	buffer.sgetn((char*)&cantElem,sizeof(int));
	if(cantElem == 0)return 0;
	cantElem--;
	int pos = sizeof(int)+ (sizeof(Referencia)*(cantElem));
	Referencia retorno;
	char array3[sizeof(Referencia)];
	buffer.pubsetbuf(array3,sizeof(Referencia));
	archivoEspaciosLibres.seekg(pos);
	archivoEspaciosLibres.read(array3,sizeof(Referencia));
	buffer.pubseekpos(0);
	buffer.sputn((char*)&retorno,sizeof(Referencia));
	buffer.pubsetbuf(array,tamanio);
	buffer.pubseekpos(0);
	archivoEspaciosLibres.seekp(0);
	archivoEspaciosLibres.write(array,tamanio);
	return retorno;
}

Nodo* BSharpTree::obtenerNodoPorPosiciones(Referencia posInicial){
	std::stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
	buffer.pubseekpos(0);
	Nodo* nodo;
	archivoArbol.seekp(posInicial);
	char array2[tamanioNodo];
	buffer.pubsetbuf(array2,tamanioNodo);
	archivoArbol.read(array2,tamanioNodo);
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
    cout<<"Numero de Registros por nodo: "<< numeroDeElementosXnodo <<endl;;
    cout<<"Tamanio de los nodos: "<< tamanioNodo <<endl;
    cout<<"posicion de la raiz: "<<posicionRaiz<<endl;
	cout<<" "<<endl;
	imprimirIterativo(Raiz,Raiz->getNumeroNivel());
}

void BSharpTree::imprimirIterativo(Nodo* nodoE,unsigned int nivelRaiz){
	if(nodoE->getNumeroNivel()!=0){
		NodoIntermedio* nodo = dynamic_cast<NodoIntermedio*>(nodoE);
	    cout <<" Nodo Intermedio: ";
		cout <<" Nivel: " << nodo->getNumeroNivel();
	    cout <<" Referencia Izquierda: " << nodo->getReferenciaIzq();
	    cout <<" Elementos: ";
	    std::list<ElementoNodo*>::iterator it = nodo->getListaElementos()->begin();
	    while(it!=nodo->getListaElementos()->end()){
		     ElementoNodo* elem = *it;
		     for(unsigned int i = 0; i<elem->getClave()->getCantidadAtributos();i++){
			        cout<<" ";
			        elem->getClave()->getAtributo(i)->imprimir(cout);
			        cout<<" ";
		        }
		      cout << elem->getReferencia();
		      cout<<" "<<endl;
		      ++it;
	     }
	    Nodo* nodoSiguiente = obtenerNodoPorPosiciones(nodo->getReferenciaIzq());
	    imprimirIterativo(nodoSiguiente,nivelRaiz);
	    delete nodoSiguiente;
	    it = nodo->getListaElementos()->begin();
	    while(it!=nodo->getListaElementos()->end()){
	    	ElementoNodo* elem = *it;
	    	nodoSiguiente = obtenerNodoPorPosiciones(elem->getReferencia());
	    	imprimirIterativo(nodoSiguiente,nivelRaiz);
	    	delete nodoSiguiente;
	    	++it;
	    }
	}else{
		NodoHoja* nodo = dynamic_cast<NodoHoja*>(nodoE);
		cout <<" Nodo Hoja: ";
		cout <<" Nivel: " << nodo->getNumeroNivel();
		cout <<" Referencia Siguente: " << nodo->getReferenciaSiguiente();
		cout <<" Elementos: ";
		std::list<ElementoNodo*>::iterator it = nodo->getListaElementos()->begin();
		while(it!=nodo->getListaElementos()->end()){
			ElementoNodo* elem = *it;
			for(unsigned int i = 0; i<elem->getClave()->getCantidadAtributos();i++){
					cout<<" ";
					elem->getClave()->getAtributo(i)->imprimir(cout);
					cout<<" ";
				}
		cout << elem->getReferencia();
		cout<<" "<<endl;
		++it;
		 }
	}
}

BSharpTree::~BSharpTree() {
  delete Raiz;
  archivoArbol.close();
  archivoEspaciosLibres.close();
}

