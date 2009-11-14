/*
 * TamanioInsuficienteException.h
 *
 *  Created on: 27/10/2009
 *      Author: daniel
 */

#ifndef TAMANIOINSUFICIENTEEXCEPTION_H_
#define TAMANIOINSUFICIENTEEXCEPTION_H_
#include <exception>
#include <string>
class TamanioInsuficienteException : public std::exception {
	std::string mensaje;
public:
	TamanioInsuficienteException()throw();
	virtual ~TamanioInsuficienteException()throw();
	const char* what() const throw();
};

#endif /* TAMANIOINSUFICIENTEEXCEPTION_H_ */
