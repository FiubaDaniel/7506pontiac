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
 void BufferCache :: remover_primer_buffer(Buffer_header **buff_para_reemplazar)
 {

         (*buff_para_reemplazar)  = buffers_libres.primer_buffer;

        if (buffers_libres.primer_buffer == buffers_libres.ultimo_buffer)
        {
                //Significa que la lista de buffers libres tenia un solo buffer
                buffers_libres.primer_buffer = buffers_libres.ultimo_buffer = NULL;
        }else{
                buffers_libres.primer_buffer = (*buff_para_reemplazar)->siguiente_libre;
                (*buff_para_reemplazar)->siguiente_libre->anterior_libre = (Buffer_header*) &buffers_libres;
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
        int total_diferidos = cant_diferidos;

        for(int i=0; i < total_diferidos; i++)
        {


                //Se escribe en disco un bloque o un registro segun corresponda

                almacen->posicionarComponente( delayed[i]->numero_bloque);
                almacen->escribir(delayed[i]->datos,tam_datos);

                //Se pone al principio de la lista de libres
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

        cant_diferidos = 0;
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
        post-condicion: - deja en "buff_para_usar" un buffer en estado "ocupado", listo para usarse por el proceso actual a traves de
                                los metodos publicos: leer() y escribir()
                                - devuelve false en caso de que no pueda devolver un buffer debido a que TODOS estaban esperando para escribirse
                                true sino

*/
bool BufferCache :: asignar_bloque(int nro_bloque, Buffer_header **buff_para_usar)
{
        bool buffer_no_encontrado = true;
        bool bloque_no_encontrado = true;
        bool all_delayed = false; //true si TODOS estan en estado "delayed write"
        Buffer_header *ptr_buff_recorrido = buffers.primer_buffer;


        while (buffer_no_encontrado)
        {
                //Se busca el bloque en la lista de buffers (hash queue en el original)

                for(int i= 0; ( (i < CANT_BUFFERS) && (bloque_no_encontrado) ); i++)
                {
                        if ( (ptr_buff_recorrido->numero_bloque) == nro_bloque )
                        {
                                *buff_para_usar = ptr_buff_recorrido; //Se guarda la cabecera
                                bloque_no_encontrado = false;
                        }

                        ptr_buff_recorrido = (Buffer_header*) (ptr_buff_recorrido->siguiente_buffer);
                }

                if( ! bloque_no_encontrado )
                {

                        if( (*buff_para_usar)->estado & OCUPADO)
                        {
                                //Si no esta en la free list
                                dormir_proceso(); //despertar cuando se libere EL BUFFER ENCONTRADO
                                continue;
                        }
                        //Se enciende el estado ocupado
                        ( (*buff_para_usar)->estado) |= 0x01 ;//0000 0001

                        //Se remueve el buffer de la free list, ya que estará ocupado por el proceso actual
                        remover_buffer_libre(*buff_para_usar);

                        return true;

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

                        while( ( ( (*buff_para_usar)->estado) & DELAYED_WRITE) && (!all_delayed) )
                        {

                                delayed[cant_diferidos] = *buff_para_usar; //se suma al arreglo de buffers que deben ser escritos
                                cant_diferidos ++;
                                remover_primer_buffer(buff_para_usar);

                                if( (buffers_libres.primer_buffer==NULL)&& ( ( (*buff_para_usar)->estado) & DELAYED_WRITE) )
                                {
                                        /**Se vacio la lista de buffers libres porque todos estaban en "delayed write"**/
                                        delayed[cant_diferidos] = *buff_para_usar; //se suma al arreglo de buffers que deben ser escritos
                                        cant_diferidos ++;
                                        all_delayed = true;
                                }
                        }

                        if(all_delayed)
                        {
                                //No se puede devolver un buffer hasta que se hayan escrito en disco
                                return false;
                        }


                        (*buff_para_usar)->numero_bloque = nro_bloque;
                        //Se enciende el estado ocupado
                        ( (*buff_para_usar)->estado) |= 0x01 ;//0000 0001

                        //Se apaga el estado "valido"
                        ( (*buff_para_usar)->estado) &= 0xFD;// 1111 1101

                        return true;
                }
        }
}

/*

*/
void BufferCache :: leer(int nro_bloque, char *datos)
{
        Buffer_header *buff;

        //se obtiene un buffer para el bloque pedido
        if ( !asignar_bloque(nro_bloque, &buff))
        {
                //Se tienen que manejar los buffers diferidos antes de poder conseguir uno para usar
                manejar_diferidos();
                asignar_bloque(nro_bloque, &buff);
        }

        if ( (buff->estado) & DATOS_VALIDOS )
        {
                //Se aprovecha el buffer cache, no hay necesidad de leer del disco
                memcpy(datos, buff->datos, tam_datos);

        }else{
                //Hay que leer los datos (registro o bloque) desde el disco (segun el nro_bloque) y ponerlos en buff->datos

                //ESCRIBIR EN EL BUFFER Y EN EL REGISTRO

                almacen->posicionarComponente(nro_bloque);// nro_bloque==nro_buffer
                almacen->leer(buff->datos,tam_datos);//Se escribe en el buffer para que futuros accesos den "hit"
                memcpy(datos, buff->datos, tam_datos);
                //Se enciende el estado: "datos validos"
                (buff->estado) |= 0x02;//0000 0010
        }

        if( cant_diferidos)
                manejar_diferidos();

        liberar_buffer(buff);
}

void BufferCache :: escribir(int nro_bloque, char *datos)
{
        Buffer_header *buff;

        //obtengo un buffer para el bloque pedido
        if( ! asignar_bloque(nro_bloque, &buff) )
        {

                //Se tienen que manejar los buffers diferidos antes de poder conseguir uno para usar
                manejar_diferidos();
                asignar_bloque(nro_bloque, &buff);
        }

        memcpy(buff->datos, datos, tam_datos); //se escribe en el buffer, y se posterga la escritura al disco
        //Se enciende el estado delayed write
        (buff->estado) |= 0x04; //0000 0100

        if (! ( (buff->estado) & DATOS_VALIDOS ))
        {
                //Se enciende el estado "datos validos"
                (buff->estado) |= 0x02;//0000 0010
        }

        if(cant_diferidos)
        {
                /**En caso de que se hayan encontrado buffers "delayed write" en la free list durante la busqueda del bloque**/
                manejar_diferidos();
        }

        liberar_buffer(buff);
}

