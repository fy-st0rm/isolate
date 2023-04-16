from config import *
import sys

class IpmBuilder:
	def compile(self, config: IpmConfig):
		if config.platform == PLAT_WINDOWS:
			self.__compile_windows(config)
		elif config.platform == PLAT_LINUX:
			self.__compile_linux(config)
		else:
			print("Unsupported platform:", config.platform)
			sys.exit()

	def exec(self, cmd):
		print("[CMD]:", cmd, end="\n\n")
		os.system(cmd)

	def __compile(self, config: IpmConfig):

		# Creating all the necessary commands
		c_flags       = " ".join(config.c_flags[config.platform])
		include_path  = " ".join(map(("-I" + config.project_dir + "{0}").format, config.include_path[config.platform]))
		lib_path      = " ".join(map(("-L" + config.project_dir + "{0}").format, config.lib_path[config.platform]))
		lib           = " ".join(map("-l{0}".format, config.libs[config.platform]))

		# Compiling
		for c_file, o_file in zip(config.c_files, config.o_files):
			cmp_cmd = f"{config.cc} {c_flags} {include_path} -o {o_file} -c {c_file}"
			self.exec(cmp_cmd)

		# Making build directory
		if not os.path.exists(config.out_dir):
			os.makedirs(config.out_dir)

		# Building
		files = " ".join(config.o_files)
		out = config.out_dir + config.slash + config.out[config.platform]
		build_cmd = f"{config.cc} {c_flags} -o {out} {files} {lib_path} {lib}"
		self.exec(build_cmd)

		# Generating compile commands for Clang
		self.__generate_cmp_cmds(config, c_flags, include_path)

	def __compile_windows(self, config: IpmConfig):
		self.__compile(config)

		# Coping the .dll into executable folder
		for dll in config.dll_path[config.platform]:
			self.exec(f"copy {dll}*.dll {config.out_dir}")

		# Cleaning
		files = " ".join(config.o_files)
		self.exec(f"erase {files}")

	def __compile_linux(self, config):
		self.__compile(config)

		# Coping the .so into executable folder
		for dll in config.dll_path[config.platform]:
			self.exec(f"cp {dll}*.so {config.out_dir}")

		# Cleaning
		files = " ".join(config.o_files)
		self.exec(f"rm {files}")

	def __generate_cmp_cmds(self, config: IpmConfig, c_flags, include_path):
		cmp_cmds = {}

		# Loading old file
		if os.path.isfile(config.project_dir + CMP_CMDS_FILE):
			cmp_cmds_org = json.load(open(config.project_dir + CMP_CMDS_FILE, "r"))
			for i in cmp_cmds_org:
				cmp_cmds.update({i["file"]: i})

		for c_file, o_file in zip(config.c_files, config.o_files):
			cmd = f"{config.cc} {c_flags} {include_path} -o {o_file} -c {c_file}"
			section = {
				"directory": config.project_dir,
				"command": cmd, 
				"file": c_file
			}
			cmp_cmds.update({c_file: section})

		dump = [cmp_cmds[i] for i in cmp_cmds]
		json.dump(dump, open(config.project_dir + CMP_CMDS_FILE, "w"), indent=4)

