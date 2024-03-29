/*
 * TablaOrden0.h
 *
 *  Created on: 01/12/2009
 *      Author: paulo
 */

#ifndef TABLAORDEN0_H_
#define TABLAORDEN0_H_
#include <cmath>
#define tipo_long float
class TablaOrden0 {
	unsigned ocurrencias[256];
	unsigned total;
public:
	TablaOrden0(){
		for(int i=0;i<256;i++){
			ocurrencias[i]=1;
		}
		total=256;
	}
	bool esEscape(){
		return false;
	}
	void obtenerExtremos(unsigned char contexto,unsigned char simbolo,unsigned & piso,unsigned &techo){
		//unsigned long longitud=techo-piso;
		tipo_long longitud=techo-piso;
		longitud+=1;
		double prob;
		for(unsigned char i=0;i<simbolo;i++){
			prob=ocurrencias[i];
			prob/=total;
			prob*=longitud;
			piso=piso+prob;
		}
		prob=ocurrencias[simbolo];
		prob/=total;
		prob*=longitud;
		techo=floor(prob+piso)-1;
	}
	void incremtarOcurrencia(unsigned char contexto,unsigned char simbolo){
		ocurrencias[simbolo]++;
		total++;
	}
	void decremetarOcurrencia(unsigned char contexto,unsigned char simbolo){
		if(ocurrencias[simbolo]){
			ocurrencias[simbolo]--;
			total--;
		}
	}
	unsigned char calcularEmision(unsigned &piso,unsigned &techo,unsigned codigo,unsigned char anterior){
		//unsigned long longitud=techo-piso;
		tipo_long longitud=techo-piso;
		longitud+=1;
		bool encontrado=false;
		unsigned short i=0;
		double prob;
		prob=ocurrencias[0];
		prob/=total;
		prob*=longitud;
		techo=floor(piso + prob)-1;
		while(not encontrado and i<256){
			if(piso<=codigo and  codigo<=techo){
				encontrado=true;
			}else{
				i++;
				piso=techo+1;
				prob=ocurrencias[i];
				prob/=total;
				prob*=longitud;
				techo=floor(piso+prob)-1;
			}
		}
		return i;
	}
};

#endif /* TABLAORDEN0_H_ */
