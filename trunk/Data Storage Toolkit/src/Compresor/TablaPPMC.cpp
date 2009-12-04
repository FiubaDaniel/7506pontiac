/*
 * TablaPPMC.cpp
 *
 *  Created on: 02/12/2009
 *      Author: daniel
 */

#include "TablaPPMC.h"

namespace PPMC{ //inicio NAMESPACE
TablaPPMC::TablaPPMC() {
	this->alerta_escape=false;
}
TablaPPMC::~TablaPPMC() {

}
bool TablaPPMC::vacia(){
	return contextos.empty();
}
bool TablaPPMC::esEscape(){
	return this->alerta_escape;
}
/*Metodo para q use el PPMC inicialmente solo recorriendo lo elementos existentes*/

void TablaPPMC::obtenerExtremos(unsigned char contexto,unsigned char simbolo,unsigned & piso,unsigned &techo){
	int retorno=0;
	unsigned longitud = techo - piso;
	unsigned temp=piso;

	float total = obtenerTotalContexto(contexto);//Se obtiene segun condicones iguales a q while  tomar
	if(this->alerta_escape){//Contexto -1
		do{
			piso=temp;
			float ocurrencias=1;
			temp=floor(piso+ocurrencias*((longitud/total)+(1/total)));
			techo=temp-1;
			retorno++;
		}while(retorno<=simbolo&&retorno<257);
		this->alerta_escape=false;
	}else{//Recorro solo los simbolos del contexto existente
		tipo_contextos::iterator it = contextos.find(contexto);
		if(contextos.empty()||it == contextos.end()){//Si no existe contexto paso dirtectamente a contexto -1
			this->alerta_escape=true;
		}else{//Existe el contexto, recorro elementos existentes del contexo
			Contexto contextoBuscado = it->second;
			if(contextoBuscado.tablaFrecuencias.empty()){
				this->alerta_escape=true;
				return;
			}
			std::list<ElementoContexto>::iterator it_contexto = contextoBuscado.tablaFrecuencias.begin();
			bool encontrado=false;
			while(!encontrado&&it_contexto!=contextoBuscado.tablaFrecuencias.end()){
				ElementoContexto elemento = *it_contexto;
				if(elemento.simbolo==simbolo){
					encontrado=true;
				}
				piso=temp;
				float ocurrencias=elemento.frecuencia;
				temp=floor(piso+ocurrencias*((longitud/total)+(1/total)));
				techo=temp-1;
				++it_contexto;
			}
			if(!encontrado){
				piso=temp;
				float ocurrencias=contextoBuscado.frecuencia_escape;//Buscar ocurrencia debe devolver frecuencia de escape
				techo=floor(piso+ocurrencias*((longitud/total)+(1/total)))-1;
				this->alerta_escape=true;
			}
		}
	}
	if(techo<piso)
		throw 8;
}
/*
 * Posibles actualizaciones del map:
 * 1-)Existe el contexto y el simbolo dentro de dicho contexto. Se aumenta la frecuencia del simbolo
 * y la frecuencia_simbolo del contexto
 * 2-)El contexto no existe, lo que implica la no existencia como contexto del anteriorEmitido. En tal caso se
 * agrega dicho contexto poniendo su freucencia de simbolo en 1 y frecuencia escape en 1.
 * 3-)El contexto existe, pero no el simbolo, se emite la frecuencia_ escape.
 */

void TablaPPMC::incremtarOcurrencia(unsigned char contexto,unsigned char simbolo){
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
					elemento.frecuencia++;
					contextoAmodificar.frecuencias_simbolos++;
					encontrado=true;
				}
				itLista++;
			}
			if(encontrado==false){//No existe simbolo
				Contexto& contextoAmodificar = it->second;
				this->ageragarElementoContexto(contextoAmodificar,simbolo);
				contextoAmodificar.frecuencia_escape++;
				contextoAmodificar.frecuencias_simbolos++;
			}
		}else{//no existe contexto
			agregarContexto(contexto,simbolo);
		}
	}
	/*std::cout<<"Nuevo "<<std::endl;
	this->imprimir();*/
}

void TablaPPMC::agregarContexto(unsigned char contexto,unsigned char simbolo){
	Contexto contextoNuevo;
	if(!contextos.empty()){
		contextoNuevo.frecuencias_simbolos=1;
		contextoNuevo.frecuencia_escape=1;//Poner en 1
		this->ageragarElementoContexto(contextoNuevo,simbolo);
		contextos.insert(std::pair<unsigned char,Contexto>(contexto,contextoNuevo));
	}else{
		contextoNuevo.frecuencias_simbolos=0;
		contextoNuevo.frecuencia_escape=0;
		contextos.insert(std::pair<unsigned char,Contexto>(simbolo,contextoNuevo));
	}
}

void TablaPPMC::ageragarElementoContexto(Contexto& contextoModificar,unsigned char simbolo){
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
				encontrado=true;
			}
			++it;
		}
		if(!encontrado){
			contextoModificar.tablaFrecuencias.push_back(elementoNuevo);
		}
	}
}

void TablaPPMC::decremetarOcurrencia(unsigned char contexto,unsigned char simbolo){
	if(!contextos.empty()){
		tipo_contextos::iterator it = contextos.find(contexto);
		if(it != contextos.end()){
			Contexto&  contextoAmodificar = it->second;
			bool encontrado = false;
			std::list<ElementoContexto>::iterator itLista = contextoAmodificar.tablaFrecuencias.begin();
			while(!encontrado && itLista!=contextoAmodificar.tablaFrecuencias.end()){
				ElementoContexto& elemento = *itLista;
				if(elemento.simbolo==simbolo){
					if(elemento.frecuencia > 1){
						elemento.frecuencia--;
					}else{
						contextoAmodificar.tablaFrecuencias.erase(itLista);
						contextoAmodificar.frecuencia_escape--;
					}
					contextoAmodificar.frecuencias_simbolos--;
					encontrado=true;
				}
				itLista++;
			}
			if(it->second.tablaFrecuencias.empty()){
				contextos.erase(it);
			}
		}
	}
}
unsigned char TablaPPMC::calcularEmision(unsigned &piso,unsigned &techo,unsigned codigo,unsigned char anterior){
	//unsigned char retorno=0;
	unsigned longitud = techo - piso;
	unsigned temp=piso;
	/*Ya que la lista puede no tener todos los simbolos el totalOcurrencias es el total de los simbolos q contiene la lista
	 * como concecuencia para obtener probabilidades busco el total sumando los simbolos no exitentes en la lista
	 */
	float total = obtenerTotalContexto(anterior);
	if(this->alerta_escape){
		int retorno2=0;
		do{
			piso=temp;
			float ocurrencias=1;
			temp=floor(piso+ocurrencias*((longitud/total)+(1/total)));//temp seria el piso siguiente.
			techo=temp-1;//al piso siguiente le resto 1.
			retorno2++;
		}while(codigo>=techo&&retorno2<256);
		this->alerta_escape=false;
		return retorno2-1;
	}else{
		tipo_contextos::iterator it = contextos.find(anterior);
		if(contextos.empty()||it == contextos.end()){//Si no existe contexto paso dirtectamente a contexto -1
			this->alerta_escape=true;
			return 0;
		}else{//Existe el contexto, recorro elementos exiistentes del contexo
			Contexto contextoBuscado = it->second;
			if(contextoBuscado.tablaFrecuencias.empty()){
				this->alerta_escape=true;
				return 0;
			}
			std::list<ElementoContexto>::iterator it_contexto = contextoBuscado.tablaFrecuencias.begin();
			while(it_contexto!=contextoBuscado.tablaFrecuencias.end()){
				ElementoContexto elemento = *it_contexto;
				piso=temp;
				float ocurrencias=elemento.frecuencia;
				temp=floor(piso+ocurrencias*((longitud/total)+(1/total)));
				techo=temp-1;
				if(piso<=codigo and codigo<=techo)
					return elemento.simbolo;
				++it_contexto;
			}
			piso=temp;
			float ocurrencias=contextoBuscado.frecuencia_escape;//ocurrecia debe devolver frecuencia de escape
			techo=floor(piso+ocurrencias*((longitud/total)+(1/total)))-1;
			this->alerta_escape=true;
			return 0;
		}
	}
}
float TablaPPMC::obtenerTotalContexto(unsigned char simbolo){
	tipo_contextos::iterator it = contextos.find(simbolo);
	if(it != contextos.end()){//Existe contexto
		Contexto contexto = it->second;
		if(this->alerta_escape){
			return 256-contexto.tablaFrecuencias.size();//Elimino los de la lista por exclusion.
		}
		return contexto.frecuencia_escape+contexto.frecuencias_simbolos;
	}else{
		return 256;
	}
	return 1;//No existe contexto, no impòrta lo devuelto ya q no se usará.
}
//TODO Esta funcion imprime la cantidad de ocurrencias de un caracter segun un contexto
void TablaPPMC::imprimir(unsigned char contexto,unsigned char simbolo){
	tipo_contextos::iterator it = contextos.find(contexto);
	if(it != contextos.end()){
		Contexto buscado = it->second;
		std::list<ElementoContexto>::iterator itLista = buscado.tablaFrecuencias.begin();
		while(itLista != buscado.tablaFrecuencias.end()){
			ElementoContexto elemento = *itLista;
			if(simbolo == elemento.simbolo){
				std::cout<<"Fercuencia del simbolo: "<<simbolo<<std::endl;
				std::cout<<elemento.frecuencia<<std::endl;
			}
			++itLista;
		}
	}
}

void TablaPPMC::imprimir(){
	tipo_contextos::iterator contexto = contextos.begin();
	while(contexto!=contextos.end()){
		tipo_tabla_frecuencias::iterator elemento_contexto = contexto->second.tablaFrecuencias.begin();
		std::cout<<"CTX "<<(int)contexto->first<<" (S:"<< contexto->second.frecuencias_simbolos<<",E:"<< contexto->second.frecuencia_escape<<"): ";
		while(elemento_contexto != contexto->second.tablaFrecuencias.end()){
			std::cout<<"("<<(int)elemento_contexto->simbolo<<" , ";
			std::cout<<elemento_contexto->frecuencia<<") ";
			++elemento_contexto;
		}
		contexto++;
		std::cout<<std::endl;
	}
}
TablaPPMC* TablaPPMC::clonar(){
	TablaPPMC* clon=new TablaPPMC;
	tipo_contextos::iterator contexto = contextos.begin();
	clon->alerta_escape=alerta_escape;
	while(contexto!=contextos.end()){
		clon->contextos[contexto->first];
		clon->contextos[contexto->first].frecuencia_escape=contexto->second.frecuencia_escape;
		clon->contextos[contexto->first].frecuencias_simbolos=contexto->second.frecuencias_simbolos;
		clon->contextos[contexto->first].tablaFrecuencias=contexto->second.tablaFrecuencias;
		contexto++;
	}
	return clon;
};
void TablaPPMC::copiar(TablaPPMC&tabla){
	/*Borro lo anterior*/
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
			contextos[contexto->first].frecuencia_escape=contexto->second.frecuencia_escape;
			contextos[contexto->first].frecuencias_simbolos=contexto->second.frecuencias_simbolos;
			contextos[contexto->first].tablaFrecuencias=contexto->second.tablaFrecuencias;
			contexto++;
	}
}
}



