<h1>Entregas</h1>

<a href="https://youtu.be/RnSIzN6QPj8" target="_blank">Entrega 1</a>
<br>
<a href="https://youtu.be/Cht93-X8mbc" target="_blank">Entrega 2</a>
<br>
<a href="https://www.youtube.com/watch?v=d2iWqc8enDo" target="_blank">Entrega 3</a>

<h1>Como ejecutar el equipo: </h1>

Sigue los siguientes pasos para compilar y ejecutar el proyecto: <br>
Descargar el proyecto (o clonar el repositorio) y entrar en la carpeta del proyecto (Soccer/) <br>

```bash
mkdir build
cd build
cmake ..
make
cd ..
./launchplayers.sh <nombre-del-equipo> <puerto-inicial>
```
Si necesitas conceder permisos de ejecucion, introduce:
```bash
chmod +x ./launchplayers.sh
