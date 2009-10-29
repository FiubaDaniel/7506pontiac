/*
 * ElementoNodo.h
 *
 *  Created on: 02/10/2009
 *      Author: daniel
 */

#ifndef ELEMENTONODOHOJA_H_
#define ELEMENTONODOHOJA_H_
#include "Clave.h"

class ElementoNodo {
public:
	ElementoNodo(Referencia referencia,Clave* cla);
	ElementoNodo();
	virtual ~ElementoNodo();
	Referencia getReferencia();
	void setReferencia(Referencia ref);
	Clave* getClave();
	ElementoNodo* clonarce();
	Referencia bucarReferenciaAsiguienteNodo(Clave* clave);
private:
	    Clave* clave;
	    Referencia referenciaAlArchivo;
	    void setClave(Clave* cla);
};
#endif /* ELEMENTONODOHOJA_H_ */
