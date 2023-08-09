from gl import Renderer
import Shaders

width = 960
height = 540

# Se crea el renderizador
rend = Renderer(width, height)

# Le damos los shaders que se utilizar�s
rend.vertexShader = Shaders.vertexShader
rend.fragmentShader = Shaders.fragmentShader

# Cargamos los modelos que rederizaremos
rend.glLoadModel(filename = "models/model.obj",
                 textureName = "textures/model.bmp",
                 translate = (width/4, height/2, 0),
                 rotate = (0, 90, 0),
                 scale = (150,150,150))


rend.glLoadModel(filename = "models/model.obj",
                 textureName = "textures/model.bmp",
                 translate = (width * 3/4, height/2, 0),
                 rotate = (0, 180, 0),
                 scale = (150,150,150))

# Se renderiza la escena
rend.glRender()

# Se crea el FrameBuffer con la escena renderizada
rend.glFinish("output.bmp")
