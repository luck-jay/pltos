#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import os, sys
from kconfiglib import Kconfig
from menuconfig import menuconfig

# 设置Kconfig环境变量
def mconf_set_env():
    # os.environ["MENUCONFIG_STYLE"]      = "default selection=fg:white,bg:red" # 配置menuconfig的颜色主题
    os.environ["KCONFIG_CONFIG"]        = os.path.join(".config")             # 输出的配置文件名
    os.environ["KCONFIG_CONFIG_HEADER"] = "# Generated by Kconfig Tool\n"     # 设置生成的配置文件开头的字符串，会在生成的配置文件开头的第一行进行显示。
    os.environ["KCONFIG_AUTOHEADER"]    = os.path.join("include/generated/autoconfig.h")        # 指定输出符号到C头文件的文件名
    # os.environ["CONFIG_"]               = "MYTEST"

def auto_save():
    auto_conf = "include/config/auto.conf"

    auto_conf_dir = os.path.dirname(auto_conf)
    if not os.path.exists(auto_conf_dir):
        os.makedirs(auto_conf_dir)
    autofile = open(auto_conf, 'w', encoding='utf-8')
    with open('.config', 'r') as conf:
        for line in conf.readlines():
            if line[0] != "#":
                autofile.write(line)

    autofile.close()


def mconfig(argv):
    mconf_set_env()
    kconfig = os.path.join("Kconfig") # menuconfig配置文件
    kconf = Kconfig(filename=kconfig)

    if len(argv) == 2 and argv[1] == 'savemenuconfig':
        kconf.load_config()         # 加载配置
        print(kconf.write_config()) # 保存配置到配置文件
    elif len(argv) == 2 and argv[1] == 'defconfig':
        kconf.load_allconfig("alldef.config") # 加载默认配置
        print(kconf.write_config())
    elif len(argv) == 2 and argv[1] == 'allyesconfig':
        kconf.warn = False
        for sym in kconf.unique_defined_syms:
            sym.set_value(1 if sym.choice else 2)
        for choice in kconf.unique_choices:
            choice.set_value(2)
        kconf.warn = True
        kconf.load_allconfig("allyes.config") # 尽可能设置多的配置为'y'
        print(kconf.write_config())
    elif len(argv) == 2 and argv[1] == 'allnoconfig':
        kconf.warn = False
        for sym in kconf.unique_defined_syms:
            sym.set_value(2 if sym.is_allnoconfig_y else 0)
        kconf.warn = True
        kconf.load_allconfig("allno.config") # 尽可能设置多的配置为'n'
        print(kconf.write_config())
    elif len(argv) == 2 and argv[1] == 'silentoldconfig':
        auto_save()
    else:
        print("python menuconfig ... \n")
        menuconfig(kconf)

    if len(argv) != 2 or argv[1] != 'silentoldconfig':
        auto_file_dir = os.path.dirname(os.environ["KCONFIG_AUTOHEADER"])
        if not os.path.exists(auto_file_dir):
            os.makedirs(auto_file_dir)
        kconf.write_autoconf()


if __name__ == "__main__":
    mconfig(sys.argv)