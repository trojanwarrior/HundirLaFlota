#ifndef GETRECORDS_H
#define GETRECORDS_H

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <string>
#include <stdexcept>
#include <vector>

using namespace std;
using namespace xercesc;

    // Error codes
    enum
    {
       ERROR_ARGS = 1, 
       ERROR_XERCES_INIT,
       ERROR_PARSE,
       ERROR_EMPTY_DOCUMENT
    };
    
    //Estructura de un record
    struct record 
    {
      int posicion;
      string nombre;
      int puntuacion;
    };


class GetRecords
{
    public:
       GetRecords();
      ~GetRecords();
       void readRecords(string& rutaRecords) throw(std::runtime_error);
       std::vector<record> Records();
     

    private:
       xercesc::XercesDOMParser *_recordFileParser;
       
       std::vector<record> _records;

       // Internal class use only. Hold Xerces data in UTF-16 SMLCh type.
       XMLCh *TAG_recordSet;
       XMLCh *TAG_record;
       XMLCh *ATTR_nombre;
       XMLCh *ATTR_posicion;
       XMLCh *ATTR_puntuacion;
};
#endif