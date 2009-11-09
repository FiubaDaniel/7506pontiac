#ifndef _HASHINGEXT_HPP_
#define _HASHINGEXT_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "../ArbolBSharp/Clave.h"
#include "EstrategiaIndice.h"
#include "../ArbolBSharp/ComparadorClave.h"

#define incrementar30(cant_Reg, cant_esperado) (cant_esperado) = (int)(cant_Reg)*(100.0/70.0) + 1 //Se suponen un 30% m�s de registros de carga

#define BUSCAR_PARA_ELIMINAR 2
#define BUSCAR_PARA_RECUPERAR 1
#define BUSCAR_PARA_MODIFICAR 0

#define REF_NULA -1


typedef unsigned Ttam_disp; //tipo de dato del numero que representa el tama�o de dispersi�n de un cubo
typedef unsigned Tcant_reg; // tipo de dato para el numero que representa la cantidad de registros en un cubo

/*
   .Esta clase implementa un �ndice con organizaci�n directa extensible modular
   .La clase " Recurso "podr� hacer uso de las primitivas implementadas para esta organizaci�n

   */

class HashingExt: public EstrategiaIndice
{
        private:

                /**************************         Atributos            ***************************/

                std:: fstream tabla_dispersion; //contiene referencias a cubos de registros de �ndices
                std:: fstream cubos; //contiene cubos con tam_cubo registros (clave, referencia)
                std:: fstream cubos_libres; //contiene las referencias a los cubos vac�os y reusables en caso de colisi�n
                std:: fstream persist; //contiene los atributos del objeto para cuando se llame a ABRIR() en lugar de CREAR()

                //Nombres de los archivos usados
                std::string nom_tabla;
                std::string nom_cubos;
                std::string nom_libres;
                std::string nom_persist;

                unsigned bytes_cubo; //cantidad de bytes que ocupa un cubo
                unsigned tam_cubo; //cantidad de registros(de longitud variable) que entran en cada Bucket
                unsigned tam_tabla; //tama�o actual de la tabla de dispersi�n (cantidad de referencias contenidas)
                unsigned tam_clave_ref; //suma de tam_clave + sizeof(Referencia)
                unsigned tam_clave; // tama�o total, suma de lo datos que componen la clave
                int contador_cubos; // cantidad de cubos creados -1 (se empiezan a numerar desde cero 0)
                unsigned cant_cubos_libres; //cantidad de cubos libres referenciados en el archivo "cubos_libres"
                unsigned cant_at_clave; //cantidad de atributos por clave

                /********************************** M�todos privados  ****************************/

                /*
                   Almacena en *cant_Cubos la cantidad de cubos que se crear�n en la carga inicial (al llamarse a "crear()")

                */
                void calcular_cantidad_cubos(const unsigned tam_clave_ref, const unsigned tam_para_datos, const unsigned cant_reg_ini, unsigned *cant_cubos);

                /*
                        Recibe:
                                        ref_cubo: referencia del cubo en el cual se produjo el overflow
                                        pos_tabla: posici�n en la tabla en d�nde se encontr� la referencia al cubo

                        Si el tama�o de dispersi�n del cubo es igual al de la tabla, entonces �sta ultima se duplicar�, sino se usar�n
                        la mitad de las apariciones del cubo en la tabla para consignar las referencias del bucket nuevo

                        pre-condiciones: -ARCHIVOS DE TABLA Y CUBOS ABIERTOS
                */
                void resolver_sobreflujo(char *p_cubo, char *clave_mem, Referencia *referencia, const int ref_cubos, unsigned pos_tabla);

                /*Es la implementaci�n de la funci�n de hashing
                        Devuelve una posici�n dentro de la tabla de dispersi�n. "Funciona con la clave serializada"
                        Recibe :
                                        clave_mem : puntero al �rea de memoria que contiene los atributos que componen la clave
                */
                unsigned dispersar(char *clave_mem);

                /*
                        Crea un cubo contemplado en la carga inicial, es decir, cuando se encuentra una REFERENCIA_NULA en la tabla de dispersi�n.
                        Aclaraci�n: puede ser que alg�n cubo contemplado en "crear()", no se haya creado en dicho m�todo, y que se
                        tenga que crear como resultado de una inserci�n con "insertar()". Para estos casos, se usa este m�todo y NO crear_cubo()

                        -Este cubo se inicializar� con el tama�o de dispersi�n igual a la de la tabla, y con el primer registro cargado.
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

                        Devuelve false si no se puedo agregar el registro porque ya existía otro con = clave
                */
                bool agregar_registro(char *clave_mem, Referencia *referencia, const int ref_cubos, unsigned pos_tabla);

                /*
                        pre-condici�n: clave_mem debe apuntar a un �rea de memoria con espacio suficiente, es decir this->tam_clave
                */
                void serializar_clave(char *clave_mem, Clave *clave);

                /*
                        Pre-condici�n: -ARCHIVOS DE TABLA Y CUBOS ABIERTOS
                                                -clave_mem, contiene la clave en memoria
                        Devuelve true si se pudo agregar, false sino
                */
                bool resolver_insercion( char *clave_mem, Referencia referencia);

                /*
                        Crea un cubo vac�o, y lo escribe en el archivo de cubos
                        Recibe:
                                        tam_dispersion : el tama�o de dispersi�n del cubo a crear
                        Pre-condici�n:
                                             el atributo "contador_cubos" debe estar actualizado, es decir contabilizando el cubo
                                            que crear� este m�todo.
                */
                void crear_cubo_vacio(Ttam_disp tam_dispersion);

                /*
                        Incrementa (sumando "salto") en forma circular el contenido apuntado por "pos_tabla"
                        La "circulaci�n" se define por el tama�o de la tabla de dispersi�n actual (atributo del hashing)

                        (sirve para moverse en forma circular por la tabla de dispersi�n)
                */
                void inc_circular_a_saltos(unsigned *pos_tabla, int salto);

                void dec_circular_a_saltos(unsigned *pos_tabla, int salto);

                /*
                        Recibe:
                                        recuperar: seg�n se busque para recuperar la referencia de la clave,  para modificarla o para eliminarla deber� tener
                                        los valores: BUSCAR_PARA_RECUPERAR o BUSCAR_PARA_MODIFICAR o BUSCAR_PARA_ELIMINAR respectivamente
                */

                bool buscar_clave(Clave *clave, Referencia *referencia, unsigned recuperar);

        public:

                HashingExt(){};

                /*
                Realiza una carga inicial (NO SIGUE LA INTERFAZ ESTRATEGIAINDICE)

                Recibe:
                   tam_cubo: tama�o de los buckets (en bytes)
                   claves :   arreglo de claves iniciales
                   referencias: arreglo de referencias iniciales
                   cant_reg_ini  : la cantidad de registros iniciales a cargar  (pares  (clave,referencia))
                   nombre_arch: prefijo de los nombres de los archivos que se crearan al usar el �ndice

                   Devuelve true en caso de �xito, false sino
                 */
                bool crear_con_carga_inicial(std::string nombre_arch, unsigned int tam_cubo_bytes, Clave *claves, Referencia *referencias, unsigned cant_reg_ini);


                /*
                Recibe:
                        nombre_arch: prefijo de los nombres de los archivos que se crearan al usar el �ndice
                        tamanioBloque : tama�o del cubo en bytes
                        clave: para especificar las caracter�siticas de todas las claves (tama�o en bytes y cantidad de atributos)
                        El comparador de claves no tiene uso en este �ndice, sin embargo se incluy� para implementar el patr�n estrategia

                */
                void crear(std::string nombreArch, unsigned int tamanioBloque, Clave* clave, ComparadorClave* comp);

                /*
                        Abre los archivos de trabajo (cubo, cubos libres y tabla) existentes en el directorio de ejecuci�n
                        nombreArch: prefijo de los nombres de los archivos que se abriran para usar el �ndice
                        El comparador de claves no tiene uso en este �ndice, sin embargo se incluy� para implementar el patr�n estrategia
                Devuelve:
                        true si se pudieron abrir los archivos
                        false sino
                */
               bool abrir(std::string nombreArch,ComparadorClave* comp);


                /*
                        Agrega el par (clave, referencia) al cubo que corresponda seg�n:

                        dispersion(clave) ---> ref_en_tabla ---> arch_tabla ---> ref_cubo ---> arch_cubos
                */
               bool insertar(Referencia ref, Clave* clave);


                /*
                        Elimina el registro de �ndice correspondiente a la clave pasada por par�metro

                        Devuelve:
                                        false en caso de error (la clave no correspond�a a un registro de �ndice por ejemplo)
                                        true en caso de eliminaci�n exitosa
                */
                bool eliminar(const Clave* clave);


                /*
                        clave: clave cuya referencia asociada se quiere recuperar
                        referencia: contendr� la referencia de la unidad l�gica que contenga la clave

                        false si no se encontr� la clave
                        true en caso de �xito
                */
                bool BuscarReferencia(const Clave *clave, Referencia* referencia);

                /*Cambia la referencia de la clave
                //1 en caso de error, 0 sino
                */
              	bool modificar(const Clave *clave, Referencia refNueva);

                /*
                        Muestra el contenido actual de la tabla de dispersi�n, de todos los cubos y el archivo de cubos libres
                        Pre-condici�n : se debe haber llamado antes al m�todo crear() o abrir()
                */
              	void mostrarEstado();

                int tamanioBloque(){
                        return this->bytes_cubo;
                };

                void cerrar()
                {
                        this->tabla_dispersion.close();
                        this->cubos.close();
                        this->cubos_libres.close();
                        this->persist.close();
                };


                virtual ~HashingExt()
                {
                        this->tabla_dispersion.close();
                        this->cubos.close();
                        this->cubos_libres.close();
                        this->persist.close();
               };

	         //Referencia Siguiente(bool ultimo);
};


#endif
