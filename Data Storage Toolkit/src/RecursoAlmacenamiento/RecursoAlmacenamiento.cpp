#include "RecursoAlmacenamiento.h"

// Constructors/Destructors
//  

RecursoAlmacenamiento::RecursoAlmacenamiento ( ) {
}

RecursoAlmacenamiento::~RecursoAlmacenamiento ( ) { }

/*
 * Delega la inicializacion segun la estrategia recurso.
 * Referencia a archivo/buffer y los dimensiona en función de la relación
 * Compuesto/Componente.
 * Si el file no existe, lo crea, si existe solo lo abre para RW.
 */

bool RecursoAlmacenamiento::inicializar() {

}

/*
 * Agrega registros utilizando el criterio utilizado.
 * Escritura (Archivo/Buffer), solo lo agrega en el almacenamiento.
 * EscrituraDirecta  (Archivo/Buffer), lo agrega en ambos lugares al mismo tiempo
 * secuencialmente.
 * EscrituraDiferida  (Archivo/Buffer), lo agrega en el buffer y espera tener
 * un porcentaje de ocupación para escribirlo en disco.
 */

bool RecursoAlmacenamiento::agregar() {

}

/*
 * Elimina registros utilizando el criterio utilizado.
 *
 * Verifica si existe el registro utilizando el indice, o secuencialmente en el almacenamiento
 *
 * En caso afirmativo:
 *
 * Escritura (Archivo/Buffer), solo lo borra del almacenamiento.
 * EscrituraDirecta  (Archivo/Buffer), levanta el componente, hace la operacion
 * necesaria y lo baja a disco.
 * EscrituraDiferida  (Archivo/Buffer), levanta el componente, lo borra en memoria y
 * lo marca para bajar a disco.
 */

bool RecursoAlmacenamiento::eliminar() {

}

/*
 * Modifica registros utilizando el criterio utilizado.
 *
 * Verifica si existe el registro utilizando el indice, o secuencialmente en el almacenamiento
 *
 * En caso afirmativo:
 *
 * Escritura (Archivo/Buffer), solo lo modifica.
 * EscrituraDirecta  (Archivo/Buffer), levanta el componente al buffer, hace la busqueda
 * secuencial, lo modifica en memoria y baja el bloque a disco.
 * EscrituraDiferida  (Archivo/Buffer), levanta el componente, lo modifica en memoria y
 * lo marca para bajar a disco.
 */

bool RecursoAlmacenamiento::modificar() {

}

/*
 * Busca el registro de acuerdo a la clave.
 *
 * Si existe indice busca en el, recibe como parametro referencia al disco y busca
 * secuencialmente en el disco.
 * De otra manera, busca secuencialmente en el archivo.
 *
 */

Componente RecursoAlmacenamiento::obtener() {

}

/*
 * Cierra el archivo
 *
 *
 * Escritura (Archivo/Buffer)
 * EscrituraDirecta  (Archivo/Buffer)
 * EscrituraDiferida  (Archivo/Buffer), en este caso verifica que no haya sectores
 * del buffer sin escribir, en tal caso los baja a disco y cierra.
 */

void RecursoAlmacenamiento::cerrarArchivo() {

}


void RecursoAlmacenamiento::mostrarContenidoBuffer() {

}

void RecursoAlmacenamiento::mostrarIndice() {

}

void RecursoAlmacenamiento::mostrarDatos() {

}

