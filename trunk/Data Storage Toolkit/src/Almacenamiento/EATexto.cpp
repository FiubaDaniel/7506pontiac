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
	ultimo=false;
};
void EATexto::crear(Almacenamiento *almacen){
	this->almacen=almacen;
	posComp=0;
	ultimoLeido="";
	ultimo=false;
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
		size_t posicion;
		do{
			posicion=buscarProximoLibre();
		}while(!almacen->fin() && strRegistro.size() > ultimoLeido.size());

		almacen->reiniciar();
		almacen->posicionar(posicion);

		almacen->escribir(strRegistro.c_str(),strRegistro.size());

		if(logActivo){
			clave->set(registro);
			cambiosLog.push(new Cambio(*clave,posComp-1,Cambio::Alta));
		}

		return true;
	}
	return false;
};

bool EATexto::eliminar(Componente *componente){
	Registro* registro=dynamic_cast<Registro*>(componente);
	if(registro){
			size_t posicion=almacen->posicionActual();
			siguiente();
			almacen->reiniciar();
			stringAregistro(registro,ultimoLeido);
			std::string str=ultimoLeido;
			std::cout<<str<<endl;
			str.replace(str.begin(),str.end()-1,str.size(),' ');
			str.at(str.size()-1)='\n';
			std::cout<<str<<endl;
			almacen->posicionar(posicion);
			almacen->escribir(str.c_str(),str.size());
			if(logActivo){
				clave->set(registro);
				cambiosLog.push(new Cambio(*clave,posComp,Cambio::Baja));
			}
			posComp++;
			return true;
	}
	return false;
};
bool EATexto::modificar(Componente *componente){
	Registro* registro=dynamic_cast<Registro*>(componente);
	if(registro){
		std::string str=registroAstring(registro);
		str.append(ultimoLeido.size()-str.size()-1,' ');
		str.push_back('\n');
		almacen->escribir(str.c_str(),str.size());
		posComp++;
		if(str.size()<ultimoLeido.size()){
			if(logActivo){
				clave->set(registro);
				cambiosLog.push(new Cambio(*clave,posComp-1,Cambio::Modificacion));
			}
		}else{
			size_t posicion;
			do{
				posicion=buscarProximoLibre();
			}while(!almacen->fin() && str.size() > ultimoLeido.size());
			if(almacen->fin())
				almacen->reiniciar();
			almacen->posicionar(posicion);
			almacen->escribir(str.c_str(),str.size());
			posComp++;
			if(logActivo){
				clave->set(registro);
				cambiosLog.push(new Cambio(*clave,posComp,Cambio::Reubicacion));
			}
		};
		return true;
	}
	return false;
}

void EATexto::siguiente(){
	ultimoLeido.clear();
	char chr;
	do{
		almacen->leer(&chr);
		ultimoLeido.push_back(chr);
	}while(chr!='\n' && !almacen->fin());
	ultimoLeido.push_back(chr);
};

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
	strs.put('\n');
	return strs.str();
};

Componente *EATexto::getComponente(){
	return registro;
}
size_t EATexto::buscarProximoLibre(){
	/*busco un espacio libre donde entre el texto*/
	bool encontrado=false;
	size_t posicion;
	do{
		posicion=almacen->posicionActual();
		siguiente();
		if(ultimoLeido.at(0)==' '){
			encontrado=true;
		}else posComp++;
	}while(!encontrado && !almacen->fin());

	return posicion;
};
bool EATexto::obtener(Componente*componente){
	return siguiente(componente);
};
bool EATexto::siguiente(Componente *componente){
	Registro* registro=dynamic_cast<Registro*>(componente);
	if(!almacen->fin()&& !ultimo ){
		siguiente();
		stringAregistro(registro,ultimoLeido);
		return true;
	}else if(!ultimo){
		almacen->reiniciar();
		ultimo=true;
	}
	return false;
};
size_t EATexto::posicionComponente(){return this->posComp;};
