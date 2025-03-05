#!/usr/bin/env python3
#
# SPDX-FileCopyrightText: Copyright 2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the License); you may
# not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an AS IS BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
import os

# See https://github.com/tensorflow/tensorflow/releases/tag/v2.16.1
os.environ["TF_USE_LEGACY_KERAS"] = "1"
# See https://github.com/tensorflow/tensorflow/issues/59779
os.environ["TF_CPP_MIN_LOG_LEVEL"] = "2"

import argparse
import pathlib
import sys

import Lib.test_plan


def main():
    parser = argparse.ArgumentParser(
        prog="generate_test_data",
        description="Generates reference data and test code for NMSIS-NN kernels. Tensorflow, Tflite or TFLM may be "
        "used for reference.",
    )

    script_path = pathlib.Path(__file__).parent.absolute()
    default_test_plan = script_path / "test_plan.json"
    default_schema = script_path / "schema.fbs"
    default_root = script_path.parent

    parser.add_argument(
        "-p",
        "--test_plan",
        type=pathlib.Path,
        default=default_test_plan,
        help="Path to a json test-plan",
    )
    parser.add_argument(
        "-s",
        "--test_suites",
        nargs="+",
        default=[],
        help="Generate specific test suites, defaults to all test suites",
    )
    parser.add_argument(
        "-t",
        "--tests",
        action="append",
        default=[],
        help="Generate specific tests, defaults to all tests",
    )
    parser.add_argument(
        "--schema",
        type=pathlib.Path,
        default=default_schema,
        help="Path to the schema-file needed for generating tflite-files with flatc",
    )
    parser.add_argument(
        "--list", action="store_true", help="Only list tests in test plan"
    )
    parser.add_argument(
        "--root",
        type=pathlib.Path,
        default=default_root.as_posix(),
        help="Path to <NMSIS-NN>/NN/Tests/UnitTest",
    )

    args = parser.parse_args()

    Lib.test_plan.generate(args)


if __name__ == "__main__":
    sys.exit(main())
