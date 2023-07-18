#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import sys

dep_file_name = ""
target = ""
cmdline = ""

def print_cmdline():
    print("cmd_%s :=  %s\n" % (target, cmdline))

def parse_cmdline():
    target_file_name = target[:-2]
    try:
        fp = open(dep_file_name, "r")
        context = fp.read()
        fp.close()

        # 去除目标内容
        context = ":".join(context.split(":")[1:])
        dep_files_index = context.find(target_file_name)
        context_list =  context[dep_files_index:].split(" ")
        # 获取待编译文件
        source = context_list[0]
        # 获取待编译文件依赖的头文件
        dep_files = " ".join(context_list[1:])

        print("source_%s := %s\n" % (target, source))

        print("deps_%s := %s\n" % (target, dep_files))

        print("%s: $(deps_%s)\n" % (target, target))
        print("$(deps_%s):\n" % target)
    except FileNotFoundError:
        sys.stderr.writelines("not found file %s.\r\n" % target_file_name)

def main():
    print_cmdline()
    parse_cmdline()


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("fixdep.py: Number of incorrect parameters.")
        exit(1)

    dep_file_name = sys.argv[1]
    target = sys.argv[2]
    cmdline = sys.argv[3]
    main()