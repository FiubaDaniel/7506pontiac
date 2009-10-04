/*
 * AtributoFijo.h
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */

#ifndef ATRIBUTOFIJO_H_
#define ATRIBUTOFIJO_H_
#include <cassert>
#include "Atributo.h"


template<typename T_tipo>
class AtributoFijo: public Atributo {
private:
	char*datos;
	void init(std::string nombreAtributo);
public:
	AtributoFijo(std::string nombreAtributo);
	void imprimir(std::ostream salida);
	void leer(std::istream entrada);
	void setbytes(char* value);
	void getbytes(char* value);
	void setbytes(std::streambuf& pbuffer);
	void getbytes(std::streambuf& pbuffer);
	Ttamanio cantidadbytes();
	Atributo* clonar();
	virtual ~AtributoFijo();
};
/*----------------------------------------------------------------------------*/
/*Template Control de tipo*/
template<typename T_tipo>
AtributoFijo<T_tipo>::AtributoFijo(std::string nombreAtributo){
	assert(false);
	//TODO evitar compilacion
};
/*----------------------------------------------------------------------------*/
/*Templates sin especializacion*/
template<typename T_tipo>
AtributoFijo<T_tipo>::~AtributoFijo(){
	delete[] datos;
};
template<typename T_tipo>
void AtributoFijo<T_tipo>::init(std::string nombreAtributo){
	nrobytes=sizeof(T_tipo);
	datos=new char[nrobytes];
	_nombre=nombreAtributo;
}
template<typename T_tipo>
void AtributoFijo<T_tipo>::imprimir(std::ostream salida){
	salida<< (T_tipo*)datos;
};
template<typename T_tipo>
void AtributoFijo<T_tipo>::leer(std::istream entrada){
	//TODO especializacion clases o abortar compilacion
	T_tipo* aux=(T_tipo*)datos;
	entrada>>*aux;
};
template<typename T_tipo>
void AtributoFijo<T_tipo>::setbytes(char* value){
	cpy(datos,value,nrobytes);
};
template<typename T_tipo>
void AtributoFijo<T_tipo>::getbytes(char* value){
	cpy(value,datos,nrobytes);
};
template<typename T_tipo>
void AtributoFijo<T_tipo>::setbytes(std::streambuf& pbuffer){
	pbuffer.sgetn(datos,nrobytes);
};
template<typename T_tipo>
void AtributoFijo<T_tipo>::getbytes(std::streambuf& pbuffer){
	pbuffer.sputn(datos,nrobytes);
}
template<typename T_tipo>
Ttamanio AtributoFijo<T_tipo>::cantidadbytes(){return sizeof(T_tipo);};
template<typename T_tipo>
Atributo* AtributoFijo<T_tipo>::clonar(){
	return new AtributoFijo<T_tipo>(_nombre);
};
/*----------------------------------------------------------------------------*/
/*Templates Especializados contructores*/
template<>AtributoFijo<char>::AtributoFijo(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoFijo<short>::AtributoFijo(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoFijo<int>::AtributoFijo(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoFijo<long>::AtributoFijo(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoFijo<long long>::AtributoFijo(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoFijo<unsigned char>::AtributoFijo(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoFijo<unsigned short>::AtributoFijo(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoFijo<unsigned int>::AtributoFijo(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoFijo<unsigned long>::AtributoFijo(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoFijo<unsigned long long>::AtributoFijo(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoFijo<float>::AtributoFijo(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoFijo<double>::AtributoFijo(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoFijo<long double>::AtributoFijo(std::string nombreAtributo){init(nombreAtributo);}
/*----------------------------------------------------------------------------------------------------*/
/*Especializacion de la clase para cadena de chars*/
template<>
class AtributoFijo<char*>: public Atributo {
private:
	char*datos;
	Ttamanio longitud;
public:
	Ttamanio nrobytes;
	AtributoFijo(std::string nombreAtributo,Ttamanio n):Atributo(nombreAtributo){
		nrobytes=longitud=n;
		datos=new char[longitud];
	};
	void imprimir(std::ostream salida){
		salida.write(datos,nrobytes);
	};
	void leer(std::istream entrada){
		entrada.read(datos,nrobytes);
	};
	virtual void setbytes(char* value){
		cpy(datos,value,nrobytes);
	};
	virtual void getbytes(char* value){
		cpy(value,datos,nrobytes);
	};
	void setbytes(std::streambuf& pbuffer){
		pbuffer.sgetn(datos,nrobytes);
	};
	void getbytes(std::streambuf& pbuffer){
		pbuffer.sputn(datos,nrobytes);
	};
	Ttamanio cantidadbytes(){return nrobytes;};
	virtual ~AtributoFijo(){
		delete[] datos;
	};
	Atributo* clonar(){
		return new AtributoFijo<char*>(this->_nombre,nrobytes);
	};
};
/*----------------------------------------------------------------------------------------------------*/
/*Especializacion punteros*/
template<typename T_tipo>
class AtributoFijo<T_tipo*>: public Atributo {
public:
	AtributoFijo(std::string nombreAtributo):Atributo(nombreAtributo){
		//TODO throw exception
		assert(false);
	};
	void imprimir(std::ostream salida){assert(false);};
	void leer(std::istream entrada){assert(false);};
	virtual void setbytes(char* value){assert(false);};
	virtual void getbytes(char* value){assert(false);};
	void setbytes(std::streambuf& pbuffer){assert(false);};
	void getbytes(std::streambuf& pbuffer){assert(false);};
	Ttamanio cantidadbytes(){assert(false);return 0;};
	virtual ~AtributoFijo(){assert(false);};
};
/*----------------------------------------------------------------------------*/
#endif /* ATRIBUTOFIJO_H_ */






