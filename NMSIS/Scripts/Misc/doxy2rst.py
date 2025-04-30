#!/usr/bin/env python3
import os
import sys
from jinja2 import FileSystemLoader
from jinja2.environment import Environment

from xml.dom.minidom import parse
import xml.dom.minidom

## Jinja Templates
API_INDEX_TMPL = 'index.rst.jinja'
API_DOC_TMPL = 'apidoc.rst.jinja'
TEMPLATES_DIRIECTORY = "templates/"

def generate_file_from_template(tpl_file, render_data, file):
    tpl_env = Environment()
    tpl_env.loader = FileSystemLoader(TEMPLATES_DIRIECTORY)
    jina_tpl = tpl_env.get_template(tpl_file)
    firdir = os.path.dirname(file)
    if os.path.isdir(firdir) == False:
        os.makedirs(firdir)

    with open(file, "w") as f:
        file_content = jina_tpl.render(render_data)
        f.write(file_content)

    pass

def generate_api_index(apiindex, file):
    apiindex["header_tag"] = "=" * len(apiindex["header"])
    generate_file_from_template(API_INDEX_TMPL, apiindex, file)
    pass

def generate_api_doc(apidoc, file):
    apidoc["header_tag"] = "=" * len(apidoc["header"])
    if "modules" in apidoc:
        for module in apidoc["modules"]:
            module["header_tag"] = "-" * len(module["header"])
    generate_file_from_template(API_DOC_TMPL, apidoc, file)

def parse_doxygen_module(xmldir, apidir, module_file, dproj, apiindex_desc):
    if os.path.isfile(module_file) == False:
        return None, None
    print("parsing xml {}".format(module_file))
    apidoc_all = dict()
    apidoc = dict()
    doc_element = xml.dom.minidom.parse(module_file).documentElement
    module_desc = doc_element.getElementsByTagName("title").item(0).childNodes[0].data
    module_groupName = doc_element.getElementsByTagName("compoundname").item(0).childNodes[0].data
    module_groupName_lower = module_groupName.lower()

    apifile = os.path.join(apidir, "api_" + module_groupName_lower + ".rst")

    apidoc["header"] = module_desc
    apidoc["inner_ref"] = apiindex_desc + "_" + module_desc.lower().replace(" ", "_")
    apidoc["project"] = dproj
    apidoc["name"] = module_groupName

    apifiles = []
    innergroups = doc_element.getElementsByTagName("innergroup")
    for innergroup in innergroups:
        innergroup_refid = innergroup.getAttribute("refid")
        innergroup_file = os.path.join(xmldir, innergroup_refid + ".xml")
        if os.path.isfile(innergroup_file) == False:
            continue
        else:
            subapidir = os.path.join(apidir, module_groupName_lower)
            sub_apidoc_all, subapifile = parse_doxygen_module(xmldir, subapidir, innergroup_file, dproj, apiindex_desc)
            if sub_apidoc_all:
                apidoc_all.update(sub_apidoc_all)
                relative_subapifile = os.path.relpath(subapifile, apidir)
                apifiles.append(relative_subapifile)
    if len(apifiles) > 0:
        apidoc["apifiles"] = apifiles

    apidoc_all[module_groupName_lower] = {"file": apifile, "apidoc": apidoc}

    return apidoc_all, apifile


def generate_rst_document(apidir, prj_desc, dproj, dlink, xmldir, modules):
    if not os.path.exists(apidir):
        os.makedirs(apidir)

    apiindex = {}
    apiindex["header"] = prj_desc.upper()
    apiindex_desc = prj_desc.lower().replace(" ", "_")
    apiindex["inner_ref"] = prj_desc.lower().replace(" ", "_")
    apiindex["api_link"] = dlink
    apifiles = list()

    apidoc_all = dict()
    for module in modules:
        module = module.strip()
        module_file = os.path.join(xmldir, "group__" + module + ".xml")
        module_apidoc_all, apifile = parse_doxygen_module(xmldir, apidir, module_file, dproj, apiindex_desc)
        if module_apidoc_all:
            apidoc_all.update(module_apidoc_all)
            apifiles.append(os.path.basename(apifile))

    apiindex["apifiles"] = apifiles
    # Generate index.rst
    apiindex_file_path = os.path.join(apidir, "index.rst")
    generate_api_index(apiindex, apiindex_file_path)
    print("Generate API Index File for {} into {}".format(prj_desc, apiindex_file_path))

    # Generate api doc for each group
    for module in apidoc_all:
        apifile_path = apidoc_all[module]["file"]
        apidoc = apidoc_all[module]["apidoc"]
        print("Generate API Documentation for {} into {}".format(module, apifile_path))
        generate_api_doc(apidoc, apifile_path)

    pass

def get_modules(js_modules):
    modules = {}
    for js_module in js_modules:
        if js_module[2] == "":
            js_module[2] = js_module[2].rstrip(".html")
        module_name = js_module[2].replace("group__", "")
        module_desc = js_module[0]
        modules[module_name] = module_desc
    return modules

# Documentation Parameters
DOC_ROOT = "../../doc/"
DOXYGEN_GENROOT = os.path.join(DOC_ROOT, "build/html/doxygen/")
def generate_dsp_apidoc(doc_root):
    # TODO: need to update this modules when NMSIS DSP library sync with CMSIS DSP
    # DSP Documentation Parameters doc/build/html/doxygen/dsp/html/modules.js
    # null need to be changed to ""
    dsp_js_modules = [
        [ "Examples", "group__groupExamples.html", "group__groupExamples" ],
        [ "Basic Math Functions", "group__groupMath.html", "group__groupMath" ],
        [ "Bayesian estimators", "group__groupBayes.html", "group__groupBayes" ],
        [ "Complex Math Functions", "group__groupCmplxMath.html", "group__groupCmplxMath" ],
        [ "Controller Functions", "group__groupController.html", "group__groupController" ],
        [ "Distance Functions", "group__groupDistance.html", "group__groupDistance" ],
        [ "Fast Math Functions", "group__groupFastMath.html", "group__groupFastMath" ],
        [ "Filtering Functions", "group__groupFilters.html", "group__groupFilters" ],
        [ "Interpolation Functions", "group__groupInterpolation.html", "group__groupInterpolation" ],
        [ "Matrix Functions", "group__groupMatrix.html", "group__groupMatrix" ],
        [ "Quaternion Math Functions", "group__groupQuaternionMath.html", "group__groupQuaternionMath" ],
        [ "Statistics Functions", "group__groupStats.html", "group__groupStats" ],
        [ "Support Functions", "group__groupSupport.html", "group__groupSupport" ],
        [ "SVM Functions", "group__groupSVM.html", "group__groupSVM" ],
        [ "Transform Functions", "group__groupTransforms.html", "group__groupTransforms" ],
        [ "Window Functions", "group__groupWindow.html", "group__groupWindow" ],
        [ "Generic Types", "group__genericTypes.html", "group__genericTypes" ]
    ]

    dsp_modules = get_modules(dsp_js_modules)

    dsp_api_dir = os.path.join(doc_root, "source/dsp/api")
    dsp_doxygen_module_html = "../../doxygen/dsp/html/modules.html"
    dsp_doxygen_xml_dir = os.path.join(DOXYGEN_GENROOT, "dsp/xml")
    generate_rst_document(dsp_api_dir, "NMSIS DSP API", "nmsis_dsp", \
                          dsp_doxygen_module_html, dsp_doxygen_xml_dir, dsp_modules)

### Generate NN
def generate_nn_apidoc(doc_root):
    # TODO: need to update this modules when NMSIS NN library sync with CMSIS NN
    # get latest from doc/build/html/doxygen/nn/html/modules.js
    # null need to be changed to ""
    nn_js_modules = [
        [ "Convolutional Neural Network Example", "group__CNNExample.html", "group__CNNExample" ],
        [ "Gated Recurrent Unit Example", "group__GRUExample.html", "group__GRUExample" ],
        [ "Public", "group__Public.html", "group__Public" ],
        [ "Private", "group__groupSupport.html", "group__groupSupport" ]
    ]

    nn_modules = get_modules(nn_js_modules)

    nn_api_dir = os.path.join(doc_root, "source/nn/api")
    nn_doxygen_module_html = "../../doxygen/nn/html/modules.html"
    nn_doxygen_xml_dir = os.path.join(DOXYGEN_GENROOT, "nn/xml")
    generate_rst_document(nn_api_dir, "NMSIS NN API", "nmsis_nn", \
                          nn_doxygen_module_html, nn_doxygen_xml_dir, nn_modules)

### Generate Core
def generate_core_apidoc(doc_root):
    # TODO: need to update this modules when upgrade core APIs
    # get latest from doc/build/html/doxygen/core/html/modules.js
    # null need to be changed to ""
    core_js_modules = [
        [ "Version Control", "group__NMSIS__Core__VersionControl.html", "group__NMSIS__Core__VersionControl" ],
        [ "Peripheral Access", "group__NMSIS__Core__PeriphAccess.html", "group__NMSIS__Core__PeriphAccess" ],
        [ "Compiler Control", "group__NMSIS__Core__CompilerControl.html", "group__NMSIS__Core__CompilerControl" ],
        [ "Core CSR Encodings", "group__NMSIS__Core__CSR__Encoding.html", "group__NMSIS__Core__CSR__Encoding" ],
        [ "Register Define and Type Definitions", "group__NMSIS__Core__Registers.html", "group__NMSIS__Core__Registers" ],
        [ "Core CSR Register Access", "group__NMSIS__Core__CSR__Register__Access.html", "group__NMSIS__Core__CSR__Register__Access" ],
        [ "Intrinsic Functions for CPU Intructions", "group__NMSIS__Core__CPU__Intrinsic.html", "group__NMSIS__Core__CPU__Intrinsic" ],
        [ "Interrupts and Exceptions", "group__NMSIS__Core__IntExc.html", "group__NMSIS__Core__IntExc" ],
        [ "PLIC Interrupt", "group__NMSIS__Core__IntPlic.html", "group__NMSIS__Core__IntPlic" ],
        [ "CIDU Functions", "group__NMSIS__Core__CIDU.html", "group__NMSIS__Core__CIDU" ],
        [ "SysTimer Functions", "group__NMSIS__Core__SysTimer.html", "group__NMSIS__Core__SysTimer" ],
        [ "FPU Functions", "group__NMSIS__Core__FPU__Functions.html", "group__NMSIS__Core__FPU__Functions" ],
        [ "Intrinsic Functions for Vector Instructions", "group__NMSIS__Core__Vector__Intrinsic.html", "group__NMSIS__Core__Vector__Intrinsic" ],
        [ "Intrinsic Functions for Bitmanipulation Instructions", "group__NMSIS__Core__Bitmanip__Intrinsic.html", "" ],
        [ "Intrinsic Functions for SIMD Instructions", "group__NMSIS__Core__DSP__Intrinsic.html", "group__NMSIS__Core__DSP__Intrinsic" ],
        [ "PMP Functions", "group__NMSIS__Core__PMP__Functions.html", "group__NMSIS__Core__PMP__Functions" ],
        [ "sPMP or sMPU Functions", "group__NMSIS__Core__SPMP.html", "group__NMSIS__Core__SPMP" ],
        [ "PMA Functions", "group__NMSIS__Core__PMA__Functions.html", "group__NMSIS__Core__PMA__Functions" ],
        [ "Cache Functions", "group__NMSIS__Core__Cache.html", "group__NMSIS__Core__Cache" ],
        [ "ARM Compatiable Functions", "group__NMSIS__Core__ARMCompatiable__Functions.html", "group__NMSIS__Core__ARMCompatiable__Functions" ],
        [ "System Device Configuration", "group__NMSIS__Core__SystemConfig.html", "group__NMSIS__Core__SystemConfig" ]
    ]

    core_modules = get_modules(core_js_modules)

    core_api_dir = os.path.join(doc_root, "source/core/api")
    core_doxygen_module_html = "../../doxygen/core/html/modules.html"
    core_doxygen_xml_dir = os.path.join(DOXYGEN_GENROOT, "core/xml")
    generate_rst_document(core_api_dir, "NMSIS Core API", "nmsis_core", \
                          core_doxygen_module_html, core_doxygen_xml_dir, core_modules)

## Generate DSP/NN API Doc
print("Please build the doc first, and then check whether the core_js_modules/dsp_js_modules/nn_js_modules need to be update or not!")
os.system("sleep 3")
GEN_DOC_ROOT = "."
generate_dsp_apidoc(GEN_DOC_ROOT)
generate_nn_apidoc(GEN_DOC_ROOT)
## Please only copy the dsp api part for NMSIS-Core
## keep others as it before
generate_core_apidoc(GEN_DOC_ROOT)
