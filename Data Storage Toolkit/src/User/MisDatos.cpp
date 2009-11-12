#include "MisDatos.h"
MisDatos::MisDatos(){}
MisDatos::~MisDatos(){}
/*
 * Abre el archivo con el path correspondiente y lo deja listo para su uso. Si no existe, lo crea.
 * Si existe previamente, verifica que se correspondan los parametros de longitud de bloque,
 * tipo y longitud de bloque indice (si es que tiene), que si no correspondieran, lanza una excepcion.
 * En caso de fallar la inicializacion, se lanza una ExcepcionMisDatos con el mensaje de error.
 */
 void MisDatos::inicializarArchivo1(std::string path, int longitudBloque, bool tieneBuffer,
		 int longitudBuffer, bool tieneIndice, TipoIndice tipo, int longitudBloqueIndice) throw (ExcepcionMisDatos)
{
	 /*inicializo el archivo y la estrategia*/
	 AtributoFijo<char*> mistringid("miStringID",16);
	 mistringid ="abarquillamiento";
	 AtributoFijo<int> miInt("miInt");
	 AtributoVariable<int> miListaInt("miListaInt");
	 Registro reg(3,&mistringid,&miInt,&miListaInt);
	 Clave claveEstructural(&reg,1,"miStringID");
	 ComparadorClave* comparador = new ComparadorRegistroVariable();

	 Archivo* archivo=new Archivo();
	 EABloques * estrategiaBloques=new EABloques(&reg,longitudBloque,0.8);


	 if(!archivo->abrir(path.c_str())){
		 archivo->crear(path.c_str());
		 estrategiaBloques->crear(archivo);
	 }else{
		 estrategiaBloques->abrir(archivo);
		 long longitud=longitudBloque;
		 if(estrategiaBloques->getCapacBloque()!=longitud){
			 estrategiaBloques->cerrar();
			 archivo->cerrar();
			 delete comparador;
			 delete archivo;
			 delete estrategiaBloques;
			 throw ExcepcionMisDatos("Error en inicializarArchivo1:Longitud del bloque incorrecta");
		 }

	 }
	 estrategiaBloques->setComparador(comparador);
	 estrategiaBloques->setClave(&claveEstructural);
	 /*incializar indice*/
	 EstrategiaIndice* Indice=NULL;
	 if(tieneIndice){

		 /*creo una clave estructural*/

		 if(tipo==0){
			 Indice = new EstrategiaBSharp(&claveEstructural);
		 }else{
			 Indice = new HashingExt();
		 }
		 if(!Indice->abrir(path,comparador)){
			 Indice->crear(path,longitudBloqueIndice,&claveEstructural,comparador);
		 }else if(Indice->tamanioBloque()!=longitudBloqueIndice){
			 estrategiaBloques->cerrar();
			 archivo->cerrar();
			 Indice->cerrar();
			 delete comparador;
			 delete archivo;
			 delete estrategiaBloques;
			 delete Indice;
			 throw ExcepcionMisDatos("Error en inicializarArchivo1:Longitud del bloqueIndice incorrecta");
		 }
	 }
	 /*inicializar el estrategia Recurso*/
	 EstrategiaRecursos* estrategiaRecurso=NULL;
	 if(tieneBuffer){
		 EABloques * estrategiaBuffer=new EABloques(&reg,longitudBloque,0.8);//TODO delete
		 Buffer* buffer=new Buffer(longitudBuffer);
		 buffer->setNombre("buffer");
		 estrategiaBuffer->crear(buffer);
		 estrategiaRecurso=new EREscrituraDirecta(Indice,estrategiaBloques,estrategiaBuffer,longitudBuffer/longitudBloque);

	 }else{
		 estrategiaRecurso=new ERUnAlmacenamiento(Indice,estrategiaBloques);
	 };

	 recurso1=new Recurso(estrategiaRecurso);
 }
 /*
  * Abre el archivo con el path correspondiente y lo deja listo para su uso. Si no existe,
  * lo crea. Si existe previamente, verifica que se correspondan los parametros de tipo y
  * longitud de bloque indice (si es que tiene), que si no correspondieran, lanza una excepcion.
  * En caso de fallar la inicializacion, se lanza una ExcepcionMisDatos con el mensaje de error.
  */
 void MisDatos::inicializarArchivo2(std::string path, bool tieneBuffer, int longitudBuffer,
		 bool tieneIndice, TipoIndice tipo, int longitudBloqueIndice) throw (ExcepcionMisDatos)
{
	 /*creo una clave estructural*/
	 ComparadorClave* comparador = new ComparadorRegistroFijo();
	 AtributoFijo<int> miIntID("miIntID");
	 AtributoFijo<char> miCharID("miCharID");
	 AtributoFijo<int> miInt("miInt");
	 Registro reg(3,&miIntID,&miCharID,&miInt);
	 Clave claveEstructural(&reg,2,"miIntID","miCharID");

	 Archivo* archivo=new Archivo();
	 EARegistros * estrategiaregistro=new EARegistros(&reg);

	 archivo->crear(path.c_str());
	 estrategiaregistro->crear(archivo);
	 if(!archivo->abrir(path.c_str())){
		 archivo->crear(path.c_str());
		 estrategiaregistro->crear(archivo);
	 }else{
		 estrategiaregistro->abrir(archivo);
	 }
	 estrategiaregistro->setComparador(comparador);
	 estrategiaregistro->setClave(&claveEstructural);
	 /*incializar indice*/
	 EstrategiaIndice* Indice=NULL;
	 if(tieneIndice){

		 /*creo una clave estructural*/

		 if(tipo==0){
			 Indice = new EstrategiaBSharp(&claveEstructural);
		 }else{
			 Indice = new HashingExt();
		 }

		 if(!Indice->abrir(path,comparador)){
			 Indice->crear(path,longitudBloqueIndice,&claveEstructural,comparador);
		 }else if(Indice->tamanioBloque()!=longitudBloqueIndice){
			 estrategiaregistro->cerrar();
			 archivo->cerrar();
			 Indice->cerrar();
			 delete comparador;
			 delete archivo;
			 delete estrategiaregistro;
			 delete Indice;
			 throw ExcepcionMisDatos("Error en inicializarArchivo1:Longitud del bloque incorrecta");
		 }
	 }
	 /*inicializar el estrategia Recurso*/
	 EstrategiaRecursos* estrategiaRecurso=NULL;
	 if(tieneBuffer){
		 EARegistros* estrategiaBuffer=new EARegistros(&reg);//TODO delete
		 Buffer* buffer=new Buffer(longitudBuffer);
		 buffer->setNombre("buffer");
		 estrategiaBuffer->crear(buffer);
		 estrategiaRecurso=new EREscrituraDirecta(Indice,estrategiaregistro,estrategiaBuffer,longitudBuffer);

	 }else{
		 estrategiaRecurso=new ERUnAlmacenamiento(Indice,estrategiaregistro);
	 };

	 recurso2=new Recurso(estrategiaRecurso);
 }
 /*
  * Abre el archivo con el path correspondiente y lo deja listo para su uso. Si no existe, lo crea.
  * En caso de fallar la inicializacion, se lanza una ExcepcionMisDatos con el mensaje de error.
  */
 void MisDatos::inicializarArchivo3(std::string path) throw (ExcepcionMisDatos){
	 AtributoVariable<string> mistring("claveIntId");
	 Registro registro(1,&mistring);
	 Archivo* archivo=new Archivo();
	 EATexto * estrategiaTexto=new EATexto(&registro);
	 if(!archivo->abrir(path.c_str())){
		 archivo->crear(path.c_str());
		 estrategiaTexto->crear(archivo);
	 }else{
		 estrategiaTexto->abrir(archivo);
	 }
	 EstrategiaRecursos *estrategiaRecurso=new ERUnAlmacenamiento(NULL,estrategiaTexto);
	 recurso3=new Recurso(estrategiaRecurso);
 }
 /*
  * Pre: Archivo inicializado.
  * Agrega un registro al archivo1. Si ya existe un registro con ese id, lanza una excepcion
  * con el mensaje de error correspondiente.
  */
 void MisDatos::agregarRegistroArchivo1(MiRegistroVariable registro) throw (ExcepcionMisDatos){
	 AtributoFijo<char*> mistringid("miStringID",16);
	 mistringid=registro.getMiStringID();
	 AtributoFijo<int> miInt("miInt");
	 miInt=registro.getMiInt();
	 AtributoVariable<int> miListaInt("miListaInt");
	 int * pLista=registro.getMiLista();
	 for(int i=0;i<registro.getCantidadElementosLista();i++){
		 miListaInt.getVector().push_back(pLista[i]);
	 }
	 Registro registroRecurso(3,&mistringid,&miInt,&miListaInt);
	 if(not recurso1->insertar(&registroRecurso))
		 throw ExcepcionMisDatos("No se pudo Insertar el registro en Archivo1");
 }
 /*
  * Pre: Archivo inicializado.
  * Agrega un registro al archivo2. Si ya existe un registro con ese id, lanza una excepcion
  * con el mensaje de error correspondiente.
  */
 void MisDatos::agregarRegistroArchivo2(MiRegistroFijo registro) throw (ExcepcionMisDatos){
	 AtributoFijo<int> miIntID("miIntID");
	 AtributoFijo<char> miCharID("miCharID");
	 AtributoFijo<int> miInt("miInt");

	 miIntID=registro.getMiIntID();
	 miCharID=registro.getMiCharID();
	 miInt=registro.getMiInt();

	 Registro registroRecurso(3,&miIntID,&miCharID,&miInt);
	 Clave clave(&registroRecurso,2,"miIntID","miCharID");

	 if(not recurso2->insertar(&registroRecurso))
		 throw ExcepcionMisDatos("No se pudo Insertar el registro en Archivo2");
 }
 /*
  * Pre: Archivo inicializado.
  * Agrega un registro al final del archivo3.
  */
 void MisDatos::agregarRegistroArchivo3(MiRegistroTexto registro) throw (ExcepcionMisDatos){
	 AtributoVariable<string> mistring("claveIntId");
	 mistring=registro.getTexto();
	 Registro registroRecurso(1,&mistring);
	 if(not recurso2->insertar(&registroRecurso))
		 throw ExcepcionMisDatos("No se pudo Insertar el registro en Archivo3");
 }
 /*
  * Pre: Archivo inicializado.
  * Elimina el registro correspondiente a la clave indicada. Si no existiera el registro con esa
  * clave, lanza una excepcion con el mensaje de error correspondiente.
  */
 void MisDatos::eliminarRegistroArchivo1(std::string clave) throw (ExcepcionMisDatos){
	 AtributoFijo<char*> mistringid("miStringID",16);
	 mistringid=clave.c_str();
	 AtributoFijo<int> miInt("miInt");
	 AtributoVariable<int> miListaInt("miListaInt");

	 Registro registroRecurso(3,&mistringid,&miInt,&miListaInt);
	 Clave claveRecurso(&registroRecurso,1,"miStringID");
	 if(not recurso1->eliminar(&claveRecurso))
		 throw ExcepcionMisDatos("No se pudo Eliminar el registro en Archivo1");

 }
 /*
  * Pre: Archivo inicializado.
  * Elimina el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
  *  lanza una excepcion con el mensaje de error correspondiente.
  */
 void MisDatos::eliminarRegistroArchivo2(int claveInt, char claveChar) throw (ExcepcionMisDatos){
	 AtributoFijo<int> miIntID("miIntID");
	 AtributoFijo<char> miCharID("miCharID");
	 AtributoFijo<int> miInt("miInt");

	 miIntID=claveInt;
	 miCharID=claveChar;

	 Registro registro(3,&miIntID,&miCharID,&miInt);
	 Clave clave(&registro,2,"miIntID","miCharID");

	 if(not recurso2->eliminar(&clave))
		 throw ExcepcionMisDatos("No se pudo eliminar el registro en Archivo2");
 }
 /*
  * Pre: Archivo inicializado.
  * Modifica el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
  *  lanza una excepcion con el mensaje de error correspondiente.
  */
 void MisDatos::modificarRegistroArchivo1(MiRegistroVariable registro) throw (ExcepcionMisDatos){
	 AtributoFijo<char*> mistringid("miStringID",16);
	 mistringid=registro.getMiStringID();
	 AtributoFijo<int> miInt("miInt");
	 miInt=registro.getMiInt();
	 AtributoVariable<int> miListaInt("miListaInt");
	 int * pLista=registro.getMiLista();
	 for(int i=0;i<registro.getCantidadElementosLista();i++){
	 	miListaInt.getVector().push_back(pLista[i]);
	 }
	 Registro registroRecurso(3,&mistringid,&miInt,&miListaInt);
	 Clave claveRecurso(&registroRecurso,1,"miStringID");
	 if(not recurso1->modificar(&claveRecurso,&registroRecurso))
	 	throw ExcepcionMisDatos("No se pudo Modificar el registro en Archivo1");
 }
 /*
  * Pre: Archivo inicializado.
  * Modifica el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
  * lanza una excepcion con el mensaje de error correspondiente.
  */
 void MisDatos::modificarRegistroArchivo2(MiRegistroFijo registro) throw (ExcepcionMisDatos){
	 AtributoFijo<int> miIntID("miIntID");
	 AtributoFijo<char> miCharID("miCharID");
	 AtributoFijo<int> miInt("miInt");

	 miIntID=registro.getMiIntID();
	 miCharID=registro.getMiCharID();
	 miInt=registro.getMiInt();

	 Registro registroRecurso(3,&miIntID,&miCharID,&miInt);
	 Clave clave(&registroRecurso,2,"miIntID","miCharID");

	 if(not recurso2->modificar(&clave,&registroRecurso))
		 throw ExcepcionMisDatos("No se pudo Modificar el registro en Archivo2");
 }
 /*
  * Pre: Archivo inicializado.
  * Retorna el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
  *  lanza una excepcion con el mensaje de error correspondiente.
  */
 MiRegistroVariable MisDatos::obtenerRegistroArchivo1(std::string clave) throw (ExcepcionMisDatos){
	 AtributoFijo<char*> mistringid("miStringID",16);
	 AtributoFijo<int> miInt("miInt");
	 AtributoVariable<int> miListaInt("miListaInt");
	 mistringid=clave;

	 Registro registroRecurso(3,&mistringid,&miInt,&miListaInt);
	 Clave claveRecurso(&registroRecurso,1,"miStringID");

	 if(not recurso1->obtener(&claveRecurso,&registroRecurso))
	 	throw ExcepcionMisDatos("No se pudo Modificar el registro en Archivo1");

	 miListaInt=*registroRecurso.get("miListaInt");
	 miInt=*registroRecurso.get("miInt");

	 int * pLista=new int[miListaInt.getVector().size()];
	 for(unsigned int i=0;i < miListaInt.getVector().size();i++){
		 pLista[i]=miListaInt.getVector().at(i);
	 }

	 return MiRegistroVariable(mistringid,miInt,pLista,miListaInt.getVector().size());
 }
 /*
  * Pre: Archivo inicializado.
  * Retorna el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
  * lanza una excepcion con el mensaje de error correspondiente.
  */
 MiRegistroFijo MisDatos::obtenerRegistroArchivo2(int claveInt, char claveChar) throw (ExcepcionMisDatos){
	 AtributoFijo<int> miIntID("miIntID");
	 AtributoFijo<char> miCharID("miCharID");
	 AtributoFijo<int> miInt("miInt");
	 Registro registro(3,&miIntID,&miCharID,&miInt);
	 Clave clave(&registro,2,"miIntID","miCharID");
	 /*obtengo*/
	 if(not recurso2->obtener(&clave,&registro))
		 throw ExcepcionMisDatos("No pudo obtener el registro en Archivo2");
	 /**/
	 miIntID=*registro.get("claveIntId");
	 miCharID=*registro.get("claveCharId");
	 miInt=*registro.get("claveInt");
	 return MiRegistroFijo(miIntID,miCharID,miInt);
 }
 /*
  * Pre: Archivo inicializado.
  * Muestra por salida standard toda la metadata del buffer, de los bloques y sus registros, asi como
  *  los datos contenidos en estos ultimos.
  * Si el archivo no tiene buffer asociado, muestra un mensaje que lo especifique.
  */
 void MisDatos::mostrarContenidoBufferArchivo1(){
	 EREscrituraDirecta* estrategiaDirecta = dynamic_cast<EREscrituraDirecta*>(recurso1->getEstrategia());
	 if(estrategiaDirecta==NULL){
		 cout<<"NO tieneBuffer";
		 return;
	 }
	 EstrategiaAlmacenamiento *estrategiaBuffer=estrategiaDirecta->getEstrategiaBuffer();
	 AtributoFijo<char*> mistringid("miStringID",16);
	 AtributoFijo<int> miInt("miInt");
	 AtributoVariable<int> miListaInt("miListaInt");
	 Registro registro(3,&mistringid,&miInt,&miListaInt);

	 estrategiaBuffer->posicionarComponente(0);
	 while( estrategiaBuffer->siguiente(&registro) ){
		 cout<<"Posicion : "<<estrategiaBuffer->posicionComponente()<<" ";
		 for(unsigned i=0;i<registro.cantidadAtributos();i++){
			 cout<<registro.get(i)->getNombre()<<" : ";
			 registro.get(i)->imprimir(cout);
			 cout<<" ";
		 }
		 cout<<endl;
	 }
	 cout<<endl;
 }
 /*
  * Pre: Archivo inicializado.
  * Muestra por salida standard toda la metadata del buffer y sus registros, asi como los datos
  * contenidos en estos ultimos.
  * Si el archivo no tiene buffer asociado, muestra un mensaje que lo especifique.
  */
 void MisDatos::mostrarContenidoBufferArchivo2(){
	 EREscrituraDirecta* estrategiaDirecta = dynamic_cast<EREscrituraDirecta*>(recurso2->getEstrategia());
	 if(estrategiaDirecta==NULL){
		 cout<<"NO tieneBuffer";
		 return;
	 }
	 EstrategiaAlmacenamiento *estrategia=estrategiaDirecta->getEstrategiaBuffer();
	 AtributoFijo<int> miIntID("miIntID");
	 AtributoFijo<char> miCharID("miCharID");
	 AtributoFijo<int> miInt("miInt");
	 Registro registro(3,&miIntID,&miCharID,&miInt);

	 estrategia->posicionarComponente(0);
	 while( estrategia->siguiente(&registro) ){
		 cout<<"Posicion : "<<estrategia->posicionComponente()<<" ";
		 for(unsigned i=0;i<registro.cantidadAtributos();i++){
			 cout<<registro.get(i)->getNombre()<<" : ";
			 registro.get(i)->imprimir(cout);
			 cout<<" ";
		 }
		 cout<<endl;
	 }
	 cout<<endl;
 }
 /*
  * Pre: Archivo inicializado.
  * Muestra por salida standard toda la metadata del indice, de sus bloques y sus registros, asi como
  * los datos contenidos en estos ultimos.
  * Si el archivo no tiene indice asociado, muestra un mensaje que lo especifique.
  */
 void MisDatos::mostrarIndiceArchivo1(){

        EstrategiaIndice *indice=const_cast<EstrategiaIndice *>(recurso1->getEstrategia()->getIndice());
        if(indice!=NULL){
        //tiene indice
                indice->mostrarEstado();

        }else{
                cout<< "El archivo1 no tiene indice asociado" << endl;
        }

}
 /*
  * Pre: Archivo inicializado.
  * Muestra por salida standard toda la metadata del indice, de sus bloques y sus registros, asi como
  * los datos contenidos en estos ultimos.
  * Si el archivo no tiene indice asociado, muestra un mensaje que lo especifique.
  */
 void MisDatos::mostrarIndiceArchivo2(){

        EstrategiaIndice *indice=const_cast<EstrategiaIndice *>(recurso2->getEstrategia()->getIndice());
        if(indice!=NULL){
        //tiene indice
                indice->mostrarEstado();
        }else{
                cout<< "El archivo2 no tiene indice asociado" << endl;
        }

}
 /*
  * Pre: Archivo inicializado.
  * Muestra por salida standard toda la metadata de los bloques y sus registros, asi como los datos
  * contenidos en estos ultimos.
  */
 void MisDatos::mostrarDatosArchivo1(){
	 EstrategiaAlmacenamiento *estrategia=const_cast<EstrategiaAlmacenamiento *>(recurso1->getEstrategia()->getEstrategiaAlmacenamiento());

	 AtributoFijo<char*> mistringid("miStringID",16);
	 AtributoFijo<int> miInt("miInt");
	 AtributoVariable<int> miListaInt("miListaInt");
	 Registro registro(3,&mistringid,&miInt,&miListaInt);

	 estrategia->posicionarComponente(0);
	 while( estrategia->siguiente(&registro) ){
		 cout<<"Pos : "<<estrategia->posicionComponente()<<" ";
		 for(unsigned i=0;i<registro.cantidadAtributos();i++){
			 cout<<registro.get(i)->getNombre()<<" : ";
			 registro.get(i)->imprimir(cout);
			 cout<<" ";
		 }
		 cout<<endl;
	 }
	 cout<<endl;
 }
 /*
  * Pre: Archivo inicializado.
  * Muestra por salida standard toda la metadata de los registros, asi como los datos contenidos
  *  en los mismos.
  */
 void MisDatos::mostrarDatosArchivo2(){
	 EstrategiaAlmacenamiento *estrategia=const_cast<EstrategiaAlmacenamiento*>(recurso2->getEstrategia()->getEstrategiaAlmacenamiento());

	 AtributoFijo<int> miIntID("miIntID");
	 AtributoFijo<char> miCharID("miCharID");
	 AtributoFijo<int> miInt("miInt");
	 Registro registro(3,&miIntID,&miCharID,&miInt);

	 estrategia->posicionarComponente(0);
	 while( estrategia->siguiente(&registro) ){
		 cout<<"Posicion : "<<estrategia->posicionComponente()<<" ";
		 for(unsigned i=0;i<registro.cantidadAtributos();i++){
			 cout<<registro.get(i)->getNombre()<<" : ";
			 registro.get(i)->imprimir(cout);
			 cout<<" ";
		 }
		 cout<<endl;
	 }
	 cout<<endl;
 }
 /*
  * Pre: Archivo inicializado.
  * Cierra el archivo correspondiente.
  */
 void MisDatos::cerrarArchivo1(){
	 EstrategiaRecursos* estrategia = recurso1->getEstrategia();
	 EREscrituraDirecta* estrategiaDirecta = dynamic_cast<EREscrituraDirecta*>(estrategia);
	 if(estrategiaDirecta!=NULL){
 		 delete estrategiaDirecta->getEstrategiaBuffer()->getAlmacenamiento();
 		 delete estrategiaDirecta->getEstrategiaBuffer();
	 }
	 delete estrategia->getEstrategiaAlmacenamiento()->getAlmacenamiento();
	 delete estrategia->getEstrategiaAlmacenamiento()->getComparador();
	 delete estrategia->getEstrategiaAlmacenamiento();
	 delete estrategia->getIndice();
	 delete estrategia;
	 delete recurso1;
 }
 /*
  * Pre: Archivo inicializado.
  * Cierra el archivo correspondiente.
  */
 void MisDatos::cerrarArchivo2(){
	 EstrategiaRecursos* estrategia = recurso2->getEstrategia();
	 EREscrituraDirecta* estrategiaDirecta = dynamic_cast<EREscrituraDirecta*>(estrategia);
	 if(estrategiaDirecta!=NULL){
 		 delete estrategiaDirecta->getEstrategiaBuffer()->getAlmacenamiento();
 		 delete estrategiaDirecta->getEstrategiaBuffer();
	 }
	 delete estrategia->getEstrategiaAlmacenamiento()->getAlmacenamiento();
	 delete estrategia->getEstrategiaAlmacenamiento()->getComparador();
	 delete estrategia->getEstrategiaAlmacenamiento();
	 delete estrategia->getIndice();
	 delete estrategia;
	 delete recurso2;
 }
 /*
  * Pre: Archivo inicializado.
  * Cierra el archivo correspondiente.
  */
 void MisDatos::cerrarArchivo3(){
	 delete recurso3->getEstrategia()->getEstrategiaAlmacenamiento()->getAlmacenamiento();
	 delete recurso3->getEstrategia()->getEstrategiaAlmacenamiento();
     delete recurso3->getEstrategia();
     delete recurso3;
 }



