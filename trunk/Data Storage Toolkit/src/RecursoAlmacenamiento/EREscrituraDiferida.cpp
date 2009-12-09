#include "EREscrituraDiferida.h"


bool EREscrituraDiferida :: insertar(Registro* registro)
{

	std::stringbuf buf;
	buf.pubseekpos(0,std::ios_base::binary | std::ios_base::in );
	registro->serializar(buf);

	Ttamanio tam_registro=registro->tamanioSerializado();
	char serializado[tam_registro];
	memcpy(serializado, buf.str().data(), tam_registro);


        Cambio* cambio=&cambiosArchivo.front();
	while(cambio->operacion!=Cambio::Alta){
		cambiosArchivo.pop();
		cambio=&cambiosArchivo.front();
	}

	/*

        //Se serializa el componente (bloque o registro)
        Componente* un_componente=almacen->getEstrategia()->getComponenteUsado()->clonar();
        Ttamanio tam_datos=almacen->getEstrategia()->getTamanioComponenteUsado();

        almacen->posicionarComponente( cambio->referencia );
        almacen->leer(un_componente);

        char serializado[tam_datos];
        std:: stringbuf buf;
        un_componente->serializar(buf);
        buf.pubseekpos(0,std::ios_base::binary | std::ios_base::in );

        memcpy(serializado,buf.str().data(),tam_datos);

        delete un_componente;

        */

        /**Se escribe en el buffer**/
        buffer_recurso.escribir(cambio->referencia, serializado, tam_registro);

        cambiosArchivo.pop();

        //actualizacion del indice?
        return true;
}

bool EREscrituraDiferida :: eliminar(Clave* unaClave)
{
        Registro *reg_ultimo;
        Referencia ref_ultimo = almacen->ultimoComponente();
        Referencia ref_eliminar;

       //bool truncar(Referencia ultimo)

        //Se obtiene la referencia del registro a eliminar
        if(indice!=NULL){
		if(!indice->BuscarReferencia(unaClave,&ref_eliminar))
			return false;
	}else{
                //busqueda secuencial?
	}

	//Algoritmo de eliminacion usando buffer cache

	buffer_recurso.leer(ref_ultimo, reg_ultimo);

	//Se serializa reg_ultimo
        std::stringbuf buf;
	buf.pubseekpos(0,std::ios_base::binary | std::ios_base::in );
	reg_ultimo->serializar(buf);

	Ttamanio tam_registro=reg_ultimo->tamanioSerializado();
	char serializado[tam_registro];
	memcpy(serializado, buf.str().data(), tam_registro);

	buffer_recurso.escribir(ref_eliminar, serializado, tam_registro);

	almacen->truncar(ref_ultimo);

}

bool EREscrituraDiferida :: modificar(Clave* unaClave,Registro* registro)
{
        Referencia referencia;
        //Se obtiene la referencia
	if(indice!=NULL){
		if(!indice->BuscarReferencia(unaClave,&referencia))
			return false;
	}else{
                //busqueda secuencial?
	}

        std::stringbuf buf;
	buf.pubseekpos(0,std::ios_base::binary | std::ios_base::in );
	registro->serializar(buf);

	Ttamanio tam_registro=registro->tamanioSerializado();
	char serializado[tam_registro];
	memcpy(serializado, buf.str().data(), tam_registro);

	buffer_recurso.escribir(referencia, serializado, tam_registro);

}

bool EREscrituraDiferida :: obtener(Clave* unaClave, Registro*registro)
{
        Referencia referencia; //numero de bloque

        /**Se obtiene la referencia**/
	if(indice!=NULL){
		if(!indice->BuscarReferencia(unaClave,&referencia))
			return false;
	}else{
                //busqueda secuencial?
	}

        /**Se trata de leer desde el buffer**/
        buffer_recurso.leer( (int) referencia, registro);
}
