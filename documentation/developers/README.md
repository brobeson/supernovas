# SuperNOVAS Developer Documentation

## Branching Model

The repository uses two long-running branches:

1. [`main`](https://github.com/brobeson/supernovas/tree/main) — This is the stable branch.
   Releases come from this branch.
   At release time, if the `dev` branch passes testing, it is merged to `main`, then a new release is generated.
1. [`dev`](https://github.com/brobeson/supernovas/tree/dev) — This branch contains the latest code changes.
   These changes are generally vetted, but not yet released.

## Continuous Integration & Testing

Merges to `main` and `dev` must pass some workflows run as [GitHub Actions](https://github.com/brobeson/supernovas/actions).

### [Build & Test](https://github.com/brobeson/supernovas/actions/workflows/build.yaml) Workflow

#### Project Support/Static Analysis Job

This job runs static analysis on files like configuration files and documentation.
It also runs some static analysis that is the same for all languages.
This is a reusable workflow from [brobeson/SupportFileQuality](https://github.com/brobeson/SupportFileQuality).
Check the [README](https://github.com/brobeson/SupportFileQuality/blob/main/README.md) file for a description of the steps.

| Platform | Configuration | Generator |
| :------- | :------------ | :-------- |
| Linux    | Debug         | Ninja     |
|          | Release       | Ninja     |

## Test Coverage

### Clang

<!-- markdownlint-disable MD031 -->
<!-- cspell:ignore ctest profdata profraw -->

1. Configure a build with `SUPERNOVAS_BUILD_COVERAGE` set to `true`.
   The `debug` CMake configure preset does this.
   ```bash
   cmake --preset debug
   ```
1. Build the software.
1. Run the CTest.
   Set the environment variable `LLVM_PROFILE_FILE` to `coverage-%p-%m.profraw`.
   The `debug` CMake test preset does this.
   ```bash
   ctest --preset debug
   ```
1. Index and merge raw profile data.
   ```bash
   llvm-profdata merge -sparse build/debug/**/*.profraw -o coverage.profdata
   ```
1. Generate a console report.
   ```bash
   llvm-cov report \
     ./build/debug/tests/supernovas/sample_test \
     -instr-profile=coverage.profdata \
     -ignore-filename-regex='build/*'
   ```
1. Generate an HTML report.
   ```bash
   llvm-cov show \
     build/debug/tests/supernovas/sample_test \
     -instr-profile=coverage.profdata \
     -ignore-filename-regex='build/*' \
     -format=html \
     -output-dir=build/coverage-html
   ```
1. Export LCOV data
   ```bash
   llvm-cov export \
     build/debug/tests/supernovas/sample_test \
     -instr-profile=coverage.profdata \
     -ignore-filename-regex='build/.*' \
     -format=lcov \
     >coverage.info
   ```

#### Open Questions

1. Do we need to list every executable in the `llvm-cov report` command?
