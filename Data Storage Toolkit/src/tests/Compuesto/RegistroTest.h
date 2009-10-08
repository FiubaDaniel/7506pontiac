/*
 * RegistroTest.h
 *
 *  Created on: 08/10/2009
 *      Author: paulo
 */

#ifndef REGISTROTEST_H_
#define REGISTROTEST_H_
#include "compuestolib.h"
class RegistroTest {
public:
	RegistroTest();
	virtual ~RegistroTest();
	void runTest(){
		AtributoFijo<int> padron("padron");
		int a=8;
		padron.set(&a);
		AtributoFijo<long> dni("dni");
		AtributoFijo<char*> nombre("nombre",15);
		char array[10]="hola";
		nombre.set(array);
		Registro reg(1,&nombre);
		//std::cout<<reg.cantidadAtributos();
		reg.serializar(std::cout);
	};
	void verificar(bool test,std::string mensaje){
		//nroprueba++;
		//if(!test)
			//std::cerr<<"Error: "<<mensaje<<" nro "<<nroprueba<<endl;
	}
};

#endif /* REGISTROTEST_H_ */
