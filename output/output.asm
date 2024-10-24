.text
.globl main
main:
	li $t0, 0 #ASSIGN [i0 = 0 assign.int (null)]
	sw $t0, i0
	lw $t0, i0 #STORE [baseIndex_var = i0 store.int (null)]
	sw $t0, baseIndex_var
	lw $t0, baseIndex_var #LOAD [i1 = baseIndex_var load.int (null)]
	sw $t0, i1
	li $t0, 32 #ASSIGN [i2 = 32 assign.int (null)]
	sw $t0, i2
	la $t0, intArray_var #STORE (int arr) [intArray_var = i2 store.intIndex i1]
	lw $t1, i1
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, i2
	sw $t3, 0($t2)
	li $t0, 3 #ASSIGN [i3 = 3 assign.int (null)]
	sw $t0, i3
	li $t0, 2 #ASSIGN [i4 = 2 assign.int (null)]
	sw $t0, i4
	lw $t0, i3 #SUB [i5 = i3 -.int i4]
	lw $t1, i4
	sub $t0, $t0, $t1
	sw $t0, i5
	li $t0, 67 #ASSIGN [i6 = 67 assign.int (null)]
	sw $t0, i6
	la $t0, intArray_var #STORE (int arr) [intArray_var = i6 store.intIndex i5]
	lw $t1, i5
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, i6
	sw $t3, 0($t2)
	li $t0, 1 #ASSIGN [i7 = 1 assign.int (null)]
	sw $t0, i7
	li $t0, 1 #ASSIGN [i8 = 1 assign.int (null)]
	sw $t0, i8
	lw $t0, i7 #ADD [i9 = i7 +.int i8]
	lw $t1, i8
	add $t0, $t0, $t1
	sw $t0, i9
	li $t0, 45 #ASSIGN [i10 = 45 assign.int (null)]
	sw $t0, i10
	la $t0, intArray_var #STORE (int arr) [intArray_var = i10 store.intIndex i9]
	lw $t1, i9
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, i10
	sw $t3, 0($t2)
	li $t0, 9 #ASSIGN [i11 = 9 assign.int (null)]
	sw $t0, i11
	li $t0, 3 #ASSIGN [i12 = 3 assign.int (null)]
	sw $t0, i12
	lw $t0, i11 #MULTIPLY [i13 = i11 /.int i12]
	lw $t1, i12
	div $t0, $t1
	mflo $t0
	sw $t0, i13
	li $t0, 98 #ASSIGN [i14 = 98 assign.int (null)]
	sw $t0, i14
	la $t0, intArray_var #STORE (int arr) [intArray_var = i14 store.intIndex i13]
	lw $t1, i13
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, i14
	sw $t3, 0($t2)
	li $t0, 2 #ASSIGN [i15 = 2 assign.int (null)]
	sw $t0, i15
	li $t0, 2 #ASSIGN [i16 = 2 assign.int (null)]
	sw $t0, i16
	lw $t0, i15 #MULTIPLY [i17 = i15 *.int i16]
	lw $t1, i16
	mul $t0, $t0, $t1
	sw $t0, i17
	li $t0, 21 #ASSIGN [i18 = 21 assign.int (null)]
	sw $t0, i18
	la $t0, intArray_var #STORE (int arr) [intArray_var = i18 store.intIndex i17]
	lw $t1, i17
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, i18
	sw $t3, 0($t2)
	li $t0, 0 #ASSIGN [i19 = 0 assign.int (null)]
	sw $t0, i19
	la $t0, intArray_var #LOAD (int arr) [i20 = intArray_var load.intIndex i19]
	lw $t1, i19
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, 0($t2)
	sw $t3, i20
	lw $t0, i20 #STORE [outputInt_var = i20 store.int (null)]
	sw $t0, outputInt_var
	lw $t0, outputInt_var #LOAD [i21 = outputInt_var load.int (null)]
	sw $t0, i21
	lw $a0, i21 #WRITE [(null) = i21 write.int (null)]
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $t0, 1 #ASSIGN [i22 = 1 assign.int (null)]
	sw $t0, i22
	la $t0, intArray_var #LOAD (int arr) [i23 = intArray_var load.intIndex i22]
	lw $t1, i22
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, 0($t2)
	sw $t3, i23
	lw $t0, i23 #STORE [outputInt_var = i23 store.int (null)]
	sw $t0, outputInt_var
	lw $t0, outputInt_var #LOAD [i24 = outputInt_var load.int (null)]
	sw $t0, i24
	lw $a0, i24 #WRITE [(null) = i24 write.int (null)]
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $t0, 2 #ASSIGN [i25 = 2 assign.int (null)]
	sw $t0, i25
	la $t0, intArray_var #LOAD (int arr) [i26 = intArray_var load.intIndex i25]
	lw $t1, i25
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, 0($t2)
	sw $t3, i26
	lw $t0, i26 #STORE [outputInt_var = i26 store.int (null)]
	sw $t0, outputInt_var
	lw $t0, outputInt_var #LOAD [i27 = outputInt_var load.int (null)]
	sw $t0, i27
	lw $a0, i27 #WRITE [(null) = i27 write.int (null)]
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $t0, 3 #ASSIGN [i28 = 3 assign.int (null)]
	sw $t0, i28
	la $t0, intArray_var #LOAD (int arr) [i29 = intArray_var load.intIndex i28]
	lw $t1, i28
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, 0($t2)
	sw $t3, i29
	lw $t0, i29 #STORE [outputInt_var = i29 store.int (null)]
	sw $t0, outputInt_var
	lw $t0, outputInt_var #LOAD [i30 = outputInt_var load.int (null)]
	sw $t0, i30
	lw $a0, i30 #WRITE [(null) = i30 write.int (null)]
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $t0, 4 #ASSIGN [i31 = 4 assign.int (null)]
	sw $t0, i31
	la $t0, intArray_var #LOAD (int arr) [i32 = intArray_var load.intIndex i31]
	lw $t1, i31
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, 0($t2)
	sw $t3, i32
	lw $t0, i32 #STORE [outputInt_var = i32 store.int (null)]
	sw $t0, outputInt_var
	lw $t0, outputInt_var #LOAD [i33 = outputInt_var load.int (null)]
	sw $t0, i33
	lw $a0, i33 #WRITE [(null) = i33 write.int (null)]
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	lw $t0, baseIndex_var #LOAD [i34 = baseIndex_var load.int (null)]
	sw $t0, i34
	lwc1 $f0, float_const_0 #ASSIGN [f0 = 3.200000 assign.float (null)]
	s.s $f0, f0
	la $t0, floatArray_var #STORE (int arr) [floatArray_var = f0 store.intIndex i34]
	lw $t1, i34
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, f0
	sw $t3, 0($t2)
	lw $t0, baseIndex_var #LOAD [i35 = baseIndex_var load.int (null)]
	sw $t0, i35
	li $t0, 1 #ASSIGN [i36 = 1 assign.int (null)]
	sw $t0, i36
	lw $t0, i35 #ADD [i37 = i35 +.int i36]
	lw $t1, i36
	add $t0, $t0, $t1
	sw $t0, i37
	lwc1 $f0, float_const_1 #ASSIGN [f1 = 6.700000 assign.float (null)]
	s.s $f0, f1
	la $t0, floatArray_var #STORE (int arr) [floatArray_var = f1 store.intIndex i37]
	lw $t1, i37
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, f1
	sw $t3, 0($t2)
	lw $t0, baseIndex_var #LOAD [i38 = baseIndex_var load.int (null)]
	sw $t0, i38
	li $t0, 2 #ASSIGN [i39 = 2 assign.int (null)]
	sw $t0, i39
	lw $t0, i38 #ADD [i40 = i38 +.int i39]
	lw $t1, i39
	add $t0, $t0, $t1
	sw $t0, i40
	lwc1 $f0, float_const_2 #ASSIGN [f2 = 4.500000 assign.float (null)]
	s.s $f0, f2
	la $t0, floatArray_var #STORE (int arr) [floatArray_var = f2 store.intIndex i40]
	lw $t1, i40
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, f2
	sw $t3, 0($t2)
	lw $t0, baseIndex_var #LOAD [i41 = baseIndex_var load.int (null)]
	sw $t0, i41
	li $t0, 3 #ASSIGN [i42 = 3 assign.int (null)]
	sw $t0, i42
	lw $t0, i41 #ADD [i43 = i41 +.int i42]
	lw $t1, i42
	add $t0, $t0, $t1
	sw $t0, i43
	lwc1 $f0, float_const_3 #ASSIGN [f3 = 9.800000 assign.float (null)]
	s.s $f0, f3
	la $t0, floatArray_var #STORE (int arr) [floatArray_var = f3 store.intIndex i43]
	lw $t1, i43
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, f3
	sw $t3, 0($t2)
	lw $t0, baseIndex_var #LOAD [i44 = baseIndex_var load.int (null)]
	sw $t0, i44
	li $t0, 4 #ASSIGN [i45 = 4 assign.int (null)]
	sw $t0, i45
	lw $t0, i44 #ADD [i46 = i44 +.int i45]
	lw $t1, i45
	add $t0, $t0, $t1
	sw $t0, i46
	lwc1 $f0, float_const_4 #ASSIGN [f4 = 2.100000 assign.float (null)]
	s.s $f0, f4
	la $t0, floatArray_var #STORE (int arr) [floatArray_var = f4 store.intIndex i46]
	lw $t1, i46
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, f4
	sw $t3, 0($t2)
	li $t0, 0 #ASSIGN [i47 = 0 assign.int (null)]
	sw $t0, i47
	la $t0, floatArray_var #LOAD (int arr) [i48 = floatArray_var load.intIndex i47]
	lw $t1, i47
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, 0($t2)
	sw $t3, i48
	l.s $f0, i48 #STORE [outputFloat_var = i48 store.float (null)]
	s.s $f0, outputFloat_var
	l.s $f0, outputFloat_var #LOAD [f5 = outputFloat_var load.float (null)]
	s.s $f0, f5
	l.s $f12, f5 #WRITE [(null) = f5 write.float (null)]
	li $v0, 2
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $t0, 1 #ASSIGN [i49 = 1 assign.int (null)]
	sw $t0, i49
	la $t0, floatArray_var #LOAD (int arr) [i50 = floatArray_var load.intIndex i49]
	lw $t1, i49
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, 0($t2)
	sw $t3, i50
	l.s $f0, i50 #STORE [outputFloat_var = i50 store.float (null)]
	s.s $f0, outputFloat_var
	l.s $f0, outputFloat_var #LOAD [f6 = outputFloat_var load.float (null)]
	s.s $f0, f6
	l.s $f12, f6 #WRITE [(null) = f6 write.float (null)]
	li $v0, 2
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $t0, 2 #ASSIGN [i51 = 2 assign.int (null)]
	sw $t0, i51
	la $t0, floatArray_var #LOAD (int arr) [i52 = floatArray_var load.intIndex i51]
	lw $t1, i51
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, 0($t2)
	sw $t3, i52
	l.s $f0, i52 #STORE [outputFloat_var = i52 store.float (null)]
	s.s $f0, outputFloat_var
	l.s $f0, outputFloat_var #LOAD [f7 = outputFloat_var load.float (null)]
	s.s $f0, f7
	l.s $f12, f7 #WRITE [(null) = f7 write.float (null)]
	li $v0, 2
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $t0, 3 #ASSIGN [i53 = 3 assign.int (null)]
	sw $t0, i53
	la $t0, floatArray_var #LOAD (int arr) [i54 = floatArray_var load.intIndex i53]
	lw $t1, i53
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, 0($t2)
	sw $t3, i54
	l.s $f0, i54 #STORE [outputFloat_var = i54 store.float (null)]
	s.s $f0, outputFloat_var
	l.s $f0, outputFloat_var #LOAD [f8 = outputFloat_var load.float (null)]
	s.s $f0, f8
	l.s $f12, f8 #WRITE [(null) = f8 write.float (null)]
	li $v0, 2
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $t0, 4 #ASSIGN [i55 = 4 assign.int (null)]
	sw $t0, i55
	la $t0, floatArray_var #LOAD (int arr) [i56 = floatArray_var load.intIndex i55]
	lw $t1, i55
	sll $t1, $t1, 2
	add $t2, $t0, $t1
	lw $t3, 0($t2)
	sw $t3, i56
	l.s $f0, i56 #STORE [outputFloat_var = i56 store.float (null)]
	s.s $f0, outputFloat_var
	l.s $f0, outputFloat_var #LOAD [f9 = outputFloat_var load.float (null)]
	s.s $f0, f9
	l.s $f12, f9 #WRITE [(null) = f9 write.float (null)]
	li $v0, 2
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $v0, 10 #END
	syscall

.data
   newline: .asciiz "\n"
	outputFloat_var: .float 0.0
	baseIndex_var: .word 0
	floatArray_var: .word 0, 0, 0, 0, 0, 0
	intArray_var: .word 0, 0, 0, 0, 0, 0
	outputInt_var: .word 0
	i0: .word 0
	i1: .word 0
	i2: .word 0
	i3: .word 0
	i4: .word 0
	i5: .word 0
	i6: .word 0
	i7: .word 0
	i8: .word 0
	i9: .word 0
	i10: .word 0
	i11: .word 0
	i12: .word 0
	i13: .word 0
	i14: .word 0
	i15: .word 0
	i16: .word 0
	i17: .word 0
	i18: .word 0
	i19: .word 0
	i20: .word 0
	i21: .word 0
	i22: .word 0
	i23: .word 0
	i24: .word 0
	i25: .word 0
	i26: .word 0
	i27: .word 0
	i28: .word 0
	i29: .word 0
	i30: .word 0
	i31: .word 0
	i32: .word 0
	i33: .word 0
	i34: .word 0
	i35: .word 0
	i36: .word 0
	i37: .word 0
	i38: .word 0
	i39: .word 0
	i40: .word 0
	i41: .word 0
	i42: .word 0
	i43: .word 0
	i44: .word 0
	i45: .word 0
	i46: .word 0
	i47: .word 0
	i48: .word 0
	i49: .word 0
	i50: .word 0
	i51: .word 0
	i52: .word 0
	i53: .word 0
	i54: .word 0
	i55: .word 0
	i56: .word 0
	f0: .float 0.0
	f1: .float 0.0
	f2: .float 0.0
	f3: .float 0.0
	f4: .float 0.0
	f5: .float 0.0
	f6: .float 0.0
	f7: .float 0.0
	f8: .float 0.0
	f9: .float 0.0
	float_const_0: .float 3.200000
	float_const_1: .float 6.700000
	float_const_2: .float 4.500000
	float_const_3: .float 9.800000
	float_const_4: .float 2.100000
