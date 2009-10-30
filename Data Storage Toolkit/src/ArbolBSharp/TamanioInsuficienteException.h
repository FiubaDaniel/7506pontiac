/*
 * TamanioInsuficienteException.h
 *
 *  Created on: 27/10/2009
 *      Author: daniel
 */

#ifndef TAMANIOINSUFICIENTEEXCEPTION_H_
#define TAMANIOINSUFICIENTEEXCEPTION_H_
#include <exception>
#include <string.h>
class TamanioInsuficienteException {
public:
	TamanioInsuficienteException();
	virtual ~TamanioInsuficienteException();
	const char* what() const throw();
};

#endif /* TAMANIOINSUFICIENTEEXCEPTION_H_ */
