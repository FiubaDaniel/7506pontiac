#ifndef _BUFFER_CACHE_HPP_
#define _BUFFER_CACHE_HPP_

#include <cstdlib>
#include <fstream>
#include "../Compuesto/Registro.h"
#include "../Almacenamiento/Almacenamiento.h"

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
                unsigned tam_datos;
                int cant_buffers;

                ofstream salida_comportamiento;  //archivo de texto que permite leer el comportamiento del buffer cache
                Buffer_header **delayed; //conjunto de buffers diferidos que hay que escribir en disco
                unsigned cant_diferidos; //cantidad de buffers "delayed write" que se encontraron al principio de la free list
                Almacenamiento *almacen; //para escribir/leer los datos hacia y desde el disco

                void liberar_buffer(Buffer_header *buff_bloqueado);
                bool asignar_bloque(int nro_bloque, Buffer_header **buff_para_usar);

                void remover_buffer_libre(Buffer_header *buff_para_usar);
                void dormir_proceso();
                void despertar_procesos();
                void remover_primer_buffer(Buffer_header **buff_para_reemplazar);
                void manejar_diferidos();

                /*Muestra el estado de los flags de "buffer" , asi como tambien el numero de bloque al que esta asociado*/
                void mostrar_estado(Buffer_header *buffer);

                /*Muestra la lista de buffers y la de buffers libres*/
                void mostrar_listas();

        public:

                BufferCache(Almacenamiento *almacen, unsigned tam_datos, int cant_buffers){

                        this->cant_buffers = cant_buffers;
                        delayed = new Buffer_header*[cant_buffers];
                        cant_diferidos =0;

                        string nom_arch = almacen->getNombre();
                        string nom_log = "buffer_cache_"+nom_arch+".txt";
                        this->salida_comportamiento.open(nom_log.c_str());
                        this->salida_comportamiento << "\t \t :::::::: Buffer Cache ::::::::: " << endl;

                        this->tam_datos = tam_datos;
                        this->almacen = almacen;


                        Buffer_header *ptr_anterior;
                        Buffer_header *ptr_anterior_libre;
                        Buffer_header *buf = new Buffer_header;

                        buffers.primer_buffer = buf;
                        buffers_libres.primer_buffer = buf;

                        buf->numero_bloque = -1;
                        buf->estado = 0;
                        buf->datos = new char[this->tam_datos];
                        buf->siguiente_buffer = NULL;
                        buf->anterior_buffer = (Buffer_header*) &buffers;
                        buf->siguiente_libre = NULL;
                        buf->anterior_libre = (Buffer_header*) &buffers_libres;

                        ptr_anterior = ptr_anterior_libre = buf;

                        for(int i=0; i < cant_buffers - 2 ; i++)
                        {
                                buf = new Buffer_header;

                                buf->numero_bloque = -1;
                                buf->estado = 0;
                                buf->datos = new char[this->tam_datos];
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
                        buf->datos = new char[this->tam_datos];
                        buf->siguiente_buffer = (Buffer_header*) &buffers;
                        buf->anterior_buffer = ptr_anterior;
                        buf->siguiente_libre = (Buffer_header*) &buffers_libres;
                        buf->anterior_libre = ptr_anterior_libre;

                        ptr_anterior->siguiente_buffer = ptr_anterior->siguiente_libre = buf;

                        buffers.ultimo_buffer = buf;
                        buffers_libres.ultimo_buffer = buf;

                };

                void leer(int nro_bloque, char* datos);

                void escribir(int nro_bloque, char *datos);

                ~BufferCache(){

                        this->salida_comportamiento.close();

                        //Punteros a buffers_headers
                        Buffer_header *ptr_buffer = buffers.primer_buffer;
                        Buffer_header *ptr_aux = NULL;


                        /** Si quedaron buffers sin escribirse en disco "delayed write", escribirlos **/

                        for(int j = 0; j < cant_buffers; j++)
                        {
                                if( ptr_buffer->estado & DELAYED_WRITE )
                                {
                                        //cant_diferidos empieza en cero
                                        delayed[cant_diferidos] = ptr_buffer;
                                        cant_diferidos++;
                                        ptr_buffer = (Buffer_header*) ptr_buffer->siguiente_buffer;
                                }

                        }
                        if( cant_diferidos)
                        {
                                manejar_diferidos();
                        }

                        ptr_buffer = buffers.primer_buffer;

                        for(int i=0; i < cant_buffers  ; i++)
                        {
                                delete [] ptr_buffer->datos;
                                ptr_aux = (Buffer_header*) ptr_buffer->siguiente_buffer;
                                delete ptr_buffer;

                                ptr_buffer = ptr_aux;
                        }

                        delete [] delayed;

                };

};


#endif

