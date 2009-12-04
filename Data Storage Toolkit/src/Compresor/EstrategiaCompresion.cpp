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

}

void EstrategiaCompresion::compresionArbol(BSharpTree* arbol,string archivoComprimido,unsigned tamanio_buffer_comprimido){

	//Creo el contenedor adecuado a las condiciones dadas.
	unsigned *buffer=new unsigned[tamanio_buffer_comprimido];
	Compresor contenedor(buffer,tamanio_buffer_comprimido);
	unsigned tamanioSerializado = arbol->tamanioBloque();
	std::stringbuf serializado;

	//abro el archivo comprimido. Su nombre será el nombre del archivo original + _Comprimido.
	std::fstream archivo_comprimido;
	archivo_comprimido.open(archivoComprimido.c_str(),fstream::trunc|fstream::out|fstream::binary);

	if(archivo_comprimido.is_open()){
		Nodo* nodo;
		//Paso la metadata (que no se comprime) al archivo de datos comprimido
		std::string metadata=arbol->getMetadata();
		unsigned tamanio_meta=metadata.size();
		archivo_comprimido.write((char*)&tamanio_meta,sizeof(tamanio_meta));
		archivo_comprimido.write(metadata.data(),tamanio_meta);

		//Comienzo la compresion

		//Comprimo el primero
		arbol->posicionarArchivo();
		nodo->serializate(&serializado);
		contenedor.comprimirPrimeros((unsigned char*)serializado.str().data(),tamanioSerializado);

		//Comprimo los siguiente
		while(arbol->siguienteAlmacenado(nodo)){
			serializado.pubseekpos(0,ios::out|ios::binary|ios::in);
			nodo->serializate(&serializado);
			if(not contenedor.agregar((unsigned char*)serializado.str().data(),tamanioSerializado)){
				/*no pudo agregar,cierra el contenedor , graba y empieza uno nuevo */
				contenedor.cerrar();

				archivo_comprimido.write((char*)buffer,sizeof(unsigned)*tamanio_buffer_comprimido);

				contenedor.setContinuacionCompresion(buffer,tamanio_buffer_comprimido);

				contenedor.comprimirPrimeros((unsigned char*)serializado.str().data(),tamanioSerializado);
			}
			delete nodo;
		}

		contenedor.cerrar();
		archivo_comprimido.write((char*)buffer,sizeof(unsigned)*tamanio_buffer_comprimido);
		archivo_comprimido.close();
		delete nodo;
	}
	delete[] buffer;
}

void EstrategiaCompresion::descompresionArbol(BSharpTree*arbol,string archivoComprimido){

	std::fstream archivo_comprimido;
	std::stringbuf serializado;

	unsigned tamanio_comprimido;//Tamanio del contenedor.
	/*recuperao tamanio del buffer */
	archivo_comprimido.read((char*)&tamanio_comprimido,sizeof(tamanio_comprimido));
	unsigned *buffer=new unsigned[tamanio_comprimido];

	//Creo el contenedor segun datos recuperados
	Compresor contenedor(buffer,tamanio_comprimido);
	//Abro el archivo comprimido
	archivo_comprimido.open(archivoComprimido.c_str(),fstream::in|fstream::binary);
	//Comienza la descompresion
	if(archivo_comprimido.is_open()){

		//Obtengo metadata
		unsigned tamanio_meta;
		archivo_comprimido.read((char*)&tamanio_meta,sizeof(tamanio_meta));
		char* metadata=new char[tamanio_meta];
		archivo_comprimido.read(metadata,tamanio_meta);
		arbol->setMetadata(metadata);

		//Descompresion de Nodos
		arbol->posicionarArchivo();
		std::string descomprimido;
		try{
			while(archivo_comprimido.peek()!= EOF and not archivo_comprimido.eof()){
				/*recupero una tira de componentes serializados*/
				archivo_comprimido.read((char*)buffer,sizeof(unsigned)*tamanio_comprimido);

				descomprimido.clear();

				contenedor.descomprimir(buffer,descomprimido,tamanio_comprimido);

				while(not descomprimido.empty()){
					/*escribo los componentes q recupere*/

					serializado.str(descomprimido);//inicializo el buff con el string descomprimido

					serializado.pubseekpos(0,ios::out|ios::binary|ios::in);

					arbol->escribir(&serializado);

					descomprimido.erase(0,arbol->tamanioBloque());//elimino el primer registro/bloque del string

				}
			}
		}catch(...){
			cout<<"ERROR: DESERIALIZAR"<<endl;
		}
		archivo_comprimido.close();

	}
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

	//componente->imprimir(cout);cout<<endl;

	contenedor.comprimirPrimeros((unsigned char*)serializado.str().data(),tamanioSerializado);
	int cont=1;//TODO sacar
	/*los siguiente componente*/
	while(almacen->leer(componente)){

		serializado.pubseekpos(0,ios::out|ios::binary|ios::in);//TODO estaba abajo

		componente->serializar(serializado);

		//componente->imprimir(cout);cout<<endl;

		if(not contenedor.agregar((unsigned char*)serializado.str().data(),tamanioSerializado)){

			/*no pudo agregar,cierra el contenedor , graba y empieza uno nuevo */
			contenedor.cerrar();

			archivo_comprimido.write((char*)buffer,sizeof(unsigned)*tamanio_buffer_comprimido);

			contenedor.reiniciarBuffer();

			contenedor.comprimirPrimeros((unsigned char*)serializado.str().data(),tamanioSerializado);
		}
		cont++;

	}
	cout<<cont<<endl;
	contenedor.cerrar();

	archivo_comprimido.write((char*)buffer,sizeof(unsigned)*tamanio_buffer_comprimido);

	archivo_comprimido.close();

	delete componente;

	delete[] buffer;

}
void EstrategiaCompresion::descompresion(Almacenamiento*almacen,string archivoComprimido){

	std::fstream archivo_comprimido;

	std::stringbuf serializado;
	//TODO saque trunc
	archivo_comprimido.open(archivoComprimido.c_str(),fstream::in|fstream::binary);
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

	almacen->getEstrategia()->setMetadata(metadata);

	/*Recupero los componentes*/

	almacen->posicionarComponente(0);

	std::string descomprimido;

	try{

		while(archivo_comprimido.peek()!= EOF and not archivo_comprimido.eof()){
			/*recupero una tira de componentes serializados*/

			archivo_comprimido.read((char*)buffer,sizeof(unsigned)*tamanio_comprimido);

			contenedor.setExtremos();

			descomprimido.clear();

			contenedor.descomprimir(buffer,descomprimido,tamanio_comprimido);

			serializado.str(descomprimido);//TODO cambio

			serializado.pubseekpos(0,ios::out|ios::binary|ios::in);

			while(serializado.sgetc()!=EOF){
				/*escribo los componentes q recupere*/

				componente->deserializar(serializado);

				componente->imprimir(cout);cout<<endl;

				almacen->escribir(componente);

			}
		}
	}catch(...){
		cout<<"ERROR: DESERIALIZAR"<<endl;
	}
	archivo_comprimido.close();

	delete componente;

	delete[] buffer;

}
