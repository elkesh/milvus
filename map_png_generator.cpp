#include "map_generator.hpp"
#include "tinyxml2.h"
#include <iostream>
#include "xml_interface.hpp"
#include "model_interface.hpp"
using namespace std;
using namespace tinyxml2;

XMLElement* xml_iterator(char* target, XMLElement* root){

	XMLElement* current;
	cout<<root->Name()<<endl;
	if(root->Name()==target)
		return root;

	else {
		if(root->FirstChildElement()!=0){

			current = xml_iterator(target,root->FirstChildElement());
			if(current!=0 && current->Name()==target){
				return current;
			}
		}	
		else{
			while(root->NextSiblingElement()!=0){				
				current = xml_iterator(target,root->NextSiblingElement());
				if(current!=0 && current->Name()==target)
					return current;
				}
	}
	return 0;

}

char** string_parser(const char* head,int size){

	char** return_string=nullptr;
	return_string=new char*[size];//bunu yapmadan da dene




	int index=0;
	int inner_index=0;

	while(index<size){

		if(*head!=' '){
			index++;
			return_string[index]=new char[10];

		}else{

			return_string[index][inner_index]=*head;
			inner_index++;
		}

		head++;
	}

	return return_string;

}


void generate_map(string model, string pose,Mat &image,double scale){

	XMLDocument xml_doc;
	
	XMLError eResult = xml_doc.LoadFile(caster("/usr/share/gazebo-9/models/"+model+"/model.sdf"));

	XMLElement* root=xml_doc.FirstChildElement("sdf");

	XMLElement* model_name = root->FirstChildElement("model");

	XMLElement* link = model_name->FirstChildElement("link");

	

	for(; link != 0; link=link->NextSiblingElement("link")){

		XMLElement* iterator=xml_iterator("collision",link);
		XMLElement* geo=xml_iterator("geometry",iterator);
		XMLElement* pose=xml_iterator("pose",link);

		const char* position_attitude=pose->GetText();


		char** pose_string = string_parser(position_attitude,6);



		if(geo->FirstChildElement()->Name()=="box"){

			XMLElement* size=xml_iterator("size",geo);
			const char* size_value=size->GetText();

			char** size_string=string_parser(size_value,3);

			Box new_box=Box(pose_string[0],pose_string[1],pose_string[2],
			pose_string[3],pose_string[4],pose_string[5],size_string[0],size_string[1]
			,size_string[2]);

			new_box.put_map(image,scale);

		}else if(geo->FirstChildElement()->Name()=="sphere"){

			XMLElement* radius=xml_iterator("radius",geo);
			const char* radius_value=radius->GetText();

			char** radius_string=string_parser(radius_value,1);

			Sphere new_sphere=Sphere(pose_string[0],pose_string[1],pose_string[2],
			pose_string[3],pose_string[4],pose_string[5],radius_string[0]);

			new_sphere.put_map(image,scale);

		}else if(geo->FirstChildElement()->Name()=="cylinder"){


			XMLElement* radius=xml_iterator("radius",geo);
			XMLElement* height=xml_iterator("height",geo);
			const char* radius_value=radius->GetText();
			const char* height_value=height->GetText();

			char** radius_string=string_parser(radius_value,1);
			char** height_string=string_parser(height_value,1);

			Cylinder new_cylinder=Cylinder(pose_string[0],pose_string[1],pose_string[2],
			pose_string[3],pose_string[4],pose_string[5],radius_string[0]
			,height_string[0]);

			new_cylinder.put_map(image,scale);
		}
		

	}	

}
