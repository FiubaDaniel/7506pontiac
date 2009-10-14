/*
 * EABloques.cpp
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#include "EABloques.h"

EABloques::EABloques(const char*rutaArchivoEspacios,Bloque* tipoBloque,Ttamanio tamanioBloque) {
	this->bloque=(Bloque*)tipoBloque->clonar();
	capacBloque=tamanioBloque;
	bloqueSerializado=new char[tamanioBloque];
	archivoEspacioLibre.open(rutaArchivoEspacios,std::fstream::binary | std::fstream::in| std::fstream::out );
	porcCarga=0.8;
};

EABloques::~EABloques() {
	delete[] bloqueSerializado;
	delete bloque;
	archivoEspacioLibre.close();
};
void EABloques::posicionarComponente(Almacenamiento* almacen,size_t nroCompuesto){
	corrienteBloque=nroCompuesto;
};
void EABloques::escribir(Almacenamiento* almacen,Componente*compuesto){
	Bloque*bloque=NULL;
	if((bloque=dynamic_cast<Bloque*>(compuesto))){
		if(bloque->tamanioSerializado()<=capacBloque){
			std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
			buf.pubsetbuf(bloqueSerializado,capacBloque);
			bloque->serializar(buf);
			almacen->posicionarByte(capacBloque*corrienteBloque);
			almacen->escribir(bloqueSerializado,capacBloque);
		}//TODO demaciado grande
	}//TODO no es un bloque
};

void EABloques::leer(Almacenamiento* almacen,Componente*compuesto){
	Bloque*bloque=NULL;
	if((bloque=dynamic_cast<Bloque*>(compuesto))){
		std::stringbuf buf(std::ios_base::binary | std::ios_base::in );
		buf.pubsetbuf(bloqueSerializado,capacBloque);
		almacen->posicionarByte(capacBloque*corrienteBloque);
		almacen->leer(bloqueSerializado,capacBloque);
		bloque->deserializar(buf);
	}//TODO no es un bloque
};
bool EABloques::buscarEspacioLibre(Ttamanio espacio,size_t & nroBloque){
	bool encontrado=false;
	archivoEspacioLibre.seekg(std::fstream::beg);
	nroBloque=-1;
	while(!encontrado && archivoEspacioLibre.eof()&& archivoEspacioLibre.good()){
		Ttamanio disponibles;
		nroBloque++;
		archivoEspacioLibre.read((char*)&libres,sizeof(Ttamanio));
		disponibles=libres-(1-porcCarga)*capacBloque;
		if(disponibles>espacio)
			encontrado=true;
	}
	return encontrado;
};

size_t EABloques::insertar(Almacenamiento* almacen,Componente*componente){
	size_t nroBloque;
	Ttamanio  nuevoTamanio;
	if(dynamic_cast<Compuesto*>(componente)!=NULL);//TODO exepcion
	Ttamanio espacioNecesario=componente->tamanioSerializado();
	if(buscarEspacioLibre(espacioNecesario,nroBloque)){
		posicionarComponente(almacen,nroBloque);
		leer(almacen,bloque);
		bloque->agregar(componente);
	}else{
		bloque->inicializar(componente);
		nroBloque++;
	}
	nuevoTamanio=bloque->tamanioSerializado();

	archivoEspacioLibre.seekp(nroBloque*sizeof(Ttamanio));
	archivoEspacioLibre.write((char*)&nuevoTamanio,sizeof(Ttamanio));

	posicionarComponente(almacen,nroBloque);
	escribir(almacen,bloque);
	return nroBloque;
};
bool EABloques::modificar(Almacenamiento*almacen,Componente*componente){
	Ttamanio nuevoTamanio;
	archivoEspacioLibre.seekg(corrienteBloque*sizeof(Ttamanio));
	archivoEspacioLibre.read((char*)&nuevoTamanio,sizeof(Ttamanio));
	almacen->leer(bloque);
	Ttamanio nroComponente=0;
	//TODO busqueda del componente nroComponente= componente a reemplazar
	nuevoTamanio+=bloque->get(nroComponente)->tamanioSerializado()-componente->tamanioSerializado();
	if(nuevoTamanio< capacBloque){
		Componente* eliminado=bloque->reemplazar(componente,nroComponente);
		if(eliminado!=NULL){
			escribir(almacen,bloque);
			nuevoTamanio=bloque->tamanioSerializado();
			archivoEspacioLibre.seekp(corrienteBloque*sizeof(Ttamanio));
			archivoEspacioLibre.write((char*)&nuevoTamanio,sizeof(Ttamanio));
			delete eliminado;
			return true;
		}
	}
	return false;
};
bool EABloques::buscarComponente(Componente*comp,Ttamanio & posicion){
	/*
	 * clave.set(dynamic_cast<Registro*>(componente));
	 * Clave aux=clave.clonar();
	 */
	for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
		//TODO generar clave para el componente i
		/*
		 * if(comparar(clave,aux)==0){
		 *       posicion=i;
		 *       delete aux;
		 *       return true;
		 *  }
		 */
	}
	/*delete aux;*/
	return false;
};
void EABloques::eliminar(Almacenamiento*almacen,Componente*componente){
	leer(almacen,bloque);
	Ttamanio nroComponente;
	if(!buscarComponente(componente,nroComponente)) return;
	bloque->eliminar(nroComponente);
	Ttamanio nuevoTamanio;
	archivoEspacioLibre.seekg(corrienteBloque*sizeof(Ttamanio));
	archivoEspacioLibre.read((char*)&nuevoTamanio,sizeof(Ttamanio));
	escribir(almacen,bloque);
	nuevoTamanio=bloque->tamanioSerializado();
	archivoEspacioLibre.seekp(corrienteBloque*sizeof(Ttamanio));
	archivoEspacioLibre.write((char*)&nuevoTamanio,sizeof(Ttamanio));
};
bool EABloques::buscar(Almacenamiento*almacen,Componente*componente){
	return false;
};
