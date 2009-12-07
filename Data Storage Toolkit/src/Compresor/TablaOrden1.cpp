#include "TablaOrden1.h"
using namespace std;
TablaOrden1::TablaOrden1() {
	this->alerta_escape=false;
}
TablaOrden1::~TablaOrden1() {
}
bool TablaOrden1::vacia(){
	return contextos.empty();
}
bool TablaOrden1::esEscape(){
	return this->alerta_escape;
}
void TablaOrden1::obtenerExtremos(unsigned char contexto,unsigned char simbolo,unsigned & piso,unsigned &techo){
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

void TablaOrden1::incremtarOcurrencia(unsigned char contexto,unsigned char simbolo){
	if(contextos.empty()){
		agregarContexto(contexto,simbolo);
	}else{
		tipo_contextos::iterator it = contextos.find(contexto);
		if(it != contextos.end()){
			//Existe contexto
			Contexto& contextoAmodificar = it->second;
			std::list<ElementoContexto>::iterator itLista = contextoAmodificar.tablaFrecuencias.begin();
			bool encontrado=false;
			while(!encontrado&&itLista!=contextoAmodificar.tablaFrecuencias.end()){
				ElementoContexto& elemento = *itLista;
				if(elemento.simbolo==simbolo){//Existe simbolo en el contexto
					elemento.frecuencia++;
					contextoAmodificar.totalFrecuencias++;
					encontrado=true;
				}
				itLista++;
			}
			if(encontrado==false){
				this->ageragarElementoContexto(contextoAmodificar,simbolo);
				contextoAmodificar.totalFrecuencias+=2;
			}

		}else{//no existe contexto
			agregarContexto(contexto,simbolo);
		}
	}
}

void TablaOrden1::agregarContexto(unsigned char contexto,unsigned char simbolo){
	Contexto contextoNuevo;
	if(!contextos.empty()){
		contextoNuevo.totalFrecuencias=2;
		this->ageragarElementoContexto(contextoNuevo,simbolo);
		contextos.insert(std::pair<unsigned char,Contexto>(contexto,contextoNuevo));
	}else{
		contextoNuevo.totalFrecuencias=0;
		contextos.insert(std::pair<unsigned char,Contexto>(simbolo,contextoNuevo));
	}
}

void TablaOrden1::ageragarElementoContexto(Contexto& contextoModificar,unsigned char simbolo){
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

void TablaOrden1::decremetarOcurrencia(unsigned char contexto,unsigned char simbolo){
	if(!contextos.empty()){
		tipo_contextos::iterator it_contexto = contextos.find(contexto);
		if(it_contexto != contextos.end()){
			tipo_tabla_frecuencias::iterator it_elemento_ctx = it_contexto->second.tablaFrecuencias.begin();

			while((it_elemento_ctx->simbolo!=simbolo) and it_elemento_ctx!=it_contexto->second.tablaFrecuencias.end())
				it_elemento_ctx++;

			if(it_elemento_ctx!=it_contexto->second.tablaFrecuencias.end()){
				tipo_frecuencia frec=it_elemento_ctx->frecuencia;
				frec=it_contexto->second.totalFrecuencias;
				if(it_elemento_ctx->frecuencia > 2){
					it_elemento_ctx->frecuencia--;
					it_contexto->second.totalFrecuencias--;
				}else{
					it_contexto->second.totalFrecuencias-=it_elemento_ctx->frecuencia;
					it_contexto->second.tablaFrecuencias.erase(it_elemento_ctx);

				}
				if(it_contexto->second.tablaFrecuencias.empty()){
					contextos.erase(it_contexto);
				}
			}

		}
	}
}

tipo_frecuencia TablaOrden1::buscarOcurrencias(unsigned char anterior,unsigned char caracterBuscado){
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
//Todo ACA
unsigned char TablaOrden1::calcularEmision(unsigned &piso,unsigned &techo,unsigned codigo,unsigned char anterior){
	unsigned char retorno=0;//Todo ACA
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
	}while(codigo>techo&&retorno<256);
	return retorno-1;
}

float TablaOrden1::obtenerTotalContexto(unsigned char simbolo){
	tipo_contextos::iterator it = contextos.find(simbolo);
	if(it != contextos.end()){
		return it->second.totalFrecuencias+(256-it->second.tablaFrecuencias.size());
	}
	return 256;
}
//TODO Esta funcion imprime la cantidad de ocurrencias de un caracter segun un contexto
void TablaOrden1::imprimir(unsigned char contexto,unsigned char simbolo){
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
void TablaOrden1::imprimir(){
	tipo_contextos::iterator contexto = contextos.begin();
	while(contexto!=contextos.end()){
		tipo_tabla_frecuencias::iterator elemento_contexto = contexto->second.tablaFrecuencias.begin();
		cout<<"CTX("<< contexto->second.totalFrecuencias<<"):"<<contexto->first<<" ";
		while(elemento_contexto != contexto->second.tablaFrecuencias.end()){
			cout<<"("<<elemento_contexto->simbolo<<" , ";
			cout<<elemento_contexto->frecuencia<<") ";
			++elemento_contexto;
		}
		contexto++;
		cout<<endl;
	}
}
TablaOrden1* TablaOrden1::clonar(){
	TablaOrden1* clon=new TablaOrden1;
	tipo_contextos::iterator contexto = contextos.begin();
	clon->alerta_escape=alerta_escape;
	while(contexto!=contextos.end()){
		clon->contextos[contexto->first];
		clon->contextos[contexto->first].totalFrecuencias=contexto->second.totalFrecuencias;
		clon->contextos[contexto->first].tablaFrecuencias=contexto->second.tablaFrecuencias;
		contexto++;
	}
	return clon;
}
void TablaOrden1::copiar(TablaOrden1&tabla){
	tipo_contextos::iterator contexto = contextos.begin();
	while(contexto!=contextos.end()){
		contexto->second.tablaFrecuencias.clear();
		contexto++;
	}
	contextos.clear();
	/*Copiando*/
	alerta_escape=tabla.alerta_escape;
	contexto = tabla.contextos.begin();
	while(contexto!=tabla.contextos.end()){
			contextos[contexto->first];
			contextos[contexto->first].totalFrecuencias=contexto->second.totalFrecuencias;
			contextos[contexto->first].tablaFrecuencias=contexto->second.tablaFrecuencias;
			contexto++;
	}
}
