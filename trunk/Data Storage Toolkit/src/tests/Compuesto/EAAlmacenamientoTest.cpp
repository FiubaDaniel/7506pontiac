/*
 * EAAlmacenamientoTest.cpp
 *
 *  Created on: 03/11/2009
 *      Author: paulo
 */

#include "EAAlmacenamientoTest.h"
void imprimir(Registro*reg){
	reg->get(0)->imprimir(cout);
	cout<<" \t : \t ";
	reg->get(1)->imprimir(cout);
	cout<<endl;
};
EAAlmacenamientoTest::~EAAlmacenamientoTest() {
	delete registro1;
	delete registro2;
	delete clave;
	delete escritor;
}

void EAAlmacenamientoTest::testInsertar(){
	escritor->logActivo=true;
	set(registro1,0,"paulo");
	escritor->insertar(registro1);
	set(registro1,1,"juan");
	escritor->insertar(registro1);
	set(registro1,2,"daniel");
	escritor->insertar(registro1);
	set(registro1,3,"maximo");
	escritor->insertar(registro1);
	set(registro1,4,"samuel");
	escritor->insertar(registro1);
	/*********************************/
	while(!escritor->hayMasCambios()){
		const Cambio* cambio=escritor->siguienteCambio();
		escritor->posicionarComponente(cambio->referencia);
		escritor->obtener(registro2);
		setClave(registro1,&cambio->clave);
		if(!assertIguales(registro1,registro2))
			throw ExcepcionEAA("No son iguales "+registro1->get(0)->getNombre());
		escritor->pop();
	};
}
void EAAlmacenamientoTest::testEliminar(){
	escritor->logActivo=false;
	/**********************************************************************/
	escritor->posicionarComponente(0);
	set(registro1,0,"paulo");
	escritor->eliminar(registro1);
	escritor->posicionarComponente(0);
	while( escritor->obtener(registro2) ){
		if(assertIguales(registro1,registro2))
			throw ExcepcionEAA("No son iguales "+ registro1->get(0)->getNombre() );
	}
	/**********************************************************************/
	escritor->posicionarComponente(0);
	set(registro1,0,"paulo");
	escritor->insertar(registro1);//reinserto para no tener espacios libres
	escritor->posicionarComponente(0);
	escritor->logActivo=true;
	set(registro1,5,"juan carlos antonio");// por ser el mas largo q los anterioes va al final
	escritor->insertar(registro1);
	const Cambio* cambio=escritor->siguienteCambio();
	escritor->posicionarComponente(cambio->referencia);
	escritor->pop();
	/**********************************************************************/
	escritor->eliminar(registro1);
	escritor->posicionarComponente(0);
	while( escritor->obtener(registro2) ){
		if(assertIguales(registro1,registro2))
			throw ExcepcionEAA("No son iguales "+ registro1->get(0)->getNombre() );
	}
}
void EAAlmacenamientoTest::testModificar(){
}

void EAAlmacenamientoTest::testObtener(){
	escritor->posicionarComponente(0);
	escritor->obtener(registro2);
	set(registro1,0,"paulo");
	if(!assertIguales(registro1,registro2))
		throw ExcepcionEAA("No son iguales "+registro1->get(0)->getNombre());
	escritor->obtener(registro2);
	set(registro1,1,"juan");
	if(!assertIguales(registro1,registro2))
		throw ExcepcionEAA("No son iguales "+registro1->get(0)->getNombre());
	escritor->obtener(registro2);
	set(registro1,2,"daniel");
	if(!assertIguales(registro1,registro2))
		throw ExcepcionEAA("No son iguales "+registro1->get(0)->getNombre());
	escritor->obtener(registro2);
	set(registro1,3,"maximo");
	if(!assertIguales(registro1,registro2))
		throw ExcepcionEAA("No son iguales "+registro1->get(0)->getNombre());
	escritor->obtener(registro2);
	set(registro1,4,"samuel");
	if(escritor->obtener(registro2))
		throw ExcepcionEAA("Obtuvo despues del ultimo");
}

