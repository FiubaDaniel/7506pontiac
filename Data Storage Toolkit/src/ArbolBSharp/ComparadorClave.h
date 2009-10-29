/*
 * ComparadorClave.h
 *
 *  Created on: 12/10/2009
 *      Author: daniel
 */

/*
 * Clase abastracta que le permite al usuario poder comparar las claves
 * que forman su arbol de la forma q le convenga o requiera.
 */
#ifndef COMPARADORCLAVE_H_
#define COMPARADORCLAVE_H_
#include "Clave.h"

class ComparadorClave {
public:
	ComparadorClave(){};
	/*
	 * La funcion mostrar devuelve mayor, igual o menos a 0 si la clave pasada
	 * como primer parametro es mayor, igual o menor a la clave pasada como segundo
	 * parametro.
	 *
	 */
	virtual int Comparar(Clave* clave1,Clave* clave2) const = 0;
	virtual ~ComparadorClave(){};
};
#endif /* COMPARADORCLAVE_H_ */
