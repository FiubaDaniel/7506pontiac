/*
 * AtributoFijo.h
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */
#ifndef ATRIBUTOFIJO_H_
#define ATRIBUTOFIJO_H_
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
	Ttamanio serializar(std::ostream&salida);
	Ttamanio deserializar(std::istream&entrada);
	Ttamanio tamanioSerializado();
	bool esfijo();
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
void AtributoFijo<T_tipo>::set(void* valor){dato=*(T_tipo*)valor;};
template<typename T_tipo>
void AtributoFijo<T_tipo>::get(void* valor){*(T_tipo*)valor=dato;};
template<typename T_tipo>
Ttamanio AtributoFijo<T_tipo>::tamanio(){ return sizeof(T_tipo*);};
template<typename T_tipo>
Atributo* AtributoFijo<T_tipo>::clonar(){ return new AtributoFijo<T_tipo>(nombre);};
template<typename T_tipo>
Ttamanio AtributoFijo<T_tipo>::serializar(std::ostream&salida){
	Ttamanio tamanioDato=sizeof(dato);
	salida.write((char*)&dato,tamanioDato);
	return tamanioDato;
};
template<typename T_tipo>
Ttamanio AtributoFijo<T_tipo>::deserializar(std::istream&entrada){
	Ttamanio tamanioDato=sizeof(dato);
	entrada.read((char*)&dato,tamanioDato);
	return tamanioDato;
};
template<typename T_tipo>
Ttamanio AtributoFijo<T_tipo>::tamanioSerializado(){
	return sizeof(T_tipo)+sizeof(Ttamanio);
};
template<typename T_tipo>
bool AtributoFijo<T_tipo>::esfijo(){
	return true;
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
	void imprimir(std::ostream &salida){
		salida<<datos;
	};
	void leer(std::istream &entrada){
		entrada>>datos;
	};
	void set(void* valor){
		cpy(datos,(char*)valor,longitud);
	};
	void get(void* valor){
		cpy((char*)valor,datos,longitud);
	};
	Ttamanio tamanio(){return longitud;};
	Atributo* clonar(){
		AtributoFijo<char*>* clon=new AtributoFijo<char*>(nombre,longitud);
		cpy(clon->datos,datos,longitud);
		return clon;
	};
	Ttamanio serializar(std::ostream&salida){
		salida.write(datos,longitud);
		return longitud;

	};
	Ttamanio deserializar(std::istream&entrada){
		entrada.read(datos,longitud);
		return longitud;
	};
	Ttamanio tamanioSerializado(){return longitud+sizeof(Ttamanio);};
	bool esfijo(){
		return true;
	};
};

/*----------------------------------------------------------------------------*/
#endif /* ATRIBUTOFIJO_H_ */






