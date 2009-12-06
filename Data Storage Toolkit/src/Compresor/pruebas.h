/*
 * pruebas.h
 *
 *  Created on: 30/11/2009
 *      Author: paulo
 */

#ifndef _PRUEBAS_COMPRESION_
#define _PRUEBAS_COMPRESION_
#include "Compresor.h"
#include "EstrategiaCompresion.h"
#include "../Almacenamiento/Archivo.h"
#include "../Almacenamiento/EARegistros.h"
#include "../Almacenamiento/EABloques.h"
#include "../Compuesto/Registro.h"
#include "../Compuesto/Bloque.h"
#include "../Compuesto/AtributoFijo.h"
#include "../Compuesto/AtributoVariable.h"
#include "../ArbolBSharp/BSharpTree.h"
#include "../ArbolBSharp/Clave.h"
#include "../ArbolBSharp/ComparadorRegistroVariable.h"
#include "../Terminos.h"
#include <iostream>
#include <list>

using namespace std;

int pruebacompresion1();
int pruebacompresion2();
int pruebaCompresion3();
int pruebaEstragiaCompresionAlmacenamiento();
int pruebaEstragiaCompresionAlmacenamiento1();
int pruebaEstrategiaCompresionArbol();
int PruebaComprimirNodo();
int pruebaCompresion5();

#endif
