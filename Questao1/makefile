# Variáveis de Configuração
C_FLAGS = -Wall -Wextra -Wconversion -Wpointer-arith -Wpedantic \
          -Werror=deprecated-declarations -Wformat -Wnull-dereference \
          -Wshadow -Wimplicit-fallthrough -Wunreachable-code \
          -Wduplicated-cond -Wduplicated-branches -Wrestrict \
          -Wdouble-promotion -Wformat-y2k -Wmissing-include-dirs \
          -Wlogical-op -Wswitch-enum -Wswitch-default -Wcast-align \
          -Wcast-qual -Wwrite-strings -Wundef -Wredundant-decls \
          -Wfloat-equal -Winline -Wstack-protector -Woverlength-strings \
          -Wc++-compat  -Wshift-overflow \
          -Wpacked -Wdisabled-optimization -Wunused-result


GCC_FLAGS = -g -I. $(C_FLAGS)

PATH_OUTPUT_DIR = output
PATH_SRC_DIR = src

COMAND_SEPARATOR = echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"

all: output build_files main

output:
	@mkdir -p $(PATH_OUTPUT_DIR)

# Buildar um arquivo de teste
$(PATH_OUTPUT_DIR)/%: $(PATH_TEST_DIR)/%.c
	@gcc $(GCC_FLAGS) ${PATH_SRC_FILES} $< -o $@

build_files: $(PATH_TEST_FILES)
	@gcc $(GCC_FLAGS) ${PATH_SRC_FILES} main.c -o $(PATH_OUTPUT_DIR)/main -lm

main:
	@$(COMAND_SEPARATOR)
	@./$(PATH_OUTPUT_DIR)/main
	@$(COMAND_SEPARATOR)


clean:
	@rm -rf $(PATH_OUTPUT_DIR)
