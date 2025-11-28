#!/bin/bash

cd player/build && make
sleep 3;

for i in {0..10}
do
  gnome-terminal -- bash -c "~/Projects/SoccerCup/player/build/player Oesia 505$i" &
  sleep 1
done
