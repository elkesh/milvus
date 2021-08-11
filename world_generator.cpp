#include <iostream>
#include "xml_interface.hpp"
#include "tinyxml2.h"
#include "world_interface.hpp"
#include <string>
using namespace std;
using namespace tinyxml2;


void add_model(XMLDocument &doc,string model_name,XMLElement *world, string pose){

	XMLElement *terrain = doc.NewElement("include");
    world->InsertEndChild(terrain);

    XMLElement *terrain_uri = doc.NewElement("uri");
    terrain_uri->SetText(caster("model://"+model_name));
    terrain->InsertEndChild(terrain_uri);

    XMLElement *pPose = doc.NewElement("pose");
    pPose->SetText(caster(pose));
    terrain->InsertEndChild(pPose);


}

void generator(double rows, double colms,double scale){

	XMLDocument xmlDoc;

	XMLDeclaration* declaration=xmlDoc.NewDeclaration();
    xmlDoc.InsertFirstChild(declaration);

    XMLElement *sdfversion = xmlDoc.NewElement("sdf");
    sdfversion->SetAttribute("version","1.5");
    xmlDoc.InsertEndChild(sdfversion);

    XMLElement *pWorld = xmlDoc.NewElement("world");
    pWorld->SetAttribute("name","default");
    sdfversion->InsertEndChild(pWorld);

    string walls_x;
    string walls_y;

    walls_y=to_string(-(rows/2)*scale);
    walls_x=to_string(-(colms/2)*scale);

    add_model(xmlDoc,"new_model",pWorld,walls_x+" "+walls_y+" 0 0 0 0");


	

	
	string model;
	string pose;
	char disected_pose[11];

	
	getline(cin,model);


	while(1){

		cout<<"give the name and position of the models that you want to generate inside the world."<<endl;
		cout<<"give the name press enter for next one. enter q to quit."<<endl;

				
		getline(cin,model);
		
		cout<<model<<endl;
		
		

		if(model=="q")
			break;
		else{

			cout<<"give the position of the model relative to the origin of the world"<<endl;
			

			getline(cin,pose);
			
			cout<<"uras"<<endl;
			
			add_model(xmlDoc,model,pWorld,pose);


		}

		

	}


	XMLElement *sun = xmlDoc.NewElement("include");
    pWorld->InsertEndChild(sun);

    XMLElement *sun_uri= xmlDoc.NewElement("uri");
    sun_uri->SetText("model://sun");
    sun->InsertEndChild(sun_uri);

    XMLError eResult = xmlDoc.SaveFile("generated_world.world");




}