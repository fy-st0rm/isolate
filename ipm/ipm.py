#!/bin/python
import sys
from config import *
from builder import *


def print_help():
	print("ipm [cmd]")
	print("    - init: Initializes the isolate project")
	print("    - run [file(optional)]: Runs the project by reading the default config file or supplied config file")


if len(sys.argv) <= 1:
	print_help()
	sys.exit()

# Removing the program name
sys.argv.pop(0)

# Config file
config_file = IpmConfig()

# Builder
builder = IpmBuilder()

# Going through all the command
for cmd in sys.argv:

	if cmd == "init":
		project_name = input("Project name: ")
		isolate_path = input("Isolate path: ")

		if os.path.exists(project_name):
			print(project_name, "project already exists.")
			sys.exit()

		print("Creating new project")
		os.mkdir(project_name)
		config_file.generate_default(project_name, os.path.curdir + config_file.slash + project_name, isolate_path)

	elif cmd == "run":
		idx = sys.argv.index(cmd)

		# Reading the file name if provided
		conf = DEF_CONFIG_NAME
		if idx < len(sys.argv) - 1:
			conf = sys.argv.pop(idx + 1)

		if not os.path.exists(conf):
			print("File not found:", conf)
			sys.exit()

		print("Loading config file...")
		config_file.load(conf)

		print("Building...")
		builder.compile(config_file)

	else:
		print("Unknown command:", cmd)
		print_help()
		sys.exit()

