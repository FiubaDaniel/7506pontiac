#include "Archivo.h"

Archivo::Archivo (){}
Archivo::~Archivo (){
	archivo.close();
}
void Archivo::escribir(const void* bytes,size_t cantidad){

	archivo.write((char*)bytes,cantidad);

}
void Archivo::escribir(const void* unByte){

	archivo.put(*(char*)unByte);
}
void Archivo::leer(void* bytes,size_t cantidad){
	archivo.read((char*)bytes,cantidad);
}
void Archivo::leer(void* unBytes){
	*(char*)unBytes=archivo.get();

}
void Archivo::posicionar(size_t posicion){
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
}

bool Archivo::abrir(const char *ruta){
	archivo.open(ruta,std::fstream::out|std::fstream::binary|std::fstream::in);
	nombre=ruta;
	return archivo.is_open();
}
void Archivo::cerrar(){
	archivo.close();
}

void Archivo::reiniciar(){
	archivo.clear();
}

size_t Archivo::posicionActual(){
	return archivo.tellg();
}
