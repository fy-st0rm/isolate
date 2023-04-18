PLAT_WINDOWS    = "windows"
PLAT_LINUX      = "linux"
DEF_CONFIG_NAME = "ipm_config.json"
CMP_CMDS_FILE   = "compile_commands.json"
SLASH = {
	PLAT_WINDOWS: "\\",
	PLAT_LINUX: "/"
}

def log_info(msg):
	print("\033[95m[INFO]:", msg);
	print("\033[0m", end="")

def log_sucess(msg):
	print("\033[92m[SUCESS]:", msg);
	print("\033[0m", end="")
