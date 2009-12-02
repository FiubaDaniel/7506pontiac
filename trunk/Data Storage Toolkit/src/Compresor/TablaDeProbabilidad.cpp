#include "TablaDeProbabilidad.h"
using namespace std;
TablaDeProbabilidad::TablaDeProbabilidad() {
}
TablaDeProbabilidad::~TablaDeProbabilidad() {
}
bool TablaDeProbabilidad::vacia(){//Joya
	return contextos.empty();
}
/*void TablaDeProbabilidad::obtenerExtremos(char contexto,char simbolo,unsigned & piso,unsigned &techo){
	//todo remover
	if(techo<piso)
		throw 8;
	unsigned longitud=techo-piso;
	tipo_frecuencia incremento;
	tipo_frecuencia total;
	tipo_contextos::iterator it=contextos.find(contexto);
	if(it!=contextos.end()){
		total=256-it->second.tablaFrecuencias.size()+it->second.totalFrecuencias;
		tipo_tabla_frecuencias::iterator itTabla=it->second.tablaFrecuencias.begin();
		unsigned char cant_elementos=0;
		acumulacion de los que ESTAN en la tabla antes del pedido
		while(itTabla!=it->second.tablaFrecuencias.end() and itTabla->simbolo<simbolo){
			incremento=float(longitud)/float(total);
			incremento+=1/float(total);
			incremento*=itTabla->frecuencia;
			piso+=floor(piso+incremento);
			cant_elementos++;
			itTabla++;
		}
		acumulacion de los que NO ESTAN en tabla antes q el pedido
		if(cant_elementos<(unsigned char)simbolo){
			cant_elementos=((unsigned char)simbolo)-cant_elementos;
			incremento=float(longitud)/float(total);
			incremento+=1/float(total);
			incremento*=cant_elementos;
			piso+=floor(piso+incremento);
		}
		incremento para el techo
		if(itTabla!=it->second.tablaFrecuencias.end() and itTabla->simbolo==simbolo){
			incremento=float(longitud)/float(total);
			incremento+=1/float(total);
			incremento*=itTabla->frecuencia;
		}else{
			//no es el piso del simbolo
			incremento=float(longitud)/float(total);
			incremento+=1/float(total);
		}
	}else{
		tabla completamente vacia
		total=256;
		incremento=float(longitud)/float(total);
		incremento+=1/float(total);
		incremento*=(unsigned char)simbolo;
		piso+=floor(piso+incremento);
		incremento=float(longitud)/float(total);
		incremento+=1/float(total);
	}
	techo=floor(piso+incremento)-1;
	//TODO remover
	if(techo<piso)
		throw 8;
}*/
void TablaDeProbabilidad::obtenerExtremos(unsigned char contexto,unsigned char simbolo,unsigned & piso,unsigned &techo){
	int retorno=0;
	unsigned longitud = techo - piso;
	unsigned temp=piso;

	float total = obtenerTotalContexto(contexto);
	do{
		piso=temp;
		float ocurrencias=buscarOcurrencias(contexto,retorno);
		temp=floor(piso+ocurrencias*((longitud/total)+(1/total)));
		techo=temp-1;
		retorno++;
	}while(retorno<=simbolo);
	if(techo<piso)
		throw 8;
}
/*
 * Posibles actualizaciones del map:
 * 1-)Existe el contexto y el simbolo dentro de dicho contexto. Se aumenta la frecuencia del simbolo
 * y la frecuencia total del contexto
 * 2-)El contexto no existe, lo que implica la no existencia como contexto del anteriorEmitido. En tal caso se
 * agrega dicho contexto, y a su vez se lo actualiza con el valor emitido.
 * 3-)El contexto existe, pero el valor emitido no existe, en dicho caso se agrega el emitido a la lista del contexto.
 */

void TablaDeProbabilidad::incremtarOcurrencia(unsigned char contexto,unsigned char simbolo){
	if(contextos.empty()){
		agregarContexto(contexto,simbolo);
	}else{
		tipo_contextos::iterator it = contextos.find(contexto);
		if(it != contextos.end()){//Existe contexto
			Contexto& contextoAmodificar = it->second;
			std::list<ElementoContexto>::iterator itLista = contextoAmodificar.tablaFrecuencias.begin();
			bool encontrado=false;
			while(!encontrado&&itLista!=contextoAmodificar.tablaFrecuencias.end()){
				ElementoContexto& elemento = *itLista;
				if(elemento.simbolo==simbolo){//Existe simbolo en el contexto
					elemento.frecuencia=elemento.frecuencia+1;
					encontrado=true;
				}
				itLista++;
			}
			if(encontrado==false){
				Contexto& contextoAmodificar = it->second;
				this->ageragarElementoContexto(contextoAmodificar,simbolo);
			}
			contextoAmodificar.totalFrecuencias=contextoAmodificar.totalFrecuencias+1;
		}else{//no existe contexto
			agregarContexto(contexto,simbolo);
		}
	}
}

void TablaDeProbabilidad::agregarContexto(unsigned char contexto,unsigned char simbolo){
	Contexto contextoNuevo;
	if(!contextos.empty()){
		contextoNuevo.totalFrecuencias=0;//Tiene q ser dos pq se su
		this->ageragarElementoContexto(contextoNuevo,simbolo);
		contextos.insert(std::pair<unsigned char,Contexto>(contexto,contextoNuevo));
	}else{
		contextoNuevo.totalFrecuencias=2;
		contextos.insert(std::pair<unsigned char,Contexto>(simbolo,contextoNuevo));
	}
}

void TablaDeProbabilidad::ageragarElementoContexto(Contexto& contextoModificar,unsigned char simbolo){
	ElementoContexto elementoNuevo;
	elementoNuevo.frecuencia=2;
	elementoNuevo.simbolo=simbolo;
	if(contextoModificar.tablaFrecuencias.empty()){
		contextoModificar.tablaFrecuencias.push_back(elementoNuevo);
	}else{
		std::list<ElementoContexto>::iterator it = contextoModificar.tablaFrecuencias.begin();
		bool encontrado = false;
		while(!encontrado && it != contextoModificar.tablaFrecuencias.end()){
			ElementoContexto elemento = *it;
			if(simbolo<elemento.simbolo){
				contextoModificar.tablaFrecuencias.insert(it,elementoNuevo);
				encontrado=true;
			}
			++it;
		}
		if(!encontrado){
			contextoModificar.tablaFrecuencias.push_back(elementoNuevo);
		}
	}
}

void TablaDeProbabilidad::decremetarOcurrencia(unsigned char contexto,unsigned char simbolo){
	if(!contextos.empty()){
		tipo_contextos::iterator it = contextos.find(contexto);
		if(it != contextos.end()){
			Contexto&  contextoAmodificar = it->second;
			bool encontrado = false;
			std::list<ElementoContexto>::iterator itLista = contextoAmodificar.tablaFrecuencias.begin();
			while(!encontrado && itLista!=contextoAmodificar.tablaFrecuencias.end()){
				ElementoContexto& elemento = *itLista;
				if(elemento.simbolo==simbolo){
					if(elemento.frecuencia > 1)
						elemento.frecuencia=elemento.frecuencia-1;
					else
						contextoAmodificar.tablaFrecuencias.erase(itLista);
					contextoAmodificar.totalFrecuencias--;
					encontrado=true;
				}
				itLista++;
			}
		}
	}
}

tipo_frecuencia TablaDeProbabilidad::buscarOcurrencias(unsigned char anterior,unsigned char caracterBuscado){
	if(!contextos.empty()){
		tipo_contextos::iterator it = contextos.find(anterior);
		if(it != contextos.end()){
			Contexto&  buscado = it->second;
			std::list<ElementoContexto>::iterator itLista = buscado.tablaFrecuencias.begin();
			while(itLista != buscado.tablaFrecuencias.end()){
				ElementoContexto elemento = *itLista;
				if(caracterBuscado == elemento.simbolo){
					return elemento.frecuencia;
				}
				++itLista;
			}
		}
	}
	return 1;
}

unsigned char TablaDeProbabilidad::calcularEmision(unsigned &piso,unsigned &techo,unsigned codigo,unsigned char anterior){
	unsigned char retorno=0;
	unsigned longitud = techo - piso;
	unsigned temp=piso;
	/*Ya que la lista puede no tener todos los simbolos el totalOcurrencias es el total de los simbolos q contiene la lista
	 * como concecuencia para obtener probabilidades busco el total sumando los simbolos no exitentes en la lista
	 */
	float total = obtenerTotalContexto(anterior);
	do{
		piso=temp;
		float ocurrencias=buscarOcurrencias(anterior,retorno);
		temp=floor(piso+ocurrencias*((longitud/total)+(1/total)));//temp seria el piso siguiente.
		techo=temp-1;//al piso siguiente le resto 1.
		retorno++;
	}while(codigo>=techo&&retorno<256);
	return retorno-1;
}

float TablaDeProbabilidad::obtenerTotalContexto(unsigned char simbolo){
	tipo_contextos::iterator it = contextos.find(simbolo);
	if(it != contextos.end()){
	return it->second.totalFrecuencias+(256-it->second.tablaFrecuencias.size());
	}
	return 256;
}
//TODO Esta funcion imprime la cantidad de ocurrencias de un caracter segun un contexto
void TablaDeProbabilidad::imprimir(unsigned char contexto,unsigned char simbolo){
	tipo_contextos::iterator it = contextos.find(contexto);
	if(it != contextos.end()){
		Contexto buscado = it->second;
		std::list<ElementoContexto>::iterator itLista = buscado.tablaFrecuencias.begin();
		while(itLista != buscado.tablaFrecuencias.end()){
			ElementoContexto elemento = *itLista;
			if(simbolo == elemento.simbolo){
				cout<<"Fercuencia del simbolo: "<<simbolo<<endl;
				cout<<elemento.frecuencia<<endl;
			}
			++itLista;
		}
	}
}
