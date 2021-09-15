#include <iostream>
#include "../include/xml_interface.hpp"
#include "tinyxml2.h"
#include "../include/map_generator.hpp"
#include "../include/model_interface.hpp"

struct basic_model{
  std::string x;
  std::string y;
  std::string z;
  std::string x_size;
  std::string y_size;
  std::string height;
};
const char* caster(std::string a){
  return a.c_str();
}

void generateWall(int** rectangle,tinyxml2::XMLDocument &doc,int number,tinyxml2::XMLElement* root,double scale,cv::Mat &image){
  double x1=rectangle[(number-1)][0];
  double y1=rectangle[(number-1)][1];
  double x2=rectangle[(number-1)][2];
  double y2=rectangle[(number-1)][3];
  
  Box wall;
  wall.pose.x=std::to_string((x2+x1)/2*scale);
  wall.pose.y=std::to_string((y2+y1)/2*scale);
  wall.pose.z=std::to_string(15.0/2.0*scale);
  wall.size.x_size=std::to_string(((x2-x1+1)+0.125)*scale);//0.125ler
  wall.size.y_size=std::to_string(((y2-y1+1)+0.125)*scale);//çözülecek
  wall.size.z_size=std::to_string(15*scale);

  wall.put_map(image,scale,1);

  std::string wall_number=std::to_string(number);   

  tinyxml2::XMLElement *pElement =doc.NewElement("link");
  pElement->SetAttribute("name",caster("wall"+wall_number));
  root->InsertEndChild(pElement);

  tinyxml2::XMLElement *pElement2=doc.NewElement("collision");
  pElement2->SetAttribute("name",caster("wall"+wall_number+"_collision"));
  pElement->InsertEndChild(pElement2);

  tinyxml2::XMLElement *pElement3=doc.NewElement("geometry");
  pElement2->InsertEndChild(pElement3);

  tinyxml2::XMLElement *pElement4=doc.NewElement("box");
  pElement3->InsertEndChild(pElement4);

  tinyxml2::XMLElement *pElement5=doc.NewElement("size");
  pElement5->SetText(caster(wall.size.x_size+" "+wall.size.y_size+" "+wall.size.z_size));
  pElement4->InsertEndChild(pElement5);

  pElement3=doc.NewElement("pose");
  pElement3->SetAttribute("frame","");
  pElement3->SetText(caster(wall.pose.x+" "+wall.pose.y+" "+wall.pose.z+" 0 0 0"));
  pElement2->InsertEndChild(pElement3);

  pElement2=doc.NewElement("visual");
  pElement2->SetAttribute("name",caster("Wall_"+wall_number+"_Visual"));
  pElement->InsertEndChild(pElement2);

  pElement3=doc.NewElement("pose");
  pElement3->SetAttribute("frame","");
  pElement3->SetText(caster(wall.pose.x+" "+wall.pose.y+" "+wall.pose.z+" 0 0 0"));
  pElement2->InsertEndChild(pElement3);

  pElement3=doc.NewElement("geometry");
  pElement2->InsertEndChild(pElement3);

  pElement4=doc.NewElement("box");
  pElement3->InsertEndChild(pElement4);

  pElement5=doc.NewElement("size");
  pElement5->SetText(caster(wall.size.x_size+" "+wall.size.y_size+" "+wall.size.z_size));
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

void parser_func(int** rectangles,double scale,cv::Mat &image)
{   
  //int rectangles[2][4]={{13,47,86,56},{55,83,71,93}};
  tinyxml2::XMLDocument xmlDoc;
  
  tinyxml2::XMLDeclaration* declaration=xmlDoc.NewDeclaration();
  xmlDoc.InsertFirstChild(declaration);
  

  tinyxml2::XMLElement *sdfversion = xmlDoc.NewElement("sdf");
  sdfversion->SetAttribute("version","1.6");
  xmlDoc.InsertEndChild(sdfversion);

  tinyxml2::XMLElement *pRoot=xmlDoc.NewElement("model");
  pRoot->SetAttribute("name","new_model");
  sdfversion->InsertEndChild(pRoot);

  tinyxml2::XMLElement *pFrame=xmlDoc.NewElement("pose");
  pFrame->SetAttribute("frame","");
  pFrame->SetText("0 0 0 0 0 0");
  pRoot->InsertEndChild(pFrame);
  int index=0;

  while(rectangles[index][0]!=-123){

    generateWall(rectangles,xmlDoc,index+1,pRoot,scale,image);
    index++;

  } 

  tinyxml2::XMLError eResult = xmlDoc.SaveFile("new_model/model.sdf");
}
