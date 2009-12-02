/*
 * TablaPPMC.h
 *
 *  Created on: 02/12/2009
 *      Author: daniel
 */

#ifndef TABLAPPMC_H_
#define TABLAPPMC_H_
#include <map>
#include <list>
#include <cmath>
#include <iostream>

typedef float tipo_frecuencia;

struct ElementoContexto{
	unsigned char simbolo;
	tipo_frecuencia frecuencia;
};

typedef std::list<ElementoContexto> tipo_tabla_frecuencias;

struct Contexto{
	tipo_tabla_frecuencias tablaFrecuencias;
	tipo_frecuencia frecuencias_simbolos;
	tipo_frecuencia frecuencia_escape;
};

typedef std::map<unsigned char,Contexto> tipo_contextos;

class TablaPPMC {
	tipo_contextos contextos;
public:

	TablaPPMC();
	/*A partir de simbolo, calcula techo y piso, copiando el resultado a techo y piso*/
	void obtenerExtremos(unsigned char contexto,unsigned char simbolo,unsigned & piso,unsigned &techo);
	void incremtarOcurrencia(unsigned char contexto,unsigned char simbolo);
	void decremetarOcurrencia(unsigned char contexto,unsigned char simbolo);
	unsigned char calcularEmision(unsigned &piso,unsigned &techo,unsigned codigo,unsigned char anterior);
	float buscarOcurrencias(unsigned char anterior,unsigned char buscado);
	void imprimir(unsigned char contexto,unsigned char simbolo);
	bool vacia();
	virtual ~TablaPPMC();
private:
	float obtenerTotalContexto(unsigned char simbolo);
	void agregarContexto(unsigned char contexto,unsigned char simbolo);
	void ageragarElementoContexto(Contexto& contextoModificar,unsigned char simbolo);
};

#endif /* TABLAPPMC_H_ */
