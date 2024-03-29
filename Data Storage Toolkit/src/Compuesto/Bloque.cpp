/*
 * Bloque.cpp
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */

#include "Bloque.h"
/*------------------------------------------------------------------------------------*/
Bloque::Bloque(){}
Bloque::Bloque(Componente*componente) : Bloque::Componente(){
	componentes.push_back(componente->clonar());
}
Bloque::~Bloque() {
	for(Ttamanio i=0; i<componentes.size();i++){
		delete componentes.at(i);
	}
}
Componente* Bloque::clonar()const{
	Bloque* clon=new Bloque(componentes.at(0)->clonar());
	return clon;
}
bool Bloque::esfijo(){
	return false;
}
Ttamanio Bloque::deserializar(std::streambuf&entrada){
	unsigned char nrocomponetes;
	Ttamanio offset=sizeof(unsigned char);
	entrada.sgetn((char*)&nrocomponetes,offset);
	Ttamanio i=0;
	while(i<nrocomponetes and i<componentes.size()){
		offset+=componentes.at(i)->deserializar(entrada);
		i++;
	}
	/*le sobran componentes*/
	while(i<componentes.size()){
		delete componentes.at(i);
		componentes.erase(componentes.begin()+i);
	}
	/*le faltan componentes*/
	Componente* auxiliar;
	while(i<nrocomponetes){
		auxiliar=componentes.at(0)->clonar();
		offset+=auxiliar->deserializar(entrada);
		componentes.push_back(auxiliar);
		i++;
	}
	return offset;
}
Ttamanio Bloque::serializar(std::streambuf&salida)const{
	unsigned char nrocomponetes=componentes.size();
	Ttamanio offset=sizeof(unsigned char);
	salida.sputn((char*)&nrocomponetes,offset);
	for(Ttamanio i=0;i<nrocomponetes;i++){
		offset+=componentes.at(i)->serializar(salida);
	}
	return offset;
}
Ttamanio Bloque::tamanioSerializado()const{
	Ttamanio offset=sizeof(unsigned char);
	for(Ttamanio i=0;i<componentes.size();i++){
		offset+=componentes.at(i)->tamanioSerializado();
	}
	return offset;
}
/*------------------------------------------------------------------------------------*/
Componente* Bloque::get(Ttamanio nroComponente){
	return componentes.at(nroComponente);
}
bool Bloque::eliminar(Ttamanio posicion){
	if(posicion>=componentes.size())
		return false;
	delete componentes.at(posicion);
	componentes.erase(componentes.begin()+posicion);
	return true;
}
bool Bloque::insertar(Componente* nuevo,Ttamanio posicion){
	componentes.insert(componentes.begin()+posicion,nuevo);
	return true;
}
//intenta insertar detras del registro actual si puede
bool Bloque::agregar(Componente* nuevo){
	componentes.push_back(nuevo->clonar());
	return true;
}//intenta insdertar detras del ultimo registro
Componente* Bloque::reemplazar(Componente*nuevo,Ttamanio posicion){
	Componente *anterior=componentes.at(posicion);
	componentes.erase(componentes.begin()+posicion);
	componentes.insert(componentes.begin()+posicion,nuevo->clonar());
	return anterior;
}
Ttamanio Bloque::cantidadComponentes(){
	return componentes.size();
}
void Bloque::inicializar(Componente*componente){
	while(!componentes.empty()){
		delete componentes.back();
		componentes.pop_back();
	}
	componentes.push_back(componente->clonar());
}
void Bloque::imprimir(std::ostream&salida)const{
	salida<<"º";
	salida<<componentes.size();
	salida<<"("<<tamanioSerializado()<<")"<<std::endl;
	for(Ttamanio i=0;i<componentes.size();i++){
		componentes[i]->imprimir(salida);
		salida<<" : "<<"("<<componentes[i]->tamanioSerializado()<<")"<<std::endl;
	}
}
std::ostream& operator<<(std::ostream&out,Bloque&bloque){
	bloque.imprimir(out);
	return out;
}
bool Bloque::copiar(Componente*componente){
	Bloque* fuente=dynamic_cast<Bloque*>(componente);
	if(fuente==NULL)
		return false;
	inicializar(fuente->get(0));
	for (Ttamanio i=1;i<fuente->cantidadComponentes();i++){
		componentes.push_back(fuente->get(i)->clonar());
	}
	return true;
}
