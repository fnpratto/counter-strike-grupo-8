# Documentación técnica

Todas las funcionalidades y mejoras se rastrean mediante issues y pull requests en GitHub.

## Aspectos más relevantes del desarrollo

### 1. Client

- Shop 
- Implementación del campo de visión
- Implementación de animaciones del disparo y plantar bomba
- Visualización del contador de tiempo bomba y objetos en el suelo
- Integración de sonidos (gestor y eventos)
- Visualización de fin de ronda ( mostrar los scores de equipos, mensaje de quipo ganador de ronda)

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

### 3. Editor de niveles

- Interfaz gráfica implementada con Qt.
- Permite crear y editar mapas con estructuras sólidas, terreno, y zonas para plantar bombas.
- Scroll horizontal y vertical para explorar mapas más grandes que el tamaño de ventana.
- Guardado y apertura de mapas en formato YAML.
- Implementación de drag and drop y point and click para facilitar la edición del mapa.

## Funcionalidades incompletas

- Que en el YAML de configuración se pueda definir el tamaño de pantalla; actualmente está hardcodeado.
- Los sonidos no varían según la distancia del evento.
- Falta la animación de explosión al detonar la bomba.
- El editor de niveles no permite colocar armas en el mapa (solo estructuras y zonas de inicio).

---