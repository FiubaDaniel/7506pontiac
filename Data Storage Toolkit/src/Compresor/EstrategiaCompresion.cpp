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
	std::fstream archivo_comprimido;

	//Creo el contenedor adecuado a las condiciones dadas.
	unsigned *buffer=new unsigned[tamanio_buffer_comprimido];
	Compresor contenedor(buffer,tamanio_buffer_comprimido);
	unsigned tamanioSerializado = arbol->tamanioBloque();
	std::stringbuf serializado;

	archivo_comprimido.open(archivoComprimido.c_str(),fstream::trunc|fstream::out|fstream::binary);
	if(!archivo_comprimido.is_open()){
		return;
	}
	archivo_comprimido.seekp(0);
	archivo_comprimido.write((char*)&tamanio_buffer_comprimido,sizeof(tamanio_buffer_comprimido));

	Nodo* nodo;

	//Paso la metadata (que no se comprime) al archivo de datos comprimido
	std::string metadata=arbol->getMetadata();
	unsigned tamanio_meta=metadata.size();
	archivo_comprimido.write((char*)&tamanio_meta,sizeof(tamanio_meta));
	archivo_comprimido.write(metadata.data(),tamanio_meta);

	//Comienzo la compresion
	//Comprimo el primero
	arbol->posicionarArchivo();
	arbol->siguienteAlmacenado(nodo);

	nodo->serializate(&serializado);

	contenedor.comprimirPrimeros((unsigned char*)serializado.str().data(),tamanioSerializado);

	short cont = 1;
	//Comprimo los siguiente
	while(arbol->siguienteAlmacenado(nodo)){

		serializado.pubseekpos(0,ios::out|ios::binary|ios::in);

		nodo->serializate(&serializado);

		if(not contenedor.agregar((unsigned char*)serializado.str().data(),tamanioSerializado)){

			/*no pudo agregar,cierra el contenedor , graba y empieza uno nuevo */
			contenedor.cerrar();

			archivo_comprimido.write((char*)&cont,sizeof(cont));

			archivo_comprimido.write((char*)buffer,sizeof(unsigned)*tamanio_buffer_comprimido);

			contenedor.setContinuacionCompresion(buffer,tamanio_buffer_comprimido);

			contenedor.comprimirPrimeros((unsigned char*)serializado.str().data(),tamanioSerializado);

			cont=1;

		}else cont++;

		delete nodo;
	}

	contenedor.cerrar();

	archivo_comprimido.write((char*)&cont,sizeof(cont));

	archivo_comprimido.write((char*)buffer,sizeof(unsigned)*tamanio_buffer_comprimido);

	archivo_comprimido.close();

	delete[] buffer;
}

void EstrategiaCompresion::compresionIndice(string nombreIndice,string archivoComprimido,unsigned tamanio_buffer_comprimido){
	std::fstream archivo_indice;
	std::fstream archivo_comprimido;

	unsigned *comprimido=new unsigned[tamanio_buffer_comprimido];

	Compresor contenedor(comprimido,tamanio_buffer_comprimido);

	archivo_indice.open(nombreIndice.c_str(),fstream::in|fstream::binary);

	archivo_comprimido.open(archivoComprimido.c_str(),fstream::trunc|fstream::out|fstream::binary);

	if(archivo_indice.is_open()&&archivo_comprimido.is_open()){

		archivo_indice.seekg(0);

		archivo_comprimido.seekp(0);

		archivo_comprimido.write((char*)&tamanio_buffer_comprimido,sizeof(tamanio_buffer_comprimido));

		//Comienza compresion.
		//Gurado primero.
		unsigned tamanio_sin_comprimir=tamanio_buffer_comprimido/2;

		unsigned char sin_comprimir[tamanio_sin_comprimir];

		archivo_indice.read((char*)sin_comprimir,tamanio_sin_comprimir);

		contenedor.comprimirPrimeros(sin_comprimir,tamanio_sin_comprimir);

		//Comprimo el resto
		short cont = 1;

		while(archivo_comprimido.peek()!= EOF and not archivo_indice.eof()){

			archivo_indice.read((char*)sin_comprimir,tamanio_sin_comprimir);

			if(not contenedor.agregar(sin_comprimir,tamanio_sin_comprimir)){

				contenedor.cerrar();

				archivo_comprimido.write((char*)&cont,sizeof(cont));

				archivo_comprimido.write((char*)comprimido,sizeof(unsigned)*tamanio_buffer_comprimido);

				contenedor.reiniciarBuffer();

				contenedor.comprimirPrimeros(sin_comprimir,tamanio_sin_comprimir);

				cont=1;
			}else cont++;
		}

		contenedor.cerrar();

		archivo_comprimido.write((char*)comprimido,sizeof(unsigned)*tamanio_buffer_comprimido);

		delete[] comprimido;

		archivo_indice.clear();
	}
	archivo_comprimido.close();
	archivo_indice.close();
}

void EstrategiaCompresion::descompresionInsdice(string nombre_indice,string nombre_comprimido){

	std::string descomprimido;
	std::fstream archivo_indice;
	std::fstream archivo_comprimido;

	archivo_indice.open(nombre_indice.c_str(),fstream::trunc|fstream::out|fstream::binary);
	archivo_comprimido.open(nombre_comprimido.c_str(),fstream::in|fstream::binary);

	if(archivo_indice.is_open()&&archivo_comprimido.is_open()){

		archivo_indice.seekp(0);
		archivo_comprimido.seekg(0);

		//Obtenego el tamaño de los contenedores
		unsigned tamanio_buffer_comprimido;
		archivo_comprimido.read((char*)&tamanio_buffer_comprimido,sizeof(tamanio_buffer_comprimido));

		//Creo el contenedor
		unsigned *buffer=new unsigned[tamanio_buffer_comprimido];
		Compresor contenedor(buffer,tamanio_buffer_comprimido);

		unsigned tamanio_sin_comprimir=tamanio_buffer_comprimido/2;

		//Comienzo a descomprimir

		while(archivo_comprimido.peek()!= EOF and not archivo_comprimido.eof()){

			short cont;

			archivo_comprimido.read((char*)&cont,sizeof(cont));

			archivo_comprimido.read((char*)buffer,sizeof(unsigned)*tamanio_buffer_comprimido);

			contenedor.setCaracteres(tamanio_sin_comprimir*cont);

			contenedor.descomprimir(buffer,descomprimido,tamanio_buffer_comprimido);

			archivo_indice.write(descomprimido.data(),descomprimido.size());

		}
		delete[] buffer;
		archivo_comprimido.clear();// saca el flag de eof
	}
	archivo_comprimido.close();
	archivo_indice.close();
}

/*
 * Arbol creado, con archivo de espacios libres seteado, archivo del arbol abierto y comparador seteado.
 */
void EstrategiaCompresion::descompresionArbol(BSharpTree*arbol,string archivoComprimido){

	std::fstream archivo_comprimido;
	std::stringbuf serializado;

	archivo_comprimido.open(archivoComprimido.c_str(),fstream::in|fstream::binary);

	if(!archivo_comprimido.is_open()){
		return;
	}
	unsigned tamanio_comprimido;//Tamanio del contenedor.

	archivo_comprimido.seekg(0);

	archivo_comprimido.read((char*)&tamanio_comprimido,sizeof(tamanio_comprimido));

	unsigned *buffer=new unsigned[tamanio_comprimido];

	//Creo el contenedor segun datos recuperados

	Compresor contenedor(buffer,tamanio_comprimido);

	//Obtengo metadata
	unsigned tamanio_meta;

	archivo_comprimido.read((char*)&tamanio_meta,sizeof(tamanio_meta));

	char* metadata=new char[tamanio_meta];

	archivo_comprimido.read(metadata,tamanio_meta);

	arbol->setMetadata(metadata);

	unsigned tamanio_serializado = arbol->tamanioBloque();

	//Descompresion de Nodos
	arbol->posicionarArchivo();

	std::string descomprimido;

	try{
		descomprimido.clear();

		while(archivo_comprimido.peek()!= EOF and not archivo_comprimido.eof()){
			/*recupero una tira de componentes serializados*/
			short cont;

			archivo_comprimido.read((char*)&cont,sizeof(cont));

			archivo_comprimido.read((char*)buffer,sizeof(unsigned)*tamanio_comprimido);

			descomprimido.clear();

			contenedor.setCaracteres(tamanio_serializado*cont);

			contenedor.descomprimir(buffer,descomprimido,tamanio_comprimido);

			while( not descomprimido.empty()){
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
	arbol->recomponerRaiz();

	archivo_comprimido.close();

	delete buffer;
}


void EstrategiaCompresion::compresion(Almacenamiento*almacen,unsigned tamanio_buffer_comprimido){

	string archivoComprimido=almacen->getNombre();

	archivoComprimido+=".arit";
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

	short cont=1;//TODO sacar

	while(almacen->leer(componente)){

		serializado.pubseekpos(0,ios::out|ios::binary|ios::in);//TODO estaba abajo

		componente->serializar(serializado);

		if(not contenedor.agregar((unsigned char*)serializado.str().data(),tamanioSerializado)){

			/*no pudo agregar,cierra el contenedor , graba y empieza uno nuevo */
			contenedor.cerrar();



			archivo_comprimido.write((char*)&cont,sizeof(cont));

			archivo_comprimido.write((char*)buffer,sizeof(unsigned)*tamanio_buffer_comprimido);

			contenedor.reiniciarBuffer();

			contenedor.comprimirPrimeros((unsigned char*)serializado.str().data(),tamanioSerializado);

			cont=1;

		}else cont++;

	}


	contenedor.cerrar();



	archivo_comprimido.write((char*)&cont,sizeof(cont));

	archivo_comprimido.write((char*)buffer,sizeof(unsigned)*tamanio_buffer_comprimido);

	archivo_comprimido.close();

	delete componente;

	delete[] buffer;

}
bool EstrategiaCompresion::descompresion(Almacenamiento*almacen){

	string archivoComprimido=almacen->getNombre();

	archivoComprimido+=".arit";

	std::fstream archivo_comprimido;

	std::stringbuf serializado;
	//TODO saque trunc
	archivo_comprimido.open(archivoComprimido.c_str(),fstream::in|fstream::binary);
	/*todo checkear si abierto*/

	if(not archivo_comprimido.is_open())
		return false;

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
	//todo agregado
	unsigned tamanio_serializado=almacen->getEstrategia()->getTamanioComponenteUsado();

		descomprimido.clear();

		while(archivo_comprimido.peek()!= EOF and not archivo_comprimido.eof()){
			/*recupero una tira de componentes serializados*/
			short cont;

			archivo_comprimido.read((char*)&cont,sizeof(cont));

			archivo_comprimido.read((char*)buffer,sizeof(unsigned)*tamanio_comprimido);



			//contenedor.setExtremos();
			descomprimido.clear();

			contenedor.setCaracteres(tamanio_serializado*cont);

			contenedor.descomprimir(buffer,descomprimido,tamanio_comprimido);

			while(not descomprimido.empty() ){
				/*escribo los componentes q recupere*/

				serializado.str(descomprimido);



				serializado.pubseekpos(0,ios::out|ios::binary|ios::in);

				componente->deserializar(serializado);



				almacen->escribir(componente);

				descomprimido.erase(0,tamanio_serializado);

			}
		}

	archivo_comprimido.close();

	delete componente;

	delete[] buffer;

	delete[] metadata;

	return true;
}
