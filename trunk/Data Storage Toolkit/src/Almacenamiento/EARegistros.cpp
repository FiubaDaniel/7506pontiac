/*
 * EARegistros.cpp
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */
#include "EARegistros.h"
#include <iostream>
EARegistros::EARegistros(Registro*registro){
	almacen=NULL;
	registroSerializado=NULL;
	tamanioRegistro=registro->tamanioSerializado();
	this->registro=(Registro*)registro->clonar();
}
void EARegistros::finalizarAlmacenamiento(){
	if(almacen!=NULL){
		almacen->posicionar(0);
		almacen->escribir((char*)tamanioEncabezado,sizeof(tamanioEncabezado));
		almacen->escribir((char*)siguienteRegLibre,sizeof(siguienteRegLibre));
	}
}
int EARegistros::comparar(Registro*reg1,Registro*reg2){
	clave->set(reg1);
	Clave*clave2=clave->clonarce();
	int resultado=comparador->Comparar(clave,clave2);
	delete clave2;
	return resultado;
};
EARegistros::~EARegistros() {
	finalizarAlmacenamiento();
	delete[] registroSerializado;
	delete registro;
}
Almacenamiento* EARegistros::abrir(Almacenamiento*almacen){
	finalizarAlmacenamiento();
	Almacenamiento* anterior=this->almacen;
	this->almacen=almacen;
	almacen->posicionar(0);
	Ttamanio tamRegistro=0;
	almacen->leer((char*)tamRegistro,sizeof(tamanioRegistro));
	if(tamRegistro>tamanioRegistro){
		delete[] registroSerializado;
		registroSerializado=new char[tamRegistro];
		tamanioRegistro=tamRegistro;
	}
	almacen->leer((char*)siguienteRegLibre,sizeof(siguienteRegLibre));
	posicionarComponente(0);
	return anterior;
};
Almacenamiento* EARegistros::crear(Almacenamiento*almacenamiento){
	finalizarAlmacenamiento();
	Almacenamiento* anterior=almacen;
	almacen=almacenamiento;
	registroSerializado=new char[tamanioRegistro];
	tamanioEncabezado=sizeof(siguienteRegLibre);
	siguienteRegLibre=0;
	this->almacen->posicionar(0);
	this->almacen->escribir(&siguienteRegLibre,sizeof(siguienteRegLibre));
	return anterior;
}
void EARegistros::escribir(Registro*registro){
	if(!almacen->fin()){
		std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
		buf.pubsetbuf(registroSerializado,tamanioRegistro);
		registro->serializar(buf);
		almacen->escribir(registroSerializado,tamanioRegistro);
		nroRegistro++;
	}
}
void EARegistros::leer(Registro*registro){
	if(!almacen->fin()){
		almacen->leer(registroSerializado,tamanioRegistro);
		std::stringbuf buf;
		buf.pubseekpos(0,std::ios_base::binary | std::ios_base::in );
		buf.pubsetbuf(registroSerializado,tamanioRegistro);
		registro->deserializar(buf);
		nroRegistro++;
	};
}
bool EARegistros::escribir(Componente *componente){
	Registro*registro=dynamic_cast<Registro*>(componente);
	if(registro!=NULL&&nroRegistro<=siguienteRegLibre){
		if(logActivo){
			clave->set(registro);
			cambiosLog.push(new Cambio(*clave,nroRegistro,Cambio::Alta));
		}
		escribir(registro);
		return true;
	}
	return false;
}

bool EARegistros::leer(Componente *componente){
	Registro*registro=dynamic_cast<Registro*>(componente);
	if(registro!=NULL&&nroRegistro<siguienteRegLibre){
		leer(registro);
		return true;
	}
	return false;
}

bool EARegistros::insertar(Componente *componente){
	posicionarComponente(siguienteRegLibre);
	if(escribir(componente)){
		siguienteRegLibre++;
		return true;
	}
	return false;
}

bool EARegistros::modificar(Componente *componente){
	Registro*nuevo=dynamic_cast<Registro*>(componente);
	if(nuevo!=NULL && nroRegistro<siguienteRegLibre){
		leer(registro);
		if(comparar(nuevo,registro)==0){
			nroRegistro--;
			if(logActivo){
				clave->set(nuevo);
				cambiosLog.push(new Cambio(*clave,nroRegistro,Cambio::Modificacion));
			}
			posicionarComponente(nroRegistro);
			escribir(nuevo);

			return true;
		}
	}
	return false;
}

bool EARegistros::eliminar(Componente *componente){
	Registro*eliminado=dynamic_cast<Registro*>(componente);
	if(eliminado!=NULL && nroRegistro<siguienteRegLibre){
		size_t borrado=nroRegistro;
		leer(registro);
		if(comparar(eliminado,registro)==0){
			siguienteRegLibre--;
			posicionarComponente(siguienteRegLibre);
			leer(registro);
			if(logActivo){
				clave->set(eliminado);
				cambiosLog.push(new Cambio(*clave,borrado,Cambio::Baja));
				clave->set(registro);
				cambiosLog.push(new Cambio(*clave,borrado,Cambio::Reubicacion));
			}
			posicionarComponente(borrado);
			escribir(registro);
			return true;
		}
	}
	return false;
}
bool EARegistros::posicionarComponente(size_t nroCompuesto){
	nroRegistro=nroCompuesto;
	almacen->posicionar(nroCompuesto*tamanioRegistro+tamanioEncabezado);
	return true;
}
bool EARegistros::siguiente(Componente *componente){
	if(!almacen->fin() && nroRegistro < siguienteRegLibre)
		return leer(componente);
	return false;
};
Componente *EARegistros::getComponente(){
	return registro;
};

bool EARegistros::obtener(Componente*componente){
	return leer(componente);
};
size_t EARegistros::posicionComponente(){return nroRegistro;};
