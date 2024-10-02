.data
   newline: .asciiz "\n"
	y_var: .word 0
	x_var: .word 0
	c_var: .word 0
	b_var: .word 0
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
	li $t0, 33 #ASSIGN [t4 = 33 assign (null)]
	sw $t0, t4
	lw $t0, t4 #STORE [b_var = t4 store (null)]
	sw $t0, b_var
	lw $t0, a_var #LOAD [t5 = a_var load (null)]
	sw $t0, t5
	lw $t0, a_var #LOAD [t6 = a_var load (null)]
	sw $t0, t6
	lw $t0, t5 #ADD [t7 = t5 + t5]
	lw $t1, t5
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
	lw $t0, x_var #LOAD [t10 = x_var load (null)]
	sw $t0, t10
	lw $t0, a_var #LOAD [t11 = a_var load (null)]
	sw $t0, t11
	lw $t0, t10 #ADD [t12 = t10 + t6]
	lw $t1, t6
	add $t0, $t0, $t1
	sw $t0, t12
	lw $t0, b_var #LOAD [t13 = b_var load (null)]
	sw $t0, t13
	lw $t0, t12 #ADD [t14 = t12 + t13]
	lw $t1, t13
	add $t0, $t0, $t1
	sw $t0, t14
	lw $t0, c_var #LOAD [t15 = c_var load (null)]
	sw $t0, t15
	lw $t0, t14 #ADD [t16 = t14 + t15]
	lw $t1, t15
	add $t0, $t0, $t1
	sw $t0, t16
	lw $t0, a_var #LOAD [t17 = a_var load (null)]
	sw $t0, t17
	lw $t0, t16 #ADD [t18 = t16 + t11]
	lw $t1, t11
	add $t0, $t0, $t1
	sw $t0, t18
	li $t0, 5 #ASSIGN [t19 = 5 assign (null)]
	sw $t0, t19
	lw $t0, t18 #ADD [t20 = t18 + t19]
	lw $t1, t19
	add $t0, $t0, $t1
	sw $t0, t20
	lw $t0, t20 #STORE [y_var = t20 store (null)]
	sw $t0, y_var
	lw $t0, x_var #LOAD [t21 = x_var load (null)]
	sw $t0, t21
	lw $a0, t10 #WRITE [(null) = t10 write (null)]
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	lw $t0, y_var #LOAD [t22 = y_var load (null)]
	sw $t0, t22
	lw $a0, t22 #WRITE [(null) = t22 write (null)]
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $v0, 10
	syscall
