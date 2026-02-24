# Test Framework

Here is the test framework ported from [CMSIS-DSP Testing](https://github.com/ARM-software/CMSIS-DSP/tree/main/Testing#test-framework)

## Requirements

- Python3
- [Nuclei SDK >= 0.8.1](https://github.com/Nuclei-Software/nuclei-sdk/releases/tag/0.8.1)
- [Nuclei RISC-V Toolchain >= 2025.10](https://www.nucleisys.com/download.php)

To install the required dependencies, run the following command:

```bash
pip install -r requirements.txt
```

Make sure the toolchain is available in your `$PATH`, and set the `$NUCLEI_SDK_ROOT`
environment variable to the root directory of the Nuclei SDK.

## Available Test Suites

The full `SUITE` name is a combination of the suite name and data type.
For example, if there is a ✅ in both `StatsTests` and `Q7`, then `SUITE=StatsTestsQ7`
is available.

❌ means the test suite does not exist for that data type.

The following table lists the available test suites and supported data types:

**Data Types:**

- Q7, Q15, Q31: Fixed-point integer formats
- F16, F32, F64: Floating-point formats (16-bit, 32-bit, 64-bit)

| Suite Name        | Q7 | Q15 | Q31 | F16 | F32 | F64 |
| ----------------- | -- | --- | --- | --- | --- | --- |
| StatsTests        | ✅ | ✅  | ✅  | ✅  | ✅  | ✅  |
| SupportTests      | ✅ | ✅  | ✅  | ✅  | ✅  | ✅  |
| SupportBarTests   | ❌ | ❌  | ❌  | ✅  | ✅  | ❌  |
| InterpolationTests| ✅ | ✅  | ✅  | ✅  | ✅  | ❌  |
| QuaternionTests   | ❌ | ❌  | ❌  | ❌  | ✅  | ❌  |
| BasicTests        | ✅ | ✅  | ✅  | ✅  | ✅  | ✅  |
| ComplexTests      | ❌ | ✅  | ✅  | ✅  | ✅  | ✅  |
| FastMath          | ❌ | ✅  | ✅  | ✅  | ✅  | ✅  |
| SVM               | ❌ | ❌  | ❌  | ✅  | ✅  | ❌  |
| Bayes             | ❌ | ❌  | ❌  | ✅  | ✅  | ❌  |
| DistanceTests     | ❌ | ❌  | ❌  | ✅  | ✅  | ✅  |
| DECIM             | ❌ | ✅  | ✅  | ❌  | ✅  | ✅  |
| MISC              | ✅ | ✅  | ✅  | ✅  | ✅  | ✅  |
| FIR               | ✅ | ✅  | ✅  | ✅  | ✅  | ✅  |
| BIQUAD            | ❌ | ✅  | ✅  | ✅  | ✅  | ✅  |
| UnaryTests        | ✅ | ✅  | ✅  | ✅  | ✅  | ✅  |
| BinaryTests       | ✅ | ✅  | ✅  | ✅  | ✅  | ✅  |
| MFCC              | ❌ | ✅  | ✅  | ✅  | ✅  | ❌  |
| TransformC        | ❌ | ✅  | ✅  | ✅  | ✅  | ✅  |
| TransformR        | ❌ | ✅  | ✅  | ✅  | ✅  | ✅  |
| WindowTests       | ❌ | ❌  | ❌  | ❌  | ✅  | ✅  |
| TransformRVVC     | ❌ | ✅  | ✅  | ✅  | ✅  | ❌  |

And there are two additional test suites not listed in above table:

- DistanceTestsU32
- FastMathQ63

These two test suites are also available.

## Run Tests

The default `SUITE` in [Makefile](./Makefile) is `SUITE=BasicTestsF32`. To build
tests with different `SUITE`, specify it in the make command:

```bash
make CORE=n300fd ARCH_EXT=_xxldsp SUITE=StatsTestsQ7 clean all
```

For more make options, please refer to the [Nuclei SDK document](https://doc.nucleisys.com/nuclei_sdk/develop/index.html).

After building, run the tests using QEMU or on hardware. The test will output
"Done" on success or "Exception" on failure.

> [!NOTE]
> There is a known issue that the test suite `ComplexTestsQ15` will
> fail to run with the Nuclei 2025.10 QEMU when arch extension `_xxldspn3x` is
> enabled and vector extension is disabled. This issue will be fixed
> in a future release.
