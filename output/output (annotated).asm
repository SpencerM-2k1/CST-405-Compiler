.text
.globl main
main:
	li $t0, 20 #ASSIGN [i0 = 20 assign.int (null)]
	sw $t0, i0
	lw $t0, i0 #STORE [baseIndex_var = i0 store.int (null)]
	sw $t0, baseIndex_var
	lw $t0, baseIndex_var #LOAD [i1 = baseIndex_var load.int (null)]
	sw $t0, i1
	li $t0, 5 #ASSIGN [i2 = 5 assign.int (null)]
	sw $t0, i2
	la $t0, testArray_var #LOAD (int arr) [testArray_var = i2 store.intIndex i1]
	lw $t1, i1
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, testArray_var
	sw $t3, 0($t2)
	lw $t0, baseIndex_var #LOAD [i3 = baseIndex_var load.int (null)]
	sw $t0, i3
	la $t0, testArray_var #STORE (int arr) [i4 = testArray_var load.intIndex i3]
	lw $t1, i3
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, 0($t2)
	sw $t3, i4
	lw $t0, i4 #STORE [outputVar_var = i4 store.int (null)]
	sw $t0, outputVar_var
	lw $t0, outputVar_var #LOAD [i5 = outputVar_var load.int (null)]
	sw $t0, i5
	lw $a0, i5 #WRITE [(null) = i5 write.int (null)]
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $v0, 10 #END
	syscall

.data
   newline: .asciiz "\n"
	outputVar_var: .word 0
	baseIndex_var: .word 0
	testArray_var: .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	i0: .word 0
	i1: .word 0
	i2: .word 0
	i3: .word 0
	i4: .word 0
	i5: .word 0
