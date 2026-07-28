## Brom defuse

This directory contains a BROM payload that can be used to disable security checks on MediaTek devices vulnerable to Kamakiri2 (Linecode).

### Building

You'll need the arm-none-eabi toolchain to build the payload.
To build the payload, run:
```bash
make clean
make
```

### License
This payload is licensed under the *AGPL-3.0-or-later* License, copyright (c) 2025-2026 Shomy.

The code has been written from scratch, but it is inspired and built upon studying the generic patcher by [@bkerler](https://github.com/bkerler) and [@k4y0z](https://github.com/chaosmaster).
See the [LICENSE](../LICENSE.agpl) file for details.

### Disclaimer

This payload is intended for educational purposes only. Use it at your own risk. The author is not responsible for any damage caused by using this payload.
