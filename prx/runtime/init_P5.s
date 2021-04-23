.global _shk_prx_elf_substitute_impl
_shk_prx_elf_substitute_impl:
.int ._shk_prx_elf_substitute_impl
.int _shk_elf_toc

# function extracted from the game to make extra room for code
# __int64 __fastcall sub_A3BE6C(unsigned int *a1, unsigned int a2, __int64 a3)
.global ._shk_prx_elf_substitute_impl
._shk_prx_elf_substitute_impl:
li        r6, 0         # Load Immediate
cmpwi     cr6, r4, 0x17 # Compare Word Immediate
cmpwi     cr1, r4, 0x3F # '?' # Compare Word Immediate
cmplwi    r4, 0xF       # Compare Logical Word Immediate
stw       r6, 4(r5)     # Store Word
stw       r6, 8(r5)     # Store Word
bgt       loc_A3BE94    # Branch if greater than
cmplwi    r4, 6         # Compare Logical Word Immediate
ble       loc_A3CA88    # Branch if less than or equal
b         loc_A3BF40    # Branch
# ---------------------------------------------------------------------------

loc_A3BE94:
cmplwi    r4, 0x1E      # Compare Logical Word Immediate
bgt       loc_A3BEE4    # Branch if greater than
cmplwi    r4, 0x15      # Compare Logical Word Immediate
bgt       loc_A3BEC0    # Branch if greater than
cmplwi    r4, 0x12      # Compare Logical Word Immediate
ble       loc_A3BF40    # Branch if less than or equal
cmplwi    r4, 0x13      # Compare Logical Word Immediate
ble       loc_A3BF40    # Branch if less than or equal
cmplwi    r4, 0x14      # Compare Logical Word Immediate
ble       loc_A3C098    # Branch if less than or equal
b         loc_A3BF40    # Branch
# ---------------------------------------------------------------------------

loc_A3BEC0:
cmplwi    r4, 0x1A      # Compare Logical Word Immediate
bgt       loc_A3BED0    # Branch if greater than
beq       cr6, loc_A3BFE0 # Branch if equal
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3BED0:
cmplwi    r4, 0x1B      # Compare Logical Word Immediate
ble       loc_A3C29C    # Branch if less than or equal
cmplwi    r4, 0x1D      # Compare Logical Word Immediate
ble       loc_A3CA88    # Branch if less than or equal
b         loc_A3C354    # Branch
# ---------------------------------------------------------------------------

loc_A3BEE4:
cmplwi    r4, 0x25 # '%' # Compare Logical Word Immediate
bgt       loc_A3BF1C    # Branch if greater than
cmplwi    r4, 0x21 # '!' # Compare Logical Word Immediate
bgt       loc_A3BF08    # Branch if greater than
cmplwi    r4, 0x1F      # Compare Logical Word Immediate
ble       loc_A3C40C    # Branch if less than or equal
cmplwi    r4, 0x20 # ' ' # Compare Logical Word Immediate
ble       loc_A3C57C    # Branch if less than or equal
b         loc_A3C638    # Branch
# ---------------------------------------------------------------------------

loc_A3BF08:
cmplwi    r4, 0x23 # '#' # Compare Logical Word Immediate
ble       loc_A3CA88    # Branch if less than or equal
cmplwi    r4, 0x24 # '$' # Compare Logical Word Immediate
ble       loc_A3C6F4    # Branch if less than or equal
b         loc_A3C7AC    # Branch
# ---------------------------------------------------------------------------

loc_A3BF1C:
cmplwi    r4, 0x28 # '(' # Compare Logical Word Immediate
bgt       loc_A3BF38    # Branch if greater than
cmplwi    r4, 0x26 # '&' # Compare Logical Word Immediate
ble       loc_A3C864    # Branch if less than or equal
cmplwi    r4, 0x27 # ''' # Compare Logical Word Immediate
ble       loc_A3C91C    # Branch if less than or equal
b         loc_A3C9D4    # Branch
# ---------------------------------------------------------------------------

loc_A3BF38:
beq       cr1, loc_A3C4C4 # Branch if equal
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3BF40:
lwz       r7, 0(r3)     # Load Word and Zero
slwi      r6, r4, 1     # Shift Left Immediate
lwz       r8, 0x24(r7)  # Load Word and Zero
lwz       r7, 8(r7)     # Load Word and Zero
lhzx      r8, r8, r6    # Load Half Word and Zero Indexed
addc      r7, r7, r8    # Add Carrying
addic     r7, r7, 4     # Add Immediate Carrying
clrldi    r7, r7, 32    # Clear Left Immediate
lbz       r8, 2(r7)     # Load Byte and Zero
lbz       r9, 3(r7)     # Load Byte and Zero
lbz       r10, 1(r7)    # Load Byte and Zero
insrwi    r9, r8, 8,16  # Insert from Right Immediate
lbz       r7, 0(r7)     # Load Byte and Zero
slwi      r8, r10, 16   # Shift Left Immediate
insrwi    r8, r7, 8,0   # Insert from Right Immediate
or        r7, r9, r8    # OR
stw       r7, 8(r5)     # Store Word
lwz       r3, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r3)  # Load Word and Zero
lwz       r8, 8(r3)     # Load Word and Zero
lwz       r3, 0x10(r3)  # Load Word and Zero
lhzx      r6, r7, r6    # Load Half Word and Zero Indexed
addc      r7, r8, r6    # Add Carrying
addic     r9, r7, 2     # Add Immediate Carrying
addic     r10, r7, 3    # Add Immediate Carrying
clrldi    r9, r9, 32    # Clear Left Immediate
lbzx      r6, r8, r6    # Load Byte and Zero Indexed
addic     r7, r7, 1     # Add Immediate Carrying
clrldi    r10, r10, 32  # Clear Left Immediate
clrldi    r7, r7, 32    # Clear Left Immediate
lbz       r8, 0(r9)     # Load Byte and Zero
lbz       r9, 0(r10)    # Load Byte and Zero
lbz       r7, 0(r7)     # Load Byte and Zero
insrwi    r9, r8, 8,16  # Insert from Right Immediate
slwi      r7, r7, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r9, r7    # OR
addc      r3, r3, r6    # Add Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3BFE0:
lwz       r7, 0(r3)     # Load Word and Zero
lhz       r8, 0x18(r7)  # Load Half Word and Zero
cmpwi     r8, 0x17      # Compare Word Immediate
ble       loc_A3C08C    # Branch if less than or equal
slwi      r6, r4, 1     # Shift Left Immediate
lwz       r8, 0x24(r7)  # Load Word and Zero
lwz       r7, 8(r7)     # Load Word and Zero
lhzx      r8, r8, r6    # Load Half Word and Zero Indexed
addc      r7, r7, r8    # Add Carrying
addic     r7, r7, 4     # Add Immediate Carrying
clrldi    r7, r7, 32    # Clear Left Immediate
lbz       r8, 2(r7)     # Load Byte and Zero
lbz       r9, 3(r7)     # Load Byte and Zero
lbz       r10, 1(r7)    # Load Byte and Zero
insrwi    r9, r8, 8,16  # Insert from Right Immediate
lbz       r7, 0(r7)     # Load Byte and Zero
slwi      r8, r10, 16   # Shift Left Immediate
insrwi    r8, r7, 8,0   # Insert from Right Immediate
or        r7, r9, r8    # OR
stw       r7, 8(r5)     # Store Word
lwz       r3, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r3)  # Load Word and Zero
lwz       r8, 8(r3)     # Load Word and Zero
lwz       r3, 0x10(r3)  # Load Word and Zero
lhzx      r6, r7, r6    # Load Half Word and Zero Indexed
addc      r7, r8, r6    # Add Carrying
addic     r9, r7, 2     # Add Immediate Carrying
addic     r10, r7, 3    # Add Immediate Carrying
lbzx      r6, r8, r6    # Load Byte and Zero Indexed
clrldi    r9, r9, 32    # Clear Left Immediate
addic     r7, r7, 1     # Add Immediate Carrying
clrldi    r10, r10, 32  # Clear Left Immediate
clrldi    r7, r7, 32    # Clear Left Immediate
lbz       r8, 0(r9)     # Load Byte and Zero
lbz       r9, 0(r10)    # Load Byte and Zero
lbz       r7, 0(r7)     # Load Byte and Zero
insrwi    r9, r8, 8,16  # Insert from Right Immediate
slwi      r7, r7, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r9, r7    # OR
addc      r3, r3, r6    # Add Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C08C:
stw       r6, 4(r5)     # Store Word
stw       r6, 8(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C098:
lis       r7, 0x20 # ' ' # Load Immediate Shifted
lwz       r6, 0x3C(r3)  # Load Word and Zero
cmplw     r6, r7        # Compare Logical Word
bgt       loc_A3C144    # Branch if greater than
lwz       r6, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r6)  # Load Word and Zero
lwz       r6, 8(r6)     # Load Word and Zero
lhz       r7, 0x28(r7)  # Load Half Word and Zero
addc      r6, r6, r7    # Add Carrying
addic     r6, r6, 4     # Add Immediate Carrying
clrldi    r6, r6, 32    # Clear Left Immediate
lbz       r7, 2(r6)     # Load Byte and Zero
lbz       r8, 3(r6)     # Load Byte and Zero
lbz       r9, 1(r6)     # Load Byte and Zero
insrwi    r8, r7, 8,16  # Insert from Right Immediate
lbz       r6, 0(r6)     # Load Byte and Zero
slwi      r7, r9, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r8, r7    # OR
stw       r6, 8(r5)     # Store Word
lwz       r3, 0(r3)     # Load Word and Zero
lwz       r6, 0x24(r3)  # Load Word and Zero
lwz       r7, 8(r3)     # Load Word and Zero
lwz       r3, 0x10(r3)  # Load Word and Zero
lhz       r6, 0x28(r6)  # Load Half Word and Zero
addc      r8, r7, r6    # Add Carrying
addic     r9, r8, 2     # Add Immediate Carrying
addic     r10, r8, 3    # Add Immediate Carrying
lbzx      r6, r7, r6    # Load Byte and Zero Indexed
clrldi    r9, r9, 32    # Clear Left Immediate
addic     r8, r8, 1     # Add Immediate Carrying
clrldi    r10, r10, 32  # Clear Left Immediate
clrldi    r8, r8, 32    # Clear Left Immediate
lbz       r7, 0(r9)     # Load Byte and Zero
lbz       r9, 0(r10)    # Load Byte and Zero
lbz       r8, 0(r8)     # Load Byte and Zero
insrwi    r9, r7, 8,16  # Insert from Right Immediate
slwi      r7, r8, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r9, r7    # OR
addc      r3, r3, r6    # Add Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C144:
lis       r7, 0x60 # '`' # Load Immediate Shifted
cmplw     r6, r7        # Compare Logical Word
bgt       loc_A3C200    # Branch if greater than
lwz       r6, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r6)  # Load Word and Zero
lwz       r6, 8(r6)     # Load Word and Zero
lhz       r7, 0x38(r7)  # Load Half Word and Zero
addc      r6, r6, r7    # Add Carrying
addic     r6, r6, 4     # Add Immediate Carrying
clrldi    r6, r6, 32    # Clear Left Immediate
lbz       r7, 2(r6)     # Load Byte and Zero
lbz       r8, 3(r6)     # Load Byte and Zero
lbz       r9, 1(r6)     # Load Byte and Zero
insrwi    r8, r7, 8,16  # Insert from Right Immediate
lbz       r6, 0(r6)     # Load Byte and Zero
slwi      r7, r9, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r8, r7    # OR
stw       r6, 8(r5)     # Store Word
lwz       r7, 0(r3)     # Load Word and Zero
lwz       r8, 0x24(r7)  # Load Word and Zero
lwz       r9, 8(r7)     # Load Word and Zero
lwz       r7, 0x10(r7)  # Load Word and Zero
lhz       r8, 0x38(r8)  # Load Half Word and Zero
addc      r10, r9, r8   # Add Carrying
addic     r11, r10, 2   # Add Immediate Carrying
addic     r12, r10, 3   # Add Immediate Carrying
lbzx      r8, r9, r8    # Load Byte and Zero Indexed
clrldi    r11, r11, 32  # Clear Left Immediate
addic     r10, r10, 1   # Add Immediate Carrying
clrldi    r12, r12, 32  # Clear Left Immediate
clrldi    r10, r10, 32  # Clear Left Immediate
lbz       r9, 0(r11)    # Load Byte and Zero
lbz       r11, 0(r12)   # Load Byte and Zero
lbz       r10, 0(r10)   # Load Byte and Zero
insrwi    r11, r9, 8,16 # Insert from Right Immediate
slwi      r9, r10, 16   # Shift Left Immediate
insrwi    r9, r8, 8,0   # Insert from Right Immediate
or        r8, r11, r9   # OR
addc      r7, r7, r8    # Add Carrying
stw       r7, 4(r5)     # Store Word
lwz       r7, 0x44(r3)  # Load Word and Zero
lwz       r3, 0x40(r3)  # Load Word and Zero
addc      r3, r7, r3    # Add Carrying
subfc     r3, r6, r3    # Subtract from Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C200:
lwz       r6, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r6)  # Load Word and Zero
lwz       r6, 8(r6)     # Load Word and Zero
lhz       r7, 0x7C(r7)  # Load Half Word and Zero
addc      r6, r6, r7    # Add Carrying
addic     r6, r6, 4     # Add Immediate Carrying
clrldi    r6, r6, 32    # Clear Left Immediate
lbz       r7, 2(r6)     # Load Byte and Zero
lbz       r8, 3(r6)     # Load Byte and Zero
lbz       r9, 1(r6)     # Load Byte and Zero
insrwi    r8, r7, 8,16  # Insert from Right Immediate
lbz       r6, 0(r6)     # Load Byte and Zero
slwi      r7, r9, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r8, r7    # OR
stw       r6, 8(r5)     # Store Word
lwz       r3, 0(r3)     # Load Word and Zero
lwz       r6, 0x24(r3)  # Load Word and Zero
lwz       r7, 8(r3)     # Load Word and Zero
lwz       r3, 0x10(r3)  # Load Word and Zero
lhz       r6, 0x7C(r6)  # Load Half Word and Zero
addc      r8, r7, r6    # Add Carrying
addic     r9, r8, 2     # Add Immediate Carrying
addic     r10, r8, 3    # Add Immediate Carrying
lbzx      r6, r7, r6    # Load Byte and Zero Indexed
clrldi    r9, r9, 32    # Clear Left Immediate
addic     r8, r8, 1     # Add Immediate Carrying
clrldi    r10, r10, 32  # Clear Left Immediate
clrldi    r8, r8, 32    # Clear Left Immediate
lbz       r7, 0(r9)     # Load Byte and Zero
lbz       r9, 0(r10)    # Load Byte and Zero
lbz       r8, 0(r8)     # Load Byte and Zero
insrwi    r9, r7, 8,16  # Insert from Right Immediate
slwi      r7, r8, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r9, r7    # OR
addc      r3, r3, r6    # Add Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C29C:
lwz       r7, 0(r3)     # Load Word and Zero
lhz       r8, 0x18(r7)  # Load Half Word and Zero
cmpwi     r8, 0x1B      # Compare Word Immediate
ble       loc_A3C348    # Branch if less than or equal
slwi      r6, r4, 1     # Shift Left Immediate
lwz       r8, 0x24(r7)  # Load Word and Zero
lwz       r7, 8(r7)     # Load Word and Zero
lhzx      r8, r8, r6    # Load Half Word and Zero Indexed
addc      r7, r7, r8    # Add Carrying
addic     r7, r7, 4     # Add Immediate Carrying
clrldi    r7, r7, 32    # Clear Left Immediate
lbz       r8, 2(r7)     # Load Byte and Zero
lbz       r9, 3(r7)     # Load Byte and Zero
lbz       r10, 1(r7)    # Load Byte and Zero
insrwi    r9, r8, 8,16  # Insert from Right Immediate
lbz       r7, 0(r7)     # Load Byte and Zero
slwi      r8, r10, 16   # Shift Left Immediate
insrwi    r8, r7, 8,0   # Insert from Right Immediate
or        r7, r9, r8    # OR
stw       r7, 8(r5)     # Store Word
lwz       r3, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r3)  # Load Word and Zero
lwz       r8, 8(r3)     # Load Word and Zero
lwz       r3, 0x10(r3)  # Load Word and Zero
lhzx      r6, r7, r6    # Load Half Word and Zero Indexed
addc      r7, r8, r6    # Add Carrying
addic     r9, r7, 2     # Add Immediate Carrying
addic     r10, r7, 3    # Add Immediate Carrying
clrldi    r9, r9, 32    # Clear Left Immediate
lbzx      r6, r8, r6    # Load Byte and Zero Indexed
addic     r7, r7, 1     # Add Immediate Carrying
clrldi    r10, r10, 32  # Clear Left Immediate
clrldi    r7, r7, 32    # Clear Left Immediate
lbz       r8, 0(r9)     # Load Byte and Zero
lbz       r9, 0(r10)    # Load Byte and Zero
lbz       r7, 0(r7)     # Load Byte and Zero
insrwi    r9, r8, 8,16  # Insert from Right Immediate
slwi      r7, r7, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r9, r7    # OR
addc      r3, r3, r6    # Add Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C348:
stw       r6, 4(r5)     # Store Word
stw       r6, 8(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C354:
lwz       r7, 0(r3)     # Load Word and Zero
lhz       r8, 0x18(r7)  # Load Half Word and Zero
cmpwi     r8, 0x1E      # Compare Word Immediate
ble       loc_A3C400    # Branch if less than or equal
slwi      r6, r4, 1     # Shift Left Immediate
lwz       r8, 0x24(r7)  # Load Word and Zero
lwz       r7, 8(r7)     # Load Word and Zero
lhzx      r8, r8, r6    # Load Half Word and Zero Indexed
addc      r7, r7, r8    # Add Carrying
addic     r7, r7, 4     # Add Immediate Carrying
clrldi    r7, r7, 32    # Clear Left Immediate
lbz       r8, 2(r7)     # Load Byte and Zero
lbz       r9, 3(r7)     # Load Byte and Zero
lbz       r10, 1(r7)    # Load Byte and Zero
insrwi    r9, r8, 8,16  # Insert from Right Immediate
lbz       r7, 0(r7)     # Load Byte and Zero
slwi      r8, r10, 16   # Shift Left Immediate
insrwi    r8, r7, 8,0   # Insert from Right Immediate
or        r7, r9, r8    # OR
stw       r7, 8(r5)     # Store Word
lwz       r3, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r3)  # Load Word and Zero
lwz       r8, 8(r3)     # Load Word and Zero
lwz       r3, 0x10(r3)  # Load Word and Zero
lhzx      r6, r7, r6    # Load Half Word and Zero Indexed
addc      r7, r8, r6    # Add Carrying
addic     r9, r7, 2     # Add Immediate Carrying
addic     r10, r7, 3    # Add Immediate Carrying
clrldi    r9, r9, 32    # Clear Left Immediate
lbzx      r6, r8, r6    # Load Byte and Zero Indexed
addic     r7, r7, 1     # Add Immediate Carrying
clrldi    r10, r10, 32  # Clear Left Immediate
clrldi    r7, r7, 32    # Clear Left Immediate
lbz       r8, 0(r9)     # Load Byte and Zero
lbz       r9, 0(r10)    # Load Byte and Zero
lbz       r7, 0(r7)     # Load Byte and Zero
insrwi    r9, r8, 8,16  # Insert from Right Immediate
slwi      r7, r7, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r9, r7    # OR
addc      r3, r3, r6    # Add Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C400:
stw       r6, 4(r5)     # Store Word
stw       r6, 8(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C40C:
lwz       r7, 0(r3)     # Load Word and Zero
lhz       r8, 0x18(r7)  # Load Half Word and Zero
cmpwi     r8, 0x1F      # Compare Word Immediate
ble       loc_A3C4B8    # Branch if less than or equal
slwi      r6, r4, 1     # Shift Left Immediate
lwz       r8, 0x24(r7)  # Load Word and Zero
lwz       r7, 8(r7)     # Load Word and Zero
lhzx      r8, r8, r6    # Load Half Word and Zero Indexed
addc      r7, r7, r8    # Add Carrying
addic     r7, r7, 4     # Add Immediate Carrying
clrldi    r7, r7, 32    # Clear Left Immediate
lbz       r8, 2(r7)     # Load Byte and Zero
lbz       r9, 3(r7)     # Load Byte and Zero
lbz       r10, 1(r7)    # Load Byte and Zero
insrwi    r9, r8, 8,16  # Insert from Right Immediate
lbz       r7, 0(r7)     # Load Byte and Zero
slwi      r8, r10, 16   # Shift Left Immediate
insrwi    r8, r7, 8,0   # Insert from Right Immediate
or        r7, r9, r8    # OR
stw       r7, 8(r5)     # Store Word
lwz       r3, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r3)  # Load Word and Zero
lwz       r8, 8(r3)     # Load Word and Zero
lwz       r3, 0x10(r3)  # Load Word and Zero
lhzx      r6, r7, r6    # Load Half Word and Zero Indexed
addc      r7, r8, r6    # Add Carrying
addic     r9, r7, 2     # Add Immediate Carrying
addic     r10, r7, 3    # Add Immediate Carrying
clrldi    r9, r9, 32    # Clear Left Immediate
lbzx      r6, r8, r6    # Load Byte and Zero Indexed
addic     r7, r7, 1     # Add Immediate Carrying
clrldi    r10, r10, 32  # Clear Left Immediate
clrldi    r7, r7, 32    # Clear Left Immediate
lbz       r8, 0(r9)     # Load Byte and Zero
lbz       r9, 0(r10)    # Load Byte and Zero
lbz       r7, 0(r7)     # Load Byte and Zero
insrwi    r9, r8, 8,16  # Insert from Right Immediate
slwi      r7, r7, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r9, r7    # OR
addc      r3, r3, r6    # Add Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C4B8:
stw       r6, 4(r5)     # Store Word
stw       r6, 8(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C4C4:
lwz       r7, 0(r3)     # Load Word and Zero
lhz       r8, 0x18(r7)  # Load Half Word and Zero
cmpwi     r8, 0x3F # '?' # Compare Word Immediate
ble       loc_A3C570    # Branch if less than or equal
slwi      r6, r4, 1     # Shift Left Immediate
lwz       r8, 0x24(r7)  # Load Word and Zero
lwz       r7, 8(r7)     # Load Word and Zero
lhzx      r8, r8, r6    # Load Half Word and Zero Indexed
addc      r7, r7, r8    # Add Carrying
addic     r7, r7, 4     # Add Immediate Carrying
clrldi    r7, r7, 32    # Clear Left Immediate
lbz       r8, 2(r7)     # Load Byte and Zero
lbz       r9, 3(r7)     # Load Byte and Zero
lbz       r10, 1(r7)    # Load Byte and Zero
insrwi    r9, r8, 8,16  # Insert from Right Immediate
lbz       r7, 0(r7)     # Load Byte and Zero
slwi      r8, r10, 16   # Shift Left Immediate
insrwi    r8, r7, 8,0   # Insert from Right Immediate
or        r7, r9, r8    # OR
stw       r7, 8(r5)     # Store Word
lwz       r3, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r3)  # Load Word and Zero
lwz       r8, 8(r3)     # Load Word and Zero
lwz       r3, 0x10(r3)  # Load Word and Zero
lhzx      r6, r7, r6    # Load Half Word and Zero Indexed
addc      r7, r8, r6    # Add Carrying
addic     r9, r7, 2     # Add Immediate Carrying
addic     r10, r7, 3    # Add Immediate Carrying
clrldi    r9, r9, 32    # Clear Left Immediate
lbzx      r6, r8, r6    # Load Byte and Zero Indexed
addic     r7, r7, 1     # Add Immediate Carrying
clrldi    r10, r10, 32  # Clear Left Immediate
clrldi    r7, r7, 32    # Clear Left Immediate
lbz       r8, 0(r9)     # Load Byte and Zero
lbz       r9, 0(r10)    # Load Byte and Zero
lbz       r7, 0(r7)     # Load Byte and Zero
insrwi    r9, r8, 8,16  # Insert from Right Immediate
slwi      r7, r7, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r9, r7    # OR
addc      r3, r3, r6    # Add Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C570:
stw       r6, 4(r5)     # Store Word
stw       r6, 8(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C57C:
lis       r7, 0x81      # Load Immediate Shifted
lwz       r8, 0x3C(r3)  # Load Word and Zero
ori       r7, r7, 0x810200@l # OR Immediate
cmplw     r8, r7        # Compare Logical Word
blt       loc_A3C62C    # Branch if less than
lwz       r6, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r6)  # Load Word and Zero
lwz       r6, 8(r6)     # Load Word and Zero
lhz       r7, 0x40(r7)  # Load Half Word and Zero
addc      r6, r6, r7    # Add Carrying
addic     r6, r6, 4     # Add Immediate Carrying
clrldi    r6, r6, 32    # Clear Left Immediate
lbz       r7, 2(r6)     # Load Byte and Zero
lbz       r8, 3(r6)     # Load Byte and Zero
lbz       r9, 1(r6)     # Load Byte and Zero
insrwi    r8, r7, 8,16  # Insert from Right Immediate
lbz       r6, 0(r6)     # Load Byte and Zero
slwi      r7, r9, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r8, r7    # OR
stw       r6, 8(r5)     # Store Word
lwz       r3, 0(r3)     # Load Word and Zero
lwz       r6, 0x24(r3)  # Load Word and Zero
lwz       r7, 8(r3)     # Load Word and Zero
lwz       r3, 0x10(r3)  # Load Word and Zero
lhz       r6, 0x40(r6)  # Load Half Word and Zero
addc      r8, r7, r6    # Add Carrying
addic     r9, r8, 2     # Add Immediate Carrying
addic     r10, r8, 3    # Add Immediate Carrying
lbzx      r6, r7, r6    # Load Byte and Zero Indexed
clrldi    r9, r9, 32    # Clear Left Immediate
addic     r8, r8, 1     # Add Immediate Carrying
clrldi    r10, r10, 32  # Clear Left Immediate
clrldi    r8, r8, 32    # Clear Left Immediate
lbz       r7, 0(r9)     # Load Byte and Zero
lbz       r9, 0(r10)    # Load Byte and Zero
lbz       r8, 0(r8)     # Load Byte and Zero
insrwi    r9, r7, 8,16  # Insert from Right Immediate
slwi      r7, r8, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r9, r7    # OR
addc      r3, r3, r6    # Add Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C62C:
stw       r6, 4(r5)     # Store Word
stw       r6, 8(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C638:
lis       r7, 0x81      # Load Immediate Shifted
lwz       r8, 0x3C(r3)  # Load Word and Zero
ori       r7, r7, 0x810200@l # OR Immediate
cmplw     r8, r7        # Compare Logical Word
blt       loc_A3C6E8    # Branch if less than
lwz       r6, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r6)  # Load Word and Zero
lwz       r6, 8(r6)     # Load Word and Zero
lhz       r7, 0x42(r7)  # Load Half Word and Zero
addc      r6, r6, r7    # Add Carrying
addic     r6, r6, 4     # Add Immediate Carrying
clrldi    r6, r6, 32    # Clear Left Immediate
lbz       r7, 2(r6)     # Load Byte and Zero
lbz       r8, 3(r6)     # Load Byte and Zero
lbz       r9, 1(r6)     # Load Byte and Zero
insrwi    r8, r7, 8,16  # Insert from Right Immediate
lbz       r6, 0(r6)     # Load Byte and Zero
slwi      r7, r9, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r8, r7    # OR
stw       r6, 8(r5)     # Store Word
lwz       r3, 0(r3)     # Load Word and Zero
lwz       r6, 0x24(r3)  # Load Word and Zero
lwz       r7, 8(r3)     # Load Word and Zero
lwz       r3, 0x10(r3)  # Load Word and Zero
lhz       r6, 0x42(r6)  # Load Half Word and Zero
addc      r8, r7, r6    # Add Carrying
addic     r9, r8, 2     # Add Immediate Carrying
addic     r10, r8, 3    # Add Immediate Carrying
clrldi    r9, r9, 32    # Clear Left Immediate
lbzx      r6, r7, r6    # Load Byte and Zero Indexed
addic     r8, r8, 1     # Add Immediate Carrying
clrldi    r10, r10, 32  # Clear Left Immediate
clrldi    r8, r8, 32    # Clear Left Immediate
lbz       r7, 0(r9)     # Load Byte and Zero
lbz       r9, 0(r10)    # Load Byte and Zero
lbz       r8, 0(r8)     # Load Byte and Zero
insrwi    r9, r7, 8,16  # Insert from Right Immediate
slwi      r7, r8, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r9, r7    # OR
addc      r3, r3, r6    # Add Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C6E8:
stw       r6, 4(r5)     # Store Word
stw       r6, 8(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C6F4:
lis       r7, 0x113     # Load Immediate Shifted
lwz       r8, 0x3C(r3)  # Load Word and Zero
cmplw     r8, r7        # Compare Logical Word
blt       loc_A3C7A0    # Branch if less than
lwz       r6, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r6)  # Load Word and Zero
lwz       r6, 8(r6)     # Load Word and Zero
lhz       r7, 0x48(r7)  # Load Half Word and Zero
addc      r6, r6, r7    # Add Carrying
addic     r6, r6, 4     # Add Immediate Carrying
clrldi    r6, r6, 32    # Clear Left Immediate
lbz       r7, 2(r6)     # Load Byte and Zero
lbz       r8, 3(r6)     # Load Byte and Zero
lbz       r9, 1(r6)     # Load Byte and Zero
insrwi    r8, r7, 8,16  # Insert from Right Immediate
lbz       r6, 0(r6)     # Load Byte and Zero
slwi      r7, r9, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r8, r7    # OR
stw       r6, 8(r5)     # Store Word
lwz       r3, 0(r3)     # Load Word and Zero
lwz       r6, 0x24(r3)  # Load Word and Zero
lwz       r7, 8(r3)     # Load Word and Zero
lwz       r3, 0x10(r3)  # Load Word and Zero
lhz       r6, 0x48(r6)  # Load Half Word and Zero
addc      r8, r7, r6    # Add Carrying
addic     r9, r8, 2     # Add Immediate Carrying
addic     r10, r8, 3    # Add Immediate Carrying
clrldi    r9, r9, 32    # Clear Left Immediate
lbzx      r6, r7, r6    # Load Byte and Zero Indexed
addic     r8, r8, 1     # Add Immediate Carrying
clrldi    r10, r10, 32  # Clear Left Immediate
clrldi    r8, r8, 32    # Clear Left Immediate
lbz       r7, 0(r9)     # Load Byte and Zero
lbz       r9, 0(r10)    # Load Byte and Zero
lbz       r8, 0(r8)     # Load Byte and Zero
insrwi    r9, r7, 8,16  # Insert from Right Immediate
slwi      r7, r8, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r9, r7    # OR
addc      r3, r3, r6    # Add Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C7A0:
stw       r6, 4(r5)     # Store Word
stw       r6, 8(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C7AC:
lis       r7, 0x115     # Load Immediate Shifted
lwz       r8, 0x3C(r3)  # Load Word and Zero
cmplw     r8, r7        # Compare Logical Word
blt       loc_A3C858    # Branch if less than
lwz       r6, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r6)  # Load Word and Zero
lwz       r6, 8(r6)     # Load Word and Zero
lhz       r7, 0x4A(r7)  # Load Half Word and Zero
addc      r6, r6, r7    # Add Carrying
addic     r6, r6, 4     # Add Immediate Carrying
clrldi    r6, r6, 32    # Clear Left Immediate
lbz       r7, 2(r6)     # Load Byte and Zero
lbz       r8, 3(r6)     # Load Byte and Zero
lbz       r9, 1(r6)     # Load Byte and Zero
insrwi    r8, r7, 8,16  # Insert from Right Immediate
lbz       r6, 0(r6)     # Load Byte and Zero
slwi      r7, r9, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r8, r7    # OR
stw       r6, 8(r5)     # Store Word
lwz       r3, 0(r3)     # Load Word and Zero
lwz       r6, 0x24(r3)  # Load Word and Zero
lwz       r7, 8(r3)     # Load Word and Zero
lwz       r3, 0x10(r3)  # Load Word and Zero
lhz       r6, 0x4A(r6)  # Load Half Word and Zero
addc      r8, r7, r6    # Add Carrying
addic     r9, r8, 2     # Add Immediate Carrying
addic     r10, r8, 3    # Add Immediate Carrying
clrldi    r9, r9, 32    # Clear Left Immediate
lbzx      r6, r7, r6    # Load Byte and Zero Indexed
addic     r8, r8, 1     # Add Immediate Carrying
clrldi    r10, r10, 32  # Clear Left Immediate
clrldi    r8, r8, 32    # Clear Left Immediate
lbz       r7, 0(r9)     # Load Byte and Zero
lbz       r9, 0(r10)    # Load Byte and Zero
lbz       r8, 0(r8)     # Load Byte and Zero
insrwi    r9, r7, 8,16  # Insert from Right Immediate
slwi      r7, r8, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r9, r7    # OR
addc      r3, r3, r6    # Add Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C858:
stw       r6, 4(r5)     # Store Word
stw       r6, 8(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C864:
lis       r7, 0x116     # Load Immediate Shifted
lwz       r8, 0x3C(r3)  # Load Word and Zero
cmplw     r8, r7        # Compare Logical Word
blt       loc_A3C910    # Branch if less than
lwz       r6, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r6)  # Load Word and Zero
lwz       r6, 8(r6)     # Load Word and Zero
lhz       r7, 0x4C(r7)  # Load Half Word and Zero
addc      r6, r6, r7    # Add Carrying
addic     r6, r6, 4     # Add Immediate Carrying
clrldi    r6, r6, 32    # Clear Left Immediate
lbz       r7, 2(r6)     # Load Byte and Zero
lbz       r8, 3(r6)     # Load Byte and Zero
lbz       r9, 1(r6)     # Load Byte and Zero
insrwi    r8, r7, 8,16  # Insert from Right Immediate
lbz       r6, 0(r6)     # Load Byte and Zero
slwi      r7, r9, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r8, r7    # OR
stw       r6, 8(r5)     # Store Word
lwz       r3, 0(r3)     # Load Word and Zero
lwz       r6, 0x24(r3)  # Load Word and Zero
lwz       r7, 8(r3)     # Load Word and Zero
lwz       r3, 0x10(r3)  # Load Word and Zero
lhz       r6, 0x4C(r6)  # Load Half Word and Zero
addc      r8, r7, r6    # Add Carrying
addic     r9, r8, 2     # Add Immediate Carrying
addic     r10, r8, 3    # Add Immediate Carrying
clrldi    r9, r9, 32    # Clear Left Immediate
lbzx      r6, r7, r6    # Load Byte and Zero Indexed
addic     r8, r8, 1     # Add Immediate Carrying
clrldi    r10, r10, 32  # Clear Left Immediate
clrldi    r8, r8, 32    # Clear Left Immediate
lbz       r7, 0(r9)     # Load Byte and Zero
lbz       r9, 0(r10)    # Load Byte and Zero
lbz       r8, 0(r8)     # Load Byte and Zero
insrwi    r9, r7, 8,16  # Insert from Right Immediate
slwi      r7, r8, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r9, r7    # OR
addc      r3, r3, r6    # Add Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C910:
stw       r6, 4(r5)     # Store Word
stw       r6, 8(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C91C:
lis       r7, 0x123     # Load Immediate Shifted
lwz       r8, 0x3C(r3)  # Load Word and Zero
cmplw     r8, r7        # Compare Logical Word
blt       loc_A3C9C8    # Branch if less than
lwz       r6, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r6)  # Load Word and Zero
lwz       r6, 8(r6)     # Load Word and Zero
lhz       r7, 0x4E(r7)  # Load Half Word and Zero
addc      r6, r6, r7    # Add Carrying
addic     r6, r6, 4     # Add Immediate Carrying
clrldi    r6, r6, 32    # Clear Left Immediate
lbz       r7, 2(r6)     # Load Byte and Zero
lbz       r8, 3(r6)     # Load Byte and Zero
lbz       r9, 1(r6)     # Load Byte and Zero
insrwi    r8, r7, 8,16  # Insert from Right Immediate
lbz       r6, 0(r6)     # Load Byte and Zero
slwi      r7, r9, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r8, r7    # OR
stw       r6, 8(r5)     # Store Word
lwz       r3, 0(r3)     # Load Word and Zero
lwz       r6, 0x24(r3)  # Load Word and Zero
lwz       r7, 8(r3)     # Load Word and Zero
lwz       r3, 0x10(r3)  # Load Word and Zero
lhz       r6, 0x4E(r6)  # Load Half Word and Zero
addc      r8, r7, r6    # Add Carrying
addic     r9, r8, 2     # Add Immediate Carrying
addic     r10, r8, 3    # Add Immediate Carrying
clrldi    r9, r9, 32    # Clear Left Immediate
lbzx      r6, r7, r6    # Load Byte and Zero Indexed
addic     r8, r8, 1     # Add Immediate Carrying
clrldi    r10, r10, 32  # Clear Left Immediate
clrldi    r8, r8, 32    # Clear Left Immediate
lbz       r7, 0(r9)     # Load Byte and Zero
lbz       r9, 0(r10)    # Load Byte and Zero
lbz       r8, 0(r8)     # Load Byte and Zero
insrwi    r9, r7, 8,16  # Insert from Right Immediate
slwi      r7, r8, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r9, r7    # OR
addc      r3, r3, r6    # Add Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C9C8:
stw       r6, 4(r5)     # Store Word
stw       r6, 8(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3C9D4:
lis       r7, 0x124     # Load Immediate Shifted
lwz       r8, 0x3C(r3)  # Load Word and Zero
cmplw     r8, r7        # Compare Logical Word
blt       loc_A3CA80    # Branch if less than
lwz       r6, 0(r3)     # Load Word and Zero
lwz       r7, 0x24(r6)  # Load Word and Zero
lwz       r6, 8(r6)     # Load Word and Zero
lhz       r7, 0x50(r7)  # Load Half Word and Zero
addc      r6, r6, r7    # Add Carrying
addic     r6, r6, 4     # Add Immediate Carrying
clrldi    r6, r6, 32    # Clear Left Immediate
lbz       r7, 2(r6)     # Load Byte and Zero
lbz       r8, 3(r6)     # Load Byte and Zero
lbz       r9, 1(r6)     # Load Byte and Zero
insrwi    r8, r7, 8,16  # Insert from Right Immediate
lbz       r6, 0(r6)     # Load Byte and Zero
slwi      r7, r9, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r8, r7    # OR
stw       r6, 8(r5)     # Store Word
lwz       r3, 0(r3)     # Load Word and Zero
lwz       r6, 0x24(r3)  # Load Word and Zero
lwz       r7, 8(r3)     # Load Word and Zero
lwz       r3, 0x10(r3)  # Load Word and Zero
lhz       r6, 0x50(r6)  # Load Half Word and Zero
addc      r8, r7, r6    # Add Carrying
addic     r9, r8, 2     # Add Immediate Carrying
addic     r10, r8, 3    # Add Immediate Carrying
clrldi    r9, r9, 32    # Clear Left Immediate
lbzx      r6, r7, r6    # Load Byte and Zero Indexed
addic     r8, r8, 1     # Add Immediate Carrying
clrldi    r10, r10, 32  # Clear Left Immediate
clrldi    r8, r8, 32    # Clear Left Immediate
lbz       r7, 0(r9)     # Load Byte and Zero
lbz       r9, 0(r10)    # Load Byte and Zero
lbz       r8, 0(r8)     # Load Byte and Zero
insrwi    r9, r7, 8,16  # Insert from Right Immediate
slwi      r7, r8, 16    # Shift Left Immediate
insrwi    r7, r6, 8,0   # Insert from Right Immediate
or        r6, r9, r7    # OR
addc      r3, r3, r6    # Add Carrying
stw       r3, 4(r5)     # Store Word
b         loc_A3CA88    # Branch
# ---------------------------------------------------------------------------

loc_A3CA80:
stw       r6, 4(r5)     # Store Word
stw       r6, 8(r5)     # Store Word

loc_A3CA88:
li        r3, 1         # Load Immediate
stb       r4, 0(r5)     # Store Byte
blr                     # Branch unconditionally
# End of function .sub_A3BE6C
