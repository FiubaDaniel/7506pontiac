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
template<typename T>
class ProvadorAtributoFijo{
	AtributoFijo<T> at1;
	AtributoFijo<T> at2;
	T valor1;
	T valor2;
public:
	ProvadorAtributoFijo(T valor1,T valor2,string nombre):at1(nombre),at2(nombre+"2"){
		this->valor1=valor1;
		this->valor2=valor2;
		cout<<boolalpha;
		cout<<"SetGet "<<testgetySet()<<endl;
		cout<<"Serializacion "<<testserializarDeserializar()<<endl;
		cout<<"esFijo "<<testEsfijo()<<endl;
		cout<<"Comparar "<<testcomparar()<<endl;
		cout<<"ImprimirLeer "<<testimprimirYleer(valor1)<<endl;
		cout<<"Nombre "<<testNombre()<<endl;
	};
	~ProvadorAtributoFijo(){};
	bool testNombre(){
		return at1.getNombre()+"2"==at2.getNombre();
	}
	bool testgetySet(){
		T aux=valor2;
		at1.set(&valor1);
		at1.get(&aux);
		return (aux==valor1)&& (at1.tamanio()==sizeof(T));
	};
	bool testserializarDeserializar(){
		stringbuf str;
		at1.serializar(str);
		Ttamanio tamanio=str.str().size();
		at2.deserializar(str);
		T aux;
		at1.get(&valor1);
		at2.get(&aux);
		return (valor1==aux)&&(tamanio==at1.tamanioSerializado());
	};
	bool testEsfijo(){
		return at1.esfijo()&& at2.esfijo();
	};
	bool testcomparar(){
		at1.get(&valor1);
		at2.get(&valor2);
		return (valor1-valor2)==( at1.comparar(&at2) );
	};
	bool testimprimirYleer(T& valor1){
		stringstream stream;
		T aux;
		at1.set(&valor1);
		at1.imprimir(stream);
		at2.leer(stream);
		at2.get(&aux);
		return aux==valor1;
	};
};
class AtributoTest {

public:
	AtributoTest(){
		ProvadorAtributoFijo<int>(111,8,"entero");
		ProvadorAtributoFijo<short>(2,15,"entero");
		ProvadorAtributoFijo<long>(7,11,"entero");
		ProvadorAtributoFijo<float>(0.02,8000,"entero");
		ProvadorAtributoFijo<char>('a','z',"entero");
		ProvadorAtributoFijo<double>(0.02548,25489.289,"entero");
	};
	virtual ~AtributoTest(){};
	void CasoEspeacial(){
		char str1[15]="un string";
		char str2[15]="123 string";
		char aux[15];
		AtributoFijo<char*> at1("string",15);
		AtributoFijo<char*> at2("string",15);
		cout<<(at1.tamanioSerializado()==15)<<endl;
		cout<<(at1.tamanio()==15)<<endl;
		at1.set(str1);
		at1.get(aux);
		cout<<(0==strcmp(aux,str1))<<endl;
		at2.set(aux);

	}
};

#endif /* ATRIBUTOTEST_H_ */
