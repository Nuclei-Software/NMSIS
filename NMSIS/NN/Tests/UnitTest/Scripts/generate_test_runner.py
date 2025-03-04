#!/usr/bin/env python3

import pathlib
import sys
import subprocess


def main():
    try:
        ruby_version_result = subprocess.run(
            ["ruby", "--version"], check=True, capture_output=True, text=True
        )
        print("Ruby is installed. Version:", ruby_version_result.stdout.split()[1])
    except subprocess.CalledProcessError:
        print("Ruby is not installed.")
        return -1

    script_directory = pathlib.Path(__file__).parent.absolute()
    test_cases_directory = script_directory.parent / "TestCases"
    ruby_script_path = (
        test_cases_directory / "Common" / "Unity" / "auto" / "generate_test_runner.rb"
    )

    for test_case_dir in test_cases_directory.iterdir():
        if test_case_dir.name in {"Common", "TestData", "Utils"}:
            continue

        unity_test_file = test_case_dir / "Unity" / f"unity_{test_case_dir.name}.c"

        if not unity_test_file.exists():
            print(f"Test file {unity_test_file} does not exist.")
            return -1

        test_runner_file = (
            test_case_dir
            / "Unity"
            / "TestRunner"
            / f"unity_{test_case_dir.name}_runner.c"
        )
        test_runner_file.parent.mkdir(parents=True, exist_ok=True)

        ruby_execution_result = subprocess.run(
            [
                "ruby",
                str(ruby_script_path),
                str(unity_test_file),
                str(test_runner_file),
            ],
            check=True,
            capture_output=True,
            text=True,
        )
        if ruby_execution_result.returncode != 0:
            print(ruby_execution_result.stderr)
            return -1
        else:
            print("Generated test runner:", test_runner_file)

    return 0


if __name__ == "__main__":
    sys.exit(main())
