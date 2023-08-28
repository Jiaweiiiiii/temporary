#!/usr/local/bin/python3
import os
import sys
import re
import string

sys.path.append("./")
from def_mxu3_asm import *
from def_mxu3_1_asm import *

def usage():
    print("""
mxu3_asm.py <file_name>
-h/--help       : Display this usage.
-mxu3.1         : Enable ISA MXU3.1, default is MXU3.0.
                  T40(MXU3.0) T41(MXU3.1) A1(MXU3.1)
-load-sync      : Load sync-nstruction, fix bug for T40/T41/A1.
""")
    sys.exit()

def rename(re_name):
    if re_name == 'zero':
        re_name = 0
    elif re_name == 'at':
        re_name = 1
    elif re_name == 'v0':
        re_name = 2
    elif re_name == 'v1':
        re_name = 3
    elif re_name == 'a0':
        re_name = 4
    elif re_name == 'a1':
        re_name = 5
    elif re_name == 'a2':
        re_name = 6
    elif re_name == 'a3':
        re_name = 7
    elif re_name == 't0':
        re_name = 8
    elif re_name == 't1':
        re_name = 9
    elif re_name == 't2':
        re_name = 10
    elif re_name == 't3':
        re_name = 11
    elif re_name == 't4':
        re_name = 12
    elif re_name == 't5':
        re_name = 13
    elif re_name == 't6':
        re_name = 14
    elif re_name == 't7':
        re_name = 15
    elif re_name == 's0':
        re_name = 16
    elif re_name == 's1':
        re_name = 17
    elif re_name == 's2':
        re_name = 18
    elif re_name == 's3':
        re_name = 19
    elif re_name == 's4':
        re_name = 20
    elif re_name == 's5':
        re_name = 21
    elif re_name == 's6':
        re_name = 22
    elif re_name == 's7':
        re_name = 23
    elif re_name == 't8':
        re_name = 24
    elif re_name == 't9':
        re_name = 25
    elif re_name == 'k0':
        re_name = 26
    elif re_name == 'k1':
        re_name = 27
    elif re_name == 'gp':
        re_name = 28
    elif re_name == 'sp':
        re_name = 29
    elif (re_name == 'fp') or (re_name == 's8'):
        re_name = 30
    elif re_name == 'ra':
        re_name = 31
    else:
        return -1
    return re_name

def has_bug(insn):
    if (insn == "luw")    or \
       (insn == "lud")    or \
       (insn == "law")    or \
       (insn == "lad")    or \
       (insn == "law2b")  or \
       (insn == "law2h")  or \
       (insn == "law4b")  or \
       (insn == "law4h")  or \
       (insn == "law8b")  or \
       (insn == "law8h")  or \
       (insn == "law16b") or \
       (insn == "law16h") or \
       (insn == "lad2b")  or \
       (insn == "lad2h")  or \
       (insn == "lad2w")  or \
       (insn == "lad4b")  or \
       (insn == "lad4h")  or \
       (insn == "lad4w")  or \
       (insn == "lad8b")  or \
       (insn == "lad8h")  or \
       (insn == "lad8w"):
        return 1
    else:
        return 0

class asm_msg:
    def __init__(self):
        self.guid           = 0
        self.name           = ''
        self.field          = []
        self.comment        = ''
        self.asm_str        = ''

if __name__=="__main__":
    guid = 0
    asm_l = []
    total_l = 1
    no_source = 1
    load_sync = 0
    isa = 0

    for i in range(len(sys.argv)):
        arg = sys.argv[i]
        if (i == 0):
            continue
        elif (arg[-5:] == ".mxu3" or arg[-2:] == ".s" or arg[-2:] == ".S"):
            asm_source = open(arg)
            no_source = 0
        elif (arg == "-h" or arg == "--help"):
            usage()
        elif (arg == "-load-sync" or arg == "--load-sync"):
            load_sync = 1
        elif (arg == "-mxu3.1" or arg == "--mxu3.1"): # __mips_mxu3_1
            isa = 1
        else:
            print("Error unrecognized option \"%s\""%(arg), file=sys.stderr)
            usage()

    if (no_source):
        usage()

    for l in asm_source:
        #print(l, file=sys.stderr)
        #l = (l.split('//'))[0]
        cc = 0
        ll = len(l)
        for c in range(len(l) - 1):
            #print(l[c], file=sys.stderr)
            if l[c] == '"':
                cc += 1
            if (l[c:c+2] == '//') and (cc % 2 == 0):
                ll = c
                break;
        l = l[:ll]
        #print(l, file=sys.stderr)
        w_info = l
        l = l.strip('\n\t').replace('\t',' ')
        l = l.lstrip()
        #print(list(w_info))
        rl = re.split(" ", l)
        #print(l,rl, len(rl))
        if len(rl[0]) > 0:
            #print(rl[0])
            simd_insn = rl[0].split('_')
            #print(simd_insn)
            if simd_insn[0] == 'mxu3':
                #print(rl)
                asm = asm_msg();
                asm.guid = guid
                asm.name = simd_insn[1]
                insn_field = ''.join(rl[1:])
                #print(insn_field)
                insn_field = insn_field.replace('$','').replace(',',' ')
                insn_field = insn_field.replace('(',' ').replace(')','')
                insn_field = insn_field.replace('[',' ').replace(']','')
                insn_field = insn_field.split(' ')
                #print(insn_field)
                asm.field = []
                tr_ele = 0
                for ele in insn_field:
                    if ele.isdigit() or (ele[0] == '-'):
                        asm.field.append(int(ele))
                    elif ele[0:2] == '0x':
                        asm.field.append(int(ele, 16))
                    elif (ele != ' ') and (ele != ''):
                        tr_ele = rename(ele)
                        if tr_ele != -1:
                            asm.field.append(tr_ele)
                        else:
                            break

                if tr_ele != -1:
                    if (asm.name.lower() == "law") or \
                       (asm.name.lower() == "saw"):
                        asm.field[2] = asm.field[2] >> 2 # offset
                    elif (asm.name.lower() == "lad") or \
                         (asm.name.lower() == "sad"):
                        asm.field[2] = asm.field[2] >> 3 # offset
                    elif (asm.name.lower() == "laq") or \
                         (asm.name.lower() == "saq"):
                        asm.field[2] = asm.field[2] >> 4 # offset
                    elif (asm.name.lower() == "lao") or \
                         (asm.name.lower() == "sao"):
                        asm.field[2] = asm.field[2] >> 5 # offset

                    if (isa == 0):
                        insn = def_mxu3_asm(asm)
                    elif (isa == 1):
                        insn = def_mxu3_1_asm(asm)
                    else:
                        print("Error ISA %d"%(isa), file=sys.stderr)
                else:
                    insn = 'error asm format'

                if ((str(insn))[0:5] != 'error'):
                    insn = "\t.word\t"+str(hex(insn))
                else:
                    print("\nError: %s line:%d \"%s\" %s"%(sys.argv[1], total_l, l, insn), file=sys.stderr)
                    total_l += 1
                    continue

                # Fix bug for SIMD word/double-word load of T40/A1
                if (load_sync and has_bug(asm.name.lower())):
                    insn = "\tsync\n" + insn

                #print(insn_field)
                #print(asm.name)
                #print(insn)
                asm.comment = l
                #guid += 1
                w_info = insn
            #else:
            #    print("***********")
        #else:
        #    print(l)
        total_l += 1
        print(w_info)
        continue

    asm_source.close()
    sys.exit()
