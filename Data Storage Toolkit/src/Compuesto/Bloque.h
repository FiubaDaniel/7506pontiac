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
#include "Compuesto.h"

class Bloque :public Componente, Compuesto {
public:
	Bloque(Ttamanio tamanio,Componente*compenente);
	virtual ~Bloque();
public:/*Heredados de Componente*/
	virtual Ttamanio deserializar(std::istream&entrada);
	virtual Ttamanio serializar(std::ostream&salida);
	virtual Ttamanio tamanioSerializado();
	virtual Componente* clonar();
public:/*Heredados de compuesto*/
	virtual Componente* get(Ttamanio nroComponente);
	virtual Ttamanio cantidadComponentes();
	virtual bool insertar(Componente*componente,Ttamanio posicion);
	virtual bool eliminar(Ttamanio posicion);
	virtual bool agregar(Componente*componente);
	virtual Componente* reemplazar(Componente*componente,Ttamanio posicion);
protected:
	Bloque();
	std::vector<Componente*> componentes;
	Ttamanio tamanioBloque;
	Ttamanio espacioLibre;
};
#endif /* BLOQUE_H_ */
