#include "EstrategiaBSharp.h"

EstrategiaBSharp::EstrategiaBSharp(Clave* clave){
	arbol = new BSharpTree(clave);
}

EstrategiaBSharp::~EstrategiaBSharp ( ){
	delete arbol;
}

void EstrategiaBSharp::crear(std::string nombreArch,unsigned int tamanioBloque,Clave* clave,ComparadorClave* comp){
	arbol->crear(nombreArch,tamanioBloque,clave,comp);
}

bool EstrategiaBSharp::abrir(std::string nombreArch,ComparadorClave* comp){
	return arbol->abrir(nombreArch,comp);
}

bool EstrategiaBSharp::BuscarReferencia(const Clave* clave,Referencia* referencia){
	return arbol->Buscar(clave,referencia);
}

Referencia EstrategiaBSharp::Siguiente(bool ultimo){
	return arbol->Siguiente(ultimo);
}

bool EstrategiaBSharp::insertar(Referencia ref,Clave* clave){
	return arbol->insertar(ref,clave);
}

bool EstrategiaBSharp::eliminar(const Clave* clave){
	return arbol->eliminar(clave);
}

bool EstrategiaBSharp::modificar(const Clave* clave,Referencia refNueva){
	return arbol->modificar(clave,refNueva);
}

void EstrategiaBSharp::cerrar(){
	arbol->cerrar();
}
int EstrategiaBSharp::tamanioBloque(){
	return arbol->tamanioBloque();
}
void EstrategiaBSharp::mostrarEstado(){
	arbol->imprimir();
}

BSharpTree* EstrategiaBSharp::obtenerArbol(){
	return this->arbol;
}

