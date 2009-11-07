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
	AtributoVariable(std::string nombreAtributo);//crea espacio para almenos un valor
	virtual ~AtributoVariable();
	void set(void* value);//modifican el valor apuntado
	void get(void* value);
	Ttamanio tamanio();
	Atributo* clonar();
	Ttamanio serializar(std::streambuf &salida)throw(ErrorSerializacionExcepcion);
	Ttamanio deserializar(std::streambuf &entrada)throw(ErrorSerializacionExcepcion);
	Ttamanio tamanioSerializado();
	bool esfijo();
	int comparar(const Atributo*otroAtributo);
	void imprimir(std::ostream&salida);
	void leer(std::istream&entrada);
	void copiar(const Atributo* att);
public:
	/* metodo propios de la clase permiten iteracion sobre los valores guardados*/
	void append(void*valor);// se agregan tantos bytes como indica nrobytes
	void apuntar(Ttamanio nroValor);
	void eliminarApuntado();
	Ttamanio cantidadValores();
};
/*----------------------------------------------------------------------------*/
/*Template Control de tipo*/
template<typename T_tipo>
AtributoVariable<T_tipo>::AtributoVariable(std::string nombreAtributo):AtributoVariable<T_tipo>::Atributo(nombreAtributo){
	T_tipo aux;
	valores.push_back(aux);
	valorActual=0;
};
/*----------------------------------------------------------------------------*/
/*Templates sin especializacion*/
template<typename T_tipo>
AtributoVariable<T_tipo>::~AtributoVariable(){};
template<typename T_tipo>
void AtributoVariable<T_tipo>::set(void* value){
	valores.at(valorActual)=*(T_tipo*)value;
};
template<typename T_tipo>
void AtributoVariable<T_tipo>::get(void* value){
	*(T_tipo*)value=valores.at(valorActual);
};
template<typename T_tipo>
Ttamanio AtributoVariable<T_tipo>::tamanio(){return sizeof(T_tipo)*valores.size();};
template<typename T_tipo>
Atributo* AtributoVariable<T_tipo>::clonar(){
	AtributoVariable<T_tipo> *clon=new AtributoVariable<T_tipo>(nombre);
	for(Ttamanio i= valores.size()-1;i>=0;i--){
		clon->valores.push_back(valores.at(i));
	}
	return clon;
};
template<typename T_tipo>
Ttamanio AtributoVariable<T_tipo>::serializar(std::streambuf &salida)throw(ErrorSerializacionExcepcion){
	Ttamanio offset=sizeof(Ttamanio);
	Ttamanio aux=valores.size();
	salida.sputn((char*)&aux,offset);
	for(Ttamanio i=0;i<valores.size();i++){
		salida.sputn((char*)&valores.at(i),sizeof(T_tipo));
		offset+=sizeof(T_tipo);
	}
	return offset;
};
template<typename T_tipo>
Ttamanio AtributoVariable<T_tipo>::deserializar(std::streambuf &entrada)throw(ErrorSerializacionExcepcion){
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
template<typename T_tipo>
Ttamanio AtributoVariable<T_tipo>::tamanioSerializado(){
	return valores.size()*sizeof(T_tipo)+sizeof(Ttamanio);
};
template<typename T_tipo>
bool AtributoVariable<T_tipo>::esfijo(){
	return false;
};
template<typename T_tipo>
void AtributoVariable<T_tipo>::append(void*valor){
	T_tipo aux=*(T_tipo*)valor;
	valores.push_back(aux);
};
template<typename T_tipo>
void AtributoVariable<T_tipo>::apuntar(Ttamanio nroValor){valorActual=nroValor;};
template<typename T_tipo>
void AtributoVariable<T_tipo>::eliminarApuntado(){valores.erase(valores.begin()+valorActual);};
template<typename T_tipo>
Ttamanio AtributoVariable<T_tipo>::cantidadValores(){return valores.size();};
template<typename T_tipo>
int AtributoVariable<T_tipo>::comparar(const Atributo*otroAtributo){
	AtributoVariable<T_tipo>* otro=dynamic_cast<AtributoVariable<T_tipo>*>(const_cast<Atributo*>(otroAtributo));
	if(otro!=NULL){
		return this->valores.at(valorActual)-otro->valores.at(otro->valorActual);
	}//TODO exception
	return 0;
};
template<typename T_tipo>
void AtributoVariable<T_tipo>::imprimir(std::ostream&salida){
	for(Ttamanio i=0;i<valores.size();i++){
			salida<<valores.at(i)<<" ";
	}
};
template<typename T_tipo>
void AtributoVariable<T_tipo>::leer(std::istream&entrada){
	for(Ttamanio i=0;i<valores.size();i++){
				entrada>>valores.at(i);
	}
};
template<typename T_tipo>
void AtributoVariable<T_tipo>::copiar(const Atributo* att){
		AtributoVariable<T_tipo>* otro=dynamic_cast<AtributoVariable<T_tipo>*>(const_cast<Atributo*>(att));
		this->valores.clear();
		this->valores.assign(otro->valores.begin(),otro->valores.end());
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






