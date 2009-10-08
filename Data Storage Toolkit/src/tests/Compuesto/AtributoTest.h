/*
 * AtributoTest.h
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#ifndef ATRIBUTOTEST_H_
#define ATRIBUTOTEST_H_
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include "compuestolib.h"
using namespace std;
class AtributoTest {
	int nroprueba;
public:
	AtributoTest(){nroprueba=0;};
	virtual ~AtributoTest();
	void runTest(){
		int a=8;float b=0.1f;
		testAtributoFijo(a);
		testAtributoFijo(b);
		char prueba[10]="123456789";
		testAtributoFijoCadena(prueba,strlen(prueba));
		testAtributoVariable(a);
		testAtributoVariable(b);

	};
	void verificar(bool test,std::string mensaje){
		nroprueba++;
		if(!test)
			std::cerr<<"Error: "<<mensaje<<" nro "<<nroprueba<<endl;
	}
	template<typename T>
	void testAtributoFijo(T valor){
		T aux;
		AtributoFijo<T> atributo("nombre");
		atributo.set(&valor);
		atributo.get(&aux);
		verificar(valor==aux,"No se guarda correctamente");
		verificar(atributo.tamanioSerializado()==sizeof(T),"Tamanio Serializacion incorrecto");
		verificar(atributo.tamanio()==sizeof(T),"Tamanio incorrecto");
		verificar(atributo.esfijo(),"No es fijo");
		stringstream ostrm;
		atributo.serializar(ostrm);
		aux*=aux;
		atributo.set(&aux);
		atributo.deserializar(ostrm);
		atributo.get(&aux);
		verificar(valor==aux,"No se deserializacion.");
	}
	void testAtributoFijoCadena(const char* str,Ttamanio n){
		AtributoFijo<char*> atributo("nombre",n);
		char aux[n];
		atributo.set(&str);
		atributo.get(&aux);
		verificar(strncmp(str,aux,n),"No se guarda correctamente");
		verificar(atributo.tamanioSerializado()==n,"Tamanio Serializacion incorrecto");
		verificar(atributo.tamanio()==n,"Tamanio Serializacion incorrecto");
		verificar(!atributo.esfijo(),"No es fijo");
		stringstream ostrm;
		atributo.serializar(ostrm);
		strcpy(aux,"hola");
		atributo.set(aux);
		atributo.deserializar(ostrm);
		atributo.get(&aux);
		verificar(strncmp(str,aux,n),"No se deserializa correctamente.");
	}
	template<typename T>
	void testAtributoVariable(T valor){
		T aux;
		AtributoVariable<T> atributo("nombre");
		atributo.set(&valor);
		atributo.get(&aux);
		verificar(valor==aux,"No se guarda correctamente");
		verificar(atributo.tamanioSerializado()==(sizeof(T)+sizeof(Ttamanio)),"Tamanio Serializacion incorrecto");
		verificar(atributo.tamanio()==sizeof(T),"Tamanio incorrecto");
		verificar(atributo.esfijo(),"No es fijo");
		stringstream ostrm;
		atributo.serializar(ostrm);
		aux*=aux;
		atributo.set(&aux);
		atributo.deserializar(ostrm);
		atributo.get(&aux);
		verificar(valor==aux,"No se deserializacion.");
	}
	void testAtributoVariableCadena(const char* str,Ttamanio n){
		AtributoFijo<char*> atributo("nombre",n);
		char aux[n];
		atributo.set(&str);
		atributo.get(&aux);
		verificar(strncmp(str,aux,n),"No se guarda correctamente");
		verificar(atributo.tamanioSerializado()==n,"Tamanio Serializacion incorrecto");
		verificar(atributo.tamanio()==n,"Tamanio Serializacion incorrecto");
		verificar(!atributo.esfijo(),"No es fijo");
		stringstream ostrm;
		atributo.serializar(ostrm);
		strcpy(aux,"hola");
		atributo.set(aux);
		atributo.deserializar(ostrm);
		atributo.get(&aux);
		verificar(strncmp(str,aux,n),"No se deserializa correctamente.");
	}
};

#endif /* ATRIBUTOTEST_H_ */
