from gl import Renderer
import Shaders

width = 3840 
height = 2160
rend = Renderer(width, height)
rend.vertexShader = Shaders.vertexShader
rend.fragmentShader = Shaders.fragmentShader
rend.glLoadModel(filename = "models/model.obj",textureName = "textures/dog.bmp",translate = (width/4, height/2, 0),rotate = (0, 90, 0),scale = (150,150,150))
rend.glLoadModel(filename = "models/Dog.obj",textureName = "textures/Brick2.bmp",translate = (width * 3/4, height/2, 0),rotate = (0, 180, 0),scale = (10,10,10))
rend.glRender()
rend.glFinish("image.bmp")
