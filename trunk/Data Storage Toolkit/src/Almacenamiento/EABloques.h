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
	Ttamanio nroRegistro;
	Ttamanio siguienteLibre;
	size_t nroBloque;
	Ttamanio libres;
	Ttamanio capacBloque;
	char* bloqueSerializado;
	double porcCarga;
	Bloque* bloque;
	std::fstream archivoEspacioLibre;
	size_t buscarEspacioLibre(Ttamanio espacio,bool& encontrado);
	bool buscarComponente(Registro*registro,Ttamanio&posicion);
	void finalizarAlamcenamiento();
	bool leerBloque(Bloque*bloque);
	bool escribirBloque(Bloque*bloque);
public:
	EABloques();
	EABloques(Registro* tipoRegistro,Ttamanio tamanioBloque,double porcCarga=0.8);
	virtual ~EABloques();
	bool abrir(Almacenamiento*almacen);
	bool crear(Almacenamiento*almacen);
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
	bool buscar(Componente*componente);
    /*------------------------------------------------------------*/
	Almacenamiento *getAlmacen() const{
        return almacen;
    }
    Ttamanio getCapacBloque() const{
        return capacBloque;
    }
    double getPorcCarga() const{
        return porcCarga;
    }
    /*------------------------------------------------------------*/
};

#endif /* EABLOQUES_H_ */
