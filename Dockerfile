FROM ros:kinetic-ros-core

MAINTAINER Sergey Divakov <sergei.divakov@skoltech.ru>

RUN apt-get update && \
    apt-get install -y --no-install-recommends git wget unzip cmake

RUN apt-get install -y ros-kinetic-usb-cam
RUN apt-get install -y ros-kinetic-image-view
RUN apt-get install -y nano
RUN apt-get install -y ros-kinetic-camera-calibration
RUN apt-get install -y ros-kinetic-ar-track-alvar