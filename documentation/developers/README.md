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
