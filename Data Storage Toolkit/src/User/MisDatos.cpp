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
	 Archivo* archivo=new Archivo();
	 EABloques * estrategiaBloques=new EABloques();
	 if(!archivo->abrir(path.c_str())){
		 archivo->crear(path.c_str());
		 estrategiaBloques->crear(archivo);
	 }else{
		 estrategiaBloques->abrir(archivo);
		 if(estrategiaBloques->getCapacBloque()!=longitudBloque){
			 //estrategiaBloques->cerrar();
			 archivo->cerrar();
			 throw ExcepcionMisDatos("Error en inicializarArchivo1:Longitud del bloque incorrecta");
		 }
	 }
	 /*incializar indice*/
	 /*armar recurso1*/


 };
 /*
  * Abre el archivo con el path correspondiente y lo deja listo para su uso. Si no existe,
  * lo crea. Si existe previamente, verifica que se correspondan los parametros de tipo y
  * longitud de bloque indice (si es que tiene), que si no correspondieran, lanza una excepcion.
  * En caso de fallar la inicializacion, se lanza una ExcepcionMisDatos con el mensaje de error.
  */
 void MisDatos::inicializarArchivo2(std::string path, bool tieneBuffer, int longitudBuffer,
	bool tieneIndice, TipoIndice tipo, int longitudBloqueIndice) throw (ExcepcionMisDatos){
	 Archivo* archivo=new Archivo();
	 EARegistros * estrategiaRegistros=new EARegistros();
	 if(!archivo->abrir(path.c_str())){
		 archivo->crear(path.c_str());
		 estrategiaRegistros->crear(archivo);
	 }else{
		 estrategiaRegistros->abrir(archivo);
	 }
 };
 /*
  * Abre el archivo con el path correspondiente y lo deja listo para su uso. Si no existe, lo crea.
  * En caso de fallar la inicializacion, se lanza una ExcepcionMisDatos con el mensaje de error.
  */
 void MisDatos::inicializarArchivo3(std::string path) throw (ExcepcionMisDatos){
	 Archivo* archivo=new Archivo();
	 EATexto * estrategiaTexto=new EATexto();
	 if(!archivo->abrir(path.c_str())){
		 archivo->crear(path.c_str());
		 estrategiaTexto->crear(archivo);
	 }else{
		 estrategiaTexto->abrir(archivo);
	 }
 };
 /*
  * Pre: Archivo inicializado.
  * Agrega un registro al archivo1. Si ya existe un registro con ese id, lanza una excepcion
  * con el mensaje de error correspondiente.
  */
 void MisDatos::agregarRegistroArchivo1(MiRegistroVariable registro) throw (ExcepcionMisDatos){
	 AtributoVariable<string> mistringid("miStringID");
	 string str=registro.getMiStringID();
	 mistringid.set(&str);
	 AtributoFijo<int> miInt("miInt");
	 int unint=registro.getMiInt();
	 miInt.set(&unint);
	 AtributoVariable<int> miListaInt("miListaInt");
	 int * pLista=registro.getMiLista();
	 for(int i=0;i<registro.getCantidadElementosLista();i++){
		 miListaInt.append(&pLista[i]);
	 }
	 Registro registroRecurso(3,&mistringid,&miInt,&miListaInt);
	 if(not recurso1->insertar(&registroRecurso))
		 throw ExcepcionMisDatos("No se pudo Insertar el registro en Archivo1");
 };
 /*
  * Pre: Archivo inicializado.
  * Agrega un registro al archivo2. Si ya existe un registro con ese id, lanza una excepcion
  * con el mensaje de error correspondiente.
  */
 void MisDatos::agregarRegistroArchivo2(MiRegistroFijo registro) throw (ExcepcionMisDatos){
	 AtributoFijo<int> miIntID("claveIntId");
	 AtributoFijo<char> miCharID("claveCharId");
	 AtributoFijo<int> miInt("miInt");

	 int miIntid=registro.getMiIntID();
	 miIntID.set(&miIntid);
	 char micharId=registro.getMiCharID();
	 miCharID.set(&micharId);
	 int miint=registro.getMiInt();
	 miInt.set(&miint);

	 Registro registroRecurso(3,&miIntID,&miCharID,&miInt);
	 Clave clave(&registroRecurso,2,"claveIntId","claveCharId");

	 if(not recurso2->insertar(&registroRecurso))
		 throw ExcepcionMisDatos("No se pudo Insertar el registro en Archivo2");
 };
 /*
  * Pre: Archivo inicializado.
  * Agrega un registro al final del archivo3.
  */
 void MisDatos::agregarRegistroArchivo3(MiRegistroTexto registro) throw (ExcepcionMisDatos){
	 AtributoVariable<string> mistring("claveIntId");
	 string str=registro.getTexto();
	 mistring.set(&str);
	 Registro registroRecurso(1,&mistring);
	 if(not recurso2->insertar(&registroRecurso))
		 throw ExcepcionMisDatos("No se pudo Insertar el registro en Archivo3");
 };
 /*
  * Pre: Archivo inicializado.
  * Elimina el registro correspondiente a la clave indicada. Si no existiera el registro con esa
  * clave, lanza una excepcion con el mensaje de error correspondiente.
  */
 void MisDatos::eliminarRegistroArchivo1(std::string clave) throw (ExcepcionMisDatos){
	 AtributoVariable<string> mistringid("miStringID");
	 AtributoFijo<int> miInt("miInt");
	 AtributoVariable<int> miListaInt("miListaInt");
	 Registro registroRecurso(3,&mistringid,&miInt,&miListaInt);
	 Clave claveRecurso(&registroRecurso,1,"miStringID");
	 if(not recurso1->eliminar(&claveRecurso))
		 throw ExcepcionMisDatos("No se pudo Eliminar el registro en Archivo1");

 };
 /*
  * Pre: Archivo inicializado.
  * Elimina el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
  *  lanza una excepcion con el mensaje de error correspondiente.
  */
 void MisDatos::eliminarRegistroArchivo2(int claveInt, char claveChar) throw (ExcepcionMisDatos){
	 AtributoFijo<int> miIntID("claveIntId");
	 AtributoFijo<char> miCharID("claveCharId");
	 AtributoFijo<int> miInt("miInt");

	 miIntID.set(&claveInt);
	 miCharID.set(&claveChar);
	 Registro registro(3,&miIntID,&miCharID,&miInt);
	 Clave clave(&registro,2,"claveIntId","claveCharId");

	 if(not recurso2->eliminar(&clave))
		 throw ExcepcionMisDatos("No se pudo eliminar el registro en Archivo2");
 };
 /*
  * Pre: Archivo inicializado.
  * Modifica el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
  *  lanza una excepcion con el mensaje de error correspondiente.
  */
 void MisDatos::modificarRegistroArchivo1(MiRegistroVariable registro) throw (ExcepcionMisDatos){
	 AtributoVariable<string> mistringid("miStringID");
	 string str=registro.getMiStringID();
	 mistringid.set(&str);
	 AtributoFijo<int> miInt("miInt");
	 int unint=registro.getMiInt();
	 miInt.set(&unint);
	 AtributoVariable<int> miListaInt("miListaInt");
	 int * pLista=registro.getMiLista();
	 for(int i=0;i<registro.getCantidadElementosLista();i++){
	 	miListaInt.append(&pLista[i]);
	 }
	 Registro registroRecurso(3,&mistringid,&miInt,&miListaInt);
	 Clave claveRecurso(&registroRecurso,1,"miStringID");
	 if(not recurso1->modificar(&claveRecurso,&registroRecurso))
	 	throw ExcepcionMisDatos("No se pudo Modificar el registro en Archivo1");
 };
 /*
  * Pre: Archivo inicializado.
  * Modifica el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
  * lanza una excepcion con el mensaje de error correspondiente.
  */
 void MisDatos::modificarRegistroArchivo2(MiRegistroFijo registro) throw (ExcepcionMisDatos){
	 AtributoFijo<int> miIntID("claveIntId");
	 AtributoFijo<char> miCharID("claveCharId");
	 AtributoFijo<int> miInt("miInt");

	 int miIntid=registro.getMiIntID();
	 miIntID.set(&miIntid);
	 char micharId=registro.getMiCharID();
	 miCharID.set(&micharId);
	 int miint=registro.getMiInt();
	 miInt.set(&miint);

	 Registro registroRecurso(3,&miIntID,&miCharID,&miInt);
	 Clave clave(&registroRecurso,2,"claveIntId","claveCharId");

	 if(not recurso2->modificar(&clave,&registroRecurso))
		 throw ExcepcionMisDatos("No se pudo Modificar el registro en Archivo2");
 };
 /*
  * Pre: Archivo inicializado.
  * Retorna el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
  *  lanza una excepcion con el mensaje de error correspondiente.
  */
 MiRegistroVariable MisDatos::obtenerRegistroArchivo1(std::string clave) throw (ExcepcionMisDatos){
	 AtributoVariable<string> mistringid("miStringID");
	 string str=clave;
	 mistringid.set(&str);
	 AtributoFijo<int> miInt("miInt");
	 AtributoVariable<int> miListaInt("miListaInt");
	 Registro registroRecurso(3,&mistringid,&miInt,&miListaInt);
	 Clave claveRecurso(&registroRecurso,1,"miStringID");
	 if(not recurso1->obtener(&claveRecurso,&registroRecurso))
	 	throw ExcepcionMisDatos("No se pudo Modificar el registro en Archivo1");
	 int unint;
	 registroRecurso.get("miInt")->get(&unint);
	 AtributoVariable<int>* miListaIntResultado=(AtributoVariable<int>*)registroRecurso.get("miListaInt");
	 int * pLista=new int[miListaIntResultado->cantidadValores()];
	 for(unsigned int i=0;i < miListaIntResultado->cantidadValores();i++){
		 miListaIntResultado->apuntar(i);
		 miListaIntResultado->get(&pLista[i]);
	 }
	 return MiRegistroVariable(str,unint,pLista,miListaIntResultado->cantidadValores());
 };
 /*
  * Pre: Archivo inicializado.
  * Retorna el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
  * lanza una excepcion con el mensaje de error correspondiente.
  */
 MiRegistroFijo MisDatos::obtenerRegistroArchivo2(int claveInt, char claveChar) throw (ExcepcionMisDatos){
	 AtributoFijo<int> miIntID("claveIntId");
	 AtributoFijo<char> miCharID("claveCharId");
	 AtributoFijo<int> miInt("miInt");
	 Registro registro(3,&miIntID,&miCharID,&miInt);
	 Clave clave(&registro,2,"claveIntId","claveCharId");
	 /*obtengo*/
	 if(not recurso2->obtener(&clave,&registro))
		 throw ExcepcionMisDatos("No pudo obtener el registro en Archivo2");
	 /**/
	 int miIntReg;
	 registro.get("claveIntId")->get(&claveInt);
	 registro.get("claveCharId")->get(&claveChar);
	 registro.get("claveInt")->get(&miIntReg);
	 return MiRegistroFijo(claveInt,claveChar,miIntReg);
 };
 /*
  * Pre: Archivo inicializado.
  * Muestra por salida standard toda la metadata del buffer, de los bloques y sus registros, asi como
  *  los datos contenidos en estos ultimos.
  * Si el archivo no tiene buffer asociado, muestra un mensaje que lo especifique.
  */
 void MisDatos::mostrarContenidoBufferArchivo1(){};
 /*
  * Pre: Archivo inicializado.
  * Muestra por salida standard toda la metadata del buffer y sus registros, asi como los datos
  * contenidos en estos ultimos.
  * Si el archivo no tiene buffer asociado, muestra un mensaje que lo especifique.
  */
 void MisDatos::mostrarContenidoBufferArchivo2(){};
 /*
  * Pre: Archivo inicializado.
  * Muestra por salida standard toda la metadata del indice, de sus bloques y sus registros, asi como
  * los datos contenidos en estos ultimos.
  * Si el archivo no tiene indice asociado, muestra un mensaje que lo especifique.
  */
 void MisDatos::mostrarIndiceArchivo1(){};
 /*
  * Pre: Archivo inicializado.
  * Muestra por salida standard toda la metadata del indice, de sus bloques y sus registros, asi como
  * los datos contenidos en estos ultimos.
  * Si el archivo no tiene indice asociado, muestra un mensaje que lo especifique.
  */
 void MisDatos::mostrarIndiceArchivo2(){};
 /*
  * Pre: Archivo inicializado.
  * Muestra por salida standard toda la metadata de los bloques y sus registros, asi como los datos
  * contenidos en estos ultimos.
  */
 void MisDatos::mostrarDatosArchivo1(){};
 /*
  * Pre: Archivo inicializado.
  * Muestra por salida standard toda la metadata de los registros, asi como los datos contenidos
  *  en los mismos.
  */
 void MisDatos::mostrarDatosArchivo2(){};
 /*
  * Pre: Archivo inicializado.
  * Cierra el archivo correspondiente.
  */
 void MisDatos::cerrarArchivo1(){};
 /*
  * Pre: Archivo inicializado.
  * Cierra el archivo correspondiente.
  */
 void MisDatos::cerrarArchivo2(){};
 /*
  * Pre: Archivo inicializado.
  * Cierra el archivo correspondiente.
  */
 void MisDatos::cerrarArchivo3(){};


