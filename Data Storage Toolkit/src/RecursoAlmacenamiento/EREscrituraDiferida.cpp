#include "EREscrituraDiferida.h"


bool EREscrituraDiferida :: insertar(Registro* registro)
{
        /**Se serializa el registro hacia un arreglo de chars llamado "serializado"**/
	std::stringbuf buf;
	buf.pubseekpos(0,std::ios_base::binary | std::ios_base::in );
	registro->serializar(buf);

	Ttamanio tam_registro=registro->tamanioSerializado();
	char serializado[tam_registro];
	memcpy(serializado, buf.str().data(), tam_registro);

        /**Se escribe en el buffer**/
        buffer_recurso.escribir(0, serializado, tam_registro);


        //ESTA LISTO SI SE SABE EL NRO_BLOQUE donde se quiere insertar!!!
}

bool EREscrituraDiferida :: eliminar(Clave* unaClave)
{
}

bool EREscrituraDiferida :: modificar(Clave* unaClave,Registro* registro)
{
}

bool EREscrituraDiferida :: obtener(Clave* unaClave,Registro*registro)
{
        Referencia referencia; //numero de bloque !

	if(indice!=NULL){
		if(!indice->BuscarReferencia(unaClave,&referencia))
			return false;
	}else{
                //busqueda secuencial?
	}

        /**Se trata de leer desde el buffer**/
        buffer_recurso.leer( (int) referencia, registro);
}
