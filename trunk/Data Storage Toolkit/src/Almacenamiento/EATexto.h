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

class EATexto: public EstrategiaAlmacenamiento {
	Almacenamiento* almacen;
	size_t posComp;
	std::string ultimoLeido;
	void siguiente();
	std::string registroAstring(Registro*registro);
	void stringAregistro(Registro*registro,std::string &str);
	size_t buscar(Registro*registro,bool&encontrado);
	int comparar(Registro*reg1,Registro*reg2);
public:
	EATexto();
	virtual ~EATexto();
	void abrir(Almacenamiento*almacen);
	void nuevo(Almacenamiento*almacen);
	void posicionarComponente(size_t nroCompuesto);
	bool escribir(Componente*componente);
	bool leer(Componente*componente);
	bool insertar(Componente*componente);
	bool modificar(Componente*componente);
	bool eliminar(Componente*componente);
	bool siguiente(Componente*componente);
	bool obtenerSiguiente(Componente*componente);
};

#endif /* EATEXTO_H_ */
