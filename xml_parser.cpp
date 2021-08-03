#include <iostream>
#include "xml_interface.hpp"
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;

void generateWall(int rectangle[4]){

    std::string x1=std::to_string(rectangle[0]);
    std::string y1=std::to_string(rectangle[1]);
    std::string x2=std::to_string(rectangle[2]);
    std::string y2=std::to_string(rectangle[3]);
    

    XMLElement *pElement = xmlDoc.NewElement("pose");
    pElement->SetAttribute("frame","");
    pElement->SetText(x1+" "+y1+" 0 0 0 0");
    pRoot->InsertEndChild(pElement);

    pElement=xmlDoc.NewElement("link");
    pElement->SetAttribute("name","wall2");
    pRoot->InsertEndChild(pElement);

    XMLElement *pElement2=xmlDoc.NewElement("collision");
    pElement2->SetAttribute("name","wall2_collision");
    pElement->InsertEndChild(pElement2);

    XMLElement *pElement3=xmlDoc.NewElement("geometry");
    pElement2->InsertEndChild(pElement3);

    XMLElement *pElement4=xmlDoc.NewElement("box");
    pElement3->InsertEndChild(pElement4);

    XMLElement *pElement5=xmlDoc.NewElement("size");
    pElement5->SetText("5.25 0.15 2.5");
    pElement4->InsertEndChild(pElement5);

    pElement3=xmlDoc.NewElement("pose");
    pElement3->SetAttribute("frame","");


}

void parser_func(int *rectangles)
{   
    XMLDocument xmlDoc;
    
    XMLDeclaration* declaration=xmlDoc.NewDeclaration();
    xmlDoc.InsertFirstChild(declaration);
    

    XMLElement *sdfversion = xmlDoc.NewElement("sdf");
    sdfversion->SetAttribute("version","1.6");
    xmlDoc.InsertEndChild(sdfversion);

    XMLElement *pRoot=xmlDoc.NewElement("model");
    pRoot->SetAttribute("name","uras");
    sdfversion->InsertEndChild(pRoot);

    int index=0;

    while(*rectangles[index][0]!=-123){

        generateWall(rectangles[index]);

    }

    

    XMLError eResult = xmlDoc.SaveFile("last.xml");






}

