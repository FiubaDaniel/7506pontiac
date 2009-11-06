#include "EstrategiaBSharp.h"

EstrategiaBSharp::EstrategiaBSharp(Clave* clave){
	arbol = new BSharpTree(clave);
};

virtual EstrategiaBSharp::EstrategiaBSharp ( ){
	delete arbol;
};

void EstrategiaBSharp::crear(std::string nombreArch,unsigned int tamanioBloque,int tamanioSerializadoClave,ComparadorClave* comp){
	arbol->crear(nombreArch,tamanioBloque,tamanioSerializadoClave,comp);
};

void EstrategiaBSharp::abrir(std::string nombreArch,ComparadorClave* comp){
	arbol->abrir(nombreArch,comp);
};

bool EstrategiaBSharp::BuscarReferencia(const Clave* clave,Referencia* referencia){
	arbol->Buscar(clave,referencia);
};

Referencia EstrategiaBSharp::Siguiente(bool ultimo){
	arbol->Siguiente(ultimo);
};

bool EstrategiaBSharp::insertar(Referencia ref,Clave* clave){
	arbol->insertar(ref,clave);
};

bool EstrategiaBSharp::eliminar(const Clave* clave){
	arbol->eliminar(clave);
};

bool EstrategiaBSharp::modificar(const Clave* clave,Referencia refNueva){
	arbol->modificar(clave,refNueva);
};


