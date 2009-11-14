/*
 * Set.h
 *
 *  Created on: 14/11/2009
 *      Author: paulo
 */

#ifndef SET_H_
#define SET_H_
#include <set>
#include <map>
#include <string>
#include "EstrategiaIndice.h"

class Set: public EstrategiaIndice {
	std::map<std::string,Referencia> indice;
	std::map<std::string,Referencia>::iterator it;
public:
	Set(){};
	void crear(std::string nombreArch,unsigned int tamanioBloque,Clave* clave,ComparadorClave* comp){};
	bool abrir(std::string nombreArch,ComparadorClave* comp){
		return true;
	};
	bool BuscarReferencia(const Clave* clave,Referencia* referencia){
		if(indice.empty())
			return false;
		AtributoFijo<char*>* att=(AtributoFijo<char*>*)const_cast<Clave*>(clave)->getAtributo(0);
		std::string nombre=*att;
		it=indice.find(nombre);
		if(it==indice.end())
			return false;
		*referencia=it->second;
		return true;
	};
	bool insertar(Referencia ref,Clave* clave){
		std::string nombre=*(AtributoFijo<char*>*)const_cast<Clave*>(clave)->getAtributo(0);
		return indice.insert(pair<std::string,Referencia>(nombre,ref)).second;
	};
    bool eliminar(const Clave* clave){
		if(indice.empty())
			return false;
		AtributoFijo<char*>* att=(AtributoFijo<char*>*)const_cast<Clave*>(clave)->getAtributo(0);
		std::string nombre=*att;
		it=indice.find(nombre);
		if(it==indice.end())
			return false;
		indice.erase(it);
		return true;
    };
	bool modificar(const Clave* clave,Referencia refNueva){
		if(indice.empty())
			return false;
		std::string nombre=*(AtributoFijo<char*>*)const_cast<Clave*>(clave)->getAtributo(0);
		it=indice.find(nombre);
		it->second=refNueva;
		return true;
	};
	void cerrar(){};
	int tamanioBloque(){
		return 0;
	};
	void mostrarEstado(){
		it=indice.begin();
		std::cout<<"indice"<<std::endl;
		while(it!=indice.end()){
			std::cout<<"clave "<< it->first.c_str() ;
			std::cout<<" ref  " << it->second <<std::endl;
			it++;
		}
	};
	virtual ~Set(){};
};

#endif /* SET_H_ */
