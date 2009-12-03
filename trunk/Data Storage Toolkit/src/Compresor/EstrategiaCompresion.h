/*
 * EstrategiaCompresion.h
 *
 *  Created on: 03/12/2009
 *      Author: daniel
 */

#ifndef ESTRATEGIACOMPRESION_H_
#define ESTRATEGIACOMPRESION_H_
#include "../Almacenamiento/Almacenamiento.h"
#include "Compresor.h"
#include <fstream>
#include <string>
class EstrategiaCompresion {
public:
	EstrategiaCompresion();
	void compresion();
	void compresion(Almacenamiento*almacen,string archivoComprimido,unsigned tamanio);
	void compresionArbol();
	void compresionHash();
	virtual ~EstrategiaCompresion();
private:
	std::fstream archivoComprimidoArbol;
	std::fstream archivoComprimidoHash;
	std::fstream archivoComprimido;
};
#endif /* ESTRATEGIACOMPRESION_H_ */
