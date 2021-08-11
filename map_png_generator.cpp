#include "map_generator.hpp"
#include "tinyxml2.h"
#include <iostream>
using namespace std;
using namespace tinyxml2;

XMLElement* xml_iterator(char* target, XMLElement* root){

	XMLElement* current;

	if(root->Name()==target)
		return root;

	if(root->Name()!=target && root->FirstChildElement()!=nullptr){

		current = xml_iterator(target,root->FirstChildElement());
	}else if(root->Name()!=target){
		while(root->NextSiblingElement()!=nullptr){
			current = xml_iterator(target,root->NextSiblingElement());
		}
		
	}

	return current;

}

void generate_map(/*string model, string pose*/){

	XMLDocument xml_doc;

	XMLError eResult = xml_doc.LoadFile("new_model/model.sdf");

	XMLElement* root=xml_doc.FirstChildElement("sdf");

	XMLElement* model = root->FirstChildElement("model");

	XMLElement* link = model->FirstChildElement("link");

	

	for(; link != nullptr; link=model->NextSiblingElement("link")){

			

		

	}

	

	

	

}
