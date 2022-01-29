# HexEditor
a CLI command tool for editing binary file with hex format.
# Installation
from HexEditor directory run this command
```bash
g++ -o hexed.exe src/main.cpp src/editor.cpp
```
# Usage
after compilation, you can run the executable and then write program path as input or add an argument.
```bash
hexed.exe prog.bin
```
## Controls
- use arrow keys to move between different cells.
- **Backspace** to edit one cell.
- **Enter** to edit multiple cells (until **ESC**).
- **Ctrl + s** - exit program and save changes.
- **ESC** - to leave program **without saving**.
