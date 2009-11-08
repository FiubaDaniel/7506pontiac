#include "Recurso.h"

// Constructors/Destructors
//  

Recurso::Recurso (Almacenamiento* archivo,EstrategiaIndice* indice,EstrategiaRecursos* estrategiaUsada,EstrategiaAlmacenamiento*estretegiaAlmacenamiento) {
	this->archivo=archivo;
	this->indice=indice;
	this->estrategiaUsada=estrategiaUsada;
	this->estrategiaAlmacenamiento=estretegiaAlmacenamiento;
	this->estrategiaUsada->setEstrategiAlmacenamiento(estretegiaAlmacenamiento);
	this->estrategiaUsada->setIndice(indice);
	//TODO verificar q el registro este seteado en todos.
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

void Recurso::mostrarIndice(){}
void Recurso::mostrarDatos(){}

