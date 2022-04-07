#!/usr/bin/env python3

import os
import sys
import time
import copy
import glob
import tempfile
import re
import json
import argparse
import shutil
from prettytable import PrettyTable


def mkdirs(dir, remove_first=False):
    if os.path.isdir(dir) == False:
        os.makedirs(dir)
    else:
        if remove_first:
            shutil.rmtree(dir)
            os.makedirs(dir)
    pass

def run_command(cmd):
    try:
        print("Run command: %s" %(cmd))
        ret = os.system(cmd)
        print("return value is %d" % (ret))
        if ret == 0:
            return True
    except Exception as exc:
        print("Execute command, catch exception: %s", e)
    return False

def checkbuilderror(log):
    if os.path.isfile(log) == False:
        return False
    with open(log, 'r') as lf:
        repattern = 'make.+Error.+'
        for line in lf.readlines():
            match = re.match(repattern, line)
            if match:
                return False
    return True

class nl_build(object):
    def __init__(self, src, libprefix):
        self.nl_src = src
        self.libprefix = libprefix
        pass

    def get_build_artifacts(self, target:str):
        builddir = os.path.join("build", self.libprefix, target)
        cmakelog = os.path.join(builddir, "cmake.log")
        buildlog = os.path.join(builddir, "build.log")
        return builddir, cmakelog, buildlog

    def build(self, target:str, target_alias:list, config:dict, installdir:str, parallel="-j", norebuild=False):
        if isinstance(config, dict) == False:
            return False
        cmakefile = os.path.join(self.nl_src, "CMakeLists.txt")
        if os.path.isfile(cmakefile) == False:
            print("%s is not a cmake project, please check\n" %(self.nl_src))
            return False
        cmakeopts = ""
        abs_nlsrc = os.path.abspath(self.nl_src)
        for key in config:
            cmakeopts += "-D%s=%s " % (key, config[key])
        nl_buildir, nl_cmakelog, nl_buildlog = self.get_build_artifacts(target)
        makefile = os.path.join(nl_buildir, "Makefile")
        genmake = False
        if (os.path.isfile(makefile) == False):
            genmake = True
        elif norebuild == False:
            genmake = True
        if genmake == True:
            mkdirs(nl_buildir, True)
            print("Configure project %s for target %s, log record in %s" % (self.nl_src, target, nl_cmakelog))
            cmake_cmd = "cmake %s -S %s -B %s 2>&1 | tee %s" % (cmakeopts, abs_nlsrc, nl_buildir, nl_cmakelog)
            run_command(cmake_cmd)
        else:
            print("Reuse previous generated Makefile configured by cmake!")
        if os.path.isfile(makefile) == False:
            print("Makefile for project %s is not generated" % (self.nl_src))
            return False
        make_cmd = "make"
        parallel = parallel.strip()
        if parallel.startswith("-j"):
            make_cmd += " %s " % (parallel)
        make_cmd += " -C %s  2>&1 | tee %s" % (nl_buildir, nl_buildlog)
        print("Build project %s for target %s, log record in %s" % (self.nl_src, target, nl_buildlog))
        ret = run_command(make_cmd)
        if ret == False:
            print(">>> ERROR: Failed to build project %s for target %s, log record in %s" % (self.nl_src, target, nl_buildlog))
            return False
        if checkbuilderror(nl_buildlog) == False:
            print(">>> ERROR: Failed to build project %s for target %s, log record in %s" % (self.nl_src, target, nl_buildlog))
            return False
        mkdirs(installdir)
        abs_installdir = os.path.abspath(installdir)
        libname = "lib%s_%s.a" % (self.libprefix, target)
        ar_libcmd = 'cd %s ; find ./ -name "*.o" | xargs riscv-nuclei-elf-ar rcs %s' % (nl_buildir, libname)
        run_command(ar_libcmd)
        print("Install library %s for target %s" % (self.nl_src, target))
        install_libcmd = "cp -f %s/%s %s" % (nl_buildir, libname, abs_installdir)
        run_command(install_libcmd)

        # Install alias library
        if isinstance(target_alias, list):
            for alias in target_alias:
                print("Install %s library alias %s for target %s" % (self.nl_src, alias, target))
                alias_libname = "lib%s_%s.a" % (self.libprefix, alias)
                install_libcmd = "cp -f %s/%s %s/%s" % (nl_buildir, libname, abs_installdir, alias_libname)
                run_command(install_libcmd)

        return True

JSON_OK=0
JSON_NOFILE=1
JSON_INVAILD=2
def load_json(file):
    if isinstance(file, str) == False or os.path.isfile(file) == False:
        return JSON_NOFILE, None
    try:
        data = json.load(open(file, 'r'))
        return JSON_OK, data
    except:
        print("Error: %s is an invalid json file!" % (file))
        return JSON_INVAILD, None

def get_buildcfgs(jsonconfig:dict):
    glcfg = jsonconfig.get("global", dict())
    percfgs = jsonconfig.get("target", None)
    buildcfgs = dict()
    try:
        for key in percfgs:
            tmpcfg = glcfg.copy()
            tmpcfg.update(percfgs[key])
            buildcfgs[key] = tmpcfg
    except:
        return None
    return buildcfgs

def get_aliascfgs(jsonconfig:dict):
    aliascfgs = jsonconfig.get("alias_target", dict())
    return aliascfgs

def strip_library(libroot):
    if os.path.isdir(libroot) == False:
        return
    strip_cmd = "riscv-nuclei-elf-strip -g %s/*.a" %(libroot)
    run_command(strip_cmd)
    pass

def install_library(libsrc, buildcfgs:dict, aliascfgs:dict, libprefix, libroot, target:str="all", strip=True, parallel="-j", ignore_fail=False, norebuild=False):
    if isinstance(buildcfgs, dict) == False:
        print("No build configuration found")
        return False
    nlb = nl_build(libsrc, libprefix)
    rst_table = PrettyTable()
    rst_table.field_names = ["Build Target", "Status", "Build Time", "Build Log"]
    runstatus = True
    if target.lower() == "all":
        print("Build all the configurations, count %d" %(len(buildcfgs)))
        for key in buildcfgs:
            start_time = time.time()
            print(">>> Build and install %s library for config %s" % (libsrc, key))
            _, _, buildlog = nlb.get_build_artifacts(key)
            target_alias = aliascfgs.get(key, [])
            ret = nlb.build(key, target_alias, buildcfgs[key], libroot, parallel, norebuild)

            cost_time = round(time.time() - start_time, 2)
            rst_table.add_row([key, ret, cost_time, buildlog])
            if ret == False:
                runstatus = False
                print(">>> ERROR failed to build %s library for config %s" % (libsrc, key))
                if ignore_fail == False:
                    break
    else:
        print("Build selected configuration %s" %(target))
        if target in buildcfgs:
            start_time = time.time()
            print(">>> Build and install %s library for config %s" % (libsrc, target))
            _, _, buildlog = nlb.get_build_artifacts(target)
            ret = nlb.build(target, buildcfgs[target], libroot, parallel, norebuild)
            cost_time = round(time.time() - start_time, 2)
            rst_table.add_row([target, ret, cost_time, buildlog])
            if ret == False:
                runstatus = False
                print(">>> ERROR failed to build %s library for config %s" % (libsrc, target))
        else:
            runstatus = False
            print(">>> ERROR: config %s not found" % (target))
    if strip:
        print("Strip libraries located in %s" % (libroot))
        strip_library(libroot)
    print("All generated library could be found in %s" % (libroot))
    print("Show build status of all the configurations")
    print(rst_table)
    return runstatus

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Nuclei Library Build Tool")
    parser.add_argument('--config', required=True, help="JSON Configuration File")
    parser.add_argument('--lib_src', default="DSP/Source", help="Where library source code's cmakefile located")
    parser.add_argument('--lib_prefix', default="nmsis_dsp", help="Library prefix")
    parser.add_argument('--lib_root', default="Library/DSP/GCC", help="Library built and generate to")
    parser.add_argument('--norebuild', action='store_true', help="Don't clean build directories, and rebuild it, just reuse previous build objects for faster build")
    parser.add_argument('--strip', action='store_true', help="If specified, the installed library will strip out debug symbols")
    parser.add_argument('--target', default="all", help="if target = all, it means run all the targets defined in config")
    parser.add_argument('--parallel', default="-j4", help="parallel build library, default -j4")
    parser.add_argument('--ignore_fail', action='store_true', help="If specified, will ignore fail even any build configuration failed")

    args = parser.parse_args()

    valid, jsoncfg = load_json(args.config)

    if valid != JSON_OK:
        print("Invalid json file %s, please check!" % (args.config))
        sys.exit(1)
    buildcfgs = get_buildcfgs(jsoncfg)
    aliascfgs = get_aliascfgs(jsoncfg)
    runrst = install_library(args.lib_src, buildcfgs, aliascfgs, args.lib_prefix, args.lib_root, args.target, args.strip, args.parallel, args.ignore_fail, args.norebuild)
    print("Build Library %s with config %s, generated into %s status: %s" %(args.lib_src, args.config, args.lib_root, runrst))
    if args.norebuild:
        print("!!!Use Caution: This build is not fully rebuilt, please take care!!!!")
    if runrst:
        sys.exit(0)
    else:
        sys.exit(1)
