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
#include <typeinfo>
#include <exception>
typedef unsigned int Ttamanio;

class IOSerializacionExcepcion : public std::exception {
	std::string mensaje;
public:
	IOSerializacionExcepcion(std::string detalle) throw(){
		mensaje=detalle;
	};
	const char* what() const throw(){ return mensaje.c_str();};
	~IOSerializacionExcepcion() throw(){};
};

class Atributo {
public:
	Atributo(std::string nombreAtributo){nombre=nombreAtributo;};
	virtual ~Atributo(){};
	/* Asignacion los datos al atributo a partir de otro atributo.
	 * Arroja una Excepcion std::bad_cast en caso de que los atributos no sean del mismo tipo.
	 */
	virtual Atributo& operator=(const Atributo& att)throw(std::bad_cast) =0;
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
	 * Devuelve una copia alocada mediante new del atributo.
	 * Debe aplicarsele delete al finalizar el uso del clon.
	 * Ej:
	 * Atributo* clon=atributo.clonar();
	 * ...
	 * delete clon;
	 *
	 */
	virtual Atributo* clonar()=0;
	/*
	 * Escribe los bytes forman el dato del atributo en salida.
	 * devuelve la cantidad de bytes escritos en salida.
	 */
	virtual Ttamanio serializar(std::streambuf&salida) throw(IOSerializacionExcepcion)=0;
	/* Carga el atributo a partir de los bytes leido de entrada.
	 * si la entrada se agota antes de que se hayan leidos los bytes necesario,
	 * arroja una excepcion  ErrorSerializacionExcepcion
	 * @return la cantidad de bytes leidos.
	 */
	virtual Ttamanio deserializar(std::streambuf&entrada) throw(IOSerializacionExcepcion) =0;
	/*
	 * @return devuelve la cantidad de bytes que conforma la forma serializada del dato
	 * contenido en atributo.
	 */
	virtual Ttamanio tamanioSerializado()=0;
	/* Compara con otroAtributo.
	 * @return : >0  si el atributo es mayor que otroAtributo
	 * 			 ==0  si el atributo es igual que otroAtributo
	 * 			 <0  si el atributo es menor que otroAtributo
	 *
	 * la implementacion puede variar.
	 * Arroja una excepcion: std::bad_cast si los atributos no son del mismo tipo.
	 */
	virtual int comparar(const Atributo*otroAtributo)throw(std::bad_cast) =0;
	/*
	 *imprimi a salida el atributo en formato texto
	 */
	virtual void imprimir(std::ostream&salida)=0;
	/*
	 * lee de entrada el atributo en formato texto.
	 */
	virtual void leer(std::istream&entrada)=0;
protected:
	std::string nombre;
};

/*----------------------------------------------------------------------------*/
#endif /* ATRIBUTO_H_ */
