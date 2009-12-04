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
#include "Almacenamiento.h"

class EARegistros: public EstrategiaAlmacenamiento {
	Registro*registro;
	Referencia nroRegistro;
	Referencia siguienteRegLibre;
	Ttamanio tamanioRegistro;
	char* registroSerializado;
	int comparar(Registro*reg1,Registro*reg2);
	/*escribe el registro en la posicion.*/
	void escribir(Registro*registro);
	bool leer(Registro*registro);
public:
	EARegistros(Registro*registro);
	virtual ~EARegistros();
	bool abrir();
	void crear();
	void cerrar();
	bool posicionarComponente(Referencia nroCompuesto);
	bool escribir(Componente*componente);
	bool leer(Componente*componente);
	bool insertar(Componente*componente);
	bool modificar(Componente*componente);
	bool eliminar(Componente*componente);
	bool eliminar(Clave *unaClave);
	bool siguiente(Componente*componente);
	bool obtener(Componente*componente);
	Referencia posicionComponente();
	bool buscar(Componente*componente);
	bool buscar(Clave*unaClave);
	Componente*getComponenteUsado();
	unsigned getTamanioComponenteUsado();
	void imprimirMetada(std::ostream&out);
	std::string getMetadata();
	void setMetadata(char* metadata);
};

#endif /* EAREGISTROS_H_ */
