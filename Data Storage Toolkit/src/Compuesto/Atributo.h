/*
 * Atributo.h
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */

#ifndef ATRIBUTO_H_
#define ATRIBUTO_H_
#include <streambuf>
#include <iostream>
#include <exception>
typedef unsigned int Ttamanio;

class ErrorSerializacionExcepcion : public std::exception {
	std::string mensaje;
public:
	ErrorSerializacionExcepcion(std::string detalle) throw(){
		mensaje=detalle;
	};
	const char* what() const throw(){ return mensaje.c_str();};
	~ErrorSerializacionExcepcion() throw(){};
};

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
	virtual Ttamanio serializar(std::streambuf&salida) throw(ErrorSerializacionExcepcion)=0;
	/*
	 *
	 */
	virtual Ttamanio deserializar(std::streambuf&entrada) throw(ErrorSerializacionExcepcion) =0;
	/*
	 *
	 */
	virtual Ttamanio tamanioSerializado()=0;
	/*
	 *
	 *
	 */
	virtual int comparar(const Atributo*otroAtributo)=0;
	virtual void imprimir(std::ostream&salida)=0;
	virtual void leer(std::istream&entrada)=0;
	virtual Atributo& operator=(const Atributo& att)=0;
protected:
	std::string nombre;
/*	*/
};

/*----------------------------------------------------------------------------*/
#endif /* ATRIBUTO_H_ */
