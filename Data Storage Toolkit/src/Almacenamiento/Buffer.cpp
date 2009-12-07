#include "Buffer.h"

Buffer::Buffer(){
	archivo =NULL;
	estrategia=NULL;
}
Buffer::Buffer(unsigned longitudBuffer){
	this->longitudBuffer=longitudBuffer;
	archivo=NULL;
	estrategia=NULL;
}
Buffer::Buffer(EstrategiaAlmacenamiento*estrategia,unsigned longitudBuffer){
	this->longitudBuffer=longitudBuffer;
	archivo=NULL;
	if(estrategia){
	this->estrategia=estrategia;
	this->estrategia->setAlmacenamiento(this);
	}
}
Buffer::~Buffer (){
	if(archivo)
		cerrar();
}
bool Buffer::abrir(const char* nombre){
	this->nombre=nombre;
	archivo =new std::stringstream(std::ios_base::out|std::ios_base::binary|std::ios_base::in);
	return true;
	if(estrategia)
		estrategia->abrir();
}
void Buffer::crear(const char* nombre){
	this->nombre=nombre;
	archivo =new std::stringstream(std::ios_base::out|std::ios_base::binary|std::ios_base::in);
	if(estrategia)
		estrategia->crear();
}
void Buffer::escribir(const void* bytes,Referencia cantidad){
	if(archivo)
		archivo->write((char*)bytes,cantidad);
}
void Buffer::escribir(const void* unByte){
	if(archivo) archivo->put(*(char*)unByte);

}
void Buffer::leer(void* bytes,Referencia cantidad){
	if(archivo) archivo->read((char*)bytes,cantidad);
}
void Buffer::leer(void* unBytes){
	*(char*)unBytes=archivo->get();
}
void Buffer::posicionar(Referencia posicion){
	if(archivo) {archivo->seekg(posicion);
	archivo->seekp(posicion);}
}
bool Buffer::bien(){
	if(!archivo)
		return false;
	return archivo->good();
}
void Buffer::posicionarAlfinal(){
	if(archivo) {archivo->seekg(std::ios_base::end);
	archivo->seekp(std::ios_base::end);}
}
bool Buffer::fin(){
	if(!archivo)
		return true;
	return archivo->eof();
}
void Buffer::clear(){
	if(archivo) archivo->clear();
}
Referencia Buffer::posicionActual(){
	if(archivo)
		return archivo->tellg();
	else return 0;
}
void Buffer::cerrar(){
	if(archivo) {
		if(estrategia)
			estrategia->cerrar();
		delete archivo;
		archivo=NULL;
	}
}
Almacenamiento*Buffer::clonar(){
	Buffer*clon=new Buffer(NULL,longitudBuffer);
	clon->estrategia=NULL;
	return clon;
}
