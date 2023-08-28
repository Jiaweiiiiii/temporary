# Auto gen, don't modify this code

import sys
def def_mxu3_disasm(asm):
    if asm.code == 0x0:
        print("nop")
    elif (asm.code & 0xffe0003f)  == 0x4a600014:
        vrs = 0;
        offset = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        offset |= ((asm.code >> 6) & 0x3ff) << 0; # offset
        print("bnezb %d, 0x%x" % (vrs, offset))
    elif (asm.code & 0xffe0003f)  == 0x4a600015:
        vrs = 0;
        offset = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        offset |= ((asm.code >> 6) & 0x3ff) << 0; # offset
        print("bnezh %d, 0x%x" % (vrs, offset))
    elif (asm.code & 0xffe0003f)  == 0x4a600016:
        vrs = 0;
        offset = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        offset |= ((asm.code >> 6) & 0x3ff) << 0; # offset
        print("bnezw %d, 0x%x" % (vrs, offset))
    elif (asm.code & 0xffe0003f)  == 0x4a600017:
        vrs = 0;
        offset = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        offset |= ((asm.code >> 6) & 0x3ff) << 0; # offset
        print("bnezv %d, 0x%x" % (vrs, offset))
    elif (asm.code & 0xffe0003f)  == 0x4a600010:
        vrs = 0;
        offset = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        offset |= ((asm.code >> 6) & 0x3ff) << 0; # offset
        print("beqzb %d, 0x%x" % (vrs, offset))
    elif (asm.code & 0xffe0003f)  == 0x4a600011:
        vrs = 0;
        offset = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        offset |= ((asm.code >> 6) & 0x3ff) << 0; # offset
        print("beqzh %d, 0x%x" % (vrs, offset))
    elif (asm.code & 0xffe0003f)  == 0x4a600012:
        vrs = 0;
        offset = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        offset |= ((asm.code >> 6) & 0x3ff) << 0; # offset
        print("beqzw %d, 0x%x" % (vrs, offset))
    elif (asm.code & 0xffe0003f)  == 0x4a600013:
        vrs = 0;
        offset = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        offset |= ((asm.code >> 6) & 0x3ff) << 0; # offset
        print("beqzv %d, 0x%x" % (vrs, offset))
    elif (asm.code & 0xffe0003f)  == 0x4a000028:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ceqb %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000029:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ceqh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a00002a:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ceqw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0f83f)  == 0x4a000020:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ceqzb %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x4a000021:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ceqzh %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x4a000022:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ceqzw %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0003f)  == 0x4a00003c:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("clesb %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a00003d:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("clesh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a00003e:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("clesw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("cleub %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000039:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("cleuh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a00003a:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("cleuw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0f83f)  == 0x4a00002c:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("clezb %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x4a00002d:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("clezh %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x4a00002e:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("clezw %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0003f)  == 0x4a000034:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("cltsb %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000035:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("cltsh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000036:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("cltsw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000030:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("cltub %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000031:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("cltuh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000032:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("cltuw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0f83f)  == 0x4a000024:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("cltzb %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x4a000025:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("cltzh %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x4a000026:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("cltzw %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0003f)  == 0x4a800000:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("addb %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a800001:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("addh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a800002:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("addw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe00038)  == 0x4b600020:
        vwrd = 0;
        vwrp = 0;
        imm = 0;
        imm |= ((asm.code >> 16) & 0x1f) << 3; # imm
        imm |= ((asm.code >> 0) & 0x7) << 0; # imm
        vwrp |= ((asm.code >> 11) & 0x1f) << 0; # vwrp
        vwrd |= ((asm.code >> 6) & 0x1f) << 0; # vwrd
        print("addiw %d, %d, 0x%x" % (vwrd, vwrp, imm))
    elif (asm.code & 0xffff003f)  == 0x4b600034:
        vwrd = 0;
        vwrp = 0;
        vwrp |= ((asm.code >> 11) & 0x1f) << 0; # vwrp
        vwrd |= ((asm.code >> 6) & 0x1f) << 0; # vwrd
        print("addrw %d, %d" % (vwrd, vwrp))
    elif (asm.code & 0xffe0003f)  == 0x4a800008:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("subb %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a800009:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("subh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a80000a:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("subw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a800024:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("waddsbl %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a800026:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("waddsbh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a800025:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("waddshl %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a800027:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("waddshh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a800020:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("waddubl %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a800022:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("waddubh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a800021:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wadduhl %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a800023:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wadduhh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a80002c:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wsubsbl %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a80002e:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wsubsbh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a80002d:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wsubshl %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a80002f:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wsubshh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a800028:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wsububl %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a80002a:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wsububh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a800029:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wsubuhl %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a80002b:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wsubuhh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0ff3f)  == 0x4ae00000:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("sr1sum2bi %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3e)  == 0x4ae00800:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x1) << 0; # m
        print("sr2sum2bi %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3c)  == 0x4ae01000:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x3) << 0; # m
        print("sr4sum2bi %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff38)  == 0x4ae01800:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x7) << 0; # m
        print("sr8sum2bi %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff30)  == 0x4ae02000:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0xf) << 0; # m
        print("sr16sum2bi %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3f)  == 0x4ae00100:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("sr1sum4bi %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3e)  == 0x4ae00900:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x1) << 0; # m
        print("sr2sum4bi %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3c)  == 0x4ae01100:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x3) << 0; # m
        print("sr4sum4bi %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff38)  == 0x4ae01900:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x7) << 0; # m
        print("sr8sum4bi %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff30)  == 0x4ae02100:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0xf) << 0; # m
        print("sr16sum4bi %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3f)  == 0x4ae00200:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("sr1sumub %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3e)  == 0x4ae00a00:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x1) << 0; # m
        print("sr2sumub %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3c)  == 0x4ae01200:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x3) << 0; # m
        print("sr4sumub %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff38)  == 0x4ae01a00:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x7) << 0; # m
        print("sr8sumub %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff30)  == 0x4ae02200:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0xf) << 0; # m
        print("sr16sumub %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3f)  == 0x4ae00300:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("sr1sumuh %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3e)  == 0x4ae00b00:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x1) << 0; # m
        print("sr2sumuh %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3c)  == 0x4ae01300:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x3) << 0; # m
        print("sr4sumuh %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff38)  == 0x4ae01b00:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x7) << 0; # m
        print("sr8sumuh %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff30)  == 0x4ae02300:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0xf) << 0; # m
        print("sr16sumuh %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3f)  == 0x4ae00400:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("sr1sumsb %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3e)  == 0x4ae00c00:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x1) << 0; # m
        print("sr2sumsb %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3c)  == 0x4ae01400:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x3) << 0; # m
        print("sr4sumsb %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff38)  == 0x4ae01c00:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x7) << 0; # m
        print("sr8sumsb %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff30)  == 0x4ae02400:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0xf) << 0; # m
        print("sr16sumsb %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3f)  == 0x4ae00500:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("sr1sumsh %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3e)  == 0x4ae00d00:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x1) << 0; # m
        print("sr2sumsh %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3c)  == 0x4ae01500:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x3) << 0; # m
        print("sr4sumsh %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff38)  == 0x4ae01d00:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x7) << 0; # m
        print("sr8sumsh %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff30)  == 0x4ae02500:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0xf) << 0; # m
        print("sr16sumsh %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3f)  == 0x4ae00600:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("sr1sumw %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3e)  == 0x4ae00e00:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x1) << 0; # m
        print("sr2sumw %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff3c)  == 0x4ae01600:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x3) << 0; # m
        print("sr4sumw %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff38)  == 0x4ae01e00:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0x7) << 0; # m
        print("sr8sumw %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0ff30)  == 0x4ae02600:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 0) & 0xf) << 0; # m
        print("sr16sumw %d[%d], %d" % (vsd, m, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x4a80000c:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("absb %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x4a80000d:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("absh %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x4a80000e:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("absw %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0003f)  == 0x4a600020:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("mulb %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a600021:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("mulh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a600022:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("mulw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a60002c:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("smulbe %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a60002e:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("smulbo %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a60002d:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("smulhe %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a60002f:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("smulho %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a600028:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("umulbe %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a60002a:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("umulbo %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a600029:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("umulhe %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a60002b:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("umulho %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4b60002c:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wsmulbl %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4b60002e:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wsmulbh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4b60002d:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wsmulhl %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4b60002f:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wsmulhh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4b600028:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wumulbl %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4b60002a:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wumulbh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4b600029:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wumulhl %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4b60002b:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("wumulhh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0073f)  == 0x4a600038:
        vsd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("mlaw %d, %d, %d" % (vsd, vrs, vrp))
    elif (asm.code & 0xffe0073f)  == 0x4a60003c:
        vsd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("mlsw %d, %d, %d" % (vsd, vrs, vrp))
    elif (asm.code & 0xffe0073f)  == 0x4a600039:
        vsd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("smlahe %d, %d, %d" % (vsd, vrs, vrp))
    elif (asm.code & 0xffe0073f)  == 0x4a60003b:
        vsd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("smlaho %d, %d, %d" % (vsd, vrs, vrp))
    elif (asm.code & 0xffe0073f)  == 0x4a60003d:
        vsd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("smlshe %d, %d, %d" % (vsd, vrs, vrp))
    elif (asm.code & 0xffe0073f)  == 0x4a60003f:
        vsd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("smlsho %d, %d, %d" % (vsd, vrs, vrp))
    elif (asm.code & 0xffe0073f)  == 0x4b600039:
        vsd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("wsmlahl %d, %d, %d" % (vsd, vrs, vrp))
    elif (asm.code & 0xffe0073f)  == 0x4b60003b:
        vsd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("wsmlahh %d, %d, %d" % (vsd, vrs, vrp))
    elif (asm.code & 0xffe0073f)  == 0x4b60003d:
        vsd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("wsmlshl %d, %d, %d" % (vsd, vrs, vrp))
    elif (asm.code & 0xffe0073f)  == 0x4b60003f:
        vsd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("wsmlshh %d, %d, %d" % (vsd, vrs, vrp))
    elif (asm.code & 0xffe00720)  == 0x4b800000:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        n |= ((asm.code >> 0) & 0x1f) << 0; # n
        print("sr1mac2bi %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4b800020:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 4) & 0x1) << 0; # m
        n |= ((asm.code >> 0) & 0xf) << 0; # n
        print("sr2mac2bi %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4ba00000:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 3) & 0x3) << 0; # m
        n |= ((asm.code >> 0) & 0x7) << 0; # n
        print("sr4mac2bi %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4ba00020:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 2) & 0x7) << 0; # m
        n |= ((asm.code >> 0) & 0x3) << 0; # n
        print("sr8mac2bi %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4bc00000:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 1) & 0xf) << 0; # m
        n |= ((asm.code >> 0) & 0x1) << 0; # n
        print("sr16mac2bi %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4b800100:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        n |= ((asm.code >> 0) & 0x1f) << 0; # n
        print("sr1mac4bi %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4b800120:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 4) & 0x1) << 0; # m
        n |= ((asm.code >> 0) & 0xf) << 0; # n
        print("sr2mac4bi %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4ba00100:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 3) & 0x3) << 0; # m
        n |= ((asm.code >> 0) & 0x7) << 0; # n
        print("sr4mac4bi %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4ba00120:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 2) & 0x7) << 0; # m
        n |= ((asm.code >> 0) & 0x3) << 0; # n
        print("sr8mac4bi %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4bc00100:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 1) & 0xf) << 0; # m
        n |= ((asm.code >> 0) & 0x1) << 0; # n
        print("sr16mac4bi %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4b800200:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        n |= ((asm.code >> 0) & 0x1f) << 0; # n
        print("sr1macuub %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4b800220:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 4) & 0x1) << 0; # m
        n |= ((asm.code >> 0) & 0xf) << 0; # n
        print("sr2macuub %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4ba00200:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 3) & 0x3) << 0; # m
        n |= ((asm.code >> 0) & 0x7) << 0; # n
        print("sr4macuub %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4ba00220:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 2) & 0x7) << 0; # m
        n |= ((asm.code >> 0) & 0x3) << 0; # n
        print("sr8macuub %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4bc00200:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 1) & 0xf) << 0; # m
        n |= ((asm.code >> 0) & 0x1) << 0; # n
        print("sr16macuub %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4b800600:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        n |= ((asm.code >> 0) & 0x1f) << 0; # n
        print("sr1macsub %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4b800620:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 4) & 0x1) << 0; # m
        n |= ((asm.code >> 0) & 0xf) << 0; # n
        print("sr2macsub %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4ba00600:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 3) & 0x3) << 0; # m
        n |= ((asm.code >> 0) & 0x7) << 0; # n
        print("sr4macsub %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4ba00620:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 2) & 0x7) << 0; # m
        n |= ((asm.code >> 0) & 0x3) << 0; # n
        print("sr8macsub %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4bc00600:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 1) & 0xf) << 0; # m
        n |= ((asm.code >> 0) & 0x1) << 0; # n
        print("sr16macsub %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4b800400:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        n |= ((asm.code >> 0) & 0x1f) << 0; # n
        print("sr1macssb %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4b800420:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 4) & 0x1) << 0; # m
        n |= ((asm.code >> 0) & 0xf) << 0; # n
        print("sr2macssb %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4ba00400:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 3) & 0x3) << 0; # m
        n |= ((asm.code >> 0) & 0x7) << 0; # n
        print("sr4macssb %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4ba00420:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 2) & 0x7) << 0; # m
        n |= ((asm.code >> 0) & 0x3) << 0; # n
        print("sr8macssb %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4bc00400:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 1) & 0xf) << 0; # m
        n |= ((asm.code >> 0) & 0x1) << 0; # n
        print("sr16macssb %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4b800500:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        n |= ((asm.code >> 0) & 0x1f) << 0; # n
        print("sr1macssh %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4b800520:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 4) & 0x1) << 0; # m
        n |= ((asm.code >> 0) & 0xf) << 0; # n
        print("sr2macssh %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4ba00500:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 3) & 0x3) << 0; # m
        n |= ((asm.code >> 0) & 0x7) << 0; # n
        print("sr4macssh %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4ba00520:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 2) & 0x7) << 0; # m
        n |= ((asm.code >> 0) & 0x3) << 0; # n
        print("sr8macssh %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe00720)  == 0x4bc00500:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        n = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 1) & 0xf) << 0; # m
        n |= ((asm.code >> 0) & 0x1) << 0; # n
        print("sr16macssh %d[%d], %d, %d[%d]" % (vsd, m, vrs, vrp, n))
    elif (asm.code & 0xffe0073f)  == 0x4b800720:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("s1macuub %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe0071f)  == 0x4b800710:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 5) & 0x1) << 0; # m
        print("s2macuub %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe0070f)  == 0x4b800708:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 4) & 0x3) << 0; # m
        print("s4macuub %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe00707)  == 0x4b800704:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 3) & 0x7) << 0; # m
        print("s8macuub %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe00703)  == 0x4b800702:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 2) & 0xf) << 0; # m
        print("s16macuub %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe0073f)  == 0x4ba00720:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("s1macsub %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe0071f)  == 0x4ba00710:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 5) & 0x1) << 0; # m
        print("s2macsub %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe0070f)  == 0x4ba00708:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 4) & 0x3) << 0; # m
        print("s4macsub %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe00707)  == 0x4ba00704:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 3) & 0x7) << 0; # m
        print("s8macsub %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe00703)  == 0x4ba00702:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 2) & 0xf) << 0; # m
        print("s16macsub %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe0073f)  == 0x4bc00720:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("s1macssb %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe0071f)  == 0x4bc00710:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 5) & 0x1) << 0; # m
        print("s2macssb %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe0070f)  == 0x4bc00708:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 4) & 0x3) << 0; # m
        print("s4macssb %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe00707)  == 0x4bc00704:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 3) & 0x7) << 0; # m
        print("s8macssb %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe00703)  == 0x4bc00702:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 2) & 0xf) << 0; # m
        print("s16macssb %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe0073f)  == 0x4be00720:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("s1macssh %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe0071f)  == 0x4be00710:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 5) & 0x1) << 0; # m
        print("s2macssh %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe0070f)  == 0x4be00708:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 4) & 0x3) << 0; # m
        print("s4macssh %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe00707)  == 0x4be00704:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 3) & 0x7) << 0; # m
        print("s8macssh %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe00703)  == 0x4be00702:
        vsd = 0;
        m = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        m |= ((asm.code >> 2) & 0xf) << 0; # m
        print("s16macssh %d[%d], %d, %d" % (vsd, m, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000018:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("maxab %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000019:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("maxah %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a00001a:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("maxaw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a00001c:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("maxsb %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a00001d:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("maxsh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a00001e:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("maxsw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000008:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("maxub %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000009:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("maxuh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a00000a:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("maxuw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a00000c:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("maxu2bi %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a00000d:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("maxu4bi %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000010:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("minab %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000011:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("minah %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000012:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("minaw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000014:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("minsb %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000015:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("minsh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000016:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("minsw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000000:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("minub %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000001:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("minuh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000002:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("minuw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000004:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("minu2bi %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000005:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("minu4bi %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0f83f)  == 0x7120002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satsshb %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x7140002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satsswb %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x71c0002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satsswh %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x7200002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satsub2bi %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x7280002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satsub4bi %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x7220002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satsuh2bi %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x72a0002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satsuh4bi %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x7320002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satsuhb %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x7240002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satsuw2bi %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x72c0002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satsuw4bi %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x7340002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satsuwb %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x73c0002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satsuwh %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x7000002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satuub2bi %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x7020002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satuub4bi %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x7040002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satuuh2bi %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x7060002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satuuh4bi %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x7080002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satuuhb %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x70a0002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satuuw4bi %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x70c0002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satuuwb %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x70e0002f:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("satuuwh %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0ff3f)  == 0x4a600018:
        vsd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("tocb %d, %d" % (vsd, vrs))
    elif (asm.code & 0xffe0ff3f)  == 0x4a600019:
        vsd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("toch %d, %d" % (vsd, vrs))
    elif (asm.code & 0xffe0ff3f)  == 0x4a60001a:
        vsd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("tocw %d, %d" % (vsd, vrs))
    elif (asm.code & 0xffe0003f)  == 0x4a600002:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("andv %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a600003:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("andnv %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe00038)  == 0x4b600008:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        imm |= ((asm.code >> 0) & 0x7) << 5; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("andib %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4a600004:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("orv %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a600005:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ornv %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe00038)  == 0x4b600010:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        imm |= ((asm.code >> 0) & 0x7) << 5; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("orib %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4a600006:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("xorv %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a600007:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("xornv %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe00038)  == 0x4b600018:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        imm |= ((asm.code >> 0) & 0x7) << 5; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("xorib %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4a60000e:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("bselv %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a800003:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("faddw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a80000b:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fsubw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a600023:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fmulw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4b600030:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fcmulrw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4b600032:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fcmuliw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a60003a:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fcaddw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffff003f)  == 0x4a60003e:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fxas1w %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x4a61003e:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fxas2w %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x4a62003e:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fxas4w %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x4a63003e:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fxas8w %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a00001f:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fmaxw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a00001b:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fmaxaw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000017:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fminw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000013:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fminaw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0f83f)  == 0x4900002e:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fclassw %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0003f)  == 0x4a00002b:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fceqw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a00003f:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fclew %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000037:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fcltw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a000033:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("fcorw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0f83f)  == 0x7040002e:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ffsiw %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x7000002e:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ffuiw %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x70e0002e:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ftsiw %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x70a0002e:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ftuiw %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x7020002e:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("frintw %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x70c0002e:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ftruncsw %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0f83f)  == 0x7080002e:
        vrd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ftruncuw %d, %d" % (vrd, vrs))
    elif (asm.code & 0xffe0003f)  == 0x4a200020:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("sllb %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a200021:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("sllh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a200022:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("sllw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4aa00020:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("sllib %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4aa00021:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("sllih %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4aa00022:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("slliw %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4a200038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srab %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a200039:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srah %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a20003a:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("sraw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4aa00038:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("sraib %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4aa00039:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("sraih %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4aa0003a:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("sraiw %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4a20003c:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srarb %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a20003d:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srarh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a20003e:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srarw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4aa0003c:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srarib %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4aa0003d:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srarih %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4aa0003e:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srariw %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4a200030:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srlb %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a200031:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srlh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a200032:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srlw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4aa00030:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srlib %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4aa00031:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srlih %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4aa00032:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srliw %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4a200034:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srlrb %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a200035:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srlrh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a200036:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srlrw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4aa00034:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srlrib %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4aa00035:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srlrih %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffe0003f)  == 0x4aa00036:
        vrd = 0;
        vrs = 0;
        imm = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("srlriw %d, %d, 0x%x" % (vrd, vrs, imm))
    elif (asm.code & 0xffff003f)  == 0x71c40034:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("gt1bi %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71c50034:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("gt2bi %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71c60034:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("gt4bi %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71c00034:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("gtb %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71c10034:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("gth %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffde003f)  == 0x70000035:
        vrd = 0;
        m = 0;
        vrp = 0;
        pos = 0;
        pos |= ((asm.code >> 21) & 0x1) << 0; # pos
        m |= ((asm.code >> 16) & 0x1) << 0; # m
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("gt2w %d[%d], %d, %d" % (vrd, m, vrp, pos))
    elif (asm.code & 0xff9c003f)  == 0x70040035:
        vrd = 0;
        m = 0;
        vrp = 0;
        pos = 0;
        pos |= ((asm.code >> 21) & 0x3) << 0; # pos
        m |= ((asm.code >> 16) & 0x3) << 0; # m
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("gt4w %d[%d], %d, %d" % (vrd, m, vrp, pos))
    elif (asm.code & 0xff18003f)  == 0x70080035:
        vrd = 0;
        m = 0;
        vrp = 0;
        pos = 0;
        pos |= ((asm.code >> 21) & 0x7) << 0; # pos
        m |= ((asm.code >> 16) & 0x7) << 0; # m
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("gt8w %d[%d], %d, %d" % (vrd, m, vrp, pos))
    elif (asm.code & 0xffde003f)  == 0x72020035:
        vrd = 0;
        m = 0;
        vrp = 0;
        pos = 0;
        pos |= ((asm.code >> 21) & 0x1) << 0; # pos
        m |= ((asm.code >> 16) & 0x1) << 0; # m
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("gt2d %d[%d], %d, %d" % (vrd, m, vrp, pos))
    elif (asm.code & 0xff9c003f)  == 0x72040035:
        vrd = 0;
        m = 0;
        vrp = 0;
        pos = 0;
        pos |= ((asm.code >> 21) & 0x3) << 0; # pos
        m |= ((asm.code >> 16) & 0x3) << 0; # m
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("gt4d %d[%d], %d, %d" % (vrd, m, vrp, pos))
    elif (asm.code & 0xffde003f)  == 0x73020035:
        vrd = 0;
        m = 0;
        vrp = 0;
        pos = 0;
        pos |= ((asm.code >> 21) & 0x1) << 0; # pos
        m |= ((asm.code >> 16) & 0x1) << 0; # m
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("gt2q %d[%d], %d, %d" % (vrd, m, vrp, pos))
    elif (asm.code & 0xffdf003f)  == 0x72000031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extuwll %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffdf003f)  == 0x72080031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extuwlh %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffdf003f)  == 0x72100031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extuwhl %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffdf003f)  == 0x72180031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extuwhh %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffdf003f)  == 0x72010031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extudll %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffdf003f)  == 0x72090031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extudlh %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffdf003f)  == 0x72110031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extudhl %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffdf003f)  == 0x72190031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extudhh %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffdf003f)  == 0x72020031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extuqll %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffdf003f)  == 0x720a0031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extuqlh %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffdf003f)  == 0x72120031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extuqhl %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffdf003f)  == 0x721a0031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extuqhh %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffdf003f)  == 0x72030031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extuoll %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffdf003f)  == 0x720b0031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extuolh %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffdf003f)  == 0x72130031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extuohl %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffdf003f)  == 0x721b0031:
        vrd = 0;
        vrp = 0;
        w = 0;
        w |= ((asm.code >> 21) & 0x1) << 0; # w
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extuohh %d, %d, %d" % (vrd, vrp, w))
    elif (asm.code & 0xffff003f)  == 0x71a40031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extu1bil %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71a50031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extu2bil %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71a60031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extu4bil %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71a00031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extubl %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71a10031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extuhl %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71ac0031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extu1bih %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71ad0031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extu2bih %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71ae0031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extu4bih %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71a80031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extubh %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71a90031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extuhh %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71b40031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("exts1bil %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71b50031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("exts2bil %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71b60031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("exts4bil %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71b00031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extsbl %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71b10031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extshl %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71bc0031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("exts1bih %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71bd0031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("exts2bih %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71be0031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("exts4bih %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71b80031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extsbh %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71b90031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extshh %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71800031:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("extu3bw %d, %d" % (vrd, vrp))
    elif (asm.code & 0xff80003f)  == 0x70000030:
        vrd = 0;
        vrp = 0;
        n = 0;
        n |= ((asm.code >> 16) & 0x7f) << 0; # n
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("repib %d, %d[%d]" % (vrd, vrp, n))
    elif (asm.code & 0xffc0003f)  == 0x70800030:
        vrd = 0;
        vrp = 0;
        n = 0;
        n |= ((asm.code >> 16) & 0x3f) << 0; # n
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("repih %d, %d[%d]" % (vrd, vrp, n))
    elif (asm.code & 0xffe0003f)  == 0x70c00030:
        vrd = 0;
        vrp = 0;
        n = 0;
        n |= ((asm.code >> 16) & 0x1f) << 0; # n
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("repiw %d, %d[%d]" % (vrd, vrp, n))
    elif (asm.code & 0xfff0003f)  == 0x70e00030:
        vrd = 0;
        vrp = 0;
        n = 0;
        n |= ((asm.code >> 16) & 0xf) << 0; # n
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("repid %d, %d[%d]" % (vrd, vrp, n))
    elif (asm.code & 0xfff8003f)  == 0x70f00030:
        vrd = 0;
        vrp = 0;
        n = 0;
        n |= ((asm.code >> 16) & 0x7) << 0; # n
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("repiq %d, %d[%d]" % (vrd, vrp, n))
    elif (asm.code & 0xfffc003f)  == 0x70f80030:
        vrd = 0;
        vrp = 0;
        n = 0;
        n |= ((asm.code >> 16) & 0x3) << 0; # n
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("repio %d, %d[%d]" % (vrd, vrp, n))
    elif (asm.code & 0xfc00003f)  == 0x70000036:
        vrd = 0;
        m = 0;
        vrp = 0;
        n = 0;
        m |= ((asm.code >> 21) & 0x1f) << 0; # m
        n |= ((asm.code >> 16) & 0x1f) << 0; # n
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("movw %d[%d], %d[%d]" % (vrd, m, vrp, n))
    elif (asm.code & 0xfe10003f)  == 0x70000037:
        vrd = 0;
        m = 0;
        vrp = 0;
        n = 0;
        m |= ((asm.code >> 21) & 0xf) << 0; # m
        n |= ((asm.code >> 16) & 0xf) << 0; # n
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("movd %d[%d], %d[%d]" % (vrd, m, vrp, n))
    elif (asm.code & 0xfe10003f)  == 0x70100037:
        vrd = 0;
        m = 0;
        vrp = 0;
        n = 0;
        m |= ((asm.code >> 21) & 0xf) << 0; # m
        n |= ((asm.code >> 16) & 0xf) << 0; # n
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("movq %d[%d], %d[%d]" % (vrd, m, vrp, n))
    elif (asm.code & 0xfe10003f)  == 0x72000037:
        vrd = 0;
        m = 0;
        vrp = 0;
        n = 0;
        m |= ((asm.code >> 21) & 0xf) << 0; # m
        n |= ((asm.code >> 16) & 0xf) << 0; # n
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("movo %d[%d], %d[%d]" % (vrd, m, vrp, n))
    elif (asm.code & 0xffff003f)  == 0x70000033:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("shufw2 %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x70800033:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("shufw4 %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x71000033:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("shufw8 %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x70200033:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("shufd2 %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x70a00033:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("shufd4 %d, %d" % (vrd, vrp))
    elif (asm.code & 0xffff003f)  == 0x70400033:
        vrd = 0;
        vrp = 0;
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("shufq2 %d, %d" % (vrd, vrp))
    elif (asm.code & 0xff80003f)  == 0x7200003b:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        imm = 0;
        imm |= ((asm.code >> 21) & 0x3) << 0; # imm
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("gshufw %d, %d, %d, 0x%x" % (vrd, vrs, vrp, imm))
    elif (asm.code & 0xffc0003f)  == 0x7280003b:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        imm = 0;
        imm |= ((asm.code >> 21) & 0x1) << 0; # imm
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("gshufwb_1 %d, %d, %d, 0x%x" % (vrd, vrs, vrp, imm))
    elif (asm.code & 0xffc0003f)  == 0x7380003b:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        imm = 0;
        imm |= ((asm.code >> 21) & 0x1) << 0; # imm
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("gshufwb_2 %d, %d, %d, 0x%x" % (vrd, vrs, vrp, imm))
    elif (asm.code & 0xffe0003f)  == 0x7380002e:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("gshufvb %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x70000038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilve2bi %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x70200038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilve4bi %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x70400038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilveb %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x70600038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilveh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x70800038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilvew %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x70a00038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilved %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x70c00038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilveq %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x70e00038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilveo %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x72000038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilvo2bi %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x72200038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilvo4bi %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x72400038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilvob %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x72600038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilvoh %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x72800038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilvow %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x72a00038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilvod %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x72c00038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilvoq %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x72e00038:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("ilvoo %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xff80003f)  == 0x70000039:
        vrd = 0;
        vrp = 0;
        imm = 0;
        imm |= ((asm.code >> 16) & 0x7f) << 0; # imm
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("bshli %d, %d, 0x%x" % (vrd, vrp, imm))
    elif (asm.code & 0xff80003f)  == 0x71000039:
        vrd = 0;
        vrp = 0;
        imm = 0;
        imm |= ((asm.code >> 16) & 0x7f) << 0; # imm
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("bshri %d, %d, 0x%x" % (vrd, vrp, imm))
    elif (asm.code & 0xffe0003f)  == 0x70800039:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("bshl %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x71800039:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("bshr %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffff003f)  == 0x7340002e:
        rd = 0;
        vwrp = 0;
        vwrp |= ((asm.code >> 11) & 0x1f) << 0; # vwrp
        rd |= ((asm.code >> 6) & 0x1f) << 0; # rd
        print("mtcpuw %d, %d" % (rd, vwrp))
    elif (asm.code & 0xfc1ff83f)  == 0x701b0003:
        vwrd = 0;
        rs = 0;
        rs |= ((asm.code >> 21) & 0x1f) << 0; # rs
        vwrd |= ((asm.code >> 6) & 0x1f) << 0; # vwrd
        print("mfcpuw %d, %d" % (vwrd, rs))
    elif (asm.code & 0xffff003f)  == 0x7300002e:
        fd = 0;
        vwrp = 0;
        vwrp |= ((asm.code >> 11) & 0x1f) << 0; # vwrp
        fd |= ((asm.code >> 6) & 0x1f) << 0; # fd
        print("mtfpuw %d, %d" % (fd, vwrp))
    elif (asm.code & 0xffe0f83f)  == 0x7320002e:
        vwrd = 0;
        fs = 0;
        fs |= ((asm.code >> 16) & 0x1f) << 0; # fs
        vwrd |= ((asm.code >> 6) & 0x1f) << 0; # vwrd
        print("mffpuw %d, %d" % (vwrd, fs))
    elif (asm.code & 0xfc1f07ff)  == 0x70110003:
        mcd = 0;
        rs = 0;
        rs |= ((asm.code >> 21) & 0x1f) << 0; # rs
        mcd |= ((asm.code >> 11) & 0x1f) << 0; # mcd
        print("ctcmxu %d, %d" % (mcd, rs))
    elif (asm.code & 0xffff003f)  == 0x70100003:
        rd = 0;
        mcs = 0;
        mcs |= ((asm.code >> 11) & 0x1f) << 0; # mcs
        rd |= ((asm.code >> 6) & 0x1f) << 0; # rd
        print("cfcmxu %d, %d" % (rd, mcs))
    elif (asm.code & 0xffffff3f)  == 0x4a60001c:
        vsd = 0;
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("sumz %d" % (vsd))
    elif (asm.code & 0xffffe03f)  == 0x4a60000f:
        vrd = 0;
        vss = 0;
        vss |= ((asm.code >> 11) & 0x3) << 0; # vss
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("mfsum %d, %d" % (vrd, vss))
    elif (asm.code & 0xffffe03f)  == 0x4a60001e:
        vrd = 0;
        vsd = 0;
        vsd |= ((asm.code >> 11) & 0x3) << 0; # vsd
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("mfsumz %d, %d" % (vrd, vsd))
    elif (asm.code & 0xffe0ff3f)  == 0x4a60001d:
        vsd = 0;
        vrs = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 6) & 0x3) << 0; # vsd
        print("mtsum %d, %d" % (vsd, vrs))
    elif (asm.code & 0xffe0e03f)  == 0x4a60001f:
        vrd = 0;
        vrs = 0;
        vsd = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vsd |= ((asm.code >> 11) & 0x3) << 0; # vsd
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("mxsum %d, %d, %d" % (vrd, vrs, vsd))
    elif (asm.code & 0xffe00000)  == 0x4b000000:
        vrd = 0;
        imm = 0;
        imm |= ((asm.code >> 16) & 0x1f) << 11; # imm
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        imm |= ((asm.code >> 0) & 0x3f) << 5; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lih %d, 0x%x" % (vrd, imm))
    elif (asm.code & 0xffe00000)  == 0x4b200000:
        vrd = 0;
        imm = 0;
        imm |= ((asm.code >> 16) & 0x1f) << 11; # imm
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        imm |= ((asm.code >> 0) & 0x3f) << 5; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("liw %d, 0x%x" % (vrd, imm))
    elif (asm.code & 0xffe00000)  == 0x4b400000:
        vrd = 0;
        imm = 0;
        imm |= ((asm.code >> 16) & 0x1f) << 11; # imm
        imm |= ((asm.code >> 11) & 0x1f) << 0; # imm
        imm |= ((asm.code >> 0) & 0x3f) << 5; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("liwh %d, 0x%x" % (vrd, imm))
    elif (asm.code & 0xffe0003f)  == 0x4a600000:
        vwrd = 0;
        imm = 0;
        imm |= ((asm.code >> 11) & 0x3ff) << 0; # imm
        vwrd |= ((asm.code >> 6) & 0x1f) << 0; # vwrd
        print("liwr %d, 0x%x" % (vwrd, imm))
    elif (asm.code & 0xfe00003f)  == 0x70000032:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        imm = 0;
        imm |= ((asm.code >> 21) & 0xf) << 0; # imm
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("cmvw %d, %d, %d, 0x%x" % (vrd, vrs, vrp, imm))
    elif (asm.code & 0xffe0003f)  == 0x4a600009:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("pmaph %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xffe0003f)  == 0x4a60000a:
        vrd = 0;
        vrs = 0;
        vrp = 0;
        vrs |= ((asm.code >> 16) & 0x1f) << 0; # vrs
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("pmapw %d, %d, %d" % (vrd, vrs, vrp))
    elif (asm.code & 0xfc00003f)  == 0x70000010:
        vrd = 0;
        n = 0;
        offset = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        offset |= ((asm.code >> 16) & 0x1f) << 0; # offset
        n |= ((asm.code >> 11) & 0x1f) << 0; # n
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("law %d[%d], 0x%x(%d)" % (vrd, n, offset, base))
    elif (asm.code & 0xfc00083f)  == 0x70000011:
        vrd = 0;
        n = 0;
        offset = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        offset |= ((asm.code >> 16) & 0x1f) << 0; # offset
        n |= ((asm.code >> 12) & 0xf) << 0; # n
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lad %d[%d], 0x%x(%d)" % (vrd, n, offset, base))
    elif (asm.code & 0xfc00183f)  == 0x70000811:
        vrd = 0;
        n = 0;
        offset = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        offset |= ((asm.code >> 16) & 0x1f) << 0; # offset
        n |= ((asm.code >> 13) & 0x7) << 0; # n
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("laq %d[%d], 0x%x(%d)" % (vrd, n, offset, base))
    elif (asm.code & 0xfc00383f)  == 0x70001811:
        vrd = 0;
        n = 0;
        offset = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        offset |= ((asm.code >> 16) & 0x1f) << 0; # offset
        n |= ((asm.code >> 14) & 0x3) << 0; # n
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lao %d[%d], 0x%x(%d)" % (vrd, n, offset, base))
    elif (asm.code & 0xfc00003f)  == 0x70000014:
        vrp = 0;
        n = 0;
        offset = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        offset |= ((asm.code >> 16) & 0x1f) << 0; # offset
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 6) & 0x1f) << 0; # n
        print("saw %d[%d], 0x%x(%d)" % (vrp, n, offset, base))
    elif (asm.code & 0xfc00007f)  == 0x70000015:
        vrp = 0;
        n = 0;
        offset = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        offset |= ((asm.code >> 16) & 0x1f) << 0; # offset
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 7) & 0xf) << 0; # n
        print("sad %d[%d], 0x%x(%d)" % (vrp, n, offset, base))
    elif (asm.code & 0xfc0000ff)  == 0x70000055:
        vrp = 0;
        n = 0;
        offset = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        offset |= ((asm.code >> 16) & 0x1f) << 0; # offset
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 8) & 0x7) << 0; # n
        print("saq %d[%d], 0x%x(%d)" % (vrp, n, offset, base))
    elif (asm.code & 0xfc0001ff)  == 0x700000d5:
        vrp = 0;
        n = 0;
        offset = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        offset |= ((asm.code >> 16) & 0x1f) << 0; # offset
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 9) & 0x3) << 0; # n
        print("sao %d[%d], 0x%x(%d)" % (vrp, n, offset, base))
    elif (asm.code & 0xfc1f003f)  == 0x70000012:
        vrd = 0;
        n = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 11) & 0x1f) << 0; # n
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luw %d[%d], %d" % (vrd, n, base))
    elif (asm.code & 0xfc1f083f)  == 0x70000013:
        vrd = 0;
        n = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 12) & 0xf) << 0; # n
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lud %d[%d], %d" % (vrd, n, base))
    elif (asm.code & 0xfc1f183f)  == 0x70000813:
        vrd = 0;
        n = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 13) & 0x7) << 0; # n
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luq %d[%d], %d" % (vrd, n, base))
    elif (asm.code & 0xfc1f383f)  == 0x70001813:
        vrd = 0;
        n = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 14) & 0x3) << 0; # n
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luo %d[%d], %d" % (vrd, n, base))
    elif (asm.code & 0xfc1f003f)  == 0x70000016:
        vrp = 0;
        n = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 6) & 0x1f) << 0; # n
        print("suw %d[%d], %d" % (vrp, n, base))
    elif (asm.code & 0xfc1f007f)  == 0x70000017:
        vrp = 0;
        n = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 7) & 0xf) << 0; # n
        print("sud %d[%d], %d" % (vrp, n, base))
    elif (asm.code & 0xfc1f00ff)  == 0x70000057:
        vrp = 0;
        n = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 8) & 0x7) << 0; # n
        print("suq %d[%d], %d" % (vrp, n, base))
    elif (asm.code & 0xfc1f01ff)  == 0x700000d7:
        vrp = 0;
        n = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 9) & 0x3) << 0; # n
        print("suo %d[%d], %d" % (vrp, n, base))
    elif (asm.code & 0xfc1e083f)  == 0x70000019:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 13) & 0xf) << 0; # n
        p |= ((asm.code >> 12) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("law2b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e083f)  == 0x70100019:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 13) & 0xf) << 0; # n
        p |= ((asm.code >> 12) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("law2h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e083f)  == 0x70020019:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 14) & 0x7) << 0; # n
        p |= ((asm.code >> 12) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("law4b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e083f)  == 0x70120019:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 14) & 0x7) << 0; # n
        p |= ((asm.code >> 12) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("law4h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e083f)  == 0x70040019:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 12) & 0x7) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("law8b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e083f)  == 0x70140019:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 12) & 0x7) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("law8h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e083f)  == 0x70060019:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 12) & 0xf) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("law16b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e083f)  == 0x70160019:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 12) & 0xf) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("law16h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x70000819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 14) & 0x7) << 0; # n
        p |= ((asm.code >> 13) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lad2b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x70080819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 14) & 0x7) << 0; # n
        p |= ((asm.code >> 13) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lad2h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x70100819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 14) & 0x7) << 0; # n
        p |= ((asm.code >> 13) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lad2w %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x70020819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 13) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lad4b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x700a0819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 13) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lad4h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x70120819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 13) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lad4w %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x70040819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 13) & 0x7) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lad8b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x700c0819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 13) & 0x7) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lad8h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x70140819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 13) & 0x7) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lad8w %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x70001819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 14) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("laq2b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x70081819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 14) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("laq2h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x70101819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 14) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("laq2w %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x70181819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 14) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("laq2d %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x70021819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 14) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("laq4b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x700a1819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 14) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("laq4h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x70121819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 14) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("laq4w %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x701a1819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 14) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("laq4d %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e783f)  == 0x70003819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 15) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lao2b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e783f)  == 0x70043819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 15) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lao2h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e783f)  == 0x70083819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 15) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lao2w %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e783f)  == 0x700c3819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 15) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lao2d %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e783f)  == 0x70103819:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 15) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lao2q %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e083f)  == 0x7000001b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 13) & 0xf) << 0; # n
        p |= ((asm.code >> 12) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luw2b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e083f)  == 0x7010001b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 13) & 0xf) << 0; # n
        p |= ((asm.code >> 12) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luw2h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e083f)  == 0x7002001b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 14) & 0x7) << 0; # n
        p |= ((asm.code >> 12) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luw4b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e083f)  == 0x7012001b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 14) & 0x7) << 0; # n
        p |= ((asm.code >> 12) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luw4h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e083f)  == 0x7004001b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 12) & 0x7) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luw8b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e083f)  == 0x7014001b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 12) & 0x7) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luw8h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e083f)  == 0x7006001b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 12) & 0xf) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luw16b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e083f)  == 0x7016001b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 12) & 0xf) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luw16h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x7000081b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 14) & 0x7) << 0; # n
        p |= ((asm.code >> 13) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lud2b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x7008081b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 14) & 0x7) << 0; # n
        p |= ((asm.code >> 13) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lud2h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x7010081b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 14) & 0x7) << 0; # n
        p |= ((asm.code >> 13) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lud2w %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x7002081b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 13) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lud4b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x700a081b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 13) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lud4h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x7012081b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 13) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lud4w %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x7004081b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 13) & 0x7) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lud8b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x700c081b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 13) & 0x7) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lud8h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e183f)  == 0x7014081b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 13) & 0x7) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("lud8w %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x7000181b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 14) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luq2b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x7008181b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 14) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luq2h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x7010181b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 14) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luq2w %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x7018181b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 15) & 0x3) << 0; # n
        p |= ((asm.code >> 14) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luq2d %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x7002181b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 14) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luq4b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x700a181b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 14) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luq4h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x7012181b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 14) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luq4w %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e383f)  == 0x701a181b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 14) & 0x3) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luq4d %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e783f)  == 0x7000381b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 15) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luo2b %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e783f)  == 0x7004381b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 15) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luo2h %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e783f)  == 0x7008381b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 15) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luo2w %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e783f)  == 0x700c381b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 15) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luo2d %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1e783f)  == 0x7010381b:
        vrd = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        n |= ((asm.code >> 16) & 0x1) << 0; # n
        p |= ((asm.code >> 15) & 0x1) << 0; # p
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("luo2q %d[%d], %d, %d" % (vrd, n, p, base))
    elif (asm.code & 0xfc1f007f)  == 0x7000001d:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 8) & 0x7) << 0; # n
        p |= ((asm.code >> 7) & 0x1) << 0; # p
        print("sad2w %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f007f)  == 0x7008001d:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 9) & 0x3) << 0; # n
        p |= ((asm.code >> 7) & 0x3) << 0; # p
        print("sad4w %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f007f)  == 0x7010001d:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 10) & 0x1) << 0; # n
        p |= ((asm.code >> 7) & 0x7) << 0; # p
        print("sad8w %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f00ff)  == 0x7000005d:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 9) & 0x3) << 0; # n
        p |= ((asm.code >> 8) & 0x1) << 0; # p
        print("saq2w %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f00ff)  == 0x7010005d:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 9) & 0x3) << 0; # n
        p |= ((asm.code >> 8) & 0x1) << 0; # p
        print("saq2d %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f00ff)  == 0x7004005d:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 10) & 0x1) << 0; # n
        p |= ((asm.code >> 8) & 0x3) << 0; # p
        print("saq4w %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f00ff)  == 0x7014005d:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 10) & 0x1) << 0; # n
        p |= ((asm.code >> 8) & 0x3) << 0; # p
        print("saq4d %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f01ff)  == 0x700000dd:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 10) & 0x1) << 0; # n
        p |= ((asm.code >> 9) & 0x1) << 0; # p
        print("sao2w %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f01ff)  == 0x700800dd:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 10) & 0x1) << 0; # n
        p |= ((asm.code >> 9) & 0x1) << 0; # p
        print("sao2d %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f01ff)  == 0x701000dd:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 10) & 0x1) << 0; # n
        p |= ((asm.code >> 9) & 0x1) << 0; # p
        print("sao2q %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f007f)  == 0x7000001f:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 8) & 0x7) << 0; # n
        p |= ((asm.code >> 7) & 0x1) << 0; # p
        print("sud2w %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f007f)  == 0x7008001f:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 9) & 0x3) << 0; # n
        p |= ((asm.code >> 7) & 0x3) << 0; # p
        print("sud4w %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f007f)  == 0x7010001f:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 10) & 0x1) << 0; # n
        p |= ((asm.code >> 7) & 0x7) << 0; # p
        print("sud8w %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f00ff)  == 0x7000005f:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 9) & 0x3) << 0; # n
        p |= ((asm.code >> 8) & 0x1) << 0; # p
        print("suq2w %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f00ff)  == 0x7010005f:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 9) & 0x3) << 0; # n
        p |= ((asm.code >> 8) & 0x1) << 0; # p
        print("suq2d %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f00ff)  == 0x7004005f:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 10) & 0x1) << 0; # n
        p |= ((asm.code >> 8) & 0x3) << 0; # p
        print("suq4w %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f00ff)  == 0x7014005f:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 10) & 0x1) << 0; # n
        p |= ((asm.code >> 8) & 0x3) << 0; # p
        print("suq4d %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f01ff)  == 0x700000df:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 10) & 0x1) << 0; # n
        p |= ((asm.code >> 9) & 0x1) << 0; # p
        print("suo2w %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f01ff)  == 0x700800df:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 10) & 0x1) << 0; # n
        p |= ((asm.code >> 9) & 0x1) << 0; # p
        print("suo2d %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xfc1f01ff)  == 0x701000df:
        vrp = 0;
        n = 0;
        p = 0;
        base = 0;
        base |= ((asm.code >> 21) & 0x1f) << 0; # base
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        n |= ((asm.code >> 10) & 0x1) << 0; # n
        p |= ((asm.code >> 9) & 0x1) << 0; # p
        print("suo2q %d[%d], %d, %d" % (vrp, n, p, base))
    elif (asm.code & 0xffe0003f)  == 0x7000003a:
        vwrp = 0;
        imm = 0;
        imm |= ((asm.code >> 16) & 0x1f) << 5; # imm
        imm |= ((asm.code >> 6) & 0x1f) << 0; # imm
        vwrp |= ((asm.code >> 11) & 0x1f) << 0; # vwrp
        print("nnrwr %d, 0x%x" % (vwrp, imm))
    elif (asm.code & 0xffe0003f)  == 0x7020003a:
        vrd = 0;
        imm = 0;
        imm |= ((asm.code >> 11) & 0x3ff) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("nnrrd %d, 0x%x" % (vrd, imm))
    elif (asm.code & 0xffe0003f)  == 0x7040003a:
        vrp = 0;
        imm = 0;
        imm |= ((asm.code >> 16) & 0x1f) << 5; # imm
        imm |= ((asm.code >> 6) & 0x1f) << 0; # imm
        vrp |= ((asm.code >> 11) & 0x1f) << 0; # vrp
        print("nndwr %d, 0x%x" % (vrp, imm))
    elif (asm.code & 0xffe0003f)  == 0x7060003a:
        vrd = 0;
        imm = 0;
        imm |= ((asm.code >> 11) & 0x3ff) << 0; # imm
        vrd |= ((asm.code >> 6) & 0x1f) << 0; # vrd
        print("nndrd %d, 0x%x" % (vrd, imm))
    elif (asm.code & 0xffe0003f)  == 0x70a0003a:
        imm = 0;
        imm |= ((asm.code >> 6) & 0x7fff) << 0; # imm
        print("nncmd 0x%x" % (imm))
    elif (asm.code & 0xffe0003f)  == 0x7080003a:
        vwrp = 0;
        imm = 0;
        imm |= ((asm.code >> 16) & 0x1f) << 5; # imm
        imm |= ((asm.code >> 6) & 0x1f) << 0; # imm
        vwrp |= ((asm.code >> 11) & 0x1f) << 0; # vwrp
        print("nnmac %d, 0x%x" % (vwrp, imm))
    else:
        print("========== error code ==========", file=sys.stderr)
        print(asm.code, file=sys.stderr)
        print("========== error code ==========", file=sys.stderr)
        assert(0)
