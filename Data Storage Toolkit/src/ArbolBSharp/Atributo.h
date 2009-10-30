/*
 * Atributo.h
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */

#ifndef ATRIBUTO_H_
#define ATRIBUTO_H_
#include <streambuf>
typedef unsigned int Ttamanio;
class Atributo {
public:
	Atributo(std::string nombreAtributo){nombre=nombreAtributo;};
	virtual ~Atributo(){};
	/*
	 *Devuelve un string con el nombre del atributo.
	 */
	std::string getNombre(){return nombre;};
	/*
	 * Setea el atributo con el valor pasado.
	 * Dicho valor debe tener tamanio() char's de longitud.
	 */
	virtual void set(void* valor)=0;
	/*
	 * Copia en char*valor el valor del atributo.
	 * Dicho valor debe tener tamanio() char's de longitud.
	 */
	virtual void get(void* valor)=0;
	/*
	 * Devuelve el tamanio en bytes del atributo.
	 */
	virtual Ttamanio tamanio()=0;
	/*
	 * Devuelve una copia instanciada mediante new del atributo.
	 * Debe aplicarsele delete al finalizar el uso del clon.
	 * Ej:
	 * Atributo* clon=atributo.clonar();
	 * ...
	 * delete clon;
	 *
	 */
	virtual Atributo* clonar()=0;
	/*
	 *
	 */
	virtual Ttamanio serializar(std::streambuf&salida)=0;
	/*
	 *
	 */
	virtual Ttamanio deserializar(std::streambuf&entrada)=0;
	/*
	 *
	 */
	virtual Ttamanio tamanioSerializado()=0;
	/*
	 *
	 *
	 */
	virtual bool esfijo()=0;
protected:
	std::string nombre;
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