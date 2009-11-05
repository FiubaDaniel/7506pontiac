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
#include "../../ArbolBSharp/Clave.h"
#include "../../ArbolBSharp/ComparadorClave.h"
#include <string>
#include <exception>
using namespace std;

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
public:
	EAAlmacenamientoTest(bool registrofijo,EstrategiaAlmacenamiento* escritor);
	void run(){
		try{
			testInsertar();
			testEliminar();
			testModificar();
			testObtener();
		}catch(exception& e){
			cout<<e.what()<<endl;
		};
	};
	void testInsertar();
	void testEliminar();
	void testModificar();
	void testObtener();

	void set(Registro*reg,int dni,string nombre){
		reg->get("dni")->set(&dni);
		if(registrofijo){
			reg->get("nombre")->set((void*)nombre.c_str());
		}else{
			reg->get("nombre")->set(&nombre);
		}
	}
	void assertIguales(Registro*reg1,Registro*reg2){
		for(Ttamanio i=0;i<reg1->cantidadAtributos();i++){
			if(reg1->get(0)->comparar(reg2->get(0))!=0){
				throw ExcepcionEAA("No son iguales"+reg1->get(0)->getNombre());
			};
		}
	}
	virtual ~EAAlmacenamientoTest();
};


#endif /* EAALMACENAMIENTOTEST_H_ */
