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
}
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
	set(registro1,2,"dani");
	escritor->insertar(registro1);
	set(registro1,3,"maxi");
	escritor->insertar(registro1);
	set(registro1,4,"samuel");
	escritor->insertar(registro1);
	/*********************************/
	while(!escritor->NoHayMasCambios()){
		const Cambio* cambio=escritor->siguienteCambio();
		escritor->posicionarComponente(cambio->referencia);
		Atributo*att=(const_cast<Cambio*>(cambio))->clave.getAtributo("dni");
		*registro2->get("dni")=*att;
		escritor->obtener(registro2);
		setClave(registro1,&cambio->clave);
		if(!assertIguales(registro1,registro2)){
			imprimir(registro1);
			throw ExcepcionEAA("No son iguales "+registro1->get(0)->getNombre());
		}
		escritor->pop();
	};
}
void EAAlmacenamientoTest::testEliminar(){

	/**********************************************************************/
	escritor->logActivo=true;
	escritor->posicionarComponente(0);
	set(registro1,0,"paulo");
	escritor->eliminar(registro1);
	escritor->posicionarComponente(0);
	set(registro1,1,"juan");
	escritor->eliminar(registro1);
	while(!escritor->NoHayMasCambios()){
		const Cambio* cambio=escritor->siguienteCambio();
		setClave(registro1,&cambio->clave);
		escritor->posicionarComponente(0);
		if(cambio->operacion==Cambio::Baja){
			while(escritor->siguiente(registro2)){
				if(assertIguales(registro1,registro2)){
					imprimir(registro1);
					throw ExcepcionEAA("No Elimino "+ registro1->get(0)->getNombre() );
				}
			}
		}
		escritor->pop();
	}
	escritor->logActivo=false;
	/**********************************************************************/
	escritor->posicionarComponente(0);
	set(registro1,0,"paulo");
	escritor->insertar(registro1);
	set(registro1,1,"juan");
	escritor->insertar(registro1);
	escritor->posicionarComponente(0);
	set(registro1,7,"pepe");
	escritor->insertar(registro1);

/*	escritor->posicionarComponente(0);
	while( escritor->siguiente(registro2) ){
		cout<<escritor->posicionComponente()<<" ";
		imprimir(registro2);
	}
	cout<<endl;*/
	/**********************************************************************/
	escritor->logActivo=true;
	escritor->posicionarComponente(2);
	set(registro1,0,"paulo");
	escritor->eliminar(registro1);
	escritor->posicionarComponente(2);
	set(registro1,7,"pepe");
	escritor->eliminar(registro1);

	while(!escritor->NoHayMasCambios()){
		const Cambio* cambio=escritor->siguienteCambio();
		setClave(registro1,&cambio->clave);
		escritor->posicionarComponente(0);
		while(escritor->siguiente(registro2)){
			if(assertIguales(registro1,registro2)){
				throw ExcepcionEAA("No se elimino"+ registro1->get(0)->getNombre() );
			}
		}
		if(cambio->operacion!=Cambio::Baja)
			throw ExcepcionEAA("Hubo una operacion distinta de baja para una eliminacion en el extremo"+ registro1->get(0)->getNombre() );
		escritor->pop();
	}
	escritor->logActivo=false;
	/**********************************************************************/
	/*escritor->posicionarComponente(0);
	while( escritor->siguiente(registro2) ){
		cout<<escritor->posicionComponente()<<" ";
		imprimir(registro2);
	}
	cout<<endl;*/

	/**********************************************************************/
	escritor->posicionarComponente(2);
	set(registro1,0,"paulo");
	escritor->insertar(registro1);
	escritor->posicionarComponente(2);
	set(registro1,7,"pepe");
	escritor->insertar(registro1);
	/**********************************************************************/
	escritor->logActivo=true;
	escritor->posicionarComponente(1);
	set(registro1,2,"dani");
	escritor->eliminar(registro1);
	escritor->posicionarComponente(1);
	set(registro1,3,"maxi");
	escritor->eliminar(registro1);
	while(!escritor->NoHayMasCambios()){
		const Cambio* cambio=escritor->siguienteCambio();
		setClave(registro1,&cambio->clave);
		escritor->posicionarComponente(0);
		if(cambio->operacion==Cambio::Baja){
			while(escritor->siguiente(registro2)){
				if(assertIguales(registro1,registro2)){
					imprimir(registro1);
					throw ExcepcionEAA("No Elimino "+ registro1->get(0)->getNombre() );
				}
			}
		}
		escritor->pop();
	}
	escritor->logActivo=false;
	/**********************************************************************/
	/**********************************************************************/
	escritor->posicionarComponente(0);
	while( escritor->siguiente(registro2) ){
		cout<<escritor->posicionComponente()<<" ";
		imprimir(registro2);
	}
	cout<<endl;
}


void EAAlmacenamientoTest::testObtener(){
	escritor->posicionarComponente(0);
	set(registro2,0,"");
	escritor->siguiente(registro2);
	set(registro1,0,"paulo");
	if(!assertIguales(registro1,registro2))
		throw ExcepcionEAA("No son iguales "+registro1->get(0)->getNombre());
	set(registro2,1,"");
	escritor->siguiente(registro2);
	set(registro1,1,"juan");
	if(!assertIguales(registro1,registro2))
		throw ExcepcionEAA("No son iguales "+registro1->get(0)->getNombre());
	set(registro2,2,"");
	escritor->siguiente(registro2);
	set(registro1,2,"dani");
	if(!assertIguales(registro1,registro2))
		throw ExcepcionEAA("No son iguales "+registro1->get(0)->getNombre());
	set(registro2,3,"");
	escritor->siguiente(registro2);
	set(registro1,3,"maxi");
	if(!assertIguales(registro1,registro2))
		throw ExcepcionEAA("No son iguales "+registro1->get(0)->getNombre());
	set(registro2,4,"");
	escritor->siguiente(registro2);
	set(registro1,4,"samuel");
	if(escritor->siguiente(registro2))
		throw ExcepcionEAA("Obtuvo despues del ultimo");
}
void EAAlmacenamientoTest::testModificar(){
	escritor->logActivo=false;
	escritor->posicionarComponente(0);
	set(registro1,4,"juan martin gaviota");
	escritor->modificar(registro1);
	escritor->posicionarComponente(0);
	set(registro1,1,"ju");
	escritor->modificar(registro1);
	escritor->posicionarComponente(1);
	set(registro1,7,"alice cooper sideshow");
	escritor->modificar(registro1);
	escritor->posicionarComponente(3);
	set(registro1,7,"alice cooper");
	escritor->modificar(registro1);
	escritor->posicionarComponente(0);
	while( escritor->siguiente(registro2) ){
		cout<<escritor->posicionComponente()<<" ";
		imprimir(registro2);
	}
	cout<<endl;
/*		while(!escritor->hayMasCambios()){
			const Cambio* cambio=escritor->siguienteCambio();
			setClave(registro1,&cambio->clave);
			escritor->posicionarComponente(0);
			if(cambio->operacion==Cambio::Baja){
				while(escritor->siguiente(registro2)){
					if(assertIguales(registro1,registro2)){
						imprimir(registro1);
						throw ExcepcionEAA("No Elimino "+ registro1->get(0)->getNombre() );
					}
				}
			}
			escritor->pop();
		}
		escritor->logActivo=false;*/


}
void EAAlmacenamientoTest::run2(){


}
