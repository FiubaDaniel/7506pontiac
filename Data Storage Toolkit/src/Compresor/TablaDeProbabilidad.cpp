#include "TablaDeProbabilidad.h"

TablaDeProbabilidad::TablaDeProbabilidad() {
	// TODO Auto-generated constructor stub

}

TablaDeProbabilidad::~TablaDeProbabilidad() {
	// TODO Auto-generated destructor stub
}

void TablaDeProbabilidad::obtenerExtremos(char contexto,char simbolo,unsigned & piso,unsigned &techo){
	unsigned longitud=techo-piso;
	tipo_frecuencia incremento;
	tipo_frecuencia total;
	tipo_contextos::iterator it=contextos.find(contexto);
	if(it!=contextos.end()){
		total=it->second.totalFrecuencias;
		tipo_tabla_frecuencias::iterator itTabla=it->second.tablaFrecuencias.begin();
		unsigned char cant_elementos=0;
		/*acumulacion de los que ESTAN en la tabla*/
		while(itTabla!=it->second.tablaFrecuencias.end() and itTabla->simbolo<simbolo){
			incremento=itTabla->frecuencia*((longitud/total)+(1/total));
			piso+=floor(piso+incremento);
			cant_elementos++;
			itTabla++;
		}
		/*acumulacion de los que NO ESTAN en tabla*/
		if(cant_elementos<(unsigned char)simbolo){
			cant_elementos=((unsigned char)simbolo)-cant_elementos;
			incremento=cant_elementos*((longitud/total)+(1/total));
			piso+=floor(piso+incremento);
		}
		/*incremento para el techo*/
		if(itTabla!=it->second.tablaFrecuencias.end() and itTabla->simbolo==simbolo){
			incremento=itTabla->frecuencia*((longitud/total)+(1/total));
		}else{
			incremento=((longitud/total)+(1/total));
		}
	}else{
		/*tabla completamente vacia*/
		total=256;
		incremento=((unsigned char)(simbolo-1))*((longitud/total)+(1/total));
		piso+=floor(piso+incremento);
		incremento=((longitud/total)+(1/total));
	}
	techo=floor(piso+incremento)-1;
}
