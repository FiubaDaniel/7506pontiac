/*
 * EARegistros.h
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#ifndef EAREGISTROS_H_
#define EAREGISTROS_H_
#include <sstream>
#include "EstrategiaAlmacenamiento.h"
#include "../Compuesto/Registro.h"

class EARegistros: public EstrategiaAlmacenamiento {

	size_t nroRegistro;
	Ttamanio tamanioRegistro;
	Ttamanio tamanioEncabezado;
	char* registroSerializado;
	size_t siguienteLibre;
	Almacenamiento* almacen;
	void finalizarAlmacenamiento();
	int comparar(Registro*reg1,Registro*reg2){return 0;}
public:

	EARegistros();
	virtual ~EARegistros();
	Almacenamiento* abrir(Almacenamiento*almacen);
	Almacenamiento* nuevo(Almacenamiento*almacenamiento,Ttamanio tamRegistro);
	void posicionarComponente(size_t nroCompuesto);
	bool escribir(Componente*componente);
	bool leer(Componente*componente);
	size_t insertar(Componente*componente);
	bool modificar(Componente*componente);
	bool eliminar(Componente*componente);
	bool buscar(Componente*componente);
};

#endif /* EAREGISTROS_H_ */
