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

COMAND_SEPARATOR = @echo "--------------------------------------------------"

all: output build_funcao_hash funcao_hash

output:
	@mkdir -p $(PATH_OUTPUT_DIR)

build_funcao_hash:
	@echo "Compilando funcao_hash_101.c e funcao_hash_150.c..."
	@gcc $(GCC_FLAGS) funcao_hash_101.c -o output/funcao_hash_101
	@gcc $(GCC_FLAGS) funcao_hash_150.c -o output/funcao_hash_150

funcao_hash:
	@$(COMAND_SEPARATOR) "\nQuantidade 101\n"
	@./$(PATH_OUTPUT_DIR)/funcao_hash_101
	@$(COMAND_SEPARATOR) "\nQuantidade 150\n"
	@./$(PATH_OUTPUT_DIR)/funcao_hash_150
	@$(COMAND_SEPARATOR)

clean:
	@rm -rf $(PATH_OUTPUT_DIR)
