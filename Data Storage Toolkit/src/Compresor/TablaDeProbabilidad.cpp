#include "TablaDeProbabilidad.h"

TablaDeProbabilidad::TablaDeProbabilidad() {
	// TODO Auto-generated constructor stub

}

TablaDeProbabilidad::~TablaDeProbabilidad() {
	// TODO Auto-generated destructor stub
}

void TablaDeProbabilidad::obtenerExtremos(char contexto,char simbolo,unsigned & piso,unsigned &techo){
	unsigned longitud=techo-piso;
	tipo_frecuencia incremento;
	tipo_frecuencia total;
	tipo_contextos::iterator it=contextos.find(contexto);
	if(it!=contextos.end()){
		total=it->second.totalFrecuencias;
		tipo_tabla_frecuencias::iterator itTabla=it->second.tablaFrecuencias.begin();
		unsigned char cant_elementos=0;
		/*acumulacion de los que ESTAN en la tabla*/
		while(itTabla!=it->second.tablaFrecuencias.end() and itTabla->simbolo<simbolo){
			incremento=itTabla->frecuencia*((longitud/total)+(1/total));
			piso+=floor(piso+incremento);
			cant_elementos++;
			itTabla++;
		}
		/*acumulacion de los que NO ESTAN en tabla*/
		if(cant_elementos<(unsigned char)simbolo){
			cant_elementos=((unsigned char)simbolo)-cant_elementos;
			incremento=cant_elementos*((longitud/total)+(1/total));
			piso+=floor(piso+incremento);
		}
		/*incremento para el techo*/
		if(itTabla!=it->second.tablaFrecuencias.end() and itTabla->simbolo==simbolo){
			incremento=itTabla->frecuencia*((longitud/total)+(1/total));
		}else{
			incremento=((longitud/total)+(1/total));
		}
	}else{
		/*tabla completamente vacia*/
		total=256;
		incremento=((unsigned char)(simbolo-1))*((longitud/total)+(1/total));
		piso+=floor(piso+incremento);
		incremento=((longitud/total)+(1/total));
	}
	techo=floor(piso+incremento)-1;
}
/*
 * Posibles actualizaciones del map:
 * 1-)Existe el contexto y el simbolo dentro de dicho contexto. Se aumenta la frecuencia del simbolo
 * y la frecuencia total del contexto
 * 2-)El contexto no existe, lo que implica la no existencia como contexto del anteriorEmitido. En tal caso se
 * agrega dicho contexto, y a su vez se lo actualiza con el valor emitido.
 * 3-)El contexto existe, pero el valor emitido no existe, en dicho caso se agrega el emitido a la lista del contexto.
 */
void TablaDeProbabilidad::incremtarOcurrencia(char contexto,char simbolo){
	if(contextos.empty()){
		agregarContexto(contexto,simbolo);
	}else{
		Contexto contextoAmodificar;
		tipo_contextos::iterator it = contextos.find(contexto);
		if(it != contextos.end()){//Existe contexto
			contextoAmodificar = it->second();
			std::list<ElementoContexto>::iterator itLista = contextoAmodificar.tablaFrecuencias.begin();
			bool encontrado=false;
			while(!encontrado&&itLista!=contextoAmodificar.tablaFrecuencias.end()){
				ElementoContexto elemento = *itLista;
				if(elemento.simbolo==simbolo){//Existe simbolo en el contexto
					elemento.frecuencia=elemento.frecuencia+1;
					encontrado=true;
				}
				itLista++;
			}
			if(encontrado==false){
				ageragarElementoContexto(contextoAmodificar,contexto,simbolo);
			}
			contextoAmodificar.totalFrecuencias=contextoAmodificar.totalFrecuencias+1;
		}else{//no existe contexto
			agregarContexto(contexto,simbolo);
		}
	}
}

void TablaDeProbabilidad::agregarContexto(char contexto,char simbolo){
	Contexto contextoNuevo;
	contextoNuevo.totalFrecuencias=1;
	agregarElementoContexto(contextoNuevo,simbolo);
	contextos.insert(simbolo,contextoNuevo);
}

void TablaDeProbabilidad::ageragarElementoContexto(Contexto& contextoModificar,char simbolo){
	ElementoContexto elementoNuevo;
	elementoNuevo.frecuencia=1;
	elementoNuevo.simbolo=simbolo;
	if(contextoModificar.tablaFrecuencias.empty()){
         contextoModificar.tablaFrecuencias.insert(elementoNuevo);
	}else{
         std::list<ElementoContexto>::reverse_iterator it = contextoModificar.tablaFrecuencias.rbegin();
         bool encontrado = false;
         while(!encontrado && it != contextoModificar.tablaFrecuencias.rend()){
        	 ElementoContexto elemento = *it;
        	 if(elemento.simbolo<simbolo){
        		 contextoModificar.tablaFrecuencias.insert(elementoNuevo);
        	 }
        	 ++it;
         }
         if(!encontrado){
        	 contextoModificar.tablaFrecuencias.push_front(elementoNuevo);
         }
	}
}
void TablaDeProbabilidad::decremetarOcurrencia(char contexto,char simbolo){
	//Busco el contexto a decrementar
}
