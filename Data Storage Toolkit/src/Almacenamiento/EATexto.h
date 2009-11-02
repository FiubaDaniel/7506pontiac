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
	Registro*registro;
	bool ultimo;
	Almacenamiento* almacen;
	size_t posComp;
	std::string ultimoLeido;
	void siguiente();
	std::string registroAstring(Registro*registro);
	void stringAregistro(Registro*registro,std::string &str);
	size_t buscarProximoLibre();
public:
	EATexto(Registro*registro);
	virtual ~EATexto();
	void abrir(Almacenamiento*almacen);
	void crear(Almacenamiento*almacen);
	Componente* getComponente();
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

#endif /* EATEXTO_H_ */
