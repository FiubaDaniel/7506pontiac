#include "Archivo.h"

Archivo::Archivo (const char*ruta){
	archivo.open(ruta,std::fstream::out|std::fstream::app);
	archivo.close();
	archivo.open(ruta,std::fstream::out|std::fstream::binary|std::fstream::in);
};
Archivo::~Archivo (){
	archivo.close();
};
void Archivo::escribir(const void* bytes,size_t cantidad){
	archivo.write((char*)&bytes,cantidad);
	archivo.flush();
};
void Archivo::escribir(const void* unByte){
	archivo.put(*(char*)unByte);
};
void Archivo::leer(void* bytes,size_t cantidad){
	archivo.read((char*)&bytes,cantidad);
	archivo.sync();
};
void Archivo::leer(void* unBytes){
	*(char*)unBytes=archivo.get();

};
void Archivo::posicionarByte(size_t posicion){
	archivo.seekg(posicion);
	archivo.seekp(posicion);
};
bool Archivo::bienByte(){return archivo.good();};
void Archivo::posicionarCompuesto(size_t nroCompuesto){

};
void Archivo::escribir(Compuesto*compuesto){

};
void Archivo::leer(Compuesto*compuesto){

};
bool Archivo::fin(){
	return false;
};
bool Archivo::bien(){
	return false;
};
