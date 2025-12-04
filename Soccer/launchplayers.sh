#!/bin/bash

TEAM_NAME="$1"
PORT_START="$2"

# Validación de parámetros
if [ -z "$TEAM_NAME" ] || [ -z "$PORT_START" ]; then
  echo "Uso: $0 <nombre_equipo> <puerto_inicial>"
  exit 1
fi

cd build/ && make
sleep 3

for i in {0..10}
do
  PORT=$((PORT_START + i))
  gnome-terminal -- bash -c "./player $TEAM_NAME $PORT" &
  sleep 1
done
