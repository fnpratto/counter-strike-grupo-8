# Documentación técnica

Todas las funcionalidades y mejoras se rastrean mediante issues y pull requests en GitHub. A continuación, se documentan los aspectos más relevantes del desarrollo.

## Funcionalidades completas

- Lobby (crear / unirse a partidas)
- Implementación de arquitectura cliente-servidor básica
- HUD funcional
- Definición del protocolo de mensajes
- Implementación de cámara
- Switch display entre QT y SDL
- Pre game (selección de equipos y skin)
- Warm up (espera a que todos los usuarios hayan terminado sus selecciones)

## Funcionalidades en progreso o planificadas

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

- Tenemos una constante global meter_size. Estamos en duda de si centralizar el manejo de esta constante en el Physics System, pero, a su vez, queremos guardar atributos como las posiciones de los tiles en el mapa (clase Map) o el máximo rango de las armas (max_range) de acuerdo a la métrica del meter_size. Es por eso que en principio lo pusimos como una constante global.
- Arreglar los parámetros de velocidad del jugador y las configuraciones de las armas para que el juego esté mejor balanceado.

## Bugs

- Movimiento izquierda y/o arriba
- Animación
