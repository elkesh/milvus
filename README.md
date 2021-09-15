# milvus
gazebo world generator<br />
before executing the program, make sure that you have a binary image to generate the world from in the same directory as the output file named deneme.jpg (you can use the one given in the repo) and also have a new_model file with the given config file to save the wall model created.<br />
run the output file.<br />
give a scale between the image and the model world as asked.(a scale of 2 means each pixel is 2*2 units (can be tought as 2 by 2 meters in simulation world) gazebo squares)<br />
then give the names and poses of the models that you want to add as ordered but make sure that the models that you add are inside the gazebo models folder. enter q to quit entering new model names.<br />
now both your world and wall model is ready (model as new_model, world file as generated_world.world)<br />
add the new_model folder to the gazebo/models directory and the generated_world.world file to gazebox/worlds directory<br />
now you can open the generated world by opening gazebo with the command gazebo worlds/generated_world.world<br />
the map of the newly generated world can be found as resultant.jpg in the same directory as the ouput file
