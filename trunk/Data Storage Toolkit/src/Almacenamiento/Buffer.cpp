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
void Buffer::posicionar(size_t posicion){
	archivo->seekg(posicion);
	archivo->seekp(posicion);
};
bool Buffer::bien(){
	return archivo->good();
};
void Buffer::posicionarAlfinal(){
	archivo->seekg(std::ios_base::end);
	archivo->seekp(std::ios_base::end);
};
bool Buffer::fin(){
	return archivo->eof();
}
void Buffer::reiniciar(){
	archivo->clear();
};
size_t Buffer::posicionActual(){
	return archivo->tellg();
};
