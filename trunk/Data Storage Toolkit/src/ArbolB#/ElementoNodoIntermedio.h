/*
 * ElementoNodoIntermedio.h
 *
 *  Created on: 08/10/2009
 *      Author: daniel
 */

#ifndef ELEMENTONODOINTERMEDIO_H_
#define ELEMENTONODOINTERMEDIO_H_
#include "Clave.h"
class ElementoNodoIntermedio {
private:
	    Referencia referenciaDer;
	    Clave* clave;
	    ElementoNodoIntermedio();
public:
	void setClave(Clave* cla);
	ElementoNodoIntermedio(Referencia refDer,Clave* cla);
	Referencia getReferenciaDer();
	Clave* getClave();
	void setReferenciaDer(Referencia ref);
	ElementoNodoIntermedio* clonarce();
	virtual ~ElementoNodoIntermedio();
};
#endif /* ELEMENTONODOINTERMEDIO_H_ */
