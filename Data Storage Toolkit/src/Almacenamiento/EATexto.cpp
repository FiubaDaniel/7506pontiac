/*
 * EATexto.cpp
 *
 *  Created on: 13/10/2009
 *      Author: paulo
 */
#include "EATexto.h"

EATexto::EATexto(Registro*registro) {
	this->registro=(Registro*)registro->clonar();
};

EATexto::~EATexto() {
	delete registro;
};

Almacenamiento* EATexto::abrir(Almacenamiento*almacen){

	Almacenamiento* anterior=this->almacen;
	this->almacen=almacen;
	posComp=0;
	linea.clear();
	ultimo=false;
	return anterior;
};
Almacenamiento* EATexto::crear(Almacenamiento *almacen){
	Almacenamiento* anterior=this->almacen;
	this->almacen=almacen;
	posComp=0;
	linea.clear();
	ultimo=false;
	return anterior;
};

int EATexto::comparar(Registro*registro,Registro*registro2){
		clave->set(registro);
		Clave*clave2=clave->clonarce();
		clave2->set(registro2);
		int resultado=comparador->Comparar(clave,clave2);
		delete clave2;
		return resultado;
};
Componente *EATexto::getComponente(){
	return registro;
}
bool EATexto::escribir(Componente *componente){
	Registro* registro=dynamic_cast<Registro*>(componente);
	if(registro){
		this->registroALinea(registro);
		escribirLinea();
		posComp++;
		return true;
	}
	return false;
}
bool EATexto::leer(Componente *componente){
	Registro* registro=dynamic_cast<Registro*>(componente);
	if(registro){
		posComp++;
		if(leerLinea() and linea[0]!=' '){
			lineaARegistro(registro);
			return true;
		}
	}
	return false;
}
bool EATexto::insertar(Componente *componente){
	Registro* nuevo=dynamic_cast<Registro*>(componente);
	if(nuevo){
		registroALinea(nuevo);
		size_t tamanio=linea.length();
		size_t posicion;
		// busco posicion de insercion
		bool encontrado=false;
		bool fin=false;
		posicion=almacen->posicionActual();
		while((not fin)and not encontrado){
			fin=not leerLinea();
			if(linea.length()>=tamanio){
				encontrado=(linea[0]==' ');
			}else posicion=almacen->posicionActual();
			posComp++;
			//encontrado=verdadero si es el ultimo lugar o si la linea tiene espacio suficiente
		};
		if(encontrado)
			almacen->posicionar(posicion);
		registroALinea(nuevo);
		escribirLinea();
		if(logActivo){
			clave->set(nuevo);
			pushCambio(Cambio(clave,posComp-1,Cambio::Alta));
		}
		return true;
	}
	return false;
}
bool EATexto::eliminar(Componente *componente){
	Registro* eliminado=dynamic_cast<Registro*>(componente);
	if(eliminado){
		size_t posicion=almacen->posicionActual();
		if(leer(registro)){
			if(comparar(registro,eliminado)==0){
				size_t tamanio=linea.length();
				linea.clear();
				linea.resize(tamanio,' ');
				almacen->posicionar(posicion);
				escribirLinea();
				if(logActivo){
					clave->set(eliminado);
					pushCambio(Cambio(clave,posComp-1,Cambio::Baja));
				}
				return true;
			}
		}
	}
	return false;
}
bool EATexto::modificar(Componente *componente){
	Registro* modificado=dynamic_cast<Registro*>(componente);// no elimina el ultimo
	if(modificado){
		size_t posicion=almacen->posicionActual();
		if(leer(registro)){
			if(comparar(registro,modificado)==0){
				size_t tamanioAnterior=linea.length();
				registroALinea(modificado);
				if(linea.length()<=tamanioAnterior){
					/*si el tamanio del modificado es menor o igual se escribe en la posicion actual*/
					almacen->posicionar(posicion);
					escribirLinea();
					if(logActivo){
						clave->set(modificado);
						pushCambio(Cambio(clave,posComp-1,Cambio::Modificacion));
					}
				}else{
					size_t nuevoTamanio=linea.length();
					linea.clear();
					linea.resize(tamanioAnterior,' ');
					almacen->posicionar(posicion);
					escribirLinea();
					/*busco nuevo lugar*/
					bool encontrado=false;
					bool fin=false;
					posicion=almacen->posicionActual();
					while((not fin) and not encontrado){
						fin=not leerLinea();
						if(linea.length()>=nuevoTamanio){
							encontrado=(linea[0]==' ');
						}else posicion=almacen->posicionActual();
						posComp++;
						//encontrado=verdadero si es el ultimo lugar o si la linea tiene espacio suficiente
					};
					if(encontrado)
						almacen->posicionar(posicion);
					registroALinea(modificado);
					escribirLinea();
					if(logActivo){
						clave->set(modificado);
						pushCambio(Cambio(clave,posComp-1,Cambio::Reubicacion));
					}
				}
				return true;
			}
		}
	}
	return false;
};
bool EATexto::obtener(Componente *componente){
	return siguiente(componente);
};
size_t EATexto::posicionComponente(){
	return posComp;
};
bool EATexto::posicionarComponente(size_t nroCompuesto){
	if(nroCompuesto<posComp){
			almacen->posicionar(0);
			posComp=0;
	}
	bool ultimo=false;
	while((not ultimo) and nroCompuesto>posComp){
		if(leerLinea()) posComp++;
		else ultimo=true;
	}
	return ultimo;
};
bool EATexto::siguiente(Componente *componente){
	bool encontrado=false;
	bool fin=false;
	while((not fin )and not encontrado){
		if(leerLinea()) posComp++;
		else fin=true;
		if(linea.length()>1 && linea.at(0)!=' '){
			encontrado=true;
			lineaARegistro((Registro*)componente);
		}
	};
	return encontrado;
};
void EATexto::lineaARegistro(Registro *registro){
	std::stringstream strs(linea);
	for(Ttamanio i=0;i<registro->cantidadAtributos();i++){
		registro->get(i)->leer(strs);
		strs<<" ";
	};
}
void EATexto::registroALinea(Registro *registro){
	std::stringstream strs;
	for(Ttamanio i=0;i<registro->cantidadAtributos();i++){
		registro->get(i)->imprimir(strs);
		strs<<" ";
	}
	linea=strs.str();
}
bool EATexto::leerLinea(){
	char chr=0;
	linea.clear();
	almacen->leer(&chr);
	while(!almacen->fin()&& chr!='\n'){
		linea.push_back(chr);
		almacen->leer(&chr);
	};
	if(chr!='\n'){
		almacen->reiniciar();
		return false;
	}
	return true;
};
void EATexto::escribirLinea(){
	linea.push_back('\n');
	almacen->reiniciar();
	almacen->escribir(linea.c_str(),linea.size());

};



