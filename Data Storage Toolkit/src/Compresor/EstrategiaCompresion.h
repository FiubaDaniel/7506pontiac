/*
 * EstrategiaCompresion.h
 *
 *  Created on: 03/12/2009
 *      Author: daniel
 */

#ifndef ESTRATEGIACOMPRESION_H_
#define ESTRATEGIACOMPRESION_H_

class EstrategiaCompresion {
public:
	EstrategiaCompresion();
	void compresion();
	void compresionArbol();
	void compresionHash();
	virtual ~EstrategiaCompresion();
private:
	std::fstream archivoComprimidoArbol;
	std::fstream archivoComprimidoHash;
	std::fstream archivoComprimido;
};
#endif /* ESTRATEGIACOMPRESION_H_ */
