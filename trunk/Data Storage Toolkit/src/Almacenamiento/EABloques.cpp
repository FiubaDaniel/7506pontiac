/*
 * EABloques.cpp
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */
#include "EABloques.h"
#include <iostream>

EABloques::EABloques(Registro* tipoRegistro,Ttamanio tamanioBloque,double porcentajeCarga){
	bloque=new Bloque(tipoRegistro);
	capacBloque=tamanioBloque;
	porcCarga=porcentajeCarga;
	archivoEspacioLibre=NULL;
}
EABloques::EABloques(Bloque* tipoBloque,Ttamanio tamanioBloque,double porcentajeCarga){
	bloque=(Bloque*)tipoBloque->clonar();
	capacBloque=tamanioBloque;
	porcCarga=porcentajeCarga;
	archivoEspacioLibre=NULL;
}
EABloques::~EABloques() {
	delete archivoEspacioLibre;
	delete bloque;
}
void EABloques::crear(){
	nroRegistro=0;
	nroBloque=0;
	siguienteLibre=0;
	std::string ruta=almacen->getNombre()+".bloques";
	if(!archivoEspacioLibre)
		archivoEspacioLibre=almacen->clonar();
	archivoEspacioLibre->crear(ruta.c_str());
	almacen->posicionar(0);
	almacen->escribir(&capacBloque , sizeof(capacBloque) );
	almacen->escribir(&siguienteLibre , sizeof(siguienteLibre) );
	almacen->escribir(&porcCarga,sizeof(porcCarga));
	bloqueSerializado=new char[capacBloque];

}
bool EABloques::abrir(){
	std::string ruta=almacen->getNombre()+".bloques";
	if(!archivoEspacioLibre)
		archivoEspacioLibre=almacen->clonar();
	if(not archivoEspacioLibre->abrir(ruta.c_str()))
		return false;
	almacen->posicionar(0);
	almacen->leer(&capacBloque , sizeof(capacBloque) );
	almacen->leer(&siguienteLibre , sizeof(siguienteLibre) );
	almacen->leer(&porcCarga,sizeof(porcCarga));
	bloqueSerializado=new char[capacBloque];
	nroRegistro=0;
	return true;
}

void EABloques::cerrar(){
	delete[] bloqueSerializado;
	bloqueSerializado=NULL;
	almacen->posicionar(0);
	almacen->escribir(&capacBloque , sizeof(capacBloque) );
	almacen->escribir(&siguienteLibre , sizeof(siguienteLibre) );
	almacen->escribir(&porcCarga,sizeof(porcCarga));
	if(archivoEspacioLibre){
		archivoEspacioLibre->cerrar();
		delete archivoEspacioLibre;
		archivoEspacioLibre=NULL;
	}
}
bool EABloques::posicionarComponente(Referencia nroCompuesto){
	nroBloque=nroCompuesto;
	almacen->posicionar(capacBloque*nroBloque+sizeof(siguienteLibre)+sizeof(capacBloque)+sizeof(porcCarga));
	if(almacen->fin()){
		almacen->clear();
		return false;
	}
	nroRegistro=0;
	return true;
}
void EABloques::setRegistro(Registro*registro){
	if(bloque!=NULL){
		bloque->inicializar(registro);
	}else bloque=new Bloque(registro);
}

bool EABloques::leerBloque(Bloque*bloque){
	/*leo el bloque*/
	almacen->leer(bloqueSerializado,capacBloque);
	if(almacen->fin()){
		almacen->clear();
		return false;
	}
	/*leo espacio libres del bloque*/
	if(archivoEspacioLibre){
		archivoEspacioLibre->posicionar(nroBloque*sizeof(Ttamanio));
		archivoEspacioLibre->leer((char*)&libres,sizeof(Ttamanio));
	}
	/*deserializo*/
	std::stringbuf buf;
	buf.pubsetbuf(bloqueSerializado,capacBloque);
	buf.pubseekpos(0,std::ios_base::binary | std::ios_base::in);
	bloque->deserializar(buf);
	nroBloque++;
	return true;
}
bool EABloques::escribirBloque(Bloque*bloque){
	/*Serializo*/
	std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
	buf.pubsetbuf(bloqueSerializado,capacBloque);
	bloque->serializar(buf);
	/*Actualizo archivo de espacios libres*/
	libres=capacBloque-bloque->tamanioSerializado();
	if(archivoEspacioLibre){
		archivoEspacioLibre->posicionar(nroBloque*sizeof(Ttamanio));
		archivoEspacioLibre->escribir((char*)&libres,sizeof(Ttamanio));
	}
	/*Escribo el bloque*/
	almacen->escribir(bloqueSerializado,capacBloque);
	nroBloque++;
	return true;
}
bool EABloques::escribir(Componente*compuesto){
	Bloque*bloque=dynamic_cast<Bloque*>(compuesto);
	if(bloque!=NULL&&nroBloque<=siguienteLibre){
		if(bloque->tamanioSerializado()<=capacBloque){
			escribirBloque(bloque);
			if(siguienteLibre<=nroBloque)
				siguienteLibre=nroBloque;
/*			if(colaDeCambios){
				for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
					clave->set((Registro*)bloque->get(i));
					colaDeCambios->push(Cambio(clave,nroBloque,Cambio::Alta));
				}
			}*/
			return true;
		}
	}
	Registro*registro=dynamic_cast<Registro*>(compuesto);
	if(registro!=NULL){
		if(leerBloque(this->bloque)){
			archivoEspacioLibre->posicionar(nroBloque*sizeof(Ttamanio));
			archivoEspacioLibre->leer((char*)&libres,sizeof(Ttamanio));
			if( libres  <  registro->tamanioSerializado() )
				return false;
			this->bloque->agregar(registro);
			posicionarComponente(nroBloque-1);
			escribirBloque(this->bloque);
			return true;
		}
	}
	return false;
}

bool EABloques::leer(Componente*compuesto){
	Bloque*bloque=dynamic_cast<Bloque*>(compuesto);
	if(bloque!=NULL &&nroBloque<siguienteLibre){
		return leerBloque(bloque);
	}
	Registro*registro=dynamic_cast<Registro*>(compuesto);
	if(registro!=NULL){
		if(leerBloque(this->bloque)){
			Ttamanio pos;
			if(buscarComponente(registro,pos)){
				std::stringbuf buf(std::ios_base::binary | std::ios_base::in | std::ios_base::out );
				buf.pubseekpos(0);
				this->bloque->get(pos)->serializar(buf);
				buf.pubseekpos(0);
				registro->deserializar(buf);
				return true;
			}
		}
	}
	return false;
}
Referencia EABloques::buscarEspacioLibre(Ttamanio espacio,bool& encontrado){
	Referencia nroBloque=0;
	libres=capacBloque;
	encontrado=false;
	if(not archivoEspacioLibre)
		return 0;
	archivoEspacioLibre->posicionar(0);
	while(!encontrado && nroBloque<siguienteLibre && !archivoEspacioLibre->fin()){
		archivoEspacioLibre->leer((char*)&libres,sizeof(Ttamanio));
		Ttamanio disponibles=libres-(1-porcCarga)*capacBloque;
		encontrado=disponibles>espacio and capacBloque>disponibles;
		if(!encontrado)nroBloque++;
	}
	archivoEspacioLibre->clear();
	return nroBloque;
}

bool EABloques::insertar(Componente*componente){
	bool encontrado;
	Registro*nuevo=dynamic_cast<Registro*>(componente);
	if(nuevo!=NULL){
		Referencia posInsercion=buscarEspacioLibre(nuevo->tamanioSerializado(),encontrado);
		if(encontrado){
			posicionarComponente(posInsercion);
			if(!leerBloque(bloque)) return false;
			bloque->agregar(componente);
		}else{
			bloque->inicializar(componente);
			siguienteLibre++;
		}
		posicionarComponente(posInsercion);
		escribirBloque(bloque);

		if(colaDeCambios){
			clave->set(nuevo);
			colaDeCambios->push(Cambio(clave,posInsercion,Cambio::Alta));
		}
	}
	return true;
}



bool EABloques::modificar(Componente*componente){
	Referencia posModificado=nroBloque;
	/*Si el bloque que sigue es valido*/
	if(nroBloque<siguienteLibre){
		/*leo el bloque que sigue*/
		if(!leerBloque(bloque)) return false;
		Ttamanio nroComponente=0;
		/*busco el registro en el bloque leido*/
		if(buscarComponente((Registro*)componente,nroComponente)){
			Ttamanio tamanioDisponible=bloque->get(nroComponente)->tamanioSerializado()+libres;
			Ttamanio tamanioNuevo=componente->tamanioSerializado();
			if(tamanioNuevo>=capacBloque)
				return false;/*una excepcion para la proxima*/
			if(tamanioDisponible<tamanioNuevo){
				/*elimino del bloque y escribo*/
				bloque->eliminar(nroComponente);
				posicionarComponente(posModificado);
				escribirBloque(bloque);
				/* busco un bloque donde insertar*/
				bool encontrado;
				posModificado=buscarEspacioLibre(tamanioNuevo,encontrado);
				if(encontrado){
					/*existe un bloque con espacio*/
					posicionarComponente(posModificado);
					if(not leerBloque(bloque))return false;
					bloque->agregar(componente);
				}else{
					/*agrego al final*/
					bloque->inicializar(componente);
					posModificado=siguienteLibre;
					siguienteLibre++;
				}
				if(colaDeCambios){
					//TODO modificacion ? reubicacion?
					clave->set((Registro*)componente);
					colaDeCambios->push(Cambio(clave,posModificado,Cambio::Reubicacion));
				}
			}else{
				/*hay espacio para la modificacion en el bloque actual*/
				Componente* eliminado=bloque->reemplazar(componente,nroComponente);
				if(eliminado!=NULL)
					delete eliminado;
			}
			/*se escribe el bloque con el registro modificado*/
			posicionarComponente(posModificado);
			escribirBloque(bloque);
			return true;
		}
	}
	return false;
}
bool EABloques::buscarComponente(Registro*registro,Ttamanio & posicion){
	clave->set(registro);
	Clave*aux=clave->clonarce();
	for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
		clave->set((Registro*)bloque->get(i));
		if(comparador->Comparar(clave,aux)==0){
			posicion=i;
			delete aux;
			return true;
		}
	}
	delete aux;
	return false;
}
bool EABloques::buscarComponente(Clave*unaClave,Ttamanio & posicion){
	for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
		clave->set((Registro*)bloque->get(i));
		if(comparador->Comparar(clave,unaClave)==0){
			posicion=i;
			return true;
		}
	}
	return false;
}
bool EABloques::eliminar(Componente*componente){
	Bloque*bloqueeliminado=dynamic_cast<Bloque*>(componente);
	if(bloqueeliminado){
		libres=0;
		archivoEspacioLibre->posicionar(nroBloque*sizeof(Ttamanio));
		archivoEspacioLibre->escribir((char*)&libres,sizeof(Ttamanio));
	}
	Referencia posBorrado=nroBloque;
	if(nroBloque<siguienteLibre){
		if(not leerBloque(bloque))return false;
		Ttamanio nroComponente;
		if(buscarComponente((Registro*)componente,nroComponente)){
			if(bloque->cantidadComponentes()<=1){
				if(posBorrado<siguienteLibre-1){
					posicionarComponente(siguienteLibre-1);
					leerBloque(bloque);
					if(colaDeCambios){
						for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
							clave->set((Registro*)bloque->get(i));
							colaDeCambios->push(Cambio(clave,posBorrado,Cambio::Reubicacion));
						}
					}
				}
				siguienteLibre--;
			}else{
				bloque->eliminar(nroComponente);
			}
			nroBloque=posBorrado;
			if(posicionarComponente(posBorrado)){
				escribirBloque(bloque);
			}
			if(colaDeCambios){
				clave->set((Registro*)componente);
				colaDeCambios->push(Cambio(clave,posBorrado,Cambio::Baja));
			}
			return true;
		}
	}
	return false;
}
bool EABloques::eliminar(Clave*unaClave){
	Referencia posBorrado=nroBloque;
	if(nroBloque<siguienteLibre){
		if(not leerBloque(bloque))
			return false;
		Ttamanio nroComponente;
		if(buscarComponente(unaClave,nroComponente)){
			if(bloque->cantidadComponentes()<=1){
				if(posBorrado<siguienteLibre-1){
					posicionarComponente(siguienteLibre-1);
					leerBloque(bloque);
					if(colaDeCambios){
						for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
							clave->set((Registro*)bloque->get(i));
							colaDeCambios->push(Cambio(clave,posBorrado,Cambio::Reubicacion));
						}
					}
				}
				siguienteLibre--;
			}else{
				bloque->eliminar(nroComponente);
			}
			nroBloque=posBorrado;
			if(posicionarComponente(posBorrado)){
				escribirBloque(bloque);
			}
			if(colaDeCambios){
				colaDeCambios->push(Cambio(unaClave,nroBloque,Cambio::Baja));
			}
			return true;
		}
	}
	return false;
}
bool EABloques::siguiente(Componente*componente){
	if(nroBloque==0 and nroRegistro==0){
		leerBloque(bloque);
		nroBloque=0;
	}
	if(nroRegistro>= bloque->cantidadComponentes()){
		if(almacen->fin() or nroBloque+1>=siguienteLibre){
			return false;
		}
		if(not leerBloque(bloque))return false;
		nroRegistro=0;
	}
	std::stringbuf buf(std::ios_base::binary | std::ios_base::in | std::ios_base::out );
	buf.pubsetbuf(bloqueSerializado,capacBloque);
	buf.pubseekpos(0);

	bloque->get(nroRegistro)->serializar(buf);
	buf.pubseekpos(0);

	componente->deserializar(buf);
	nroRegistro++;
	return true;
}
Componente *EABloques::getRegistro(){
	return bloque->get(0);
}

bool EABloques::obtener(Componente*componente){
	if(nroBloque<siguienteLibre){
		leerBloque(bloque);
		if(!buscarComponente((Registro*)componente,nroRegistro))
			return false;

		std::stringbuf buf(std::ios_base::binary | std::ios_base::in | std::ios_base::out );
		buf.pubsetbuf(bloqueSerializado,capacBloque);
		buf.pubseekpos(0);

		bloque->get(nroRegistro)->serializar(buf);
		buf.pubseekpos(0);

		componente->deserializar(buf);
		nroRegistro++;
		return true;
	}
	return false;
}
Referencia EABloques::posicionComponente(){
	return nroBloque;
}
bool EABloques::buscar(Componente*componente){
	bool encontrado=false;
	bool fin=false;

	posicionarComponente(0);
	while( (not fin) and not encontrado ){
		Ttamanio pos;
		Referencia posComp=posicionComponente();
		fin=leer(bloque);
		if(buscarComponente((Registro*)componente,pos)){
			encontrado=true;
			std::stringbuf buf(std::ios_base::binary | std::ios_base::in | std::ios_base::out );
			buf.pubsetbuf(bloqueSerializado,capacBloque);
			buf.pubseekpos(0);

			bloque->get(pos)->serializar(buf);
			buf.pubseekpos(0);

			componente->deserializar(buf);
			if(colaDeCambios){
				clave->set((Registro*)componente);
				colaDeCambios->push(Cambio(clave,posComp,Cambio::Reubicacion));
			}

		}
	}
	return encontrado;
}
bool EABloques::buscar(Clave*unaClave){
	bool encontrado=false;
	bool fin=false;

	posicionarComponente(0);
	while( (not fin) and not encontrado ){
		Ttamanio pos;
		Referencia posComp=posicionComponente();
		fin=leer(bloque);
		if(buscarComponente(unaClave,pos)){
			nroRegistro=pos-1;
			posicionarComponente(posComp);
			return true;
		}
	}
	return false;
}
Ttamanio EABloques::getCapacBloque(){
	return capacBloque;
}
double EABloques::getPorcCarga(){
	return porcCarga;
}
Componente*EABloques::getComponenteUsado(){
	return bloque;
}

void EABloques::imprimirMetada(std::ostream&out){
	out<<"Estrategia Bloques: "<<std::endl;
	out<< " capacidadBloque: "<<capacBloque;
	out<< "% carga "<< porcCarga;
	out<< " cantidad bloques "<<siguienteLibre;
	out<<std::endl;
}
std::string EABloques::getMetadata(){
	std::string resultado;
	resultado.append((char*)&capacBloque,sizeof(Ttamanio));
	resultado.append((char*)&porcCarga,sizeof(double));
	resultado.append((char*)&siguienteLibre,sizeof(Ttamanio));
	return resultado;
};
unsigned EABloques::getTamanioComponenteUsado(){
	return capacBloque;
}
Referencia EABloques::ultimoComponente(){
	return siguienteLibre-1;
}
void EABloques::setMetadata(char* metadata){
	capacBloque=*(Ttamanio*)metadata;
	metadata+=sizeof(Ttamanio);
	porcCarga=*(double*)metadata;
	metadata+=sizeof(double);
	siguienteLibre=*(Ttamanio*)metadata;
}
