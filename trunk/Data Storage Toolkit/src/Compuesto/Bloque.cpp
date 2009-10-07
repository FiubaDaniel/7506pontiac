/*
 * Bloque.cpp
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */

#include "Bloque.h"
/*------------------------------------------------------------------------------------*/
Bloque::Bloque(){};
Bloque::Bloque(Ttamanio tamanio,Componente*componente) : Bloque::Componente(){
	componentes.push_back(componente->clonar());
	espacioLibre=tamanioBloque=tamanio;
	espacioLibre-=componente->tamanioSerializado();
};
Bloque::~Bloque() {
	for(Ttamanio i= componentes.size()-1;i>=0;i--){
				delete componentes.at(i);
	}
};
Componente* Bloque::clonar(){
	//TODO ver si sirve o no
	Bloque* clon=new Bloque(tamanioBloque,componentes.at(0)->clonar());
	return clon;
};
Ttamanio Bloque::deserializar(std::istream&entrada){
	Ttamanio nrocomponetes;
	Ttamanio offset=sizeof(Ttamanio);
	entrada.read((char*)&nrocomponetes,offset);
	Ttamanio i=0;
	while(i<nrocomponetes and i<componentes.size()){
		offset+=componentes.at(i)->deserializar(entrada);
		i++;
	}
	/*le sobran componentes*/
	while(i<componentes.size()){
		componentes.erase(componentes.begin()+i);
	}
	/*le faltan componentes*/
	Componente* auxiliar=componentes.at(i-1)->clonar();
	while(i<nrocomponetes){
		offset+=auxiliar->deserializar(entrada);
		componentes.push_back(auxiliar->clonar());
		i++;
	}
	delete auxiliar;
	return offset;
};
Ttamanio Bloque::serializar(std::ostream&salida){
	Ttamanio nrocomponetes=componentes.size();
	Ttamanio offset=sizeof(Ttamanio);
	salida.write((char*)&nrocomponetes,offset);
	for(Ttamanio i=0;i<nrocomponetes;i++){
		offset+=componentes.at(i)->serializar(salida);
	}
	return offset;
};
Ttamanio Bloque::tamanioSerializado(){return tamanioBloque;};
/*------------------------------------------------------------------------------------*/
Componente* Bloque::get(Ttamanio nroComponente){
	return componentes.at(nroComponente);
};
bool Bloque::eliminar(Ttamanio posicion){
	if(posicion>=componentes.size())return false;
	espacioLibre+=componentes.at(posicion)->tamanioSerializado();
	componentes.erase(componentes.begin()+posicion);
	return true;
};
bool Bloque::insertar(Componente* nuevo,Ttamanio posicion){
	Ttamanio nuevoTamanio=espacioLibre;
	nuevoTamanio-=nuevo->tamanioSerializado();
	if(nuevoTamanio>0){
		espacioLibre=nuevoTamanio;
		componentes.insert(componentes.begin()+posicion,nuevo);
		return true;
	}
	return false;
};
//intenta insertar detras del registro actual si puede
bool Bloque::agregar(Componente* nuevo){
	Ttamanio nuevoTamanio=espacioLibre;
	nuevoTamanio-=nuevo->tamanioSerializado();
	if(nuevoTamanio>0){
		espacioLibre=nuevoTamanio;
		componentes.push_back(nuevo);
		return true;
	}
	return false;
};//intenta insdertar detras del ultimo registro
Componente* Bloque::reemplazar(Componente*nuevo,Ttamanio posicion){
	Componente *anterior=componentes.at(posicion);
	Ttamanio nuevoTamanio=espacioLibre;
	nuevoTamanio-=nuevo->tamanioSerializado();
	nuevoTamanio+=anterior->tamanioSerializado();
	if(nuevoTamanio>0){
		espacioLibre=nuevoTamanio;
		std::vector<Componente*>::iterator iterador=componentes.begin();
		iterador+=posicion;
		componentes.erase(iterador);
		componentes.insert(iterador,nuevo);
		return anterior;
	}
	return NULL;
};
Ttamanio Bloque::cantidadComponentes(){
	return componentes.size();
};
