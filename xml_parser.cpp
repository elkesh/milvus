#include <iostream>
#include "xml_interface.hpp"
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;

struct basic_model{

    string x;
    string y;
    string z;
    string x_size;
    string y_size;
    string height;

};
const char* caster(string a){

    return a.c_str();

}

void generateWall(int** rectangle,XMLDocument &doc,int number,XMLElement* root,double scale){

    double x1=rectangle[(number-1)][0];
    double y1=rectangle[(number-1)][1];
    double x2=rectangle[(number-1)][2];
    double y2=rectangle[(number-1)][3];

    
    basic_model wall;
    wall.x=to_string((x2+x1)/2*scale);
    wall.y=to_string((y2+y1)/2*scale);
    wall.x_size=to_string(((x2-x1+1)+0.125)*scale);//0.125ler
    wall.y_size=to_string(((y2-y1+1)+0.125)*scale);//çözülecek
    wall.height=to_string(15*scale);
    wall.z=to_string(15.0/2.0*scale);

    string wall_number=to_string(number);   

    
    

    XMLElement *pElement =doc.NewElement("link");
    pElement->SetAttribute("name",caster("wall"+wall_number));
    root->InsertEndChild(pElement);

    XMLElement *pElement2=doc.NewElement("collision");
    pElement2->SetAttribute("name",caster("wall"+wall_number+"_collision"));
    pElement->InsertEndChild(pElement2);

    XMLElement *pElement3=doc.NewElement("geometry");
    pElement2->InsertEndChild(pElement3);

    XMLElement *pElement4=doc.NewElement("box");
    pElement3->InsertEndChild(pElement4);

    XMLElement *pElement5=doc.NewElement("size");
    pElement5->SetText(caster(wall.x_size+" "+wall.y_size+" "+wall.height));
    pElement4->InsertEndChild(pElement5);

    pElement3=doc.NewElement("pose");
    pElement3->SetAttribute("frame","");
    pElement3->SetText(caster(wall.x+" "+wall.y+" "+wall.z+"0 0 0"));
    pElement2->InsertEndChild(pElement3);

    pElement2=doc.NewElement("visual");
    pElement2->SetAttribute("name",caster("Wall_"+wall_number+"_Visual"));
    pElement->InsertEndChild(pElement2);

    pElement3=doc.NewElement("pose");
    pElement3->SetAttribute("frame","");
    pElement3->SetText(caster(wall.x+" "+wall.y+" "+wall.z+"0 0 0"));
    pElement2->InsertEndChild(pElement3);

    pElement3=doc.NewElement("geometry");
    pElement2->InsertEndChild(pElement3);

    pElement4=doc.NewElement("box");
    pElement3->InsertEndChild(pElement4);

    pElement5=doc.NewElement("size");
    pElement5->SetText(caster(wall.x_size+" "+wall.y_size+" "+wall.height));
    pElement4->InsertEndChild(pElement5);

    pElement3=doc.NewElement("material");
    pElement2->InsertEndChild(pElement3);

    pElement4=doc.NewElement("script");
    pElement3->InsertEndChild(pElement4);

    pElement5=doc.NewElement("uri");
    pElement5->SetText("file://media/materials/scripts/gazebo.material");
    pElement4->InsertEndChild(pElement5);

    pElement5=doc.NewElement("name");
    pElement5->SetText("Gazebo/Grey");
    pElement4->InsertEndChild(pElement5);

    pElement4=doc.NewElement("ambient");
    pElement4->SetText("1 1 1 1");
    pElement3->InsertEndChild(pElement4);

    pElement3=doc.NewElement("meta");
    pElement2->InsertEndChild(pElement3);

    pElement4=doc.NewElement("layer");
    pElement4->SetText("0");
    pElement3->InsertEndChild(pElement4);

    pElement2=doc.NewElement("pose");
    pElement2->SetAttribute("frame","");
    pElement2->SetText(caster("0 0 0 0 0 0"));
    pElement->InsertEndChild(pElement2);
    
    pElement=doc.NewElement("static");
    pElement->SetText("1");
    root->InsertEndChild(pElement);
    


}

void parser_func(int** rectangles,double scale)
{   
    //int rectangles[2][4]={{13,47,86,56},{55,83,71,93}};
    XMLDocument xmlDoc;
    
    XMLDeclaration* declaration=xmlDoc.NewDeclaration();
    xmlDoc.InsertFirstChild(declaration);
    

    XMLElement *sdfversion = xmlDoc.NewElement("sdf");
    sdfversion->SetAttribute("version","1.6");
    xmlDoc.InsertEndChild(sdfversion);

    XMLElement *pRoot=xmlDoc.NewElement("model");
    pRoot->SetAttribute("name","new_model");
    sdfversion->InsertEndChild(pRoot);

    XMLElement *pFrame=xmlDoc.NewElement("pose");
    pFrame->SetAttribute("frame","");
    pFrame->SetText("0 0 0 0 0 0");
    pRoot->InsertEndChild(pFrame);

    int index=0;

    while(rectangles[index][0]!=-123){

        generateWall(rectangles,xmlDoc,index+1,pRoot,scale);
        index++;

    }

    

    XMLError eResult = xmlDoc.SaveFile("new_model/model.sdf");






}