.data
   newline: .asciiz "\n"
	y_var: .word 0
	x_var: .word 0
	c_var: .word 0
	b_var: .word 0
	f_var: .float 0.0
	a_var: .word 0
	t0: .word 0
	t1: .word 0
	t2: .word 0
	t3: .word 0
	t4: .word 0
	t5: .word 0
	t6: .word 0
	t7: .word 0
	t8: .word 0
	t9: .word 0
	t10: .word 0
	t11: .word 0
	t12: .word 0
	t13: .word 0
	t14: .word 0
	t15: .word 0
	t16: .word 0
	t17: .word 0
	t18: .word 0
	t19: .word 0
	t20: .word 0
	t21: .word 0
	t22: .word 0
	t23: .word 0
.text
.globl main
main:
	li $t0, 1 #ASSIGN [t0 = 1 assign (null)]
	sw $t0, t0
	lw $t0, t0 #STORE [x_var = t0 store (null)]
	sw $t0, x_var
	li $t0, 12 #ASSIGN [t1 = 12 assign (null)]
	sw $t0, t1
	lw $t0, t1 #STORE [a_var = t1 store (null)]
	sw $t0, a_var
	li $t0, 23 #ASSIGN [t2 = 23 assign (null)]
	sw $t0, t2
	li $t0, 10 #ASSIGN [t3 = 10 assign (null)]
	sw $t0, t3
	lw $t0, t2 #ADD [t4 = t2 + t3]
	lw $t1, t3
	add $t0, $t0, $t1
	sw $t0, t4
	lw $t0, t4 #STORE [b_var = t4 store (null)]
	sw $t0, b_var
	lw $t0, a_var #LOAD [t5 = a_var load (null)]
	sw $t0, t5
	lw $t0, a_var #LOAD [t6 = a_var load (null)]
	sw $t0, t6
	lw $t0, t5 #ADD [t7 = t5 + t6]
	lw $t1, t6
	add $t0, $t0, $t1
	sw $t0, t7
	li $t0, 1 #ASSIGN [t8 = 1 assign (null)]
	sw $t0, t8
	lw $t0, t7 #ADD [t9 = t7 + t8]
	lw $t1, t8
	add $t0, $t0, $t1
	sw $t0, t9
	lw $t0, t9 #STORE [x_var = t9 store (null)]
	sw $t0, x_var
	li $t0, 15 #ASSIGN [t10 = 15 assign (null)]
	sw $t0, t10
	lw $t0, t10 #STORE [c_var = t10 store (null)]
	sw $t0, c_var
	lw $t0, x_var #LOAD [t11 = x_var load (null)]
	sw $t0, t11
	lw $t0, a_var #LOAD [t12 = a_var load (null)]
	sw $t0, t12
	lw $t0, t11 #ADD [t13 = t11 + t12]
	lw $t1, t12
	add $t0, $t0, $t1
	sw $t0, t13
	lw $t0, b_var #LOAD [t14 = b_var load (null)]
	sw $t0, t14
	lw $t0, t13 #SUB [t15 = t13 - t14]
	lw $t1, t14
	sub $t0, $t0, $t1
	sw $t0, t15
	lw $t0, c_var #LOAD [t16 = c_var load (null)]
	sw $t0, t16
	lw $t0, a_var #LOAD [t17 = a_var load (null)]
	sw $t0, t17
	lw $t0, t16 #MULTIPLY [t18 = t16 * t17]
	lw $t1, t17
	mul $t0, $t0, $t1
	sw $t0, t18
	lw $t0, t15 #ADD [t19 = t15 + t18]
	lw $t1, t18
	add $t0, $t0, $t1
	sw $t0, t19
	li $t0, 5 #ASSIGN [t20 = 5 assign (null)]
	sw $t0, t20
	lw $t0, t19 #SUB [t21 = t19 - t20]
	lw $t1, t20
	sub $t0, $t0, $t1
	sw $t0, t21
	lw $t0, t21 #STORE [y_var = t21 store (null)]
	sw $t0, y_var
	lw $t0, x_var #LOAD [t22 = x_var load (null)]
	sw $t0, t22
	lw $a0, t22 #WRITE [(null) = t22 write (null)]
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	lw $t0, y_var #LOAD [t23 = y_var load (null)]
	sw $t0, t23
	lw $a0, t23 #WRITE [(null) = t23 write (null)]
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $v0, 10
	syscall
