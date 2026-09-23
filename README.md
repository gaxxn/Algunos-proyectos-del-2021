# ◈ Motor de Física 2D en C++

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus)
![Raylib](https://img.shields.io/badge/Raylib-5.0-555555?style=flat-square)
![WebAssembly](https://img.shields.io/badge/WebAssembly-Emscripten-654FF0?style=flat-square&logo=webassembly)
![GitHub Pages](https://img.shields.io/badge/Deploy-GitHub%20Pages-222222?style=flat-square&logo=github)

Simulador de físicas interactivo en dos dimensiones desarrollado originalmente en **2021** como proyecto escolar. El motor fue programado modularmente en **C++** sobre **Visual Studio Code**, y posteriormente portado a **Raylib 5.0** con compilación a **WebAssembly (WASM)** mediante **Emscripten** para ejecución nativa en navegadores web y móviles.

---

## ✦ MÓDULOS DE SIMULACIÓN Y CÓDIGO

La simulación calcula cuadro a cuadro (*step-based physics*) la integración vectorial del movimiento dinámico en espacio 2D.

### 1. Cinemática (Kinematics)
Cálculo de movimiento parabólico y actualización de vectores de velocidad y posición en tiempo real:

```cpp
// Actualización de posición mediante vectores de velocidad y aceleración
position.x += velocity.x * dt;
position.y += velocity.y * dt;
velocity.y += gravity * dt;
// 1. Aceleración por gravedad y actualización de posición
velocity.y += gravity * dt;
position.x += velocity.x * dt;
position.y += velocity.y * dt;

// 2. Colisión con el suelo e inversión de velocidad (Rebote)
if (position.y + radius >= screenHeight) {
    position.y = screenHeight - radius;
    velocity.y *= -restitution; // P. ej. -0.8f para perder energía
}

```
https://github.com/user-attachments/assets/c955fb47-6549-4b4a-a26c-095de7695ba1

---

### 2. Ley de Hooke (Sistemas Elásticos)
Simulación de la fuerza de restitución proporcional a la deformación del resorte con amortiguamiento (*damping*):

```cpp
// Fuerza elástica de restitución: F = -k * x - c * v
Vector2 displacement = Vector2Subtract(ballPosition, anchorPoint);
Vector2 springForce = Vector2Scale(displacement, -k);
Vector2 dampingForce = Vector2Scale(velocity, -c);

acceleration = Vector2Scale(Vector2Add(springForce, dampingForce), 1.0f / mass);

```
https://github.com/user-attachments/assets/0ccb60e1-73ec-4d6a-9848-bf90b6e29600

---

### 3. Atracción Gravitacional (Gravitational Attraction)
Simulación de fuerza central tipo imán o punto masivo que atrae partículas dinámicas en el espacio 2D mediante aceleración dirigida:


```cpp
// Cálculo del vector de atracción hacia el punto central
Vector2 direction = Vector2Subtract(attractorPos, particlePos);
float distance = Vector2Length(direction);

if (distance > 5.0f) {
    Vector2 normalizeDir = Vector2Scale(direction, 1.0f / distance);
    float force = (G * mass) / (distance * distance); // Fuerza de atracción
    acceleration = Vector2Scale(normalizeDir, force);
}

```
https://github.com/user-attachments/assets/3ca7cc38-3728-48b0-8346-eb846bdf1e35

// Detección e impacto entre dos partículas circulares (Radio R1 + R2)
Vector2 delta = Vector2Subtract(p2.position, p1.position);
float distance = Vector2Length(delta);

if (distance < (p1.radius + p2.radius)) {
    // Intercambio de momentos e inversión de velocidad en eje normal
    Vector2 normal = Vector2Scale(delta, 1.0f / distance);
    Vector2 relativeVelocity = Vector2Subtract(p1.velocity, p2.velocity);
    float impulse = Vector2DotProduct(relativeVelocity, normal);
    
    p1.velocity = Vector2Subtract(p1.velocity, Vector2Scale(normal, impulse));
    p2.velocity = Vector2Add(p2.velocity, Vector2Scale(normal, impulse));

https://github.com/user-attachments/assets/1a152a09-73da-4cbf-84f0-56d9f5fc0e6d

---

> *"Estaré subiendo diariamente diferentes proyectos antiguos que he desarrollado a lo largo del tiempo, hasta enseñar mi progreso actual."*
> 
