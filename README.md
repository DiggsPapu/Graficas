## Proyecto 3
Se trató de implementar nuevos shaders, se implementó un shader para twistear, invertir colores, cambiar la temperatura, y realizar una fragmentacion, notese que para el twist shader es con tiempo, y para los demás se utiliza la dirección de manera que se pueda cambiar la dirección de la luz.

Twist shader:

![WhatsApp Image 2023-11-03 at 20 08 43_c39f0cc9](https://github.com/DiggsPapu/Graficas/assets/84475020/be46b524-0c12-44a5-a49c-21556ed436b9)

Inverse shader:

![image](https://github.com/DiggsPapu/Graficas/assets/84475020/991def66-8c74-40b8-860b-ceb8d1676682)

Temperature shader:

![image](https://github.com/DiggsPapu/Graficas/assets/84475020/f544641d-0098-40f4-bcfa-4043fcdca15a)

Fragmentation shader:

![image](https://github.com/DiggsPapu/Graficas/assets/84475020/d1122f4c-fc42-4530-bc13-a577cb451fc0)

![image](https://github.com/DiggsPapu/Graficas/assets/84475020/31a24a37-7e81-4399-9095-0fcadf920e24)

Video:

https://github.com/DiggsPapu/Graficas/assets/84475020/f7365aff-7c26-4fd6-8939-53841799dcd7

Controles: Al presionar alguna de las siguientes teclas así variará la tupla de (vertexShader, fragmentShader)

* L, cambia la dirección de la luz.
* 0, cambia el shader a original, fragment shader y vertex shader. Varía vertex y fragment shaders.
* 1, cambia a inverse shader que varía según la dirección de la luz. Varía fragment shader.
* 2, cambia al shader de temperatura, que varía según la dirección de la luz. Varía fragment shader.
* 3, cambia al shader de wave lines, que varía según el tiempo. Varía fragment shader.
* 4, cambia al shader de fragmentation, que varía según la dirección de la luz. Varía fragment shader.
* 6, cambia al shader de deformation, que varía según el tiempo. Varía vertex shader.
* 7, cambia al shader de twist, que varía según el tiempo. Varía vertex shader.

GPT reference:

https://chat.openai.com/share/5a163d32-c3a3-46aa-a45a-dc0351f7a423
