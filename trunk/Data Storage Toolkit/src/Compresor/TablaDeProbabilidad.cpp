#include "TablaDeProbabilidad.h"

TablaDeProbabilidad::TablaDeProbabilidad() {
	// TODO Auto-generated constructor stub

}

TablaDeProbabilidad::~TablaDeProbabilidad() {
	// TODO Auto-generated destructor stub
}
bool TablaDeProbabilidad::vacia(){
	return contextos.empty();
}
void TablaDeProbabilidad::obtenerExtremos(char contexto,char simbolo,unsigned & piso,unsigned &techo){
	unsigned longitud=techo-piso;
	tipo_frecuencia incremento;
	tipo_frecuencia total;
	tipo_contextos::iterator it=contextos.find(contexto);
	if(it!=contextos.end()){
		total=it->second.totalFrecuencias-(256-it->second.tablaFrecuencias.size());
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

void TablaDeProbabilidad::incremtarOcurrencia(unsigned char contexto,unsigned char simbolo){
	if(contextos.empty()){
		agregarContexto(contexto,simbolo);
	}else{
		Contexto& contextoAmodificar;
		tipo_contextos::iterator it = contextos.find(contexto);
		if(it != contextos.end()){//Existe contexto
			contextoAmodificar = it->second;
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
	contextoNuevo.totalFrecuencias=1;
	this->ageragarElementoContexto(contextoNuevo,simbolo);
	contextos.insert(std::pair<unsigned char,Contexto>(contexto,contextoNuevo));
}

void TablaDeProbabilidad::ageragarElementoContexto(Contexto& contextoModificar,unsigned char simbolo){
	ElementoContexto elementoNuevo;
	elementoNuevo.frecuencia=1;
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
			}
			++it;
		}
		if(!encontrado){
			contextoModificar.tablaFrecuencias.push_front(elementoNuevo);
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
				ElementoContexto elemento = *itLista;
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
	unsigned char retorno;
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
	}while(codigo>techo);
	return retorno;
}
float TablaDeProbabilidad::obtenerTotalContexto(unsigned char simbolo){
	tipo_contextos::iterator it = contextos.find(simbolo);
	return it->second.totalFrecuencias+(256-it->second.tablaFrecuencias.size());
}
