#ifndef _HASHINGEXT_HPP_
#define _HASHINGEXT_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "Clave.h"
#include "EstrategiaIndice.h"
#include "ComparadorClave.h"

#define incrementar30(cant_Reg, cant_esperado) (cant_esperado) = (int)(cant_Reg)*(100.0/70.0) + 1 //Se suponen un 30% más de registros de carga

#define BUSCAR_PARA_ELIMINAR 2
#define BUSCAR_PARA_RECUPERAR 1
#define BUSCAR_PARA_MODIFICAR 0

#define REF_NULA -1


typedef unsigned Ttam_disp; //tipo de dato del numero que representa el tamaño de dispersión de un cubo
typedef unsigned Tcant_reg; // tipo de dato para el numero que representa la cantidad de registros en un cubo

/*
   .Esta clase implementa un índice con organización directa extensible modular
   .La clase " Recurso "podrá hacer uso de las primitivas implementadas para esta organización

   */

class HashingExt: public EstrategiaIndice
{
        private:

                /**************************         Atributos            ***************************/

                std:: fstream tabla_dispersion; //contiene referencias a cubos de registros de índices
                std:: fstream cubos; //contiene cubos con tam_cubo registros (clave, referencia)
                std:: fstream cubos_libres; //contiene las referencias a los cubos vacíos y reusables en caso de colisión
                std:: fstream persist; //contiene los atributos del objeto para cuando se llame a ABRIR() en lugar de CREAR()

                //Nombres de los archivos usados
                std::string nom_tabla;
                std::string nom_cubos;
                std::string nom_libres;
                std::string nom_persist;

                unsigned bytes_cubo; //cantidad de bytes que ocupa un cubo
                unsigned tam_cubo; //cantidad de registros(de longitud variable) que entran en cada Bucket
                unsigned tam_tabla; //tamaño actual de la tabla de dispersión (cantidad de referencias contenidas)
                unsigned tam_clave_ref; //suma de tam_clave + sizeof(Referencia)
                unsigned tam_clave; // tamaño total, suma de lo datos que componen la clave
                int contador_cubos; // cantidad de cubos creados -1 (se empiezan a numerar desde cero 0)
                unsigned cant_cubos_libres; //cantidad de cubos libres referenciados en el archivo "cubos_libres"
                unsigned cant_at_clave; //cantidad de atributos por clave

                /********************************** Métodos privados  ****************************/

                /*
                   Almacena en *cant_Cubos la cantidad de cubos que se crearán en la carga inicial (al llamarse a "crear()")

                */
                void calcular_cantidad_cubos(const unsigned tam_clave_ref, const unsigned tam_para_datos, const unsigned cant_reg_ini, unsigned *cant_cubos);

                /*
                        Recibe:
                                        ref_cubo: referencia del cubo en el cual se produjo el overflow
                                        pos_tabla: posición en la tabla en dónde se encontró la referencia al cubo

                        Si el tamaño de dispersión del cubo es igual al de la tabla, entonces ésta ultima se duplicará, sino se usarán
                        la mitad de las apariciones del cubo en la tabla para consignar las referencias del bucket nuevo

                        pre-condiciones: -ARCHIVOS DE TABLA Y CUBOS ABIERTOS
                */
                void resolver_sobreflujo(char *p_cubo, char *clave_mem, Referencia *referencia, const int ref_cubos, unsigned pos_tabla);

                /*Es la implementación de la función de hashing
                        Devuelve una posición dentro de la tabla de dispersión. "Funciona con la clave serializada"
                        Recibe :
                                        clave_mem : puntero al área de memoria que contiene los atributos que componen la clave
                */
                unsigned dispersar(char *clave_mem);

                /*
                        Crea un cubo contemplado en la carga inicial, es decir, cuando se encuentra una REFERENCIA_NULA en la tabla de dispersión.
                        Aclaración: puede ser que algún cubo contemplado en "crear()", no se haya creado en dicho método, y que se
                        tenga que crear como resultado de una inserción con "insertar()". Para estos casos, se usa este método y NO crear_cubo()

                        -Este cubo se inicializará con el tamaño de dispersión igual a la de la tabla, y con el primer registro cargado.
                        -Incrementa el contador de cubos this->contador_cubos


                */
                void crear_cubo_ini(char* clave_mem, Referencia *referencia);

                /*
                        Agrega el par (clave, referencia) en el cubo que se encuentra en la posición "ref_cubos" del archivo de cubos
                        Si el cubo referenciado está lleno, entonces se delega la resolución del overflow al método "resolver_sobreflujo()"
                        sino, se escribe el registro(clave, ref) en el cubo

                (Agrega en un cubo existente)
                Recibe:
                                -ref_cubos: es la referencia al cubo en el archivo de cubos
                                -pos_tabla: la posición en la tabla en dónde se encontró la referencia al cubo
                pre-condiciones:
                                        -tabla_dispersion y cubos, deben estar abiertos
                                        -el cubo debe existir en el archivo de cubos
                */
                void agregar_registro(char *clave_mem, Referencia *referencia, const int ref_cubos, unsigned pos_tabla);


                /*
                        pre-condición: clave_mem debe apuntar a un área de memoria con espacio suficiente, es decir this->tam_clave
                */
                void serializar_clave(char *clave_mem, Clave *clave);

                /*
                        Pre-condición: -ARCHIVOS DE TABLA Y CUBOS ABIERTOS
                                                -clave_mem, contiene la clave en memoria
                */
                void resolver_insercion( char *clave_mem, Referencia referencia);

                /*
                        Crea un cubo vacío, y lo escribe en el archivo de cubos
                        Recibe:
                                        tam_dispersion : el tamaño de dispersión del cubo a crear
                        Pre-condición:
                                             el atributo "contador_cubos" debe estar actualizado, es decir contabilizando el cubo
                                            que creará este método.
                */
                void crear_cubo_vacio(Ttam_disp tam_dispersion);

                /*
                        Incrementa (sumando "salto") en forma circular el contenido apuntado por "pos_tabla"
                        La "circulación" se define por el tamaño de la tabla de dispersión actual (atributo del hashing)

                        (sirve para moverse en forma circular por la tabla de dispersión)
                */
                void inc_circular_a_saltos(unsigned *pos_tabla, int salto);

                void dec_circular_a_saltos(unsigned *pos_tabla, int salto);

                /*
                        Recibe:
                                        recuperar: según se busque para recuperar la referencia de la clave pasado o para modificarla deberá tener
                                        los valores: BUSCAR_PARA_RECUPERAR o BUSCAR_PARA_MODIFICAR respectivamente
                */

                bool buscar_clave(Clave *clave, Referencia *referencia, unsigned recuperar);

        public:

                HashingExt(){};

                /*
                   tam_cubo: tamaño de los buckets (en bytes)
                   claves :   arreglo de claves iniciales
                   referencias: arreglo de referencias iniciales
                   cant_reg_ini  : la cantidad de registros iniciales a cargar  (pares  (clave,referencia))
                   nombre_arch: prefijo de los nombres de los archivos que se crearan al usar el índice

                   Devuelve true en caso de éxito, false sino
                 */
                bool crear(std::string nombre_arch, unsigned int tam_cubo_bytes, Clave *claves, Referencia *referencias, unsigned cant_reg_ini);

                /*
                        Abre los archivos de trabajo (cubo, cubos libres y tabla) existentes en el directorio de ejecución
                        nombreArch: prefijo de los nombres de los archivos que se abriran para usar el índice
                */
               void abrir(std::string nombre_arch);


                /*
                        Agrega el par (clave, referencia) al cubo que corresponda según:

                        dispersion(clave) ---> ref_en_tabla ---> arch_tabla ---> ref_cubo ---> arch_cubos
                */
               bool insertar(Referencia ref, Clave* clave);


                /*
                        Elimina el registro de índice correspondiente a la clave pasada por parámetro

                        Devuelve:
                                        false en caso de error (la clave no correspondía a un registro de índice por ejemplo)
                                        true en caso de eliminación exitosa
                */
                bool eliminar(Clave* clave);


                /*
                        clave: clave cuya referencia asociada se quiere recuperar
                        referencia: contendrá la referencia de la unidad lógica que contenga la clave

                        false si no se encontró la clave
                        true en caso de éxito
                */
                bool BuscarReferencia(Clave *clave, Referencia* referencia);

                /*Cambia la referencia de la clave
                //1 en caso de error, 0 sino
                */
              	bool modificar(Clave *clave, Referencia refNueva);

                /*
                        Muestra el contenido actual de la tabla de dispersión, de todos los cubos y el archivo de cubos libres
                        Pre-condición : se debe haber llamado antes al método crear() o abrir()
                */
              	void mostrarEstado();

                virtual ~HashingExt()
                {};

	         //Referencia Siguiente(bool ultimo);
};


#endif
