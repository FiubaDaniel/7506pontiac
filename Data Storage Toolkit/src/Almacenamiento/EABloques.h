/*
 * EABloques.h
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#ifndef EABLOQUES_H_
#define EABLOQUES_H_
#include "EstrategiaAlmacenamiento.h"
#include "Almacenamiento.h"
#include "../Compuesto/Bloque.h"
#include <sstream>
#include <fstream>
class EABloques : public EstrategiaAlmacenamiento{
private:
	Ttamanio nroRegistro;
	Ttamanio siguienteLibre;
	Ttamanio libres;
	size_t nroBloque;
	Bloque* bloque;
	Ttamanio capacBloque;
	char* bloqueSerializado;
	double porcCarga;
	Almacenamiento *archivoEspacioLibre;
	size_t buscarEspacioLibre(Ttamanio espacio,bool& encontrado);
	bool buscarComponente(Registro*registro,Ttamanio&posicion);
	bool buscarComponente(Clave*unaClave,Ttamanio & posicion);
	bool leerBloque(Bloque*bloque);
	bool escribirBloque(Bloque*bloque);
public:
	EABloques(Registro* tipoRegistro,Ttamanio tamanioBloque,double porcCarga=0.8);
	virtual ~EABloques();
	bool abrir();
	void crear();
	void cerrar();
	bool posicionarComponente(size_t nroCompuesto);
	bool escribir(Componente*componente);
	bool leer(Componente*componente);
	bool insertar(Componente*componente);
	bool modificar(Componente*componente);
	bool eliminar(Componente*componente);
	bool eliminar(Clave*unaClave);
	bool siguiente(Componente*componente);
	bool obtener(Componente*componente);
	size_t posicionComponente();
	bool buscar(Componente*componente);
	bool buscar(Clave*unaClave);
	void imprimirMetada(std::ostream&out);
	 /*------------------------------------------------------------*/
	Componente* getRegistro();
	Componente*getComponenteUsado();
	void setRegistro(Registro*registro);
    Ttamanio getCapacBloque();
    double getPorcCarga();
    /*------------------------------------------------------------*/
};

#endif /* EABLOQUES_H_ */
