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

BSharpTree::BSharpTree(char* nombreArch,char* nomEspaciosLibres,unsigned int tamanioDeBloque, int tamanioSerializadoClave,ComparadorClave* comp) {
	comparador = comp;
	tamanioNodo = tamanioDeBloque;
	numeroDeElementosXnodo = calcularCantidadElementosPorNodo(tamanioSerializadoClave);
	cantidadMinimaDeElementos = (unsigned int) ((numeroDeElementosXnodo)*2)/3;
	strcpy(nomEspaciosLibres,nombreEspaciosLibres);
	archivoEspaciosLibres.open(nombreEspaciosLibres,std::fstream::out);
	strcpy(nombreArch, nombreArchivo);
	archivoArbol.open(nombreArchivo,std::fstream::out);
	if(archivoArbol.is_open()&&archivoEspaciosLibres.is_open()){
		archivoArbol.seekp(0);
		std:: stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
		buffer.pubseekpos(0);
		buffer.sputn ((char*)&numeroDeElementosXnodo,sizeof(numeroDeElementosXnodo));
		buffer.sputn ((char*)&tamanioNodo,sizeof(tamanioNodo));
		posicionRaiz = 3*sizeof(int);
		buffer.sputn ((char*)&posicionRaiz,sizeof(posicionRaiz));
		archivoArbol.write((char*)&buffer,4*sizeof(int));
		/*Establezco la raiz y su referencia izq creando la hoja izq vacia*/
		Raiz = new NodoIntermedio((unsigned int) 1,numeroDeElementosXnodo,comparador);
		Raiz->setRefereciaIzq(posicionRaiz+tamanioNodo+1);
		Raiz->serializate(&buffer);
		archivoArbol.write((char*)&buffer,tamanioNodo);
		archivoArbol.seekp(posicionRaiz+tamanioNodo+1);
		NodoHoja* hoja = new NodoHoja(numeroDeElementosXnodo,0,comparador);
		hoja->serializate(&buffer);
		archivoArbol.write((char*)&buffer,tamanioNodo);
		archivoEspaciosLibres.seekp(0);
		archivoEspaciosLibres.write((char*)0,sizeof(int));
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
	buf.sgetn((char*)&numeroDeElementosXnodo,sizeof(numeroDeElementosXnodo));
	buf.sgetn((char*)&tamanioNodo,sizeof(tamanioNodo));
	buf.sgetn((char*)&posicionRaiz,sizeof(int));
	buf.pubseekpos(0);
	archivoArbol.read((char*)&buf,tamanioNodo);
	Raiz = new NodoIntermedio(&buf,numeroDeElementosXnodo,comparador);
    archivoArbol.close();
    archivoEspaciosLibres.close();
    cantidadMinimaDeElementos = (unsigned int) ((numeroDeElementosXnodo)*2)/3;
};
int BSharpTree::calcularCantidadElementosPorNodo(int tamSerializadoClave){
	 int resultado = tamanioNodo - 2 * sizeof(int) - sizeof(Referencia);
	 int tamElemento = sizeof(Referencia)+sizeof(tamSerializadoClave);
	 resultado = (int)resultado/tamElemento;
	 if (resultado==0){
		throw  TamanioInsuficienteException();
	 }else return resultado;
};
/*
 * Devuelve false en caso de no existir la clave buscada.
 * Almacena el nodo Hoja de donde obtuvo el elemento para una posible busqueda secuencial.
 */
bool BSharpTree::Buscar(Clave clave,Referencia* referencia){
	archivoArbol.open(nombreArchivo,std::fstream::in);
	ultimoNodo->~NodoHoja();
	bool encontrado =  buscarIterativo(*Raiz,clave,referencia,ultimoNodo);
	archivoArbol.close();
    return encontrado;
};
bool BSharpTree::buscarIterativo(NodoIntermedio nodo, Clave clave,Referencia*ref,NodoHoja* ultimo){
	int nivel;
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	Referencia refAux = nodo.bucarReferenciaAsiguienteNodo(clave);
    buf.pubseekpos(0);
    archivoArbol.seekg(refAux);
    archivoArbol.read((char*)&buf,tamanioNodo);
    buf.pubseekpos(0);
    buf.sgetn((char*)&nivel,sizeof(int));
    if(nivel!=0){
       NodoIntermedio nodoAux(&buf,numeroDeElementosXnodo,comparador);
       return buscarIterativo(nodoAux,clave,ref,ultimo);
     }else{
    	 /*ver si esta bien lo de destruir el nodo hoja q tenia antes*/
       delete ultimo;
       ultimo = new NodoHoja(&buf,numeroDeElementosXnodo,comparador);
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
	buf.pubseekpos(0);
	archivoArbol.seekg(ref);
	archivoArbol.read((char*)&buf,tamanioNodo);
	buf.pubseekpos(0);
	buf.sgetn((char*)&nivel,sizeof(int));
	if(nivel!=0){
	NodoIntermedio nodoAux(&buf,numeroDeElementosXnodo,comparador);
	return buscarPrimerNodoHoja(nodoAux);
	     }else{
	    	 NodoHoja* nodoAux = new NodoHoja(&buf,numeroDeElementosXnodo,comparador);
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
bool BSharpTree::modificar(Clave clave,Referencia refNueva){
	/*primero debo buscar el nodo hoja*/
	archivoArbol.open(nombreArchivo,std::fstream::in|std::fstream::out);
	Referencia referenciaDeNodoHoja;//es la referencia donde se debe guardar el nodo en el archivo una vez modificado.
	NodoHoja* nodo = buscarHoja(*Raiz,clave,referenciaDeNodoHoja);
	bool modificado = nodo->setReferenciaDeClaveX(clave,refNueva);
	if(modificado){
		std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
		buf.pubseekpos(0);
		nodo->serializate(&buf);
		archivoArbol.seekp(referenciaDeNodoHoja);
		archivoArbol.write((char*)&buf,tamanioNodo);
	}
	return modificado;
};
NodoHoja* BSharpTree::buscarHoja(NodoIntermedio nodo,Clave clave,Referencia referenciaDeNodoHoja){
	int nivel;
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	Referencia refAux = nodo.bucarReferenciaAsiguienteNodo(clave);
	buf.pubseekpos(0);
	archivoArbol.seekg(refAux);
	archivoArbol.read((char*)&buf,tamanioNodo);
	buf.pubseekpos(0);
	buf.sgetn((char*)&nivel,sizeof(int));
	if(nivel!=0){
		NodoIntermedio nodoAux(&buf,numeroDeElementosXnodo,comparador);
		return buscarHoja(nodoAux,clave,referenciaDeNodoHoja);
	}else{
		NodoHoja* nodoAux = new NodoHoja(&buf,numeroDeElementosXnodo,comparador);
		return nodoAux;
	}
	return NULL;
};
bool BSharpTree::insertar(Referencia ref,Clave* clave){
	archivoArbol.open(nombreArchivo,std::fstream::in|std::fstream::out);
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	ElementoNodo* elemento = new ElementoNodo(ref,clave);
	if(Raiz->getEspacioLibre()==numeroDeElementosXnodo){
		/*creo el nodo derecho hoja e inserto el elemento en ambos, raiz e hijo*/
		archivoArbol.seekp(fstream::end);
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
		archivoArbol.read((char*)&buf,tamanioNodo);
		hoja = new NodoHoja(&buf,numeroDeElementosXnodo,comparador);
		hoja->setReferenciaSiguiente(refAux);
	    grabarUnitario(hoja,Raiz->getReferenciaIzq());
		return true;
	}
	Referencia refHoja;
	Nodo* hoja;
	bool esRaiz=true;
	NodoIntermedio* nodo = Raiz;
	std::list<Referencia>listaDePadres;
	listaDePadres.push_back(posicionRaiz);
	BuscarInsertarOEliminar(hoja,listaDePadres,nodo,*clave,refHoja,esRaiz,true);
	int desborde = hoja->agregarElemento(elemento);
	if(desborde==0)return false;
	if(desborde==1){
		grabarUnitario(hoja,refHoja);
	}else{
		resolverDesborde(hoja,listaDePadres,refHoja);
	}
	listaDePadres.clear();
	archivoArbol.close();
	return true;
};
void BSharpTree::BuscarInsertarOEliminar(Nodo* hoja,std::list<Referencia>&listaDePadres,NodoIntermedio* nodo,Clave clave,Referencia refHoja,bool& esRaiz,bool esInsertar){
	int nivel;
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	Referencia refAux = nodo->bucarReferenciaAsiguienteNodo(clave);
	buf.pubseekpos(0);
	archivoArbol.seekg(refAux);
	archivoArbol.read((char*)&buf,tamanioNodo);
	buf.pubseekpos(0);
	buf.sgetn((char*)&nivel,sizeof(int));
	if(!esRaiz){
		delete nodo;
	}else{ esRaiz = false;}
	if(nivel!=0){//es nodo intermedio
		NodoIntermedio* nodo = new NodoIntermedio(&buf,numeroDeElementosXnodo,comparador);
		modificarLista(listaDePadres,esInsertar,nodo);
		listaDePadres.push_front(refAux);/*el ultimo lo pongo delante siempre*/
		BuscarInsertarOEliminar(hoja,listaDePadres,nodo,clave,refHoja,esRaiz,esInsertar);
	}else{
		hoja =  new NodoHoja(&buf,numeroDeElementosXnodo,comparador);
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
	original->serializate(&buff);
	archivoArbol.seekp(refOriginal);
	buff.pubseekpos(0);
	archivoArbol.write((char*)&buff,tamanioNodo);
	padre->serializate(&buff);
	archivoArbol.seekp(refPadre);
	buff.pubseekpos(0);
	archivoArbol.write((char*)&buff,tamanioNodo);
	hermano->serializate(&buff);
	archivoArbol.seekp(refHermano);
	buff.pubseekpos(0);
	archivoArbol.write((char*)&buff,tamanioNodo);
};
void BSharpTree::grabar(Nodo* nodoOriginal,Nodo* nodoHermano,Referencia refOriginal,Referencia refHermano){
	std::stringbuf buff(ios_base :: in | ios_base :: out | ios_base :: binary);
	nodoOriginal->serializate(&buff);
	archivoArbol.seekp(refOriginal);
	buff.pubseekpos(0);
	archivoArbol.write((char*)&buff,tamanioNodo);
	nodoHermano->serializate(&buff);
	archivoArbol.seekp(refHermano);
	buff.pubseekpos(0);
	archivoArbol.write((char*)&buff,tamanioNodo);
};
void BSharpTree::grabarUnitario(Nodo* nodo,Referencia ref){
	std::stringbuf buff(ios_base :: in | ios_base :: out | ios_base :: binary);
	nodo->serializate(&buff);
	archivoArbol.seekp(ref);
	buff.pubseekpos(0);
	archivoArbol.write((char*)&buff,tamanioNodo);
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
	archivoArbol.read((char*)&buff,tamanioNodo);
	NodoIntermedio* padre = new NodoIntermedio(&buff,numeroDeElementosXnodo,comparador);
	//primero obtengo si se balancea y si es con hermano derecho o izq
	buscarNodoBalancearODividir(*padre,nodo,hermano,refHijo,izq,balancear,refHermano,elementoPadre);
	if(balancear){
		nodo->balanceo(hermano,padre,izq);
		grabado(nodo,hermano,padre,refHijo,refHermano,listaDePadres.front());
		return;
	}else{
		ElementoNodo* elementoAagregarEnPadre;
		/*Busco posicion nuevo nodo*/
		Referencia refMedio = buscarEspacioLibre();
		if(refMedio==0){
			archivoArbol.seekp(fstream::end);
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
		 if(izq){
			 elementoAagregarEnPadre =  hermano->dividirse(nodo,nodoIzq,nodoMedio,nodoDer,padre,*(elementoPadre->getClave()));
			 grabado(nodoIzq,nodoMedio,nodoDer,refHermano,refMedio,refHijo);
		 }else{
			 elementoAagregarEnPadre = nodo->dividirse(hermano,nodoIzq,nodoMedio,nodoDer,padre,*(elementoPadre->getClave()));
			 grabado(nodoIzq,nodoMedio,nodoDer,refHijo,refMedio,refHermano);
		 }
		 int desborde = padre->agregarElemento(elementoAagregarEnPadre);
		    if(desborde==1){
		 	   grabarUnitario(padre,listaDePadres.front());
		 	   return;
		    }else{
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
void BSharpTree::buscarNodoBalancearODividir(NodoIntermedio padre,Nodo* hijo,Nodo* hermano,Referencia& refHijo,bool& izq,bool& balancear,Referencia refHermano,ElementoNodo* elemPadre){
	bool encontrado = false;
	//Busco si el nodo hijo esta en alguno de los extremos
	if(refHijo == padre.getReferenciaIzq() || padre.getListaElementos()->back()->getReferencia() == refHijo){
	if(refHijo == padre.getReferenciaIzq()){
		elemPadre = padre.getListaElementos()->front();
		izq = false;
		refHermano = elemPadre->getReferencia();
	}else{
		elemPadre = padre.getListaElementos()->back();
		izq = true;
		std::list<ElementoNodo*>:: reverse_iterator it = padre.getListaElementos()->rbegin();
		++it;
		ElementoNodo* elem= *it;
		refHermano = elem->getReferencia();
	}
	obtenerHermano(refHermano,hermano,hijo->getNumeroNivel(),balancear);
	return;
	}else{
		izq = true;
		//busco hermano izq
		std::list<ElementoNodo*>:: reverse_iterator it = padre.getListaElementos()->rbegin();
			ElementoNodo* elem = hijo->getListaElementos()->front();
			while(!encontrado && it!=padre.getListaElementos()->rend()){
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
        	std::list<ElementoNodo*>:: iterator it = padre.getListaElementos()->begin();
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
void BSharpTree::obtenerHermano(Referencia ref,Nodo* hermano,unsigned int nivel,bool&balancear){
	std::stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
	archivoArbol.seekg(ref);
	archivoArbol.read((char*)&buffer,tamanioNodo);
	if(nivel==0){
	   hermano = new NodoHoja(&buffer,numeroDeElementosXnodo,comparador);
	}else{ hermano = new NodoIntermedio(&buffer,numeroDeElementosXnodo,comparador);}
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
	while(cantIzq>0){
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
	archivoArbol.seekp(fstream::end);
    Raiz->setRefereciaIzq((Referencia)archivoArbol.tellp());
    grabarUnitario(HijoIzq,archivoArbol.tellp());
    archivoArbol.seekp(fstream::end);
    elementoRaiz->setReferencia((Referencia)archivoArbol.tellp());
    grabarUnitario(HijoDer,archivoArbol.tellp());
    Raiz->getListaElementos()->clear();
    Raiz->agregarElemento(elementoRaiz);
    Raiz->setNumeroNivel(Raiz->getNumeroNivel()+1);
    grabarUnitario(Raiz,posicionRaiz);
};
bool BSharpTree::eliminar(Clave clave){
	archivoArbol.open(nombreArchivo,std::fstream::in|std::fstream::out);
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
	if(subFlujo==0){return false;}
	if(subFlujo==1){
		grabarUnitario(hoja,refHoja);
	    }else{
	    	resolverSubflujo(hoja,listaDePadres,refHoja);
	}
	listaDePadres.clear();
	archivoArbol.close();
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
	archivoArbol.read((char*)&buff,tamanioNodo);
	NodoIntermedio* padre = new NodoIntermedio(&buff,numeroDeElementosXnodo,comparador);
	buscarHermanos(nodo,padre,hermanoIzq,hermanoDer,refHermanoIzq,refHermanoDer,listaDePadres.front(),booleanoInformacion);
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
		/*en esta situacion extrema el nodo izq no es tal, sino q es el nodo medio*/
		if(hermanoDer->getEspacioLibre()>cantidadMinimaDeElementos){
			nodo->balanceoEspecial(hermanoIzq,hermanoDer,padre,false);
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
		/*en esta situacion extrema el nodo der no es tal, sino q es el nodo medio*/
		if(hermanoIzq->getEspacioLibre()>cantidadMinimaDeElementos){
			nodo->balanceoEspecial(hermanoDer,hermanoIzq,padre,true);
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
void BSharpTree::buscarHermanos(Nodo* nodoActual,NodoIntermedio* padre,Nodo* hermanoIzq,Nodo* hermanoDer,Referencia& refHermanoIzq,Referencia& refHermanoDer,Referencia refPadre,vector<bool> &informacion){

};
void BSharpTree::resolverReferenciaSiguiente(Nodo* nodoIzq,Referencia refAHermanoNuevo){
	if(nodoIzq->getNumeroNivel()==0){
		NodoHoja* nodoIzquierdo = dynamic_cast<NodoHoja*>(nodoIzq);
		nodoIzquierdo->setReferenciaSiguiente(refAHermanoNuevo);
	}
};
void BSharpTree::eliminarClaveEnIntermedio(Clave claveAeliminar,Clave* claveSetear){
	NodoIntermedio* nodoConClave = buscarIntermedio(claveAeliminar,Raiz);
	if(nodoConClave==NULL){return;}
	nodoConClave->setearClave(claveAeliminar,claveSetear);
};
NodoIntermedio* BSharpTree::buscarIntermedio(Clave clave,NodoIntermedio* nodo){
	std::stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
	if(nodo->estaClave(clave)){return nodo;
	}else{
		delete nodo;
		Referencia refAux = nodo->bucarReferenciaAsiguienteNodo(clave);
		buf.pubseekpos(0);
		archivoArbol.seekg(refAux);
		archivoArbol.read((char*)&buf,tamanioNodo);
		buf.pubseekpos(0);
		int nivel;
		buf.sgetn((char*)&nivel,sizeof(int));
		if(nivel==0){return NULL;}
		else{
			nodo = new NodoIntermedio(&buf,numeroDeElementosXnodo,comparador);
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
	archivoEspaciosLibres.open(nombreEspaciosLibres,std::fstream::in|std::fstream::out);
	std::stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
	buffer.pubseekpos(0);
	archivoEspaciosLibres.seekg(0);
	int cantElem;
	archivoEspaciosLibres.read((char*)&buffer,sizeof(int));
	buffer.pubseekpos(0);
	buffer.sgetn((char*)&cantElem,sizeof(int));
	buffer.pubseekpos(0);
	buffer.sputn((char*)&ref,sizeof(Referencia));
	buffer.pubseekpos(0);
	int pos = sizeof(int)+ (cantElem*sizeof(Referencia));
	archivoEspaciosLibres.seekp(pos);
	archivoEspaciosLibres.write((char*)&buffer,sizeof(int));
	cantElem++;
	buffer.pubseekpos(0);
	buffer.sputn((char*)&cantElem,sizeof(int));
	archivoEspaciosLibres.seekp(0);
	archivoEspaciosLibres.write((char*)&buffer,sizeof(int));
}
Referencia BSharpTree::buscarEspacioLibre(){
	archivoEspaciosLibres.open(nombreEspaciosLibres,std::fstream::in|std::fstream::out);
	std::stringbuf buffer(ios_base :: in | ios_base :: out | ios_base :: binary);
	buffer.pubseekpos(0);
	archivoEspaciosLibres.seekg(0);
	int cantElem;
	archivoEspaciosLibres.read((char*)&buffer,sizeof(int)+sizeof(Referencia));
	buffer.pubseekpos(0);
	buffer.sgetn((char*)&cantElem,sizeof(int));
	if(cantElem == 0)return 0;
	Referencia retorno;
	buffer.sgetn((char*)&retorno,sizeof(Referencia));
	cantElem--;
	int pos = sizeof(int)+ (sizeof(Referencia)*(cantElem));
	archivoEspaciosLibres.seekg(pos);
	buffer.pubseekpos(0);
	buffer.sputn((char*)&cantElem,sizeof(int));
	archivoEspaciosLibres.read((char*)&buffer,sizeof(Referencia));
	buffer.pubseekpos(0);
	archivoEspaciosLibres.seekp(0);
	archivoEspaciosLibres.write((char*)&buffer,sizeof(int)+sizeof(Referencia));
	return retorno;
};

BSharpTree::~BSharpTree() {

};

