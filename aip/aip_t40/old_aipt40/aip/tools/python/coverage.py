import os
import sys
import json
import pygal


__DEBUG__ON__="NO"

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

class coverage():
    
    def __init__(self, file):
        self.file = file
        self.data = []
        self.resize = []
        self.affine = []
        self.persp = []

    def collect(self):
        with open(self.file, "r") as f:
            for line in f:
                self.data.append(json.loads(line))

        for item in self.data:
            if item['mode'] == 'RSZ':
                self.resize.append(item)
            elif item['mode'] == 'AFFINE':
                self.affine.append(item)
            elif item['mode'] == 'PERSP':
                self.persp.append(item)

    def analysis_resize(self):
        times = [0] * 27
        for item in self.resize:
            if item['src_format'] == 'NV12':
                if item['dst_format'] == 'BGRA':
                    times[0] += 1
                elif item['dst_format'] == 'GBRA':
                    times[1] += 1
                elif item['dst_format'] == 'RBGA':
                    times[2] += 1
                elif item['dst_format'] == 'BRGA':
                    times[3] += 1
                elif item['dst_format'] == 'GRBA':
                    times[4] += 1
                elif item['dst_format'] == 'RGBA':
                    times[5] += 1
                elif item['dst_format'] == 'ABGR':
                    times[6] += 1
                elif item['dst_format'] == 'AGBR':
                    times[7] += 1
                elif item['dst_format'] == 'ARBG':
                    times[8] += 1
                elif item['dst_format'] == 'ABRG':
                    times[9] += 1
                elif item['dst_format'] == 'AGRB':
                    times[10] += 1
                elif item['dst_format'] == 'ARGB':
                    times[11] += 1
                elif item['src_format'] == 'BGRA':
                    times[12] += 1
                elif item['src_format'] == 'GBRA':
                    times[13] += 1
                elif item['src_format'] == 'RBGA':
                    times[14] += 1
                elif item['src_format'] == 'BRGA':
                    times[15] += 1
                elif item['src_format'] == 'GRBA':
                    times[16] += 1
                elif item['src_format'] == 'RGBA':
                    times[17] += 1
                elif item['src_format'] == 'ABGR':
                    times[18] += 1
                elif item['src_format'] == 'AGBR':
                    times[19] += 1
                elif item['src_format'] == 'ARBG':
                    times[20] += 1
                elif item['src_format'] == 'ABRG':
                    times[21] += 1
                elif item['src_format'] == 'AGRB':
                    times[22] += 1
                elif item['src_format'] == 'ARGB':
                    times[23] += 1
                elif item['src_format'] == 'FMU2':
                    times[24] += 1
                elif item['src_format'] == 'FMU4':
                    times[25] += 1
                elif item['src_format'] == 'FMU8':
                    times[26] += 1
        hist = pygal.Bar()
        hist.title = "resize mode statics"
        hist.x_labels = ["NV12", ]
        hist.x_title = ""
        hist.y_title = ""
        hist.add("num", times)
        hist.render_to_file("coverage_resize.svg")

    
if __name__=="__main__":
    if len(sys.argv) != 2:
        usage()
    elif sys.argv[1]=="--help" or sys.argv[1]=="-h":
        usage()
    else:
        conv = coverage("recoder")
        conv.collect()
        conv.analysis_resize()
