.section .text.inject, "x"
inject:
bl           loaderMain                     # jump to loader main

.text
# main loader function
loaderMain:
.set frame_size, 0x80
.set var_10, -0x10
.set sender_lr,  0x10
.set toc_save, 0x28
# prolog
stdu        r1, -frame_size(r1)             # create stack frame
mflr        r0                              # load lr into r0
std         r0, frame_size+sender_lr(r1)    # save lr from r0
std         r2, frame_size+toc_save(r1)     # save toc to stack
# body
lis         r3, filename@ha                 # load sprx filename into r3
addi        r3, r3, filename@l
li          r4, 0                           # load param 2 into r4
li          r5, 0                           # load param 3 into r5
bl          .sys_prx_load_module            # call sys_prx_load_module
extsw       r3, r3                          # sign extend result r3     
li          r4, 0                           # load param 2 into r4 (0)
li          r5, 0                           # load param 3 into r5 (0)
addi        r6, r1, frame_size+var_10       # load param 4 (out modres) into r6, stored on stack
li          r7, 0                           # load param 5 into r7
li          r8, 0                           # load param 6 into r8
bl          .sys_prx_start_module           # call sys_prx_start_module
# epilog
ld          r0, frame_size+sender_lr(r1)    # load saved lr
ld          r2, frame_size+toc_save(r1)     # restore toc
addi        r1, r1, frame_size              # destroy stack frame
mtlr        r0                              # restore lr

trampoline:
#nop                                        # replaced instruction
blr                                         # return to caller

.balign 16
filename:
.asciz      "/app_home/mod.sprx"            # sprx filename

