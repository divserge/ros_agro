xhost +local:root
sudo docker run -it -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=unix$DISPLAY \
--device=/dev/snd/controlC0 --device=/dev/snd/pcmC0D0p \
--device=/dev/snd/seq --device=/dev/snd/timer \
--device=/dev/video0 \
divserge/ros_agro:1.0