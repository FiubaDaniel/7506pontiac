/*
 * Atributo.h
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */

#ifndef ATRIBUTO_H_
#define ATRIBUTO_H_
#include <string>
#include <sstream>
#include <iostream>
typedef unsigned int Ttamanio;
class Atributo {
public:
	Ttamanio nrobytes;/*indica cuantos bytes usar en las operaciones*/
	Atributo(){};
	Atributo(std::string nombreAtributo){
		_nombre=nombreAtributo;
	};
	virtual ~Atributo(){};
	std::string nombre(){return _nombre;};
	virtual void imprimir(std::ostream salida)=0;
	virtual void leer(std::istream entrada)=0;
	virtual void setbytes(char* value)=0;
	virtual void getbytes(char* value)=0;
	virtual void setbytes(std::streambuf& pbuffer)=0;
	virtual void getbytes(std::streambuf& pbuffer)=0;
	virtual Atributo* clonar()=0;
	virtual Ttamanio cantidadbytes()=0;
protected:
	std::string _nombre;
	void cpy(char*dest,const char*origen,Ttamanio n){
		while(n>0){
			*dest=*origen;
			dest++;
			origen++;
			n--;
		}
	};
};

/*----------------------------------------------------------------------------*/
#endif /* ATRIBUTO_H_ */
