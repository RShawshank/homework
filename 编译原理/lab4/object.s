.data
_Prompt: .asciiz "Enter an integer:  "
_ret: .asciiz "\n"
.globl main
.text
j main
read:
  li $v0,4
  la $a0,_Prompt
  syscall
  li $v0,5
  syscall
  jr $ra
write:
  li $v0,1
  syscall
  li $v0,4
  la $a0,_ret
  syscall
  move $v0,$0
  jr $ra

fibo:
  li $t3, 1
  sw $t3, 16($sp)
  lw $t1, 12($sp)
  lw $t2, 16($sp)
  beq $t1,$t2,label3
  j label4
label4:
  li $t3, 2
  sw $t3, 20($sp)
  lw $t1, 12($sp)
  lw $t2, 20($sp)
  beq $t1,$t2,label3
  j label2
label3:
  li $t3, 1
  sw $t3, 24($sp)
  lw $v0,24($sp)
  jr $ra
label2:
  li $t3, 1
  sw $t3, 28($sp)
  lw $t1, 12($sp)
  lw $t2, 28($sp)
  sub $t3, $t1, $t2
  sw $t3, 32($sp)
  move $t0,$sp
  addi $sp, $sp, -56
  sw $ra,0($sp)
  lw $t1, 32($t0)
  move $t3,$t1
  sw $t3,12($sp)
  jal fibo
  lw $ra,0($sp)
  addi $sp,$sp,56
  sw $v0,36($sp)
  li $t3, 2
  sw $t3, 40($sp)
  lw $t1, 12($sp)
  lw $t2, 40($sp)
  sub $t3, $t1, $t2
  sw $t3, 44($sp)
  move $t0,$sp
  addi $sp, $sp, -56
  sw $ra,0($sp)
  lw $t1, 44($t0)
  move $t3,$t1
  sw $t3,12($sp)
  jal fibo
  lw $ra,0($sp)
  addi $sp,$sp,56
  sw $v0,48($sp)
  lw $t1, 36($sp)
  lw $t2, 48($sp)
  add $t3, $t1, $t2
  sw $t3, 52($sp)
  lw $v0,52($sp)
  jr $ra
label1:

main:
  addi $sp, $sp, -56
  li $t3, 1
  sw $t3, 24($sp)
  lw $t1, 24($sp)
  move $t3, $t1
  sw $t3, 16($sp)
label17:
  li $t3, 10
  sw $t3, 28($sp)
  lw $t1, 16($sp)
  lw $t2, 28($sp)
  blt $t1,$t2,label7
  j label6
label7:
  li $t3, 9
  sw $t3, 0($sp)
  lw $t1, 16($sp)
  lw $t2, 0($sp)
  bgt $t1,$t2,label10
  j label11
label10:
  lw $t1, 16($sp)
  move $t3, $t1
  sw $t3, 72($gp)
  move $t0,$sp
  addi $sp, $sp, -56
  sw $ra,0($sp)
  lw $t1, 16($t0)
  move $t3,$t1
  sw $t3,12($sp)
  jal fibo
  lw $ra,0($sp)
  addi $sp,$sp,56
  sw $v0,4($sp)
  lw $t1, 4($sp)
  move $t3, $t1
  sw $t3, 20($sp)
  lw $a0, 20($sp)
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal write
  lw $ra,0($sp)
  addi $sp, $sp, 4
  j label6
  j label9
label11:
  move $t0,$sp
  addi $sp, $sp, -56
  sw $ra,0($sp)
  lw $t1, 16($t0)
  move $t3,$t1
  sw $t3,12($sp)
  jal fibo
  lw $ra,0($sp)
  addi $sp,$sp,56
  sw $v0,12($sp)
  lw $t1, 12($sp)
  move $t3, $t1
  sw $t3, 20($sp)
  lw $a0, 20($sp)
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal write
  lw $ra,0($sp)
  addi $sp, $sp, 4
  j label8
label9:
  li $t3, 1
  sw $t3, 20($sp)
  lw $t1, 16($sp)
  lw $t2, 20($sp)
  add $t3, $t1, $t2
  sw $t3, 24($sp)
  lw $t1, 24($sp)
  move $t3, $t1
  sw $t3, 16($sp)
label8:
  li $t3, 1
  sw $t3, 32($sp)
  lw $t1, 16($sp)
  lw $t2, 32($sp)
  add $t3, $t1, $t2
  sw $t3, 36($sp)
  lw $t1, 36($sp)
  move $t3, $t1
  sw $t3, 16($sp)
  j label17
label6:
  li $t3, 1
  sw $t3, 52($sp)
  li $v0, 10
  syscall
label5:
