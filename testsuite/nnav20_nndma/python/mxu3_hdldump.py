#!/usr/bin/env python3
 #
 #        (C) COPYRIGHT Ingenic Limited.
 #             ALL RIGHTS RESERVED
 #
 # File       : f-mxu3_objdump.py
 # Authors    : yfan@jesse
 # Create Time: 2019-12-17:08:34:42 PM
 # Update Time: 2020-05-14:11:42:46 AM
 # Description:
 # 
 ##

import os
import sys
import socket
import argparse
import subprocess

path_mxu3_tool = os.getenv('INGENIC_CPU_ROOT')+"/tools/mips_isa/xburst2_v2_tools/mxu3_tools"
file_mxu3_disasm = path_mxu3_tool+"/mxu3_disasm.py"
sys.path.append(path_mxu3_tool)

#import re
#import json
#from pprint import pprint

# HOW TO CALL SHELL CMD:
#from   subprocess import call
#call(["gunzip", file])
#      list: \__cmd  \__arg
#
#os.system("ls")    # print result of ls  and then return 0
#os.system("lsxxx") # print error message and then return 32512

UPDATE_TIME="2020-05-14:11:42:46 AM"
__DEBUG__ON__="NO"

#{{{
def __DEBUG__(msg):
    if __DEBUG__ON__ == "YES":
        msg="__DEBUG__:"+msg+"\n"    
        sys.stdout.write(msg)

def usage():
    usage_info=\
"""
Usage: %s [option]
"""%(os.path.basename(__file__))
    sys.stdout.write(usage_info)
    sys.exit()
#}}}

def def_option():
    parse = argparse.ArgumentParser()
    parse.add_argument("asmfile",   help="Assembly file.")
    #parse.add_argument("-p", "--print",   help="Optional arg, True or False.", action="store_true")
    #parse.add_argument("-p", "--print",   help="Optional arg with default value.", default="be_summary.xlsx")
    #parse.add_argument("-v", "--verbosity", help="optional with argument", choise=[0, 1, 2])     # e.g. f-script_test.py -v [0 2 1 2 2]
    #parse.add_argument("-v", "--verbosity", help="optional with argument", action="count"  )     # e.g. f-script_test.py [-v] [-vv]
    parse.add_argument("-w", "--welcome",   help="display welcome message", action="store_true")
    return parse
def handle_option(parse):
    args = parse.parse_args()
    welcome = "Hello, "

    print("# ABOUT ####################################################")
    print("# f-mxu3_objdump.py | yfan@jesse |", UPDATE_TIME)

    if args.welcome:
        welcome += os.getlogin() + "@" + socket.gethostname() + "!"
    else:
        welcome += "world !"

    print("# RUN ######################################################")
    p = Process(args.asmfile)
    p.run()

    print("# END ######################################################")

class Process(object):
    def __init__(m, s_asm):
        m.s_asm = s_asm
        m.f_asm = None
        m.path_mxu3_hdldump_disasm = None
        m._file_exist(file_mxu3_disasm)
        m._file_exist(m.s_asm)
        m._gen_disasm_func()
        pass
    def run(m):
        import mxu3_disasm as g_asmmsg
        import mxu3_hdldump_disasm as g_disasm

        s_sed_replace = ""
        cmd_get_mxu3_ins = ["grep -P '[0-9a-f]+:\t[7|4][0-9a-f]+' -r %s | awk -F' ' '{ print $2 }'" % (m.s_asm)]
        s_mxu3_bin = subprocess.check_output(cmd_get_mxu3_ins, shell=True).decode()

        i = 0
        for s_bin in s_mxu3_bin.split():
            i_bin = int(s_bin, 16)
            if (not m._is_mxu3_bin(i_bin)):
                continue

            c_asm = g_asmmsg.asm_msg()
            c_asm.code = i_bin
            try:
                s_mxu3_ins = g_disasm.def_mxu3_disasm(c_asm)
            except BaseException:
                sys.stderr.write("WARNING: cannot converting '%s' to ins\n" % (s_bin))
                pass
            else:
                s_sed_replace += " -e 's/%s.*$/%s\t%s/g'" % (s_bin, s_bin, s_mxu3_ins)
                i = i+1
                if (i >= 2000):
                    m._sed_exec_file(s_sed_replace, m.s_asm)
                    s_sed_replace = ""
                    i = 0

        m._sed_exec_file(s_sed_replace, m.s_asm)
        #if (len(s_sed_replace) > 0):
        #    cmd_set_mxu3_ins = 'sed -i' + s_sed_replace + ' ' + m.s_asm
        #    __DEBUG__(cmd_set_mxu3_ins)
        #    os.system(cmd_set_mxu3_ins)
        #else:
        #    print("WARNING: No mxu3 bin will be converted.")
        os.system("rm -f %s" % (m.path_mxu3_hdldump_disasm))
        pass

    def _sed_exec_file(m, s_sed_replace, s_file):
        if (len(s_sed_replace) > 0):
            cmd_sed = 'sed -i' + s_sed_replace + ' ' + s_file
            __DEBUG__(cmd_sed)
            os.system(cmd_sed)
        else:
            print("WARNING: No mxu3 bin will be converted.")
        pass
    def _gen_disasm_func(m):
        m.path_mxu3_hdldump_disasm = path_mxu3_tool + "/mxu3_hdldump_disasm.py"
        cmds  = "rm -f %s/mxu3_hdldump_disasm.py" %(path_mxu3_tool)
        cmds += "\ncp {0}/def_mxu3_disasm.py {0}/mxu3_hdldump_disasm.py".format(path_mxu3_tool)
        cmds += "\nsed -i '/def .*:/a\    s_ins = None' %s" % (m.path_mxu3_hdldump_disasm)
        cmds += "\ngrep -nP 'else:' %s | awk -F':' '{ print $1 }' | xargs -I {} sed -i '1,{} s/print/s_ins = /g' %s " % (m.path_mxu3_hdldump_disasm, m.path_mxu3_hdldump_disasm)
        cmds += "\necho '    return s_ins' >> %s" % (m.path_mxu3_hdldump_disasm)
        __DEBUG__(cmds)
        os.system(cmds)
        pass
    def _file_exist(m, file_name):
        if (not os.path.isfile(file_name)):
            sys.stderr.write("ERROR: cannot find file: %s\n" %(file_name))
            exit()
        pass
    def _is_mxu3_bin(m, i_bin):
        OPCODE_COP2     = 0x48000000
        OPCODE_SPECIAL2 = 0x70000000

        opcode = (i_bin & 0xfc000000)

        return (opcode == OPCODE_COP2 or opcode == OPCODE_SPECIAL2)
        pass

if __name__=="__main__":
    handle_option(def_option())
    pass

