.section .text.inject, "x"
inject:
bl           loaderMain                     # jump to loader main

.text
# main loader function
loaderMain:
.set frame_size, 0x80
.set modres, -0x10
.set sender_lr,  0x10
.set toc_save, 0x28
# prolog
stdu        r1, -frame_size(r1)             # create stack frame
mflr        r0                              # load lr into r0
std         r0, frame_size+sender_lr(r1)    # save lr from r0
std         r2, frame_size+toc_save(r1)     # save toc to stack

# body
# Print starting message
lis 		r3, starting@ha 				# load starting message into r3
addi		r3, r3, starting@l
bla 		0x05cd098 						# call printf

# Start by loading the NpDrm module
li 			r3, 0x16						# Load CELL_SYSMODULE_SYSUTIL_NP into r3
bla			0x64f4a4 						# call cellSysmoduleLoadModule
ld          r2, frame_size+toc_save(r1)     # restore toc

lis 		r3, klicense@ha					# load klicense into r3
addi 		r3, r3, klicense@l
lis         r4, filename@ha                 # load sprx filename into r4
addi        r4, r4, filename@l
bl 			.sceNpDrmIsAvailable2			# call sceNpDrmIsAvailable2
ld          r2, frame_size+toc_save(r1)     # restore toc

lis         r3, filename@ha                 # load sprx filename into r3
addi        r3, r3, filename@l
li          r4, 0                           # load param 2 into r4
li          r5, 0                           # load param 3 into r5
bl          .sys_prx_load_module            # call sys_prx_load_module
ld          r2, frame_size+toc_save(r1)     # restore toc

extsw       r3, r3                          # sign extend result r3
li          r4, 0                           # load param 2 into r4 (0)
li          r5, 0                           # load param 3 into r5 (0)
addi        r6, r1, frame_size+modres       # load param 4 (out modres) into r6, stored on stack
li          r7, 0                           # load param 5 into r7
li          r8, 0                           # load param 6 into r8
bl          .sys_prx_start_module           # call sys_prx_start_module
ld          r2, frame_size+toc_save(r1)     # restore toc

mr          r4, r3
lwz         r5, frame_size+modres(r1)
lis 		r3, mod_start_status@ha
addi 		r3, r3, mod_start_status@l
bla 		0x05cd098 						# call printf

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
.asciz      "/dev_hdd0/game/BORD00001/USRDIR/mod.sprx"            # sprx filename

klicense:
.asciz 		"\xc0\xa3\xb3\x64\x1c\x2a\xd1\xef\x23\x15\x3a\x48\xa3\xe1\x2f\xe8"  # ratchet klicense

sce_init:
.asciz 		"sceNpInit: %08x\n"

np_drm:
.asciz 		"sceNpDrmIsAvailable: %d\n"

mod_load_status:
.asciz 		"mod_load: %ld\n"

mod_start_status:
.asciz 		"mod_start: %08X, %d\n"

starting:
.asciz 		"Starting CKit runtime loader...\n"
