#include <iostream>
#include "../include/xml_interface.hpp"
#include "tinyxml2.h"
#include "../include/world_interface.hpp"
#include <string>
#include "../include/map_generator.hpp"




void add_model(tinyxml2::XMLDocument &doc,std::string model_name,tinyxml2::XMLElement *world, const char* pose){

  tinyxml2::XMLElement *terrain = doc.NewElement("include");
    world->InsertEndChild(terrain);

    tinyxml2::XMLElement *terrain_uri = doc.NewElement("uri");
    terrain_uri->SetText(caster("model://"+model_name));
    terrain->InsertEndChild(terrain_uri);

    tinyxml2::XMLElement *pPose = doc.NewElement("pose");
    pPose->SetText(pose);
    terrain->InsertEndChild(pPose);
}

void generator(double rows, double colms,double scale,cv::Mat &image){
    tinyxml2::XMLDocument xmlDoc;

    tinyxml2::XMLDeclaration* declaration=xmlDoc.NewDeclaration();
    xmlDoc.InsertFirstChild(declaration);

    tinyxml2::XMLElement *sdfversion = xmlDoc.NewElement("sdf");
    sdfversion->SetAttribute("version","1.5");
    xmlDoc.InsertEndChild(sdfversion);

    tinyxml2::XMLElement *pWorld = xmlDoc.NewElement("world");
    pWorld->SetAttribute("name","default");
    sdfversion->InsertEndChild(pWorld);

    std::string walls_x;
    std::string walls_y;

    walls_y=std::to_string(-(rows/2)*scale);
    walls_x=std::to_string(-(colms/2)*scale);

    add_model(xmlDoc,"new_model",pWorld,caster(walls_x+" "+walls_y+" 0 0 0 0"));
  
  std::string model;
  std::string pose;
  
  getline(std::cin,model);
  while(1){
    std::cout<<"give the name and position of the models that you want to generate inside the world."<<std::endl;
    std::cout<<"give the name press enter for next one. enter q to quit."<<std::endl;        
    getline(std::cin,model);
    if(model=="q")
      break;
    else{

      std::cout<<"give the position of the model relative to the origin of the world"<<std::endl;
      
      getline(std::cin,pose);
      add_model(xmlDoc,model,pWorld,caster(pose));
      generate_map(model,caster(pose),image,scale);
    } 
  }  
    tinyxml2::XMLElement *sun = xmlDoc.NewElement("include");
    pWorld->InsertEndChild(sun);

    tinyxml2::XMLElement *sun_uri= xmlDoc.NewElement("uri");
    sun_uri->SetText("model://sun");
    sun->InsertEndChild(sun_uri);

    tinyxml2::XMLError eResult = xmlDoc.SaveFile("generated_world.world");
}