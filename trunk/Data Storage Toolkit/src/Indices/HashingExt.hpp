#ifndef _HASHINGEXT_HPP_
#define _HASHINGEXT_HPP_

#include <iostream>
#include <fstream>
#include "Clave.hpp"

#define incrementar30(cant_Reg) (cant_Reg) = (cant_Reg)*(100.0/70.0) + 1 //Se usa para obtener un 30% más de espacio por cubo


typedef struct TregIndice
{
        Clave clave;
        size_t referencia;
};

typedef struct Tcubo
{
        unsigned espacio_libre;
        unsigned tam_dispersión; // tamaño de la tabla de dispersión cuando se balanceó por última vez el cubo
        TregIndice reg;
};
/*
   .Esta clase implementa un índice con organización directa extensible modular
   .La clase " Recurso "podrá hacer uso de las primitivas implementadas para esta organización
   .Los cubos, la tabla de dispersión y los bloques libres son implementados como archivos de organización secuencial.


   */


class HashingExt
{
        private:

                fstream TablaDispersion; //contiene referencias a cubos de registros de índices
                fstream Cubos; //contiene cubos con tam_cubo registros (clave, referencia)
                fstream BloquesLibres; //contiene las referencias a los cubos vacíos y reusables en caso de colisión

                unsigned tam_Cubo; //cantidad de registros(de longitud variable) que tendrá cada Bucket (en los ejemplos de clase, vale 3)
                unsigned tam_tabla; //tamaño actual de la tabla de dispersión (cantidad de referencias contenidas)

                /*Almacena en *cant_Cubos la cantidad de cubos que se crearán en la carga inicial (al llamarse el constructor)*/
                void calcular_Cantidad_Cubos(const Clave *reg_Iniciales, const int cant_Reg_Ini, int *cant_Cubos);

                /*ref_cubo: referencia del cubo en el cual se produjo la colisión

                        Si el tamaño de dispersión del cubo es igual al de la tabla, entonces esta ultima se duplicará, sino se usarán
                        la mitad de las apariciones del cubo en la tabla para consignar las referencias del bucket nuevo
                */
                void resolver_colision(unsigned ref_cubo);

                //Devuelve una posición dentro de la tabla de dispersión
                int dispersar(Clave clave);

        public:
                /*
                   tam_Cubo: tamaño de los buckets (3 por ejemplo)

                   reg_Iniciales: apunta a un conjunto de registros que deben estar inicializados y cargados con información
                                         del usuario del toolkit
                   claves :   arreglo de claves iniciales
                   referencias: arreglo de referencias iniciales

                   cant_Reg_Ini  : la cantidad de registros iniciales a cargar  (pares  (clave,referencia))
                 */
                HashingExt(int tam_Cubo, const Clave *claves, size_t *referencias, const int cant_Reg_Ini);

                /*
                        Agrega el par (clave, referencia)
                */
                int agregar(const Clave clave, const size_t referencia);


                /*
                        Elimina el registro de índice correspondiente a la clave pasada por parámetro

                        1 en caso de error (la clave no correspondía a un registro de índice por ejemplo)
                        0 en caso de eliminación exitosa
                */
                int eliminar(const Clave clave);


                /*
                        clave: clave del registro cuya referencia se quiere recuperar

                        1 si no se encontró la clave
                        0 en caso de éxito
                */
                int buscar(const Clave clave, size_t &referencia);

                //Cambia la referencia de la clave
                //1 en caso de error, 0 sino
                int modificar(const Clave clave, const size_t referencia);

                ~HashingExt();
};


#endif
