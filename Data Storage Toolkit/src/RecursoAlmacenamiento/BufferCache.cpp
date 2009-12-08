#include "BufferCache.hpp"
#include <iostream>
#include <string>


void BufferCache :: remover_buffer_libre(Buffer_header *buff_para_usar){

        if (buffers_libres.primer_buffer == buffers_libres.ultimo_buffer)
        {
                //Significa que la lista de buffers libres tenia un solo buffer
                buffers_libres.primer_buffer = buffers_libres.ultimo_buffer = NULL;
        }else{

                if( buffers_libres.primer_buffer == buff_para_usar)
                {
                        //El buffer a liberar es el primero de la lista
                        buffers_libres.primer_buffer = buff_para_usar->siguiente_libre;
                        buff_para_usar->siguiente_libre->anterior_libre = (Buffer_header*) &buffers_libres;
                }else{
                        if( buffers_libres.ultimo_buffer == buff_para_usar)
                        {
                                //El buffer esta al final de la lista
                                buff_para_usar->anterior_libre->siguiente_libre = (Buffer_header*) &buffers_libres;
                                buffers_libres.ultimo_buffer = buff_para_usar->anterior_libre;

                        }else{
                                //El buffer esta entre dos buffers

                                buff_para_usar->anterior_libre->siguiente_libre = buff_para_usar->siguiente_libre;
                                buff_para_usar->siguiente_libre->anterior_libre = buff_para_usar->anterior_libre;

                        }
                }
        }

}

/*
        pre-condicion: la lista de libres no debe estar vacia
        post-condicion: se quita el primer buffer de la lista de buffers libres (puede quedar vacia)
*/
 void BufferCache :: remover_primer_buffer(Buffer_header *buff_para_reemplazar)
 {

         buff_para_reemplazar = buffers_libres.primer_buffer;

        if (buffers_libres.primer_buffer == buffers_libres.ultimo_buffer)
        {
                //Significa que la lista de buffers libres tenia un solo buffer
                buffers_libres.primer_buffer = buffers_libres.ultimo_buffer = NULL;
        }else{

                buffers_libres.primer_buffer = buff_para_reemplazar->siguiente_libre;
                buff_para_reemplazar->siguiente_libre->anterior_libre = (Buffer_header*) &buffers_libres;
        }
 }


void BufferCache :: dormir_proceso()
{
}

void BufferCache :: despertar_procesos()
{
}

/*
        Escribe todos los contenidos (a disco) de los buffers "delayed write" que hayan sido encontrados en el metodo "asignar_bloque()"
        Luego los coloca al principio de la free list

        pre-condicion : cant_diferidos distinto de de cero
*/
void BufferCache :: manejar_diferidos()
{
        for(int i=0; i < cant_diferidos; cant_diferidos--)
        {

                //referencia = delayed[i]->numero_bloque;
                // componente.escribir( delayed[i]->numero_bloque->datos );

                //Se pone al principio de la liste de libres
                if ( buffers_libres.primer_buffer == NULL)
                {
                        //lista vacia
                        buffers_libres.primer_buffer = buffers_libres.ultimo_buffer = delayed[i];
                        delayed[i]->anterior_libre = delayed[i]->siguiente_libre = (Buffer_header*) &buffers_libres;
                }else{

                        buffers_libres.primer_buffer->anterior_libre = delayed[i];
                        delayed[i]->siguiente_libre = buffers_libres.primer_buffer;
                        buffers_libres.primer_buffer = delayed[i];
                        delayed[i]->anterior_libre = (Buffer_header*) &buffers_libres;
                }

                //Se apaga su estado "delayed write"
                (delayed[i]->estado) &= 0xfb ;// 1111 1011
        }
}

/*

        pre-condicion : el buffer pasado debe estar bloqueado
        post-condicion: el buffer se coloca al final de la lista de buffers libres (y se deja en estado "libre")

*/
void BufferCache :: liberar_buffer(Buffer_header *buff_bloqueado){

        Buffer_header *ptr_ultimo_aux;

        if (buff_bloqueado->estado & OCUPADO)
        {

                despertar_procesos();

                //eL buffer debe ubicarse al final de la free list para mantener la politica de reemplazo LRU

                if(buffers_libres.ultimo_buffer != NULL)
                {
                        ptr_ultimo_aux = buffers_libres.ultimo_buffer;
                        buffers_libres.ultimo_buffer = buff_bloqueado;
                        ptr_ultimo_aux->siguiente_libre = buff_bloqueado;

                        buff_bloqueado->anterior_libre = ptr_ultimo_aux;
                        buff_bloqueado->siguiente_libre = (Buffer_header*) &buffers_libres;
                }else{

                        //la lista estaba vacia
                        buffers_libres.primer_buffer = buffers_libres.ultimo_buffer = buff_bloqueado;
                        buff_bloqueado->anterior_libre = (Buffer_header*) &buffers_libres;
                        buff_bloqueado->siguiente_libre = (Buffer_header*) &buffers_libres;
                }

                //se desbloquea el buffer

                (buff_bloqueado->estado) &=  0xfe; //1111 1110

        }
}

/*
        post-condicion: deja en "buff_para_usar" un buffer en estado "ocupado", listo para usarse por el proceso actual a traves de
                                los metodos publicos: leer() y escribir()

*/
void BufferCache :: asignar_bloque(int nro_bloque, Buffer_header *buff_para_usar)
{
        bool buffer_no_encontrado = true;
        bool bloque_no_encontrado = true;
        Buffer_header *ptr_buff_recorrido = buffers.primer_buffer;

        while (buffer_no_encontrado)
        {
                //Se busca el bloque en la lista de buffers (hash queue en el original)

                for(int i= 0; (i < CANT_BUFFERS) && bloque_no_encontrado; i++)
                {
                        if ( (ptr_buff_recorrido->numero_bloque) == nro_bloque )
                        {
                                buff_para_usar = ptr_buff_recorrido; //Se guarda la cabecera
                                bloque_no_encontrado = false;
                        }

                        ptr_buff_recorrido = ptr_buff_recorrido->siguiente_buffer;
                }

                if( ! bloque_no_encontrado )
                {
                        if( buff_para_usar->estado & OCUPADO)
                        {
                                //Si no esta en la free list
                                dormir_proceso(); //despertar cuando se libere EL BUFFER ENCONTRADO
                                continue;
                        }
                        //Se enciende el estado ocupado
                        (buff_para_usar->estado) |= 0x01 ;//0000 0001

                        //Se remueve el buffer de la free list, ya que estará ocupado por el proceso actual
                        remover_buffer_libre(buff_para_usar);

                        return;

                }else{
                        /**No se encontro el bloque en la lista de datos (porque nunca se uso)**/

                        if ( buffers_libres.primer_buffer == NULL)
                        {
                                //La lista de buffers libres esta vacia
                                dormir_proceso(); //despertar cuando se libere CUALQUIERA
                                continue;
                        }

                        //Se remueve el primer buffer de la lista para REUSARSE (politica de reemplazo LRU)
                        remover_primer_buffer(buff_para_usar);

                        if ( (buff_para_usar->estado) & DELAYED_WRITE)
                        {
                                /**Escribir en disco: "buff_para_usar->datos" **/

                                delayed[cant_diferidos] = buff_para_usar; //se suma al arreglo de buffers que deben ser escritos
                                cant_diferidos ++;
                                continue;
                        }

                        buff_para_usar->numero_bloque = nro_bloque;
                        //Se enciende el estado ocupado
                        (buff_para_usar->estado) |= 0x01 ;//0000 0001

                        //Se apaga el estado "valido"
                        (buff_para_usar->estado) &= 0xFD;// 1111 1101

                        return;
                }
        }

}


/*
        Coloca en "datos" la informacion corresponiendte al bloque "nro_bloque"

        pre-condicion : tam_datos debe ser menor o igual que TAM_BUFFER
*/
void BufferCache :: leer(int nro_bloque, char *datos, unsigned tam_datos)
{
        Buffer_header *buff;
        asignar_bloque(nro_bloque, buff); //obtengo un buffer para el bloque pedido
        if ( (buff->estado) & DATOS_VALIDOS )
        {
                //Se aprovecha el buffer cache, no hay necesidad de leer del disco
                memcpy(datos, buff->datos, tam_datos);
        }else{
                //Hay que leer los datos del disco (segun el nro_bloque) y ponerlos en buff->datos

                //usar nro_bloque como Referencia para leer de un componente hacia "buff->datos"

                memcpy(datos, buff->datos, tam_datos);
                //Se enciende el estado: "datos validos"
                (buff->estado) |= 0x02;//0000 0010
        }

        if( cant_diferidos)
                manejar_diferidos();

        liberar_buffer(buff);
}

void BufferCache :: escribir(int nro_bloque, char *datos, unsigned tam_datos)
{
        Buffer_header *buff;
        asignar_bloque(nro_bloque, buff); //obtengo un buffer para el bloque pedido


        memcpy(buff->datos, datos, tam_datos); //se escribe en el buffer, y se posterga la escritura al disco
        //Se enciende el estado delayed write
        (buff->estado) |= 0x04; //0000 0100

        if (! ( (buff->estado) & DATOS_VALIDOS ))
        {
                //Se enciende el estado "datos validos"
                (buff->estado) |= 0x02;//0000 0010
        }

        if( cant_diferidos)
                manejar_diferidos();

        liberar_buffer(buff);
}

