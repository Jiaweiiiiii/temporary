# Auto gen, don't modify this code

import sys
def def_mxu3_asm(asm):
    if asm.name.lower() == "nop":
        insn = 0x0
    elif asm.name.lower() == "bnezb":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a600014
        insn |= ((asm.field[0] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[1] >> 0) & 0x3ff) << 6 # offset 
        asm.asm_str = "bnezb vrs, offset"
    elif asm.name.lower() == "bnezh":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a600015
        insn |= ((asm.field[0] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[1] >> 0) & 0x3ff) << 6 # offset 
        asm.asm_str = "bnezh vrs, offset"
    elif asm.name.lower() == "bnezw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a600016
        insn |= ((asm.field[0] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[1] >> 0) & 0x3ff) << 6 # offset 
        asm.asm_str = "bnezw vrs, offset"
    elif asm.name.lower() == "bnezv":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a600017
        insn |= ((asm.field[0] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[1] >> 0) & 0x3ff) << 6 # offset 
        asm.asm_str = "bnezv vrs, offset"
    elif asm.name.lower() == "beqzb":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a600010
        insn |= ((asm.field[0] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[1] >> 0) & 0x3ff) << 6 # offset 
        asm.asm_str = "beqzb vrs, offset"
    elif asm.name.lower() == "beqzh":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a600011
        insn |= ((asm.field[0] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[1] >> 0) & 0x3ff) << 6 # offset 
        asm.asm_str = "beqzh vrs, offset"
    elif asm.name.lower() == "beqzw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a600012
        insn |= ((asm.field[0] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[1] >> 0) & 0x3ff) << 6 # offset 
        asm.asm_str = "beqzw vrs, offset"
    elif asm.name.lower() == "beqzv":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a600013
        insn |= ((asm.field[0] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[1] >> 0) & 0x3ff) << 6 # offset 
        asm.asm_str = "beqzv vrs, offset"
    elif asm.name.lower() == "ceqb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000028
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ceqb vrd, vrs, vrp"
    elif asm.name.lower() == "ceqh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000029
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ceqh vrd, vrs, vrp"
    elif asm.name.lower() == "ceqw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00002a
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ceqw vrd, vrs, vrp"
    elif asm.name.lower() == "ceqzb":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a000020
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ceqzb vrd, vrs"
    elif asm.name.lower() == "ceqzh":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a000021
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ceqzh vrd, vrs"
    elif asm.name.lower() == "ceqzw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a000022
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ceqzw vrd, vrs"
    elif asm.name.lower() == "clesb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00003c
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "clesb vrd, vrs, vrp"
    elif asm.name.lower() == "clesh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00003d
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "clesh vrd, vrs, vrp"
    elif asm.name.lower() == "clesw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00003e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "clesw vrd, vrs, vrp"
    elif asm.name.lower() == "cleub":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "cleub vrd, vrs, vrp"
    elif asm.name.lower() == "cleuh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000039
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "cleuh vrd, vrs, vrp"
    elif asm.name.lower() == "cleuw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00003a
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "cleuw vrd, vrs, vrp"
    elif asm.name.lower() == "clezb":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a00002c
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "clezb vrd, vrs"
    elif asm.name.lower() == "clezh":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a00002d
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "clezh vrd, vrs"
    elif asm.name.lower() == "clezw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a00002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "clezw vrd, vrs"
    elif asm.name.lower() == "cltsb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000034
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "cltsb vrd, vrs, vrp"
    elif asm.name.lower() == "cltsh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000035
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "cltsh vrd, vrs, vrp"
    elif asm.name.lower() == "cltsw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000036
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "cltsw vrd, vrs, vrp"
    elif asm.name.lower() == "cltub":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000030
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "cltub vrd, vrs, vrp"
    elif asm.name.lower() == "cltuh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "cltuh vrd, vrs, vrp"
    elif asm.name.lower() == "cltuw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000032
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "cltuw vrd, vrs, vrp"
    elif asm.name.lower() == "cltzb":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a000024
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "cltzb vrd, vrs"
    elif asm.name.lower() == "cltzh":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a000025
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "cltzh vrd, vrs"
    elif asm.name.lower() == "cltzw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a000026
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "cltzw vrd, vrs"
    elif asm.name.lower() == "addb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800000
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "addb vrd, vrs, vrp"
    elif asm.name.lower() == "addh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800001
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "addh vrd, vrs, vrp"
    elif asm.name.lower() == "addw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800002
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "addw vrd, vrs, vrp"
    elif asm.name.lower() == "addiw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b600020
        insn |= ((asm.field[2] >> 3) & 0x1f) << 16 # imm 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vwrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vwrd 
        insn |= ((asm.field[2] >> 0) & 0x7) << 0 # imm 
        asm.asm_str = "addiw vwrd, vwrp, imm"
    elif asm.name.lower() == "addrw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4b600034
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vwrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vwrd 
        asm.asm_str = "addrw vwrd, vwrp"
    elif asm.name.lower() == "subb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800008
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "subb vrd, vrs, vrp"
    elif asm.name.lower() == "subh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800009
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "subh vrd, vrs, vrp"
    elif asm.name.lower() == "subw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a80000a
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "subw vrd, vrs, vrp"
    elif asm.name.lower() == "waddsbl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800024
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "waddsbl vrd, vrs, vrp"
    elif asm.name.lower() == "waddsbh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800026
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "waddsbh vrd, vrs, vrp"
    elif asm.name.lower() == "waddshl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800025
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "waddshl vrd, vrs, vrp"
    elif asm.name.lower() == "waddshh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800027
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "waddshh vrd, vrs, vrp"
    elif asm.name.lower() == "waddubl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800020
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "waddubl vrd, vrs, vrp"
    elif asm.name.lower() == "waddubh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800022
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "waddubh vrd, vrs, vrp"
    elif asm.name.lower() == "wadduhl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800021
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wadduhl vrd, vrs, vrp"
    elif asm.name.lower() == "wadduhh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800023
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wadduhh vrd, vrs, vrp"
    elif asm.name.lower() == "wsubsbl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a80002c
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wsubsbl vrd, vrs, vrp"
    elif asm.name.lower() == "wsubsbh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a80002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wsubsbh vrd, vrs, vrp"
    elif asm.name.lower() == "wsubshl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a80002d
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wsubshl vrd, vrs, vrp"
    elif asm.name.lower() == "wsubshh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a80002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wsubshh vrd, vrs, vrp"
    elif asm.name.lower() == "wsububl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800028
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wsububl vrd, vrs, vrp"
    elif asm.name.lower() == "wsububh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a80002a
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wsububh vrd, vrs, vrp"
    elif asm.name.lower() == "wsubuhl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800029
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wsubuhl vrd, vrs, vrp"
    elif asm.name.lower() == "wsubuhh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a80002b
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wsubuhh vrd, vrs, vrp"
    elif asm.name.lower() == "sr1sum2bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae00000
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "sr1sum2bi vsd[m], vrs"
    elif asm.name.lower() == "sr2sum2bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae00800
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 0 # m 
        asm.asm_str = "sr2sum2bi vsd[m], vrs"
    elif asm.name.lower() == "sr4sum2bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae01000
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 0 # m 
        asm.asm_str = "sr4sum2bi vsd[m], vrs"
    elif asm.name.lower() == "sr8sum2bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae01800
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 0 # m 
        asm.asm_str = "sr8sum2bi vsd[m], vrs"
    elif asm.name.lower() == "sr16sum2bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae02000
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 0 # m 
        asm.asm_str = "sr16sum2bi vsd[m], vrs"
    elif asm.name.lower() == "sr1sum4bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae00100
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "sr1sum4bi vsd[m], vrs"
    elif asm.name.lower() == "sr2sum4bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae00900
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 0 # m 
        asm.asm_str = "sr2sum4bi vsd[m], vrs"
    elif asm.name.lower() == "sr4sum4bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae01100
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 0 # m 
        asm.asm_str = "sr4sum4bi vsd[m], vrs"
    elif asm.name.lower() == "sr8sum4bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae01900
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 0 # m 
        asm.asm_str = "sr8sum4bi vsd[m], vrs"
    elif asm.name.lower() == "sr16sum4bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae02100
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 0 # m 
        asm.asm_str = "sr16sum4bi vsd[m], vrs"
    elif asm.name.lower() == "sr1sumub":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae00200
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "sr1sumub vsd[m], vrs"
    elif asm.name.lower() == "sr2sumub":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae00a00
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 0 # m 
        asm.asm_str = "sr2sumub vsd[m], vrs"
    elif asm.name.lower() == "sr4sumub":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae01200
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 0 # m 
        asm.asm_str = "sr4sumub vsd[m], vrs"
    elif asm.name.lower() == "sr8sumub":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae01a00
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 0 # m 
        asm.asm_str = "sr8sumub vsd[m], vrs"
    elif asm.name.lower() == "sr16sumub":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae02200
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 0 # m 
        asm.asm_str = "sr16sumub vsd[m], vrs"
    elif asm.name.lower() == "sr1sumuh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae00300
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "sr1sumuh vsd[m], vrs"
    elif asm.name.lower() == "sr2sumuh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae00b00
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 0 # m 
        asm.asm_str = "sr2sumuh vsd[m], vrs"
    elif asm.name.lower() == "sr4sumuh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae01300
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 0 # m 
        asm.asm_str = "sr4sumuh vsd[m], vrs"
    elif asm.name.lower() == "sr8sumuh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae01b00
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 0 # m 
        asm.asm_str = "sr8sumuh vsd[m], vrs"
    elif asm.name.lower() == "sr16sumuh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae02300
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 0 # m 
        asm.asm_str = "sr16sumuh vsd[m], vrs"
    elif asm.name.lower() == "sr1sumsb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae00400
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "sr1sumsb vsd[m], vrs"
    elif asm.name.lower() == "sr2sumsb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae00c00
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 0 # m 
        asm.asm_str = "sr2sumsb vsd[m], vrs"
    elif asm.name.lower() == "sr4sumsb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae01400
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 0 # m 
        asm.asm_str = "sr4sumsb vsd[m], vrs"
    elif asm.name.lower() == "sr8sumsb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae01c00
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 0 # m 
        asm.asm_str = "sr8sumsb vsd[m], vrs"
    elif asm.name.lower() == "sr16sumsb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae02400
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 0 # m 
        asm.asm_str = "sr16sumsb vsd[m], vrs"
    elif asm.name.lower() == "sr1sumsh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae00500
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "sr1sumsh vsd[m], vrs"
    elif asm.name.lower() == "sr2sumsh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae00d00
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 0 # m 
        asm.asm_str = "sr2sumsh vsd[m], vrs"
    elif asm.name.lower() == "sr4sumsh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae01500
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 0 # m 
        asm.asm_str = "sr4sumsh vsd[m], vrs"
    elif asm.name.lower() == "sr8sumsh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae01d00
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 0 # m 
        asm.asm_str = "sr8sumsh vsd[m], vrs"
    elif asm.name.lower() == "sr16sumsh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae02500
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 0 # m 
        asm.asm_str = "sr16sumsh vsd[m], vrs"
    elif asm.name.lower() == "sr1sumw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae00600
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "sr1sumw vsd[m], vrs"
    elif asm.name.lower() == "sr2sumw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae00e00
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 0 # m 
        asm.asm_str = "sr2sumw vsd[m], vrs"
    elif asm.name.lower() == "sr4sumw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae01600
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 0 # m 
        asm.asm_str = "sr4sumw vsd[m], vrs"
    elif asm.name.lower() == "sr8sumw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae01e00
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 0 # m 
        asm.asm_str = "sr8sumw vsd[m], vrs"
    elif asm.name.lower() == "sr16sumw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4ae02600
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 0 # m 
        asm.asm_str = "sr16sumw vsd[m], vrs"
    elif asm.name.lower() == "absb":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a80000c
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "absb vrd, vrs"
    elif asm.name.lower() == "absh":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a80000d
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "absh vrd, vrs"
    elif asm.name.lower() == "absw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a80000e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "absw vrd, vrs"
    elif asm.name.lower() == "mulb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a600020
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "mulb vrd, vrs, vrp"
    elif asm.name.lower() == "mulh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a600021
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "mulh vrd, vrs, vrp"
    elif asm.name.lower() == "mulw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a600022
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "mulw vrd, vrs, vrp"
    elif asm.name.lower() == "smulbe":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a60002c
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "smulbe vrd, vrs, vrp"
    elif asm.name.lower() == "smulbo":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a60002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "smulbo vrd, vrs, vrp"
    elif asm.name.lower() == "smulhe":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a60002d
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "smulhe vrd, vrs, vrp"
    elif asm.name.lower() == "smulho":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a60002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "smulho vrd, vrs, vrp"
    elif asm.name.lower() == "umulbe":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a600028
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "umulbe vrd, vrs, vrp"
    elif asm.name.lower() == "umulbo":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a60002a
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "umulbo vrd, vrs, vrp"
    elif asm.name.lower() == "umulhe":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a600029
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "umulhe vrd, vrs, vrp"
    elif asm.name.lower() == "umulho":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a60002b
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "umulho vrd, vrs, vrp"
    elif asm.name.lower() == "wsmulbl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b60002c
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wsmulbl vrd, vrs, vrp"
    elif asm.name.lower() == "wsmulbh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b60002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wsmulbh vrd, vrs, vrp"
    elif asm.name.lower() == "wsmulhl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b60002d
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wsmulhl vrd, vrs, vrp"
    elif asm.name.lower() == "wsmulhh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b60002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wsmulhh vrd, vrs, vrp"
    elif asm.name.lower() == "wumulbl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b600028
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wumulbl vrd, vrs, vrp"
    elif asm.name.lower() == "wumulbh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b60002a
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wumulbh vrd, vrs, vrp"
    elif asm.name.lower() == "wumulhl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b600029
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wumulhl vrd, vrs, vrp"
    elif asm.name.lower() == "wumulhh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b60002b
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "wumulhh vrd, vrs, vrp"
    elif asm.name.lower() == "mlaw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a600038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "mlaw vsd, vrs, vrp"
    elif asm.name.lower() == "mlsw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a60003c
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "mlsw vsd, vrs, vrp"
    elif asm.name.lower() == "smlahe":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a600039
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "smlahe vsd, vrs, vrp"
    elif asm.name.lower() == "smlaho":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a60003b
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "smlaho vsd, vrs, vrp"
    elif asm.name.lower() == "smlshe":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a60003d
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "smlshe vsd, vrs, vrp"
    elif asm.name.lower() == "smlsho":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a60003f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "smlsho vsd, vrs, vrp"
    elif asm.name.lower() == "wsmlahl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b600039
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "wsmlahl vsd, vrs, vrp"
    elif asm.name.lower() == "wsmlahh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b60003b
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "wsmlahh vsd, vrs, vrp"
    elif asm.name.lower() == "wsmlshl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b60003d
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "wsmlshl vsd, vrs, vrp"
    elif asm.name.lower() == "wsmlshh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b60003f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "wsmlshh vsd, vrs, vrp"
    elif asm.name.lower() == "sr1mac2bi":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4b800000
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[4] >> 0) & 0x1f) << 0 # n 
        asm.asm_str = "sr1mac2bi vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr2mac2bi":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4b800020
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 4 # m 
        insn |= ((asm.field[4] >> 0) & 0xf) << 0 # n 
        asm.asm_str = "sr2mac2bi vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr4mac2bi":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4ba00000
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 3 # m 
        insn |= ((asm.field[4] >> 0) & 0x7) << 0 # n 
        asm.asm_str = "sr4mac2bi vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr8mac2bi":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4ba00020
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 2 # m 
        insn |= ((asm.field[4] >> 0) & 0x3) << 0 # n 
        asm.asm_str = "sr8mac2bi vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr16mac2bi":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4bc00000
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 1 # m 
        insn |= ((asm.field[4] >> 0) & 0x1) << 0 # n 
        asm.asm_str = "sr16mac2bi vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr1mac4bi":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4b800100
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[4] >> 0) & 0x1f) << 0 # n 
        asm.asm_str = "sr1mac4bi vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr2mac4bi":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4b800120
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 4 # m 
        insn |= ((asm.field[4] >> 0) & 0xf) << 0 # n 
        asm.asm_str = "sr2mac4bi vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr4mac4bi":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4ba00100
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 3 # m 
        insn |= ((asm.field[4] >> 0) & 0x7) << 0 # n 
        asm.asm_str = "sr4mac4bi vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr8mac4bi":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4ba00120
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 2 # m 
        insn |= ((asm.field[4] >> 0) & 0x3) << 0 # n 
        asm.asm_str = "sr8mac4bi vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr16mac4bi":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4bc00100
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 1 # m 
        insn |= ((asm.field[4] >> 0) & 0x1) << 0 # n 
        asm.asm_str = "sr16mac4bi vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr1macuub":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4b800200
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[4] >> 0) & 0x1f) << 0 # n 
        asm.asm_str = "sr1macuub vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr2macuub":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4b800220
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 4 # m 
        insn |= ((asm.field[4] >> 0) & 0xf) << 0 # n 
        asm.asm_str = "sr2macuub vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr4macuub":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4ba00200
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 3 # m 
        insn |= ((asm.field[4] >> 0) & 0x7) << 0 # n 
        asm.asm_str = "sr4macuub vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr8macuub":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4ba00220
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 2 # m 
        insn |= ((asm.field[4] >> 0) & 0x3) << 0 # n 
        asm.asm_str = "sr8macuub vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr16macuub":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4bc00200
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 1 # m 
        insn |= ((asm.field[4] >> 0) & 0x1) << 0 # n 
        asm.asm_str = "sr16macuub vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr1macsub":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4b800600
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[4] >> 0) & 0x1f) << 0 # n 
        asm.asm_str = "sr1macsub vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr2macsub":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4b800620
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 4 # m 
        insn |= ((asm.field[4] >> 0) & 0xf) << 0 # n 
        asm.asm_str = "sr2macsub vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr4macsub":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4ba00600
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 3 # m 
        insn |= ((asm.field[4] >> 0) & 0x7) << 0 # n 
        asm.asm_str = "sr4macsub vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr8macsub":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4ba00620
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 2 # m 
        insn |= ((asm.field[4] >> 0) & 0x3) << 0 # n 
        asm.asm_str = "sr8macsub vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr16macsub":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4bc00600
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 1 # m 
        insn |= ((asm.field[4] >> 0) & 0x1) << 0 # n 
        asm.asm_str = "sr16macsub vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr1macssb":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4b800400
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[4] >> 0) & 0x1f) << 0 # n 
        asm.asm_str = "sr1macssb vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr2macssb":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4b800420
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 4 # m 
        insn |= ((asm.field[4] >> 0) & 0xf) << 0 # n 
        asm.asm_str = "sr2macssb vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr4macssb":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4ba00400
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 3 # m 
        insn |= ((asm.field[4] >> 0) & 0x7) << 0 # n 
        asm.asm_str = "sr4macssb vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr8macssb":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4ba00420
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 2 # m 
        insn |= ((asm.field[4] >> 0) & 0x3) << 0 # n 
        asm.asm_str = "sr8macssb vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr16macssb":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4bc00400
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 1 # m 
        insn |= ((asm.field[4] >> 0) & 0x1) << 0 # n 
        asm.asm_str = "sr16macssb vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr1macssh":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4b800500
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[4] >> 0) & 0x1f) << 0 # n 
        asm.asm_str = "sr1macssh vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr2macssh":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4b800520
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 4 # m 
        insn |= ((asm.field[4] >> 0) & 0xf) << 0 # n 
        asm.asm_str = "sr2macssh vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr4macssh":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4ba00500
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 3 # m 
        insn |= ((asm.field[4] >> 0) & 0x7) << 0 # n 
        asm.asm_str = "sr4macssh vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr8macssh":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4ba00520
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 2 # m 
        insn |= ((asm.field[4] >> 0) & 0x3) << 0 # n 
        asm.asm_str = "sr8macssh vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "sr16macssh":
        if (len(asm.field) != 5) and (5 != 0):
            return 'error asm format'

        insn = 0x4bc00500
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 1 # m 
        insn |= ((asm.field[4] >> 0) & 0x1) << 0 # n 
        asm.asm_str = "sr16macssh vsd[m], vrs, vrp[n]"
    elif asm.name.lower() == "s1macuub":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4b800720
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "s1macuub vsd[m], vrs, vrp"
    elif asm.name.lower() == "s2macuub":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4b800710
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 5 # m 
        asm.asm_str = "s2macuub vsd[m], vrs, vrp"
    elif asm.name.lower() == "s4macuub":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4b800708
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 4 # m 
        asm.asm_str = "s4macuub vsd[m], vrs, vrp"
    elif asm.name.lower() == "s8macuub":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4b800704
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 3 # m 
        asm.asm_str = "s8macuub vsd[m], vrs, vrp"
    elif asm.name.lower() == "s16macuub":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4b800702
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 2 # m 
        asm.asm_str = "s16macuub vsd[m], vrs, vrp"
    elif asm.name.lower() == "s1macsub":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4ba00720
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "s1macsub vsd[m], vrs, vrp"
    elif asm.name.lower() == "s2macsub":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4ba00710
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 5 # m 
        asm.asm_str = "s2macsub vsd[m], vrs, vrp"
    elif asm.name.lower() == "s4macsub":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4ba00708
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 4 # m 
        asm.asm_str = "s4macsub vsd[m], vrs, vrp"
    elif asm.name.lower() == "s8macsub":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4ba00704
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 3 # m 
        asm.asm_str = "s8macsub vsd[m], vrs, vrp"
    elif asm.name.lower() == "s16macsub":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4ba00702
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 2 # m 
        asm.asm_str = "s16macsub vsd[m], vrs, vrp"
    elif asm.name.lower() == "s1macssb":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4bc00720
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "s1macssb vsd[m], vrs, vrp"
    elif asm.name.lower() == "s2macssb":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4bc00710
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 5 # m 
        asm.asm_str = "s2macssb vsd[m], vrs, vrp"
    elif asm.name.lower() == "s4macssb":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4bc00708
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 4 # m 
        asm.asm_str = "s4macssb vsd[m], vrs, vrp"
    elif asm.name.lower() == "s8macssb":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4bc00704
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 3 # m 
        asm.asm_str = "s8macssb vsd[m], vrs, vrp"
    elif asm.name.lower() == "s16macssb":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4bc00702
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 2 # m 
        asm.asm_str = "s16macssb vsd[m], vrs, vrp"
    elif asm.name.lower() == "s1macssh":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4be00720
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "s1macssh vsd[m], vrs, vrp"
    elif asm.name.lower() == "s2macssh":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4be00710
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x1) << 5 # m 
        asm.asm_str = "s2macssh vsd[m], vrs, vrp"
    elif asm.name.lower() == "s4macssh":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4be00708
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x3) << 4 # m 
        asm.asm_str = "s4macssh vsd[m], vrs, vrp"
    elif asm.name.lower() == "s8macssh":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4be00704
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0x7) << 3 # m 
        asm.asm_str = "s8macssh vsd[m], vrs, vrp"
    elif asm.name.lower() == "s16macssh":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x4be00702
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        insn |= ((asm.field[1] >> 0) & 0xf) << 2 # m 
        asm.asm_str = "s16macssh vsd[m], vrs, vrp"
    elif asm.name.lower() == "maxab":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000018
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "maxab vrd, vrs, vrp"
    elif asm.name.lower() == "maxah":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000019
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "maxah vrd, vrs, vrp"
    elif asm.name.lower() == "maxaw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00001a
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "maxaw vrd, vrs, vrp"
    elif asm.name.lower() == "maxsb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00001c
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "maxsb vrd, vrs, vrp"
    elif asm.name.lower() == "maxsh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00001d
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "maxsh vrd, vrs, vrp"
    elif asm.name.lower() == "maxsw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00001e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "maxsw vrd, vrs, vrp"
    elif asm.name.lower() == "maxub":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000008
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "maxub vrd, vrs, vrp"
    elif asm.name.lower() == "maxuh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000009
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "maxuh vrd, vrs, vrp"
    elif asm.name.lower() == "maxuw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00000a
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "maxuw vrd, vrs, vrp"
    elif asm.name.lower() == "maxu2bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00000c
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "maxu2bi vrd, vrs, vrp"
    elif asm.name.lower() == "maxu4bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00000d
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "maxu4bi vrd, vrs, vrp"
    elif asm.name.lower() == "minab":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000010
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "minab vrd, vrs, vrp"
    elif asm.name.lower() == "minah":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000011
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "minah vrd, vrs, vrp"
    elif asm.name.lower() == "minaw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000012
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "minaw vrd, vrs, vrp"
    elif asm.name.lower() == "minsb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000014
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "minsb vrd, vrs, vrp"
    elif asm.name.lower() == "minsh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000015
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "minsh vrd, vrs, vrp"
    elif asm.name.lower() == "minsw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000016
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "minsw vrd, vrs, vrp"
    elif asm.name.lower() == "minub":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000000
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "minub vrd, vrs, vrp"
    elif asm.name.lower() == "minuh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000001
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "minuh vrd, vrs, vrp"
    elif asm.name.lower() == "minuw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000002
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "minuw vrd, vrs, vrp"
    elif asm.name.lower() == "minu2bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000004
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "minu2bi vrd, vrs, vrp"
    elif asm.name.lower() == "minu4bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000005
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "minu4bi vrd, vrs, vrp"
    elif asm.name.lower() == "satsshb":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7120002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satsshb vrd, vrs"
    elif asm.name.lower() == "satsswb":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7140002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satsswb vrd, vrs"
    elif asm.name.lower() == "satsswh":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71c0002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satsswh vrd, vrs"
    elif asm.name.lower() == "satsub2bi":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7200002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satsub2bi vrd, vrs"
    elif asm.name.lower() == "satsub4bi":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7280002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satsub4bi vrd, vrs"
    elif asm.name.lower() == "satsuh2bi":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7220002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satsuh2bi vrd, vrs"
    elif asm.name.lower() == "satsuh4bi":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x72a0002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satsuh4bi vrd, vrs"
    elif asm.name.lower() == "satsuhb":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7320002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satsuhb vrd, vrs"
    elif asm.name.lower() == "satsuw2bi":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7240002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satsuw2bi vrd, vrs"
    elif asm.name.lower() == "satsuw4bi":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x72c0002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satsuw4bi vrd, vrs"
    elif asm.name.lower() == "satsuwb":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7340002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satsuwb vrd, vrs"
    elif asm.name.lower() == "satsuwh":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x73c0002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satsuwh vrd, vrs"
    elif asm.name.lower() == "satuub2bi":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7000002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satuub2bi vrd, vrs"
    elif asm.name.lower() == "satuub4bi":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7020002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satuub4bi vrd, vrs"
    elif asm.name.lower() == "satuuh2bi":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7040002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satuuh2bi vrd, vrs"
    elif asm.name.lower() == "satuuh4bi":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7060002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satuuh4bi vrd, vrs"
    elif asm.name.lower() == "satuuhb":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7080002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satuuhb vrd, vrs"
    elif asm.name.lower() == "satuuw4bi":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x70a0002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satuuw4bi vrd, vrs"
    elif asm.name.lower() == "satuuwb":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x70c0002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satuuwb vrd, vrs"
    elif asm.name.lower() == "satuuwh":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x70e0002f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "satuuwh vrd, vrs"
    elif asm.name.lower() == "tocb":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a600018
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "tocb vsd, vrs"
    elif asm.name.lower() == "toch":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a600019
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "toch vsd, vrs"
    elif asm.name.lower() == "tocw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a60001a
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "tocw vsd, vrs"
    elif asm.name.lower() == "andv":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a600002
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "andv vrd, vrs, vrp"
    elif asm.name.lower() == "andnv":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a600003
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "andnv vrd, vrs, vrp"
    elif asm.name.lower() == "andib":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b600008
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        insn |= ((asm.field[2] >> 5) & 0x7) << 0 # imm 
        asm.asm_str = "andib vrd, vrs, imm"
    elif asm.name.lower() == "orv":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a600004
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "orv vrd, vrs, vrp"
    elif asm.name.lower() == "ornv":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a600005
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ornv vrd, vrs, vrp"
    elif asm.name.lower() == "orib":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b600010
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        insn |= ((asm.field[2] >> 5) & 0x7) << 0 # imm 
        asm.asm_str = "orib vrd, vrs, imm"
    elif asm.name.lower() == "xorv":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a600006
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "xorv vrd, vrs, vrp"
    elif asm.name.lower() == "xornv":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a600007
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "xornv vrd, vrs, vrp"
    elif asm.name.lower() == "xorib":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b600018
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        insn |= ((asm.field[2] >> 5) & 0x7) << 0 # imm 
        asm.asm_str = "xorib vrd, vrs, imm"
    elif asm.name.lower() == "bselv":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a60000e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "bselv vrd, vrs, vrp"
    elif asm.name.lower() == "faddw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a800003
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "faddw vrd, vrs, vrp"
    elif asm.name.lower() == "fsubw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a80000b
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fsubw vrd, vrs, vrp"
    elif asm.name.lower() == "fmulw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a600023
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fmulw vrd, vrs, vrp"
    elif asm.name.lower() == "fcmulrw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b600030
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fcmulrw vrd, vrs, vrp"
    elif asm.name.lower() == "fcmuliw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4b600032
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fcmuliw vrd, vrs, vrp"
    elif asm.name.lower() == "fcaddw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a60003a
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fcaddw vrd, vrs, vrp"
    elif asm.name.lower() == "fxas1w":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a60003e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fxas1w vrd, vrp"
    elif asm.name.lower() == "fxas2w":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a61003e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fxas2w vrd, vrp"
    elif asm.name.lower() == "fxas4w":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a62003e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fxas4w vrd, vrp"
    elif asm.name.lower() == "fxas8w":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a63003e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fxas8w vrd, vrp"
    elif asm.name.lower() == "fmaxw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00001f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fmaxw vrd, vrs, vrp"
    elif asm.name.lower() == "fmaxaw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00001b
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fmaxaw vrd, vrs, vrp"
    elif asm.name.lower() == "fminw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000017
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fminw vrd, vrs, vrp"
    elif asm.name.lower() == "fminaw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000013
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fminaw vrd, vrs, vrp"
    elif asm.name.lower() == "fclassw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4900002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fclassw vrd, vrs"
    elif asm.name.lower() == "fceqw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00002b
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fceqw vrd, vrs, vrp"
    elif asm.name.lower() == "fclew":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a00003f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fclew vrd, vrs, vrp"
    elif asm.name.lower() == "fcltw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000037
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fcltw vrd, vrs, vrp"
    elif asm.name.lower() == "fcorw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a000033
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "fcorw vrd, vrs, vrp"
    elif asm.name.lower() == "ffsiw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7040002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ffsiw vrd, vrs"
    elif asm.name.lower() == "ffuiw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7000002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ffuiw vrd, vrs"
    elif asm.name.lower() == "ftsiw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x70e0002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ftsiw vrd, vrs"
    elif asm.name.lower() == "ftuiw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x70a0002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ftuiw vrd, vrs"
    elif asm.name.lower() == "frintw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7020002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "frintw vrd, vrs"
    elif asm.name.lower() == "ftruncsw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x70c0002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ftruncsw vrd, vrs"
    elif asm.name.lower() == "ftruncuw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7080002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ftruncuw vrd, vrs"
    elif asm.name.lower() == "sllb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a200020
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "sllb vrd, vrs, vrp"
    elif asm.name.lower() == "sllh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a200021
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "sllh vrd, vrs, vrp"
    elif asm.name.lower() == "sllw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a200022
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "sllw vrd, vrs, vrp"
    elif asm.name.lower() == "sllib":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4aa00020
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "sllib vrd, vrs, imm"
    elif asm.name.lower() == "sllih":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4aa00021
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "sllih vrd, vrs, imm"
    elif asm.name.lower() == "slliw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4aa00022
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "slliw vrd, vrs, imm"
    elif asm.name.lower() == "srab":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a200038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srab vrd, vrs, vrp"
    elif asm.name.lower() == "srah":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a200039
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srah vrd, vrs, vrp"
    elif asm.name.lower() == "sraw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a20003a
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "sraw vrd, vrs, vrp"
    elif asm.name.lower() == "sraib":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4aa00038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "sraib vrd, vrs, imm"
    elif asm.name.lower() == "sraih":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4aa00039
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "sraih vrd, vrs, imm"
    elif asm.name.lower() == "sraiw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4aa0003a
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "sraiw vrd, vrs, imm"
    elif asm.name.lower() == "srarb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a20003c
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srarb vrd, vrs, vrp"
    elif asm.name.lower() == "srarh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a20003d
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srarh vrd, vrs, vrp"
    elif asm.name.lower() == "srarw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a20003e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srarw vrd, vrs, vrp"
    elif asm.name.lower() == "srarib":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4aa0003c
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srarib vrd, vrs, imm"
    elif asm.name.lower() == "srarih":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4aa0003d
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srarih vrd, vrs, imm"
    elif asm.name.lower() == "srariw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4aa0003e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srariw vrd, vrs, imm"
    elif asm.name.lower() == "srlb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a200030
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srlb vrd, vrs, vrp"
    elif asm.name.lower() == "srlh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a200031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srlh vrd, vrs, vrp"
    elif asm.name.lower() == "srlw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a200032
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srlw vrd, vrs, vrp"
    elif asm.name.lower() == "srlib":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4aa00030
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srlib vrd, vrs, imm"
    elif asm.name.lower() == "srlih":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4aa00031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srlih vrd, vrs, imm"
    elif asm.name.lower() == "srliw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4aa00032
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srliw vrd, vrs, imm"
    elif asm.name.lower() == "srlrb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a200034
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srlrb vrd, vrs, vrp"
    elif asm.name.lower() == "srlrh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a200035
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srlrh vrd, vrs, vrp"
    elif asm.name.lower() == "srlrw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a200036
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srlrw vrd, vrs, vrp"
    elif asm.name.lower() == "srlrib":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4aa00034
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srlrib vrd, vrs, imm"
    elif asm.name.lower() == "srlrih":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4aa00035
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srlrih vrd, vrs, imm"
    elif asm.name.lower() == "srlriw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4aa00036
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "srlriw vrd, vrs, imm"
    elif asm.name.lower() == "gt1bi":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71c40034
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gt1bi vrd, vrp"
    elif asm.name.lower() == "gt2bi":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71c50034
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gt2bi vrd, vrp"
    elif asm.name.lower() == "gt4bi":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71c60034
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gt4bi vrd, vrp"
    elif asm.name.lower() == "gtb":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71c00034
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gtb vrd, vrp"
    elif asm.name.lower() == "gth":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71c10034
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gth vrd, vrp"
    elif asm.name.lower() == "gt2w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70000035
        insn |= ((asm.field[3] >> 0) & 0x1) << 21 # pos 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # m 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gt2w vrd[m], vrp, pos"
    elif asm.name.lower() == "gt4w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70040035
        insn |= ((asm.field[3] >> 0) & 0x3) << 21 # pos 
        insn |= ((asm.field[1] >> 0) & 0x3) << 16 # m 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gt4w vrd[m], vrp, pos"
    elif asm.name.lower() == "gt8w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70080035
        insn |= ((asm.field[3] >> 0) & 0x7) << 21 # pos 
        insn |= ((asm.field[1] >> 0) & 0x7) << 16 # m 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gt8w vrd[m], vrp, pos"
    elif asm.name.lower() == "gt2d":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x72020035
        insn |= ((asm.field[3] >> 0) & 0x1) << 21 # pos 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # m 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gt2d vrd[m], vrp, pos"
    elif asm.name.lower() == "gt4d":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x72040035
        insn |= ((asm.field[3] >> 0) & 0x3) << 21 # pos 
        insn |= ((asm.field[1] >> 0) & 0x3) << 16 # m 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gt4d vrd[m], vrp, pos"
    elif asm.name.lower() == "gt2q":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x73020035
        insn |= ((asm.field[3] >> 0) & 0x1) << 21 # pos 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # m 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gt2q vrd[m], vrp, pos"
    elif asm.name.lower() == "extuwll":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72000031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extuwll vrd, vrp, w"
    elif asm.name.lower() == "extuwlh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72080031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extuwlh vrd, vrp, w"
    elif asm.name.lower() == "extuwhl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72100031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extuwhl vrd, vrp, w"
    elif asm.name.lower() == "extuwhh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72180031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extuwhh vrd, vrp, w"
    elif asm.name.lower() == "extudll":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72010031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extudll vrd, vrp, w"
    elif asm.name.lower() == "extudlh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72090031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extudlh vrd, vrp, w"
    elif asm.name.lower() == "extudhl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72110031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extudhl vrd, vrp, w"
    elif asm.name.lower() == "extudhh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72190031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extudhh vrd, vrp, w"
    elif asm.name.lower() == "extuqll":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72020031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extuqll vrd, vrp, w"
    elif asm.name.lower() == "extuqlh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x720a0031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extuqlh vrd, vrp, w"
    elif asm.name.lower() == "extuqhl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72120031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extuqhl vrd, vrp, w"
    elif asm.name.lower() == "extuqhh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x721a0031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extuqhh vrd, vrp, w"
    elif asm.name.lower() == "extuoll":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72030031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extuoll vrd, vrp, w"
    elif asm.name.lower() == "extuolh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x720b0031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extuolh vrd, vrp, w"
    elif asm.name.lower() == "extuohl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72130031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extuohl vrd, vrp, w"
    elif asm.name.lower() == "extuohh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x721b0031
        insn |= ((asm.field[2] >> 0) & 0x1) << 21 # w 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extuohh vrd, vrp, w"
    elif asm.name.lower() == "extu1bil":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71a40031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extu1bil vrd, vrp"
    elif asm.name.lower() == "extu2bil":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71a50031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extu2bil vrd, vrp"
    elif asm.name.lower() == "extu4bil":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71a60031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extu4bil vrd, vrp"
    elif asm.name.lower() == "extubl":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71a00031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extubl vrd, vrp"
    elif asm.name.lower() == "extuhl":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71a10031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extuhl vrd, vrp"
    elif asm.name.lower() == "extu1bih":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71ac0031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extu1bih vrd, vrp"
    elif asm.name.lower() == "extu2bih":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71ad0031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extu2bih vrd, vrp"
    elif asm.name.lower() == "extu4bih":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71ae0031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extu4bih vrd, vrp"
    elif asm.name.lower() == "extubh":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71a80031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extubh vrd, vrp"
    elif asm.name.lower() == "extuhh":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71a90031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extuhh vrd, vrp"
    elif asm.name.lower() == "exts1bil":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71b40031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "exts1bil vrd, vrp"
    elif asm.name.lower() == "exts2bil":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71b50031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "exts2bil vrd, vrp"
    elif asm.name.lower() == "exts4bil":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71b60031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "exts4bil vrd, vrp"
    elif asm.name.lower() == "extsbl":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71b00031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extsbl vrd, vrp"
    elif asm.name.lower() == "extshl":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71b10031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extshl vrd, vrp"
    elif asm.name.lower() == "exts1bih":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71bc0031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "exts1bih vrd, vrp"
    elif asm.name.lower() == "exts2bih":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71bd0031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "exts2bih vrd, vrp"
    elif asm.name.lower() == "exts4bih":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71be0031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "exts4bih vrd, vrp"
    elif asm.name.lower() == "extsbh":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71b80031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extsbh vrd, vrp"
    elif asm.name.lower() == "extshh":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71b90031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extshh vrd, vrp"
    elif asm.name.lower() == "extu3bw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71800031
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "extu3bw vrd, vrp"
    elif asm.name.lower() == "repib":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70000030
        insn |= ((asm.field[2] >> 0) & 0x7f) << 16 # n 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "repib vrd, vrp[n]"
    elif asm.name.lower() == "repih":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70800030
        insn |= ((asm.field[2] >> 0) & 0x3f) << 16 # n 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "repih vrd, vrp[n]"
    elif asm.name.lower() == "repiw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70c00030
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # n 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "repiw vrd, vrp[n]"
    elif asm.name.lower() == "repid":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70e00030
        insn |= ((asm.field[2] >> 0) & 0xf) << 16 # n 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "repid vrd, vrp[n]"
    elif asm.name.lower() == "repiq":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70f00030
        insn |= ((asm.field[2] >> 0) & 0x7) << 16 # n 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "repiq vrd, vrp[n]"
    elif asm.name.lower() == "repio":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70f80030
        insn |= ((asm.field[2] >> 0) & 0x3) << 16 # n 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "repio vrd, vrp[n]"
    elif asm.name.lower() == "movw":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70000036
        insn |= ((asm.field[1] >> 0) & 0x1f) << 21 # m 
        insn |= ((asm.field[3] >> 0) & 0x1f) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "movw vrd[m], vrp[n]"
    elif asm.name.lower() == "movd":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70000037
        insn |= ((asm.field[1] >> 0) & 0xf) << 21 # m 
        insn |= ((asm.field[3] >> 0) & 0xf) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "movd vrd[m], vrp[n]"
    elif asm.name.lower() == "movq":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70100037
        insn |= ((asm.field[1] >> 0) & 0xf) << 21 # m 
        insn |= ((asm.field[3] >> 0) & 0xf) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "movq vrd[m], vrp[n]"
    elif asm.name.lower() == "movo":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x72000037
        insn |= ((asm.field[1] >> 0) & 0xf) << 21 # m 
        insn |= ((asm.field[3] >> 0) & 0xf) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "movo vrd[m], vrp[n]"
    elif asm.name.lower() == "shufw2":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x70000033
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "shufw2 vrd, vrp"
    elif asm.name.lower() == "shufw4":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x70800033
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "shufw4 vrd, vrp"
    elif asm.name.lower() == "shufw8":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x71000033
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "shufw8 vrd, vrp"
    elif asm.name.lower() == "shufd2":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x70200033
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "shufd2 vrd, vrp"
    elif asm.name.lower() == "shufd4":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x70a00033
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "shufd4 vrd, vrp"
    elif asm.name.lower() == "shufq2":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x70400033
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "shufq2 vrd, vrp"
    elif asm.name.lower() == "gshufw":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7200003b
        insn |= ((asm.field[3] >> 0) & 0x3) << 21 # imm 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gshufw vrd, vrs, vrp, imm"
    elif asm.name.lower() == "gshufwb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x7300003b
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gshufwb vrd, vrs, vrp"
    elif asm.name.lower() == "gshufb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x7020002b
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gshufb vrd, vrs, vrp"
    elif asm.name.lower() == "gshufwh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x7200002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gshufwh vrd, vrs, vrp"
    elif asm.name.lower() == "gshufh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x7280002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "gshufh vrd, vrs, vrp"
    elif asm.name.lower() == "ilve2bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70000038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilve2bi vrd, vrs, vrp"
    elif asm.name.lower() == "ilve4bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70200038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilve4bi vrd, vrs, vrp"
    elif asm.name.lower() == "ilveb":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70400038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilveb vrd, vrs, vrp"
    elif asm.name.lower() == "ilveh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70600038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilveh vrd, vrs, vrp"
    elif asm.name.lower() == "ilvew":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70800038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilvew vrd, vrs, vrp"
    elif asm.name.lower() == "ilved":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70a00038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilved vrd, vrs, vrp"
    elif asm.name.lower() == "ilveq":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70c00038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilveq vrd, vrs, vrp"
    elif asm.name.lower() == "ilveo":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70e00038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilveo vrd, vrs, vrp"
    elif asm.name.lower() == "ilvo2bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72000038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilvo2bi vrd, vrs, vrp"
    elif asm.name.lower() == "ilvo4bi":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72200038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilvo4bi vrd, vrs, vrp"
    elif asm.name.lower() == "ilvob":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72400038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilvob vrd, vrs, vrp"
    elif asm.name.lower() == "ilvoh":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72600038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilvoh vrd, vrs, vrp"
    elif asm.name.lower() == "ilvow":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72800038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilvow vrd, vrs, vrp"
    elif asm.name.lower() == "ilvod":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72a00038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilvod vrd, vrs, vrp"
    elif asm.name.lower() == "ilvoq":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72c00038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilvoq vrd, vrs, vrp"
    elif asm.name.lower() == "ilvoo":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x72e00038
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "ilvoo vrd, vrs, vrp"
    elif asm.name.lower() == "bshli":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70000039
        insn |= ((asm.field[2] >> 0) & 0x7f) << 16 # imm 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "bshli vrd, vrp, imm"
    elif asm.name.lower() == "bshri":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x71000039
        insn |= ((asm.field[2] >> 0) & 0x7f) << 16 # imm 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "bshri vrd, vrp, imm"
    elif asm.name.lower() == "bshl":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70800039
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "bshl vrd, vrs, vrp"
    elif asm.name.lower() == "bshr":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x71800039
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "bshr vrd, vrs, vrp"
    elif asm.name.lower() == "mtcpuw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7340002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vwrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # rd 
        asm.asm_str = "mtcpuw rd, vwrp"
    elif asm.name.lower() == "mfcpuw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x701b0003
        insn |= ((asm.field[1] >> 0) & 0x1f) << 21 # rs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vwrd 
        asm.asm_str = "mfcpuw vwrd, rs"
    elif asm.name.lower() == "mtfpuw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7300002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # vwrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # fd 
        asm.asm_str = "mtfpuw fd, vwrp"
    elif asm.name.lower() == "mffpuw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7320002e
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # fs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vwrd 
        asm.asm_str = "mffpuw vwrd, fs"
    elif asm.name.lower() == "ctcmxu":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x70110003
        insn |= ((asm.field[1] >> 0) & 0x1f) << 21 # rs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # mcd 
        asm.asm_str = "ctcmxu mcd, rs"
    elif asm.name.lower() == "cfcmxu":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x70100003
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # mcs 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # rd 
        asm.asm_str = "cfcmxu rd, mcs"
    elif asm.name.lower() == "sumz":
        if (len(asm.field) != 1) and (1 != 0):
            return 'error asm format'

        insn = 0x4a60001c
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "sumz vsd"
    elif asm.name.lower() == "mfsum":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a60000f
        insn |= ((asm.field[1] >> 0) & 0x3) << 11 # vss 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "mfsum vrd, vss"
    elif asm.name.lower() == "mfsumz":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a60001e
        insn |= ((asm.field[1] >> 0) & 0x3) << 11 # vsd 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "mfsumz vrd, vsd"
    elif asm.name.lower() == "mtsum":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a60001d
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[0] >> 0) & 0x3) << 6 # vsd 
        asm.asm_str = "mtsum vsd, vrs"
    elif asm.name.lower() == "mxsum":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a60001f
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x3) << 11 # vsd 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "mxsum vrd, vrs, vsd"
    elif asm.name.lower() == "lih":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4b000000
        insn |= ((asm.field[1] >> 11) & 0x1f) << 16 # imm 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        insn |= ((asm.field[1] >> 5) & 0x3f) << 0 # imm 
        asm.asm_str = "lih vrd, imm"
    elif asm.name.lower() == "liw":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4b200000
        insn |= ((asm.field[1] >> 11) & 0x1f) << 16 # imm 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        insn |= ((asm.field[1] >> 5) & 0x3f) << 0 # imm 
        asm.asm_str = "liw vrd, imm"
    elif asm.name.lower() == "liwh":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4b400000
        insn |= ((asm.field[1] >> 11) & 0x1f) << 16 # imm 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        insn |= ((asm.field[1] >> 5) & 0x3f) << 0 # imm 
        asm.asm_str = "liwh vrd, imm"
    elif asm.name.lower() == "liwr":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x4a600000
        insn |= ((asm.field[1] >> 0) & 0x3ff) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vwrd 
        asm.asm_str = "liwr vwrd, imm"
    elif asm.name.lower() == "cmvw":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70000032
        insn |= ((asm.field[3] >> 0) & 0xf) << 21 # imm 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "cmvw vrd, vrs, vrp, imm"
    elif asm.name.lower() == "pmaph":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a600009
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "pmaph vrd, vrs, vrp"
    elif asm.name.lower() == "pmapw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x4a60000a
        insn |= ((asm.field[1] >> 0) & 0x1f) << 16 # vrs 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "pmapw vrd, vrs, vrp"
    elif asm.name.lower() == "law":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70000010
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # offset 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # n 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "law vrd[n], offset(base)"
    elif asm.name.lower() == "lad":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70000011
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # offset 
        insn |= ((asm.field[1] >> 0) & 0xf) << 12 # n 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lad vrd[n], offset(base)"
    elif asm.name.lower() == "laq":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70000811
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # offset 
        insn |= ((asm.field[1] >> 0) & 0x7) << 13 # n 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "laq vrd[n], offset(base)"
    elif asm.name.lower() == "lao":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70001811
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # offset 
        insn |= ((asm.field[1] >> 0) & 0x3) << 14 # n 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lao vrd[n], offset(base)"
    elif asm.name.lower() == "saw":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70000014
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # offset 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 6 # n 
        asm.asm_str = "saw vrp[n], offset(base)"
    elif asm.name.lower() == "sad":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70000015
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # offset 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0xf) << 7 # n 
        asm.asm_str = "sad vrp[n], offset(base)"
    elif asm.name.lower() == "saq":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70000055
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # offset 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x7) << 8 # n 
        asm.asm_str = "saq vrp[n], offset(base)"
    elif asm.name.lower() == "sao":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x700000d5
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[2] >> 0) & 0x1f) << 16 # offset 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x3) << 9 # n 
        asm.asm_str = "sao vrp[n], offset(base)"
    elif asm.name.lower() == "luw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70000012
        insn |= ((asm.field[2] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 11 # n 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luw vrd[n], base"
    elif asm.name.lower() == "lud":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70000013
        insn |= ((asm.field[2] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0xf) << 12 # n 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lud vrd[n], base"
    elif asm.name.lower() == "luq":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70000813
        insn |= ((asm.field[2] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x7) << 13 # n 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luq vrd[n], base"
    elif asm.name.lower() == "luo":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70001813
        insn |= ((asm.field[2] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 14 # n 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luo vrd[n], base"
    elif asm.name.lower() == "suw":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70000016
        insn |= ((asm.field[2] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 6 # n 
        asm.asm_str = "suw vrp[n], base"
    elif asm.name.lower() == "sud":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70000017
        insn |= ((asm.field[2] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0xf) << 7 # n 
        asm.asm_str = "sud vrp[n], base"
    elif asm.name.lower() == "suq":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x70000057
        insn |= ((asm.field[2] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x7) << 8 # n 
        asm.asm_str = "suq vrp[n], base"
    elif asm.name.lower() == "suo":
        if (len(asm.field) != 3) and (3 != 0):
            return 'error asm format'

        insn = 0x700000d7
        insn |= ((asm.field[2] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x3) << 9 # n 
        asm.asm_str = "suo vrp[n], base"
    elif asm.name.lower() == "law2b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70000019
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0xf) << 13 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "law2b vrd[n], p, base"
    elif asm.name.lower() == "law2h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70100019
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0xf) << 13 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "law2h vrd[n], p, base"
    elif asm.name.lower() == "law4b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70020019
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x7) << 14 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "law4b vrd[n], p, base"
    elif asm.name.lower() == "law4h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70120019
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x7) << 14 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "law4h vrd[n], p, base"
    elif asm.name.lower() == "law8b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70040019
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x7) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "law8b vrd[n], p, base"
    elif asm.name.lower() == "law8h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70140019
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x7) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "law8h vrd[n], p, base"
    elif asm.name.lower() == "law16b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70060019
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0xf) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "law16b vrd[n], p, base"
    elif asm.name.lower() == "law16h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70160019
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0xf) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "law16h vrd[n], p, base"
    elif asm.name.lower() == "lad2b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70000819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x7) << 14 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lad2b vrd[n], p, base"
    elif asm.name.lower() == "lad2h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70080819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x7) << 14 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lad2h vrd[n], p, base"
    elif asm.name.lower() == "lad2w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70100819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x7) << 14 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lad2w vrd[n], p, base"
    elif asm.name.lower() == "lad4b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70020819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lad4b vrd[n], p, base"
    elif asm.name.lower() == "lad4h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x700a0819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lad4h vrd[n], p, base"
    elif asm.name.lower() == "lad4w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70120819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lad4w vrd[n], p, base"
    elif asm.name.lower() == "lad8b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70040819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x7) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lad8b vrd[n], p, base"
    elif asm.name.lower() == "lad8h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x700c0819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x7) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lad8h vrd[n], p, base"
    elif asm.name.lower() == "lad8w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70140819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x7) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lad8w vrd[n], p, base"
    elif asm.name.lower() == "laq2b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70001819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "laq2b vrd[n], p, base"
    elif asm.name.lower() == "laq2h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70081819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "laq2h vrd[n], p, base"
    elif asm.name.lower() == "laq2w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70101819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "laq2w vrd[n], p, base"
    elif asm.name.lower() == "laq2d":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70181819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "laq2d vrd[n], p, base"
    elif asm.name.lower() == "laq4b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70021819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "laq4b vrd[n], p, base"
    elif asm.name.lower() == "laq4h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x700a1819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "laq4h vrd[n], p, base"
    elif asm.name.lower() == "laq4w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70121819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "laq4w vrd[n], p, base"
    elif asm.name.lower() == "laq4d":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x701a1819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "laq4d vrd[n], p, base"
    elif asm.name.lower() == "lao2b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70003819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 15 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lao2b vrd[n], p, base"
    elif asm.name.lower() == "lao2h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70043819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 15 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lao2h vrd[n], p, base"
    elif asm.name.lower() == "lao2w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70083819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 15 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lao2w vrd[n], p, base"
    elif asm.name.lower() == "lao2d":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x700c3819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 15 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lao2d vrd[n], p, base"
    elif asm.name.lower() == "lao2q":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x70103819
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 15 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lao2q vrd[n], p, base"
    elif asm.name.lower() == "luw2b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7000001b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0xf) << 13 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luw2b vrd[n], p, base"
    elif asm.name.lower() == "luw2h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7010001b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0xf) << 13 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luw2h vrd[n], p, base"
    elif asm.name.lower() == "luw4b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7002001b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x7) << 14 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luw4b vrd[n], p, base"
    elif asm.name.lower() == "luw4h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7012001b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x7) << 14 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luw4h vrd[n], p, base"
    elif asm.name.lower() == "luw8b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7004001b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x7) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luw8b vrd[n], p, base"
    elif asm.name.lower() == "luw8h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7014001b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x7) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luw8h vrd[n], p, base"
    elif asm.name.lower() == "luw16b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7006001b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0xf) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luw16b vrd[n], p, base"
    elif asm.name.lower() == "luw16h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7016001b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0xf) << 12 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luw16h vrd[n], p, base"
    elif asm.name.lower() == "lud2b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7000081b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x7) << 14 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lud2b vrd[n], p, base"
    elif asm.name.lower() == "lud2h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7008081b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x7) << 14 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lud2h vrd[n], p, base"
    elif asm.name.lower() == "lud2w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7010081b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x7) << 14 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lud2w vrd[n], p, base"
    elif asm.name.lower() == "lud4b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7002081b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lud4b vrd[n], p, base"
    elif asm.name.lower() == "lud4h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x700a081b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lud4h vrd[n], p, base"
    elif asm.name.lower() == "lud4w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7012081b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lud4w vrd[n], p, base"
    elif asm.name.lower() == "lud8b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7004081b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x7) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lud8b vrd[n], p, base"
    elif asm.name.lower() == "lud8h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x700c081b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x7) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lud8h vrd[n], p, base"
    elif asm.name.lower() == "lud8w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7014081b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x7) << 13 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "lud8w vrd[n], p, base"
    elif asm.name.lower() == "luq2b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7000181b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luq2b vrd[n], p, base"
    elif asm.name.lower() == "luq2h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7008181b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luq2h vrd[n], p, base"
    elif asm.name.lower() == "luq2w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7010181b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luq2w vrd[n], p, base"
    elif asm.name.lower() == "luq2d":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7018181b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x3) << 15 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luq2d vrd[n], p, base"
    elif asm.name.lower() == "luq4b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7002181b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luq4b vrd[n], p, base"
    elif asm.name.lower() == "luq4h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x700a181b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luq4h vrd[n], p, base"
    elif asm.name.lower() == "luq4w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7012181b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luq4w vrd[n], p, base"
    elif asm.name.lower() == "luq4d":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x701a181b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 14 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luq4d vrd[n], p, base"
    elif asm.name.lower() == "luo2b":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7000381b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 15 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luo2b vrd[n], p, base"
    elif asm.name.lower() == "luo2h":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7004381b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 15 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luo2h vrd[n], p, base"
    elif asm.name.lower() == "luo2w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7008381b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 15 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luo2w vrd[n], p, base"
    elif asm.name.lower() == "luo2d":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x700c381b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 15 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luo2d vrd[n], p, base"
    elif asm.name.lower() == "luo2q":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7010381b
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[1] >> 0) & 0x1) << 16 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 15 # p 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "luo2q vrd[n], p, base"
    elif asm.name.lower() == "sad2w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7000001d
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x7) << 8 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 7 # p 
        asm.asm_str = "sad2w vrp[n], p, base"
    elif asm.name.lower() == "sad4w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7008001d
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x3) << 9 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 7 # p 
        asm.asm_str = "sad4w vrp[n], p, base"
    elif asm.name.lower() == "sad8w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7010001d
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x1) << 10 # n 
        insn |= ((asm.field[2] >> 0) & 0x7) << 7 # p 
        asm.asm_str = "sad8w vrp[n], p, base"
    elif asm.name.lower() == "saq2w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7000005d
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x3) << 9 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 8 # p 
        asm.asm_str = "saq2w vrp[n], p, base"
    elif asm.name.lower() == "saq2d":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7010005d
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x3) << 9 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 8 # p 
        asm.asm_str = "saq2d vrp[n], p, base"
    elif asm.name.lower() == "saq4w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7004005d
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x1) << 10 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 8 # p 
        asm.asm_str = "saq4w vrp[n], p, base"
    elif asm.name.lower() == "saq4d":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7014005d
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x1) << 10 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 8 # p 
        asm.asm_str = "saq4d vrp[n], p, base"
    elif asm.name.lower() == "sao2w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x700000dd
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x1) << 10 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 9 # p 
        asm.asm_str = "sao2w vrp[n], p, base"
    elif asm.name.lower() == "sao2d":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x700800dd
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x1) << 10 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 9 # p 
        asm.asm_str = "sao2d vrp[n], p, base"
    elif asm.name.lower() == "sao2q":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x701000dd
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x1) << 10 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 9 # p 
        asm.asm_str = "sao2q vrp[n], p, base"
    elif asm.name.lower() == "sud2w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7000001f
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x7) << 8 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 7 # p 
        asm.asm_str = "sud2w vrp[n], p, base"
    elif asm.name.lower() == "sud4w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7008001f
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x3) << 9 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 7 # p 
        asm.asm_str = "sud4w vrp[n], p, base"
    elif asm.name.lower() == "sud8w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7010001f
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x1) << 10 # n 
        insn |= ((asm.field[2] >> 0) & 0x7) << 7 # p 
        asm.asm_str = "sud8w vrp[n], p, base"
    elif asm.name.lower() == "suq2w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7000005f
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x3) << 9 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 8 # p 
        asm.asm_str = "suq2w vrp[n], p, base"
    elif asm.name.lower() == "suq2d":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7010005f
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x3) << 9 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 8 # p 
        asm.asm_str = "suq2d vrp[n], p, base"
    elif asm.name.lower() == "suq4w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7004005f
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x1) << 10 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 8 # p 
        asm.asm_str = "suq4w vrp[n], p, base"
    elif asm.name.lower() == "suq4d":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x7014005f
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x1) << 10 # n 
        insn |= ((asm.field[2] >> 0) & 0x3) << 8 # p 
        asm.asm_str = "suq4d vrp[n], p, base"
    elif asm.name.lower() == "suo2w":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x700000df
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x1) << 10 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 9 # p 
        asm.asm_str = "suo2w vrp[n], p, base"
    elif asm.name.lower() == "suo2d":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x700800df
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x1) << 10 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 9 # p 
        asm.asm_str = "suo2d vrp[n], p, base"
    elif asm.name.lower() == "suo2q":
        if (len(asm.field) != 4) and (4 != 0):
            return 'error asm format'

        insn = 0x701000df
        insn |= ((asm.field[3] >> 0) & 0x1f) << 21 # base 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x1) << 10 # n 
        insn |= ((asm.field[2] >> 0) & 0x1) << 9 # p 
        asm.asm_str = "suo2q vrp[n], p, base"
    elif asm.name.lower() == "nnrwr":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7000003a
        insn |= ((asm.field[1] >> 5) & 0x1f) << 16 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vwrp 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 6 # imm 
        asm.asm_str = "nnrwr vwrp, imm"
    elif asm.name.lower() == "nnrrd":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7020003a
        insn |= ((asm.field[1] >> 0) & 0x3ff) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "nnrrd vrd, imm"
    elif asm.name.lower() == "nndwr":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7040003a
        insn |= ((asm.field[1] >> 5) & 0x1f) << 16 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vrp 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 6 # imm 
        asm.asm_str = "nndwr vrp, imm"
    elif asm.name.lower() == "nndrd":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7060003a
        insn |= ((asm.field[1] >> 0) & 0x3ff) << 11 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 6 # vrd 
        asm.asm_str = "nndrd vrd, imm"
    elif asm.name.lower() == "nncmd":
        if (len(asm.field) != 1) and (1 != 0):
            return 'error asm format'

        insn = 0x70a0003a
        insn |= ((asm.field[0] >> 0) & 0x7fff) << 6 # imm 
        asm.asm_str = "nncmd imm"
    elif asm.name.lower() == "nnmac":
        if (len(asm.field) != 2) and (2 != 0):
            return 'error asm format'

        insn = 0x7080003a
        insn |= ((asm.field[1] >> 5) & 0x1f) << 16 # imm 
        insn |= ((asm.field[0] >> 0) & 0x1f) << 11 # vwrp 
        insn |= ((asm.field[1] >> 0) & 0x1f) << 6 # imm 
        asm.asm_str = "nnmac vwrp, imm"
    else:
        return 'error insn name'

    return insn
