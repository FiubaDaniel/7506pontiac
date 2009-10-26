#ifndef FABRICADERECURSOSALMACENAMIENTO_H
#define FABRICADERECURSOSALMACENAMIENTO_H

#include <string>
#include "RecursoAlmacenamiento.h"
#include "Compuesto.h"
#include "Componente.h"
#include ""

class FabricaDeRecursosAlmacenamiento
{
public:
  FabricaDeRecursosAlmacenamiento ( );
  virtual ~FabricaDeRecursosAlmacenamiento ( );
  RecursoAlmacenamiento RecursoDeAlmacenamientoEnBuffer(std::string path, Compuesto unCompuesto, Componente unComponente);

  RecursoAlmacenamiento RecursoDeAlmacenamientoEnArchivo(std::string path, Compuesto unCompuesto, Componente unComponente);
  RecursoAlmacenamiento RecursoDeAlmacenamientoEnArchivoBsharp(std::string path, Compuesto unCompuesto, Componente unComponente);
  RecursoAlmacenamiento RecursoDeAlmacenamientoEnArchivoHash(std::string path, Compuesto unCompuesto, Componente unComponente);

  RecursoAlmacenamiento RecursoDeAlmacenamientoEnArchivoConBuffer(std::string path, Compuesto unCompuesto, Componente unComponente);
  RecursoAlmacenamiento RecursoDeAlmacenamientoEnArchivoConBufferBsharp(std::string path, Compuesto unCompuesto, Componente unComponente);
  RecursoAlmacenamiento RecursoDeAlmacenamientoEnArchivoConBufferHash(std::string path, Compuesto unCompuesto, Componente unComponente);


private:

};

#endif // FABRICADERECURSOSALMACENAMIENTO_H
