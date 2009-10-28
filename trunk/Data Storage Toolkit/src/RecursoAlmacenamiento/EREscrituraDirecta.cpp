#include "EREscrituraDirecta.h"

EREscrituraDirecta::EREscrituraDirecta(Almacenamiento *buffer){
	this->buffer=buffer;
}

EREscrituraDirecta::~EREscrituraDirecta(){
}

bool EREscrituraDirecta::eliminar(Clave *unaClave){
	return false;
}

bool EREscrituraDirecta::insertar(Registro *registro){
	//Referencia referencia=estrategiaAlmacenamiento->insertar(Resgitro);
	return false;
}

bool EREscrituraDirecta::modificar(Clave *unaClave, Registro *registro)
{return false;
}

bool EREscrituraDirecta::obtener(Clave* unaClave, Registro *registro)
{return false;
}





