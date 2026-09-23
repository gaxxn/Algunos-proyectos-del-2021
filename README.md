# Algunos-proyectos-del-2021
Algunos proyectos viejos del 2021 que hice en la escuela

# ◈ MOTOR DE FÍSICA 2D EN C++ (2021)

Proyecto escolar desarrollado originalmente en **2021** a petición de mi profesora para programar simulaciones físicas desde cero utilizando **C++** y **Visual Studio Code**. 

Recientemente el código fue portado a **Raylib 5.0** y compilado a **WebAssembly (WASM)** con **Emscripten** para ejecutarse de forma interactiva en la web.

---

## ✦ PROCEDIMIENTO Y MÓDULOS

La simulación calcula cuadro a cuadro (*step-based physics*) el movimiento dinámico mediante integración de vectores en 2D:

### 1. Kinematics (Cinemática)
* **Objetivo:** Simular el movimiento parabólico y la trayectoria de un proyectil.
* **Cálculo:** Aplicación de velocidad constante en el eje X y aceleración por gravedad en el eje Y.
```cpp
// Actualización de posición mediante vectores de velocidad y aceleración
position.x += velocity.x * dt;
position.y += velocity.y * dt;
velocity.y += gravity * dt;

https://github.com/user-attachments/assets/ba165b42-934f-4a23-83d2-d5e89126ea05

