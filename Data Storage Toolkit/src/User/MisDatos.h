/*
 * MisDatos.h
 *
 *  Created on: 11/10/2009
 *      Author: ale
 *
 *      MisDatos es una clase que me permite administrar el almacenamiento de un archivo de bloques (que llamaremos archivo1) con los registros del tipo ((string)id, int,(int)+).
 *      Ademas, me permite administrar un segundo archivo (que llamaremos archivo2) con registros del tipo ((int,char)id, int).
 *      Por ultimo, mantendra un tercer archivo (llamado archivo3) de tipo texto, con registros del tipo (string). Este ultimo archivo no tendra indice ni buffer asociado.
 *      Cualquiera de los otros dos archivos puede tener o no un buffer para una mas rapida lectura de registros/bloques.
 *      Tambien es posible que cualquiera de ellos tenga o no un indice (que podria ser un archivo organizado como arbol b# o hash extensible).
 *
 *
 */

#ifndef MISDATOS_H_
#define MISDATOS_H_

#include <iostream>
#include "MiRegistroFijo.h"
#include "MiRegistroVariable.h"
#include "MiRegistroTexto.h"
#include "ExcepcionMisDatos.h"

#include "../RecursoAlmacenamiento/Recurso.h"
#include "../RecursoAlmacenamiento/EREscrituraDirecta.h"
#include "../RecursoAlmacenamiento/ERUnAlmacenamiento.h"
#include "../Almacenamiento/Archivo.h"
#include "../Almacenamiento/EABloques.h"
#include "../Almacenamiento/EARegistros.h"
#include "../Almacenamiento/EATexto.h"
#include "../Almacenamiento/Almacenamiento.h"
#include "../Almacenamiento/Archivo.h"
#include "../Almacenamiento/Buffer.h"
#include "../Indices/EstrategiaBSharp.h"
#include "../Indices/EstrategiaIndice.h"
#include "../Indices/HashingExt.hpp"

enum TipoIndice {ARBOL, HASH};

class MisDatos {
	Recurso *recurso1;
	Recurso *recurso2;
	Recurso *recurso3;
	Registro* registro1;
	Registro* registro2;
	Registro* registro3;
public:
	MisDatos();
	virtual ~MisDatos();
	/*
	 * Abre el archivo con el path correspondiente y lo deja listo para su uso. Si no existe, lo crea.
	 * Si existe previamente, verifica que se correspondan los parametros de longitud de bloque, tipo y
	 * longitud de bloque indice (si es que tiene), que si no correspondieran, lanza una excepcion.
	 * En caso de fallar la inicializacion, se lanza una ExcepcionMisDatos con el mensaje de error.
	 */
	 void inicializarArchivo1(std::string path, int longitudBloque, bool tieneBuffer, int longitudBuffer,
			 bool tieneIndice, TipoIndice tipo, int longitudBloqueIndice) throw (ExcepcionMisDatos);
	 /*
	  * Abre el archivo con el path correspondiente y lo deja listo para su uso. Si no existe, lo crea.
	  * Si existe previamente, verifica que se correspondan los parametros de tipo y longitud de bloque
	  * indice (si es que tiene), que si no correspondieran, lanza una excepcion.
	  * En caso de fallar la inicializacion, se lanza una ExcepcionMisDatos con el mensaje de error.
	  */
	 void inicializarArchivo2(std::string path, bool tieneBuffer, int longitudBuffer, bool tieneIndice,
			 TipoIndice tipo, int longitudBloqueIndice) throw (ExcepcionMisDatos);
	 /*
	  * Abre el archivo con el path correspondiente y lo deja listo para su uso. Si no existe, lo crea.
	  * En caso de fallar la inicializacion, se lanza una ExcepcionMisDatos con el mensaje de error.
	  */
	 void inicializarArchivo3(std::string path) throw (ExcepcionMisDatos);
	 /*
	  * Pre: Archivo inicializado.
	  * Agrega un registro al archivo1. Si ya existe un registro con ese id, lanza una excepcion
	  * con el mensaje de error correspondiente.
	  */
	 void agregarRegistroArchivo1(MiRegistroVariable registro) throw (ExcepcionMisDatos);
	 /*
	  * Pre: Archivo inicializado.
	  * Agrega un registro al archivo2. Si ya existe un registro con ese id, lanza una excepcion
	  * con el mensaje de error correspondiente.
	  */
	 void agregarRegistroArchivo2(MiRegistroFijo registro) throw (ExcepcionMisDatos);
	 /*
	  * Pre: Archivo inicializado.
	  * Agrega un registro al final del archivo3.
	  */
	 void agregarRegistroArchivo3(MiRegistroTexto registro) throw (ExcepcionMisDatos);
	 /*
	  * Pre: Archivo inicializado.
	  * Elimina el registro correspondiente a la clave indicada. Si no existiera el registro con esa
	  * clave, lanza una excepcion con el mensaje de error correspondiente.
	  */
	 void eliminarRegistroArchivo1(std::string clave) throw (ExcepcionMisDatos);
	 /*
	  * Pre: Archivo inicializado.
	  * Elimina el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
	  *  lanza una excepcion con el mensaje de error correspondiente.
	  */
	 void eliminarRegistroArchivo2(int claveInt, char claveChar) throw (ExcepcionMisDatos);
	 /*
	  * Pre: Archivo inicializado.
	  * Modifica el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
	  *  lanza una excepcion con el mensaje de error correspondiente.
	  */
	 void modificarRegistroArchivo1(MiRegistroVariable registro) throw (ExcepcionMisDatos);
	 /*
	  * Pre: Archivo inicializado.
	  * Modifica el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
	  * lanza una excepcion con el mensaje de error correspondiente.
	  */
	 void modificarRegistroArchivo2(MiRegistroFijo registro) throw (ExcepcionMisDatos);
	 /*
	  * Pre: Archivo inicializado.
	  * Retorna el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
	  *  lanza una excepcion con el mensaje de error correspondiente.
	  */
	 MiRegistroVariable *obtenerRegistroArchivo1(std::string clave) throw (ExcepcionMisDatos);
	 /*
	  * Pre: Archivo inicializado.
	  * Retorna el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
	  * lanza una excepcion con el mensaje de error correspondiente.
	  */
	 MiRegistroFijo *obtenerRegistroArchivo2(int claveInt, char claveChar) throw (ExcepcionMisDatos);
	 /*
	  * Pre: Archivo inicializado.
	  * Muestra por salida standard toda la metadata del buffer, de los bloques y sus registros, asi como
	  *  los datos contenidos en estos ultimos.
	  * Si el archivo no tiene buffer asociado, muestra un mensaje que lo especifique.
	  */
	 void mostrarContenidoBufferArchivo1();
	 /*
	  * Pre: Archivo inicializado.
	  * Muestra por salida standard toda la metadata del buffer y sus registros, asi como los datos
	  * contenidos en estos ultimos.
	  * Si el archivo no tiene buffer asociado, muestra un mensaje que lo especifique.
	  */
	 void mostrarContenidoBufferArchivo2();
	 /*
	  * Pre: Archivo inicializado.
	  * Muestra por salida standard toda la metadata del indice, de sus bloques y sus registros, asi como
	  * los datos contenidos en estos ultimos.
	  * Si el archivo no tiene indice asociado, muestra un mensaje que lo especifique.
	  */
	 void mostrarIndiceArchivo1();
	 /*
	  * Pre: Archivo inicializado.
	  * Muestra por salida standard toda la metadata del indice, de sus bloques y sus registros, asi como
	  * los datos contenidos en estos ultimos.
	  * Si el archivo no tiene indice asociado, muestra un mensaje que lo especifique.
	  */
	 void mostrarIndiceArchivo2();
	 /*
	  * Pre: Archivo inicializado.
	  * Muestra por salida standard toda la metadata de los bloques y sus registros, asi como los datos
	  * contenidos en estos ultimos.
	  */
	 void mostrarDatosArchivo1();
	 /*
	  * Pre: Archivo inicializado.
	  * Muestra por salida standard toda la metadata de los registros, asi como los datos contenidos
	  *  en los mismos.
	  */
	 void mostrarDatosArchivo2();
	 /*
	  * Pre: Archivo inicializado.
	  * Cierra el archivo correspondiente.
	  */
	 void cerrarArchivo1();
	 /*
	  * Pre: Archivo inicializado.
	  * Cierra el archivo correspondiente.
	  */
	 void cerrarArchivo2();
	 /*
	  * Pre: Archivo inicializado.
	  * Cierra el archivo correspondiente.
	  */
	 void cerrarArchivo3();

};

#endif /* MISDATOS_H_ */
