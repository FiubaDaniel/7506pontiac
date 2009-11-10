/*
 * AtributoVariable.h
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */
#ifndef AtributoVariable_H_
#define AtributoVariable_H_
#include <string>
#include <sstream>
#include <vector>
#include "Atributo.h"
using namespace std;


template<typename T_tipo>
class AtributoVariable: public Atributo {
private:
	std::vector<T_tipo> valores;
public:
/******************************** Miembros de AtributoFijo ********************************************/
	AtributoVariable(std::string nombreAtributo):Atributo(nombreAtributo){
	};
	virtual ~AtributoVariable(){};

	std::vector<T_tipo>& getVector(){
		return valores;
	};
/**********************Implementacion de metodos heredados*********************************************/
	Atributo& operator=(const Atributo& att)throw(std::bad_cast){
		AtributoVariable<T_tipo>& otro=dynamic_cast<AtributoVariable<T_tipo>&>(const_cast<Atributo&>(att));
		valores.clear();
		valores.assign(otro.valores.begin(),otro.valores.end());
		return *this;
	};

	void set(void* value){
		char*pc=(char*)value;
		unsigned char cantValores=*pc;
		pc++;
		Ttamanio i=0;
		T_tipo *pValor=(T_tipo*)pc;
		while(i< cantValores and i<valores.size() ){
			valores.at(i)=*pValor;
			pValor++;
			i++;
		}
		/*sobran valores en el vector*/
		while(i < valores.size()){
			valores.erase(valores.begin()+i);
		}
		/*quedan valores por cargar a la lista*/
		while(i<cantValores){
			valores.push_back(*pValor);
			pValor++;
			i++;
		}
	};
	void get(void* value){
		char*pc=(char*)value;
		*pc=valores.size();
		pc++;
		T_tipo *pValor=(T_tipo*)pc;
		for(Ttamanio i=0;i<valores.size();i++){
			*pValor=valores.at(i);
			pValor++;
		}
	};
	Ttamanio tamanio(){
		return tamanioSerializado();
	};

	Atributo* clonar(){
		AtributoVariable<T_tipo> *clon=new AtributoVariable<T_tipo>(nombre);
		clon->valores.clear();
		for(Ttamanio i=0;i<valores.size();i++){
			clon->valores.push_back(valores.at(i));
		}
		return clon;
	};

	Ttamanio serializar(std::streambuf &salida)throw(IOSerializacionExcepcion){
		Ttamanio offset=sizeof(unsigned char);
		unsigned char aux=valores.size();
		salida.sputn((char*)&aux,offset);
		for(Ttamanio i=0;i<valores.size();i++){
			T_tipo aux=valores.at(i);
			salida.sputn((char*)&aux,sizeof(T_tipo));
			offset+=sizeof(T_tipo);
		}
		return offset;
	};

	Ttamanio deserializar(std::streambuf &entrada)throw(IOSerializacionExcepcion){
		Ttamanio offset=sizeof(unsigned char);
		unsigned char nroValores=0;
		long leidos=entrada.sgetn((char*)&nroValores,offset);
		if(leidos!=offset)
			throw IOSerializacionExcepcion("Excepcion:AtributoVariable "+nombre+" no fue deserializado");
		Ttamanio i=0;
		T_tipo aux;
		while(i< nroValores and i<valores.size() ){
			leidos=entrada.sgetn((char*)&aux,sizeof(T_tipo));
			if(leidos !=sizeof(T_tipo))
				IOSerializacionExcepcion("Excepcion:AtributoVariable "+nombre+" no fue deserializado");
			valores.at(i)=aux;
			offset+=sizeof(T_tipo);
			i++;
		}
		while(i < valores.size()){
			valores.erase(valores.begin()+i);
		}
		while(i<nroValores){
			leidos=entrada.sgetn((char*)&aux,sizeof(T_tipo));
			if(leidos!=sizeof(T_tipo))
				throw IOSerializacionExcepcion("Excepcion:AtributoVariable "+nombre+" no fue deserializado");
			valores.push_back(aux);
			offset+=sizeof(T_tipo);
			i++;
		}
		return offset;
	};

	Ttamanio tamanioSerializado(){
		return valores.size()*sizeof(T_tipo)+sizeof(unsigned char);
	};

	int comparar(const Atributo*otroAtributo)throw(std::bad_cast){
		AtributoVariable<T_tipo>& otro=dynamic_cast<AtributoVariable<T_tipo> &>(*const_cast<Atributo*>(otroAtributo));
		return this->valores.at(0)- otro.valores.at(0);
	};

	void imprimir(std::ostream&salida){
		for(Ttamanio i=0;i<valores.size();i++){
				salida<<valores.at(i)<<" ";
		}
	};
	void leer(std::istream&entrada){
		for(Ttamanio i=0;i<valores.size();i++){
					entrada>>valores.at(i);
		}
	};

};

/*----------------------------------------------------------------------------*/
/*Templates Especializados contructores*/
/**/
/*Especializacion de la clase para cadena de chars*/
template<>
class AtributoVariable<string>: public Atributo {
private:
	string str;
public:
/******************************** Miembros de AtributoFijo ********************************************/
	AtributoVariable(std::string nombreAtributo):Atributo(nombreAtributo){};

	AtributoVariable<string>& operator=(const string & valor){
		str=valor;
		return *this;
	};
	operator string()const{
		return str;
	}
/**********************Implementacion de metodos heredados*********************************************/
	virtual void set(void* value){
		char* pc=(char*)value;
		unsigned char cantChar=*pc;
		pc++;
		str.assign(pc,cantChar);
	};
	virtual void get(void* value){
		char* pc=(char*)value;
		*pc=str.size();
		pc++;
		memcpy(pc,str.data(),str.size());
	};
	Ttamanio cantidadbytes(){return str.size()+sizeof(unsigned char);};

	int comparar(const Atributo*otroAtributo)throw(std::bad_cast){
		AtributoVariable<std::string>& otro=dynamic_cast<AtributoVariable<std::string> &>(*const_cast<Atributo*>(otroAtributo));
		return str.compare(otro.str);
	};
	void imprimir(std::ostream&salida){
		salida<<str.c_str();

	};
	void leer(std::istream&entrada){
		entrada>>str;
	};
	virtual ~AtributoVariable(){};

	Atributo& operator=(const Atributo& att)throw(std::bad_cast){
		AtributoVariable<string>& otro=dynamic_cast<AtributoVariable<string>&>(const_cast<Atributo&>(att));
		str=otro.str;
		return *this;
	};
	Ttamanio tamanio(){
		return str.size();
	};
	Atributo* clonar(){
		AtributoVariable<std::string>*clon=new AtributoVariable<std::string>(nombre);
		clon->str=str;
		return clon;
	};
	Ttamanio serializar(std::streambuf &salida)throw(IOSerializacionExcepcion){
		char tam=str.size();
		salida.sputc(tam);
		salida.sputn(str.data(),tam);
		return str.size()+1;
	};
	Ttamanio deserializar(std::streambuf &entrada)throw(IOSerializacionExcepcion){
		std::streambuf::int_type tam=entrada.sbumpc();
		if(tam==std::streambuf::traits_type::eof())
			throw IOSerializacionExcepcion("Excepcion:AtributoVariable "+nombre+" no fue deserializado, no se puede leer el tamanio");
		char *buf=new char[tam];
		entrada.sgetn(buf,tam);
		if(entrada.sgetn(buf,tam)!=tam)
			throw IOSerializacionExcepcion("Excepcion:AtributoVariable "+nombre+" no fue deserializado");
		str.clear();
		str.append(buf,tam);
		delete buf;
		return tam+1;
	};
	Ttamanio tamanioSerializado(){
		return str.size()+sizeof(char);
	};
};

/*----------------------------------------------------------------------------------------------------*/
/*Especializacion punteros*/
template<typename T_tipo>
class AtributoVariable<T_tipo*>{};
/*----------------------------------------------------------------------------------------------------*/
#endif /* AtributoVariable_H_ */






