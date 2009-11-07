/*
 * ElementoNodo.h
 *
 *  Created on: 02/10/2009
 *      Author: daniel
 */

#ifndef ELEMENTONODOHOJA_H_
#define ELEMENTONODOHOJA_H_
#include "Clave.h"


class ElementoNodo{
public:
	/*
	 * Si se encuentra en un nodo hoja es referencia al archivo, sino es la
	 * referencia derecha lo q contiene.
	 */
	ElementoNodo(Referencia ref,Clave* cla);
	ElementoNodo();
	virtual ~ElementoNodo();
	Referencia getReferencia();
	void setReferencia(Referencia ref);
	Clave* getClave();
	ElementoNodo* clonarce();
	void setClave(Clave* cla);
private:
	    Clave* clave;
	    Referencia referencia;
protected:

};
#endif /* ELEMENTONODOHOJA_H_ */
