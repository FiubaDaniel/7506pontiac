/*
 * EABloques.h
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#ifndef EABLOQUES_H_
#define EABLOQUES_H_
#include "EstrategiaAlmacenamiento.h"
#include "../Compuesto/Bloque.h"
#include <sstream>
#include <fstream>
class EABloques : public EstrategiaAlmacenamiento{
private:
	size_t corrienteBloque;
	Ttamanio libres;
	Ttamanio capacBloque;
	char* bloqueSerializado;
	double porcCarga;
	Bloque* bloque;
	std::fstream archivoEspacioLibre;
	bool buscarEspacioLibre(Ttamanio espacio,size_t & nroBloque);
	bool buscarComponente(Componente*comp,Ttamanio&posicion);
public:
	EABloques(const char*rutaArchivoEspacios,Bloque* tipoBloque,Ttamanio tamanioBloque);
	virtual ~EABloques();
	void posicionarComponente(Almacenamiento* almacen,size_t nroCompuesto);
	void escribir(Almacenamiento* almacen,Componente*componente);
	void leer(Almacenamiento* almacen,Componente*componente);
	size_t insertar(Almacenamiento* almacen,Componente*componente);
	bool modificar(Almacenamiento*almacen,Componente*componente);
	void eliminar(Almacenamiento*almacen,Componente*componente);
	bool buscar(Almacenamiento*almacen,Componente*componente);
};

#endif /* EABLOQUES_H_ */
