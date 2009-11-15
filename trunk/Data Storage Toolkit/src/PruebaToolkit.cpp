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

using namespace std;

//para mostrar los datos, buffer e indice de Archivo1
void mostrarArchivo1(MisDatos& misDatos){
	cout << "********* DATOS **********" << endl;
	misDatos.mostrarDatosArchivo1();
	cout << "********* BUFFER **********" << endl;
	misDatos.mostrarContenidoBufferArchivo1();
	cout << "********* INDICE **********" << endl;
	misDatos.mostrarIndiceArchivo1();
}

//para mostrar los datos, buffer e indice de Archivo2
void mostrarArchivo2(MisDatos& misDatos){
	cout << "********* DATOS **********" << endl;
	misDatos.mostrarDatosArchivo2();
	cout << "********* BUFFER **********" << endl;
	misDatos.mostrarContenidoBufferArchivo2();
	cout << "********* INDICE **********" << endl;
	misDatos.mostrarIndiceArchivo2();
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

	/* ARCHIVO 1 */
	try{
		misDatos.inicializarArchivo1("MiArchivo1", 256, true, 1024, true, ARBOL, 128);
		for (int i=0; i<100; ++i){
			cantElementos = i % DELTA + MINIMO;
			nroTermino = i;

			miLista = new int[cantElementos];
			for (int j=0; j<cantElementos; ++j){
				miLista[j] = j;
			}
			if(i==99)
				cout<<endl;
			miRegVariable = new MiRegistroVariable(Terminos::obtenerTermino(nroTermino), i, miLista, cantElementos);
			misDatos.agregarRegistroArchivo1(*miRegVariable);

			mostrarArchivo1(misDatos);

			delete miRegVariable;
			delete miLista;
		}
		cout<<"Elimina "<<endl;
		cout<<" "<<endl;
		int cant = 0;
		for (int i=0; i<100; ++i){
			cant++;
			misDatos.eliminarRegistroArchivo1(Terminos::obtenerTermino(i));
			mostrarArchivo1(misDatos);
			cout<<"Esto es cant: "<<cant<<endl;

		}
		/*--------------------------------------------------------------------*/
		misDatos.cerrarArchivo1();

		/* ARCHIVO 2 */

		misDatos.inicializarArchivo2("MiArchivo2", true, 128, true, HASH, 64);

		for(int i=0; i<CENTENAMAXREGISTROS*100; ++i){
			claveRegFijo = invertirDecenaUnidad(i);

			miRegFijo = new MiRegistroFijo(claveRegFijo, calcularChar(claveRegFijo), i);
			misDatos.agregarRegistroArchivo2(*miRegFijo);

			mostrarArchivo2(misDatos);

			delete(miRegFijo);
		}

		for (int i=50; i<CENTENAMAXREGISTROS*100; ++i){
			misDatos.eliminarRegistroArchivo2(i, calcularChar(i));
			mostrarArchivo2(misDatos);
		}

		misDatos.cerrarArchivo2();
	}catch(exception& e){
		cout<<e.what()<<endl;
		cout<<"Cerrando Archivo"<<endl;
		misDatos.cerrarArchivo1();
		misDatos.cerrarArchivo2();

	}catch(ExcepcionMisDatos& e){
		cout<<e.getMensaje()<<endl;
		cout<<"Cerrando Archivo"<<endl;
		misDatos.cerrarArchivo1();
		misDatos.cerrarArchivo2();
	}
	return 0;
}
