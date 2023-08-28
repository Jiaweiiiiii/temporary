#!/usr/local/bin/python3
##        (C) COPYRIGHT Ingenic Limited.
##             ALL RIGHTS RESERVED
##
## File       : mxu3_dump.py
## Authors    : zhluo@aram.ic.jz.com
## Create Time: 2020-03-23:16:10:45
## Description:
## 
##
import os
import sys
import re
import mxu3_disasm

path_mxu3_tool       = os.getenv('INGENIC_CPU_ROOT')+"/tools/mips_isa/xburst2_v2_tools/mxu3_tools"
file_mxu3_disasm     = path_mxu3_tool+"/def_mxu3_disasm.py"
file_mxu3_disasm_new = path_mxu3_tool+"/def_mxu3_disasm_new.py"

sys.path.append(path_mxu3_tool)

def new_mxu3_dissam():
    cmds  = "rm -rf %s" %(file_mxu3_disasm_new)
    cmds += "\ncp %s %s" %(file_mxu3_disasm, file_mxu3_disasm_new)
    cmds += "\nsed -i '/def .*:/a\    s_ins = None' %s" %(file_mxu3_disasm_new)
    cmds += "\ngrep -n 'else:' %s | awk -F':' '{ print $1 }' | xargs -I {} sed -i '1,{} s/print/s_ins = /g' %s " % (file_mxu3_disasm_new, file_mxu3_disasm_new)
    cmds += "\nsed -i '/print/d' %s " % (file_mxu3_disasm_new)
    cmds += "\necho '    return s_ins' >> %s" % file_mxu3_disasm_new
    os.system(cmds)

def replace_asm_name(line):
    import def_mxu3_disasm_new
    nlist = line.split()
    if (len(nlist) >= 3):
        if nlist[0][0] == "8" or nlist[0][0] == "4":
            str1 = nlist[1]
            if (str1[0] == "4") or (str1[0] == "7"):
                #print(str1)
                new_str1      = mxu3_disasm.asm_msg
                new_str1.code = int(str1, 16)
                try:
                    str1 = def_mxu3_disasm_new.def_mxu3_disasm(new_str1)
                except BaseException:
                    #sys.stderr.write("WARNING: cannot converting '%s' to ins\n" % new_str1)
                    pass
                else:
                    line = nlist[0] + "\t" + nlist[1] + "\t" + "".join(str1) + "\n";
    return line
    
def main():
    new_mxu3_dissam()
    for root, dirs, files in os.walk("."):
        for name in files:
            result = re.search("\.dump", name)
            if result:
                file_name = os.path.join(root, name)
                print ("[ Info ] Current file name: ", file_name)
                fp = open(file_name, "r+")
                line_array = fp.readlines()
                fp.seek(0, 0)
                
                for line in line_array:
                    line = replace_asm_name(line)
                    fp.write(line)
                fp.close()
    os.system("rm -rf %s" % file_mxu3_disasm_new)
    

if __name__ == '__main__':
    main()
