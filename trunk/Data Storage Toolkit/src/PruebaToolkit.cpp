//============================================================================
// Name        : PruebaToolkit.cpp
// Author      : Lampro
//============================================================================

#include <iostream>
#include "User/MisDatos.h"
#include "Terminos.h"

#define DELTA 4
#define MINIMO 1
#define CENTENAMAXREGISTROS 1
#define MINBORRARVARIABLE 20
#define MINBORRARFIJO 20
#define CANTMODIFICARVARIABLE 5
#define CANTMODIFICARFIJO 10

using namespace std;

//para mostrar los datos, buffer e indice de Archivo1
void mostrarArchivo1(MisDatos& misDatos){
	cout << "********* DATOS **********" << endl;
	misDatos.mostrarDatosArchivo1();
	cout << "********* INDICE **********" << endl;
	misDatos.mostrarIndiceArchivo1();
}

//para mostrar los datos, buffer e indice de Archivo2
void mostrarArchivo2(MisDatos& misDatos){
	cout << "********* DATOS **********" << endl;
	misDatos.mostrarDatosArchivo2();
	cout << "********* INDICE **********" << endl;
	misDatos.mostrarIndiceArchivo2();
}

void mostrarRegistroVariable(MiRegistroVariable* registro){
	cout << "***REGISTRO VARIABLE: " << registro->getMiStringID() << "-" << registro->getMiInt() << "-";
	cout << "{" << registro->getMiLista()[0];
	for(int i=1; i<registro->getCantidadElementosLista(); ++i){
		cout << ";" << registro->getMiLista()[i];
	}
	cout << "}" << " ***" << endl;
}

void mostrarRegistroFijo(MiRegistroFijo* registro){
	cout << "***REGISTRO FIJO: " << registro->getMiIntID() << " " << registro->getMiCharID() <<" " << registro->getMiInt() << " ***" << endl;
}

//para invertir la decena con la unidad
int invertirDecenaUnidad(int valor){
	int centena = 0;
	int decena = 0;
	int unidad = 0;
	
	centena = (valor/100)%10;
	decena = valor%10;
	unidad = (valor/10)%10;
	return centena*100 + decena*10 + unidad;
}

//para devolver un caracter imprimible
char calcularChar(int valor){
	return 35 + (valor%88);
}

int main() {
	int* miLista = NULL;
	MiRegistroVariable* miRegVariable = NULL;
	MiRegistroFijo* miRegFijo = NULL;
	MisDatos misDatos;
	int cantElementos = 0;
	int nroTermino = 0;
	int claveRegFijo = 0;

	// ARCHIVO 1
	// INICIALIZO EL ARCHIVO 1
	misDatos.inicializarArchivo1("MiArchivo1.dat", 256, true, DIFERIDO, 5, true, ARBOL, 128, true, 1024);
	
	// INSERTO REGISTROS VARIABLES
	for (int i=0; i<CENTENAMAXREGISTROS*100; ++i){
		cantElementos = i % DELTA + MINIMO;
		nroTermino = invertirDecenaUnidad(i);
		
		miLista = new int[cantElementos];
		for (int j=0; j<cantElementos; ++j){
			miLista[j] = j;
		}
		miRegVariable = new MiRegistroVariable(Terminos::obtenerTermino(nroTermino), i, miLista, cantElementos);
		cout << endl;
		cout << ">>> INSERTANDO -> ";
		mostrarRegistroVariable(miRegVariable);

		try{
			misDatos.agregarRegistroArchivo1(*miRegVariable);
		}catch(ExcepcionMisDatos e){
			cout << e.getMensaje() << endl;
			cout << "*****ERROR CON CLAVE: " << Terminos::obtenerTermino(nroTermino) << " *****" << endl;
		}
		
		mostrarArchivo1(misDatos);
		
		delete(miRegVariable);
		delete(miLista);
	}
	
	// BORRO REGISTROS VARIABLES
	for (int i=MINBORRARVARIABLE; i<CENTENAMAXREGISTROS*100; ++i){
		try{
			misDatos.eliminarRegistroArchivo1(Terminos::obtenerTermino(i));
		}catch(ExcepcionMisDatos e){
			cout << e.getMensaje() << endl;
			cout << "*****ERROR CON CLAVE: " << Terminos::obtenerTermino(i) << " *****" << endl;
		}

		mostrarArchivo1(misDatos);
	}
	
	// MODIFICO REGISTROS VARIABLES
	cantElementos = 10;
	for (int i=0; i<CANTMODIFICARVARIABLE; ++i){
		miLista = new int[cantElementos];
		for (int j=0; j<cantElementos; ++j){
			miLista[j] = j;
		}
		miRegVariable = new MiRegistroVariable(Terminos::obtenerTermino(i), 99, miLista, cantElementos);

		cout << endl;
		cout << ">>> MODIFICANDO -> ";
		mostrarRegistroVariable(miRegVariable);

		try{
			misDatos.modificarRegistroArchivo1(*miRegVariable);
		}catch(ExcepcionMisDatos e){
			cout << e.getMensaje() << endl;
			cout << "*****ERROR CON CLAVE: " << Terminos::obtenerTermino(i) << " *****" << endl;
		}
		delete miLista;
		delete miRegVariable;

		mostrarArchivo1(misDatos);
	}

	// OBTENGO REGISTROS VARIABLES
	for (int i=0; i<MINBORRARVARIABLE; ++i){
		try{
			miRegVariable = misDatos.obtenerRegistroArchivo1(Terminos::obtenerTermino(i));
			mostrarRegistroVariable(miRegVariable);
			delete miRegVariable;
		}catch(ExcepcionMisDatos e){
			cout << e.getMensaje() << endl;
			cout << "*****ERROR CON CLAVE: " << Terminos::obtenerTermino(i) << " *****" << endl;
		}


	}

	// CIERRO ARCHIVO 1
	misDatos.cerrarArchivo1();
	


	// ARCHIVO 2
	// INICIALIZO EL ARCHIVO 2
	misDatos.inicializarArchivo2("MiArchivo2.dat", true, DIFERIDO, 5, true, HASH, 64,true, 64);
	
	// INSERTO REGISTROS FIJOS
	for(int i=0; i<CENTENAMAXREGISTROS*100; ++i){
		claveRegFijo = invertirDecenaUnidad(i);
		
		miRegFijo = new MiRegistroFijo(claveRegFijo, calcularChar(claveRegFijo), i);

		cout << endl;
		cout << ">>> INSERTANDO -> ";
		mostrarRegistroFijo(miRegFijo);

		try{
			misDatos.agregarRegistroArchivo2(*miRegFijo);
		}catch(ExcepcionMisDatos e){
			cout << e.getMensaje() << endl;
			cout << "*****ERROR CON CLAVE: " << claveRegFijo << " " << calcularChar(claveRegFijo) << " *****" << endl;
		}
		
		mostrarArchivo2(misDatos);
		
		delete(miRegFijo);
	}

	// BORRO REGISTROS FIJOS
	for (int i=MINBORRARFIJO; i<CENTENAMAXREGISTROS*100; ++i){

		try{
			misDatos.eliminarRegistroArchivo2(i, calcularChar(i));
		}catch(ExcepcionMisDatos e){
			cout << e.getMensaje() << endl;
			cout << "*****ERROR CON CLAVE: " << i << " " << calcularChar(i) << " *****" << endl;
		}
		mostrarArchivo2(misDatos);
	}

	// MODIFICO REGISTROS FIJOS
	for(int i=0; i<CANTMODIFICARFIJO; ++i){

		miRegFijo = new MiRegistroFijo(i, calcularChar(i), 99);

		cout << endl;
		cout << ">>> MODIFICANDO -> ";
		mostrarRegistroFijo(miRegFijo);

		try{
			misDatos.modificarRegistroArchivo2(*miRegFijo);
		}catch(ExcepcionMisDatos e){
			cout << e.getMensaje() << endl;
			cout << "*****ERROR CON CLAVE: " << i << " " << calcularChar(i) << " *****" << endl;
		}

		mostrarArchivo2(misDatos);

		delete(miRegFijo);
	}

	//OBTENGO REGISTROS FIJOS
	for (int i=0; i<MINBORRARFIJO; ++i){
		try{
			miRegFijo = misDatos.obtenerRegistroArchivo2(i, calcularChar(i));
			mostrarRegistroFijo(miRegFijo);
			delete miRegFijo;
		}catch(ExcepcionMisDatos e){
			cout << e.getMensaje() << endl;
			cout << "*****ERROR CON CLAVE: " << i << " " << calcularChar(i) << " *****" << endl;
		}

	}

	// CIERRO ARCHIVO 2
	misDatos.cerrarArchivo2();

	return 0;
}
