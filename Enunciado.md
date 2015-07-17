# Introduction #

Enunciado

# Details #

Organización de Datos

Trabajo Práctico

2do Cuatrimestre 2009


Descripción:

Un Toolkit es un conjunto coherente de herramientas que, bajo una misma interfaz, provee de funcionalidades extensibles y versatiles. Un Data Storage Toolkit , es un sistema de tipo Toolkit que se especializa en resolver problemas de almacenamiento. En esta abstracción, se permite trabajar con diversos tipos de datos y diversos algoritmos que se les aplican para operar sobre ellos. El actual trabajo practico es un desarrollo de este tipo y se pretende brindar las bases del diseño para que los equipos implementen y expandan en las sucesivas entregas, construyendo (como ultima etapa) una aplicación que demuestre el potencial del trabajo realizado.


Administración del Proyecto:

+Se desarrollara en tres etapas, cuyas entregas estarán indicadas en el anexo del +enunciado.
+Grupos de Datos: 4 integrantes
+Lenguajes C/C++ (no JAVA)
SO: Linux.



Etapa 1: Herramientas de Base

Son las clases que definen el almacenamiento con las estrategias básicas de trabajo.
Se desarrollaran todas las clases indicadas en esta arquitectura. La funcionalidad a ofrecer sera:
> + Almacenamiento de Archivos de Texto
> + Almacenamiento de Archivos de Registros Fijos
> + Almacenamiento de Archivos de Registros variables en Bloques Fijos.
Búsqueda:
> + Búsqueda Secuencial de Datos
> + Búsqueda Indexada por Árbol B# (B Sharp)
> + Búsqueda Indexada por Hashing Extensible
Recursos:
> + Funcionalidad de Recurso con Escritura Directa (sin Buffer).
> + Funcionalidad de Recurso con Escritura Directa con Buffer.

Parámetros:

Archivos de Texto: Nombre de Archivo, Ruta destino.

Archivos de Registros Fijos: Nombre de Archivo, Ruta destino, Tamaño de Registro.

Archivos de Registros variables en Bloques Fijos: Nombre de Archivo, Ruta destino, Tamaño de Bloque.

Búsqueda Secuencial: Clave.

Búsqueda Indexada por Árbol B#: Tamaño de Nodo, Política de División de nodos (distribución en casos especiales), Clave.

Búsqueda por Hashing Extensible: Tamaño de Bloque, Clave.

Funcionalidad de Recurso con Escritura Directa (sin Buffer): Nombre de Archivo, Ruta destino, Datos correspondientes a lo que guardara.

Funcionalidad de Recurso con Escritura Directa (con Buffer): Nombre de Archivo, Ruta destino, Datos correspondientes a lo que guardara, Datos correspondientes al buffer (Cantiad de Bloques/Registros en Memoria).


Descripción de la Arquitectura del Sistema:
> + Compuesto: Clase que permite crear objetos que se componen por otros.
> + Almacenamiento: Clase que almacena un Compuesto en un medio.
> + RecursoDeAlmacenamiento: Clase que define una interfaz para el trabajo con un recurso de Almacenamiento.
> + FabricaDeRecursoDeAlmacenamiento: Clase para crear Recursos de Almacenamientos en forma particular (como describen los métodos propuestos) y en forma predefinida (a elaborar por el equipo).
> +     Componente: Clase que resuelve el mínimo almacenamiento de datos estructurados.
> + Registro: Unidad de almacenamiento unitario.
> + Bloque: Unidad de almacenamiento de Registros.
> + Buffer: Clase para implementar un almacenamiento en un Buffer de Memoria.
> + Archivo: Clase para implementar un almacenamiento en un Archivo de Disco.
> + EstrategiaAlmacenamiento: Clase que resuelve el algoritmo a aplicar sobre los componentes de datos manejados.
    1. EstrategiaAlmacenamientoTexto: Algoritmo para manejo de datos en texto plano.
    1. EstrategiaAlmacenamientoRegistros:Algoritmo para el manejo de datos en Archivo de Registros (si se diferencia el algoritmo para otros tipos de componentes deberá extenderla).
    1. EstrategiaAlmacenamientoBloques: Algoritmo para el manejo de datos en Archivo de Bloques con Registros (si se diferencia el algoritmo para otros tipos de componentes deberá extenderla).
> + EstrategiaIndice:

A Definir por el Grupo.
> + EstrategiaReciurso: Clase que resuelve el algoritmo de la operatoria del Recurso de Almacenamiento.
    1. EstrategiaRecursoUnAlmacenamiento: Estrategia de almacenamiento con un solo recurso.
    1. EstrategiaRecursoEscrituraDirecta: Estrategia de almacenamiento con un Buffer y un Archivo, donde se aplica el algoritmo de Escritura Directa.
    1. EstrategiaRecursoEscrituraDiferida: Estrategia de almacenamiento con un buffer y un Archivo, donde se aplica el algoritmo de Escritura Diferida.