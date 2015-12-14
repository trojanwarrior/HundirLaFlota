#include <string>

#include <iostream>
#include "SetRecords.h"

using namespace std;
using namespace xercesc;

SetRecords::SetRecords(string nombre, int puntuacion)
{
    _record.nombre = nombre;
    _record.puntuacion = puntuacion;
    
    _rec = NULL;
}

bool SetRecords::recuperaRecords()
{
    _rec = new GetRecords();
    
    if (_rec)
        return true;
    else 
        return false;
    
}

bool compara(const record& first, const record& second)
{
  return ( first.puntuacion < second.puntuacion );
}

bool SetRecords::entra()
{
    if (_rec)
    {
        if (_rec->Records().size() < 9)  // Si había menos de 9 entra si o si.
            return true;
        
        for (size_t i=0; i< _rec->Records().size(); i++) // volcamos el vector de records actual a una lista.
            if (_rec->Records()[i].puntuacion < _record.puntuacion)
                return true;
    }
    
    return false;
}

void SetRecords::guardaRecord()
{
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
    
    if (_rec) delete _rec; // destruimos _rec para que libere los recursos xml
    
    for (size_t i=0; i<_rec->Records().size(); i++)
        _listaRecords.push_back(_rec->Records()[i]);
    
    _listaRecords.sort(compara);
    
    DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("Core"));

    if (impl != NULL)
    {
        try
        {
           DOMDocument* doc = impl->createDocument(
                       0,                                          // root element namespace URI.
                       XMLString::transcode("record_set"),         // root element name
                       0);                                         // document type object (DTD).

           DOMElement* rootElem = doc->getDocumentElement();
           serializeDOM(rootElem);
           std::list<record>::iterator it;
           for (it = _listaRecords.begin(); it != _listaRecords.end(); ++it)
           {
               DOMElement*  recordElem = doc->createElement(XMLString::transcode("record"));
               recordElem->setAttribute(XMLString::transcode("nombre"), XMLString::transcode(it->nombre.c_str()));
               std::stringstream aux;
               aux << it->puntuacion;
               recordElem->setAttribute(XMLString::transcode("puntuacion"),XMLString::transcode(aux.str().c_str()));
               
               rootElem->appendChild(recordElem);
               
               serializeDOM(rootElem);

               // Now count the number of elements in the above DOM tree.
               const XMLSize_t elementCount = doc->getElementsByTagName(XMLString::transcode("*"))->getLength();
               XERCES_STD_QUALIFIER cout << "The tree just created contains: " << elementCount
                    << " elements." << XERCES_STD_QUALIFIER endl;
           } 
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
   
    
//    XMLCh tempStr[100];
//    XMLString::transcode("LS", tempStr, 99);
//    impl = DOMImplementationRegistry::getDOMImplementation(tempStr);
//    _serializer = ((DOMImplementationLS*)impl)->createLSSerializer();
//
//    // optionally you can set some features on this serializer
//    if (_serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true))
//        _serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true);
//
//    if (_serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
//        _serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
//
//    myFormTarget = new LocalFileFormatTarget("media/records.xml");
//    theOutput = ((DOMImplementationLS*)impl)->createLSOutput();
//    theOutput->setByteStream(myFormTarget);
}

int SetRecords::serializeDOM(DOMNode* node) 
{


    try {
        // do the serialization through DOMLSSerializer::write();
        _serializer->write(node, theOutput);
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

    theOutput->release();
    _serializer->release();
    delete myFormTarget;
    return 0;
}    

