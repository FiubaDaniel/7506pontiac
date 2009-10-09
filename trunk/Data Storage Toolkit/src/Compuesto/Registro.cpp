#include "Registro.h"
Registro::Registro(){};
Registro::Registro(unsigned int cantidadAtributos, ...):Registro::Componente() {
	/*locales*/
	va_list ap;
	va_start(ap, cantidadAtributos);
	Atributo* atributo;
	fijo=true;
	/*inicializacion y cargar*/
	while(cantidadAtributos>0){
		atributo = va_arg(ap, Atributo*);
		atributos.push_back(atributo->clonar());
		if(atributo->esfijo()) fijo=false;
		cantidadAtributos--;
	}
	if(!cantidadAtributos>0);//TODO else trow error
	va_end(ap);
}
Registro::~Registro() {
	for(Ttamanio i=0;i<atributos.size();i++){
			delete atributos.at(i);
	}
}
Ttamanio Registro::deserializar(std::streambuf&entrada){
	Ttamanio offset=0;
	for(Ttamanio i=0;i<atributos.size();i++){
		offset+=atributos.at(i)->deserializar(entrada);
	}
	return offset;
};
Ttamanio Registro::serializar(std::streambuf&salida){
	Ttamanio offset=0;
	for(Ttamanio i=0;i<atributos.size();i++){
		offset+=atributos.at(i)->serializar(salida);
	}
	return offset;
};
Ttamanio Registro::tamanioSerializado(){
	Ttamanio tamanio=0;
	for(Ttamanio i=0;i<atributos.size();i++){
		tamanio+=atributos.at(i)->tamanioSerializado();
	}
	return tamanio;
};
Componente* Registro::clonar(){
	Registro* clon=new Registro(0);
	for(Ttamanio i=0;i<atributos.size();i++){
		clon->atributos.push_back(atributos.at(i)->clonar());
	}
	return clon;
};
Atributo* Registro::get(Ttamanio posicion){
	return atributos.at(posicion);
};
Atributo* Registro::get(std::string nombre){
	/*Busqueda lineal de atributo con el nombre*/
	for(Ttamanio i=0;i<atributos.size();i++)
		if(atributos.at(i)->getNombre()==nombre) return atributos.at(i);
	return NULL;
};
Ttamanio Registro::cantidadAtributos(){
	return atributos.size();
};
Atributo* Registro::getId(){
	return atributos.at(0);
};
bool Registro::esfijo(){
	return fijo;
};
