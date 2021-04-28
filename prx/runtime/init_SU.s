.global _shk_prx_elf_substitute_impl
_shk_prx_elf_substitute_impl:
.int ._shk_prx_elf_substitute_impl
.int _shk_elf_toc

# function extracted from the game to make extra room for code
# sub_1D3B2C8
.global ._shk_prx_elf_substitute_impl
._shk_prx_elf_substitute_impl:
 .set back_chain, -0x28
 .set var_20, -0x20
 .set var_18, -0x18
 .set var_10, -0x10
 .set var_8, -8

cmpw      cr7, r3, r4
std       r27, back_chain(r1)
std       r31, var_8(r1)
std       r28, var_20(r1)
std       r29, var_18(r1)
std       r30, var_10(r1)
mr        r27, r3
mr        r31, r4
mr        r8, r5
beq       cr7, loc_1D3B4A4
cmplw     cr7, r3, r4
ble       cr7, loc_1D3B4C0
add       r12, r4, r5
cmplw     cr7, r3, r12
bge       cr7, loc_1D3B4C0
cmplwi    cr7, r5, 0x20 # ' '
mr        r30, r5
add       r31, r3, r5
ble       cr7, loc_1D3B3EC
clrlwi    r9, r31, 29
clrlwi    r0, r12, 29
cmplw     cr7, r9, r0
subf      r11, r0, r9
bgt       cr7, loc_1D3B32C
subf      r11, r9, r0

loc_1D3B32C:
or        r9, r9, r0
cmpwi     cr7, r9, 0
bne       cr7, loc_1D3B8BC
addi      r9, r8, -0x20
mr        r30, r8
extrdi    r9, r9, 27,32
addi      r9, r9, 1
mtctr     r9

loc_1D3B34C:
addi      r8, r12, -8
addi      r7, r12, -0x10
addi      r6, r12, -0x18
addi      r12, r12, -0x20
clrldi    r8, r8, 32
clrldi    r7, r7, 32
addi      r9, r31, -8
addi      r11, r31, -0x10
addi      r10, r31, -0x18
clrldi    r6, r6, 32
ld        r0, 0(r8)
addi      r31, r31, -0x20
ld        r5, 0(r7)
clrldi    r4, r12, 32
clrldi    r9, r9, 32
clrldi    r11, r11, 32
clrldi    r10, r10, 32
ld        r8, 0(r6)
clrldi    r3, r31, 32
ld        r7, 0(r4)
addi      r30, r30, -0x20
std       r0, 0(r9)
std       r5, 0(r11)
std       r8, 0(r10)
std       r7, 0(r3)
bdnz      loc_1D3B34C
cmplwi    cr7, r30, 7
ble       cr7, loc_1D3B3EC
addi      r9, r30, -8
extrdi    r9, r9, 29,32
addi      r9, r9, 1
mtctr     r9

loc_1D3B3CC:
addi      r12, r12, -8
addi      r31, r31, -8
clrldi    r9, r12, 32
clrldi    r11, r31, 32
addi      r30, r30, -8
ld        r0, 0(r9)
std       r0, 0(r11)
bdnz      loc_1D3B3CC

loc_1D3B3EC:

cmplwi    cr7, r30, 3
ble       cr7, loc_1D3B470

loc_1D3B3F4:
addi      r9, r30, -4
extrdi    r9, r9, 30,32
addi      r9, r9, 1
mtctr     r9
nop

loc_1D3B408:
addi      r8, r12, -1
addi      r7, r12, -2
addi      r6, r12, -3
addi      r12, r12, -4
clrldi    r8, r8, 32
clrldi    r7, r7, 32
addi      r9, r31, -1
addi      r11, r31, -2
addi      r10, r31, -3
clrldi    r6, r6, 32
lbz       r0, 0(r8)
addi      r31, r31, -4
lbz       r5, 0(r7)
clrldi    r4, r12, 32
clrldi    r9, r9, 32
clrldi    r11, r11, 32
clrldi    r10, r10, 32
lbz       r8, 0(r6)
clrldi    r3, r31, 32
lbz       r7, 0(r4)
addi      r30, r30, -4
stb       r0, 0(r9)
stb       r5, 0(r11)
stb       r8, 0(r10)
stb       r7, 0(r3)
bdnz      loc_1D3B408

loc_1D3B470:
cmpwi     cr7, r30, 0
beq       cr7, loc_1D3B4A4
addi      r9, r30, -1
clrldi    r9, r9, 32
addi      r9, r9, 1
mtctr     r9

loc_1D3B488:
addi      r12, r12, -1
addi      r31, r31, -1
clrldi    r9, r12, 32
clrldi    r11, r31, 32
lbz       r0, 0(r9)
stb       r0, 0(r11)
bdnz      loc_1D3B488

loc_1D3B4A4:

clrldi    r3, r27, 32
ld        r28, var_20(r1)
ld        r27, back_chain(r1)
ld        r29, var_18(r1)
ld        r30, var_10(r1)
ld        r31, var_8(r1)
blr
 # ---------------------------------------------------------------------------

loc_1D3B4C0:

cmplwi    cr7, r8, 0x20 # ' '
mr        r5, r8
bgt       cr7, loc_1D3B580

loc_1D3B4CC:

cmplwi    cr7, r5, 3
ble       cr7, loc_1D3B52C

loc_1D3B4D4:
addi      r9, r5, -4
li        r6, 0
extrdi    r9, r9, 30,32
addi      r9, r9, 1
mtctr     r9

loc_1D3B4E8:
add       r9, r6, r4
add       r11, r6, r3
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -4
addi      r6, r6, 4
lbz       r10, 0(r9)
lbz       r8, 1(r9)
lbz       r7, 2(r9)
lbz       r0, 3(r9)
stb       r10, 0(r11)
stb       r0, 3(r11)
stb       r8, 1(r11)
stb       r7, 2(r11)
bdnz      loc_1D3B4E8
add       r3, r3, r6
add       r4, r4, r6

loc_1D3B52C:
cmpwi     cr7, r5, 0
beq       cr7, loc_1D3B4A4
addi      r9, r5, -1
clrldi    r9, r9, 32
addi      r9, r9, 1
mtctr     r9
nop

loc_1D3B548:
clrldi    r9, r4, 32
clrldi    r11, r3, 32
addi      r4, r4, 1
addi      r3, r3, 1
lbz       r0, 0(r9)
stb       r0, 0(r11)
bdnz      loc_1D3B548
clrldi    r3, r27, 32
ld        r28, var_20(r1)
ld        r27, back_chain(r1)
ld        r29, var_18(r1)
ld        r30, var_10(r1)
ld        r31, var_8(r1)
blr
 # ---------------------------------------------------------------------------

loc_1D3B580:
clrlwi    r9, r27, 29
clrlwi    r0, r31, 29
cmplw     cr7, r9, r0
subf      r11, r0, r9
ble       cr7, loc_1D3B7B0
or        r9, r9, r0
cmpwi     cr7, r9, 0
beq       cr7, loc_1D3B67C

loc_1D3B5A0:
clrlwi    r0, r9, 30
cmpwi     cr7, r0, 0
bne       cr7, loc_1D3B7C4
addi      r9, r8, -0x10
cmpwi     cr7, r11, 0
extrdi    r9, r9, 28,32
mr        r5, r8
addi      r9, r9, 1
li        r6, 0
mtctr     r9
bne       cr7, loc_1D3B728
addi      r5, r8, -4
lwz       r0, 0(r31)
addi      r4, r31, 4
cmplwi    cr7, r5, 0x1F
stw       r0, 0(r27)
addi      r3, r27, 4
ble       cr7, loc_1D3B630
addi      r9, r8, -0x24
extrdi    r9, r9, 27,32
addi      r9, r9, 1
mtctr     r9

loc_1D3B5F8:
clrldi    r9, r4, 32
clrldi    r11, r3, 32
addi      r4, r4, 0x20 # ' '
addi      r3, r3, 0x20 # ' '
addi      r5, r5, -0x20
ld        r0, 0x18(r9)
ld        r10, 0(r9)
ld        r8, 8(r9)
ld        r7, 0x10(r9)
std       r0, 0x18(r11)
std       r10, 0(r11)
std       r8, 8(r11)
std       r7, 0x10(r11)
bdnz      loc_1D3B5F8

loc_1D3B630:
cmplwi    cr7, r5, 7
ble       cr7, loc_1D3B4CC
addi      r9, r5, -8
li        r10, 0
extrdi    r9, r9, 29,32
addi      r9, r9, 1
mtctr     r9

loc_1D3B64C:
add       r9, r10, r4
add       r11, r10, r3
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -8
addi      r10, r10, 8
ld        r0, 0(r9)
std       r0, 0(r11)
bdnz      loc_1D3B64C

loc_1D3B670:

add       r4, r4, r10
add       r3, r3, r10
b         loc_1D3B4CC
 # ---------------------------------------------------------------------------

loc_1D3B67C:

addi      r9, r8, -0x20
mr        r5, r8
extrdi    r9, r9, 27,32
li        r6, 0
addi      r9, r9, 1
mtctr     r9
nop

loc_1D3B698:
add       r9, r31, r6
add       r11, r6, r27
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -0x20
addi      r6, r6, 0x20 # ' '
ld        r10, 0(r9)
ld        r8, 8(r9)
ld        r7, 0x10(r9)
ld        r0, 0x18(r9)
std       r10, 0(r11)
std       r0, 0x18(r11)
std       r8, 8(r11)
std       r7, 0x10(r11)
bdnz      loc_1D3B698
cmplwi    cr7, r5, 7
add       r4, r31, r6
add       r3, r27, r6
ble       cr7, loc_1D3B4CC
addi      r9, r5, -8
li        r8, 0
extrdi    r9, r9, 29,32
addi      r9, r9, 1
mtctr     r9

loc_1D3B6F8:
add       r9, r8, r4
add       r11, r8, r3
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -8
addi      r8, r8, 8
ld        r0, 0(r9)
std       r0, 0(r11)
bdnz      loc_1D3B6F8

loc_1D3B71C:

add       r4, r4, r8
add       r3, r3, r8
b         loc_1D3B4CC
 # ---------------------------------------------------------------------------

loc_1D3B728:

add       r9, r31, r6
add       r11, r6, r27
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -0x10
addi      r6, r6, 0x10
lwz       r10, 0(r9)
lwz       r8, 4(r9)
lwz       r7, 8(r9)
lwz       r0, 0xC(r9)
stw       r10, 0(r11)
stw       r0, 0xC(r11)
stw       r8, 4(r11)
stw       r7, 8(r11)
bdnz      loc_1D3B728
cmplwi    cr7, r5, 3
add       r4, r31, r6
add       r3, r6, r27
ble       cr7, loc_1D3B4CC
addi      r9, r5, -4
li        r8, 0
extrdi    r9, r9, 30,32
addi      r9, r9, 1
mtctr     r9

loc_1D3B788:
add       r9, r8, r4
add       r11, r8, r3
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -4
addi      r8, r8, 4
lwz       r0, 0(r9)
stw       r0, 0(r11)
bdnz      loc_1D3B788
b         loc_1D3B71C
 # ---------------------------------------------------------------------------

loc_1D3B7B0:
subf      r11, r9, r0
or        r9, r9, r0
cmpwi     cr7, r9, 0
bne       cr7, loc_1D3B5A0
b         loc_1D3B67C
 # ---------------------------------------------------------------------------

loc_1D3B7C4:
clrlwi    r0, r9, 31
cmpwi     cr7, r0, 0
beq       cr7, loc_1D3B9F8
cmpwi     cr7, r11, 0
bne       cr7, loc_1D3BD3C
clrldi    r9, r4, 32
addi      r30, r3, 1
clrldi    r11, r3, 32
clrlwi    r10, r30, 29
addi      r31, r4, 1
cmpwi     cr7, r10, 4
lbz       r0, 0(r9)
addi      r5, r8, -1
stb       r0, 0(r11)
beq       cr7, loc_1D3BF24
cmpwi     cr7, r10, 6
beq       cr7, loc_1D3C294
cmpwi     cr7, r10, 2
beq       cr7, loc_1D3BF08

loc_1D3B810:

cmplwi    cr7, r5, 0x1F
mr        r3, r30
mr        r4, r31
ble       cr7, loc_1D3B878
addi      r9, r5, -0x20
li        r6, 0
extrdi    r9, r9, 27,32
addi      r9, r9, 1
mtctr     r9

loc_1D3B834:
add       r9, r31, r6
add       r11, r6, r30
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -0x20
addi      r6, r6, 0x20 # ' '
ld        r10, 0(r9)
ld        r8, 8(r9)
ld        r7, 0x10(r9)
ld        r0, 0x18(r9)
std       r10, 0(r11)
std       r0, 0x18(r11)
std       r8, 8(r11)
std       r7, 0x10(r11)
bdnz      loc_1D3B834
add       r3, r30, r6
add       r4, r31, r6

loc_1D3B878:
cmplwi    cr7, r5, 7
ble       cr7, loc_1D3B4CC
addi      r9, r5, -8
li        r8, 0
extrdi    r9, r9, 29,32
addi      r9, r9, 1
mtctr     r9

loc_1D3B894:
add       r9, r8, r4
add       r11, r8, r3
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -8
addi      r8, r8, 8
ld        r0, 0(r9)
std       r0, 0(r11)
bdnz      loc_1D3B894
b         loc_1D3B71C
 # ---------------------------------------------------------------------------

loc_1D3B8BC:
clrlwi    r0, r9, 30
cmpwi     cr7, r0, 0
beq       cr7, loc_1D3BAE0
clrlwi    r0, r9, 31
cmpwi     cr7, r0, 0
bne       cr7, loc_1D3BE00
cmpwi     cr7, r11, 0
mr        r28, r31
mr        r29, r12
bne       cr7, loc_1D3C03C
addi      r4, r12, -2
addi      r3, r31, -2
clrldi    r9, r4, 32
clrlwi    r0, r3, 29
clrldi    r11, r3, 32
cmpwi     cr7, r0, 0
addi      r30, r8, -2
lhz       r0, 0(r9)
sth       r0, 0(r11)
beq       cr7, loc_1D3B930
addi      r9, r12, -6
addi      r0, r31, -6
clrldi    r11, r9, 32
clrldi    r10, r0, 32
mr        r3, r0
addi      r30, r8, -6
mr        r4, r9
lwz       r0, 0(r11)
stw       r0, 0(r10)

loc_1D3B930:
cmplwi    cr7, r30, 0x1F
mr        r31, r3
mr        r29, r4
ble       cr7, loc_1D3B9B8
addi      r9, r30, -0x20
extrdi    r9, r9, 27,32
addi      r9, r9, 1
mtctr     r9

loc_1D3B950:
addi      r8, r29, -8
addi      r7, r29, -0x10
addi      r6, r29, -0x18
addi      r29, r29, -0x20
clrldi    r8, r8, 32
clrldi    r7, r7, 32
addi      r9, r31, -8
addi      r11, r31, -0x10
addi      r10, r31, -0x18
clrldi    r6, r6, 32
ld        r0, 0(r8)
addi      r31, r31, -0x20
ld        r5, 0(r7)
clrldi    r4, r29, 32
clrldi    r9, r9, 32
clrldi    r11, r11, 32
clrldi    r10, r10, 32
ld        r8, 0(r6)
clrldi    r3, r31, 32
ld        r7, 0(r4)
addi      r30, r30, -0x20
std       r0, 0(r9)
std       r5, 0(r11)
std       r8, 0(r10)
std       r7, 0(r3)
bdnz      loc_1D3B950

loc_1D3B9B8:
cmplwi    cr7, r30, 7
ble       cr7, loc_1D3B9F0
addi      r9, r30, -8
extrdi    r9, r9, 29,32
addi      r9, r9, 1
mtctr     r9

loc_1D3B9D0:
addi      r29, r29, -8
addi      r31, r31, -8
clrldi    r9, r29, 32
clrldi    r11, r31, 32
addi      r30, r30, -8
ld        r0, 0(r9)
std       r0, 0(r11)
bdnz      loc_1D3B9D0

loc_1D3B9F0:

mr        r12, r29
b         loc_1D3B3EC
 # ---------------------------------------------------------------------------

loc_1D3B9F8:
cmpwi     cr7, r11, 0
bne       cr7, loc_1D3BC94
addi      r29, r27, 2
lhz       r0, 0(r31)
addi      r30, r31, 2
clrlwi    r9, r29, 29
sth       r0, 0(r27)
cmpwi     cr7, r9, 0
addi      r5, r8, -2
beq       cr7, loc_1D3BA34
lwz       r0, 0(r30)
addi      r5, r8, -6
addi      r30, r31, 6
stw       r0, 0(r29)
addi      r29, r27, 6

loc_1D3BA34:
cmplwi    cr7, r5, 0x1F
mr        r3, r29
mr        r4, r30
ble       cr7, loc_1D3BA9C
addi      r9, r5, -0x20
li        r6, 0
extrdi    r9, r9, 27,32
addi      r9, r9, 1
mtctr     r9

loc_1D3BA58:
add       r9, r6, r30
add       r11, r6, r29
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -0x20
addi      r6, r6, 0x20 # ' '
ld        r10, 0(r9)
ld        r8, 8(r9)
ld        r7, 0x10(r9)
ld        r0, 0x18(r9)
std       r10, 0(r11)
std       r0, 0x18(r11)
std       r8, 8(r11)
std       r7, 0x10(r11)
bdnz      loc_1D3BA58
add       r3, r29, r6
add       r4, r30, r6

loc_1D3BA9C:
cmplwi    cr7, r5, 7
ble       cr7, loc_1D3B4CC
addi      r9, r5, -8
li        r8, 0
extrdi    r9, r9, 29,32
addi      r9, r9, 1
mtctr     r9

loc_1D3BAB8:
add       r9, r8, r4
add       r11, r8, r3
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -8
addi      r8, r8, 8
ld        r0, 0(r9)
std       r0, 0(r11)
bdnz      loc_1D3BAB8
b         loc_1D3B71C
 # ---------------------------------------------------------------------------

loc_1D3BAE0:
addi      r9, r8, -0x10
cmpwi     cr7, r11, 0
extrdi    r9, r9, 28,32
mr        r28, r31
addi      r9, r9, 1
mr        r29, r12
mtctr     r9
mr        r30, r8
bne       cr7, loc_1D3BBE8
addi      r11, r12, -4
addi      r0, r31, -4
clrldi    r9, r11, 32
clrldi    r10, r0, 32
addi      r30, r8, -4
mr        r31, r0
cmplwi    cr7, r30, 0x1F
lwz       r0, 0(r9)
mr        r29, r11
stw       r0, 0(r10)
ble       cr7, loc_1D3BBA8
addi      r9, r8, -0x24
extrdi    r9, r9, 27,32
addi      r9, r9, 1
mtctr     r9

loc_1D3BB40:
addi      r8, r29, -8
addi      r7, r29, -0x10
addi      r6, r29, -0x18
addi      r29, r29, -0x20
clrldi    r8, r8, 32
clrldi    r7, r7, 32
addi      r9, r31, -8
addi      r11, r31, -0x10
addi      r10, r31, -0x18
clrldi    r6, r6, 32
ld        r0, 0(r8)
addi      r31, r31, -0x20
ld        r5, 0(r7)
clrldi    r4, r29, 32
clrldi    r9, r9, 32
clrldi    r11, r11, 32
clrldi    r10, r10, 32
ld        r8, 0(r6)
clrldi    r3, r31, 32
ld        r7, 0(r4)
addi      r30, r30, -0x20
std       r0, 0(r9)
std       r5, 0(r11)
std       r8, 0(r10)
std       r7, 0(r3)
bdnz      loc_1D3BB40

loc_1D3BBA8:
cmplwi    cr7, r30, 7
ble       cr7, loc_1D3B9F0
addi      r9, r30, -8
extrdi    r9, r9, 29,32
addi      r9, r9, 1
mtctr     r9

loc_1D3BBC0:
addi      r29, r29, -8
addi      r31, r31, -8
clrldi    r9, r29, 32
clrldi    r11, r31, 32
addi      r30, r30, -8
ld        r0, 0(r9)
std       r0, 0(r11)
bdnz      loc_1D3BBC0
mr        r12, r29
b         loc_1D3B3EC
 # ---------------------------------------------------------------------------

loc_1D3BBE8:

addi      r8, r29, -4
addi      r7, r29, -8
addi      r6, r29, -0xC
addi      r29, r29, -0x10
clrldi    r8, r8, 32
clrldi    r7, r7, 32
addi      r9, r28, -4
addi      r11, r28, -8
addi      r10, r28, -0xC
clrldi    r6, r6, 32
lwz       r0, 0(r8)
addi      r28, r28, -0x10
lwz       r5, 0(r7)
clrldi    r4, r29, 32
clrldi    r9, r9, 32
clrldi    r11, r11, 32
clrldi    r10, r10, 32
lwz       r8, 0(r6)
clrldi    r3, r28, 32
lwz       r7, 0(r4)
addi      r30, r30, -0x10
stw       r0, 0(r9)
stw       r5, 0(r11)
stw       r8, 0(r10)
stw       r7, 0(r3)
bdnz      loc_1D3BBE8
cmplwi    cr7, r30, 3
ble       cr7, loc_1D3BC88
addi      r9, r30, -4
extrdi    r9, r9, 30,32
addi      r9, r9, 1
mtctr     r9

loc_1D3BC68:
addi      r29, r29, -4
addi      r28, r28, -4
clrldi    r9, r29, 32
clrldi    r11, r28, 32
addi      r30, r30, -4
lwz       r0, 0(r9)
stw       r0, 0(r11)
bdnz      loc_1D3BC68

loc_1D3BC88:

mr        r31, r28
mr        r12, r29
b         loc_1D3B3EC
 # ---------------------------------------------------------------------------

loc_1D3BC94:
addi      r9, r8, -8
cmpwi     cr7, r11, 4
extrdi    r9, r9, 29,32
mr        r5, r8
addi      r9, r9, 1
li        r6, 0
mtctr     r9
beq       cr7, loc_1D3C1EC

loc_1D3BCB4:
add       r9, r31, r6
add       r11, r6, r27
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -8
addi      r6, r6, 8
lhz       r10, 0(r9)
lhz       r8, 2(r9)
lhz       r7, 4(r9)
lhz       r0, 6(r9)
sth       r10, 0(r11)
sth       r0, 6(r11)
sth       r8, 2(r11)
sth       r7, 4(r11)
bdnz      loc_1D3BCB4
cmplwi    cr7, r5, 1
add       r4, r6, r31
add       r3, r6, r27
ble       cr7, loc_1D3B4CC
addi      r9, r5, -2
li        r8, 0
extrdi    r9, r9, 31,32
addi      r9, r9, 1
mtctr     r9

loc_1D3BD14:
add       r9, r8, r4
add       r11, r8, r3
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -2
addi      r8, r8, 2
lhz       r0, 0(r9)
sth       r0, 0(r11)
bdnz      loc_1D3BD14
b         loc_1D3B71C
 # ---------------------------------------------------------------------------

loc_1D3BD3C:
cmpwi     cr7, r11, 4
beq       cr7, loc_1D3C0FC
cmpwi     cr7, r11, 2
mr        r5, r8
bne       cr7, loc_1D3B4D4
clrldi    r9, r4, 32
clrldi    r11, r3, 32
addi      r5, r8, -1
addi      r3, r3, 1
cmplwi    cr7, r5, 7
lbz       r0, 0(r9)
addi      r4, r4, 1
stb       r0, 0(r11)
ble       cr7, loc_1D3BDBC
addi      r9, r8, -9
extrdi    r9, r9, 29,32
addi      r9, r9, 1
mtctr     r9

loc_1D3BD84:
clrldi    r9, r4, 32
clrldi    r11, r3, 32
addi      r4, r4, 8
addi      r3, r3, 8
addi      r5, r5, -8
lhz       r0, 6(r9)
lhz       r10, 0(r9)
lhz       r8, 2(r9)
lhz       r7, 4(r9)
sth       r0, 6(r11)
sth       r10, 0(r11)
sth       r8, 2(r11)
sth       r7, 4(r11)
bdnz      loc_1D3BD84

loc_1D3BDBC:
cmplwi    cr7, r5, 1
ble       cr7, loc_1D3B4CC
addi      r9, r5, -2
li        r10, 0
extrdi    r9, r9, 31,32
addi      r9, r9, 1
mtctr     r9

loc_1D3BDD8:
add       r9, r10, r4
add       r11, r10, r3
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -2
addi      r10, r10, 2
lhz       r0, 0(r9)
sth       r0, 0(r11)
bdnz      loc_1D3BDD8
b         loc_1D3B670
 # ---------------------------------------------------------------------------

loc_1D3BE00:
cmpwi     cr7, r11, 0
bne       cr7, loc_1D3BF44
addi      r4, r12, -1
addi      r3, r31, -1
clrldi    r9, r4, 32
clrlwi    r10, r3, 29
clrldi    r11, r3, 32
cmpwi     cr7, r10, 4
addi      r30, r8, -1
lbz       r0, 0(r9)
stb       r0, 0(r11)
beq       cr7, loc_1D3C2D8
cmpwi     cr7, r10, 6
beq       cr7, loc_1D3C4F8
cmpwi     cr7, r10, 2
beq       cr7, loc_1D3C2B4

loc_1D3BE40:

cmplwi    cr7, r30, 0x1F
mr        r31, r3
mr        r29, r4
ble       cr7, loc_1D3BEC8
addi      r9, r30, -0x20
extrdi    r9, r9, 27,32
addi      r9, r9, 1
mtctr     r9

loc_1D3BE60:
addi      r8, r29, -8
addi      r7, r29, -0x10
addi      r6, r29, -0x18
addi      r29, r29, -0x20
clrldi    r8, r8, 32
clrldi    r7, r7, 32
addi      r9, r31, -8
addi      r11, r31, -0x10
addi      r10, r31, -0x18
clrldi    r6, r6, 32
ld        r0, 0(r8)
addi      r31, r31, -0x20
ld        r5, 0(r7)
clrldi    r4, r29, 32
clrldi    r9, r9, 32
clrldi    r11, r11, 32
clrldi    r10, r10, 32
ld        r8, 0(r6)
clrldi    r3, r31, 32
ld        r7, 0(r4)
addi      r30, r30, -0x20
std       r0, 0(r9)
std       r5, 0(r11)
std       r8, 0(r10)
std       r7, 0(r3)
bdnz      loc_1D3BE60

loc_1D3BEC8:
cmplwi    cr7, r30, 7
ble       cr7, loc_1D3B9F0
addi      r9, r30, -8
extrdi    r9, r9, 29,32
addi      r9, r9, 1
mtctr     r9

loc_1D3BEE0:
addi      r29, r29, -8
addi      r31, r31, -8
clrldi    r9, r29, 32
clrldi    r11, r31, 32
addi      r30, r30, -8
ld        r0, 0(r9)
std       r0, 0(r11)
bdnz      loc_1D3BEE0
mr        r12, r29
b         loc_1D3B3EC
 # ---------------------------------------------------------------------------

loc_1D3BF08:
clrldi    r9, r31, 32
clrldi    r11, r30, 32
addi      r5, r8, -3
addi      r30, r3, 3
addi      r31, r4, 3
lhz       r0, 0(r9)
sth       r0, 0(r11)

loc_1D3BF24:
clrldi    r9, r31, 32
clrldi    r11, r30, 32
addi      r5, r5, -4
addi      r30, r30, 4
addi      r31, r31, 4
lwz       r0, 0(r9)
stw       r0, 0(r11)
b         loc_1D3B810
 # ---------------------------------------------------------------------------

loc_1D3BF44:
cmpwi     cr7, r11, 4
beq       cr7, loc_1D3C3E4
cmpwi     cr7, r11, 2
mr        r30, r8
bne       cr7, loc_1D3B3F4
addi      r11, r12, -1
addi      r0, r31, -1
clrldi    r9, r11, 32
clrldi    r10, r0, 32
addi      r30, r8, -1
mr        r31, r0
cmplwi    cr7, r30, 7
lbz       r0, 0(r9)
mr        r29, r11
stb       r0, 0(r10)
ble       cr7, loc_1D3BFFC
addi      r9, r8, -9
extrdi    r9, r9, 29,32
addi      r9, r9, 1
mtctr     r9

loc_1D3BF94:
addi      r8, r29, -2
addi      r7, r29, -4
addi      r6, r29, -6
addi      r29, r29, -8
clrldi    r8, r8, 32
clrldi    r7, r7, 32
addi      r9, r31, -2
addi      r11, r31, -4
addi      r10, r31, -6
clrldi    r6, r6, 32
lhz       r0, 0(r8)
addi      r31, r31, -8
lhz       r5, 0(r7)
clrldi    r4, r29, 32
clrldi    r9, r9, 32
clrldi    r11, r11, 32
clrldi    r10, r10, 32
lhz       r8, 0(r6)
clrldi    r3, r31, 32
lhz       r7, 0(r4)
addi      r30, r30, -8
sth       r0, 0(r9)
sth       r5, 0(r11)
sth       r8, 0(r10)
sth       r7, 0(r3)
bdnz      loc_1D3BF94

loc_1D3BFFC:
cmplwi    cr7, r30, 1
ble       cr7, loc_1D3B9F0
addi      r9, r30, -2
extrdi    r9, r9, 31,32
addi      r9, r9, 1
mtctr     r9

loc_1D3C014:
addi      r29, r29, -2
addi      r31, r31, -2
clrldi    r9, r29, 32
clrldi    r11, r31, 32
addi      r30, r30, -2
lhz       r0, 0(r9)
sth       r0, 0(r11)
bdnz      loc_1D3C014
mr        r12, r29
b         loc_1D3B3EC
 # ---------------------------------------------------------------------------

loc_1D3C03C:
addi      r9, r8, -8
cmpwi     cr7, r11, 4
extrdi    r9, r9, 29,32
mr        r30, r8
addi      r9, r9, 1
mtctr     r9
beq       cr7, loc_1D3C300

loc_1D3C058:
addi      r8, r29, -2
addi      r7, r29, -4
addi      r6, r29, -6
addi      r29, r29, -8
clrldi    r8, r8, 32
clrldi    r7, r7, 32
addi      r9, r28, -2
addi      r11, r28, -4
addi      r10, r28, -6
clrldi    r6, r6, 32
lhz       r0, 0(r8)
addi      r28, r28, -8
lhz       r5, 0(r7)
clrldi    r4, r29, 32
clrldi    r9, r9, 32
clrldi    r11, r11, 32
clrldi    r10, r10, 32
lhz       r8, 0(r6)
clrldi    r3, r28, 32
lhz       r7, 0(r4)
addi      r30, r30, -8
sth       r0, 0(r9)
sth       r5, 0(r11)
sth       r8, 0(r10)
sth       r7, 0(r3)
bdnz      loc_1D3C058
cmplwi    cr7, r30, 1
ble       cr7, loc_1D3BC88
addi      r9, r30, -2
extrdi    r9, r9, 31,32
addi      r9, r9, 1
mtctr     r9

loc_1D3C0D8:
addi      r29, r29, -2
addi      r28, r28, -2
clrldi    r9, r29, 32
clrldi    r11, r28, 32
addi      r30, r30, -2
lhz       r0, 0(r9)
sth       r0, 0(r11)
bdnz      loc_1D3C0D8
b         loc_1D3BC88
 # ---------------------------------------------------------------------------

loc_1D3C0FC:
clrldi    r11, r4, 32
addi      r30, r3, 1
clrldi    r10, r3, 32
clrlwi    r0, r30, 30
addi      r31, r4, 1
cmpwi     cr7, r0, 0
lbz       r9, 0(r11)
addi      r5, r8, -1
stb       r9, 0(r10)
beq       cr7, loc_1D3C140
clrldi    r9, r31, 32
clrldi    r11, r30, 32
addi      r5, r8, -3
addi      r30, r3, 3
addi      r31, r4, 3
lhz       r0, 0(r9)
sth       r0, 0(r11)

loc_1D3C140:
cmplwi    cr7, r5, 0xF
mr        r3, r30
mr        r4, r31
ble       cr7, loc_1D3C1A8
addi      r9, r5, -0x10
li        r6, 0
extrdi    r9, r9, 28,32
addi      r9, r9, 1
mtctr     r9

loc_1D3C164:
add       r9, r31, r6
add       r11, r6, r30
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -0x10
addi      r6, r6, 0x10
lwz       r10, 0(r9)
lwz       r8, 4(r9)
lwz       r7, 8(r9)
lwz       r0, 0xC(r9)
stw       r10, 0(r11)
stw       r0, 0xC(r11)
stw       r8, 4(r11)
stw       r7, 8(r11)
bdnz      loc_1D3C164
add       r3, r30, r6
add       r4, r31, r6

loc_1D3C1A8:
cmplwi    cr7, r5, 3
ble       cr7, loc_1D3B4CC
addi      r9, r5, -4
li        r8, 0
extrdi    r9, r9, 30,32
addi      r9, r9, 1
mtctr     r9

loc_1D3C1C4:
add       r9, r8, r4
add       r11, r8, r3
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -4
addi      r8, r8, 4
lwz       r0, 0(r9)
stw       r0, 0(r11)
bdnz      loc_1D3C1C4
b         loc_1D3B71C
 # ---------------------------------------------------------------------------

loc_1D3C1EC:
addi      r5, r8, -2
lhz       r0, 0(r31)
addi      r4, r31, 2
cmplwi    cr7, r5, 0xF
sth       r0, 0(r27)
addi      r3, r27, 2
ble       cr7, loc_1D3C250
addi      r9, r8, -0x12
extrdi    r9, r9, 28,32
addi      r9, r9, 1
mtctr     r9

loc_1D3C218:
clrldi    r9, r4, 32
clrldi    r11, r3, 32
addi      r4, r4, 0x10
addi      r3, r3, 0x10
addi      r5, r5, -0x10
lwz       r0, 0xC(r9)
lwz       r10, 0(r9)
lwz       r8, 4(r9)
lwz       r7, 8(r9)
stw       r0, 0xC(r11)
stw       r10, 0(r11)
stw       r8, 4(r11)
stw       r7, 8(r11)
bdnz      loc_1D3C218

loc_1D3C250:
cmplwi    cr7, r5, 3
ble       cr7, loc_1D3B4CC
addi      r9, r5, -4
li        r10, 0
extrdi    r9, r9, 30,32
addi      r9, r9, 1
mtctr     r9

loc_1D3C26C:
add       r9, r10, r4
add       r11, r10, r3
clrldi    r9, r9, 32
clrldi    r11, r11, 32
addi      r5, r5, -4
addi      r10, r10, 4
lwz       r0, 0(r9)
stw       r0, 0(r11)
bdnz      loc_1D3C26C
b         loc_1D3B670
 # ---------------------------------------------------------------------------

loc_1D3C294:
clrldi    r9, r31, 32
clrldi    r11, r30, 32
addi      r5, r8, -3
addi      r30, r3, 3
addi      r31, r4, 3
lhz       r0, 0(r9)
sth       r0, 0(r11)
b         loc_1D3B810
 # ---------------------------------------------------------------------------

loc_1D3C2B4:
addi      r9, r12, -3
addi      r0, r31, -3
clrldi    r11, r9, 32
clrldi    r10, r0, 32
mr        r3, r0
addi      r30, r8, -3
mr        r4, r9
lhz       r0, 0(r11)
sth       r0, 0(r10)

loc_1D3C2D8:
addi      r9, r4, -4
addi      r0, r3, -4
clrldi    r11, r9, 32
clrldi    r10, r0, 32
mr        r3, r0
addi      r30, r30, -4
mr        r4, r9
lwz       r0, 0(r11)
stw       r0, 0(r10)
b         loc_1D3BE40
 # ---------------------------------------------------------------------------

loc_1D3C300:
addi      r11, r12, -2
addi      r0, r31, -2
clrldi    r9, r11, 32
clrldi    r10, r0, 32
addi      r30, r8, -2
mr        r31, r0
cmplwi    cr7, r30, 0xF
lhz       r0, 0(r9)
mr        r29, r11
sth       r0, 0(r10)
ble       cr7, loc_1D3C3A4
addi      r9, r8, -0x12
extrdi    r9, r9, 28,32
addi      r9, r9, 1
mtctr     r9

loc_1D3C33C:
addi      r8, r29, -4
addi      r7, r29, -8
addi      r6, r29, -0xC
addi      r29, r29, -0x10
clrldi    r8, r8, 32
clrldi    r7, r7, 32
addi      r9, r31, -4
addi      r11, r31, -8
addi      r10, r31, -0xC
clrldi    r6, r6, 32
lwz       r0, 0(r8)
addi      r31, r31, -0x10
lwz       r5, 0(r7)
clrldi    r4, r29, 32
clrldi    r9, r9, 32
clrldi    r11, r11, 32
clrldi    r10, r10, 32
lwz       r8, 0(r6)
clrldi    r3, r31, 32
lwz       r7, 0(r4)
addi      r30, r30, -0x10
stw       r0, 0(r9)
stw       r5, 0(r11)
stw       r8, 0(r10)
stw       r7, 0(r3)
bdnz      loc_1D3C33C

loc_1D3C3A4:
cmplwi    cr7, r30, 3
ble       cr7, loc_1D3B9F0
addi      r9, r30, -4
extrdi    r9, r9, 30,32
addi      r9, r9, 1
mtctr     r9

loc_1D3C3BC:
addi      r29, r29, -4
addi      r31, r31, -4
clrldi    r9, r29, 32
clrldi    r11, r31, 32
addi      r30, r30, -4
lwz       r0, 0(r9)
stw       r0, 0(r11)
bdnz      loc_1D3C3BC
mr        r12, r29
b         loc_1D3B3EC
 # ---------------------------------------------------------------------------

loc_1D3C3E4:
addi      r4, r12, -1
addi      r3, r31, -1
clrldi    r9, r4, 32
clrlwi    r0, r3, 30
clrldi    r11, r3, 32
cmpwi     cr7, r0, 0
addi      r30, r8, -1
lbz       r0, 0(r9)
stb       r0, 0(r11)
beq       cr7, loc_1D3C430
addi      r9, r12, -3
addi      r0, r31, -3
clrldi    r11, r9, 32
clrldi    r10, r0, 32
mr        r3, r0
addi      r30, r8, -3
mr        r4, r9
lhz       r0, 0(r11)
sth       r0, 0(r10)

loc_1D3C430:
cmplwi    cr7, r30, 0xF
mr        r31, r3
mr        r29, r4
ble       cr7, loc_1D3C4B8
addi      r9, r30, -0x10
extrdi    r9, r9, 28,32
addi      r9, r9, 1
mtctr     r9

loc_1D3C450:
addi      r8, r29, -4
addi      r7, r29, -8
addi      r6, r29, -0xC
addi      r29, r29, -0x10
clrldi    r8, r8, 32
clrldi    r7, r7, 32
addi      r9, r31, -4
addi      r11, r31, -8
addi      r10, r31, -0xC
clrldi    r6, r6, 32
lwz       r0, 0(r8)
addi      r31, r31, -0x10
lwz       r5, 0(r7)
clrldi    r4, r29, 32
clrldi    r9, r9, 32
clrldi    r11, r11, 32
clrldi    r10, r10, 32
lwz       r8, 0(r6)
clrldi    r3, r31, 32
lwz       r7, 0(r4)
addi      r30, r30, -0x10
stw       r0, 0(r9)
stw       r5, 0(r11)
stw       r8, 0(r10)
stw       r7, 0(r3)
bdnz      loc_1D3C450

loc_1D3C4B8:
cmplwi    cr7, r30, 3
ble       cr7, loc_1D3B9F0
addi      r9, r30, -4
extrdi    r9, r9, 30,32
addi      r9, r9, 1
mtctr     r9

loc_1D3C4D0:
addi      r29, r29, -4
addi      r31, r31, -4
clrldi    r9, r29, 32
clrldi    r11, r31, 32
addi      r30, r30, -4
lwz       r0, 0(r9)
stw       r0, 0(r11)
bdnz      loc_1D3C4D0
mr        r12, r29
b         loc_1D3B3EC
 # ---------------------------------------------------------------------------

loc_1D3C4F8:
addi      r9, r12, -3
addi      r0, r31, -3
clrldi    r11, r9, 32
clrldi    r10, r0, 32
mr        r3, r0
addi      r30, r8, -3
mr        r4, r9
lhz       r0, 0(r11)
sth       r0, 0(r10)
b         loc_1D3BE40
 # End of function sub_1D3B2C8
