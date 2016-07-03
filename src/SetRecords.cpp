#include <string>

#include <iostream>
#include "SetRecords.h"

using namespace std;
using namespace xercesc;

SetRecords::SetRecords()
{
    _record.nombre = "";
    _record.puntuacion = 0;
    
    _rec = NULL;
    
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch(const XMLException& toCatch)
    {
        char *pMsg = XMLString::transcode(toCatch.getMessage());
        XERCES_STD_QUALIFIER cerr << "Error during Xerces-c Initialization.\n"
             << "  Exception message:"
             << pMsg;
        XMLString::release(&pMsg);
    }

    
    XMLCh tempStr[100];
    XMLString::transcode("LS", tempStr, 99);
    _impl = DOMImplementationRegistry::getDOMImplementation(tempStr);
    _serializer = ((DOMImplementationLS*)_impl)->createLSSerializer();

    // optionally you can set some features on this serializer
    if (_serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true))
        _serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true);

    if (_serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
        _serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);

//    _myFormTarget = new LocalFileFormatTarget("media/records.xml");
//    _theOutput = ((DOMImplementationLS*)_impl)->createLSOutput();
//    _theOutput->setByteStream(_myFormTarget);

    recuperaRecords();
    
}

SetRecords::~SetRecords()
{

    if (_theOutput) _theOutput->release();
    if (_serializer) _serializer->release();
    if (_myFormTarget) delete _myFormTarget;

}

void SetRecords::setRecordAIntroducir(string nombre, int puntuacion)
{
    _record.nombre = nombre;
    _record.puntuacion = puntuacion;
}

bool SetRecords::recuperaRecords()
{
    _rec = new GetRecords();
    
    try
    {
        if (_rec)
        {
            string a = "media/records.xml";
            _rec->readRecords(a);
            return true;
        }
    }
    catch (std::exception &e)
    {
      cout << "el fichero de records probablemente no existe, habrá que jugar para que se creen nuevos records. \n";
    }
        
    return false;
    
}

bool compara(const record& first, const record& second)
{
  return ( first.puntuacion > second.puntuacion );
}

bool SetRecords::entra()
{
    if (_rec)
    {
        if (_rec->Records().size() < 9)  // Si había menos de 9 entra si o si.
            return true;
        
        for (size_t i=0; i< _rec->Records().size(); i++)
            if (_rec->Records()[i].puntuacion < _record.puntuacion)
                return true;
    }
    
    return false;
}

void SetRecords::guardaRecord()
{
//    try
//    {
//        XMLPlatformUtils::Initialize();
//    }
//    catch(const XMLException& toCatch)
//    {
//        char *pMsg = XMLString::transcode(toCatch.getMessage());
//        XERCES_STD_QUALIFIER cerr << "Error during Xerces-c Initialization.\n"
//             << "  Exception message:"
//             << pMsg;
//        XMLString::release(&pMsg);
//    }
    
    
    for (size_t i=0; i<_rec->Records().size(); i++)
        _listaRecords.push_back(_rec->Records()[i]);
        
    if (_rec) delete _rec; // destruimos _rec para que libere los recursos xml

    _listaRecords.push_back(_record); // Añado el record nuevo a la lista y la ordeno para OUTPUTEARLA en orden 
    _listaRecords.sort(compara);
    
    //Estas tres lineas se cargan el fichero que hubiera, de momento es lo que he podido averiguar. Si existe  
    //el fichero crea uno nuevo y se carga lo anterior por eso las pongo después de haber leído los records actuales
    //de lo contrario solo existirá el que intentemos escribir y siempre entrará por que solo hay uno de 9 posibles records
    //xerces será muy potente pero cuesta una "jartá" saber como usarlo :(
    _myFormTarget = new LocalFileFormatTarget("media/records.xml");
    _theOutput = ((DOMImplementationLS*)_impl)->createLSOutput();
    _theOutput->setByteStream(_myFormTarget);

    if (_impl != NULL)
    {
        try
        {
           DOMDocument* doc = _impl->createDocument(
                       0,                                          // root element namespace URI.
                       XMLString::transcode("record_set"),         // root element name
                       0);                                         // document type object (DTD).

           DOMElement* rootElem = doc->getDocumentElement();
           std::list<record>::iterator it;
           for (it = _listaRecords.begin(); it != _listaRecords.end(); ++it)
           {
               DOMElement*  recordElem = doc->createElement(XMLString::transcode("record"));
               recordElem->setAttribute(XMLString::transcode("nombre"), XMLString::transcode(it->nombre.c_str()));
               std::stringstream aux;
               aux << it->puntuacion;
               recordElem->setAttribute(XMLString::transcode("puntuacion"),XMLString::transcode(aux.str().c_str()));
               
               rootElem->appendChild(recordElem);
               

               // Now count the number of elements in the above DOM tree.
               const XMLSize_t elementCount = doc->getElementsByTagName(XMLString::transcode("*"))->getLength();
               XERCES_STD_QUALIFIER cout << "The tree just created contains: " << elementCount
                    << " elements." << XERCES_STD_QUALIFIER endl;
           } 
           serializeDOM(doc);
           doc->release();
        }
        catch (const OutOfMemoryException&)
        {
           XERCES_STD_QUALIFIER cerr << "OutOfMemoryException" << XERCES_STD_QUALIFIER endl;
        }
        catch (const DOMException& e)
        {
           XERCES_STD_QUALIFIER cerr << "DOMException code is:  " << e.code << XERCES_STD_QUALIFIER endl;
        }
        catch (...)
        {
           XERCES_STD_QUALIFIER cerr << "An error occurred creating the document" << XERCES_STD_QUALIFIER endl;
        }
    }  // (inpl != NULL)
    else
    {
       XERCES_STD_QUALIFIER cerr << "Requested implementation is not supported" << XERCES_STD_QUALIFIER endl;
    }
    
}

int SetRecords::serializeDOM(DOMNode* node) 
{


    try {
        // do the serialization through DOMLSSerializer::write();
        _serializer->write(node, _theOutput);
    }
    catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "Exception message is: \n"
             << message << "\n";
        XMLString::release(&message);
        return -1;
    }
    catch (const DOMException& toCatch) {
        char* message = XMLString::transcode(toCatch.msg);
        cout << "Exception message is: \n"
             << message << "\n";
        XMLString::release(&message);
        return -1;
    }
    catch (...) {
        cout << "Unexpected Exception \n" ;
        return -1;
    }

    return 0;
}    

