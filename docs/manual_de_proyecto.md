# Manual de Proyecto

## 1. Introducción

El presente manual documenta el trabajo realizado por el equipo durante el desarrollo del proyecto **Counter-Strike Grupo 8**. En él se detallan aspectos relacionados con la organización del equipo, las herramientas empleadas, los desafíos enfrentados y los aprendizajes adquiridos a lo largo del proceso de desarrollo.

---

## 2. Organización del Equipo

### **Roles y Responsabilidades**

- **[fernandYu](https://github.com/FernandoYu)**: Desarrollo del lobby y editor de niveles.
- **[tcorzo](https://github.com/tcorzo)**: Diseño del protocolo de comunicación, arquitectura general y lógica de conexión cliente-servidor.
- **[fnpratto](https://github.com/fnpratto)**: Implementación del cliente utilizando SDL.
- **[jesapugliese](https://github.com/jesapugliese)**: Desarrollo de la lógica del juego, física y funcionalidades del servidor.

### **Plan Semanal**

- **Semana 1**: Setup del repositorio, interiorizacion con herramientas, se agregaron rules,flags, test al repositorio.
- **Semana 2**: Desarrollo en paralelo del client y server. Se plantearon y establecio un manejo de threads correcto
- **Semana 3**: Protocolo establecido , se empezo a conectar el client con el server
- **Semana 4**: Se tiene una version conectada del client y server pero no se tienen las funcionalidades principales para jugar una roda(disparar, plantar bomba)
- **Semana 5**: Se tiene una version final jugable

### **Diferencias con el Plan Inicial**

- Se planificó tener un juego en la semana 2, pero se extendió hasta la semana 4 debido a problemas con la sincronización entre cliente y servidor.
- Se cambiaron las responsabilidades iniciales ya que se planteo que las fisicas se encargara otra persona y (t -> solo comunicacion y protocolo pero termino ayudando )
- Se planteo para la semana 3 ya tener conectado todo el client con el server pero dado ciertas dificultades y no tener bien establecido el protocolo llevo mas tiempo.

---

## 3. Herramientas Utilizadas

### **IDEs**

- Visual Studio Code

### **Linters**

- `clang-tidy` para análisis estático de código.
- `cppcheck` para detección de errores comunes.

### **GitHubProyect**

- Para la organizacion y gestion del proyecto se uso un template tipo kanban y se asignaban las responsabiolidades semanalmente atravez de issues

### **Documentación**

- Documentación oficial de SDL2.
- Tutoriales de CMake y Make.
- Recursos de YAML para configuración de mapas.

---

## 5. Problemas Enfrentados

### **Errores Conocidos**

- Logica en el cliente al crear el mensaje de plantar la bomba o defusing, aunque anden, se tiene que mantener la tecla debida sin mover el mouse( lo cual no era lo planeado) 
- Falta la funcionalidad de comprar ammo para arma principal
- Items de viejas rondas no se resetean
- Falta la funcionalidad del Pick up item

---

## 7. Créditos

Este proyecto fue desarrollado por el equipo **Counter-Strike Grupo 8** como parte del Taller de Programación.
