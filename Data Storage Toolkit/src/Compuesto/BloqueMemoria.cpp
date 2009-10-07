/*
 * BloqueMemoria.cpp
 *
 *  Created on: 06/10/2009
 *      Author: paulo
 */

#include "BloqueMemoria.h"
BloqueMemoria::BloqueMemoria(Bloque* bloque): BloqueMemoria::Bloque(){

};
BloqueMemoria::~BloqueMemoria(){};
bool BloqueMemoria::estaEscrito(){};
bool BloqueMemoria::estaSucio(){};
bool BloqueMemoria::setSucio(){};
bool BloqueMemoria::setEscrito(){};
Ttamanio BloqueMemoria::deserializar(void*entrada){};
Ttamanio BloqueMemoria::serializar(void*salida){};
Ttamanio BloqueMemoria::tamanioSerializado(){};
Componente* BloqueMemoria::clonar(){};
Componente* BloqueMemoria::get(Ttamanio nroComponente){};
Ttamanio BloqueMemoria::cantidadComponentes(){};
bool BloqueMemoria::insertar(Componente*componente,Ttamanio posicion){};
bool BloqueMemoria::eliminar(Ttamanio posicion){};
bool BloqueMemoria::agregar(Componente*componente){};
Componente* BloqueMemoria::reemplazar(Componente*componente,Ttamanio posicion){};
