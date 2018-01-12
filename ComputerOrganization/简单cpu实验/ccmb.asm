.text
begin:
addi $1, $0, 16
addi $2, $0, 2

# test srlv
srlv $4, $1, $2
addi $2, $0, 34
syscall

# test sltiu
sltiu $4, $2, 100
addi $2, $0, 34
syscall

# test lbu
addi $1, $0, 0x1516
sw $1, 100
lbu $4, 101
addi $2, $0, 34
syscall

# test bltz
addi $1, $0, -1
bltz $1, begin
