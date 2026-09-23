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
```
https://github.com/user-attachments/assets/c955fb47-6549-4b4a-a26c-095de7695ba1


