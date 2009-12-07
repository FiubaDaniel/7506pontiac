#include "Archivo.h"

Archivo::Archivo (EstrategiaAlmacenamiento*estrategia){
	if(estrategia){
		this->estrategia=estrategia;
		this->estrategia->setAlmacenamiento(this);
	}
}
Archivo::~Archivo (){
	archivo.close();
}
void Archivo::escribir(const void* bytes,Referencia cantidad){

	archivo.write((char*)bytes,cantidad);

}
void Archivo::escribir(const void* unByte){

	archivo.put(*(char*)unByte);
}
void Archivo::leer(void* bytes,Referencia cantidad){
	archivo.read((char*)bytes,cantidad);
}
void Archivo::leer(void* unBytes){
	*(char*)unBytes=archivo.get();

}
void Archivo::posicionar(Referencia posicion){
	archivo.seekp(posicion,std::fstream::beg);
	archivo.seekg(posicion,std::fstream::beg);
	archivo.clear();
}
bool Archivo::bien(){
	return archivo.good() && archivo.is_open();
}
void Archivo::posicionarAlfinal(){
	archivo.seekg(std::fstream::end);
	archivo.seekp(std::fstream::end);
}
bool Archivo::fin(){
	return archivo.eof();
}

void Archivo::crear(const char *ruta){
	archivo.open(ruta,std::fstream::out|std::fstream::trunc|std::fstream::binary);
	archivo.close();
	archivo.open(ruta,std::ios_base::out|std::fstream::in|std::fstream::binary);
	nombre=ruta;
	if(estrategia)
		estrategia->crear();
}

bool Archivo::abrir(const char *ruta){
	archivo.open(ruta,std::fstream::out|std::fstream::binary|std::fstream::in);
	nombre=ruta;
	if(not archivo.is_open())
		return false;
	if(estrategia)
		if(not estrategia->abrir()){
			archivo.close();
			return false;
		};
	return true;
}
void Archivo::cerrar(){
	if(estrategia)
		estrategia->cerrar();
	archivo.close();
}

void Archivo::clear(){
	archivo.clear();
}

Referencia Archivo::posicionActual(){
	return archivo.tellg();
}
Almacenamiento*Archivo::clonar(){
	Archivo*clon=new Archivo(NULL);
	clon->estrategia=NULL;
	return clon;
}
