# Manual de Proyecto

## 1. Introducción

Este manual documenta el trabajo realizado por el equipo durante el desarrollo del proyecto **Counter-Strike Grupo 8**. Incluye detalles sobre la organización, herramientas utilizadas, problemas enfrentados y aprendizajes obtenidos.

---

## 2. Organización del Equipo

### **Roles y Responsabilidades**

- **Integrante 1**: Desarrollo del cliente (interfaz gráfica, manejo de eventos).
- **Integrante 2**: Implementación del servidor (lógica del juego, sincronización).
- **Integrante 3**: Diseño del mapa y configuración de YAML.
- **Integrante 4**: Integración de SDL2 y manejo de gráficos.
- **Integrante 5**: Pruebas y documentación.

### **Plan Semanal**

- **Semana 1**: Investigación de tecnologías y diseño inicial del proyecto.
- **Semana 2**: Configuración del entorno de desarrollo y creación de prototipos.
- **Semana 3**: Implementación de las funcionalidades principales (cliente y servidor).
- **Semana 4**: Integración de componentes y pruebas.
- **Semana 5**: Resolución de errores y documentación.

### **Diferencias con el Plan Inicial**

- Se planificó terminar la implementación en la semana 3, pero se extendió hasta la semana 4 debido a problemas con la sincronización entre cliente y servidor.
- La documentación se realizó en paralelo con las pruebas, lo que permitió optimizar el tiempo.

---

## 3. Herramientas Utilizadas

### **IDEs**

- Visual Studio Code
- CLion

### **Linters**

- `clang-tidy` para análisis estático de código.
- `cppcheck` para detección de errores comunes.

### **Documentación**

- Documentación oficial de SDL2.
- Tutoriales de CMake y Make.
- Recursos de YAML para configuración de mapas.

---

## 4. Sugerencias para el Taller

- Incluir una introducción más detallada sobre el uso de SDL2 y su integración con C++.
- Explicar cómo manejar la sincronización entre cliente y servidor en proyectos multijugador.
- Proveer ejemplos prácticos de configuración de CMake para proyectos grandes.

---

## 5. Problemas Enfrentados

### **Puntos Problemáticos**

- **Sincronización Cliente-Servidor**: Fue difícil garantizar que los estados del juego se mantuvieran consistentes.
- **Integración de SDL2**: Algunos errores relacionados con la inicialización de gráficos y eventos.
- **Configuración de Mapas**: La estructura YAML tuvo problemas de validación y formato.

### **Errores Conocidos**

- En ciertas ocasiones, el cliente puede perder conexión con el servidor si hay alta latencia.
- Algunos elementos gráficos no se renderizan correctamente en resoluciones bajas.

---

## 6. Reflexión y Aprendizajes

### **¿Qué cambiaríamos?**

- **A nivel código**: Modularizar más el código para facilitar la prueba y el mantenimiento.
- **A nivel organizacional**: Dedicar más tiempo a la planificación inicial y pruebas unitarias.

### **¿Qué aprendimos?**

- La importancia de la comunicación constante entre los integrantes del equipo.
- Cómo manejar proyectos grandes utilizando herramientas como CMake y SDL2.
- La necesidad de realizar pruebas frecuentes para detectar errores temprano.

---

## 7. Créditos

Este proyecto fue desarrollado por el equipo **Counter-Strike Grupo 8** como parte del Taller de Programación.
