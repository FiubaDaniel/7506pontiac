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


typedef float tipo_frecuencia;

struct ElementoContexto{
	char simbolo;
	tipo_frecuencia frecuencia;
};

struct Contexto{
	tipo_Contexto Map_Contextos;
	tipo_frecuencia totalFrecuencias;
};

typedef std::list<ElementoContexto> tipo_tabla_frecuencias;
typedef std::map<char,tipo_tabla_frecuencias> tipo_contextos;

class TablaDeProbabilidad{
	Contexto contextos;
public:
	TablaDeProbabilidad();
	/*A partir de simbolo, calcula techo y piso, copiando el resultado a techo y piso*/
	void obtenerExtremos(char contexto,char simbolo,unsigned & piso,unsigned &techo);
	void incremtarOcurrencia(char contexto,char simbolo);
	void decremetarOcurrencia(char contexto,char simbolo);
	virtual ~TablaDeProbabilidad();
};

#endif /* TABLADEPROBABILIDAD_H_ */
