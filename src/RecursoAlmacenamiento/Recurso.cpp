#include "Recurso.h"

// Constructors/Destructors
//  

Recurso::Recurso (EstrategiaRecursos* estrategiaUsada) {
	this->estrategiaUsada=estrategiaUsada;
}

Recurso::~Recurso () {}

bool Recurso::insertar(Registro* registro){
	return estrategiaUsada->insertar(registro);
}


bool Recurso::eliminar(Clave*unaClave){
	return estrategiaUsada->eliminar(unaClave);
}



bool Recurso::modificar(Clave* unaClave,Registro* registro){
	return estrategiaUsada->modificar(unaClave,registro);
}



bool Recurso::obtener(Clave* unaClave,Registro*registro){
	return estrategiaUsada->obtener(unaClave,registro);
}

/*
 * Cierra el archivo
 *
 *
 * Escritura (Archivo/Buffer)
 * EscrituraDirecta  (Archivo/Buffer)
 * EscrituraDiferida  (Archivo/Buffer), en este caso verifica que no haya sectores
 * del buffer sin escribir, en tal caso los baja a disco y cierra.
 */

void Recurso::cerrarArchivo(){

}
EstrategiaRecursos* Recurso::getEstrategia(){
	return estrategiaUsada;
}
void Recurso::mostrarIndice(){}
void Recurso::mostrarDatos(){}
EstrategiaRecursos* Recurso::getEstrategiaRecurso(){
	return estrategiaUsada;
}

