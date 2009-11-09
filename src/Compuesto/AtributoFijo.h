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
/******************************** Miembros de AtributoFijo ********************************************/
	AtributoFijo(std::string nombreAtributo): AtributoFijo::Atributo(nombreAtributo){};

	~AtributoFijo(){};

	/*asignacion de un valor Segun el tipo del atributo*/
	AtributoFijo<T_tipo>& operator=(const T_tipo & valor){
		dato=valor;
		return *this;
	};
	/*Sobrecarga de cast para asignar el atributo a una variable*/
	operator T_tipo()const{
		return dato;
	}
/**********************Implementacion de metodos heredados*********************************************/
	Atributo& operator=(const Atributo& att) throw(std::bad_cast){
			AtributoFijo<T_tipo>& otro=dynamic_cast<AtributoFijo<T_tipo>&>(const_cast<Atributo&>(att));
			dato=otro.dato;
			return *this;
	};

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

	Ttamanio serializar(std::streambuf&salida)throw(IOSerializacionExcepcion){
		Ttamanio tamanioDato=sizeof(dato);
		salida.sputn((char*)&dato,tamanioDato);
		return tamanioDato;
	};

	Ttamanio deserializar(std::streambuf&entrada)throw(IOSerializacionExcepcion){
		long leidos=entrada.sgetn((char*)&dato,sizeof(dato));
		if(leidos!=sizeof(dato))
			throw IOSerializacionExcepcion("Excepcion:AtributoFijo "+nombre+" no fue deserializado");
		return sizeof(dato);
	};

	Ttamanio tamanioSerializado(){
		return sizeof(T_tipo);
	};


	int comparar(const Atributo *otroAtributo)throw(std::bad_cast){
		AtributoFijo<T_tipo>& otro=dynamic_cast<AtributoFijo<T_tipo>&>(*const_cast<Atributo*>(otroAtributo));
		return (this->dato)-(otro.dato);
	};

	void imprimir(std::ostream&salida){
		salida<<dato;
	};

	void leer(std::istream&entrada){
		entrada>>dato;
	};
	/**/


};
/*----------------------------------------------------------------------------------------------------*/
/*Especializacion de la clase para cadena de chars*/
template<>
class AtributoFijo<char*> : public Atributo{
private:
	char*datos;
	Ttamanio longitud;
public:
/******************************** Miembros de AtributoFijo ********************************************/
	AtributoFijo(std::string nombreAtributo,Ttamanio nroCaracteres): Atributo(nombreAtributo)
	{
		longitud=nroCaracteres;
		datos=new char[longitud+1];
		datos[longitud]=0;
	};
	/*Sobrecarga de la asignacion para asignar un puntero a char.
	 *carga el atributo con tamanio() char apartir del apuntado por valor
	 */
	AtributoFijo<char*>& operator=(const char* &valor){
		strncpy(datos,valor,longitud);
		return *this;
	};
	/*Sobrecarga de la asignacion para asignar un std::string.
	 *carga el atributo con tamanio() char a tomados de del string valor.
	 */
	AtributoFijo<char*>& operator=(const string & valor){
		strncpy(datos,valor.c_str(),longitud);
		return *this;
	};
	/*Sobrecarga de cast para obtener un puntero a sucesion de chars contenidos en atributo*/
	operator char*(){
		return datos;
	}
	/*Sobrecarga de cast para asignar el atributo a una string*/
	operator string(){
		return string(datos,longitud);
	}
	virtual ~AtributoFijo(){
		delete[]datos;
	};
/**********************Implementacion de metodos heredados*********************************************/
	Atributo& operator=(const Atributo& att)throw(std::bad_cast){
		AtributoFijo<char*>& otro=dynamic_cast<AtributoFijo<char*>&>(const_cast<Atributo&>(att));
		strncpy(datos,otro.datos,longitud);
		return *this;
	};
	void set(void* valor){
		strncpy(datos,(char*)valor,longitud);
	};
	void get(void* valor){
		strncpy((char*)valor,datos,longitud);
	};
	Ttamanio tamanio(){
		return longitud;
	};
	Atributo* clonar(){
		AtributoFijo<char*>* clon=new AtributoFijo<char*>(nombre,longitud);
		strncpy(clon->datos,datos,longitud);
		return clon;
	};
	Ttamanio serializar(std::streambuf&salida)throw(IOSerializacionExcepcion){
		salida.sputn(datos,longitud);
		return longitud;

	};
	Ttamanio deserializar(std::streambuf&entrada)throw(IOSerializacionExcepcion){
		long leidos=entrada.sgetn(datos,longitud);
		if(leidos!=longitud)
			throw IOSerializacionExcepcion("Excepcion:AtributoFijo "+nombre+" no fue deserializado");
		return longitud;
	};
	Ttamanio tamanioSerializado(){
		return longitud;
	};

	int comparar(const Atributo*otroAtributo)throw(std::bad_cast){
		const AtributoFijo<char*>& otro=dynamic_cast<AtributoFijo<char*>&>(*const_cast<Atributo*>(otroAtributo));
		return strcmp(this->datos,otro.datos);
	}
	void imprimir(std::ostream&salida){
		salida<<datos;
	};

	void leer(std::istream&entrada){
		entrada.getline(datos,longitud);
	};
};
/*----------------------------------------------------------------------------------------------------*/
/*  Especializacion punteros para evitar que se pueda crear un atributo q guarda un puntero , pues
 *	serializarlo/ deserializarlo no tiene sentido, ya que la posiciones de  memoria dependen del momento de ejecucion
 */
template<typename T_tipo>
class AtributoFijo<T_tipo*> : public Atributo {
private:
	AtributoFijo();
	virtual void dummy()=0;
};
/*----------------------------------------------------------------------------*/
#endif /* ATRIBUTOFIJO_H_ */






