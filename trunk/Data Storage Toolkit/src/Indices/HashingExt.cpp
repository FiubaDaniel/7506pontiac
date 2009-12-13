#include "HashingExt.hpp"

using namespace std;

/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/***************************************************    Metodos privados   *******************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/

void HashingExt::calcular_cantidad_cubos(const unsigned tam_clave_ref, const unsigned tam_para_datos, const unsigned cant_reg_ini, unsigned *cant_cubos)
{
        int cant_esperado;
        /*
           Se supone un 30% mas de registros para evitar una mayor cantidad de sobreflujos
        */
        incrementar30(cant_reg_ini, cant_esperado);

        (*cant_cubos)  = (int) ( cant_esperado * tam_clave_ref / tam_para_datos ) + 1; //se suma 1 para redondear hacia arriba

}
unsigned HashingExt::dispersar(char *clave_mem)
{
        /*
             Se transforma la clave a un valor entero, mediante la sumatoria de los bytes en base 2
             Luego se aplica el MÓDULO (tamaño de la tabla)
        */
        unsigned ref_en_tabla=0; //valor de dispersión
        int numero_a_dispersar = 0;

        for(int i = 0; i < this->tam_clave; i++)
        {
                numero_a_dispersar += pow(2, i)*(*(clave_mem+i));
        }

        ref_en_tabla = numero_a_dispersar % (this->tam_tabla);

        return ref_en_tabla;
}

void HashingExt::crear_cubo_ini(char* clave_mem, Referencia *referencia)
{
        char *p_cubo = new char[this->bytes_cubo]; //memoria para el cubo completo que se va a agregar
        Tcant_reg cant_reg = 1; //los cubos de la carga inicial se crean con un registro
        Ttam_disp tam_disp_cubo = this->tam_tabla;
        char *registro_indice = new char[this->tam_clave_ref]; // memoria para el par (clave, referencia)

        //Se carga registro_indice con los datos de la clave Y LUEGO el de la referencia asociada
        memcpy(registro_indice, clave_mem, this->tam_clave);
        memcpy(registro_indice + (this->tam_clave), referencia, sizeof(Referencia));

        /*************     Se inicializa el cubo      *************/
        //Primero se guardan los metadatos (cantidad de registros, tama�o de dispersi�n del cubo)
        memcpy(p_cubo, &cant_reg, sizeof(Tcant_reg) );
        memcpy(p_cubo + sizeof(Tcant_reg), &tam_disp_cubo, sizeof(Ttam_disp) );
        //Por �ltimo se carga el registro de �ndice que provoc� la creaci�n del cubo
        memcpy(p_cubo + sizeof(Tcant_reg) + sizeof(Ttam_disp), registro_indice, this->tam_clave_ref);

        /************Se carga en el archivo de cubos***********/
        this->contador_cubos ++;
        this->cubos.seekp((this->contador_cubos)*(this->bytes_cubo), ios_base:: beg);
        this->cubos.write( p_cubo, (this->bytes_cubo));

        delete []p_cubo;
        delete []registro_indice;
}

void HashingExt::crear_cubo_vacio(Ttam_disp tam_dispersion)
{
        char *p_cubo = new char[this->bytes_cubo]; //memoria para el cubo completo que se va a agregar
        Tcant_reg cant_reg = 0;
        Ttam_disp tam_disp_cubo = tam_dispersion;

        /**        Se inicializa el cubo       **/
        //Se guardan los metadatos (cantidad de registros, tama�o de dispersi�n del cubo)
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
void HashingExt::dec_circular_a_saltos(unsigned *pos_tabla, int salto)
{
        int pos_aux = (*pos_tabla);
        pos_aux -= salto;

        if( pos_aux < 0)
        {
                pos_aux = this->tam_tabla + pos_aux;
        }
        (*pos_tabla) = pos_aux;
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
                this->cubos_libres.seekg(0, ios_base::beg);
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
                //SE DUPLICA LA TABLA de dispersión, y se duplica el tamaño de dispersión del cubo desbordado
                char *contenido_tabla = new char[(this->tam_tabla) * sizeof(int)]; //para almacenar la tabla ACTUAL en memoria

                this->tabla_dispersion.seekg(0, ios_base::beg); //posicionarse al principio
                this->tabla_dispersion.read( (char*) contenido_tabla, (this->tam_tabla) * sizeof(int) ); //leer todo el contenido de la tabla

                this->tabla_dispersion.seekp((this->tam_tabla)*sizeof(int), ios_base::beg); // posicionarse al final
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

                //Se consigna al nuevo cubo en la tabla en la posición en dónde ocurrió el overflow ( en "pos_tabla")
                this->tabla_dispersion.seekp( pos_tabla * sizeof(int), ios_base::beg);
                this->tabla_dispersion.write( (char*) &(this->contador_cubos), sizeof(int));

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
                //El cubo desbordado aparece más de una vez, y NO SE DUPLICA LA TABLA

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
                       this->resolver_insercion( una_clave, una_ref); //resolver_inserción podrá llamar a esta función de nuevo (recursividad indirecta)
                }
                //Se reintenta insertar el registro que causó el overflow
                this->resolver_insercion(clave_mem, *referencia);

        }

        delete [] un_registro;
        delete [] una_clave;
}


bool HashingExt::agregar_registro(char *clave_mem, Referencia *referencia, const int ref_cubos, unsigned pos_tabla)
{
        char *p_cubo = new char[this->bytes_cubo]; //memoria para el cubo completo
        int cant_regi; //cantidad de registros que contiene el cubo referenciado
        char *un_registro = new char[this->tam_clave_ref]; //variable auxiliar
        char *una_clave = new char[this->tam_clave]; //variable auxiliar

        //Se lee el cubo que corresponde
        this->cubos.seekg(ref_cubos*(this->bytes_cubo), ios_base::beg );
        this->cubos.read( (char*) p_cubo, this->bytes_cubo);

        //Se interpreta la cantidad de registros que contiene el cubo
        memcpy(&cant_regi, p_cubo, sizeof(Tcant_reg) );


        /******* Se verifica que la clave no exista en el cubo ******/
        /**Esto evita que cuando se llame a "resolver_sobreflujo" se tenga que estar verificando si se puede insertar o no**/
        for ( int i = 0; i < cant_regi; i++)
        {
                memcpy(un_registro, p_cubo + sizeof(Tcant_reg) + sizeof(Ttam_disp) + i*(this->tam_clave_ref), this->tam_clave_ref);
                memcpy(una_clave, un_registro, this->tam_clave);
                if ( ! strncmp( clave_mem, una_clave, this->tam_clave) )
                {
                        cout << "No se pudo insertar el registro porque hay otro con igual clave"<<endl;
                        delete [] p_cubo;
                        delete [] un_registro;
                        delete [] una_clave;
                        return false;
                }
        }

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
        delete [] un_registro;
        delete [] una_clave;
        return true;
}

void HashingExt::serializar_clave(char *clave_mem, Clave *clave)
{
        unsigned tam_parcial = 0;
        Atributo *atrib_aux;

        //Se cargan en la variable clave_mem, TODOS los datos que componen la clave
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

bool HashingExt::resolver_insercion( char *clave_mem, Referencia referencia)
{
        unsigned pos_tabla;
        int ref_cubos;

        pos_tabla = this->dispersar(clave_mem); //se obtiene una referencia a la tabla de dispersión
        //cout << "pos_tabla: "<< pos_tabla << endl;

        this->tabla_dispersion.seekg(pos_tabla*sizeof(Referencia), ios_base::beg);
        this->tabla_dispersion.read((char*) &ref_cubos, sizeof(Referencia));

        //cout << "ref_cubos " << ref_cubos << endl;

        if(ref_cubos == REF_NULA)
        {
                this->crear_cubo_ini(clave_mem, &referencia);
                //Se escribe la referencia al nuevo cubo creado, en la tabla de dispersión
                this->tabla_dispersion.seekp(pos_tabla * sizeof(ref_cubos), ios_base::beg);
                this->tabla_dispersion.write( (char*) &( this->contador_cubos), sizeof(this->contador_cubos) );

        }else
        { //El cubo ya existe, y hay que insertar el registro contemplando sobreflujos
                if ( !(this->agregar_registro(clave_mem, &referencia, ref_cubos, pos_tabla)) )
                        return false;
        }
        return true;
}


bool HashingExt::buscar_clave(Clave *clave, Referencia *referencia, unsigned recuperar)
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

        this->tabla_dispersion.seekg(pos_tabla*sizeof(ref_cubo), ios_base::beg);
        this->tabla_dispersion.read( (char*) &ref_cubo, sizeof(ref_cubo));

        if(ref_cubo == REF_NULA)
        {
                //Significa que no hay ningún registro almacenado
                delete [] clave_mem;
                delete [] p_cubo;
                delete [] un_registro;
                delete [] una_clave;

                return false;
        }

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


                return false;
        }

        for ( int i = 0; i < cant_registros; i++)
        {
                memcpy(un_registro, p_cubo + sizeof(Tcant_reg) + sizeof(Ttam_disp) + i*(this->tam_clave_ref), this->tam_clave_ref);
                memcpy(una_clave, un_registro, this->tam_clave); //El formato es (clave PRIMERO, referencia DESPUÉS)
                if (!strncmp(una_clave, clave_mem, this->tam_clave) )
                {
                       //Se encontró la clave
                       switch(recuperar)
                       {
                               case (BUSCAR_PARA_MODIFICAR):

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

                                        return true;

                                case(BUSCAR_PARA_RECUPERAR):

                                        memcpy(referencia, un_registro + this->tam_clave, sizeof(Referencia));

                                        delete [] clave_mem;
                                        delete [] p_cubo;
                                        delete [] un_registro;
                                        delete [] una_clave;

                                        return true;

                                case(BUSCAR_PARA_ELIMINAR):
                                        if(cant_registros==1)
                                        {
                                                //El cubo se vacía

                                                cant_registros--; //la "eliminación" es el decremento de este dato de control
                                                memcpy(p_cubo, &cant_registros, sizeof(Tcant_reg) ); //se actualiza el cubo EN MEMORIA
                                                this->cubos.seekp(ref_cubo*(this->bytes_cubo), ios_base::beg); //se graba el cubo actualizado
                                                this->cubos.write( (char*) p_cubo, this->bytes_cubo);

                                                //Se decide si se puede quitar la referencia de la tabla
                                                unsigned pos_tabla_aux  = pos_tabla; //se inicializa con la posición obtenida por la función de dispersión
                                                int salto; //será el tamaño de dispersión del cubo involucrado dividido 2
                                                unsigned ref_cubo_izq;
                                                unsigned ref_cubo_der;
                                                Ttam_disp nuevo_tam_disp;

                                                memcpy((char*) &salto, p_cubo + sizeof(Tcant_reg), sizeof(Ttam_disp) );
                                                salto /= 2;

                                                inc_circular_a_saltos(&pos_tabla_aux, salto);
                                                this->tabla_dispersion.seekg(pos_tabla_aux*sizeof(ref_cubo), ios_base::beg);
                                                this->tabla_dispersion.read( (char*) &ref_cubo_der, sizeof(ref_cubo));

                                                pos_tabla_aux = pos_tabla; //se vuelve a la posición original
                                                dec_circular_a_saltos(&pos_tabla_aux, salto);
                                                this->tabla_dispersion.seekg(pos_tabla_aux*sizeof(ref_cubo), ios_base::beg);
                                                this->tabla_dispersion.read( (char*) &ref_cubo_izq, sizeof(ref_cubo));

                                                if( ref_cubo_der == ref_cubo_izq)
                                                {
                                                        /**En este caso se puede liberar el cubo, es decir, quitar su referencia de la tabla de dispersión**/

                                                        /*Se usa p_cubo para almacenar el cubo cuyas referencias se van copiar en la tabla
                                                                para quitar de la misma, las ocurrencias del cubo vaciado */
                                                        this->cubos.seekg(ref_cubo_der*(this->bytes_cubo), ios_base::beg);
                                                        this->cubos.read( (char*) p_cubo, this->bytes_cubo);

                                                        //Se modifica su tamaño de dispersión, dividiéndolo por 2
                                                        memcpy(&nuevo_tam_disp, p_cubo + sizeof(Tcant_reg), sizeof(Ttam_disp) );
                                                        nuevo_tam_disp /= 2;
                                                        memcpy(p_cubo + sizeof(Tcant_reg), &nuevo_tam_disp, sizeof(Ttam_disp) );

                                                        this->cubos.seekp(ref_cubo_der*(this->bytes_cubo), ios_base::beg);
                                                        this->cubos.write( (char*) p_cubo, this->bytes_cubo);

                                                        /*Se modifica la tabla de dispersión, copiando referencias "ref_cubo_der" (o izq) de a saltos "nuevo_tam_disp"
                                                        desde "pos_tabla" (el lugar en dónde se encontró la referencia al cubo que contenía la clave a eliminar)*/

                                                        pos_tabla_aux = pos_tabla; //se vuelve a la posición original y se reusa la variable para RECORRER LA TABLA
                                                        do{
                                                                this->tabla_dispersion.seekp(pos_tabla_aux*sizeof(ref_cubo), ios_base::beg);
                                                                this->tabla_dispersion.write( (char*) &ref_cubo_der, sizeof(ref_cubo));
                                                                inc_circular_a_saltos(&pos_tabla_aux, nuevo_tam_disp);

                                                        }while(pos_tabla_aux != pos_tabla);

                                                        //Finalmente se coloca la referencia del cubo vaciado en el archivo de cubos libres

                                                        this->cubos_libres.seekp((this->cant_cubos_libres)*sizeof(ref_cubo), ios_base::beg);
                                                        this->cubos_libres.write( (char*) &ref_cubo, sizeof(ref_cubo));

                                                        this->cant_cubos_libres ++;

                                                        /************************SE VERIFICA QUE LA TABLA NO ESTE ESPEJADA******************************/
                                                        int *referencias_a_cubos = new int[this->tam_tabla];
                                                        bool mitades_iguales =true;
                                                        this->tabla_dispersion.seekg(0, ios_base::beg);
                                                        this->tabla_dispersion.read( (char*) referencias_a_cubos, (this->tam_tabla)*sizeof(int) );

                                                        for( int v = 0; v  < ( (this->tam_tabla) /2 ) ; v++)
                                                        {
                                                                if ( referencias_a_cubos[v] != referencias_a_cubos[(this->tam_tabla)/2 + v])
                                                                        mitades_iguales = false;
                                                        }

                                                        if (mitades_iguales)
                                                        {
                                                                (this->tam_tabla) /= 2;  //se "trunca" la tabla por la mitad
                                                        }

                                                        delete [] referencias_a_cubos;

                                                }

                                        }else{
                                                //el cubo no se vacía

                                                //Si el registro a eliminar era el último en el cubo
                                                if( (i+1) == cant_registros )
                                                {
                                                        cant_registros--; //la "eliminación" es el decremento de este dato de control
                                                        memcpy(p_cubo, &cant_registros, sizeof(Tcant_reg) );
                                                        this->cubos.seekp(ref_cubo*(this->bytes_cubo), ios_base::beg);
                                                        this->cubos.write( (char*) p_cubo, this->bytes_cubo);

                                                }else{
                                                        //sino hay que desplazar "un lugar" todos los registros que estaban DESPUÉS
                                                        int j = i+1;
                                                        do{
                                                                memcpy(un_registro, p_cubo + sizeof(Tcant_reg) + sizeof(Ttam_disp) + j*(this->tam_clave_ref), this->tam_clave_ref);
                                                                memcpy(p_cubo + sizeof(Tcant_reg) + sizeof(Ttam_disp) + (j-1)*(this->tam_clave_ref), un_registro, this->tam_clave_ref);
                                                                j++;
                                                        }while( j < cant_registros );
                                                        cant_registros--;
                                                        memcpy(p_cubo, &cant_registros, sizeof(Tcant_reg) );
                                                        this->cubos.seekp(ref_cubo*(this->bytes_cubo), ios_base::beg);
                                                        this->cubos.write( (char*) p_cubo, this->bytes_cubo);
                                                }
                                        }

                                        delete [] clave_mem;
                                        delete [] p_cubo;
                                        delete [] un_registro;
                                        delete [] una_clave;
                                        return true;

                        }//fin switch
                }//fin if
        }//fin for
        //Si se llegó a esta línea significa que no se encontró la clave en el cubo
        delete [] clave_mem;
        delete [] p_cubo;
        delete [] un_registro;
        delete [] una_clave;
        return false;
}

/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/
/***************************************************    Metodos publicos   **********************************************************************/
/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/

bool HashingExt::crear_con_carga_inicial(string nombre_arch, unsigned tam_cubo_bytes, Clave *claves, Referencia *referencias, unsigned cant_reg_ini)
{
        unsigned cant_cubos = 0;  //cantidad de cubos iniciales
        int ref_cubos = REF_NULA; // apunta a un cubo en el archivo de cubos
        unsigned tam_para_datos = 0; //tama�o para registros de �ndice por cubo
        Atributo *un_atributo_aux;  // variable auxiliar

        this->contador_cubos =-1;
        this->cant_cubos_libres = 0;
        this->tam_clave = 0;
        this->tam_clave_ref = 0;

        this->nom_tabla = nombre_arch + "_tabla";
        this->nom_cubos = nombre_arch + "_cubos";
        this->nom_libres = nombre_arch + "_libres";

        this->bytes_cubo = tam_cubo_bytes;

        this->cant_at_clave = claves->getCantidadAtributos();

       /*****************  Se calcula la cantidad registros (clave, referencia) que caben por cubo      ******************/
        //Primero se calcula la cantidad de bytes ocupada por los datos que componen la clave
        for(unsigned k=0; k < cant_at_clave; k++)
        {
                un_atributo_aux = claves->getAtributo(k);
                (this->tam_clave) += un_atributo_aux->tamanio();
        }

        //Luego se calcula el tama�o de cada registro de �ndice (sumando el tama�o ocupado por una referencia)
        this->tam_clave_ref = this->tam_clave + sizeof(Referencia);

       //Se calcula el tama�o para datos que tiene un cubo (restandole el espacio para metadatos)
        tam_para_datos = this->bytes_cubo - ( sizeof(Ttam_disp) + sizeof(Tcant_reg) );

        //Finalmente se calcula cuantos registros de �ndice entran por cubo:
        this->tam_cubo = tam_para_datos / (this->tam_clave_ref);

        /**      Se calculan la cantidad de cubos considerando una densidad de carga del 70% a efecto de
                reducir la cantidad de sobreflujos que puedan ocurrir                                           **/
        this->calcular_cantidad_cubos(this->tam_clave_ref, tam_para_datos, cant_reg_ini, &cant_cubos);

        /***********************************      Se cargan los archivos    *****************************************/
        //Se carga la tabla con una cantidad de referencias nulas igual a  la cantidad de cubos
        this->tabla_dispersion.open(nom_tabla.c_str(), fstream:: out | fstream::trunc | fstream::binary);
        if(! ( tabla_dispersion))
        {
                cout << "Error al abrir el archivo de tabla" << endl;
                return false;
        }

        for(unsigned i=0; i < cant_cubos; i++)
        {
                this->tabla_dispersion.write( (char*) &ref_cubos, sizeof(ref_cubos) );
        }
        this->tabla_dispersion.close();

        this->tam_tabla = cant_cubos; // se inicializa con el tama�o actual de la tabla

        //Se crea el archivo de cubos libres vac�o
        this->cubos_libres.open(nom_libres.c_str(), fstream:: out | fstream::trunc| fstream::binary);
        if ( !cubos_libres)
        {
                cout << "Error al abrir el archivo de cubos libres " << endl;
                return false;
        }
        this->cubos_libres.close();

        //Se crea el archivo de cubos vac�o
        this->cubos.open(nom_cubos.c_str(), fstream:: out | fstream::trunc | fstream::binary);
        if(!cubos)
        {
                cout << "Error al abrir el archivo de cubos " <<endl;
                return false;
        }
        this->cubos.close();

        /**********************************Se cargan los registros iniciales*******************************************/
        this->tabla_dispersion.open( nom_tabla.c_str(), fstream::out | fstream::in | fstream::binary);
       if(! ( tabla_dispersion))
        {
                cout << "Error al abrir el archivo de la tabla" << endl;
                return false;
        }

        this->cubos.open(nom_cubos.c_str(),  fstream::out | fstream:: in |fstream::binary);
        if(!cubos)
        {
                cout << "Error al abrir el archivo de cubos " <<endl;
                return false;
        }

        for(unsigned j=0; j < cant_reg_ini ; j++)
        {
                char *clave_mem = new char[this->tam_clave]; //memoria solo para los datos que componen la clave
                this->serializar_clave(clave_mem, claves+j);

                resolver_insercion(clave_mem, referencias[j]);
                delete []clave_mem;
        }
        this->tabla_dispersion.close();
        //cout << "hola hola que tendran los cubos" << endl;

        this->cubos.close();

        /******************************Persistencia del objeto****************************************/

        this->nom_persist = nombre_arch + "_persistencia" ;
        this->persist.open(nom_persist.c_str(),  fstream:: out | fstream::trunc| fstream::binary);
        if(!persist)
        {
                cout << "Error al abrir el archivo de persistencia " <<endl;
                return false;
        }

        //Los 3 primeros atributos son los que cambian a lo largo del uso del �ndice
        this->persist.write( (char*) &(this->tam_tabla), sizeof(this->tam_tabla) );
        this->persist.write( (char*) &(this->contador_cubos), sizeof(this->contador_cubos) );
        this->persist.write( (char*) &(this->cant_cubos_libres), sizeof(this->cant_cubos_libres) );

        //Los �ltimos atributos son los que no cambian
        this->persist.write( (char*) &(this->bytes_cubo), sizeof(this->bytes_cubo));
        this->persist.write( (char*) &(this->tam_cubo), sizeof( this->tam_cubo));
        this->persist.write( (char*) &(this->tam_tabla), sizeof(this->tam_tabla));
        this->persist.write( (char*) &(this->tam_clave_ref), sizeof( this->tam_clave_ref));
        this->persist.write( (char*) &(this->tam_clave), sizeof(this->tam_clave) );

        this->persist.close();

        return true;

}

             /*
                Recibe:
                        nombre_arch: prefijo de los nombres de los archivos que se crearan al usar el �ndice
                        tamanioBloque : tama�o del cubo en bytes
                        clave: para especificar las caracter�siticas de todas las claves (tama�o en bytes y cantidad de atributos)
                        El comparador de claves no tiene uso en este �ndice, sin embargo se incluy� para implementar el patr�n estrategia

                */
void HashingExt::crear(std::string nombreArch, unsigned int tamanioBloque, Clave* clave, ComparadorClave* comp)
{
        int ref_cubo_nul = REF_NULA;
        unsigned tam_para_datos = 0; //tamaño para registros de índice por cubo
        Atributo *un_atributo_aux;  // variable auxiliar

        this->contador_cubos =-1;
        this->cant_cubos_libres = 0;
        this->tam_clave = 0;
        this->tam_clave_ref = 0;

        this->nom_tabla = nombreArch + "_tabla";
        this->nom_cubos = nombreArch + "_cubos";
        this->nom_libres = nombreArch + "_libres";
        this->nom_persist = nombreArch + "_persistencia" ;

        this->bytes_cubo = tamanioBloque;
        this->cant_at_clave = clave->getCantidadAtributos();
        this->tam_tabla = 1;

        /*****************  Se calcula la cantidad registros (clave, referencia) que caben por cubo      ******************/
        //Primero se calcula la cantidad de bytes ocupada por los datos que componen la clave
        for(unsigned k=0; k < this->cant_at_clave; k++)
        {
                un_atributo_aux = clave->getAtributo(k);
                (this->tam_clave) += un_atributo_aux->tamanio();
        }

        //Luego se calcula el tamaño de cada registro de índice (sumando el tamaño ocupado por una referencia)
        this->tam_clave_ref = this->tam_clave + sizeof(Referencia);

       //Se calcula el tamaño para datos que tiene un cubo (restandole el espacio para metadatos)
        tam_para_datos = this->bytes_cubo - ( sizeof(Ttam_disp) + sizeof(Tcant_reg) );

        //Finalmente se calcula cuantos registros de índice entran por cubo:
        this->tam_cubo = tam_para_datos / (this->tam_clave_ref);


        this->persist.open(nom_persist.c_str(),  fstream:: out | fstream::trunc| fstream::binary);
        if(!persist.is_open())
        {
                cout << "Error al abrir el archivo de persistencia " <<endl;
                return;
        }

        //Los 3 primeros atributos son los que cambian a lo largo del uso del índice
        this->persist.write( (char*) &(this->tam_tabla), sizeof(this->tam_tabla) );
        this->persist.write( (char*) &(this->contador_cubos), sizeof(this->contador_cubos) );
        this->persist.write( (char*) &(this->cant_cubos_libres), sizeof(this->cant_cubos_libres) );

        //Los últimos atributos son los que no cambian
        this->persist.write( (char*) &(this->bytes_cubo), sizeof(this->bytes_cubo));
        this->persist.write( (char*) &(this->tam_cubo), sizeof( this->tam_cubo));
        this->persist.write( (char*) &(this->cant_at_clave), sizeof(this->cant_at_clave));
        this->persist.write( (char*) &(this->tam_clave_ref), sizeof( this->tam_clave_ref));
        this->persist.write( (char*) &(this->tam_clave), sizeof(this->tam_clave) );

        this->persist.close();

        this->tabla_dispersion.open(nom_tabla.c_str(), fstream:: out | fstream::trunc | fstream::binary);
        if(! tabla_dispersion.is_open())
        {
                cout << "Error al abrir el archivo de tabla" << endl;
                return;
        }
        this->tabla_dispersion.write((char*) &ref_cubo_nul, sizeof(ref_cubo_nul) );
        this->tabla_dispersion.close();

        //Se crea el archivo de cubos libres vacío
        this->cubos_libres.open(nom_libres.c_str(), fstream:: out | fstream::trunc| fstream::binary);
        if ( !cubos_libres.is_open())
        {
                cout << "Error al abrir el archivo de cubos libres " << endl;
                return;
        }
        this->cubos_libres.close();

        //Se crea el archivo de cubos vacío
        this->cubos.open(nom_cubos.c_str(), fstream:: out | fstream::trunc | fstream::binary);
        if(!cubos.is_open())
        {
                cout << "Error al abrir el archivo de cubos " <<endl;
                return;
        }
        this->cubos.close();

        this->tabla_dispersion.open(nom_tabla.c_str(), fstream:: in | fstream:: out | fstream::binary);
        this->cubos.open(nom_cubos.c_str(), fstream:: in | fstream::out | fstream::binary);
        this->cubos_libres.open(nom_libres.c_str(), fstream:: in | fstream ::out | fstream ::binary);
        this->persist.open(nom_persist.c_str(), fstream:: in | fstream ::out | fstream ::binary);

}


bool HashingExt::abrir(std::string nombreArch,ComparadorClave* comp)
{
        this->nom_tabla = nombreArch + "_tabla";
        this->nom_cubos = nombreArch + "_cubos";
        this->nom_libres = nombreArch + "_libres";
        this->nom_persist = nombreArch + "_persistencia" ;

        this->tabla_dispersion.open(nom_tabla.c_str(), fstream::in | fstream:: out |fstream::binary);
        if ( tabla_dispersion.fail() )
        {
                tabla_dispersion.clear();
                cout << "El archivo de tabla NO EXISTE (se creara) " << endl;
                return false;
        }


        this->cubos_libres.open(nom_libres.c_str(), fstream::in | fstream::out| fstream::binary);
        if ( cubos_libres.fail() )
        {
                cubos_libres.clear();
                cout << "El archivo de cubos libres NO EXISTE (se creara) " << endl;
                return false;
        }


        this->cubos.open(nom_cubos.c_str(),  fstream::in | fstream::out| fstream::binary);
        if( cubos.fail() )
        {
                cubos.clear();
                cout << "El archivo de cubos N0 EXISTE (se creara)" <<endl;
                return false;
        }


        this->persist.open(nom_persist.c_str(),  fstream:: in | fstream::out| fstream::binary);
        if( persist.fail() )
        {
                persist.clear();
                cout << "El archivo de persistencia NO EXISTE (se creara)" <<endl;
                return false;
        }


        this->persist.read( (char*) &(this->tam_tabla), sizeof(this->tam_tabla) );
        this->persist.read( (char*) &(this->contador_cubos), sizeof(this->contador_cubos) );
        this->persist.read( (char*) &(this->cant_cubos_libres), sizeof(this->cant_cubos_libres) );

        this->persist.read( (char*) &(this->bytes_cubo), sizeof(this->bytes_cubo));
        this->persist.read( (char*) &(this->tam_cubo), sizeof( this->tam_cubo));
        this->persist.read( (char*) &(this->cant_at_clave), sizeof(this->cant_at_clave));
        this->persist.read( (char*) &(this->tam_clave_ref), sizeof( this->tam_clave_ref));
        this->persist.read( (char*) &(this->tam_clave), sizeof(this->tam_clave) );

        return true;
}

 bool HashingExt::insertar(Referencia ref, Clave* clave)
 {

         //Se serializa la clave
        char *clave_mem = new char[this->tam_clave]; //memoria solo para los datos que componen la clave
        this->serializar_clave(clave_mem, clave);

        if (! resolver_insercion(clave_mem, ref) )
        {
                delete [] clave_mem;
                return false;
        }

        delete []clave_mem;

        //Se persisten los atributos que pudieron haber cambiado

        this->persist.seekp(0, ios_base::beg);
        this->persist.write( (char*) &(this->tam_tabla), sizeof(this->tam_tabla) );
        this->persist.write( (char*) &(this->contador_cubos), sizeof(this->contador_cubos));
        this->persist.write( (char*) &(this->cant_cubos_libres), sizeof(this->cant_cubos_libres));

        return true;
}


bool HashingExt::BuscarReferencia(const Clave *clave, Referencia* referencia)
{
        Clave* clave_aux = const_cast<Clave*>(clave);
        return this->buscar_clave(clave_aux, referencia, BUSCAR_PARA_RECUPERAR);

}

bool HashingExt::modificar(const Clave *clave, Referencia refNueva)
{

        Clave* clave_aux = const_cast<Clave*>(clave);
        return this->buscar_clave(clave_aux, &refNueva, BUSCAR_PARA_MODIFICAR);
}

bool HashingExt::eliminar(const Clave* clave)
{
        Clave* clave_aux = const_cast<Clave*>(clave);
        bool retorno = this->buscar_clave(clave_aux, NULL, BUSCAR_PARA_ELIMINAR);

        if ( retorno == true)
        {
                //Se persiste los atributos que pudieron haber cambiado

                this->persist.seekp(0, ios_base::beg);
                this->persist.write( (char*) &(this->tam_tabla), sizeof(this->tam_tabla) );
                this->persist.write( (char*) &(this->contador_cubos), sizeof(this->contador_cubos));
                this->persist.write( (char*) &(this->cant_cubos_libres), sizeof(this->cant_cubos_libres));

        }

        return retorno;
}

void HashingExt::mostrarEstado()
{
        int ref_cubo; //variable auxiliar
        char *p_cubo = new char[this->bytes_cubo];
        Tcant_reg cant_reg;
        Ttam_disp tam_disp;
        char *un_registro = new char[this->tam_clave_ref]; //variable auxiliar
        char *una_clave = new char[this->tam_clave]; //variable auxiliar
        int numero_a_dispersar;

        Referencia una_ref;

        cout<<"**********************   Tabla de dispersion   *************************"<<endl;
        this->tabla_dispersion.seekg(0, ios_base::beg);
        for(int i=0; i < (this->tam_tabla)-1; i++)
        {
                this->tabla_dispersion.read( (char*) &ref_cubo, sizeof(ref_cubo));
                cout << ref_cubo << ", ";
        }
        this->tabla_dispersion.read( (char*) &ref_cubo, sizeof(ref_cubo));
        cout << ref_cubo <<endl;
        cout<<endl;

        cout<<"***********  Cubos  (cantidad de registros, tamanio de dispersion)  ********"<<endl;
        cout<<"****            En cada cubo el formato es (clave, referencia)          ****"<<endl;
        this->cubos.seekg(0, ios_base::beg);
        for(int j=0; j < (this->contador_cubos + 1); j++)
        {
                this->cubos.read( (char*) p_cubo, this->bytes_cubo);
                memcpy( &cant_reg, p_cubo, sizeof(Tcant_reg) );
                memcpy( &tam_disp, p_cubo + sizeof(Tcant_reg), sizeof(Ttam_disp) );

                cout<<"Cubo "<< j <<": ("<<cant_reg<<", "<<tam_disp<< ")   ";

                if( !cant_reg){
                        //el cubo está vacio
                        cout<< " - " <<endl;
                }else{

                        for(int k =0; k < cant_reg-1; k++)
                        {
                                memcpy(un_registro, p_cubo + sizeof(Tcant_reg) + sizeof(Ttam_disp) + k*(this->tam_clave_ref), this->tam_clave_ref );
                                memcpy(una_clave, un_registro, this->tam_clave);
                                memcpy(&una_ref, un_registro + this->tam_clave, sizeof(Referencia));

                                numero_a_dispersar = 0;
                                for(int i = 0; i < this->tam_clave; i++)
                                {
                                        numero_a_dispersar += pow(2, i)*(*(una_clave+i));
                                }
                                cout << numero_a_dispersar << ", " << una_ref << " || ";
                        }
                        memcpy(un_registro, p_cubo + sizeof(Tcant_reg) + sizeof(Ttam_disp) + (cant_reg -1)*(this->tam_clave_ref), this->tam_clave_ref );
                        memcpy(una_clave, un_registro, this->tam_clave);
                        memcpy(&una_ref, un_registro + this->tam_clave, sizeof(Referencia));

                        numero_a_dispersar = 0;
                        for(int v = 0; v < this->tam_clave; v++)
                        {
                                numero_a_dispersar += pow(2, v)*(*(una_clave+v));
                        }
                        cout << numero_a_dispersar << ", " << una_ref << endl;
                }
        }
        cout<<endl;

        cout<< "********************  Cubos libres   **************************"<<endl;

        if(!(this->cant_cubos_libres))
        {
                cout<< " -  "<<endl;
        }else{
                this->cubos_libres.seekg(0, ios_base::beg);
                for(int u = 0; u < (this->cant_cubos_libres-1); u++)
                {
                        this->cubos_libres.read( (char*) &ref_cubo, sizeof(ref_cubo));
                        cout << ref_cubo<< ", ";
                }
                this->cubos_libres.read( (char*) &ref_cubo, sizeof(ref_cubo));
                cout << ref_cubo<< endl;
        }
        cout<<endl;

        delete [] p_cubo;
        delete [] un_registro;
        delete [] una_clave;

}
