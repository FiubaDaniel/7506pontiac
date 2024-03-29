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
	Referencia nroBloque;
	Bloque* bloque;
	Ttamanio capacBloque;
	char* bloqueSerializado;
	double porcCarga;
	Almacenamiento *archivoEspacioLibre;
	Referencia buscarEspacioLibre(Ttamanio espacio,bool& encontrado);
	bool buscarComponente(Registro*registro,Ttamanio&posicion);
	bool buscarComponente(Clave*unaClave,Ttamanio & posicion);
	bool leerBloque(Bloque*bloque);
	bool escribirBloque(Bloque*bloque);
public:
	EABloques(Registro* tipoRegistro,Ttamanio tamanioBloque,double porcCarga=0.8);
	EABloques(Bloque* tipoBloque,Ttamanio tamanioBloque,double porcCarga=0.8);
	virtual ~EABloques();
	bool abrir();
	void crear();
	void cerrar();
	bool posicionarComponente(Referencia nroCompuesto);
	bool escribir(Componente*componente);
	bool leer(Componente*componente);
	bool insertar(Componente*componente);
	bool modificar(Componente*componente);
	bool eliminar(Componente*componente);
	bool eliminar(Clave*unaClave);
	bool siguiente(Componente*componente);
	bool obtener(Componente*componente);
	Referencia posicionComponente();
	bool buscar(Componente*componente);
	bool buscar(Clave*unaClave);
	void imprimirMetada(std::ostream&out);
	std::string getMetadata();
	void setMetadata(char* metadata);
	unsigned getTamanioComponenteUsado();
	virtual Referencia ultimoComponente();
	bool truncar(Referencia ultimo){
		siguienteLibre=ultimo+1;
		return true;
	}
	 /*------------------------------------------------------------*/
	Componente* getRegistro();
	Componente*getComponenteUsado();
	void setRegistro(Registro*registro);
    Ttamanio getCapacBloque();
    double getPorcCarga();
    /*------------------------------------------------------------*/
};

#endif /* EABLOQUES_H_ */
