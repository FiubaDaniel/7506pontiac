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
	Ttamanio valorActual;
public:
	AtributoVariable(std::string nombreAtributo):AtributoVariable<T_tipo>::Atributo(nombreAtributo){
		T_tipo aux;
		valores.push_back(aux);
		valorActual=0;
	};
	~AtributoVariable(){};
	void set(void* value){
		valores.at(valorActual)=*(T_tipo*)value;
	};

	void get(void* value){
		*(T_tipo*)value=valores.at(valorActual);
	};

	Ttamanio tamanio(){return sizeof(T_tipo)*valores.size();};

	Atributo* clonar(){
		AtributoVariable<T_tipo> *clon=new AtributoVariable<T_tipo>(nombre);
		for(Ttamanio i= valores.size()-1;i>=0;i--){
			clon->valores.push_back(valores.at(i));
		}
		return clon;
	};

	Ttamanio serializar(std::streambuf &salida)throw(ErrorSerializacionExcepcion){
		Ttamanio offset=sizeof(Ttamanio);
		Ttamanio aux=valores.size();
		salida.sputn((char*)&aux,offset);
		for(Ttamanio i=0;i<valores.size();i++){
			salida.sputn((char*)&valores.at(i),sizeof(T_tipo));
			offset+=sizeof(T_tipo);
		}
		return offset;
	};

	Ttamanio deserializar(std::streambuf &entrada)throw(ErrorSerializacionExcepcion){
		Ttamanio offset=sizeof(Ttamanio);
		Ttamanio nroValores=valores.size();
		if(entrada.sgetn((char*)&nroValores,offset)!=offset)
			throw ErrorSerializacionExcepcion("Excepcion:AtributoVariable "+nombre+" no fue deserializado");
		Ttamanio i=0;
		T_tipo aux;
		while(i< nroValores and i<valores.size() ){
			if( entrada.sgetn((char*)&aux,sizeof(T_tipo))!=sizeof(T_tipo))
				ErrorSerializacionExcepcion("Excepcion:AtributoVariable "+nombre+" no fue deserializado");;//TODO
			valores.at(i)=aux;
			offset+=sizeof(T_tipo);
			i++;
		}
		while(i < valores.size()){
			valores.erase(valores.begin()+i);
		}
		while(i<nroValores){
			if(entrada.sgetn((char*)&aux,sizeof(T_tipo))!=sizeof(T_tipo))
				throw ErrorSerializacionExcepcion("Excepcion:AtributoVariable "+nombre+" no fue deserializado");;//TODO
			valores.at(i)=aux;
			offset+=sizeof(T_tipo);
			i++;
		}
		return offset;
	};

	Ttamanio tamanioSerializado(){
		return valores.size()*sizeof(T_tipo)+sizeof(Ttamanio);
	};

	bool esfijo(){
		return false;
	};
public:
	void append(void*valor){
		T_tipo aux=*(T_tipo*)valor;
		valores.push_back(aux);
	};

	void apuntar(Ttamanio nroValor){valorActual=nroValor;};

	void eliminarApuntado(){valores.erase(valores.begin()+valorActual);};

	Ttamanio cantidadValores(){return valores.size();};

	int comparar(const Atributo*otroAtributo){
		AtributoVariable<T_tipo>* otro=dynamic_cast<AtributoVariable<T_tipo>*>(const_cast<Atributo*>(otroAtributo));
		if(otro!=NULL){
			return this->valores.at(valorActual)-otro->valores.at(otro->valorActual);
		}//TODO exception
		return 0;
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

	void copiar(const Atributo* att){
			AtributoVariable<T_tipo>* otro=dynamic_cast<AtributoVariable<T_tipo>*>(const_cast<Atributo*>(att));
			this->valores.clear();
			this->valores.assign(otro->valores.begin(),otro->valores.end());
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
	AtributoVariable(std::string nombreAtributo):Atributo(nombreAtributo){};
	virtual void set(void* value){
		std::string* aux=(std::string*)value;
		str=*aux;
	};
	virtual void get(void* value){
		std::string* aux=(std::string*)value;
		*aux=str;
	};
	Ttamanio cantidadbytes(){return str.size();};
	bool esfijo(){
		return false;
	};
	int comparar(const Atributo*otroAtributo){
		AtributoVariable<std::string>* otro=dynamic_cast<AtributoVariable<std::string>*>(const_cast<Atributo*>(otroAtributo));
		return str.compare(otro->str);
	};
	void imprimir(std::ostream&salida){
		salida<<str.c_str();

	};
	void leer(std::istream&entrada){
		entrada>>str;
	};
	virtual ~AtributoVariable(){};

	void copiar(const Atributo* att){
			AtributoVariable<string>* otro=dynamic_cast<AtributoVariable<string>*>(const_cast<Atributo*>(att));
			str=otro->str;
	};
	Ttamanio tamanio(){
		return str.size();
	};
	Atributo* clonar(){
		AtributoVariable<std::string>*clon=new AtributoVariable<std::string>(nombre);
		clon->str=str;
		return clon;
	};
	Ttamanio serializar(std::streambuf &salida)throw(ErrorSerializacionExcepcion){
		char tam=str.size();
		salida.sputc(tam);
		salida.sputn(str.data(),tam);
		return str.size()+1;
	};
	Ttamanio deserializar(std::streambuf &entrada)throw(ErrorSerializacionExcepcion){
		std::streambuf::int_type tam=entrada.sbumpc();
		if(tam==std::streambuf::traits_type::eof())
			throw ErrorSerializacionExcepcion("Excepcion:AtributoVariable "+nombre+" no fue deserializado, no se puede leer el tamanio");;//TODO
		char *buf=new char[tam];
		if(entrada.sgetn(buf,tam)!=tam)
			throw ErrorSerializacionExcepcion("Excepcion:AtributoVariable "+nombre+" no fue deserializado");;//TODO
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






