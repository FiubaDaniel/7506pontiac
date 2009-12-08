#ifndef _BUFFER_CACHE_HPP_
#define _BUFFER_CACHE_HPP_

#include <cstdlib>

//Buffers de 32 bytes
#define TAM_BUFFER 32

#define CANT_BUFFERS 100

//Se definen las máscaras de la variable "estado" de cada buffer
#define OCUPADO 1
#define DATOS_VALIDOS 2
#define DELAYED_WRITE 4
#define LEYENDO_ESCRIBIENDO 8
#define PROCESO_EN_ESPERA 16


//Estructura de los datos de control de los buffers
typedef struct Buffer_header{

        int numero_bloque;
        char estado;
        char *datos; //apunta a una region de TAM_BUFFER bytes en el HEAP
        Buffer_header *siguiente_buffer;
        Buffer_header *anterior_buffer;
        Buffer_header *siguiente_libre;
        Buffer_header *anterior_libre;

}Buffer_header;

typedef struct Lista_libres{

        Buffer_header *primer_buffer;
        Buffer_header *ultimo_buffer;
};

typedef struct Lista_buffers{

        Buffer_header *primer_buffer;
        Buffer_header *ultimo_buffer;
};

class BufferCache{

        private:

                Lista_buffers buffers;
                Lista_libres buffers_libres;

                Buffer_header **delayed;
                unsigned cant_diferidos; //cantidad de buffers "delayed write" que se encontraron al principio de la free list

                void liberar_buffer(Buffer_header *buff_bloqueado);
                void asignar_bloque(int nro_bloque, Buffer_header *buff_para_usar);

                void remover_buffer_libre(Buffer_header *buff_para_usar);
                void dormir_proceso();
                void despertar_procesos();
                void remover_primer_buffer(Buffer_header *buff_para_reemplazar);
                void manejar_diferidos();

        public:

                BufferCache(){

                        delayed = new Buffer_header*[CANT_BUFFERS];
                        cant_diferidos =0;

                        Buffer_header *ptr_anterior;
                        Buffer_header *ptr_anterior_libre;
                        Buffer_header *buf = new Buffer_header;

                        buffers.primer_buffer = buf;
                        buffers_libres.primer_buffer = buf;

                        buf->numero_bloque = -1;
                        buf->estado = 0;
                        buf->datos = new char[TAM_BUFFER];
                        buf->siguiente_buffer = NULL;
                        buf->anterior_buffer = (Buffer_header*) &buffers;
                        buf->siguiente_libre = NULL;
                        buf->anterior_libre = (Buffer_header*) &buffers_libres;

                        ptr_anterior = ptr_anterior_libre = buf;

                        for(int i=0; i < CANT_BUFFERS - 2 ; i++)
                        {
                                buf = new Buffer_header;

                                buf->numero_bloque = -1;
                                buf->estado = 0;
                                buf->datos = new char[TAM_BUFFER];
                                buf->siguiente_buffer = NULL;
                                buf->anterior_buffer = ptr_anterior;
                                buf->siguiente_libre = NULL;
                                buf->anterior_libre = ptr_anterior_libre;

                                ptr_anterior->siguiente_buffer = ptr_anterior->siguiente_libre = buf;
                                ptr_anterior = ptr_anterior_libre = buf;

                        }


                        //Ultimo buffer
                        buf = new Buffer_header;

                        buf->numero_bloque = -1;
                        buf->estado = 0;
                        buf->datos = new char[TAM_BUFFER];
                        buf->siguiente_buffer = (Buffer_header*) &buffers;
                        buf->anterior_buffer = ptr_anterior;
                        buf->siguiente_libre = (Buffer_header*) &buffers_libres;
                        buf->anterior_libre = ptr_anterior_libre;

                        buffers.ultimo_buffer = buf;
                        buffers_libres.ultimo_buffer = buf;

                };

                void leer(int nro_bloque, char *datos, unsigned tam_datos);

                void escribir(int nro_bloque, char* datos);

                ~BufferCache(){

                        //Punteros a buffers_headers
                        Buffer_header *ptr_buffer = buffers.primer_buffer;
                        Buffer_header *ptr_aux = NULL;

                        for(int i=0; i < CANT_BUFFERS  ; i++)
                        {
                                delete [] ptr_buffer->datos;
                                ptr_aux = (Buffer_header*) ptr_buffer->siguiente_buffer;
                                delete ptr_buffer;

                                ptr_buffer = ptr_aux;
                        }

                        delete [] delayed;

                        //Se escriben los buffers que hayan quedado en el estado "delayed write"

                };

};


#endif
