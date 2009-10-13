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

class Bloque :public Componente, public Compuesto {
public:
	Bloque(Componente*compenente);
	virtual ~Bloque();
	void inicializar(Componente*componente);
public:/*Heredados de Componente*/
	virtual Ttamanio deserializar(std::streambuf&entrada);
	virtual Ttamanio serializar(std::streambuf&salida);
	virtual Ttamanio tamanioSerializado();
	virtual Componente* clonar();
	virtual bool esfijo();//TODO
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
};
#endif /* BLOQUE_H_ */
