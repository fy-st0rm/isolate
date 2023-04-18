import json
import platform
import os
from common import *

"""

Config file example:

	build_mode: "debug/release"
	isolate_path: "relative/path/to/isolate"

	out:{
			"windows": "out.exe",
			"linux": "out"
		}
	cc: "gcc"

	c_files: []

	c_flags:{
				"windows": [],
				"linux": []
			}

	include_path:{
					"windows": [],
					"linux": []
				 }
	lib_path:{
				"windows": [],
				"linux": []
			 }

	libs: {
			"windows": [],
			"linux": []
		  }

	dll_path: {
				"windows": [],
				"linux": []
			}

"""



class IpmConfig:
	def __init__(self):
		self.platform = platform.system().lower()

		# Defaults
		self.__build_modes = {
			"debug"  : "-DISO_BUILD_DEBUG",
			"release": "-DISO_BUILD_RELEASE"
		}

		self.__out_dir_name = {
			PLAT_WINDOWS: "win",
			PLAT_LINUX  : "linux"
		}

	def load(self, path):
		self.project_dir = SLASH[self.platform].join(os.path.abspath(path).split(SLASH[self.platform])[:-1]) + SLASH[self.platform]
		self.config      = json.load(open(path, "r"));

		# Loading basic settings
		self.build_mode   = self.config["build_mode"]
		self.isolate_path = self.config["isolate_path"][self.platform] + SLASH[self.platform]
		self.out_dir      = self.project_dir + "bin" + SLASH[self.platform] + self.__out_dir_name[self.platform]

		self.out = self.config["out"]
		self.cc  = self.config["cc"]

		# Compiler flags
		self.c_flags = self.config["c_flags"]
		self.c_files = list(map((self.project_dir + "{0}").format, self.config["c_files"]))
		self.o_files = list(map((self.project_dir + "{0}.o").format, self.config["c_files"]))

		# Loading build type
		self.c_flags[PLAT_WINDOWS].append(self.__build_modes[self.build_mode])
		self.c_flags[PLAT_LINUX  ].append(self.__build_modes[self.build_mode])

		# Includes and libraries
		self.include_path = self.config["include_path"]
		self.lib_path     = self.config["lib_path"]
		self.libs         = self.config["libs"]
		self.dll_path     = self.config["dll_path"]

	def generate_default(self, project_name, project_path, isolate_path):
		isolate_path_win = isolate_path_linux = isolate_path

		if SLASH[PLAT_LINUX] in isolate_path:
			isolate_path_win = isolate_path.replace(SLASH[PLAT_LINUX], SLASH[PLAT_WINDOWS])
		if SLASH[PLAT_WINDOWS] in isolate_path:
			isolate_path_linux = isolate_path.replace(SLASH[PLAT_WINDOWS], SLASH[PLAT_LINUX])

		__isolate_libs = {
			PLAT_WINDOWS: [
				"mingw32", "SDL2main", "SDL2",
				"SDL2_image", "m", "glu32",
				"opengl32", "User32", "Gdi32",
				"Shell32", "glew32", "isolate"
			],

			PLAT_LINUX    : [
				"SDL2main", "SDL2", "SDL2_image", 
				"m", "GL", "GLU", "GLEW", "isolate"
			]
		}

		__isolate_includes = {
			PLAT_WINDOWS: [
				f"{isolate_path_win}{SLASH[PLAT_WINDOWS]}src{SLASH[PLAT_WINDOWS]}",
				f"{isolate_path_win}{SLASH[PLAT_WINDOWS]}vendor{SLASH[PLAT_WINDOWS]}GLEW{SLASH[PLAT_WINDOWS]}include{SLASH[PLAT_WINDOWS]}",
				f"{isolate_path_win}{SLASH[PLAT_WINDOWS]}vendor{SLASH[PLAT_WINDOWS]}SDL2_64bit{SLASH[PLAT_WINDOWS]}include{SLASH[PLAT_WINDOWS]}"
			],
			PLAT_LINUX    : [
				f"{isolate_path_linux}{SLASH[PLAT_LINUX]}src{SLASH[PLAT_LINUX]}",
				f"{isolate_path_linux}{SLASH[PLAT_LINUX]}vendor{SLASH[PLAT_LINUX]}GLEW{SLASH[PLAT_LINUX]}include{SLASH[PLAT_LINUX]}",
				f"{isolate_path_linux}{SLASH[PLAT_LINUX]}vendor{SLASH[PLAT_LINUX]}SDL2_64bit{SLASH[PLAT_LINUX]}include{SLASH[PLAT_LINUX]}"
			]
		}

		__isolate_lib_path = {
			PLAT_WINDOWS: [
				f"{isolate_path_win}{SLASH[PLAT_WINDOWS]}vendor{SLASH[PLAT_WINDOWS]}GLEW{SLASH[PLAT_WINDOWS]}lib{SLASH[PLAT_WINDOWS]}win{SLASH[PLAT_WINDOWS]}",
				f"{isolate_path_win}{SLASH[PLAT_WINDOWS]}vendor{SLASH[PLAT_WINDOWS]}SDL2_64bit{SLASH[PLAT_WINDOWS]}lib{SLASH[PLAT_WINDOWS]}win{SLASH[PLAT_WINDOWS]}",
				f"{isolate_path_win}{SLASH[PLAT_WINDOWS]}bin{SLASH[PLAT_WINDOWS]}win{SLASH[PLAT_WINDOWS]}"
			],
			PLAT_LINUX    : [
				f"{isolate_path_linux}{SLASH[PLAT_LINUX]}vendor{SLASH[PLAT_LINUX]}GLEW{SLASH[PLAT_LINUX]}lib{SLASH[PLAT_LINUX]}linux{SLASH[PLAT_LINUX]}",
				f"{isolate_path_linux}{SLASH[PLAT_LINUX]}vendor{SLASH[PLAT_LINUX]}SDL2_64bit{SLASH[PLAT_LINUX]}lib{SLASH[PLAT_LINUX]}linux{SLASH[PLAT_LINUX]}",
				f"{isolate_path_linux}{SLASH[PLAT_LINUX]}bin{SLASH[PLAT_LINUX]}linux{SLASH[PLAT_LINUX]}"
			]
		}

		__isolate_vendor_dlls = {
			PLAT_WINDOWS: [
				f"{isolate_path_win}{SLASH[PLAT_WINDOWS]}vendor{SLASH[PLAT_WINDOWS]}GLEW{SLASH[PLAT_WINDOWS]}bin{SLASH[PLAT_WINDOWS]}win{SLASH[PLAT_WINDOWS]}",
				f"{isolate_path_win}{SLASH[PLAT_WINDOWS]}vendor{SLASH[PLAT_WINDOWS]}SDL2_64bit{SLASH[PLAT_WINDOWS]}bin{SLASH[PLAT_WINDOWS]}win{SLASH[PLAT_WINDOWS]}",
				f"{isolate_path_win}{SLASH[PLAT_WINDOWS]}bin{SLASH[PLAT_WINDOWS]}win{SLASH[PLAT_WINDOWS]}"
			],
			PLAT_LINUX  : [
				f"{isolate_path_linux}{SLASH[PLAT_LINUX]}vendor{SLASH[PLAT_LINUX]}GLEW{SLASH[PLAT_LINUX]}bin{SLASH[PLAT_LINUX]}linux{SLASH[PLAT_LINUX]}",
				f"{isolate_path_linux}{SLASH[PLAT_LINUX]}vendor{SLASH[PLAT_LINUX]}SDL2_64bit{SLASH[PLAT_LINUX]}bin{SLASH[PLAT_LINUX]}linux{SLASH[PLAT_LINUX]}",
				f"{isolate_path_linux}{SLASH[PLAT_LINUX]}bin{SLASH[PLAT_LINUX]}linux{SLASH[PLAT_LINUX]}"
			]
		}

		def_config = {

			"build_mode"  : "debug",
			"isolate_path": {
				PLAT_WINDOWS: isolate_path_win,
				PLAT_LINUX: isolate_path_linux
			},
			"cc": "gcc",

			"out":{
				PLAT_WINDOWS: f"{project_name}.exe",
				PLAT_LINUX  : project_name
			},

			"c_files": [],

			"c_flags":{
				PLAT_WINDOWS: ["-Wl,-rpath='$ORIGIN'"],
				PLAT_LINUX  : ["-Wl,-rpath='$ORIGIN'"]
			},

			"include_path": {
				PLAT_WINDOWS: __isolate_includes[PLAT_WINDOWS],
				PLAT_LINUX  : __isolate_includes[PLAT_LINUX]
			},
			
			"lib_path": {
				PLAT_WINDOWS: __isolate_lib_path[PLAT_WINDOWS],
				PLAT_LINUX  : __isolate_lib_path[PLAT_LINUX]
			 },

			"libs": {
				PLAT_WINDOWS: __isolate_libs[PLAT_WINDOWS],
				PLAT_LINUX  : __isolate_libs[PLAT_LINUX]
			},

			"dll_path": {
				PLAT_WINDOWS: __isolate_vendor_dlls[PLAT_WINDOWS],
				PLAT_LINUX  : __isolate_vendor_dlls[PLAT_LINUX]
			}

		}
		print("[CMD]: Generating default config file.")
		json.dump(def_config, open(project_path + SLASH[self.platform] + DEF_CONFIG_NAME, "w"), indent=2)



