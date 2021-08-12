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

const char** string_parser(const char* head,int size){

	const char** return_string=nullptr;
	return_string=new char*[size];//bunu yapmadan da dene




	int index=0;
	int inner_index=0;

	while(index<size){

		if(*head!=' '){
			index++;
			return_string[index]=new char[10];

		}else{

			return_string[index][inner_index]=*head
			inner_index++;
		}

		head++;
	}

	return return_string;

}


void generate_map(/*string model, string pose*/){

	XMLDocument xml_doc;

	XMLError eResult = xml_doc.LoadFile("new_model/model.sdf");

	XMLElement* root=xml_doc.FirstChildElement("sdf");

	XMLElement* model = root->FirstChildElement("model");

	XMLElement* link = model->FirstChildElement("link");

	

	for(; link != nullptr; link=model->NextSiblingElement("link")){

			
		XMLElement* iterator=xml_iterator("collision",link);
		XMLElement* geo=xml_iterator("geometry",iterator);
		XMLElement* pose=xml_iterator("pose",link);

		const char* position_attitude=pose->GetText();


		char** pose_string = string_parser(position_attitude,6);



		if(geo->FirstChildElement()=="box"){

			XMLElement* size=xml_iterator("size",geo);
			char* size_value=size->GetText();

			char** size_string=string_parser(size_value,3);

			Box new_box=Box(pose_string[0],pose_string[1],pose_string[2],
			pose_string[3],pose_string[4],pose_string[5],size_string[0],size_string[1]
			,size_string[2]);

			new_box.put_map(image,scale);

		}else if(geo->FirstChildElement()=="sphere"){

			XMLElement* radius=xml_iterator("radius",geo);
			char* radius_value=radius->GetText();

			char** radius_string=string_parser(radius_value,1);

			Sphere new_sphere=Sphere(pose_string[0],pose_string[1],pose_string[2],
			pose_string[3],pose_string[4],pose_string[5],radius_string[0]);

			new_sphere.put_map(image,scale);

		}else if(geo->FirstChildElement()=="cylinder"){


			XMLElement* radius=xml_iterator("radius",geo);
			XMLElement* height=xml_iterator("height",geo);
			char* radius_value=radius->GetText();
			char* height_value=height->GetText();

			char** radius_string=string_parser(radius_value,1);
			char** height_string=string_parser(height_value,1);

			Sphere new_cylinder=Sphere(pose_string[0],pose_string[1],pose_string[2],
			pose_string[3],pose_string[4],pose_string[5],radius_string[0]
			,height_string[0]);

			new_cylinder.put_map(image,scale);
		}
		

	}	

}
