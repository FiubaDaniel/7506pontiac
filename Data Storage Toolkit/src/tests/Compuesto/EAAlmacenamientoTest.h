/*
 * EAAlmacenamientoTest.h
 *
 *  Created on: 03/11/2009
 *      Author: paulo
 */

#ifndef EAALMACENAMIENTOTEST_H_
#define EAALMACENAMIENTOTEST_H_
#include "compuestolib.h"
#include "../../Almacenamiento/Almacenamiento.h"
#include "../../Almacenamiento/EstrategiaAlmacenamiento.h"
#include "../../Almacenamiento/EABloques.h"
#include "../../Almacenamiento/EARegistros.h"
#include "../../Almacenamiento/EATexto.h"
#include "../../Almacenamiento/Buffer.h"
#include "../../Almacenamiento/Archivo.h"
#include "../../ArbolBSharp/Clave.h"
#include "../../ArbolBSharp/ComparadorClave.h"
#include <string>
#include <exception>
using namespace std;
/*
class ExcepcionEAA : public exception{
private:
	string mensaje;
public:
	ExcepcionEAA(const string& mensaje)throw (){
		this->mensaje=mensaje;
	}
	virtual const char* what() const throw(){
		return mensaje.c_str();
	}
	~ExcepcionEAA () throw (){};
};

class ComparadorDni: public ComparadorClave{
public:
	ComparadorDni(){};
	int Comparar(const Clave* clave1,const Clave* clave2){
		Clave* aux1 = const_cast<Clave*>(clave1);
		Clave* aux2 = const_cast<Clave*>(clave2);
		return aux1->getAtributo("dni")->comparar(aux2->getAtributo("dni"));
	};
	virtual ~ComparadorDni(){};
};

class EAAlmacenamientoTest {
	EstrategiaAlmacenamiento* escritor;
	ComparadorDni comparador;
	bool registrofijo;
	Registro* registro1;
	Registro* registro2;
	Clave*clave;
	Archivo almacen;
public:
	EAAlmacenamientoTest(int tipo){
		this->registrofijo=(tipo>1);

		AtributoFijo<int> dni("dni");
		Atributo* nombre;
		if(registrofijo){
			nombre=new AtributoFijo<char*>("nombre",10);
		}else {
			nombre=new AtributoVariable<string>("nombre");
		};
		registro1=new Registro(2,&dni,nombre);
		registro2=new Registro(2,&dni,nombre);
		clave=new Clave(registro1,2,"dni","nombre");

		delete nombre;
		switch(tipo){
			case 0:
				escritor=new EATexto(registro1);
				almacen.crear("salida.txt");
			break;
			case 1:
				escritor=new EABloques(registro1,32);
				almacen.crear("salida.bloq");
			break;
			default :
				escritor=new EARegistros(registro1);
				almacen.crear("salida.reg");
			break;
		}

		escritor->crear(&almacen);
		escritor->setComparador(&comparador);
		escritor->setClave(clave);
	};
	void run(){
		try{
			testInsertar();
			testObtener();
			testEliminar();
			testModificar();
		}catch(exception& e){
			cout<<e.what()<<endl;
		};
	};
	void testInsertar();
	void testEliminar();
	void testModificar();
	void testObtener();
	void run2();
	void set(Registro*reg,int dni,string nombre){
		reg->get("dni")->set(&dni);
		if(registrofijo){
			reg->get("nombre")->set((void*)nombre.c_str());
		}else{
			reg->get("nombre")->set(&nombre);
		}
	}
	bool assertIguales(Registro*reg1,Registro*reg2){
		for(Ttamanio i=0;i<reg1->cantidadAtributos();i++){
			if(reg1->get(i)->comparar(reg2->get(i))!=0)
				return false;
		}
		return true;
	}
	void setClave(Registro*reg,const Clave*unclave){
		Clave*clave=const_cast<Clave*>(unclave);
		for(Ttamanio i=0;i<clave->getCantidadAtributos();i++){
			Atributo*att=clave->getAtributo(i);
			*reg->get(att->getNombre())=*att;
		}
	}
	virtual ~EAAlmacenamientoTest();
};*/


#endif /* EAALMACENAMIENTOTEST_H_ */
