# milvus
gazebo world generator
before executing the program, make sure that you have a binary image to generate the world from in the same directory as the output file and also the new model file with the given config file to save the wall model created.
run the output file.
give a scale between the image and the model world as asked.(a scale of 2 means each pixel is 2*2 units (can be tought as 2 by 2 meters in simulation world) gazebo squares)
then give the names and poses of the models that you want to add as ordered but make sure that the models that you add are inside the gazebo models folder. enter q to quit entering new model names.
now both your world and wall model is ready (model as new_model world file as generated_world.world)
add the new_model folder to the gazebox/models directory and the generated_world.world file to gazebox/worlds directory
now you can open the generated world by opening gazebo with the command gazebo worlds/generated_world.world
