#include "../include/map_generator.hpp"
#include "tinyxml2.h"
#include <iostream>
#include "../include/xml_interface.hpp"
#include "../include/model_interface.hpp"
#include <fstream>



tinyxml2::XMLElement* xml_iterator(char* target, tinyxml2::XMLElement* root){

  tinyxml2::XMLElement* current;
  
  std::string target_full="";
  int i=0;
  while(1){

    if(target[i]=='\0')
      break;
    target_full=target_full+target[i];
    i++;

  }
  if(root->Name()==target_full){
    return root;  
  }else{
    if(root->FirstChildElement()!=0){

      current = xml_iterator(target,root->FirstChildElement());
      if(current!=0)
        return current;
    }
    if(root->NextSiblingElement()!=0){      
                    
      current = xml_iterator(target,root->NextSiblingElement());
      if(current!=0)
        return current;     
    }
    return 0;
  }
}
bool compare_string_with_ptrtochar(std::string a,const char* b){

  int index=0;
  while(a[index]!='\0' && b[index]!='\0'){
    if(a[index]!=b[index])
      return false;
    index++;
  }
  if(a[index]!=b[index])
    return false;

  return true;
}
char** string_parser(const char* head,int size){

  char** return_string=nullptr;
  return_string=new char*[size];
  return_string[0]=new char[50];

  int index=0;
  int inner_index=0;
  int head_index=0;

  while(index<size){

    if(head_index==0)
      while(head[head_index]==' ')
        head_index++;
    
    if(head[head_index]=='\0'){
      return_string[index][inner_index]='\0';
      break;
    }
    if(head[head_index]==' '){

      return_string[index][inner_index]='\0';
      index++;
      if(index<size)
        return_string[index]=new char[50];
      inner_index=0;

    }else{
      return_string[index][inner_index]=head[head_index];
      inner_index++;
    }

    head_index++;
  }

  return return_string;
}
void generate_map(std::string model, const char* pose,cv::Mat &image,double scale){

  tinyxml2::XMLDocument xml_doc;
  
  tinyxml2::XMLError eResult = xml_doc.LoadFile(caster("/usr/share/gazebo-9/models/"+model+"/model.sdf"));

  tinyxml2::XMLElement* root=xml_doc.FirstChildElement("sdf");

  tinyxml2::XMLElement* model_name = root->FirstChildElement("model");

  tinyxml2::XMLElement* link = model_name->FirstChildElement("link");

  for(; link != 0; link=link->NextSiblingElement("link")){

    tinyxml2::XMLElement* iterator=xml_iterator("collision",link);
    tinyxml2::XMLElement* geo=xml_iterator("geometry",iterator);

    char** pose_string = string_parser(pose,6);
  
    if(compare_string_with_ptrtochar("box",geo->FirstChildElement()->Name())) {//box
      
      tinyxml2::XMLElement* size=xml_iterator("size",geo);
      const char* size_value=size->GetText();

      char** size_string=string_parser(size_value,3);

      Box new_box=Box(pose_string[0],pose_string[1],pose_string[2],
      pose_string[3],pose_string[4],pose_string[5],size_string[0],size_string[1]
      ,size_string[2]);

      new_box.put_map(image,scale,false);

    }else if(compare_string_with_ptrtochar("sphere",geo->FirstChildElement()->Name())){//sphere

      tinyxml2::XMLElement* radius=xml_iterator("radius",geo);
      const char* radius_value=radius->GetText();

      char** radius_string=string_parser(radius_value,1);

      Sphere new_sphere=Sphere(pose_string[0],pose_string[1],pose_string[2],
      pose_string[3],pose_string[4],pose_string[5],radius_string[0]);

      new_sphere.put_map(image,scale,0);

    }else if(compare_string_with_ptrtochar("cylinder",geo->FirstChildElement()->Name())){//cylinder


      tinyxml2::XMLElement* radius=xml_iterator("radius",geo);
      tinyxml2::XMLElement* height=xml_iterator("length",geo);
      const char* radius_value=radius->GetText();
      const char* height_value=height->GetText();

      char** radius_string=string_parser(radius_value,1);
      char** height_string=string_parser(height_value,1);

      Cylinder new_cylinder=Cylinder(pose_string[0],pose_string[1],pose_string[2],
      pose_string[3],pose_string[4],pose_string[5],radius_string[0]
      ,height_string[0]);

      new_cylinder.put_map(image,scale,0);
    }else if(compare_string_with_ptrtochar("mesh",geo->FirstChildElement()->Name())){
      
      tinyxml2::XMLElement* uri=xml_iterator("uri",geo->FirstChildElement());
      
      std::string filepath="/usr/share/gazebo-9/models/";      
      
      const char* remain=uri->GetText();

      int index=0;
      bool first_part=true;
      while(remain[index]!='\0'){

        if(!first_part)
          filepath=filepath+remain[index];
        if(index>0)
          if (remain[index]=='/' && remain[index-1]=='/')
            first_part=false;
        index++;
      }/*
      char* whole_line=new char[200];
      ifstream* infile;
      infile.open(caster(filepath));
      Point three_points[3];
      bool start_flag=true;
      int count_of_vertexes=0;*/

      Mesh mesh_model=Mesh(pose_string[0],pose_string[1],pose_string[2],
      pose_string[3],pose_string[4],pose_string[5]);

      mesh_model.put_map(image,scale,filepath,0.21);
/*
      while(!infile.eof()){


        infile.getline(whole_line,100);
        char** line_disected=string_parser(whole_line,10);

        if(compare_string_with_ptrtochar("vertex",line_disected[0])){

          double x=(stod(line_disected[1])+stod(pose_string[0]))/scale;
          double y=(stod(line_disected[2])+stod(pose_string[1]))/scale;

          x=x+image.rows/2.0;
          y=y+image.cols/2.0;

          three_points[count_of_vertexes].x=x;
          three_points[count_of_vertexes].y=y;
          count_of_vertexes++;

          if(count_of_vertexes==3){
            
            for(int q=0;q<3;q++){
              line(image,three_points[q],three_points[(q+1)%3],(255,255,255),1);
            }
            count_of_vertexes=0;            
          }

        }

      }*/

    }
    

  } 

}
