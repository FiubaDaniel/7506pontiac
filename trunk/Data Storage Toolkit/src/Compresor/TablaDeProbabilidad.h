/*
 * TablaDeProbabilidad.h
 *
 *  Created on: 27/11/2009
 *      Author: paulo
 */

#ifndef TABLADEPROBABILIDAD_H_
#define TABLADEPROBABILIDAD_H_

typedef char tipo_simbolo;
typedef unsigned tipo_extremos;

class TablaDeProbabilidad{
public:
	TablaDeProbabilidad();
	/*A partir de simbolo, calcula techo y piso, copiando el resultado a techo y piso*/
	void obtenerExtremos(const tipo_simbolo & simbolo,tipo_extremos&piso,tipo_extremos&techo);
	/**/
	tipo_simbolo obtenerSimbolo(const tipo_extremos codigo,tipo_extremos&piso,tipo_extremos&techo);


	virtual ~TablaDeProbabilidad();
};

#endif /* TABLADEPROBABILIDAD_H_ */
