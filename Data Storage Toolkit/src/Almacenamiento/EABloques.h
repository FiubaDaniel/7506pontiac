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
	Almacenamiento* almacen;
	size_t corrienteBloque;
	Ttamanio libres;
	Ttamanio capacBloque;
	char* bloqueSerializado;
	double porcCarga;
	Bloque* bloque;
	std::fstream archivoEspacioLibre;
	bool buscarEspacioLibre(Ttamanio espacio,size_t & nroBloque);
	bool buscarComponente(Componente*comp,Ttamanio&posicion);
	void finalizarAlamcenamiento();
public:
	EABloques(Bloque* tipoBloque,Ttamanio tamanioBloque);
	virtual ~EABloques();
	Almacenamiento* abrir(Almacenamiento*almacen,const char*rutaArchivoEspacios);
	Almacenamiento* nuevo(Almacenamiento*almacen,const char*rutaArchivoEspacios);
	void posicionarComponente(size_t nroCompuesto);
	void escribir(Componente*componente);
	void leer(Componente*componente);
	size_t insertar(Componente*componente);
	bool modificar(Componente*componente);
	bool eliminar(Componente*componente);
	bool buscar(Componente*componente);
};

#endif /* EABLOQUES_H_ */
