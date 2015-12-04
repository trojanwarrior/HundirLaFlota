#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <list>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "GetRecords.h"

using namespace xercesc;
using namespace std;

/**
 *  Constructor initializes xerces-C libraries.
 *  The XML tags and attributes which we seek are defined.
 *  The xerces-C DOM parser infrastructure is initialized.
 */

GetRecords::GetRecords()
{
   try
   {
      XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
   }
   catch( XMLException& e )
   {
      char* message = XMLString::transcode( e.getMessage() );
      cerr << "XML toolkit initialization error: " << message << endl;
      XMLString::release( &message );
      // throw exception here to return ERROR_XERCES_INIT
   }

   // Tags and attributes used in XML file.
   // Can't call transcode till after Xerces Initialize()
   TAG_recordSet        = XMLString::transcode("record_set");
   TAG_record = XMLString::transcode("record");
   ATTR_nombre = XMLString::transcode("nombre");
   ATTR_posicion = XMLString::transcode("posicion");
   ATTR_puntuacion = XMLString::transcode("puntuacion");

   _recordFileParser = new XercesDOMParser;
}

/**
 *  Class destructor frees memory used to hold the XML tag and 
 *  attribute definitions. It als terminates use of the xerces-C
 *  framework.
 */
GetRecords::~GetRecords()
{
   // Free memory

   delete _recordFileParser;

   try
   {
      XMLString::release( &TAG_recordSet );

      XMLString::release( &TAG_record );
      XMLString::release( &ATTR_nombre );
      XMLString::release( &ATTR_posicion );
      XMLString::release( &ATTR_puntuacion);
   }
   catch( ... )
   {
      cerr << "A saber \"my weapon\"" << endl;
   }

   // Terminate Xerces

   try
   {
      XMLPlatformUtils::Terminate();  // Terminate after release of memory
   }
   catch( xercesc::XMLException& e )
   {
      char* message = xercesc::XMLString::transcode( e.getMessage() );

      cerr << "XML error: " << message << endl;
      XMLString::release( &message );
   }
}

/**
 *  This function:
 *  - Tests the access and availability of the XML configuration file.
 *  - Configures the xerces-c DOM parser.
 *  - Reads and extracts the pertinent information from the XML config file.
 *
 *  @param in configFile The text string name of the HLA configuration file.
 */
void GetRecords::readRecords(string& rutaRecords) throw( std::runtime_error )
{
   // Test to see if the file is ok.
   struct stat fileStatus;

   errno = 0;
   if(stat(rutaRecords.c_str(), &fileStatus) == -1) // ==0 ok; ==-1 error
   {
       if( errno == ENOENT )      // errno declared by include file errno.h
          throw ( std::runtime_error("Ruta erronea.") );
       else if( errno == ENOTDIR )
          throw ( std::runtime_error("Algo en la ruta esta mal."));
       else if( errno == ELOOP )
          throw ( std::runtime_error("Demasiados enlaces simbolicos mientras se trazaba la ruta."));
       else if( errno == EACCES )
          throw ( std::runtime_error("Permiso denegado. ¿Donde has instalado el juego \"my weapon\"?"));
       else if( errno == ENAMETOOLONG )
          throw ( std::runtime_error("El fichero no se puede leer\n"));
   }

   // Configure DOM parser.

   _recordFileParser->setValidationScheme( XercesDOMParser::Val_Never );
   _recordFileParser->setDoNamespaces( false );
   _recordFileParser->setDoSchema( false );
   _recordFileParser->setLoadExternalDTD( false );

   try
   {
      _recordFileParser->parse(rutaRecords.c_str() );

      // no need to free this pointer - owned by the parent parser object
      DOMDocument* xmlDoc = _recordFileParser->getDocument();

      // Get the top-level element: recordset is "root". No attributes for "root"
      
      DOMElement* elementRoot = xmlDoc->getDocumentElement();
      if( !elementRoot ) throw(std::runtime_error( "El fichero XML esta vacío " ));

      // Parse para los tags de interes: "record"
      // Look one level nested within "root". (child of root)

      DOMNodeList*      children = elementRoot->getChildNodes();
      const  XMLSize_t nodeCount = children->getLength();

      // For all nodes, children of "root" in the XML tree.

      for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
      {
         DOMNode* currentNode = children->item(xx);
         if( currentNode->getNodeType() &&  // true is not NULL
             currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element 
         {
            // Found node which is an Element. Re-cast node as element
            DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );
            if( XMLString::equals(currentElement->getTagName(),TAG_record))
            {
               // Already tested node as type element and of name "record".
               // Read attributes of element "ApplicationSettings".
                record r;
                r.posicion = atoi(XMLString::transcode(currentElement->getAttribute(ATTR_posicion)));
                r.puntuacion = atoi(XMLString::transcode(currentElement->getAttribute(ATTR_puntuacion)));
                r.nombre = XMLString::transcode(currentElement->getAttribute(ATTR_nombre));
                _records.push_back(r);
            }
         }
      }
   }
   catch( xercesc::XMLException& e )
   {
      char* message = xercesc::XMLString::transcode( e.getMessage() );
      ostringstream errBuf;
      errBuf << "Error parsing file: " << message << flush;
      XMLString::release( &message );
   }
}

std::vector<record> GetRecords::Records()
{
    return _records;
}