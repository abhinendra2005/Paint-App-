# 🎨 My Paint — C++ SFML Drawing Application

A feature-rich, object-oriented Paint application built entirely in **C++** using the **SFML** graphics library. Designed with clean class hierarchies, polymorphism, and a premium dark-mode UI.

---

##  Preview

> Run the project locally to see the full dark-mode interface with drawing tools, color palette, and canvas.

---

##  Features

-  **Pencil Tool** — Smooth freehand brush strokes with custom thickness
-  **Line Tool** — Straight lines with anti-aliased caps
-  **Rectangle Tool** — Draw hollow or filled rectangles
-  **Circle Tool** — Draw hollow or filled circles
-  **Eraser Tool** — Erase strokes by painting with canvas background color
-  **Text Tool** — Click anywhere on canvas and type, with live cursor preview
-  **12-Color Palette** — Curated color grid with instant switching
-  **Fill Mode Toggle** — Switch between Hollow and Filled shapes for Rectangle & Circle tools
-  **Brush Size Control** — Adjustable thickness from 1px to 50px
-  **Undo / Redo** — Full history navigation
-  **Clear Canvas** — Wipe the canvas instantly
-  **Save / Load Project** — Serialize your drawing to a `.paint` text file and reload it
-  **Export as PNG** — Export only the canvas (excluding sidebar) as a high-quality image

---

##  Project Structure

```
MyPaint/
├── My_Paint/
│   ├── classes.h        # Abstract DrawnShape base class + derived shape classes
│   ├── classes.cpp      # Drawing math, anti-aliasing, and file serialization logic
│   ├── canvas.h         # Canvas class declarations (bounds, undo/redo, file I/O)
│   ├── canvas.cpp       # Canvas implementation (rendering, export, save/load)
│   ├── menu.h           # Button, ColorButton, and Menu class declarations
│   ├── menu.cpp         # Full sidebar UI implementation with toggle logic
│   └── main.cpp         # Application entry point, event loop, and shape coordination
└── README.md
```

---

##  Object-Oriented Design

The application follows a clean **inheritance + polymorphism** architecture:

```
DrawnShape          (Abstract Base Class)
├── BrushStroke     — Freehand path made of connected points
├── LineShape       — Straight line between two points
├── RectShape       — Axis-aligned rectangle (hollow or filled)
├── CircleShape     — Circle defined by center and radius (hollow or filled)
└── TextShape       — Text string placed at a position on the canvas
```

The `Canvas` class stores all shapes in a `std::vector<std::unique_ptr<DrawnShape>>`, leveraging polymorphism so that a single container holds every shape type. The `Menu` class encapsulates all UI elements including `Button` and `ColorButton` sub-components.

---

##  Keyboard Shortcuts

| Shortcut     | Action                        |
|-------------|-------------------------------|
| `Ctrl + Z`  | Undo last action              |
| `Ctrl + Y`  | Redo last undone action       |
| `Ctrl + S`  | Quick save project file       |
| `Ctrl + O`  | Quick load project file       |
| `Enter`     | Confirm text input            |
| `Backspace` | Delete last typed character   |
| `Escape`    | Cancel current drawing action |

---

##  Requirements

- **Visual Studio Community 2022** (or later)
- **SFML 2.6.x** — [Download here](https://www.sfml-dev.org/download.php)
  - Choose: `Visual C++ 17 (2022) — 64-bit`
- **C++17** standard or later
- **Windows OS** (uses Windows system fonts as fallback)

---

##  Setup & Build Instructions

### Step 1 — Download SFML
Download and extract SFML to `C:\SFML-2.6.1`.

### Step 2 — Open the Project
Open `MyPaint\My_Paint.slnx` in Visual Studio Community.  
Make sure the build target is set to **`x64`**.

### Step 3 — Configure Project Properties
Right-click the project → **Properties** → set **All Configurations / x64**:

| Setting | Path |
|---------|------|
| **C/C++ → Additional Include Directories** | `C:\SFML-2.6.1\include` |
| **Linker → Additional Library Directories** | `C:\SFML-2.6.1\lib` |

**Linker → Input → Additional Dependencies:**

| Configuration | Libraries |
|--------------|-----------|
| Debug | `sfml-graphics-d.lib`, `sfml-window-d.lib`, `sfml-system-d.lib` |
| Release | `sfml-graphics.lib`, `sfml-window.lib`, `sfml-system.lib` |

### Step 4 — Copy SFML DLL Files
Copy the following from `C:\SFML-2.6.1\bin` into your project folder (next to `main.cpp`):

**Debug:** `sfml-graphics-d-2.dll`, `sfml-window-d-2.dll`, `sfml-system-d-2.dll`  
**Release:** `sfml-graphics-2.dll`, `sfml-window-2.dll`, `sfml-system-2.dll`

### Step 5 — Build & Run
Press **`F5`** or click **Local Windows Debugger** to compile and launch the app.

> **Font Note:** The app automatically searches for `arial.ttf` and `segoeui.ttf` from `C:\Windows\Fonts`. No manual font setup required on standard Windows systems.

---

##  File Formats

| File | Description |
|------|-------------|
| `drawing_project.paint` | Plain-text project file storing all shape data (auto-saved to project directory) |
| `drawing.png` | Exported PNG image of the canvas only, excluding the sidebar |

---

##  Concepts Demonstrated

- Abstract base classes and pure virtual functions
- Runtime polymorphism with `virtual` dispatch
- Smart pointers (`std::unique_ptr`) for automatic memory management
- Undo/Redo using double-ended stacks
- Custom file serialization and deserialization (`std::ostream` / `std::istream`)
- Off-screen rendering with `sf::RenderTexture` for image export
- Event-driven programming with SFML's polling loop
- Mouse hit-testing for custom UI button components

---

## Screenshot of the window
<img width="1920" height="1080" alt="Screenshot 2026-07-05 012957" src="https://github.com/user-attachments/assets/a55b18d1-c790-4927-ba88-b1adb2ae3e40" />

