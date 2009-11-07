/*
 * AtributoFijo.h
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */
#ifndef ATRIBUTOFIJO_H_
#define ATRIBUTOFIJO_H_
#include <cstring>
#include <string>
#include <iostream>
#include "Atributo.h"
using namespace std;
template<typename T_tipo>
class AtributoFijo : public Atributo{
private:
	T_tipo dato;
public:
	AtributoFijo(std::string nombreAtributo): AtributoFijo::Atributo(nombreAtributo){};

	~AtributoFijo(){};

	Atributo& operator=(const Atributo& att){
			AtributoFijo<T_tipo>& otro=dynamic_cast<AtributoFijo<T_tipo>&>(const_cast<Atributo&>(att));
			dato=otro.dato;
			return *this;
	};
	AtributoFijo<T_tipo>& operator=(const T_tipo & valor){
		dato=valor;
		return *this;
	};
	operator T_tipo()const{
		return dato;
	}

	void set(const T_tipo valor){
		dato=valor;
	};

	void get(T_tipo& valor)const{
		valor=dato;
	};

	void set(void* valor){
		dato=*(T_tipo*)valor;
	};

	void get(void* valor){
		*(T_tipo*)valor=dato;
	};

	Ttamanio tamanio(){
		return sizeof(T_tipo);
	};

	Atributo* clonar(){
		AtributoFijo<T_tipo>*clon=new AtributoFijo<T_tipo>(nombre);
		clon->dato=dato;
		return clon;
	};

	Ttamanio serializar(std::streambuf&salida)throw(ErrorSerializacionExcepcion){
		Ttamanio tamanioDato=sizeof(dato);
		salida.sputn((char*)&dato,tamanioDato);
		return tamanioDato;
	};

	Ttamanio deserializar(std::streambuf&entrada)throw(ErrorSerializacionExcepcion){
		Ttamanio tamanioDato=sizeof(dato);
		if(entrada.sgetn((char*)&dato,tamanioDato)!=tamanioDato)
			throw ErrorSerializacionExcepcion("Excepcion:AtributoFijo "+nombre+" no fue deserializado");//TODO
		return tamanioDato;
	};

	Ttamanio tamanioSerializado(){
		return sizeof(T_tipo);
	};


	int comparar(const Atributo*otroAtributo){
		AtributoFijo<T_tipo>* otro=dynamic_cast<AtributoFijo<T_tipo>*>(const_cast<Atributo*>(otroAtributo));
		return (this->dato)-(otro->dato);
	};

	void imprimir(std::ostream&salida){
		salida<<dato;
	};

	void leer(std::istream&entrada){
		entrada>>dato;
	};


};
/*----------------------------------------------------------------------------------------------------*/
/*Especializacion punteros*/
template<typename T_tipo>
class AtributoFijo<T_tipo*> : public Atributo {
private:
	AtributoFijo();
	virtual void dummy()=0;
};
/*----------------------------------------------------------------------------------------------------*/
/*Especializacion de la clase para cadena de chars*/
template<>
class AtributoFijo<char*> : public Atributo{
private:
	char*datos;
	Ttamanio longitud;
public:
	AtributoFijo(std::string nombreAtributo,Ttamanio nroCaracteres): Atributo(nombreAtributo)
	{
		longitud=nroCaracteres;
		datos=new char[longitud+1];
		datos[longitud]=0;
	};
	Atributo& operator=(const Atributo& att){
		AtributoFijo<char*>& otro=dynamic_cast<AtributoFijo<char*>&>(const_cast<Atributo&>(att));
		strncpy(datos,otro.datos,longitud);
		return *this;
	};
	AtributoFijo<char*>& operator=(const char* &valor){
		strncpy(datos,valor,longitud);
		return *this;
	};

	AtributoFijo<char*>& operator=(const string & valor){
		strncpy(datos,valor.c_str(),longitud);
		return *this;
	};
	operator char*(){
		return datos;
	}

	virtual ~AtributoFijo(){
		delete[]datos;
	};
	void set(void* valor){
		strncpy(datos,(char*)valor,longitud);
	};
	void get(void* valor){
		strncpy((char*)valor,datos,longitud);
	};
	Ttamanio tamanio(){return longitud;};
	Atributo* clonar(){
		AtributoFijo<char*>* clon=new AtributoFijo<char*>(nombre,longitud);
		strncpy(clon->datos,datos,longitud);
		return clon;
	};
	Ttamanio serializar(std::streambuf&salida)throw(ErrorSerializacionExcepcion){
		salida.sputn(datos,longitud);
		return longitud;

	};
	Ttamanio deserializar(std::streambuf&entrada)throw(ErrorSerializacionExcepcion){
		if(entrada.sgetn(datos,longitud)!=longitud)
			throw ErrorSerializacionExcepcion("Excepcion:AtributoFijo "+nombre+" no fue deserializado");//TODO
		return longitud;
	};
	Ttamanio tamanioSerializado(){return longitud;};
	int comparar(const Atributo*otroAtributo){
		const AtributoFijo<char*>* otro=dynamic_cast<AtributoFijo<char*>*>(const_cast<Atributo*>(otroAtributo));

		return strcmp(this->datos,otro->datos);
	}
	void imprimir(std::ostream&salida){
		salida<<datos;
	};

	void leer(std::istream&entrada){
		entrada.getline(datos,longitud);
	};



};

/*----------------------------------------------------------------------------*/
#endif /* ATRIBUTOFIJO_H_ */






