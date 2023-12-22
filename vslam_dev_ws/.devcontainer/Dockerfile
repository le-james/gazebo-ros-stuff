# Base image to build an image from
# Replace the ROS2 Humble image with your own or from from Docker Hub
FROM osrf/ros:humble-desktop-full

# install gazebo 11
RUN curl -sSL http://get.gazebosim.org | sh

# install ros2-gazebo packages from https://classic.gazebosim.org/tutorials?tut=ros2_installing&cat=connect_ros
# replace foxy with humble
# the second line is the demo examples
RUN apt-get install -y ros-humble-gazebo-ros-pkgs \
    && apt-get install -y ros-humble-ros-core ros-humble-geometry2 \
    && rm -rf /var/lib/apt/lists/*

# install xacro
RUN apt-get install -y ros-humble-xacro

# source ros2 - DOESN'T WORK
# RUN . /opt/ros/humble/setup.sh

# Add a new user
ARG USERNAME=user0
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Create a non-root user
RUN groupadd --gid $USER_GID $USERNAME \
  && useradd -s /bin/bash --uid $USER_UID --gid $USER_GID -m $USERNAME \
  && mkdir /home/$USERNAME/.config && chown $USER_UID:$USER_GID /home/$USERNAME/.config

# Keep bash command history persisting between container start up and stops (Every container will share this history)
RUN SNIPPET="export PROMPT_COMMAND='history -a' && export HISTFILE=/commandhistory/.bash_history" \
    && mkdir /commandhistory \
    && touch /commandhistory/.bash_history \
    && chown -R $USERNAME /commandhistory \
    && echo "$SNIPPET" >> "/home/$USERNAME/.bashrc"