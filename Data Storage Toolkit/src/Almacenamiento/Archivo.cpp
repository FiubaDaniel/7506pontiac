#include "Archivo.h"

Archivo::Archivo (){};
Archivo::~Archivo (){
	archivo.close();
};
void Archivo::escribir(const void* bytes,size_t cantidad){
	archivo.write((char*)bytes,cantidad);
};
void Archivo::escribir(const void* unByte){
	archivo.put(*(char*)unByte);
};
void Archivo::leer(void* bytes,size_t cantidad){
	archivo.read((char*)bytes,cantidad);
};
void Archivo::leer(void* unBytes){
	*(char*)unBytes=archivo.get();

};
void Archivo::posicionar(size_t posicion){
	archivo.seekg(posicion);
	archivo.seekp(posicion);
};
bool Archivo::bien(){return archivo.good();};
void Archivo::posicionarAlfinal(){
	archivo.seekg(std::fstream::end);
	archivo.seekp(std::fstream::end);
};
bool Archivo::fin(){
	return archivo.eof();
}

void Archivo::crear(const char *ruta){
	archivo.open(ruta,std::fstream::out|std::fstream::trunc);
	archivo.close();
	archivo.open(ruta,std::fstream::out|std::fstream::binary|std::fstream::in);
}

void Archivo::abrir(const char *ruta){
	archivo.open(ruta,std::fstream::out|std::fstream::binary|std::fstream::in);
}
void Archivo::cerrar(){
	archivo.close();
}

size_t Archivo::posicionActual(){
	return archivo.tellg();
};
