#!/usr/bin/env python3

import re
import subprocess
import pathlib
import json


def gen_header_file(nmsis_root: pathlib.Path, filename: str):
    nn_header = nmsis_root / "NN" / "Include" / filename
    nnref_filename = re.sub(r"([^\s]+).h", r"\1_ref.h", filename)
    nnref_header = nmsis_root / "NN" / "Tests" / "Ref" / "Source" / nnref_filename
    nnref_header.parent.mkdir(parents=True, exist_ok=True)

    with open(nn_header, "r") as f:
        lines = f.readlines()

    with open(nnref_header, "w") as f:
        funcname_pattern = r"^([a-zA-Z_][a-zA-Z0-9_]*\s+[a-zA-Z_][a-zA-Z0-9_]*)\s*\("
        macro_pattern = r"(\b[A-Z][A-Z_]*)_H"

        contents = []
        for line in lines:
            line = re.sub(macro_pattern, r"\1_REF_H", line)
            line = re.sub(funcname_pattern, r"\1_ref(", line)
            contents.append(line)
        f.write("".join(contents))

    print(f"Generate header file: {nnref_header.resolve()}")


def gen_nnref_libs(libs, nmsis_root: pathlib.Path):
    for lib in libs:
        lib_file_name = f"libnmsis_nn_{lib}.a"
        lib_file = nmsis_root / "Library" / "NN" / "GCC" / lib_file_name
        dst_lib_file = (
            nmsis_root / "Library" / "NNREF" / "GCC" / (f"libnmsis_nnref_{lib}.a")
        )
        dst_lib_file.parent.mkdir(parents=True, exist_ok=True)

        if not lib_file.exists():
            print(f"{lib_file_name} not found!")
            exit(1)

        # get syms in library
        syms = subprocess.run(
            ["riscv64-unknown-elf-nm", lib_file.as_posix()], capture_output=True
        ).stdout.decode("utf-8")
        lines = syms.split("\n")
        filtered_syms = []
        for line in lines:
            groups = re.match(r"^\d{8,16}\sT\s+([a-zA-Z_][a-zA-Z0-9_]*)", line)
            if groups:
                filtered_syms.append(groups.group(1))

        # generate sym pairs file
        sym_pairs = [sym + " " + sym + "_ref" for sym in filtered_syms]
        content = "\n".join(sym_pairs) + "\n"
        sym_pairs_file = dst_lib_file.parent / (f"{lib}.txt")
        with open(sym_pairs_file, "w") as f:
            f.write(content)

        subprocess.run(
            [
                "riscv64-unknown-elf-objcopy",
                "--redefine-syms",
                sym_pairs_file.as_posix(),
                lib_file.as_posix(),
                dst_lib_file.as_posix(),
            ],
            check=True,
        )
        print(f"Generate reflib: {dst_lib_file.absolute().as_posix()}")

        # delete sym_pairs file
        sym_pairs_file.unlink(missing_ok=True)


def gen_alias(alias_target, nmsis_root: pathlib.Path):
    lib_path = nmsis_root / "Library" / "NNREF" / "GCC"
    for key, value in alias_target.items():
        origin_lib = lib_path / (f"libnmsis_nnref_{key}.a")
        for alia in value:
            alia_lib = lib_path / (f"libnmsis_nnref_{alia}.a")
            subprocess.run(
                [
                    "ln",
                    "-sf",
                    origin_lib.absolute().as_posix(),
                    alia_lib.absolute().as_posix(),
                ],
                check=True,
            )
            print(f"Generate reflib: {alia_lib.absolute().as_posix()}")


if __name__ == "__main__":
    script_dir = pathlib.Path(__file__).absolute().parent
    nmsis_root = script_dir.parent.parent

    try:
        with open(script_dir / "nmsis_nnref.json", "r") as f:
            config = json.load(f)
        libs = list(config["target"].keys())
        alias_target = config["alias_target"]
    except:
        print("Invalid config file!")
        exit(1)

    # generate header file
    gen_header_file(nmsis_root, "riscv_nnfunctions.h")
    # gen_header_file(nmsis_root, "riscv_nnsupportfunctions.h")

    # generate libraries
    gen_nnref_libs(libs, nmsis_root)

    # generate alias
    gen_alias(alias_target, nmsis_root)
