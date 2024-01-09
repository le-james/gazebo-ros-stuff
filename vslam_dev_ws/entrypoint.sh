# tells the terminal to run the script using bash, could use shells
#!/bin/bash

# bash flag - aborts if error occurs - https://www.google.com/search?q=linux+%22set+-e%22&sca_esv=593164028&ei=XROGZaKKO_WiptQPupui2Ac&oq=bash+set+-e&gs_lp=Egxnd3Mtd2l6LXNlcnAiC2Jhc2ggc2V0IC1lKgIIADIKEAAYRxjWBBiwAzIKEAAYRxjWBBiwAzIKEAAYRxjWBBiwAzIKEAAYRxjWBBiwAzIKEAAYRxjWBBiwAzIKEAAYRxjWBBiwAzIKEAAYRxjWBBiwAzIKEAAYRxjWBBiwA0jGClAAWABwAXgBkAEAmAEAoAEAqgEAuAEByAEA4gMEGAAgQYgGAZAGCA&sclient=gws-wiz-serp
set -e


echo "Running Entrypoint Script..."

echo "Sourced ROS2 into bashrc file"
# source ros2
# source /opt/ros/humble/setup.bash
# . /opt/ros/humble/setup.sh
echo "source /opt/ros/humble/setup.sh" >> ~/.bashrc

# source ros2 environment - https://answers.ros.org/question/362922/ros2-what-is-installsetupbash-doing/
# this gives and error for some reason when running entrypoing.sh
# . /install/setup.bash

# append another model and plugin path
# doesn't add variables to the environment for some reason
# the devcontainer.json can add the variables
# export GAZEBO_MODEL_PATH=~/.gazebo/models:$GAZEBO_MODEL_PATH
# export GAZEBO_PLUGIN_PATH=/workspaces/gazebo-ros-stuff/my_gazebo_plugins/plugins/:$GAZEBO_PLUGIN_PATHin