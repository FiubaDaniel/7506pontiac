/*
 * ElementoNodoHoja.h
 *
 *  Created on: 02/10/2009
 *      Author: daniel
 */

#ifndef ELEMENTONODOHOJA_H_
#define ELEMENTONODOHOJA_H_
#include "Clave.h"

class ElementoNodoHoja {
public:
	ElementoNodoHoja(Referencia referencia,Clave* cla);
	ElementoNodoHoja();
	virtual ~ElementoNodoHoja();
	Referencia getReferencia();
	void setReferencia(Referencia ref);
	Clave* getClave();
	ElementoNodoHoja* clonarce();
	Referencia bucarReferenciaAsiguienteNodo(Clave* clave);
private:
	    Clave* clave;
	    Referencia referenciaAlArchivo;
	    void setClave(Clave* cla);
};
#endif /* ELEMENTONODOHOJA_H_ */
