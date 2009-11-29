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

typedef std::list<ElementoContexto> tipo_tabla_frecuencias;

class TablaDeProbabilidad{
	std::map<char,tipo_tabla_frecuencias> contextos;
public:
	TablaDeProbabilidad();
	/*A partir de simbolo, calcula techo y piso, copiando el resultado a techo y piso*/
	void obtenerExtremos(char contexto,char simbolo,unsigned & piso,unsigned &techo);


	virtual ~TablaDeProbabilidad();
};

#endif /* TABLADEPROBABILIDAD_H_ */
