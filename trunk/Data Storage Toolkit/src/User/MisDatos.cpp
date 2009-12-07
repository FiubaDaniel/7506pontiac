#include "MisDatos.h"
#include "../Indices/Set.h"
#include "../Compuesto/BloqueMemoria.h"
#include "../Compuesto/RegistroMemoria.h"
#include "../Compresor/EstrategiaCompresion.h"

MisDatos::MisDatos(){
	recurso1=NULL;
	recurso2=NULL;
	recurso3=NULL;
	registro1=NULL;
	registro2=NULL;
	registro3=NULL;
}
MisDatos::~MisDatos(){

}
/*
 * Abre el archivo con el path correspondiente y lo deja listo para su uso. Si no existe, lo crea.
 * Si existe previamente, verifica que se correspondan los parametros de longitud de bloque,
 * tipo y longitud de bloque indice (si es que tiene), que si no correspondieran, lanza una excepcion.
 * En caso de fallar la inicializacion, se lanza una ExcepcionMisDatos con el mensaje de error.
 */
void MisDatos::inicializarArchivo1(std::string path, int longitudBloque, bool tieneBuffer, TipoBuffer tipoBuffer, int longitudBuffer, bool tieneIndice, TipoIndice tipo, int longitudBloqueIndice, bool comprime, int longitudContenedor) throw (ExcepcionMisDatos){

	/*inicializo el archivo y la estrategia*/
	AtributoFijo<char*> mistringid("miStringID",16);
	AtributoFijo<int> miInt("miInt");
	AtributoVariable<int> miListaInt("miListaInt");
	registro1=new Registro(3,&mistringid,&miInt,&miListaInt);
	Clave claveEstructural(registro1,1,"miStringID");
	ComparadorClave* comparador = new ComparadorRegistroVariable();
	EstrategiaCompresion compresor;
	/*Preparo la estregia*/
	EABloques * estrategiaBloques=new EABloques(registro1,longitudBloque,0.8);
	estrategiaBloques->setComparador(comparador);
	estrategiaBloques->setClave(&claveEstructural);
	/*inicializo el archivo*/
	Archivo* archivo=new Archivo(estrategiaBloques);
	hay_que_comprimir=comprime;
	tamanio_contendor=longitudContenedor/sizeof(unsigned);

	bool existia=true;
	if(comprime){
		archivo->crear(path.c_str());
		existia=compresor.descompresion(archivo);

	}else{
		if( not archivo->abrir(path.c_str())){
			archivo->crear(path.c_str());
			existia=false;
		}
	}

	if(existia){
		long longitud=longitudBloque;
		if(estrategiaBloques->getCapacBloque()!=longitud){
			archivo->cerrar();
			delete registro1;
			delete comparador;
			delete archivo;
			delete estrategiaBloques;
			throw ExcepcionMisDatos("Error en inicializarArchivo1:Longitud del bloque incorrecta");
		}
	}
	/*incializar indice*/
	EstrategiaIndice* Indice=NULL;
	if(tieneIndice){
		existia=true;
		if(tipo==ARBOL){
			Indice = new EstrategiaBSharp(&claveEstructural);
			if(comprime){
				BSharpTree* arbol;//=dynamic_cast<EstrategiaBSharp*>(Indice)->getBsharpTree();
				arbol->abrir(path,comparador,true);
				existia=compresor.descompresionArbol(arbol,path);
				if(existia)
					arbol->recomponerRaiz();
			}else if(!Indice->abrir(path,comparador)){
				Indice->crear(path,longitudBloqueIndice,&claveEstructural,comparador);
				existia = false;
			}
		}else{
			Indice = new HashingExt();
			if(comprime){
				/* todo Descomprimir hash*/

			}else if(!Indice->abrir(path,comparador)){
				Indice->crear(path,longitudBloqueIndice,&claveEstructural,comparador);
				existia = false;
			}
		}
		/*en caso de que existiera previamente verifica*/
		if(existia){
			if(Indice->tamanioBloque()!=longitudBloqueIndice){
				archivo->cerrar();
				Indice->cerrar();
				delete registro1;
				delete comparador;
				delete archivo;
				delete estrategiaBloques;
				delete Indice;
				throw ExcepcionMisDatos("Error en inicializarArchivo1:Longitud del bloqueIndice incorrecta");
			}
		}
	}
	/*inicializar el estrategia Recurso*/
	EstrategiaRecursos* estrategiaRecurso=NULL;
	if(tieneBuffer){
		BloqueMemoria bloqueMem(registro1);
		EABloques * estrategiaBuffer=new EABloques(&bloqueMem,longitudBloque,0.8);
		Buffer* buffer=new Buffer(estrategiaBuffer,longitudBuffer);
		buffer->crear("buffer");
		long nroElem=longitudBuffer/longitudBloque;
		estrategiaRecurso=new EREscrituraDirecta(Indice,archivo,buffer,nroElem);

	}else{
		estrategiaRecurso=new ERUnAlmacenamiento(Indice,archivo);
	};

	recurso1=new Recurso(estrategiaRecurso);
}
/*
 * Abre el archivo con el path correspondiente y lo deja listo para su uso. Si no existe,
 * lo crea. Si existe previamente, verifica que se correspondan los parametros de tipo y
 * longitud de bloque indice (si es que tiene), que si no correspondieran, lanza una excepcion.
 * En caso de fallar la inicializacion, se lanza una ExcepcionMisDatos con el mensaje de error.
 */
void MisDatos::inicializarArchivo2(std::string path, bool tieneBuffer,  TipoBuffer tipoBuffer, int longitudBuffer, bool tieneIndice, TipoIndice tipo, int longitudBloqueIndice, bool comprime, int longitudContenedor) throw (ExcepcionMisDatos){
	/*creo una clave estructural*/
	ComparadorClave* comparador = new ComparadorRegistroFijo();
	AtributoFijo<int> miIntID("miIntID");
	AtributoFijo<char> miCharID("miCharID");
	AtributoFijo<int> miInt("miInt");
	registro2= new Registro (3,&miIntID,&miCharID,&miInt);
	Clave claveEstructural(registro2,2,"miIntID","miCharID");
	EstrategiaCompresion compresor;

	EARegistros * estrategiaregistro=new EARegistros(registro2);
	estrategiaregistro->setComparador(comparador);
	estrategiaregistro->setClave(&claveEstructural);
	Archivo* archivo=new Archivo(estrategiaregistro);

	hay_que_comprimir=comprime;
	tamanio_contendor=longitudContenedor/sizeof(unsigned);
	if(comprime){
		archivo->crear(path.c_str());
		compresor.descompresion(archivo);
	}else if( not archivo->abrir(path.c_str()))
		archivo->crear(path.c_str());

	/*incializar indice*/
	EstrategiaIndice* Indice=NULL;
	if(tieneIndice){
		bool existia=true;
		if(tipo==ARBOL){
			Indice = new EstrategiaBSharp(&claveEstructural);
			if(comprime){
				BSharpTree* arbol;//=dynamic_cast<EstrategiaBSharp*>(Indice)->getBsharpTree();
				arbol->abrir(path,comparador,true);
				existia=compresor.descompresionArbol(arbol,path);
				if(existia)
					arbol->recomponerRaiz();
			}else if(!Indice->abrir(path,comparador)){
					Indice->crear(path,longitudBloqueIndice,&claveEstructural,comparador);
					existia = false;
			}
		}else{
			Indice = new HashingExt();
			if(comprime){
				/* todo Descomprimir hash*/
			}else if(!Indice->abrir(path,comparador)){
					Indice->crear(path,longitudBloqueIndice,&claveEstructural,comparador);
					existia = false;
			}
		}
		/*en caso de que existiera previamente verifica*/
		if(existia){
			if(Indice->tamanioBloque()!=longitudBloqueIndice){
				archivo->cerrar();
				Indice->cerrar();
				delete registro2;
				delete comparador;
				delete archivo;
				delete estrategiaregistro;
				delete Indice;
				throw ExcepcionMisDatos("Error en inicializarArchivo1:Longitud del bloque incorrecta");
			}
		}
	}
	/*inicializar el estrategia Recurso*/
	EstrategiaRecursos* estrategiaRecurso=NULL;
	if(tieneBuffer){
		RegistroMemoria regmem(registro2);
		EARegistros* estrategiaBuffer=new EARegistros(&regmem);
		Buffer* buffer=new Buffer(estrategiaBuffer,longitudBuffer);
		buffer->crear("buffer");
		long nroElem=longitudBuffer/regmem.tamanioSerializado();
		estrategiaRecurso=new EREscrituraDirecta(Indice,archivo,buffer,nroElem);

	}else{
		estrategiaRecurso=new ERUnAlmacenamiento(Indice,archivo);
	};


	recurso2=new Recurso(estrategiaRecurso);
}
/*
 * Abre el archivo con el path correspondiente y lo deja listo para su uso. Si no existe, lo crea.
 * En caso de fallar la inicializacion, se lanza una ExcepcionMisDatos con el mensaje de error.
 */
void MisDatos::inicializarArchivo3(std::string path) throw (ExcepcionMisDatos){
	AtributoVariable<string> mistring("claveIntId");
	registro3=new Registro(1,&mistring);
	ComparadorClave* comparador = new ComparadorRegistroVariable();
	Clave claveEstructural(registro3,1,"claveIntId");
	EATexto * estrategiaTexto=new EATexto(registro3);
	estrategiaTexto->setComparador(comparador);
	Archivo* archivo=new Archivo(estrategiaTexto);
	if(!archivo->abrir(path.c_str()))
		archivo->crear(path.c_str());
	EstrategiaRecursos *estrategiaRecurso=new ERUnAlmacenamiento(NULL,archivo);
	recurso3=new Recurso(estrategiaRecurso);
}
/*
 * Pre: Archivo inicializado.
 * Agrega un registro al archivo1. Si ya existe un registro con ese id, lanza una excepcion
 * con el mensaje de error correspondiente.
 */
void MisDatos::agregarRegistroArchivo1(MiRegistroVariable& registro) throw (ExcepcionMisDatos){
	AtributoFijo<char*>* mistringid=(AtributoFijo<char*>*)registro1->get(0);
	AtributoFijo<int>* miInt=(AtributoFijo<int>*)registro1->get(1);
	AtributoVariable<int>* miListaInt=(AtributoVariable<int>* )registro1->get(2);
	*mistringid=registro.getMiStringID();
	*miInt=registro.getMiInt();
	miListaInt->getVector().clear();
	int * pLista=registro.getMiLista();
	for(int i=0;i<registro.getCantidadElementosLista();i++){
		miListaInt->getVector().push_back(pLista[i]);
	}
	if(not recurso1->insertar(registro1))
		throw ExcepcionMisDatos("No se pudo Insertar el registro en Archivo1");
}
/*
 * Pre: Archivo inicializado.
 * Agrega un registro al archivo2. Si ya existe un registro con ese id, lanza una excepcion
 * con el mensaje de error correspondiente.
 */
void MisDatos::agregarRegistroArchivo2(MiRegistroFijo& registro) throw (ExcepcionMisDatos){
	AtributoFijo<int>* miIntID=(AtributoFijo<int>*)registro2->get(0);
	AtributoFijo<char>* miCharID=(AtributoFijo<char>*)registro2->get(1);
	AtributoFijo<int>* miInt=(AtributoFijo<int>*)registro2->get(2);
	*miIntID=registro.getMiIntID();
	*miCharID=registro.getMiCharID();
	*miInt=registro.getMiInt();
	Clave clave(registro2,2,"miIntID","miCharID");

	if(not recurso2->insertar(registro2))
		throw ExcepcionMisDatos("No se pudo Insertar el registro en Archivo2");
}
/*
 * Pre: Archivo inicializado.
 * Agrega un registro al final del archivo3.
 */
void MisDatos::agregarRegistroArchivo3(MiRegistroTexto& registro) throw (ExcepcionMisDatos){
	AtributoVariable<string>* mistring=(AtributoVariable<string>*)registro3->get(0);
	*mistring=registro.getTexto();
	if(not recurso2->insertar(registro3))
		throw ExcepcionMisDatos("No se pudo Insertar el registro en Archivo3");
}
/*
 * Pre: Archivo inicializado.
 * Elimina el registro correspondiente a la clave indicada. Si no existiera el registro con esa
 * clave, lanza una excepcion con el mensaje de error correspondiente.
 */
void MisDatos::eliminarRegistroArchivo1(std::string clave) throw (ExcepcionMisDatos){
	AtributoFijo<char*>* mistringid=(AtributoFijo<char*>*)registro1->get(0);
	*mistringid=clave.c_str();
	Clave claveRecurso(registro1,1,"miStringID");
	if(not recurso1->eliminar(&claveRecurso))
		throw ExcepcionMisDatos("No se pudo Eliminar el registro en Archivo1");

}
/*
 * Pre: Archivo inicializado.
 * Elimina el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
 *  lanza una excepcion con el mensaje de error correspondiente.
 */
void MisDatos::eliminarRegistroArchivo2(int claveInt, char claveChar) throw (ExcepcionMisDatos){
	AtributoFijo<int>* miIntID=(AtributoFijo<int>*)registro2->get(0);
	AtributoFijo<char>* miCharID=(AtributoFijo<char>*)registro2->get(1);
	*miIntID=claveInt;
	*miCharID=claveChar;
	Clave clave(registro2,2,"miIntID","miCharID");

	if(not recurso2->eliminar(&clave))
		throw ExcepcionMisDatos("No se pudo eliminar el registro en Archivo2");
}
/*
 * Pre: Archivo inicializado.
 * Modifica el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
 *  lanza una excepcion con el mensaje de error correspondiente.
 */
void MisDatos::modificarRegistroArchivo1(MiRegistroVariable& registro) throw (ExcepcionMisDatos){
	AtributoFijo<char*>* mistringid=(AtributoFijo<char*>*)registro1->get(0);
	AtributoFijo<int>* miInt=(AtributoFijo<int>*)registro1->get(1);
	AtributoVariable<int>* miListaInt=(AtributoVariable<int>* )registro1->get(2);
	*mistringid=registro.getMiStringID();
	*miInt=registro.getMiInt();
	miListaInt->getVector().clear();
	int * pLista=registro.getMiLista();
	for(int i=0;i<registro.getCantidadElementosLista();i++){
		miListaInt->getVector().push_back(pLista[i]);
	}
	Clave claveRecurso(registro1,1,"miStringID");
	if(not recurso1->modificar(&claveRecurso,registro1))
		throw ExcepcionMisDatos("No se pudo Modificar el registro en Archivo1");
}
/*
 * Pre: Archivo inicializado.
 * Modifica el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
 * lanza una excepcion con el mensaje de error correspondiente.
 */
void MisDatos::modificarRegistroArchivo2(MiRegistroFijo& registro) throw (ExcepcionMisDatos){
	AtributoFijo<int>* miIntID=(AtributoFijo<int>*)registro2->get(0);
	AtributoFijo<char>* miCharID=(AtributoFijo<char>*)registro2->get(1);
	AtributoFijo<int>* miInt=(AtributoFijo<int>*)registro2->get(2);
	*miIntID=registro.getMiIntID();
	*miCharID=registro.getMiCharID();
	*miInt=registro.getMiInt();

	Clave clave(registro2,2,"miIntID","miCharID");

	if(not recurso2->modificar(&clave,registro2))
		throw ExcepcionMisDatos("No se pudo Modificar el registro en Archivo2");
}
/*
 * Pre: Archivo inicializado.
 * Retorna el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
 *  lanza una excepcion con el mensaje de error correspondiente.
 */
MiRegistroVariable* MisDatos::obtenerRegistroArchivo1(std::string clave) throw (ExcepcionMisDatos){
	AtributoFijo<char*>* mistringid=(AtributoFijo<char*>*)registro1->get(0);
	*mistringid=clave;

	Clave claveRecurso(registro1,1,"miStringID");

	if(not recurso1->obtener(&claveRecurso,registro1))
		throw ExcepcionMisDatos("No se pudo obtener el registro en Archivo1");
	mistringid=(AtributoFijo<char*>*)registro1->get(0);
	AtributoFijo<int>* miInt=(AtributoFijo<int>*)registro1->get(1);
	AtributoVariable<int>* miListaInt=(AtributoVariable<int>* )registro1->get(2);

	int * pLista=new int[miListaInt->getVector().size()];
	for(unsigned int i=0;i < miListaInt->getVector().size();i++){
		pLista[i]=miListaInt->getVector().at(i);
	}

	//anterior return MiRegistroVariable(*mistringid,*miInt,pLista,miListaInt->getVector().size());
	return new MiRegistroVariable(*mistringid,*miInt,pLista,miListaInt->getVector().size());
}
/*
 * Pre: Archivo inicializado.
 * Retorna el registro correspondiente a la clave indicada. Si no existiera el registro con esa clave,
 * lanza una excepcion con el mensaje de error correspondiente.
 */
MiRegistroFijo* MisDatos::obtenerRegistroArchivo2(int claveInt, char claveChar) throw (ExcepcionMisDatos){
	AtributoFijo<int>* miIntID=(AtributoFijo<int>*)registro2->get(0);
	AtributoFijo<char>* miCharID=(AtributoFijo<char>*)registro2->get(1);
	AtributoFijo<int>* miInt=(AtributoFijo<int>*)registro2->get(2);
	*miIntID=claveInt;
	*miCharID=claveChar;
	Clave clave(registro2,2,"miIntID","miCharID");
	/*obtengo*/
	if(not recurso2->obtener(&clave,registro2))
		throw ExcepcionMisDatos("No pudo obtener el registro en Archivo2");
	/**/
	miIntID=(AtributoFijo<int>*)registro2->get(0);
	miCharID=(AtributoFijo<char>*)registro2->get(1);
	miInt=(AtributoFijo<int>*)registro2->get(2);
	//anterior return MiRegistroFijo(*miIntID,*miCharID,*miInt);
	return new MiRegistroFijo(*miIntID,*miCharID,*miInt);
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
		cout<<"No tiene Buffer";
		return;
	}
	Almacenamiento *buffer=estrategiaDirecta->getBuffer();
	buffer->imprimir(cout);
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
	Almacenamiento *buffer=estrategiaDirecta->getBuffer();
	buffer->imprimir(cout);
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
	Almacenamiento *archivo=recurso1->getEstrategia()->getAlmacenamiento();
	archivo->imprimir(cout);
}
/*
 * Pre: Archivo inicializado.
 * Muestra por salida standard toda la metadata de los registros, asi como los datos contenidos
 *  en los mismos.
 */
void MisDatos::mostrarDatosArchivo2(){
	Almacenamiento *archivo=recurso2->getEstrategia()->getAlmacenamiento();
	archivo->imprimir(cout);
}
/*
 * Pre: Archivo inicializado.
 * Cierra el archivo correspondiente.
 */
void MisDatos::cerrarArchivo1(){
	if(recurso1!=NULL){
		EREscrituraDirecta* estrategiaDirecta = dynamic_cast<EREscrituraDirecta*>(recurso1->getEstrategia());
		if(hay_que_comprimir){
			Almacenamiento* almacen=recurso1->getEstrategia()->getAlmacenamiento();
			string nombre_archivo=almacen->getNombre();
			EstrategiaCompresion compresor;
			compresor.compresion(almacen,tamanio_contendor);

			EstrategiaIndice *indice=recurso1->getEstrategiaRecurso()->getIndice();
			EstrategiaBSharp *indice_arbol=dynamic_cast<EstrategiaBSharp*>(indice);
			if(indice_arbol!=NULL){
				/*todo comprimir arbol*/
				BSharpTree* arbol;//=dynamic_cast<EstrategiaBSharp*>(Indice)->getBsharpTree();
				nombre_archivo=arbol->getNombreArchivo();
				compresor.compresionArbol(arbol,nombre_archivo,tamanio_contendor);
			}
			HashingExt* hash=dynamic_cast<HashingExt*>(indice);
			if(hash){
				/*todo comprimir hash*/
			}
			recurso1->cerrar();
			remove(nombre_archivo.c_str());
		}else
			recurso1->cerrar();

		EABloques* estrategia=NULL;
		if(estrategiaDirecta!=NULL){
			estrategia=(EABloques*)estrategiaDirecta->getBuffer()->getEstrategia();
			delete estrategiaDirecta->getBuffer();
			delete estrategia;
		}
		estrategia=(EABloques*)recurso1->getEstrategia()->getAlmacenamiento()->getEstrategia();
		delete recurso1->getEstrategia()->getAlmacenamiento();
		delete estrategia->getComparador();
		delete estrategia;
		delete recurso1->getEstrategia()->getIndice();
		delete recurso1->getEstrategia();
		delete recurso1;
		recurso1=NULL;
		delete registro1;
		registro1=NULL;
	}
}
/*
 * Pre: Archivo inicializado.
 * Cierra el archivo correspondiente.
 */
void MisDatos::cerrarArchivo2(){
	if(recurso2!=NULL){
		EREscrituraDirecta* estrategiaDirecta = dynamic_cast<EREscrituraDirecta*>(recurso2->getEstrategia());

		if(hay_que_comprimir){
			Almacenamiento* almacen=recurso2->getEstrategia()->getAlmacenamiento();
			string nombre_archivo=almacen->getNombre();
			EstrategiaCompresion compresor;
			compresor.compresion(almacen,tamanio_contendor);

			EstrategiaIndice *indice=recurso2->getEstrategiaRecurso()->getIndice();
			EstrategiaBSharp* arbol=dynamic_cast<EstrategiaBSharp*>(indice);
			if(arbol!=NULL){
				/*todo comprimir arbol*/
				BSharpTree* arbol;//=dynamic_cast<EstrategiaBSharp*>(Indice)->getBsharpTree();
				nombre_archivo=arbol->getNombreArchivo();
				compresor.compresionArbol(arbol,nombre_archivo,tamanio_contendor);
			}
			HashingExt* hash=dynamic_cast<HashingExt*>(indice);
			if(hash){
				/*todo comprimir hash*/
			}
			recurso2->cerrar();
			remove(nombre_archivo.c_str());
		}else
			recurso2->cerrar();

		EABloques* estrategia=NULL;
		if(estrategiaDirecta!=NULL){
			estrategia=(EABloques*)estrategiaDirecta->getBuffer()->getEstrategia();
			delete estrategiaDirecta->getBuffer();
			delete estrategia;
		}
		estrategia=(EABloques*)recurso2->getEstrategia()->getAlmacenamiento()->getEstrategia();
		delete recurso2->getEstrategia()->getAlmacenamiento();
		delete estrategia->getComparador();
		delete estrategia;
		delete recurso2->getEstrategia()->getIndice();
		delete recurso2->getEstrategia();
		delete recurso2;
		recurso2=NULL;
		delete registro2;
		registro2=NULL;
	}
}
/*
 * Pre: Archivo inicializado.
 * Cierra el archivo correspondiente.
 */
void MisDatos::cerrarArchivo3(){
	if(recurso3!=NULL){
		Almacenamiento * almacen=recurso3->getEstrategia()->getAlmacenamiento();

		if(hay_que_comprimir){
			recurso3->cerrar();
			/*string nombre_archivo=recurso3->getEstrategia()->getAlmacenamiento()->getNombre();
			EstrategiaCompresion compresor;
			compresor.compresion(almacen,tamanio_contendor);
			remove(nombre_archivo.c_str());*/
		}else
			recurso3->cerrar();

		delete almacen->getEstrategia()->getComparador();
		delete almacen->getEstrategia();
		delete almacen;
		delete recurso3->getEstrategia();
		delete recurso3;
		recurso3=NULL;
		delete registro3;
		registro3=NULL;
	}
}




