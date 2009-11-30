/*
 * TablaDeProbabilidad.h
 *
 *  Created on: 27/11/2009
 *      Author: paulo
 */

#ifndef TABLADEPROBABILIDAD_H_
#define TABLADEPROBABILIDAD_H_
#include <map>
#include <list>
#include <cmath>

typedef float tipo_frecuencia;

struct ElementoContexto{
	unsigned char simbolo;
	tipo_frecuencia frecuencia;
};

typedef std::list<ElementoContexto> tipo_tabla_frecuencias;

struct Contexto{
	tipo_tabla_frecuencias tablaFrecuencias;
	tipo_frecuencia totalFrecuencias;
};

typedef std::map<unsigned char,Contexto> tipo_contextos;

class TablaDeProbabilidad{
	tipo_contextos contextos;
public:
	TablaDeProbabilidad();
	/*A partir de simbolo, calcula techo y piso, copiando el resultado a techo y piso*/
	void obtenerExtremos(char contexto,char simbolo,unsigned & piso,unsigned &techo);
	void incremtarOcurrencia(unsigned char contexto,unsigned char simbolo);
	void decremetarOcurrencia(unsigned char contexto,unsigned char simbolo);
	void agregarContexto(unsigned char contexto,unsigned char simbolo);
	void ageragarElementoContexto(Contexto& contextoModificar,unsigned char simbolo);
	unsigned char calcularEmision(unsigned &piso,unsigned &techo,unsigned codigo,unsigned char anterior);
	float buscarOcurrencias(unsigned char anterior,unsigned char buscado);
	bool vacia();
	virtual ~TablaDeProbabilidad();
private:
	float obtenerTotalContexto(unsigned char simbolo);
};

#endif /* TABLADEPROBABILIDAD_H_ */
