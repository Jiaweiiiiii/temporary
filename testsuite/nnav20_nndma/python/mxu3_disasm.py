#!/usr/local/bin/python3
##        (C) COPYRIGHT Ingenic Limited.
##             ALL RIGHTS RESERVED
##
## File       : mxu3_disasm.py
## Authors    : tyliu@boaz.ic.jz.com
## Create Time: 2018-08-31:14:47:08
## Description:
## 
##
import os
import sys
import re
import string

sys.path.append("./")
from def_mxu3_asm import *
from def_mxu3_disasm import *
from def_mxu3_1_asm import *
from def_mxu3_1_disasm import *

def usage():
    print("""
mxu3_disasm.py <insn(0x00000000)>
-h/--help       : Display this usage.
-mxu3.1         : Enable ISA MXU3.1, default is MXU3.0.
                  T40(MXU3.0) T41(MXU3.1) A1(MXU3.1)
""")
    sys.exit()

class asm_msg:
    def __init__(self):
        self.guid           = 0
        self.name           = ''
        self.field          = []
        self.comment        = ''
        self.asm_str        = ''
        self.code           = 0
    
if __name__=="__main__":
    isa = 0
    insn = ""

    for i in range(len(sys.argv)):
        arg = sys.argv[i]
        if (i == 0):
            continue
        elif (arg == "-h" or arg == "--help"):
            usage()
        elif (arg == "-mxu3.1" or arg == "--mxu3.1"): # __mips_mxu3_1
            isa = 1
        elif (arg[:2] == "0x"):
            insn = arg
        else:
            print("Error unrecognized option \"%s\""%(arg), file=sys.stderr)
            usage()

    if (insn == ""):
        usage()
            
    asm = asm_msg();
    asm.code = int(sys.argv[1], 16)

    if (isa == 0):
        def_mxu3_disasm(asm)
    elif (isa == 1):
        def_mxu3_1_disasm(asm)

    sys.exit()
        
