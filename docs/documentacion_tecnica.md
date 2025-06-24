# Documentación técnica

Todas las funcionalidades y mejoras se rastrean mediante issues y pull requests en GitHub.

## Aspectos más relevantes del desarrollo

### 1. Client

- Shop ( el cliente le llega la información de la tienda pero no la usa esta hardcodeado y no envia los mensajes correspondientes para la compra)
- Implementación del campo de visión
- Implementación de animaciones del disparo y plantar bomba
- Visualización del contador de tiempo bomba y objetos en el suelo
- Integración de sonidos (gestor y eventos)
- Visualización de fin de ronda ( mostrar los scores de equipos, mensaje de quipo ganador de ronda)
- Hay problemas de delay al mandar muchos comandos a la vez

### 2. Server

- Mandar el mapa a través de un mensaje al client.
- Crear un macro para los states.
- Obtener la configuración inicial/default del juego en GameConfig desde un YAML.
- Scoreboard de jugadores al finalizar las rondas
- Lógica de droppear armas.
- Comando para pickear armas del suelo.
- Detección de colisiones al moverse -> colisiones con tiles e items droppeados en el suelo.
- Plantar bomba + defuse por los CTs y explosión
- Reinicio de la vida de los jugadores al empezar una nueva ronda.
- Agregado de dinero a los jugadores al empezar una nueva ronda
- Bonificaciones de dinero de acuerdo a estadísticas de los jugadores
- Finalización de la partida tras 10 rondas (victoria / derrota).
- Respuestas de error desde el server (error responses).

### Editor de niveles

## Posibles modificaciones

### 1. Server

-

## Funcionalidades incompletas

- El client yaml de config tenga si 1200x 800 o full screen
- Sonidos no varian segun la distancia
- El client animacion de explocion
- El armado del mapa del Editor de niveles no abarca el colocado de armas
