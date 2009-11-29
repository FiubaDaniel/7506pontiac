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
	char simbolo;
	tipo_frecuencia frecuencia;
};

typedef std::list<ElementoContexto> tipo_tabla_frecuencias;

struct Contexto{
	tipo_tabla_frecuencias tablaFrecuencias;
	tipo_frecuencia totalFrecuencias;
};

typedef std::map<char,Contexto> tipo_contextos;

class TablaDeProbabilidad{
	tipo_contextos contextos;
public:
	TablaDeProbabilidad();
	/*A partir de simbolo, calcula techo y piso, copiando el resultado a techo y piso*/
	void obtenerExtremos(char contexto,char simbolo,unsigned & piso,unsigned &techo);
	void incremtarOcurrencia(char contexto,char simbolo);
	void decremetarOcurrencia(char contexto,char simbolo);
	void agregarContexto(char contexto,char simbolo);
	void ageragarElementoContexto(Contexto& contextoModificar,char simbolo);
	virtual ~TablaDeProbabilidad();
};

#endif /* TABLADEPROBABILIDAD_H_ */
