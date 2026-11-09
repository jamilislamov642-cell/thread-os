# THREAD OS issue triage

Keep issues small enough to build and clear enough to test.

## Good issue shape

- state one user or kernel problem
- list the smallest useful scope
- name the expected QEMU or hardware verification
- say what is intentionally out of scope

## Labels by intent

- `kernel` for boot memory interrupts scheduling and drivers
- `desktop` for VGA views themes and input
- `docs` for guides examples and release notes
- `testing` for QEMU coverage and regressions
- `roadmap` for future capabilities

## Definition of done

A feature is done when it builds from a clean checkout the relevant test passes the user-facing behavior is documented and the README makes no stronger claim than the code supports.
