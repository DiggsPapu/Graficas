input_filename = "/home/dieggspapu/UVG/Graficas/Obj/Dinosaur OBJ/Triceratop.obj"
output_filename = "/home/dieggspapu/UVG/Graficas/Obj/Dinosaur OBJ/Triceratop2.obj"

with open(input_filename, 'r') as input_file, open(output_filename, 'w') as output_file:
    for line in input_file:
        if line.startswith("f "):
            vertices = line.strip().split()[1:]
            formatted_line = "f " + "/".join(vertices) + "\n"
            output_file.write(formatted_line)
        else:
            output_file.write(line)
