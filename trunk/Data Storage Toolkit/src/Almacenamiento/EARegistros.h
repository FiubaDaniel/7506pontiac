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
#include "../ArbolBSharp/Clave.h"
#include "../ArbolBSharp/ComparadorClave.h"
#include "../Compuesto/Registro.h"

class EARegistros: public EstrategiaAlmacenamiento {
	Registro*registro;
	size_t nroRegistro;
	size_t siguienteRegLibre;
	Ttamanio tamanioRegistro;
	Ttamanio tamanioEncabezado;
	char* registroSerializado;
	Almacenamiento* almacen;
	void finalizarAlmacenamiento();
	int comparar(Registro*reg1,Registro*reg2);
	/*escribe el registro en la posicion.*/
	void escribir(Registro*registro);
	void leer(Registro*registro);
public:
	EARegistros(Registro*registro);
	EARegistros(){};
	virtual ~EARegistros();
	bool abrir(Almacenamiento*almacen);
	bool crear(Almacenamiento*almacenamiento);
	void cerrar();
	Componente* getRegistro();
	bool posicionarComponente(size_t nroCompuesto);
	bool escribir(Componente*componente);
	bool leer(Componente*componente);
	bool insertar(Componente*componente);
	bool modificar(Componente*componente);
	bool eliminar(Componente*componente);
	bool siguiente(Componente*componente);
	bool obtener(Componente*componente);
	size_t posicionComponente();
};

#endif /* EAREGISTROS_H_ */
