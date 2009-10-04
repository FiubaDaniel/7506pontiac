/*
 * Bloque.h
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */

#ifndef BLOQUE_H_
#define BLOQUE_H_
#include <vector>
#include "Componente.h"

typedef unsigned int Ttamanio;

class Bloque: public Componente {
public:
	Bloque(Ttamanio tamanio,Componente*compenente);
	virtual ~Bloque();
	//TODO ver si sirve o no
	Componente* clonar();
	void setbytes(std::streambuf& pbuffer);
	void getbytes(std::streambuf& pbuffer);
	Ttamanio cantidadBytes();
public:
	Componente* get();
	bool apuntar(Ttamanio nroComponente);
	void eliminar();
	Componente* modificar(Componente* nuevo);
	bool insertar(Componente* nuevo);//intenta insertar detras del registro actual si puede
	bool append(Componente* nuevo);//intenta insdertar detras del ultimo registro
	Ttamanio cantidadComponente();
private:
	std::vector<Componente*> componentes;
	Ttamanio corriente;
	Ttamanio tamanioBloque;
	Ttamanio espacioLibre;
};

#endif /* BLOQUE_H_ */
