/*
 * EstrategiaCompresion.cpp
 *
 *  Created on: 03/12/2009
 *      Author: daniel
 */

#include "EstrategiaCompresion.h"

EstrategiaCompresion::EstrategiaCompresion() {
	// TODO Auto-generated constructor stub

}

EstrategiaCompresion::~EstrategiaCompresion() {
	// TODO Auto-generated destructor stub
}
void EstrategiaCompresion::compresion(Almacenamiento*almacen,string archivoComprimido,unsigned tamanio_buffer_comprimido){
	/**/
	std::fstream archivo_comprimido;

	archivo_comprimido.open(archivoComprimido.c_str(),fstream::trunc|fstream::out|fstream::binary);

	unsigned *buffer=new unsigned[tamanio_buffer_comprimido];

	unsigned tamanioSerializado=almacen->getEstrategia()->getTamanioComponenteUsado();

	std::stringbuf serializado;
	/**/
	Componente* componente=almacen->getEstrategia()->getComponenteUsado()->clonar();

	Compresor contenedor(buffer,tamanio_buffer_comprimido);

	archivo_comprimido.write((char*)&tamanio_buffer_comprimido,sizeof(tamanio_buffer_comprimido));

	/*guardar metadata*/
	std::string metadata=almacen->getEstrategia()->getMetadata();

	unsigned tamanio_meta=metadata.size();

	archivo_comprimido.write((char*)&tamanio_meta,sizeof(tamanio_meta));

	archivo_comprimido.write(metadata.data(),tamanio_meta);

	/*comprimiendo*/
	/*comprime el primer componente*/
	almacen->posicionarComponente(0);

	almacen->leer(componente);

	componente->serializar(serializado);

	contenedor.comprimirPrimeros((unsigned char*)serializado.str().data(),tamanioSerializado);

	/*los siguiente componente*/
	while(almacen->leer(componente)){

		componente->serializar(serializado);

		if(not contenedor.agregar((unsigned char*)serializado.str().data(),tamanioSerializado)){

			/*no pudo agregar,cierra el contenedor , graba y empieza uno nuevo */
			contenedor.cerrar();

			archivo_comprimido.write((char*)buffer,sizeof(unsigned)*tamanio_buffer_comprimido);

			contenedor.reiniciarBuffer();

			contenedor.comprimirPrimeros((unsigned char*)serializado.str().data(),tamanioSerializado);
		}
	}

	contenedor.cerrar();

	archivo_comprimido.write((char*)buffer,sizeof(unsigned)*tamanio_buffer_comprimido);

	archivo_comprimido.close();

	delete componente;

	delete[] buffer;

}
void EstrategiaCompresion::descompresion(Almacenamiento*almacen,string archivoComprimido){

	std::fstream archivo_comprimido;

	std::stringbuf serializado;

	archivo_comprimido.open(archivoComprimido.c_str(),fstream::trunc|fstream::in|fstream::binary);
	/*todo checkear si abierto*/

	if(not archivo_comprimido.is_open())
		return;

	Componente* componente=almacen->getEstrategia()->getComponenteUsado()->clonar();

	unsigned tamanio_comprimido;

	/*recuperao tamanio del buffer */
	archivo_comprimido.read((char*)&tamanio_comprimido,sizeof(tamanio_comprimido));

	unsigned *buffer=new unsigned[tamanio_comprimido];

	Compresor contenedor(buffer,tamanio_comprimido);
	/*recupero la metadata del almacenamiento*/

	unsigned tamanio_meta;

	archivo_comprimido.read((char*)&tamanio_meta,sizeof(tamanio_meta));

	char* metadata=new char[tamanio_meta];

	archivo_comprimido.read(metadata,tamanio_meta);

	//almacen->getEstrategia()->setMetadata(metadata);

	/*Recupero los componentes*/

	almacen->posicionarComponente(0);

	std::string descomprimido;

	while(archivo_comprimido.peek()!= EOF){
		/*recupero una tira de componentes serializados*/

		archivo_comprimido.read((char*)buffer,sizeof(unsigned)*tamanio_comprimido);

		contenedor.setExtremos();

		contenedor.descomprimir(buffer,descomprimido,tamanio_comprimido);

		serializado.pubsetbuf((char*)descomprimido.data(),descomprimido.size());

		while(serializado.sgetc()!=EOF){
			/*escribo los componentes q recupere*/

			componente->deserializar(serializado);

			almacen->escribir(componente);

		}
	}

	archivo_comprimido.close();

	delete componente;

	delete[] buffer;

}
