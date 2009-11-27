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
	/*A partir de codigo , busca los extemos para el codigo dado, y
	 *devuelve el simbolo que corresponde a los extremos
	 */
	tipo_simbolo obtenerSimbolo(const tipo_extremos &codigo,tipo_extremos&piso,tipo_extremos&techo);
	/*
	 * incremeta el numero de ocurrencias del simbolo pasado en 1
	 */
	void incremetarOcurrencias(const tipo_simbolo &simbolo);
	/*
	 * decremete el numero de ocurrencias del simbolo pasado en 1
	 */
	void decremetarOcurrencias(const tipo_simbolo &simbolo);

	virtual ~TablaDeProbabilidad();
};

#endif /* TABLADEPROBABILIDAD_H_ */
