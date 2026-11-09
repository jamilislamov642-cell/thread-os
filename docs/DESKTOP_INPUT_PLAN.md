# THREAD Desktop input plan

The current desktop is command-first and safe on every VGA-only machine.

## Next input layer

- keep every command available as a keyboard fallback
- add PS/2 mouse packet decoding
- render a visible cursor without corrupting terminal output
- add focus and hover state to dock and app cards
- map Escape to close a view and a shortcut to return home
- test input behavior in QEMU before claiming mouse support

## Safety rule

Input improvements must never remove the working terminal path. If a mouse or framebuffer feature fails the system should still boot into the text desktop.
