#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>
//#include <iostream>
#include <sstream>
#include <list>
#include "GetRecords.h"

using namespace std;
    
class SetRecords
{
    public:
        SetRecords();
        ~SetRecords();
        
        int serializeDOM(DOMNode* node);
        bool recuperaRecords();
        bool entra();
        void guardaRecord();
        void setRecordAIntroducir(string nombre, int puntuacion);
        
        
        DOMLSSerializer* _serializer;
        DOMImplementation *_impl;
        XMLFormatTarget *_myFormTarget;
        DOMLSOutput* _theOutput;
        

        
        
        
        GetRecords *_rec;
        string _nombre;
        int _puntuacion;
        record _record;
        
        std::list<record> _listaRecords;
};

/*
// construct the DOMWriter
DOMWriter* myWriter = ((DOMImplementationLS*)impl)->createDOMWriter();

// construct the LocalFileFormatTarget
XMLFormatTarget *myFormatTarget = new LocalFileFormatTarget("myXMLFile.xml");

// serialize a DOMNode to the local file "myXMLFile.xml"
myWriter->writeNode(myFormatTarget, *aDOMNode);

// optionally, you can flush the buffer to ensure all contents are written
myFormatTarget->flush();

// release the memory
myWriter->release();
delete myFormatTarget; 
*/