# patch func to call hook
.section .text.func_patch, "x"
func_hooked:
lis     r12, func_ptr@h         # load func ptr opd address (hi)
ori     r12, r12, func_ptr@l    # load func ptr opd address (lo)
b func_hook_thunk               # jump to function that calls ptr in r12

# function hook data
.section .text.func_hook, "x"

# calls a hook function pointer stored in r12
func_hook_thunk:
.set back_chain, -0x80
.set saved_toc, -0x58
.set saved_r31, -8
.set sender_lr,  0x10
stdu      r1, back_chain(r1)
mflr      r0
std       r31, 0x80+saved_r31(r1)
std       r0, 0x80+sender_lr(r1)
mr        r31, r1
lwz       r12, 0(r12)               # load opd ptr
lwz       r0, 0(r12)                # load func ptr
mtctr     r0                        # move func ptr to control register
std       r2, 0x80+saved_toc(r1)    # save toc
lwz       r2, 4(r12)                # load new toc
bctrl                               # call func tpr
ld        r2, 0x80+saved_toc(r1)    # restore toc
ld        r11, 0x80+back_chain(r1)  # load old stack ptr
ld        r0, sender_lr(r11)        # load saved lr
mtlr      r0                        # restore lr
ld        r31, saved_r31(r11)       # restore r31
mr        r1, r11
blr

# link back to original function
func_trampoline:
.set saved_r31, -8
.set sender_lr, 0x10
stdu r1, -0x80(r1)
mflr      r0
std       r0, 0x80+sender_lr(r1)
b         func_hooked+(3*4)

.section .data.func_hook, "w"
func_ptr:
.int 0 

