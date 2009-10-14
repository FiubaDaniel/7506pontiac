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
	Almacenamiento* almacen;
	size_t nroRegistro;
	Ttamanio tamanioRegistro;
	Ttamanio tamanioEncabezado;
	char* registroSerializado;
	size_t siguienteLibre;
	void finalizarAlmacenamiento();
public:
	EARegistros();
	virtual ~EARegistros();
	Almacenamiento* abrir(Almacenamiento*almacen);
	Almacenamiento* nuevo(Almacenamiento*almacen,Ttamanio tamRegistro);
	void posicionarComponente(size_t nroCompuesto);
	void escribir(Componente*componente);
	void leer(Componente*componente);
	size_t insertar(Componente*componente);
	bool modificar(Componente*componente);
	bool eliminar(Componente*componente);
	bool buscar(Componente*componente);
};

#endif /* EAREGISTROS_H_ */
