"""
This is my custom build tool.
You must define de include directories, the lib directores and
also, you can change the compiler, the flags, the C standard version and the main file name.
"""
import os

compiler = "clang"
c_version = "-std=c99"
out_name = "game.exe"

files = [
	"main.c",
	"glad.c "
]

includes_dir = [
	"",
	"",
]

libs_dir = [
	"",
]

libs = [
	"glfw3dll",
]

flags = [
	"-m32",
	"-Ofast"
]

buff_out = ""

buff_out += compiler + " " + c_version + " "

for f in files:
	print("Loading: " + f)
	buff_out += f + " "

for i in includes_dir:
	print("Loading: " + i)
	buff_out += "-I\"" + i + "\" "

for l in libs_dir:
	print("Loading: " + l)
	buff_out += "-L\"" + l + "\" "

for l in libs:
	print("Loading: " + l)
	buff_out +=  "-l" + l + " "

for f in flags:
	print("Using: " + f)
	buff_out += f + " "

buff_out += "-o " + out_name
print(buff_out)

os.system(buff_out)
os.system(out_name)

