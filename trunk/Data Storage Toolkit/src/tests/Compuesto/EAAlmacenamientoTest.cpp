/*
 * EAAlmacenamientoTest.cpp
 *
 *  Created on: 03/11/2009
 *      Author: paulo
 */

#include "EAAlmacenamientoTest.h"



EAAlmacenamientoTest::EAAlmacenamientoTest(bool registrofijo,EstrategiaAlmacenamiento* escritor){
	this->registrofijo=registrofijo;
	this->escritor=escritor;
	this->escritor->setComparador(&comparador);
	AtributoFijo<int> dni("dni");
	Atributo* nombre;
	if(registrofijo){
		nombre=new AtributoFijo<char*>("nombre",10);
	}else {
		nombre=new AtributoVariable<string>("nombre");
	};
	registro1=new Registro(2,&dni,&nombre);
	registro2=new Registro(2,&dni,&nombre);
	clave=new Clave(registro1,1,"dni");
	this->escritor->setClave(clave);
	delete nombre;
}

EAAlmacenamientoTest::~EAAlmacenamientoTest() {
	delete registro1;
	delete registro2;
	delete clave;
}

void EAAlmacenamientoTest::testInsertar(){
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
	escritor->posicionarComponente(0);

	escritor->obtener(registro2);
	set(registro1,0,"paulo");
	assertIguales(registro1,registro2);

	escritor->obtener(registro2);
	set(registro1,1,"juan");
	assertIguales(registro1,registro2);

	escritor->obtener(registro2);
	set(registro1,2,"daniel");
	assertIguales(registro1,registro2);

	escritor->obtener(registro2);
	set(registro1,3,"maximo");
	assertIguales(registro1,registro2);
	escritor->obtener(registro2);
	set(registro1,4,"samuel");
	assertIguales(registro1,registro2);

	/*set(registro1,4,"jasinto");
	escritor->insertar(registro1);
	set(registro1,5,"carlos augusto");
	escritor->insertar(registro1);
	set(registro1,6,"juan");
	escritor->insertar(registro1);
	escritor->posicionarComponente(0);
	set(registro1,0,"p");
	escritor->insertar(registro1);
	set(registro1,7,"r");
	escritor->insertar(registro1);
	set(registro1,8,"d");
	escritor->insertar(registro1);
	set(registro1,9,"mo");
	escritor->insertar(registro1);
	escritor->posicionarComponente(5);
	set(registro1,10,"ja");
	escritor->insertar(registro1);
	set(registro1,11,"carlos augusto segundo");
	escritor->insertar(registro1);
	set(registro1,12,"andrea");
	escritor->insertar(registro1);*/
}
void EAAlmacenamientoTest::testEliminar(){


}
void EAAlmacenamientoTest::testModificar(){
}

void EAAlmacenamientoTest::testObtener(){
}
