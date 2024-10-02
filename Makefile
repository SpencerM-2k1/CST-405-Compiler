# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Files
LEXER = lex.yy.c
PARSER = parser.tab.c
AST = AST.c
SYMBOL_TABLE = symbolTable.c
SEMANTIC = semantic.c
CODE_GENERATOR = codeGenerator.c
OPTIMIZER = optimizer.c
OPERAND_STACK = operandStack.c

# Header Files
HEADERS = AST.h codeGenerator.h symbolTable.h semantic.h parser.tab.h operandStack.h

# Object Files
OBJS = $(LEXER:.c=.o) $(PARSER:.c=.o) $(AST:.c=.o) $(SYMBOL_TABLE:.c=.o) $(SEMANTIC:.c=.o) $(CODE_GENERATOR:.c=.o) $(OPTIMIZER:.c=.o) $(OPERAND_STACK:.c=.o)

# Output executable
EXEC = parser

# Directory to store MIPS output
INPUT_DIR = samples
OUTPUT_DIR = output

# Default rule to build the executable
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Rules to generate .o files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Flex and Bison
lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

parser.tab.c parser.tab.h: parser.y
	bison -d -v parser.y

# Run the parser and redirect output to output.txt
run: $(EXEC)
	./$(EXEC) $(INPUT_DIR)/testProg.cmm > $(OUTPUT_DIR)/output.txt
	@echo "Parsing output saved to $(OUTPUT_DIR)/output.txt"

# Generate the MIPS assembly and save to output.asm
mips: $(EXEC)
	./$(EXEC) $(INPUT_DIR)/classProg.cmm > $(OUTPUT_DIR)/output.txt # This generates MIPS assembly via codeGenerator.c
	@echo "MIPS code saved to $(OUTPUT_DIR)/output.asm"
	@echo "Output log saved to $(OUTPUT_DIR)/output.asm"

# Test 1: Online Classroom example
test1: $(EXEC)
	./$(EXEC) $(INPUT_DIR)/testProg1.cmm > $(OUTPUT_DIR)/output.txt # This generates MIPS assembly via codeGenerator.c
	@echo "MIPS code saved to $(OUTPUT_DIR)/output.asm"
	@echo "Output log saved to $(OUTPUT_DIR)/output.txt"

# Test 2: Simple expressions
test2: $(EXEC)
	./$(EXEC) $(INPUT_DIR)/testProg2.cmm > $(OUTPUT_DIR)/output.txt # This generates MIPS assembly via codeGenerator.c
	@echo "MIPS code saved to $(OUTPUT_DIR)/output.asm"
	@echo "Output log saved to $(OUTPUT_DIR)/output.txt"

# Test 3: Modified classroom example - demonstrates preservation of associativity
test3: $(EXEC)
	./$(EXEC) $(INPUT_DIR)/testProg3.cmm > $(OUTPUT_DIR)/output.txt # This generates MIPS assembly via codeGenerator.c
	@echo "MIPS code saved to $(OUTPUT_DIR)/output.asm"
	@echo "Output log saved to $(OUTPUT_DIR)/output.txt"


# Debug with gdb
debug: $(EXEC)
	gdb --args $(EXEC) $(INPUT_DIR)/classProg.cmm

# Clean rule to remove compiled files
clean:
	rm -f $(EXEC) $(OBJS) parser.tab.c parser.tab.h lex.yy.c parser.output
	rm -f $(OUTPUT_DIR)/output.txt $(OUTPUT_DIR)/output.asm $(OUTPUT_DIR)/TAC.ir $(OUTPUT_DIR)/TACOptimized.ir

