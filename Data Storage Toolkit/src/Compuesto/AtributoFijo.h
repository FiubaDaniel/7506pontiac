/*
 * AtributoFijo.h
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */
#ifndef ATRIBUTOFIJO_H_
#define ATRIBUTOFIJO_H_
#include <cstring>
#include <iostream>
#include "Atributo.h"
template<typename T_tipo>
class AtributoFijo : public Atributo{
private:
	T_tipo dato;
public:
	AtributoFijo(std::string nombreAtributo);
	virtual ~AtributoFijo();
	void set(void* valor);
	void get(void* valor);
	Ttamanio tamanio();
	Atributo* clonar();
	Ttamanio serializar(std::streambuf&salida);
	Ttamanio deserializar(std::streambuf&entrada);
	Ttamanio tamanioSerializado();
	bool esfijo();
	int comparar(Atributo*otroAtributo);
	void imprimir(std::ostream&salida);
	void leer(std::istream&entrada);
};
/*----------------------------------------------------------------------------*/
/*Template Control de tipo*/
template<typename T_tipo>
AtributoFijo<T_tipo>::AtributoFijo(std::string nombreAtributo): AtributoFijo::Atributo(nombreAtributo)
{};
template<typename T_tipo>
AtributoFijo<T_tipo>::~AtributoFijo(){};
/*----------------------------------------------------------------------------*/
/*Templates sin especializacion*/
template<typename T_tipo>
void AtributoFijo<T_tipo>::set(void* valor){
	dato=*(T_tipo*)valor;
};
template<typename T_tipo>
void AtributoFijo<T_tipo>::get(void* valor){*(T_tipo*)valor=dato;};
template<typename T_tipo>
Ttamanio AtributoFijo<T_tipo>::tamanio(){ return sizeof(T_tipo);};
template<typename T_tipo>
Atributo* AtributoFijo<T_tipo>::clonar(){
	AtributoFijo<T_tipo>*clon=new AtributoFijo<T_tipo>(nombre);
	clon->dato=dato;
	return clon;
};
template<typename T_tipo>
Ttamanio AtributoFijo<T_tipo>::serializar(std::streambuf&salida){
	Ttamanio tamanioDato=sizeof(dato);
	salida.sputn((char*)&dato,tamanioDato);
	return tamanioDato;
};
template<typename T_tipo>
Ttamanio AtributoFijo<T_tipo>::deserializar(std::streambuf&entrada){
	Ttamanio tamanioDato=sizeof(dato);
	entrada.sgetn((char*)&dato,tamanioDato);
	return tamanioDato;
};
template<typename T_tipo>
Ttamanio AtributoFijo<T_tipo>::tamanioSerializado(){
	return sizeof(T_tipo);
};
template<typename T_tipo>
bool AtributoFijo<T_tipo>::esfijo(){ return true;};
template<typename T_tipo>
int AtributoFijo<T_tipo>::comparar(Atributo*otroAtributo){
	AtributoFijo<T_tipo>* otro=dynamic_cast<AtributoFijo<T_tipo>*>(otroAtributo);
	if(otro!=NULL){
		return (this->dato)-(otro->dato);
	}//TODO exception
	return 0;
};
template<typename T_tipo>
void AtributoFijo<T_tipo>::imprimir(std::ostream&salida){
	salida<<dato;
};
template<typename T_tipo>
void AtributoFijo<T_tipo>::leer(std::istream&entrada){
	entrada>>dato;
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
		datos=new char[longitud];
	};
	virtual ~AtributoFijo(){delete[]datos;};
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
	Ttamanio serializar(std::streambuf&salida){
		salida.sputn(datos,longitud);
		return longitud;

	};
	Ttamanio deserializar(std::streambuf&entrada){
		entrada.sgetn(datos,longitud);
		return longitud;
	};
	Ttamanio tamanioSerializado(){return longitud;};
	bool esfijo(){
		return true;
	};
	int comparar(Atributo*otroAtributo){
		AtributoFijo<char*>* otro=dynamic_cast<AtributoFijo<char*>*>(otroAtributo);
		if(otro!=NULL){
			return strncmp(this->datos,otro->datos,longitud);
		}//TODO exception
		return 0;
	}
	void imprimir(std::ostream&salida){
		salida.write(datos,longitud);
	};

	void leer(std::istream&entrada){
		entrada.read(datos,longitud);
	};
};

/*----------------------------------------------------------------------------*/
#endif /* ATRIBUTOFIJO_H_ */






