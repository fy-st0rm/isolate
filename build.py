import json
import sys
import os
import platform

class Builder:
	def __init__(self, config):
		if platform.system().lower() == "windows":
			self.slash = "{self.slash}"
		elif platform.system().lower() == "linux":
			self.slash = "/"

		self.isolate_lib = {
			"windows": ["mingw32", "SDL2main", "SDL2", "SDL2_image", "m",
						"glu32", "opengl32", "User32", "Gdi32", "Shell32", "glew32"],
			"linux"  : ["SDL2main", "SDL2", "SDL2_image", 
					    "m", "GL", "GLU", "GLEW"]
		}

		self.isolate_includes = {
			"windows": [f"includes{self.slash}", f"vendor{self.slash}GLEW{self.slash}include{self.slash}", f"vendor{self.slash}SDL2_64bit{self.slash}include{self.slash}"],
			"linux"  : [f"includes{self.slash}", f"{self.slash}usr{self.slash}include{self.slash}"]
		}

		self.isolate_lib_path = {
			"windows": [f"vendor{self.slash}GLEW{self.slash}lib{self.slash}", f"vendor{self.slash}SDL2_64bit{self.slash}lib{self.slash}"],
			"linux"  : [f"{self.slash}usr{self.slash}lib{self.slash}"]
		}

		self.vendor_dlls = [f"vendor{self.slash}GLEW{self.slash}bin{self.slash}", f"vendor{self.slash}SDL2_64bit{self.slash}bin{self.slash}"]

		self.load_config(config)

	def load_config(self, config):
		self.project_dir = os.path.dirname(config) + self.slash
		self.config = json.load(open(config, "r"));

		# Loading the data
		self.platform = self.config["platform"]

		if self.platform == "auto":
			self.platform = platform.system().lower()

		self.isolate_path = self.config["isolate_path"]
		self.out_dir = self.project_dir + "bin"
		self.out = self.out_dir + self.slash + self.config["out"]
		self.cc  = self.config["cc"]

		# Compiler flags
		self.c_flags = " ".join(self.config["c_flags"])
		self.c_files = " ".join(map((self.project_dir + "{0}").format, self.config["c_files"]))
		print(self.c_files)

		# Includes and libraries
		self.includes = (
			" ".join(map(("-I" + self.isolate_path + "{0}").format, self.isolate_includes[self.platform]))
		  + " ".join(map("-I{0}".format, self.config["includes"]))
		)

		self.lib_path = (
			" ".join(map(("-L" + self.isolate_path + "{0}").format, self.isolate_lib_path[self.platform]))
		  + " ".join(map("-L{0}".format, self.config["lib_path"]))
		)

		self.lib = (
			" ".join(map("-l{0}".format, self.isolate_lib[self.platform]))
		  + " ".join(map("-l{0}".format, self.config["lib"]))
		)

	def exec(self, cmd):
		print("[CMD]:", cmd)
		os.system(cmd)

	def __compile_windows(self):
		# Compiling
		cmp_cmd = f"{self.cc} {self.c_flags} {self.includes} -c {self.c_files}"
		self.exec(cmp_cmd)

		# Building
		if not os.path.exists(self.out_dir):
			os.mkdir(self.out_dir)

		build_cmd = f"{self.cc} -o {self.out} *.o {self.lib_path} {self.lib}"
		self.exec(build_cmd)

		# Copying dlls
		for dll in self.vendor_dlls:
			self.exec(f"copy {self.isolate_path}{dll}*.dll {self.out_dir}")

		# Cleaning
		self.exec("erase *.o")

	def __compile_linux(self):
		# Compiling
		cmp_cmd = f"{self.cc} {self.c_flags} {self.includes} -c {self.c_files}"
		self.exec(cmp_cmd)

		# Building
		if not os.path.exists(self.out_dir):
			os.mkdir(self.out_dir)

		build_cmd = f"{self.cc} -o {self.out} *.o {self.lib_path} {self.lib}"
		self.exec(build_cmd)

		# Cleaning
		self.exec("rm *.o")

	def run(self):
		if self.platform == "windows":
			self.__compile_windows()
		elif self.platform == "linux":
			self.__compile_linux()
		
if __name__ == "__main__":
	if len(sys.argv) == 1:
		print("[Usage]: build [config_file]")

	builder = Builder(sys.argv[1])
	builder.run()
