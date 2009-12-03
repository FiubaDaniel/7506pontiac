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
void EstrategiaCompresion::compresion(Almacenamiento*almacen,string archivoComprimido,unsigned tamanio){
	/**/
	std::fstream archivo_comprimido;

	archivo_comprimido.open(archivoComprimido.c_str(),fstream::trunc|fstream::out);


	unsigned *buffer=new unsigned[tamanio];

	unsigned tamanioSerializado=100;/*=almacen->getEstrategia()->getTamanioComponente();*/
	/*char* serializado=new char[tamanioSerializado];*/

	std::stringbuf serializado;
	/**/
	Componente* componente=almacen->getEstrategia()->getComponenteUsado()->clonar();

	Compresor contenedor(buffer,tamanio);
	/*guardar metadata*/

	std::string metadata=almacen->getEstrategia()->imprimirMetada();

	unsigned tamanio_meta=metadata.size();

	archivo_comprimido.write((char*)&tamanio_meta,sizeof(tamanio_meta));

	archivo_comprimido.write(metadata.c_str(),tamanio_meta);

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

			archivo_comprimido.write((char*)buffer,sizeof(unsigned)*tamanio);

			contenedor.setContinuacionCompresion(buffer,tamanio);

			contenedor.comprimirPrimeros((unsigned char*)serializado.str().data(),tamanioSerializado);
		}
	}

	contenedor.cerrar();

	archivo_comprimido.write((char*)buffer,sizeof(unsigned)*tamanio);

	archivo_comprimido.close();

	delete componente;

	delete[] buffer;
	//delete[] serializado;
}
