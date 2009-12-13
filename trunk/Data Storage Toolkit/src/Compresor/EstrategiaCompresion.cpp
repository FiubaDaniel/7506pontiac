/*
 * EstrategiaCompresion.cpp
 *
 *  Created on: 03/12/2009
 *      Author: daniel
 */

#include "EstrategiaCompresion.h"

EstrategiaCompresion::EstrategiaCompresion() {
	salida=NULL;

}

EstrategiaCompresion::~EstrategiaCompresion() {

}

bool EstrategiaCompresion::compresionArbol(BSharpTree* arbol,string archivo,unsigned tamanio_buffer_comprimido){
	std::fstream archivo_comprimido;

	//Creo el contenedor adecuado a las condiciones dadas.
	unsigned *buffer=new unsigned[tamanio_buffer_comprimido];
	Compresor contenedor(buffer,tamanio_buffer_comprimido);
	contenedor.setSalida(salida);
	unsigned tamanioSerializado = arbol->tamanioBloque();
	std::stringbuf serializado;

	string archivoComprimido = archivo +"_arbolComprimido";
	archivo_comprimido.open(archivoComprimido.c_str(),fstream::trunc|fstream::out|fstream::binary);
	if(!archivo_comprimido.is_open()){
		return false;
	}
	archivo_comprimido.seekp(0);
	archivo_comprimido.write((char*)&tamanio_buffer_comprimido,sizeof(tamanio_buffer_comprimido));

	Nodo* nodo;

	//Paso la metadata (que no se comprime) al archivo de datos comprimido
	std::string metadata=arbol->getMetadata();

	unsigned tamanio_meta=metadata.size();

	archivo_comprimido.write((char*)&tamanio_meta,sizeof(tamanio_meta));
	archivo_comprimido.write(metadata.data(),tamanio_meta);
	if(salida!=NULL){
		(*salida)<<"Comprimiendo arbol:"<<archivo<<endl;
	}
	//Comienzo la compresion
	//Comprimo el primero
	arbol->posicionarArchivo();
	arbol->siguienteAlmacenado(nodo);

	nodo->serializate(&serializado);

	contenedor.comprimirPrimeros((unsigned char*)serializado.str().data(),tamanioSerializado);

	delete nodo;

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
			contenedor.reiniciarBuffer();
			contenedor.comprimirPrimeros((unsigned char*)serializado.str().data(),tamanioSerializado);
			cont=1;

		}else cont++;
		delete nodo;
	}

	contenedor.cerrar();
	archivo_comprimido.write((char*)&cont,sizeof(cont));
	archivo_comprimido.write((char*)buffer,sizeof(unsigned)*tamanio_buffer_comprimido);
	archivo_comprimido.close();

	if(salida!=NULL){
		(*salida)<< dec<<"Termino de comprimir arbol"<<endl;
	}

	delete[] buffer;

	return true;
}

bool EstrategiaCompresion::comprimir(string nombreIndice,unsigned tamanio_buffer_comprimido){
	std::fstream archivo_indice;
	std::fstream archivo_comprimido;

	unsigned *comprimido=new unsigned[tamanio_buffer_comprimido];

	Compresor contenedor(comprimido,tamanio_buffer_comprimido);
	contenedor.setSalida(salida);
	/*Todo poner
	string nombreHash = nombreIndice+"_cubos";//TODO poner +_cubos
	string nombreComprimido = nombreIndice+"_HashComprimido";*/

	string nombreComprimido = nombreIndice+"Comprimido";//TODO sacar
	//archivo_indice.open(nombreHash.c_str(),fstream::in|fstream::binary);Todo poner

	archivo_indice.open(nombreIndice.c_str(),fstream::in|fstream::binary);//Todo sacar

	if(!archivo_indice.is_open()){
		archivo_indice.close();
		return false;
	}

	archivo_comprimido.open(nombreComprimido.c_str(),fstream::trunc|fstream::out|fstream::binary);
	archivo_indice.seekg(0);
	archivo_comprimido.seekp(0);
	archivo_comprimido.write((char*)&tamanio_buffer_comprimido,sizeof(tamanio_buffer_comprimido));
	//Comienza compresion.
	//Gurado primero.
	unsigned tamanio_sin_comprimir=tamanio_buffer_comprimido;
	if(salida!=NULL){
		(*salida)<<"Comprimiendo:"<<nombreIndice<<endl;
		(*salida)<<"Tamanio de los componentes:"<<tamanio_sin_comprimir<<endl;
	}
	unsigned char sin_comprimir[tamanio_sin_comprimir];
	archivo_indice.read((char*)sin_comprimir,tamanio_sin_comprimir);
	contenedor.comprimirPrimeros(sin_comprimir,tamanio_sin_comprimir);
	//Comprimo el resto
	short cont = 1;
	while(not archivo_indice.eof()){

		archivo_indice.read((char*)sin_comprimir,tamanio_sin_comprimir);
		if(not contenedor.agregar(sin_comprimir,tamanio_sin_comprimir)){
			contenedor.cerrar();
			/*IMPRESION ESTADISICA*/
			if(salida!=NULL){
				(*salida)<<"Cantidad componentes: "<<cont<<endl;
			}
			contenedor.imprimirEstadistica();
			/*FIN*/
			archivo_comprimido.write((char*)&cont,sizeof(cont));
			archivo_comprimido.write((char*)comprimido,sizeof(unsigned)*tamanio_buffer_comprimido);
			contenedor.reiniciarBuffer();
			contenedor.comprimirPrimeros(sin_comprimir,tamanio_sin_comprimir);
			cont=1;

		}else cont++;
	}

	contenedor.cerrar();
	/*IMPRESION ESTADISICA*/
	if(salida!=NULL){
		(*salida)<<"Cantidad componentes: "<<cont<<endl;
	}
	contenedor.imprimirEstadistica();
	/*FIN*/
	archivo_comprimido.write((char*)&cont,sizeof(cont));

	archivo_comprimido.write((char*)comprimido,sizeof(unsigned)*tamanio_buffer_comprimido);

	delete[] comprimido;

	archivo_comprimido.clear();
	archivo_indice.clear();
	archivo_comprimido.close();
	archivo_indice.close();
	return true;
}

bool EstrategiaCompresion::descomprimir(string nombreIndice){

	std::string descomprimido;
	std::fstream archivo_indice;
	std::fstream archivo_comprimido;
	/*TODO esto anterior
	string nombreHash = nombreIndice+"_cubos";
	string nombreComprimido = nombreIndice+"_HashComprimido";
	 */
	string nombreComprimido = nombreIndice+"Comprimido";
	//Todo esta archivo_indice.open(nombreHash.c_str(),fstream::trunc|fstream::out|fstream::binary);


	archivo_comprimido.open(nombreComprimido.c_str(),fstream::in|fstream::binary);

	if(!archivo_comprimido.is_open()){
		archivo_comprimido.close();
		return false;
	}
	archivo_indice.open(nombreIndice.c_str(),fstream::trunc|fstream::out|fstream::binary);
	archivo_indice.seekp(0);
	archivo_comprimido.seekg(0);

	//Obtenego el tamaño de los contenedores
	unsigned tamanio_buffer_comprimido;
	//cout<<archivo_comprimido.tellg()<<endl;
	archivo_comprimido.read((char*)&tamanio_buffer_comprimido,sizeof(tamanio_buffer_comprimido));

	//Creo el contenedor
	unsigned *buffer=new unsigned[tamanio_buffer_comprimido];
	Compresor contenedor(buffer,tamanio_buffer_comprimido);

	unsigned tamanio_sin_comprimir=tamanio_buffer_comprimido;

	//Comienzo a descomprimir

	while(archivo_comprimido.peek()!= EOF and not archivo_comprimido.eof()){

		short cont;

		archivo_comprimido.read((char*)&cont,sizeof(cont));

		archivo_comprimido.read((char*)buffer,sizeof(unsigned)*tamanio_buffer_comprimido);

		contenedor.setCaracteres(tamanio_sin_comprimir*cont);

		contenedor.descomprimir(buffer,descomprimido,tamanio_buffer_comprimido);

		archivo_indice.write(descomprimido.data(),descomprimido.size());

		descomprimido.clear();
	}


	delete[] buffer;
	archivo_comprimido.clear();// saca el flag de eof
	archivo_indice.clear();

	archivo_comprimido.close();
	archivo_indice.close();

	return true;
}

/*
 * Arbol creado, con archivo de espacios libres seteado, archivo del arbol abierto y comparador seteado.
 */
bool EstrategiaCompresion::descompresionArbol(BSharpTree*arbol,string archivo){

	std::fstream archivo_comprimido;
	std::stringbuf serializado;
	string archivoComprimido = archivo +"_arbolComprimido";

	archivo_comprimido.open(archivoComprimido.c_str(),fstream::in|fstream::binary);

	if(!archivo_comprimido.is_open()){
		return false;
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

	delete metadata;
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
			if(not archivo_comprimido.eof()){
				descomprimido.clear();
				contenedor.setCaracteres(tamanio_serializado*cont);
				contenedor.descomprimir(buffer,descomprimido,tamanio_comprimido);
				while( not descomprimido.empty()){
					/*escribo los componentes q recupere*/
					serializado.str(descomprimido);//inicializo el buff con el string descomprimido
					serializado.pubseekpos(0,ios::out|ios::binary|ios::in);
					arbol->escribir(serializado);
					descomprimido.erase(0,tamanio_serializado);//elimino el primer registro/bloque del string
				}
				descomprimido.clear();
			}
		}

	}catch(...){
		throw IOSerializacionExcepcion("Arbol");
	}
	archivo_comprimido.close();

	delete buffer;

	arbol->recomponerRaiz();

	return true;
}


void EstrategiaCompresion::compresion(Almacenamiento*almacen,unsigned tamanio_buffer_comprimido){

	string archivoComprimido=almacen->getNombre();

	archivoComprimido+="_comprimido";
	/**/
	std::fstream archivo_comprimido;

	archivo_comprimido.open(archivoComprimido.c_str(),fstream::trunc|fstream::out|fstream::binary);

	unsigned *buffer=new unsigned[tamanio_buffer_comprimido];

	unsigned tamanioSerializado=almacen->getEstrategia()->getTamanioComponenteUsado();

	std::stringbuf serializado;
	/**/
	Componente* componente=almacen->getEstrategia()->getComponenteUsado()->clonar();

	Compresor contenedor(buffer,tamanio_buffer_comprimido);

	contenedor.setSalida(salida);

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

	if(salida!=NULL){
		(*salida)<< dec<<"Termino compresion almacenamiento"<<endl;
	}

	archivo_comprimido.write((char*)&cont,sizeof(cont));

	archivo_comprimido.write((char*)buffer,sizeof(unsigned)*tamanio_buffer_comprimido);

	archivo_comprimido.close();

	delete componente;

	delete[] buffer;

}
bool EstrategiaCompresion::descompresion(Almacenamiento*almacen){

	string archivoComprimido=almacen->getNombre();

	archivoComprimido+="_comprimido";
	std::fstream archivo_comprimido;
	std::stringbuf serializado;
	archivo_comprimido.open(archivoComprimido.c_str(),fstream::in|fstream::binary);
	if(not archivo_comprimido.is_open())
		return false;
	Componente* componente=almacen->getEstrategia()->getComponenteUsado()->clonar();
	unsigned tamanio_comprimido;
	/*recuperao tamanio del buffer */
	archivo_comprimido.read((char*)&tamanio_comprimido,sizeof(tamanio_comprimido));
	unsigned *buffer=new unsigned[tamanio_comprimido];

	Compresor contenedor(buffer,tamanio_comprimido);
	contenedor.setSalida(salida);
	/*recupero la metadata del almacenamiento*/

	unsigned tamanio_meta;
	archivo_comprimido.read((char*)&tamanio_meta,sizeof(tamanio_meta));
	char* metadata=new char[tamanio_meta];
	archivo_comprimido.read(metadata,tamanio_meta);
	almacen->getEstrategia()->setMetadata(metadata);

	/*Recupero los componentes*/

	almacen->posicionarComponente(0);

	std::string descomprimido;
	unsigned tamanio_serializado=almacen->getEstrategia()->getTamanioComponenteUsado();

	descomprimido.clear();
	short cont=0;

	try{

		while(archivo_comprimido.peek()!= EOF and not archivo_comprimido.eof()){
			/*recupero una tira de componentes serializados*/

			archivo_comprimido.read((char*)&cont,sizeof(cont));

			archivo_comprimido.read((char*)buffer,sizeof(unsigned)*tamanio_comprimido);

			cout<<tamanio_comprimido<<endl;

			//contenedor.setExtremos();
			descomprimido.clear();

			contenedor.setCaracteres(tamanio_serializado*cont);

			contenedor.descomprimir(buffer,descomprimido,tamanio_comprimido);

			cout<<cont<<endl;

			cout<<descomprimido.size()<<endl;

			while(not descomprimido.empty() ){
				/*escribo los componentes q recupere*/

				serializado.str(descomprimido);



				serializado.pubseekpos(0,ios::out|ios::binary|ios::in);

				componente->deserializar(serializado);



				almacen->escribir(componente);

				descomprimido.erase(0,tamanio_serializado);


				while(not archivo_comprimido.eof()){
					/*recupero una tira de componentes serializados*/
					archivo_comprimido.read((char*)&cont,sizeof(cont));
					if(not archivo_comprimido.eof()){
						archivo_comprimido.read((char*)buffer,sizeof(unsigned)*tamanio_comprimido);
						descomprimido.clear();
						contenedor.setCaracteres(tamanio_serializado*cont);
						contenedor.descomprimir(buffer,descomprimido,tamanio_comprimido);
						cout<<cont<<endl;
						while(not descomprimido.empty() ){
							/*escribo los componentes q recupere*/
							serializado.str(descomprimido);
							serializado.pubseekpos(0,ios::out|ios::binary|ios::in);
							componente->deserializar(serializado);
							almacen->escribir(componente);
							descomprimido.erase(0,tamanio_serializado);
						}
					}

				}
			}
		}
	}catch(IOSerializacionExcepcion e){
		cout<<e.what()<<endl;
		almacen->cerrar();
		archivo_comprimido.close();
		throw IOSerializacionExcepcion("Almacenamiento");
	}
	archivo_comprimido.close();

	delete componente;

	delete[] buffer;

	delete[] metadata;

	return true;
}
void EstrategiaCompresion::setSalida(std::ostream*salida){
	this->salida=salida;
}

