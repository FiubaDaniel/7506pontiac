#include "MisDatos.h"

MisDatos::MisDatos(){};
MisDatos::~MisDatos(){};
/*
 * Abre el archivo con el path correspondiente y lo deja listo para su uso. Si no existe, lo crea.
 * Si existe previamente, verifica que se correspondan los parametros de longitud de bloque,
 * tipo y longitud de bloque indice (si es que tiene), que si no correspondieran, lanza una excepcion.
 * En caso de fallar la inicializacion, se lanza una ExcepcionMisDatos con el mensaje de error.
 */
 void MisDatos::inicializarArchivo1(std::string path, int longitudBloque, bool tieneBuffer,
		 int longitudBuffer, bool tieneIndice, TipoIndice tipo, int longitudBloqueIndice) throw (ExcepcionMisDatos)
	{

	 Archivo* archivo= new Archivo();
	 if(!archivo->abrir(path.c_str())){
		 if(!archivo->crear(path.c_str()))
			 throw miexcepcion;
	 }else{
		 EABloques * estrategiaBloques;

	 }

 };
 /*
  * Abre el archivo con el path correspondiente y lo deja listo para su uso. Si no existe,
  * lo crea. Si existe previamente, verifica que se correspondan los parametros de tipo y
  * longitud de bloque indice (si es que tiene), que si no correspondieran, lanza una excepcion.
  * En caso de fallar la inicializacion, se lanza una ExcepcionMisDatos con el mensaje de error.
  */
 void MisDatos::inicializarArchivo2(std::string path, bool tieneBuffer, int longitudBuffer,
		 bool tieneIndice, TipoIndice tipo, int longitudBloqueIndice) throw (ExcepcionMisDatos){

 };
 /*
  * Abre el archivo con el path correspondiente y lo deja listo para su uso. Si no existe, lo crea.
  * En caso de fallar la inicializacion, se lanza una ExcepcionMisDatos con el mensaje de error.
  */
 void MisDatos::inicializarArchivo3(std::string path) throw (ExcepcionMisDatos){

 };
