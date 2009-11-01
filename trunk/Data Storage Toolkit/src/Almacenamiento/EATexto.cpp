/*
 * EATexto.cpp
 *
 *  Created on: 13/10/2009
 *      Author: paulo
 */

#include "EATexto.h"

EATexto::EATexto(Registro*registro) {
	this->registro=registro;
};

EATexto::~EATexto() {};

void EATexto::abrir(Almacenamiento*almacen){
	this->almacen=almacen;
	posComp=0;
};
void EATexto::crear(Almacenamiento *almacen){
	this->almacen=almacen;
	posComp=0;
	ultimoLeido=' ';
};
bool EATexto::leer(Componente *componente){
	Registro* registro=dynamic_cast<Registro*>(componente);
	if(registro){
		siguiente();
		posComp++;
		stringAregistro(registro,ultimoLeido);
		return true;
	}
	return false;
};

bool EATexto::escribir(Componente *componente){
	Registro* registro=dynamic_cast<Registro*>(componente);
	if(registro){
		std::string str=registroAstring(registro);
		str.push_back('\n');
		almacen->escribir(str.c_str(),str.size());
		if(logActivo){
			clave->set(registro);
			cambiosLog.push(new Cambio(*clave,posComp,Cambio::Alta));
		}
		posComp++;
		return true;
	}
	return false;
};

bool EATexto::posicionarComponente(size_t nroCompuesto){
	if(nroCompuesto<posComp){
		almacen->posicionar(0);
		posComp=0;
	}
	while(nroCompuesto>posComp&&!almacen->fin()){
		siguiente();
		posComp++;
	}
	return true;
};

bool EATexto::insertar(Componente *componente){
	Registro* registro=dynamic_cast<Registro*>(componente);
	if(registro){
		std::string strRegistro=registroAstring(registro);
		std::cout<<strRegistro<<std::endl;
		size_t posicion;
		bool encontrado=false;
		almacen->posicionar(0);
		posComp=0;
		/*busco un espacio libre donde entre el texto*/
		while(!encontrado && !almacen->fin()){
			posicion=almacen->posicionActual();
			siguiente();
			if(ultimoLeido.at(0)==' ' && ultimoLeido.size()>=strRegistro.size()){
				strRegistro.append(ultimoLeido.size()-strRegistro.size(),' ');
				strRegistro.at(strRegistro.size()-1)='\n';
				encontrado=true;
			}else posComp++;
		}
		if(encontrado)
			almacen->posicionar(posicion);
		else
			posicion=almacen->posicionActual();
		almacen->escribir(strRegistro.c_str(),strRegistro.size());
		posComp++;
		if(logActivo){
			clave->set(registro);
			cambiosLog.push(new Cambio(*clave,posicion,Cambio::Alta));
		}
		return posicion;
	}
	return 0;
};

bool EATexto::eliminar(Componente *componente){
	bool encontrado=false;
	Registro* registro=dynamic_cast<Registro*>(componente);
	if(registro){
		size_t posicion=buscar(registro,encontrado);
		if(encontrado){
			std::string str=ultimoLeido;
			str.replace(str.begin(),str.end()-1,str.size(),' ');
			str.at(str.size()-1)='\n';
			almacen->posicionar(posicion);
			almacen->escribir(str.c_str(),str.size());
			if(logActivo){
				clave->set(registro);

				cambiosLog.push(new Cambio(*clave,posComp,Cambio::Baja));
			}
			posComp++;
		}
	}
	return encontrado;
};
bool EATexto::modificar(Componente *componente){
	bool encontrado=false;
	Registro* registro=dynamic_cast<Registro*>(componente);
	if(registro){
		size_t posicion=buscar(registro,encontrado);
		if(encontrado){
			std::string str=registroAstring(registro);
			if(str.size()<ultimoLeido.size()){
				str.append(ultimoLeido.size()-str.size()-1,' ');
				str.push_back('\n');
				almacen->posicionar(posicion);
				almacen->escribir(str.c_str(),str.size());
				posComp++;
			}else encontrado=false;
		}
	}
	return encontrado;
};


void EATexto::siguiente(){
	ultimoLeido.clear();
	char chr;
	do{
		almacen->leer(&chr);
		ultimoLeido.push_back(chr);
	}while(chr!='\n' && !almacen->fin());
	ultimoLeido.push_back(chr);
};


size_t EATexto::buscar(Registro*registro,bool&encontrado){
	size_t posicion;
	encontrado=false;
	Registro*regAux=(Registro*)registro->clonar();
	posComp=0;
	almacen->posicionar(0);
	do{
		posicion=almacen->posicionActual();
		siguiente();
		stringAregistro(regAux,ultimoLeido);
		if( 0==comparar(registro,regAux)) encontrado=true;
		else posComp++;
	}while(!encontrado && almacen->fin());
	delete regAux;
	return posicion;
}

void EATexto::stringAregistro(Registro*registro,std::string &str){
	std::stringstream strs(str);
	for(Ttamanio i=0;i<registro->cantidadAtributos();i++){
		registro->get(i)->leer(strs);
	}
}
std::string EATexto::registroAstring(Registro*registro){
	std::stringstream strs;
	for(Ttamanio i=0;i<registro->cantidadAtributos();i++){
		registro->get(i)->imprimir(strs);
		strs<<" ";
	}
	return strs.str();
};
int EATexto::comparar(Registro*reg1,Registro*reg2){
	clave->set(reg1);
	Clave clave2(*clave);
	int resultado=comparador->Comparar(clave,&clave2);
	return resultado;
};

Componente *EATexto::getComponente(){
	return registro;
}

bool EATexto::obtener(Componente*componente){
	return siguiente(componente);
};
bool EATexto::siguiente(Componente *componente){
	bool encontrado=false;
	Registro* registro=dynamic_cast<Registro*>(componente);
	if(registro&&!almacen->fin()){
		/*buscar(registro,encontrado);
		if(encontrado){
			stringAregistro(registro,ultimoLeido);
		}*/
		siguiente();
		stringAregistro(registro,ultimoLeido);
	}
	return encontrado;
};
size_t EATexto::posicionComponente(){return this->posComp;};
