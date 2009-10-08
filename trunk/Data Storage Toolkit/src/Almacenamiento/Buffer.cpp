#include "Buffer.h"

Buffer::Buffer(){
	archivo =new std::stringstream(std::ios_base::out|std::ios_base::binary|std::ios_base::in);
};
Buffer::~Buffer (){
	delete archivo;
};
void Buffer::escribir(const void* bytes,size_t cantidad){
	archivo->write((char*)&bytes,cantidad);
};
void Buffer::escribir(const void* unByte){
	archivo->put(*(char*)unByte);

};
void Buffer::leer(void* bytes,size_t cantidad){
	archivo->read((char*)&bytes,cantidad);
};
void Buffer::leer(void* unBytes){
	*(char*)unBytes=archivo->get();
};
void Buffer::posicionarByte(size_t posicion){
	archivo->seekg(posicion);
	archivo->seekp(posicion);
};
bool Buffer::bienByte(){return archivo->good();};
void Buffer::posicionarComponente(size_t nroCompuesto){

};
void Buffer::escribir(Compuesto*compuesto){

};
void Buffer::leer(Compuesto*compuesto){

};
bool Buffer::fin(){return false;};
bool Buffer::bien(){return false;};
