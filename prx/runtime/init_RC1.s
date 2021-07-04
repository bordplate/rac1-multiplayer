.global _shk_prx_elf_substitute_impl
_shk_prx_elf_substitute_impl:
.int ._shk_prx_elf_substitute_impl
.int _shk_elf_toc

# function extracted from the game to make extra room for code
# unsigned __int64 __fastcall sub_5F7140(unsigned int *a1, _DWORD *a2, _DWORD *a3, _BYTE *a4)
.global ._shk_prx_elf_substitute_impl
._shk_prx_elf_substitute_impl:
.set var_90, -0x90
.set var_8C, -0x8C
.set var_88, -0x88
.set var_84, -0x84
.set var_80, -0x80
.set var_7F, -0x7F
.set var_7E, -0x7E
.set var_7D, -0x7D
.set var_7C, -0x7C
.set var_7B, -0x7B
.set var_7A, -0x7A
.set var_79, -0x79
.set var_78, -0x78
.set var_77, -0x77
.set var_76, -0x76
.set var_75, -0x75
.set var_74, -0x74
.set var_73, -0x73
.set var_72, -0x72
.set var_71, -0x71
.set var_70, -0x70
.set var_68, -0x68
.set var_60, -0x60
.set var_58, -0x58
.set var_50, -0x50
.set var_48, -0x48
.set var_40, -0x40
.set back_chain, -0x38
.set var_30, -0x30
.set var_28, -0x28
.set var_20, -0x20
.set var_18, -0x18
.set var_10, -0x10
.set var_8, -8

std       r25, back_chain(r1)
mr        r25, r3
std       r29, var_18(r1)
std       r31, var_8(r1)
std       r18, var_70(r1)
std       r19, var_68(r1)
std       r20, var_60(r1)
std       r21, var_58(r1)
std       r22, var_50(r1)
std       r23, var_48(r1)
std       r24, var_40(r1)
std       r26, var_30(r1)
std       r27, var_28(r1)
std       r28, var_20(r1)
std       r30, var_10(r1)
lwz       r12, 8(r25)
mr        r3, r4
mr        r31, r5
lwz       r30, 4(r25)
cmplwi    cr7, r12, 3
lwz       r8, 0(r25)
mr        r29, r6
bgt       cr7, loc_5F71B4
clrldi    r9, r30, 32
addi      r30, r30, 4
lwz       r0, 0(r9)
sld       r0, r0, r12
addi      r12, r12, 0x20 # ' '
or        r8, r8, r0

loc_5F71B4: 
clrlwi    r5, r8, 28
lwz       r9, -0x7D90(r2)
srdi      r4, r8, 4
clrlslwi  r11, r5, 28,2
extsw     r10, r5
add       r11, r11, r9
add       r10, r10, r9
addi      r9, r9, 0x60 # '`'
cmpwi     cr7, r5, 0
addi      r7, r12, -4
lwz       r0, 0x20(r11)
lbz       r8, 0x430(r10)
add       r0, r0, r9
stw       r0, 0(r3)
stw       r8, 0(r31)
bne       cr7, loc_5F72B8
li        r0, 0xF
li        r9, 1
stb       r0, 0xF(r6)
li        r0, 2
stb       r9, 1(r6)
stb       r0, 2(r6)
li        r9, 3
li        r0, 4
stb       r9, 3(r6)
stb       r0, 4(r6)
li        r9, 5
li        r0, 6
stb       r9, 5(r6)
stb       r0, 6(r6)
li        r9, 7
li        r0, 8
stb       r9, 7(r6)
stb       r0, 8(r6)
li        r9, 9
li        r0, 0xA
stb       r9, 9(r6)
stb       r0, 0xA(r6)
li        r9, 0xB
li        r0, 0xC
stb       r9, 0xB(r6)
stb       r0, 0xC(r6)
li        r9, 0xD
li        r0, 0xE
stb       r5, 0(r6)
stb       r9, 0xD(r6)
stb       r0, 0xE(r6)

loc_5F7270: 
stw       r7, 8(r25)
stw       r30, 4(r25)
stw       r4, 0(r25)
ld        r18, var_70(r1)
ld        r19, var_68(r1)
ld        r20, var_60(r1)
ld        r21, var_58(r1)
ld        r22, var_50(r1)
ld        r23, var_48(r1)
ld        r24, var_40(r1)
ld        r25, back_chain(r1)
ld        r26, var_30(r1)
ld        r27, var_28(r1)
ld        r28, var_20(r1)
ld        r29, var_18(r1)
ld        r30, var_10(r1)
ld        r31, var_8(r1)
blr

loc_5F72B8: 
cmpwi     cr7, r7, 0
addi      r8, r12, -5
beq       cr7, loc_5F7620
clrlwi    r0, r4, 31
srwi      r9, r4, 1
cmpwi     cr7, r0, 0
beq       cr7, loc_5F7390

loc_5F72D4: 
cmplwi    cr7, r8, 2
clrldi    r11, r9, 32
ble       cr7, loc_5F7840

loc_5F72E0: 
li        r10, 0
addi      r7, r8, -3
clrlwi    r5, r11, 29
ori       r10, r10, 0xFFFF
srdi      r4, r11, 3
li        r8, 0
li        r31, 1
nop

loc_5F7300: 
cmplwi    cr7, r7, 3
add       r0, r8, r29
addi      r8, r8, 1
clrldi    r9, r30, 32
clrldi    r6, r0, 32
cmplw     cr6, r5, r8
clrldi    r11, r4, 32
bgt       cr7, loc_5F7334
lwz       r0, 0(r9)
addi      r30, r30, 4
sld       r0, r0, r7
addi      r7, r7, 0x20 # ' '
or        r11, r11, r0

loc_5F7334: 
clrlwi    r9, r11, 28
addi      r7, r7, -4
slw       r0, r31, r9
srdi      r4, r11, 4
stb       r9, 0(r6)
andc      r10, r10, r0
bge       cr6, loc_5F7300
li        r11, 0
nop

loc_5F7358: 
clrlwi    r0, r10, 31
srwi      r10, r10, 1
cmpwi     cr7, r0, 0
cmpwi     cr6, r10, 0
beq       cr7, loc_5F7384
addi      r5, r5, 1
add       r9, r5, r29
cmpwi     cr7, r5, 0xF
clrldi    r9, r9, 32
stb       r11, 0(r9)
beq       cr7, loc_5F7270

loc_5F7384: 
addi      r11, r11, 1
bne       cr6, loc_5F7358
b         loc_5F7270

loc_5F7390: 
cmplwi    cr7, r8, 1
clrldi    r11, r9, 32
ble       cr7, loc_5F785C

loc_5F739C: 
clrlwi    r22, r11, 30
srdi      r0, r11, 2
cmpwi     cr7, r22, 0
addi      r9, r8, -2
beq       cr7, loc_5F7644
cmpwi     cr7, r9, 0
addi      r10, r8, -3
bne       cr7, loc_5F73CC
clrldi    r9, r30, 32
li        r10, 0x1F
addi      r30, r30, 4
lwz       r0, 0(r9)

loc_5F73CC: 
clrlwi    r9, r0, 31
srwi      r11, r0, 1
cmpwi     cr7, r9, 0
bne       cr7, loc_5F7990
li        r0, 1
stb       r9, var_80(r1)
stb       r0, var_7F(r1)

loc_5F73E8: 
cmpwi     cr7, r10, 0
addi      r10, r10, -1
bne       cr7, loc_5F7404
clrldi    r9, r30, 32
li        r10, 0x1F
addi      r30, r30, 4
lwz       r11, 0(r9)

loc_5F7404: 
clrlwi    r0, r11, 31
srwi      r11, r11, 1
cmpwi     cr7, r0, 0
bne       cr7, loc_5F797C
li        r0, 2
li        r9, 3
stb       r0, var_7E(r1)
stb       r9, var_7D(r1)

loc_5F7424: 
cmpwi     cr7, r10, 0
addi      r10, r10, -1
bne       cr7, loc_5F7440
clrldi    r9, r30, 32
li        r10, 0x1F
addi      r30, r30, 4
lwz       r11, 0(r9)

loc_5F7440: 
clrlwi    r0, r11, 31
srwi      r11, r11, 1
cmpwi     cr7, r0, 0
bne       cr7, loc_5F7968
li        r0, 4
li        r9, 5
stb       r0, var_7C(r1)
stb       r9, var_7B(r1)

loc_5F7460: 
cmpwi     cr7, r10, 0
addi      r10, r10, -1
bne       cr7, loc_5F747C
clrldi    r9, r30, 32
li        r10, 0x1F
addi      r30, r30, 4
lwz       r11, 0(r9)

loc_5F747C: 
clrlwi    r0, r11, 31
srwi      r11, r11, 1
cmpwi     cr7, r0, 0
bne       cr7, loc_5F7954
li        r0, 6
li        r9, 7
stb       r0, var_7A(r1)
stb       r9, var_79(r1)

loc_5F749C: 
cmpwi     cr7, r10, 0
addi      r10, r10, -1
bne       cr7, loc_5F74B8
clrldi    r9, r30, 32
li        r10, 0x1F
addi      r30, r30, 4
lwz       r11, 0(r9)

loc_5F74B8: 
clrlwi    r0, r11, 31
srwi      r11, r11, 1
cmpwi     cr7, r0, 0
bne       cr7, loc_5F7940
li        r0, 8
li        r9, 9
stb       r0, var_78(r1)
stb       r9, var_77(r1)

loc_5F74D8: 
cmpwi     cr7, r10, 0
addi      r10, r10, -1
bne       cr7, loc_5F74F4
clrldi    r9, r30, 32
li        r10, 0x1F
addi      r30, r30, 4
lwz       r11, 0(r9)

loc_5F74F4: 
clrlwi    r0, r11, 31
srwi      r11, r11, 1
cmpwi     cr7, r0, 0
bne       cr7, loc_5F792C
li        r0, 0xA
li        r9, 0xB
stb       r0, var_76(r1)
stb       r9, var_75(r1)

loc_5F7514: 
cmpwi     cr7, r10, 0
addi      r10, r10, -1
bne       cr7, loc_5F7530
clrldi    r9, r30, 32
li        r10, 0x1F
addi      r30, r30, 4
lwz       r11, 0(r9)

loc_5F7530: 
clrlwi    r0, r11, 31
srwi      r11, r11, 1
cmpwi     cr7, r0, 0
bne       cr7, loc_5F7878
li        r0, 0xC
li        r9, 0xD
stb       r0, var_74(r1)
stb       r9, var_73(r1)

loc_5F7550: 
cmpwi     cr7, r10, 0
addi      r27, r10, -1
bne       cr7, loc_5F756C
clrldi    r9, r30, 32
li        r27, 0x1F
addi      r30, r30, 4
lwz       r11, 0(r9)

loc_5F756C: 
clrlwi    r0, r11, 31
srwi      r28, r11, 1
cmpwi     cr7, r0, 0
beq       cr7, loc_5F788C
li        r0, 0xF
li        r9, 0xE
stb       r0, var_72(r1)
stb       r9, var_71(r1)

loc_5F758C: 
cmpwi     cr7, r22, 1
beq       cr7, loc_5F7FD4
cmplwi    cr7, r27, 2
clrlwi    r7, r28, 29
ble       cr7, loc_5F7CD8

loc_5F75A0: 
addi      r19, r1, var_90
addi      r18, r1, var_80
mr        r10, r19
mr        r8, r18
addi      r3, r1, var_7E
li        r31, 2
li        r12, 2
li        r4, 0
b         loc_5F75EC

loc_5F75C4: 
lbz       r0, 0(r6)
addi      r10, r10, 1
addi      r3, r3, 1
addi      r12, r12, -1
stb       r0, 0(r5)

loc_5F75D8: 
cmpwi     cr7, r31, 0
srwi      r7, r7, 1
cmpwi     cr6, r12, 0
beq       cr7, loc_5F8068
beq       cr6, loc_5F79D4

loc_5F75EC: 
clrlwi    r0, r7, 31
clrldi    r6, r3, 32
cmpwi     cr7, r0, 0
clrldi    r11, r8, 32
clrldi    r5, r10, 32
addi      r4, r4, 1
bne       cr7, loc_5F75C4
lbz       r0, 0(r11)
addi      r31, r31, -1
addi      r10, r10, 1
addi      r8, r8, 1
stb       r0, 0(r5)
b         loc_5F75D8

loc_5F7620: 
clrldi    r9, r30, 32
li        r8, 0x1F
addi      r30, r30, 4
lwz       r4, 0(r9)
clrlwi    r0, r4, 31
srwi      r9, r4, 1
cmpwi     cr7, r0, 0
bne       cr7, loc_5F72D4
b         loc_5F7390

loc_5F7644: 
cmpwi     cr7, r9, 0
addi      r10, r8, -3
bne       cr7, loc_5F7660
clrldi    r9, r30, 32
li        r10, 0x1F
addi      r30, r30, 4
lwz       r0, 0(r9)

loc_5F7660: 
clrlwi    r9, r0, 31
srwi      r11, r0, 1
cmpwi     cr7, r9, 0
bne       cr7, loc_5F78A0
li        r0, 1
stb       r9, 0(r6)
stb       r0, 1(r6)

loc_5F767C: 
cmpwi     cr7, r10, 0
addi      r8, r10, -1
bne       cr7, loc_5F7698
clrldi    r9, r30, 32
li        r8, 0x1F
addi      r30, r30, 4
lwz       r11, 0(r9)

loc_5F7698: 
clrlwi    r0, r11, 31
srwi      r10, r11, 1
cmpwi     cr7, r0, 0
bne       cr7, loc_5F7914
mr        r11, r29
li        r0, 2
li        r9, 3
stbu      r0, 2(r11)
stb       r9, 1(r11)

loc_5F76BC: 
cmpwi     cr7, r8, 0
addi      r8, r8, -1
bne       cr7, loc_5F76D8
clrldi    r9, r30, 32
li        r8, 0x1F
addi      r30, r30, 4
lwz       r10, 0(r9)

loc_5F76D8: 
clrlwi    r0, r10, 31
srwi      r10, r10, 1
cmpwi     cr7, r0, 0
bne       cr7, loc_5F78FC
mr        r11, r29
li        r0, 4
li        r9, 5
stbu      r0, 4(r11)
stb       r9, 1(r11)

loc_5F76FC: 
cmpwi     cr7, r8, 0
addi      r8, r8, -1
bne       cr7, loc_5F7718
clrldi    r9, r30, 32
li        r8, 0x1F
addi      r30, r30, 4
lwz       r10, 0(r9)

loc_5F7718: 
clrlwi    r0, r10, 31
srwi      r10, r10, 1
cmpwi     cr7, r0, 0
bne       cr7, loc_5F78E4
mr        r11, r29
li        r0, 6
li        r9, 7
stbu      r0, 6(r11)
stb       r9, 1(r11)

loc_5F773C: 
cmpwi     cr7, r8, 0
addi      r8, r8, -1
bne       cr7, loc_5F7758
clrldi    r9, r30, 32
li        r8, 0x1F
addi      r30, r30, 4
lwz       r10, 0(r9)

loc_5F7758: 
clrlwi    r0, r10, 31
srwi      r10, r10, 1
cmpwi     cr7, r0, 0
bne       cr7, loc_5F78CC
mr        r11, r29
li        r0, 8
li        r9, 9
stbu      r0, 8(r11)
stb       r9, 1(r11)

loc_5F777C: 
cmpwi     cr7, r8, 0
addi      r8, r8, -1
bne       cr7, loc_5F7798
clrldi    r9, r30, 32
li        r8, 0x1F
addi      r30, r30, 4
lwz       r10, 0(r9)

loc_5F7798: 
clrlwi    r0, r10, 31
srwi      r10, r10, 1
cmpwi     cr7, r0, 0
bne       cr7, loc_5F79BC
mr        r11, r29
li        r0, 0xA
li        r9, 0xB
stbu      r0, 0xA(r11)
stb       r9, 1(r11)

loc_5F77BC: 
cmpwi     cr7, r8, 0
addi      r8, r8, -1
bne       cr7, loc_5F77D8
clrldi    r9, r30, 32
li        r8, 0x1F
addi      r30, r30, 4
lwz       r10, 0(r9)

loc_5F77D8: 
clrlwi    r0, r10, 31
srwi      r10, r10, 1
cmpwi     cr7, r0, 0
bne       cr7, loc_5F79A4
mr        r11, r29
li        r0, 0xC
li        r9, 0xD
stbu      r0, 0xC(r11)
stb       r9, 1(r11)

loc_5F77FC: 
cmpwi     cr7, r8, 0
addi      r7, r8, -1
bne       cr7, loc_5F7818
clrldi    r9, r30, 32
li        r7, 0x1F
addi      r30, r30, 4
lwz       r10, 0(r9)

loc_5F7818: 
clrlwi    r0, r10, 31
srwi      r4, r10, 1
cmpwi     cr7, r0, 0
beq       cr7, loc_5F78B4
mr        r11, r29
li        r0, 0xF
li        r9, 0xE
stbu      r0, 0xE(r11)
stb       r9, 1(r11)
b         loc_5F7270

loc_5F7840: 
clrldi    r9, r30, 32
addi      r30, r30, 4
lwz       r0, 0(r9)
sld       r0, r0, r8
addi      r8, r8, 0x20 # ' '
or        r11, r11, r0
b         loc_5F72E0

loc_5F785C: 
clrldi    r9, r30, 32
addi      r30, r30, 4
lwz       r0, 0(r9)
sld       r0, r0, r8
addi      r8, r8, 0x20 # ' '
or        r11, r11, r0
b         loc_5F739C

loc_5F7878: 
li        r0, 0xD
li        r9, 0xC
stb       r0, var_74(r1)
stb       r9, var_73(r1)
b         loc_5F7550

loc_5F788C: 
li        r0, 0xE
li        r9, 0xF
stb       r0, var_72(r1)
stb       r9, var_71(r1)
b         loc_5F758C

loc_5F78A0: 
li        r0, 1
li        r9, 0
stb       r0, 0(r6)
stb       r9, 1(r6)
b         loc_5F767C

loc_5F78B4: 
mr        r11, r29
li        r0, 0xE
li        r9, 0xF
stbu      r0, 0xE(r11)
stb       r9, 1(r11)
b         loc_5F7270

loc_5F78CC: 
mr        r11, r29
li        r0, 9
li        r9, 8
stbu      r0, 8(r11)
stb       r9, 1(r11)
b         loc_5F777C

loc_5F78E4: 
mr        r11, r29
li        r0, 7
li        r9, 6
stbu      r0, 6(r11)
stb       r9, 1(r11)
b         loc_5F773C

loc_5F78FC: 
mr        r11, r29
li        r0, 5
li        r9, 4
stbu      r0, 4(r11)
stb       r9, 1(r11)
b         loc_5F76FC

loc_5F7914: 
mr        r11, r29
li        r0, 3
li        r9, 2
stbu      r0, 2(r11)
stb       r9, 1(r11)
b         loc_5F76BC

loc_5F792C: 
li        r0, 0xB
li        r9, 0xA
stb       r0, var_76(r1)
stb       r9, var_75(r1)
b         loc_5F7514

loc_5F7940: 
li        r0, 9
li        r9, 8
stb       r0, var_78(r1)
stb       r9, var_77(r1)
b         loc_5F74D8

loc_5F7954: 
li        r0, 7
li        r9, 6
stb       r0, var_7A(r1)
stb       r9, var_79(r1)
b         loc_5F749C

loc_5F7968: 
li        r0, 5
li        r9, 4
stb       r0, var_7C(r1)
stb       r9, var_7B(r1)
b         loc_5F7460

loc_5F797C: 
li        r0, 3
li        r9, 2
stb       r0, var_7E(r1)
stb       r9, var_7D(r1)
b         loc_5F7424

loc_5F7990: 
li        r0, 1
li        r9, 0
stb       r0, var_80(r1)
stb       r9, var_7F(r1)
b         loc_5F73E8

loc_5F79A4: 
mr        r11, r29
li        r0, 0xD
li        r9, 0xC
stbu      r0, 0xC(r11)
stb       r9, 1(r11)
b         loc_5F77FC

loc_5F79BC: 
mr        r11, r29
li        r0, 0xB
li        r9, 0xA
stbu      r0, 0xA(r11)
stb       r9, 1(r11)
b         loc_5F77BC

loc_5F79D4: 
clrldi    r31, r31, 32
mtctr     r31
nop

loc_5F79E0: 
clrldi    r9, r8, 32
clrldi    r11, r10, 32
addi      r8, r8, 1
addi      r10, r10, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
bdnz      loc_5F79E0

loc_5F79FC: 
cmplw     cr7, r27, r4
srw       r26, r28, r4
subf      r28, r4, r27
blt       cr7, loc_5F7D8C

loc_5F7A0C: 
cmplwi    cr7, r28, 2
clrlwi    r8, r26, 29
ble       cr7, loc_5F7D74

loc_5F7A18: 
addi      r24, r1, var_8C
addi      r5, r1, var_7C
mr        r10, r24
addi      r3, r1, var_7A
li        r31, 2
li        r12, 2
li        r4, 0
b         loc_5F7A60

loc_5F7A38: 
lbz       r0, 0(r7)
addi      r10, r10, 1
addi      r3, r3, 1
addi      r12, r12, -1
stb       r0, 0(r6)

loc_5F7A4C: 
cmpwi     cr7, r31, 0
srwi      r8, r8, 1
cmpwi     cr6, r12, 0
beq       cr7, loc_5F80F8
beq       cr6, loc_5F7A94

loc_5F7A60: 
clrlwi    r0, r8, 31
clrldi    r7, r3, 32
cmpwi     cr7, r0, 0
clrldi    r11, r5, 32
clrldi    r6, r10, 32
addi      r4, r4, 1
bne       cr7, loc_5F7A38
lbz       r0, 0(r11)
addi      r31, r31, -1
addi      r10, r10, 1
addi      r5, r5, 1
stb       r0, 0(r6)
b         loc_5F7A4C

loc_5F7A94: 
clrldi    r31, r31, 32
mtctr     r31
nop

loc_5F7AA0: 
clrldi    r9, r5, 32
clrldi    r11, r10, 32
addi      r5, r5, 1
addi      r10, r10, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
bdnz      loc_5F7AA0

loc_5F7ABC: 
cmplw     cr7, r28, r4
srw       r26, r26, r4
subf      r27, r4, r28
blt       cr7, loc_5F7D58

loc_5F7ACC: 
cmplwi    cr7, r27, 2
clrlwi    r7, r26, 29
ble       cr7, loc_5F7D40

loc_5F7AD8: 
addi      r20, r1, var_88
addi      r21, r1, var_78
mr        r10, r20
mr        r8, r21
addi      r3, r1, var_76
li        r31, 2
li        r12, 2
li        r4, 0
b         loc_5F7B24

loc_5F7AFC: 
lbz       r0, 0(r6)
addi      r10, r10, 1
addi      r3, r3, 1
addi      r12, r12, -1
stb       r0, 0(r5)

loc_5F7B10: 
cmpwi     cr7, r31, 0
srwi      r7, r7, 1
cmpwi     cr6, r12, 0
beq       cr7, loc_5F80C8
beq       cr6, loc_5F7B58

loc_5F7B24: 
clrlwi    r0, r7, 31
clrldi    r6, r3, 32
cmpwi     cr7, r0, 0
clrldi    r11, r8, 32
clrldi    r5, r10, 32
addi      r4, r4, 1
bne       cr7, loc_5F7AFC
lbz       r0, 0(r11)
addi      r31, r31, -1
addi      r10, r10, 1
addi      r8, r8, 1
stb       r0, 0(r5)
b         loc_5F7B10

loc_5F7B58: 
clrldi    r31, r31, 32
mtctr     r31

loc_5F7B60: 
clrldi    r9, r8, 32
clrldi    r11, r10, 32
addi      r8, r8, 1
addi      r10, r10, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
bdnz      loc_5F7B60

loc_5F7B7C: 
cmplw     cr7, r27, r4
srw       r26, r26, r4
subf      r28, r4, r27
blt       cr7, loc_5F7D24

loc_5F7B8C: 
cmplwi    cr7, r28, 2
clrlwi    r8, r26, 29
ble       cr7, loc_5F7D0C

loc_5F7B98: 
addi      r23, r1, var_84
addi      r5, r1, var_74
mr        r10, r23
addi      r3, r1, var_72
li        r31, 2
li        r12, 2
li        r4, 0
b         loc_5F7BE0

loc_5F7BB8: 
lbz       r0, 0(r7)
addi      r10, r10, 1
addi      r3, r3, 1
addi      r12, r12, -1
stb       r0, 0(r6)

loc_5F7BCC: 
cmpwi     cr7, r31, 0
srwi      r8, r8, 1
cmpwi     cr6, r12, 0
beq       cr7, loc_5F8098
beq       cr6, loc_5F7C14

loc_5F7BE0: 
clrlwi    r0, r8, 31
clrldi    r7, r3, 32
cmpwi     cr7, r0, 0
clrldi    r11, r5, 32
clrldi    r6, r10, 32
addi      r4, r4, 1
bne       cr7, loc_5F7BB8
lbz       r0, 0(r11)
addi      r31, r31, -1
addi      r10, r10, 1
addi      r5, r5, 1
stb       r0, 0(r6)
b         loc_5F7BCC

loc_5F7C14: 
clrldi    r31, r31, 32
mtctr     r31
nop

loc_5F7C20: 
clrldi    r9, r5, 32
clrldi    r11, r10, 32
addi      r5, r5, 1
addi      r10, r10, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
bdnz      loc_5F7C20

loc_5F7C3C: 
cmplw     cr7, r28, r4
srw       r27, r26, r4
subf      r26, r4, r28
blt       cr7, loc_5F7CF0

loc_5F7C4C: 
cmpwi     cr7, r22, 2
beq       cr7, loc_5F8128
cmplwi    cr7, r26, 6
clrlwi    r7, r27, 25
ble       cr7, loc_5F7DA8

loc_5F7C60: 
mr        r8, r19
mr        r4, r24
mr        r10, r18
li        r12, 4
li        r3, 4
li        r31, 0
b         loc_5F7CA4

loc_5F7C7C: 
lbz       r0, 0(r6)
addi      r10, r10, 1
addi      r4, r4, 1
addi      r3, r3, -1
stb       r0, 0(r5)

loc_5F7C90: 
cmpwi     cr7, r12, 0
srwi      r7, r7, 1
cmpwi     cr6, r3, 0
beq       cr7, loc_5F849C
beq       cr6, loc_5F7DC0

loc_5F7CA4: 
clrlwi    r0, r7, 31
clrldi    r6, r4, 32
cmpwi     cr7, r0, 0
clrldi    r11, r8, 32
clrldi    r5, r10, 32
addi      r31, r31, 1
bne       cr7, loc_5F7C7C
lbz       r0, 0(r11)
addi      r12, r12, -1
addi      r10, r10, 1
addi      r8, r8, 1
stb       r0, 0(r5)
b         loc_5F7C90

loc_5F7CD8: 
clrldi    r9, r30, 32
lwz       r0, 0(r9)
slw       r0, r0, r27
or        r0, r0, r28
clrlwi    r7, r0, 29
b         loc_5F75A0

loc_5F7CF0: 
clrldi    r10, r30, 32
subf      r11, r28, r4
addi      r26, r26, 0x20 # ' '
addi      r30, r30, 4
lwz       r0, 0(r10)
srw       r27, r0, r11
b         loc_5F7C4C

loc_5F7D0C: 
clrldi    r9, r30, 32
lwz       r0, 0(r9)
slw       r0, r0, r28
or        r0, r26, r0
clrlwi    r8, r0, 29
b         loc_5F7B98

loc_5F7D24: 
clrldi    r10, r30, 32
subf      r11, r27, r4
addi      r28, r28, 0x20 # ' '
addi      r30, r30, 4
lwz       r0, 0(r10)
srw       r26, r0, r11
b         loc_5F7B8C

loc_5F7D40: 
clrldi    r9, r30, 32
lwz       r0, 0(r9)
slw       r0, r0, r27
or        r0, r26, r0
clrlwi    r7, r0, 29
b         loc_5F7AD8

loc_5F7D58: 
clrldi    r10, r30, 32
subf      r11, r28, r4
addi      r27, r27, 0x20 # ' '
addi      r30, r30, 4
lwz       r0, 0(r10)
srw       r26, r0, r11
b         loc_5F7ACC

loc_5F7D74: 
clrldi    r9, r30, 32
lwz       r0, 0(r9)
slw       r0, r0, r28
or        r0, r26, r0
clrlwi    r8, r0, 29
b         loc_5F7A18

loc_5F7D8C: 
clrldi    r10, r30, 32
subf      r11, r27, r4
addi      r28, r28, 0x20 # ' '
addi      r30, r30, 4
lwz       r0, 0(r10)
srw       r26, r0, r11
b         loc_5F7A0C

loc_5F7DA8: 
clrldi    r9, r30, 32
lwz       r0, 0(r9)
slw       r0, r0, r26
or        r0, r27, r0
clrlwi    r7, r0, 25
b         loc_5F7C60

loc_5F7DC0: 
clrldi    r12, r12, 32
mtctr     r12

loc_5F7DC8: 
clrldi    r9, r8, 32
clrldi    r11, r10, 32
addi      r8, r8, 1
addi      r10, r10, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
bdnz      loc_5F7DC8

loc_5F7DE4: 
cmplw     cr7, r26, r31
srw       r28, r27, r31
subf      r27, r31, r26
bge       cr7, loc_5F7E0C
clrldi    r10, r30, 32
subf      r11, r26, r31
addi      r27, r27, 0x20 # ' '
addi      r30, r30, 4
lwz       r0, 0(r10)
srw       r28, r0, r11

loc_5F7E0C: 
cmplwi    cr7, r27, 6
clrlwi    r7, r28, 25
bgt       cr7, loc_5F7E2C
clrldi    r9, r30, 32
lwz       r0, 0(r9)
slw       r0, r0, r27
or        r0, r28, r0
clrlwi    r7, r0, 25

loc_5F7E2C: 
mr        r8, r20
mr        r4, r23
li        r12, 4
li        r3, 4
li        r31, 0
mr        r10, r21
b         loc_5F7E70

loc_5F7E48: 
lbz       r0, 0(r6)
addi      r10, r10, 1
addi      r4, r4, 1
addi      r3, r3, -1
stb       r0, 0(r5)

loc_5F7E5C: 
cmpwi     cr7, r12, 0
srwi      r7, r7, 1
cmpwi     cr6, r3, 0
beq       cr7, loc_5F84F8
beq       cr6, loc_5F7EA4

loc_5F7E70: 
clrlwi    r0, r7, 31
clrldi    r6, r4, 32
cmpwi     cr7, r0, 0
clrldi    r11, r8, 32
clrldi    r5, r10, 32
addi      r31, r31, 1
bne       cr7, loc_5F7E48
lbz       r0, 0(r11)
addi      r12, r12, -1
addi      r10, r10, 1
addi      r8, r8, 1
stb       r0, 0(r5)
b         loc_5F7E5C

loc_5F7EA4: 
clrldi    r12, r12, 32
mtctr     r12
nop

loc_5F7EB0: 
clrldi    r9, r8, 32
clrldi    r11, r10, 32
addi      r8, r8, 1
addi      r10, r10, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
bdnz      loc_5F7EB0

loc_5F7ECC: 
cmplw     cr7, r27, r31
srw       r3, r28, r31
subf      r28, r31, r27
bge       cr7, loc_5F7EF4
clrldi    r10, r30, 32
subf      r11, r27, r31
addi      r28, r28, 0x20 # ' '
addi      r30, r30, 4
lwz       r0, 0(r10)
srw       r3, r0, r11

loc_5F7EF4: 
cmplwi    cr7, r28, 0xE
clrlwi    r7, r3, 17
bgt       cr7, loc_5F7F14
clrldi    r9, r30, 32
lwz       r0, 0(r9)
slw       r0, r0, r28
or        r0, r3, r0
clrlwi    r7, r0, 17

loc_5F7F14: 
mr        r11, r29
mr        r10, r18
mr        r6, r21
li        r4, 8
li        r31, 8
li        r5, 0
b         loc_5F7F58

loc_5F7F30: 
lbz       r0, 0(r8)
addi      r6, r6, 1
addi      r31, r31, -1
stb       r0, 0(r11)
addi      r11, r11, 1

loc_5F7F44: 
cmpwi     cr7, r4, 0
srwi      r7, r7, 1
cmpwi     cr6, r31, 0
beq       cr7, loc_5F84CC
beq       cr6, loc_5F7F88

loc_5F7F58: 
clrlwi    r0, r7, 31
clrldi    r8, r6, 32
cmpwi     cr7, r0, 0
clrldi    r9, r10, 32
addi      r5, r5, 1
bne       cr7, loc_5F7F30
lbz       r0, 0(r9)
addi      r4, r4, -1
addi      r10, r10, 1
stb       r0, 0(r11)
addi      r11, r11, 1
b         loc_5F7F44

loc_5F7F88: 
clrldi    r4, r4, 32
mtctr     r4

loc_5F7F90: 
clrldi    r9, r10, 32
addi      r10, r10, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
addi      r11, r11, 1
bdnz      loc_5F7F90

loc_5F7FA8: 
cmplw     cr7, r28, r5
srw       r4, r3, r5
subf      r7, r5, r28
bge       cr7, loc_5F7270
clrldi    r10, r30, 32
subf      r11, r28, r5
addi      r7, r7, 0x20 # ' '
addi      r30, r30, 4
lwz       r0, 0(r10)
srw       r4, r0, r11
b         loc_5F7270

loc_5F7FD4: 
cmplwi    cr7, r27, 2
clrlwi    r7, r28, 29
bgt       cr7, loc_5F7FF4
clrldi    r9, r30, 32
lwz       r0, 0(r9)
slw       r0, r0, r27
or        r0, r0, r28
clrlwi    r7, r0, 29

loc_5F7FF4: 
mr        r10, r29
addi      r6, r1, var_80
addi      r31, r1, var_7E
li        r5, 2
li        r4, 2
li        r11, 0
b         loc_5F8038

loc_5F8010: 
lbz       r0, 0(r8)
addi      r31, r31, 1
addi      r4, r4, -1
stb       r0, 0(r10)
addi      r10, r10, 1

loc_5F8024: 
cmpwi     cr7, r5, 0
srwi      r7, r7, 1
cmpwi     cr6, r4, 0
beq       cr7, loc_5F867C
beq       cr6, loc_5F81BC

loc_5F8038: 
clrlwi    r0, r7, 31
clrldi    r8, r31, 32
cmpwi     cr7, r0, 0
clrldi    r9, r6, 32
addi      r11, r11, 1
bne       cr7, loc_5F8010
lbz       r0, 0(r9)
addi      r5, r5, -1
addi      r6, r6, 1
stb       r0, 0(r10)
addi      r10, r10, 1
b         loc_5F8024

loc_5F8068: 
cmpwi     cr7, r12, 0
beq       cr7, loc_5F79FC
clrldi    r12, r12, 32
mtctr     r12

loc_5F8078: 
clrldi    r9, r3, 32
clrldi    r11, r10, 32
addi      r3, r3, 1
addi      r10, r10, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
bdnz      loc_5F8078
b         loc_5F79FC

loc_5F8098: 
cmpwi     cr7, r12, 0
beq       cr7, loc_5F7C3C
clrldi    r12, r12, 32
mtctr     r12

loc_5F80A8: 
clrldi    r9, r3, 32
clrldi    r11, r10, 32
addi      r3, r3, 1
addi      r10, r10, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
bdnz      loc_5F80A8
b         loc_5F7C3C

loc_5F80C8: 
cmpwi     cr7, r12, 0
beq       cr7, loc_5F7B7C
clrldi    r12, r12, 32
mtctr     r12

loc_5F80D8: 
clrldi    r9, r3, 32
clrldi    r11, r10, 32
addi      r3, r3, 1
addi      r10, r10, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
bdnz      loc_5F80D8
b         loc_5F7B7C

loc_5F80F8: 
cmpwi     cr7, r12, 0
beq       cr7, loc_5F7ABC
clrldi    r12, r12, 32
mtctr     r12

loc_5F8108: 
clrldi    r9, r3, 32
clrldi    r11, r10, 32
addi      r3, r3, 1
addi      r10, r10, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
bdnz      loc_5F8108
b         loc_5F7ABC

loc_5F8128: 
cmplwi    cr7, r26, 6
clrlwi    r6, r27, 25
bgt       cr7, loc_5F8148
clrldi    r9, r30, 32
lwz       r0, 0(r9)
slw       r0, r0, r26
or        r0, r27, r0
clrlwi    r6, r0, 25

loc_5F8148: 
mr        r8, r19
mr        r5, r24
mr        r10, r29
li        r4, 4
li        r31, 4
li        r11, 0
b         loc_5F818C

loc_5F8164: 
lbz       r0, 0(r7)
addi      r5, r5, 1
addi      r31, r31, -1
stb       r0, 0(r10)
addi      r10, r10, 1

loc_5F8178: 
cmpwi     cr7, r4, 0
srwi      r6, r6, 1
cmpwi     cr6, r31, 0
beq       cr7, loc_5F8700
beq       cr6, loc_5F8528

loc_5F818C: 
clrlwi    r0, r6, 31
clrldi    r7, r5, 32
cmpwi     cr7, r0, 0
clrldi    r9, r8, 32
addi      r11, r11, 1
bne       cr7, loc_5F8164
lbz       r0, 0(r9)
addi      r4, r4, -1
addi      r8, r8, 1
stb       r0, 0(r10)
addi      r10, r10, 1
b         loc_5F8178

loc_5F81BC: 
clrldi    r5, r5, 32
mtctr     r5

loc_5F81C4: 
clrldi    r9, r6, 32
addi      r6, r6, 1
lbz       r0, 0(r9)
stb       r0, 0(r10)
addi      r10, r10, 1
bdnz      loc_5F81C4

loc_5F81DC: 
cmplw     cr7, r27, r11
srw       r28, r28, r11
subf      r3, r11, r27
bge       cr7, loc_5F8204
clrldi    r10, r30, 32
subf      r11, r27, r11
addi      r3, r3, 0x20 # ' '
addi      r30, r30, 4
lwz       r0, 0(r10)
srw       r28, r0, r11

loc_5F8204: 
cmplwi    cr7, r3, 2
clrlwi    r7, r28, 29
bgt       cr7, loc_5F8224
clrldi    r9, r30, 32
lwz       r0, 0(r9)
slw       r0, r0, r3
or        r0, r28, r0
clrlwi    r7, r0, 29

loc_5F8224: 
addi      r10, r29, 4
addi      r6, r1, var_7C
addi      r31, r1, var_7A
li        r5, 2
li        r4, 2
li        r11, 0
b         loc_5F8268

loc_5F8240: 
lbz       r0, 0(r8)
addi      r31, r31, 1
addi      r4, r4, -1
stb       r0, 0(r10)
addi      r10, r10, 1

loc_5F8254: 
cmpwi     cr7, r5, 0
srwi      r7, r7, 1
cmpwi     cr6, r4, 0
beq       cr7, loc_5F86A8
beq       cr6, loc_5F8298

loc_5F8268: 
clrlwi    r0, r7, 31
clrldi    r8, r31, 32
cmpwi     cr7, r0, 0
clrldi    r9, r6, 32
addi      r11, r11, 1
bne       cr7, loc_5F8240
lbz       r0, 0(r9)
addi      r5, r5, -1
addi      r6, r6, 1
stb       r0, 0(r10)
addi      r10, r10, 1
b         loc_5F8254

loc_5F8298: 
clrldi    r5, r5, 32
mtctr     r5

loc_5F82A0: 
clrldi    r9, r6, 32
addi      r6, r6, 1
lbz       r0, 0(r9)
stb       r0, 0(r10)
addi      r10, r10, 1
bdnz      loc_5F82A0

loc_5F82B8: 
cmplw     cr7, r3, r11
srw       r27, r28, r11
subf      r28, r11, r3
bge       cr7, loc_5F82E0
clrldi    r10, r30, 32
subf      r11, r3, r11
addi      r28, r28, 0x20 # ' '
addi      r30, r30, 4
lwz       r0, 0(r10)
srw       r27, r0, r11

loc_5F82E0: 
cmplwi    cr7, r28, 2
clrlwi    r7, r27, 29
bgt       cr7, loc_5F8300
clrldi    r9, r30, 32
lwz       r0, 0(r9)
slw       r0, r0, r28
or        r0, r27, r0
clrlwi    r7, r0, 29

loc_5F8300: 
addi      r10, r29, 8
addi      r6, r1, var_78
addi      r31, r1, var_76
li        r5, 2
li        r4, 2
li        r11, 0
b         loc_5F8344

loc_5F831C: 
lbz       r0, 0(r8)
addi      r31, r31, 1
addi      r4, r4, -1
stb       r0, 0(r10)
addi      r10, r10, 1

loc_5F8330: 
cmpwi     cr7, r5, 0
srwi      r7, r7, 1
cmpwi     cr6, r4, 0
beq       cr7, loc_5F86D4
beq       cr6, loc_5F8374

loc_5F8344: 
clrlwi    r0, r7, 31
clrldi    r8, r31, 32
cmpwi     cr7, r0, 0
clrldi    r9, r6, 32
addi      r11, r11, 1
bne       cr7, loc_5F831C
lbz       r0, 0(r9)
addi      r5, r5, -1
addi      r6, r6, 1
stb       r0, 0(r10)
addi      r10, r10, 1
b         loc_5F8330

loc_5F8374: 
clrldi    r5, r5, 32
mtctr     r5

loc_5F837C: 
clrldi    r9, r6, 32
addi      r6, r6, 1
lbz       r0, 0(r9)
stb       r0, 0(r10)
addi      r10, r10, 1
bdnz      loc_5F837C

loc_5F8394: 
cmplw     cr7, r28, r11
srw       r27, r27, r11
subf      r3, r11, r28
bge       cr7, loc_5F83BC
clrldi    r10, r30, 32
subf      r11, r28, r11
addi      r3, r3, 0x20 # ' '
addi      r30, r30, 4
lwz       r0, 0(r10)
srw       r27, r0, r11

loc_5F83BC: 
cmplwi    cr7, r3, 2
clrlwi    r8, r27, 29
bgt       cr7, loc_5F83DC
clrldi    r9, r30, 32
lwz       r0, 0(r9)
slw       r0, r0, r3
or        r0, r27, r0
clrlwi    r8, r0, 29

loc_5F83DC: 
addi      r11, r29, 0xC
addi      r7, r1, var_74
addi      r31, r1, var_72
li        r5, 2
li        r4, 2
li        r6, 0
b         loc_5F8420

loc_5F83F8: 
lbz       r0, 0(r10)
addi      r31, r31, 1
addi      r4, r4, -1
stb       r0, 0(r11)
addi      r11, r11, 1

loc_5F840C: 
cmpwi     cr7, r5, 0
srwi      r8, r8, 1
cmpwi     cr6, r4, 0
beq       cr7, loc_5F8650
beq       cr6, loc_5F8450

loc_5F8420: 
clrlwi    r0, r8, 31
clrldi    r10, r31, 32
cmpwi     cr7, r0, 0
clrldi    r9, r7, 32
addi      r6, r6, 1
bne       cr7, loc_5F83F8
lbz       r0, 0(r9)
addi      r5, r5, -1
addi      r7, r7, 1
stb       r0, 0(r11)
addi      r11, r11, 1
b         loc_5F840C

loc_5F8450: 
clrldi    r5, r5, 32
mtctr     r5

loc_5F8458: 
clrldi    r9, r7, 32
addi      r7, r7, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
addi      r11, r11, 1
bdnz      loc_5F8458

loc_5F8470: 
cmplw     cr7, r3, r6
srw       r4, r27, r6
subf      r7, r6, r3
bge       cr7, loc_5F7270
clrldi    r10, r30, 32
subf      r11, r3, r6
addi      r7, r7, 0x20 # ' '
addi      r30, r30, 4
lwz       r0, 0(r10)
srw       r4, r0, r11
b         loc_5F7270

loc_5F849C: 
cmpwi     cr7, r3, 0
beq       cr7, loc_5F7DE4
clrldi    r3, r3, 32
mtctr     r3

loc_5F84AC: 
clrldi    r9, r4, 32
clrldi    r11, r10, 32
addi      r4, r4, 1
addi      r10, r10, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
bdnz      loc_5F84AC
b         loc_5F7DE4

loc_5F84CC: 
cmpwi     cr7, r31, 0
beq       cr7, loc_5F7FA8
clrldi    r31, r31, 32
mtctr     r31

loc_5F84DC: 
clrldi    r9, r6, 32
addi      r6, r6, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
addi      r11, r11, 1
bdnz      loc_5F84DC
b         loc_5F7FA8

loc_5F84F8: 
cmpwi     cr7, r3, 0
beq       cr7, loc_5F7ECC
clrldi    r3, r3, 32
mtctr     r3

loc_5F8508: 
clrldi    r9, r4, 32
clrldi    r11, r10, 32
addi      r4, r4, 1
addi      r10, r10, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
bdnz      loc_5F8508
b         loc_5F7ECC

loc_5F8528: 
clrldi    r4, r4, 32
mtctr     r4

loc_5F8530: 
clrldi    r9, r8, 32
addi      r8, r8, 1
lbz       r0, 0(r9)
stb       r0, 0(r10)
addi      r10, r10, 1
bdnz      loc_5F8530

loc_5F8548: 
cmplw     cr7, r26, r11
srw       r28, r27, r11
subf      r3, r11, r26
bge       cr7, loc_5F8570
clrldi    r10, r30, 32
subf      r11, r26, r11
addi      r3, r3, 0x20 # ' '
addi      r30, r30, 4
lwz       r0, 0(r10)
srw       r28, r0, r11

loc_5F8570: 
cmplwi    cr7, r3, 6
clrlwi    r7, r28, 25
bgt       cr7, loc_5F8590
clrldi    r9, r30, 32
lwz       r0, 0(r9)
slw       r0, r0, r3
or        r0, r28, r0
clrlwi    r7, r0, 25

loc_5F8590: 
addi      r11, r29, 8
mr        r10, r20
mr        r6, r23
li        r4, 4
li        r31, 4
li        r5, 0
b         loc_5F85D4

loc_5F85AC: 
lbz       r0, 0(r8)
addi      r6, r6, 1
addi      r31, r31, -1
stb       r0, 0(r11)
addi      r11, r11, 1

loc_5F85C0: 
cmpwi     cr7, r4, 0
srwi      r7, r7, 1
cmpwi     cr6, r31, 0
beq       cr7, loc_5F872C
beq       cr6, loc_5F8604

loc_5F85D4: 
clrlwi    r0, r7, 31
clrldi    r8, r6, 32
cmpwi     cr7, r0, 0
clrldi    r9, r10, 32
addi      r5, r5, 1
bne       cr7, loc_5F85AC
lbz       r0, 0(r9)
addi      r4, r4, -1
addi      r10, r10, 1
stb       r0, 0(r11)
addi      r11, r11, 1
b         loc_5F85C0

loc_5F8604: 
clrldi    r4, r4, 32
mtctr     r4

loc_5F860C: 
clrldi    r9, r10, 32
addi      r10, r10, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
addi      r11, r11, 1
bdnz      loc_5F860C

loc_5F8624: 
cmplw     cr7, r3, r5
srw       r4, r28, r5
subf      r7, r5, r3
bge       cr7, loc_5F7270
clrldi    r10, r30, 32
subf      r11, r3, r5
addi      r7, r7, 0x20 # ' '
addi      r30, r30, 4
lwz       r0, 0(r10)
srw       r4, r0, r11
b         loc_5F7270

loc_5F8650: 
cmpwi     cr7, r4, 0
beq       cr7, loc_5F8470
clrldi    r4, r4, 32
mtctr     r4

loc_5F8660: 
clrldi    r9, r31, 32
addi      r31, r31, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
addi      r11, r11, 1
bdnz      loc_5F8660
b         loc_5F8470

loc_5F867C: 
cmpwi     cr7, r4, 0
beq       cr7, loc_5F81DC
clrldi    r4, r4, 32
mtctr     r4

loc_5F868C: 
clrldi    r9, r31, 32
addi      r31, r31, 1
lbz       r0, 0(r9)
stb       r0, 0(r10)
addi      r10, r10, 1
bdnz      loc_5F868C
b         loc_5F81DC

loc_5F86A8: 
cmpwi     cr7, r4, 0
beq       cr7, loc_5F82B8
clrldi    r4, r4, 32
mtctr     r4

loc_5F86B8: 
clrldi    r9, r31, 32
addi      r31, r31, 1
lbz       r0, 0(r9)
stb       r0, 0(r10)
addi      r10, r10, 1
bdnz      loc_5F86B8
b         loc_5F82B8

loc_5F86D4: 
cmpwi     cr7, r4, 0
beq       cr7, loc_5F8394
clrldi    r4, r4, 32
mtctr     r4

loc_5F86E4: 
clrldi    r9, r31, 32
addi      r31, r31, 1
lbz       r0, 0(r9)
stb       r0, 0(r10)
addi      r10, r10, 1
bdnz      loc_5F86E4
b         loc_5F8394

loc_5F8700: 
cmpwi     cr7, r31, 0
beq       cr7, loc_5F8548
clrldi    r31, r31, 32
mtctr     r31

loc_5F8710: 
clrldi    r9, r5, 32
addi      r5, r5, 1
lbz       r0, 0(r9)
stb       r0, 0(r10)
addi      r10, r10, 1
bdnz      loc_5F8710
b         loc_5F8548

loc_5F872C: 
cmpwi     cr7, r31, 0
beq       cr7, loc_5F8624
clrldi    r31, r31, 32
mtctr     r31

loc_5F873C: 
clrldi    r9, r6, 32
addi      r6, r6, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
addi      r11, r11, 1
bdnz      loc_5F873C
b         loc_5F8624
