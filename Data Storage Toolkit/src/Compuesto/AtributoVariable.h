/*
 * AtributoVariable.h
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */
#ifndef AtributoVariable_H_
#define AtributoVariable_H_
#include <cassert>
#include <vector>
#include "Atributo.h"

template<typename T_tipo>
class AtributoVariable: public Atributo {
private:
	std::vector<T_tipo*> valores;
	Ttamanio valorActual;
	void init(std::string nombreAtributo);
public:
	AtributoVariable(std::string nombreAtributo);//crea espacio para almenos un valor
	virtual ~AtributoVariable();
	void set(char* value);//modifican el valor apuntado
	void get(char* value);
	Ttamanio tamanio();
	Atributo* clonar();
	void serializar(std::ostream salida);
	void deserializar(std::istream entrada);
	Ttamanio tamanioSerializado();
public:
	/* metodo propios de la clase permiten iteracion sobre los valores guardados*/
	void appbytes(void*valor);// se agregan tantos bytes como indica nrobytes
	void apuntar(Ttamanio nroValor);
	void eliminarApuntado();
	void cantidadValores();
};
/*----------------------------------------------------------------------------*/
/*Template Control de tipo*/
template<typename T_tipo>
AtributoVariable<T_tipo>::AtributoVariable(std::string nombreAtributo){
	//TODO trow exception  o abortar compilacion

};
/*----------------------------------------------------------------------------*/
/*Templates sin especializacion*/
template<typename T_tipo>
AtributoVariable<T_tipo>::~AtributoVariable(){
		for(Ttamanio i= valores.size()-1;i>=0;i--){
			delete valores.at(i);
		}
};
template<typename T_tipo>
void AtributoVariable<T_tipo>::init(std::string nombreAtributo){
	nrobytes=sizeof(T_tipo);
	nombre=nombreAtributo;
	valores.push_back(new T_tipo);
	valorActual=0;
}
template<typename T_tipo>
void AtributoVariable<T_tipo>::set(char* value){
	cpy((char*)valores.at(valorActual),value,nrobytes);
};
template<typename T_tipo>
void AtributoVariable<T_tipo>::get(char* value){
	cpy(value,(char*)valores.at(valorActual),nrobytes);
};
template<typename T_tipo>
void AtributoVariable<T_tipo>::set(std::streambuf& pbuffer){
	pbuffer.sgetn((char*)valores.at(valorActual),nrobytes);
};
template<typename T_tipo>
void AtributoVariable<T_tipo>::get(std::streambuf& pbuffer){
	pbuffer.sputn((char*)valores.at(valorActual),nrobytes);
}
template<typename T_tipo>
Ttamanio AtributoVariable<T_tipo>::tamanio(){return sizeof(T_tipo);};
template<typename T_tipo>
bool AtributoFijo<T_tipo>::esfijo(){
	return false;
};
/*----------------------------------------------------------------------------*/
/*Templates Especializados contructores*/
template<>AtributoVariable<char>::AtributoVariable(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoVariable<short>::AtributoVariable(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoVariable<int>::AtributoVariable(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoVariable<long>::AtributoVariable(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoVariable<long long>::AtributoVariable(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoVariable<unsigned char>::AtributoVariable(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoVariable<unsigned short>::AtributoVariable(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoVariable<unsigned int>::AtributoVariable(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoVariable<unsigned long>::AtributoVariable(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoVariable<unsigned long long>::AtributoVariable(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoVariable<float>::AtributoVariable(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoVariable<double>::AtributoVariable(std::string nombreAtributo){init(nombreAtributo);}
template<>AtributoVariable<long double>::AtributoVariable(std::string nombreAtributo){init(nombreAtributo);}
/**/
/*Especializacion de la clase para cadena de chars*/
template<>
class AtributoVariable<char*>: public Atributo {
private:
	std::string str;
public:
	Ttamanio nrobytes;
	AtributoVariable(std::string nombreAtributo,Ttamanio inicial):Atributo(nombreAtributo){
		nrobytes=inicial;
	};
	virtual void set(void* value){
		str="";
		str.insert(0,value,nrobytes);
	};
	virtual void get(void* value){
		str.copy(value,nrobytes);
	};
	Ttamanio cantidadbytes(){return str.size();};
	bool esfijo(){
		return false;
	};
	virtual ~AtributoVariable(){};

};

/*----------------------------------------------------------------------------------------------------*/
/*Especializacion punteros*/
template<typename T_tipo>
class AtributoVariable<T_tipo*>{};
/*----------------------------------------------------------------------------------------------------*/
#endif /* AtributoVariable_H_ */






