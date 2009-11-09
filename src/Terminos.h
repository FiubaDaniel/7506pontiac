#ifndef TERMINOS_H_
#define TERMINOS_H_

#include <iostream>

class Terminos
{
private:
	static std::string terminos[300];

public:

	static std::string obtenerTermino(int posicion);
	
};

#endif /*TERMINOS_H_*/
