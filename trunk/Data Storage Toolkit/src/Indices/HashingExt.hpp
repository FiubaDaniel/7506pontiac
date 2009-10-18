#ifndef _HASHINGEXT_HPP_
#define _HASHINGEXT_HPP_

#include <iostream>
#include <fstream>
#include "Clave.hpp"

#define incrementar30(cant_Reg) (cant_Reg) = (cant_Reg)*(100.0/70.0) + 1 //Se usa para obtener un 30% m�s de espacio por cubo


typedef struct TregIndice
{
        Clave clave;
        size_t referencia;
};

typedef struct Tcubo
{
        unsigned espacio_libre;
        unsigned tam_dispersi�n; // tama�o de la tabla de dispersi�n cuando se balance� por �ltima vez el cubo
        TregIndice reg;
};
/*
   .Esta clase implementa un �ndice con organizaci�n directa extensible modular
   .La clase " Recurso "podr� hacer uso de las primitivas implementadas para esta organizaci�n
   .Los cubos, la tabla de dispersi�n y los bloques libres son implementados como archivos de organizaci�n secuencial.


   */


class HashingExt
{
        private:

                fstream TablaDispersion; //contiene referencias a cubos de registros de �ndices
                fstream Cubos; //contiene cubos con tam_cubo registros (clave, referencia)
                fstream BloquesLibres; //contiene las referencias a los cubos vac�os y reusables en caso de colisi�n

                unsigned tam_Cubo; //cantidad de registros(de longitud variable) que tendr� cada Bucket (en los ejemplos de clase, vale 3)
                unsigned tam_tabla; //tama�o actual de la tabla de dispersi�n (cantidad de referencias contenidas)

                /*Almacena en *cant_Cubos la cantidad de cubos que se crear�n en la carga inicial (al llamarse el constructor)*/
                void calcular_Cantidad_Cubos(const Clave *reg_Iniciales, const int cant_Reg_Ini, int *cant_Cubos);

                /*ref_cubo: referencia del cubo en el cual se produjo la colisi�n

                        Si el tama�o de dispersi�n del cubo es igual al de la tabla, entonces esta ultima se duplicar�, sino se usar�n
                        la mitad de las apariciones del cubo en la tabla para consignar las referencias del bucket nuevo
                */
                void resolver_colision(unsigned ref_cubo);

                //Devuelve una posici�n dentro de la tabla de dispersi�n
                int dispersar(Clave clave);

        public:
                /*
                   tam_Cubo: tama�o de los buckets (3 por ejemplo)

                   reg_Iniciales: apunta a un conjunto de registros que deben estar inicializados y cargados con informaci�n
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
                        Elimina el registro de �ndice correspondiente a la clave pasada por par�metro

                        1 en caso de error (la clave no correspond�a a un registro de �ndice por ejemplo)
                        0 en caso de eliminaci�n exitosa
                */
                int eliminar(const Clave clave);


                /*
                        clave: clave del registro cuya referencia se quiere recuperar

                        1 si no se encontr� la clave
                        0 en caso de �xito
                */
                int buscar(const Clave clave, size_t &referencia);

                //Cambia la referencia de la clave
                //1 en caso de error, 0 sino
                int modificar(const Clave clave, const size_t referencia);

                ~HashingExt();
};


#endif
