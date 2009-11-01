#include "HashingExt.hpp"

using namespace std;

/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/***************************************************    Métodos privados   *******************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/

void HashingExt::calcular_cantidad_cubos(const unsigned tam_clave_ref, const unsigned tam_para_datos, const unsigned cant_reg_ini, unsigned *cant_cubos)
{
        int cant_esperado;
        /*
           Se supone un 30% más de registros para evitar una mayor cantidad de sobreflujos
        */
        incrementar30(cant_reg_ini, cant_esperado);

        (*cant_cubos)  = (int) ( cant_esperado * tam_clave_ref / tam_para_datos ) + 1; //se suma 1 para redondear hacia arriba


}
unsigned HashingExt::dispersar(char *clave_mem)
{
        /*
                Se enmascaran los bits menos significativos (sufijos) de la concatenación de los componentes de la clave
                En este caso los que entran en un entero sin signo, es decir 32 bits
                Luego la operación de módulo, regresará los bits menos significativos asociados con el tamaño actual de la tabla de dispersión
        */
        unsigned ref_en_tabla=0; //valor de dispersión
        unsigned numero_a_dispersar;
        unsigned offset = this->tam_clave - sizeof(numero_a_dispersar);

        memcpy(&numero_a_dispersar, clave_mem  + offset, sizeof(numero_a_dispersar));

        ref_en_tabla = numero_a_dispersar % (this->tam_tabla);

        return ref_en_tabla;
}

void HashingExt::crear_cubo_ini(char* clave_mem, Referencia *referencia)
{
        char *p_cubo = new char[this->bytes_cubo]; //memoria para el cubo completo que se va a agregar
        Tcant_reg cant_reg = 1;
        Ttam_disp tam_disp_cubo = this->tam_tabla;
        char *registro_indice = new char[this->tam_clave_ref]; // memoria para el par (clave, referencia)

        //Se carga registro_indice con los datos de la clave Y LUEGO el de la referencia asociada
        memcpy(registro_indice, clave_mem, this->tam_clave);
        memcpy(registro_indice + (this->tam_clave), referencia, sizeof(Referencia));

        /**        Se inicializa el cubo       **/
        //Primero se guardan los metadatos (cantidad de registros, tamaño de dispersión del cubo)
        memcpy(p_cubo, &cant_reg, sizeof(Tcant_reg) );
        memcpy(p_cubo + sizeof(Tcant_reg), &tam_disp_cubo, sizeof(Ttam_disp) );
        //Por último se carga el registro de índice que provocó la creación del cubo
        memcpy(p_cubo + sizeof(Tcant_reg) + sizeof(Ttam_disp), registro_indice, this->tam_clave_ref);

        /**Se carga en el archivo de cubos**/
        this->cubos.seekp((this->contador_cubos)*(this->bytes_cubo), ios_base:: beg);
        this->cubos.write((char*) p_cubo, this->bytes_cubo);
        this->contador_cubos ++;

        delete []p_cubo;
        delete []registro_indice;
}

void HashingExt::crear_cubo_vacio(Ttam_disp tam_dispersion)
{
        char *p_cubo = new char[this->bytes_cubo]; //memoria para el cubo completo que se va a agregar
        Tcant_reg cant_reg = 0;
        Ttam_disp tam_disp_cubo = tam_dispersion;

        /**        Se inicializa el cubo       **/
        //Se guardan los metadatos (cantidad de registros, tamaño de dispersión del cubo)
        memcpy(p_cubo, &cant_reg, sizeof(Tcant_reg) );
        memcpy(p_cubo + sizeof(Tcant_reg), &tam_disp_cubo, sizeof(Ttam_disp) );

        /**Se carga en el archivo de cubos**/
        this->cubos.seekp((this->contador_cubos)*(this->bytes_cubo), ios_base:: beg);
        this->cubos.write((char*) p_cubo, this->bytes_cubo);

        delete []p_cubo;

}

void HashingExt::inc_circular_a_saltos(unsigned *pos_tabla, int salto)
{
        (*pos_tabla) += salto;
        if ( (*pos_tabla) > (this->tam_tabla-1))
        {
                (*pos_tabla) = (*pos_tabla) - (this->tam_tabla);
        }
}

void HashingExt::resolver_sobreflujo(char *p_cubo, char *clave_mem, Referencia *referencia, const int ref_cubos, unsigned pos_tabla)
{
        unsigned pos_recorrido=pos_tabla; //para recorrer circularmente la tabla de dispersión
        char *un_registro = new char[this->tam_clave_ref]; //variable auxiliar
        char *una_clave = new char[this->tam_clave]; //variable auxiliar
        Referencia una_ref; //variable auxiliar
        Tcant_reg cant_reg_nuevo = 0; //variable auxiliar
        int ref_nuevo; //referencia al cubo nuevo (reusado o no)
        int tam_disp_cubo; // tamaño de dispersión del cubo en dónde se produjo el overflow
        memcpy(&tam_disp_cubo, p_cubo + sizeof(Tcant_reg), sizeof(Ttam_disp) );

        /*************Se decide si hay que crear un cubo nuevo, o se puede reusar uno que se haya liberado**************/
        if(this->cant_cubos_libres)
        {
                this->cubos_libres.open(this->nom_libres.c_str() );
                this->cubos_libres.read( (char*) &ref_nuevo, sizeof(ref_nuevo) ) ;

                (this->cant_cubos_libres)--;
                if (this->cant_cubos_libres)
                {
                        //Se desplazan el resto de las referencias a cubos libres
                        char *resto_libres = new char[(this->cant_cubos_libres)*sizeof(int)];
                        this->cubos_libres.read((char*) resto_libres, (this->cant_cubos_libres)*sizeof(int) );
                        this->cubos_libres.seekp(0, ios_base:: beg);
                        this->cubos_libres.write((char*) resto_libres, (this->cant_cubos_libres)*sizeof(int) );
                        delete [] resto_libres;
                }

                this->cubos_libres.close();

        }else
        {
                //Hay que crear un cubo nuevo
                ref_nuevo = ++(this->contador_cubos);
        }
        //(Luego para saber si el cubo se creo o no, se comparará ref_nuevo con el contador_cubos)

        /*************************Se resuelve el overflow************************************/
        if (tam_disp_cubo == (this->tam_tabla) )
        {
                //El cubo desbordado aparece referenciado UNA SOLA VEZ en la tabla
                //Se duplica la tabla de dispersión, y se duplica el tamaño de dispersión del cubo desbordado
                char *contenido_tabla = new char[(this->tam_tabla) * sizeof(int)]; //para almacenar la tabla ACTUAL en memoria

                this->tabla_dispersion.seekg(0, ios_base::beg); //posicionarse al principio
                this->tabla_dispersion.read( (char*) contenido_tabla, (this->tam_tabla) * sizeof(int) ); //leer todo el contenido de la tabla

                this->tabla_dispersion.seekp(this->tam_tabla, ios_base::beg); // posicionarse al final
                this->tabla_dispersion.write( (char*) contenido_tabla, (this->tam_tabla) * sizeof(int) );

                (this->tam_tabla) *= 2; // se duplica el tamaño de dispersión de la tabla

                this->tabla_dispersion.seekp(pos_tabla * sizeof(int), ios_base::beg); //posicionarse en la referencia que dió overflow
                this->tabla_dispersion.write( (char*) &ref_nuevo, sizeof(ref_nuevo) ); //se escribe la nueva referencia

                //Se duplica el tamaño de dispersión del cubo desbordado
                tam_disp_cubo *= 2;
                memcpy( p_cubo + sizeof(Tcant_reg), &tam_disp_cubo, sizeof(Ttam_disp));
                //Se pone en cero la cantidad de registros del cubo desbordado
                memcpy(p_cubo, &cant_reg_nuevo, sizeof(Tcant_reg) );
                //Se escribe el cubo desbordado, con sus datos de control ACTUALIZADOS
                this->cubos.seekp(ref_cubos * (this->bytes_cubo), ios_base::beg);
                this->cubos.write( (char*)p_cubo, this->bytes_cubo);

                if (ref_nuevo == (this->contador_cubos) )
                {
                        //En caso de que no hayan habido cubos libres para reusar
                        this->crear_cubo_vacio(this->tam_tabla);
                }

                /**************************Se redispersan los registros*******************************/
                //Los del cubo desbordado (aún  están en p_cubo)
                for ( int i = 0; i < (this->tam_cubo); i++)
                {
                        memcpy(un_registro, p_cubo + sizeof(Tcant_reg) + sizeof(Ttam_disp) + i*(this->tam_clave_ref), this->tam_clave_ref);
                        memcpy(una_clave, un_registro, this->tam_clave);
                        memcpy(&una_ref, un_registro + this->tam_clave, sizeof(Referencia));
                       this->resolver_insercion( una_clave, una_ref);
                }
                //El registro que causo el overflow
                this->resolver_insercion(clave_mem, *referencia);


                delete []contenido_tabla;

        }else
        {
                //El cubo desbordado aparece más de una vez, y no se duplica la tabla

                //Se duplica el tamaño de dispersión del cubo desbordado
                tam_disp_cubo *= 2;
                memcpy( p_cubo + sizeof(Tcant_reg), &tam_disp_cubo, sizeof(Ttam_disp));
                //Se pone en cero la cantidad de registros del cubo desbordado
                memcpy(p_cubo, &cant_reg_nuevo, sizeof(Tcant_reg) );
                //Se escribe el cubo desbordado, con sus datos de control ACTUALIZADOS
                this->cubos.seekp(ref_cubos * (this->bytes_cubo), ios_base::beg);
                this->cubos.write( (char*)p_cubo, this->bytes_cubo);

                if (ref_nuevo == (this->contador_cubos) )
                {
                        //En caso de que no hayan habido cubos libres para reusar
                        this->crear_cubo_vacio(tam_disp_cubo ); //con el tamaño de dispersión (nuevo) del cubo desbordado
                }

                //Se recorre circularmente la tabla (haciendo saltos igual a tam_disp_cubo) escribiendo las referencias al nuevo cubo
                do
                {
                        this->tabla_dispersion.seekp(pos_recorrido * sizeof(int), ios_base::beg); //posicionarse en la tabla (la primera vez será la que dió el overflow)
                        this->tabla_dispersion.write( (char*) &ref_nuevo, sizeof(ref_nuevo) ); //se escribe la nueva referencia
                        this->inc_circular_a_saltos(&pos_recorrido, tam_disp_cubo);

                }while(pos_recorrido != pos_tabla);

                /**************************Se redispersan los registros*******************************/
                //Los del cubo desbordado (aún  están en p_cubo)
                for ( int i = 0; i < (this->tam_cubo); i++)
                {
                        memcpy(un_registro, p_cubo + sizeof(Tcant_reg) + sizeof(Ttam_disp) + i*(this->tam_clave_ref), this->tam_clave_ref);
                        memcpy(una_clave, un_registro, this->tam_clave);
                        memcpy(&una_ref, un_registro + this->tam_clave, sizeof(Referencia));
                       this->resolver_insercion( una_clave, una_ref);
                }
                //El registro que causo el overflow
                this->resolver_insercion(clave_mem, *referencia);

        }

        delete [] un_registro;
        delete [] una_clave;
}

void HashingExt::agregar_registro(char *clave_mem, Referencia *referencia, const int ref_cubos, unsigned pos_tabla)
{
        char *p_cubo = new char[this->bytes_cubo]; //memoria para el cubo completo
        int cant_regi; //cantidad de registros que contiene el cubo referenciado

        //Se lee el cubo que corresponde
        this->cubos.seekg(ref_cubos*(this->bytes_cubo), ios_base::beg );
        this->cubos.read( (char*) p_cubo, this->bytes_cubo);

        //Se interpreta la cantidad de registros que contiene el cubo
        memcpy(&cant_regi, p_cubo, sizeof(Tcant_reg) );

        if ( cant_regi == (this->tam_cubo) )
        {
                /**************************No hay espacio suficiente para insertar el registro ***************************/
                this->resolver_sobreflujo(p_cubo, clave_mem, referencia, ref_cubos, pos_tabla);
        }else
        {
                /**************************Hay espacio suficiente para insertar un registro*******************************/
                char *registro_indice = new char[this->tam_clave_ref]; // memoria para el par (clave, referencia)

                //Se carga registro_indice con los datos de la clave Y LUEGO el de la referencia asociada
                memcpy(registro_indice, clave_mem, this->tam_clave);
                memcpy(registro_indice + (this->tam_clave), referencia, sizeof(Referencia));

                //Se escribe el par (clave, referencia) en el cubo (en memoria en p_cubo)
                memcpy(p_cubo + sizeof(Tcant_reg) + sizeof(Ttam_disp) + cant_regi*(this->tam_clave_ref), registro_indice, this->tam_clave_ref);

                //Se actualiza la cantidad de registros del cubo
                cant_regi++;
                memcpy(p_cubo, &cant_regi, sizeof(Tcant_reg) );

                //Se escribe el cubo que está en memoria, en disco
                this->cubos.seekp(ref_cubos*(this->bytes_cubo), ios_base::beg );
                this->cubos.write( (char*) p_cubo, this->bytes_cubo );

                delete [] registro_indice;
        }

        delete [] p_cubo;
}

void HashingExt::serializar_clave(char *clave_mem, const Clave *clave)
{
        unsigned tam_parcial = 0;
        //Se cargan en la variable clave_mem, TODOS los datos que componen la clave
        Atributo *atrib_aux;
        for(unsigned k=0; k < this->cant_at_clave; k++)
        {
                atrib_aux = clave->getAtributo(k);
                char *componente_clave = new char[atrib_aux->tamanio()];
                atrib_aux->get(componente_clave); //se obtiene un componente de la clave en memoria
                memcpy(clave_mem + tam_parcial, componente_clave, atrib_aux->tamanio() );
                delete [] componente_clave;
                tam_parcial += ( atrib_aux->tamanio() );
        }
}

void HashingExt::resolver_insercion( char *clave_mem, Referencia referencia)
{
        unsigned pos_tabla;
        int ref_cubos;

        pos_tabla = this->dispersar(clave_mem); //se obtiene una referencia a la tabla de dispersión
        this->tabla_dispersion.seekg(pos_tabla*sizeof(Referencia), ios_base::beg);
        this->tabla_dispersion.read((char*) &ref_cubos, sizeof(Referencia));

        if(ref_cubos == REF_NULA)
        {
                this->crear_cubo_ini(clave_mem, &referencia);
                //Se escribe la referencia al nuevo cubo creado, en la tabla de dispersión
                this->tabla_dispersion.seekp(pos_tabla * sizeof(ref_cubos), ios_base::beg);
                this->tabla_dispersion.write( (char*) &( this->contador_cubos), sizeof(this->contador_cubos));

        }else
        { //El cubo ya existe, y hay que insertar el registro contemplando sobreflujos
                this->agregar_registro(clave_mem, &referencia, ref_cubos, pos_tabla);
        }
}
/*
        Recibe:
                        recuperar: según se busque para recuperar la referencia de la clave pasado o para modificarla deberá tener
                                        los valores: BUSCAR_PARA_RECUPERAR o BUSCAR_PARA_MODIFICAR respectivamente
*/

bool HashingExt::buscar_clave(const Clave *clave, Referencia *referencia, unsigned recuperar)
{
        char *clave_mem = new char[this->tam_clave]; //clave serializada
        unsigned pos_tabla;
        int ref_cubo; //referencia a un cubo que devolverá la tabla de dispersión
        char *p_cubo = new char[this->bytes_cubo];
        Tcant_reg cant_registros;//del cubo obtenido al dispersar la clave
        char *un_registro = new char[this->tam_clave_ref]; //variable auxiliar
        char *una_clave = new char[this->tam_clave]; //variable auxiliar

        this->serializar_clave(clave_mem, clave);

        pos_tabla= this->dispersar(clave_mem);

        this->tabla_dispersion.open(this->nom_tabla.c_str());
        this->cubos.open(this->nom_cubos.c_str());

        this->tabla_dispersion.seekg(pos_tabla*sizeof(ref_cubo), ios_base::beg);
        this->tabla_dispersion.read( (char*) &ref_cubo, sizeof(ref_cubo));

        this->cubos.seekg(ref_cubo*(this->bytes_cubo), ios_base::beg);
        this->cubos.read( (char*) p_cubo, this->bytes_cubo);

        //Se busca la clave solicitada en el cubo (p_cubo)
        memcpy(&cant_registros, p_cubo, sizeof(Tcant_reg) );

        if (!cant_registros)
        {
                //Significa que el cubo está vacío, y por lo tanto no puede contener la clave solicitada
                delete [] clave_mem;
                delete [] p_cubo;
                delete [] un_registro;
                delete [] una_clave;
                this->tabla_dispersion.close();
                this->cubos.close();
                return false;
        }

        for ( int i = 0; i < cant_registros; i++)
        {
                memcpy(un_registro, p_cubo + sizeof(Tcant_reg) + sizeof(Ttam_disp) + i*(this->tam_clave_ref), this->tam_clave_ref);
                memcpy(una_clave, un_registro, this->tam_clave);
                if (!strncmp(una_clave, clave_mem, this->tam_clave) )
                {

                        //Se encontró la clave
                        if (recuperar)
                        {
                                memcpy(referencia, un_registro + this->tam_clave, sizeof(Referencia));

                                delete [] clave_mem;
                                delete [] p_cubo;
                                delete [] un_registro;
                                delete [] una_clave;
                                this->tabla_dispersion.close();
                                this->cubos.close();
                                return true;
                        }else
                        {
                                //Se buscó para modificar la referencia
                                //Se modifica
                                memcpy(un_registro + this->tam_clave, referencia, sizeof(Referencia));
                                memcpy(p_cubo +  sizeof(Tcant_reg) + sizeof(Ttam_disp) + i*(this->tam_clave_ref), un_registro, this->tam_clave_ref);
                                this->cubos.seekp(ref_cubo*(this->bytes_cubo), ios_base::beg);
                                this->cubos.write( (char*) p_cubo, this->bytes_cubo);

                                delete [] clave_mem;
                                delete [] p_cubo;
                                delete [] un_registro;
                                delete [] una_clave;
                                this->tabla_dispersion.close();
                                this->cubos.close();
                                return true;

                        }
                }
        }
        //Si se llegó a esta línea, no se encontró la clave
        delete [] clave_mem;
        delete [] p_cubo;
        delete [] un_registro;
        delete [] una_clave;
        this->tabla_dispersion.close();
        this->cubos.close();
        return false;
}

/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/
/***************************************************    Métodos públicos   **********************************************************************/
/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/

void HashingExt::crear(string nombre_arch, unsigned tam_cubo_bytes, Clave *claves, Referencia *referencias, unsigned cant_reg_ini)
{
        unsigned cant_cubos = 0;  //cantidad de cubos iniciales
        int ref_cubos = REF_NULA; // apunta a un cubo en el archivo de cubos
        unsigned tam_clave=0; //tamaño de la suma de los datos que componen la clave
        unsigned tam_para_datos = 0; //tamaño para registros de índice por cubo
        Atributo *un_atributo_aux;  // variable auxiliar
        unsigned tam_clave_ref = 0; //tamaño de cada registro de índice

        this->contador_cubos =-1;
        this->cant_cubos_libres = 0;

        this->nom_tabla = nombre_arch + "_tabla";
        this->nom_cubos = nombre_arch + "_cubos";
        this->nom_libres = nombre_arch + "_libres";

        this->bytes_cubo = tam_cubo_bytes;

        this->cant_at_clave = claves->getCantidadAtributos();
        this->tamanios = new int[(this->cant_at_clave)]; //esta memoria se liberará en el destructor

       /**      Se calcula la cantidad registros (clave, referencia) que caben por cubo            **/
        //Primero se calcula la cantidad de bytes ocupada por los datos que componen la clave
        for(unsigned k=0; k < cant_at_clave; k++)
        {
                un_atributo_aux = claves->getAtributo(k);
                this->tamanios[k] = un_atributo_aux->tamanio(); //se almacenan los tamaños que ocupan los componentes de la clave
                tam_clave += un_atributo_aux->tamanio();
        }

        //Luego se calcula el tamaño de cada registro de índice (sumando el tamaño ocupado por una referencia)
        tam_clave_ref = tam_clave + sizeof(Referencia);

       //Se calcula el tamaño para datos que tiene un cubo (restandole el espacio para metadatos)
        tam_para_datos = this->bytes_cubo - ( sizeof(Ttam_disp) + sizeof(Tcant_reg) );

        //Finalmente se calcula cuantos registros de índice entran por cubo:
        this->tam_cubo = tam_para_datos / tam_clave_ref;

        /**      Se calculan la cantidad de cubos con una densidad de carga del 70%      **/
        this->calcular_cantidad_cubos(tam_clave_ref, tam_para_datos, cant_reg_ini, &cant_cubos);

        //Carga de atributos referidos al registro índice
        this->tam_clave = tam_clave;
        this->tam_clave_ref = tam_clave_ref;

        /***********************      Se cargan los archivos    *******************************/
        //Se carga la tabla con una cantidad de referencias nulas igual a  la cantidad de cubos
        this->tabla_dispersion.open(nom_tabla.c_str(), ios::out);
        for(unsigned i=0; i < cant_cubos; i++)
        {
                this->tabla_dispersion.write( (char*) &ref_cubos, sizeof(ref_cubos) );
        }
        tabla_dispersion.close();
        this->tam_tabla = cant_cubos; // tamaño actual de la tabla

        //Se crea el archivo de cubos libres vacío
        this->cubos_libres.open(nom_cubos.c_str());
        this->cubos_libres.close();

        //Se crea el archivo de cubos vacío
        this->cubos.open(nom_cubos.c_str());
        this->cubos.close();

        /*************************Se cargan los registros iniciales***************************/
        this->tabla_dispersion.open( nom_tabla.c_str() );
        this->cubos.open(nom_cubos.c_str() );
        for(unsigned j=0; j < cant_reg_ini ; j++)
        {
                char *clave_mem = new char[this->tam_clave]; //memoria solo para los datos que componen la clave
                this->serializar_clave(clave_mem, claves+j);

                resolver_insercion(clave_mem, referencias[j]);

                delete []clave_mem;

        }
        this->tabla_dispersion.close();
        this->cubos.close();

}

 bool HashingExt::insertar(Referencia ref, Clave* clave)
 {

        //Se abren los archivos de trabajo
        this->tabla_dispersion.open(this->nom_tabla.c_str());
        this->cubos.open(this->nom_cubos.c_str());

         //Se serializa la clave
        char *clave_mem = new char[this->tam_clave]; //memoria solo para los datos que componen la clave
        this->serializar_clave(clave_mem, clave);

        resolver_insercion(clave_mem, ref);

        this->tabla_dispersion.close();
        this->cubos.close();
        delete []clave_mem;

        return true;

};

bool HashingExt::BuscarReferencia(const Clave *clave, Referencia* referencia)
{
        return this->buscar_clave(clave, referencia, BUSCAR_PARA_RECUPERAR);

}

bool HashingExt::modificar(const Clave *clave, Referencia refNueva)
{
        return this->buscar_clave(clave, &refNueva, BUSCAR_PARA_MODIFICAR);
}
