/*
 * EATexto.h
 *
 *  Created on: 13/10/2009
 *      Author: paulo
 */

#ifndef EATEXTO_H_
#define EATEXTO_H_
#include "../Compuesto/Registro.h"
#include <sstream>
#include <cstring>
#include "EstrategiaAlmacenamiento.h"
#include "Almacenamiento.h"

class EATexto: public EstrategiaAlmacenamiento {
	Registro*registro;
	size_t posComp;
	std::string linea;
	bool ultimo;
	int comparar(Registro*registro,Registro*registro2);
	/* intenta leer la siguiente linea del archivo, devuelve false,
	 * si tras intentar, encuentra el fin de archivo
	 */
	bool leerLinea();
	/*
	 * escribe linea en la posicion actual del archivo.
	 */
	void escribirLinea();
	/*carga en linea el contenido de un registro no vacio*/
	void registroALinea(Registro*registro);
	void lineaARegistro(Registro*registro);
public:
	EATexto(Registro*registro);
	virtual ~EATexto();
	bool abrir();
	void crear();
	void cerrar();
	Componente* getRegistro();
	bool posicionarComponente(size_t nroCompuesto);
	bool escribir(Componente*componente);
	bool leer(Componente*componente);
	bool insertar(Componente*componente);
	bool modificar(Componente*componente);
	bool eliminar(Componente*componente);
	bool eliminar(Clave*clave);
	bool siguiente(Componente*componente);
	bool obtener(Componente*componente);
	size_t posicionComponente();
	bool buscar(Componente*componente);
	bool buscar(Clave*unaClave);
	Componente*getComponenteUsado();
	void imprimirMetada(std::ostream&out);
};

#endif /* EATEXTO_H_ */
