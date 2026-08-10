from __future__ import annotations

import argparse
import sys
from collections.abc import Sequence
from enum import Enum
from pathlib import Path


SCRIPT_DIR = Path(__file__).resolve().parent
DEFAULT_FONT_PATH = SCRIPT_DIR.parent / "assets" / "font3.bmp"
DEFAULT_FONT_INDEX = 2
DEFAULT_FONT_SIZE = 16
DEFAULT_LINE_SPACING = 0
DEFAULT_SCREEN_WIDTH = 320
DEFAULT_COLOR = 0xFFFF
MIN_INT16 = -32768
MAX_INT16 = 32767
MAX_UINT8 = 255
MAX_UINT16 = 65535
FONT_ASCII_FIRST = 33
FONT_ASCII_LAST = 125
FONT_GLYPHS_COUNT = FONT_ASCII_LAST - FONT_ASCII_FIRST + 1
FONT_GLYPH_END_COLOR = (0, 0, 0)
FONT_SIZE_OPTIONS = tuple(1 << exponent for exponent in range(15))


class Alignment(str, Enum):
    LEFT = "left"
    RIGHT = "right"
    CENTER = "center"


def is_power_of_two(value: int) -> bool:
    return value > 0 and (value & (value - 1)) == 0


def parse_color(value: str) -> int:
    try:
        color = int(value.strip(), 0)
    except ValueError as exc:
        raise ValueError(
            "Color must be an integer, for example 0xffff or 65535."
        ) from exc

    if not 0 <= color <= MAX_UINT16:
        raise ValueError("Color must be between 0 and 65535.")
    return color


def c_escape(value: str) -> str:
    escapes = {
        "\\": "\\\\",
        '"': '\\"',
        "\t": "\\t",
        "\b": "\\b",
        "\f": "\\f",
        "\v": "\\v",
        "\a": "\\a",
    }
    result: list[str] = []
    for character in value:
        escaped = escapes.get(character)
        if escaped is not None:
            result.append(escaped)
        elif ord(character) < 32 or ord(character) == 127:
            result.append(f"\\{ord(character):03o}")
        else:
            result.append(character)
    return "".join(result)


def split_input_lines(text: str) -> list[str]:
    normalized = text.replace("\r\n", "\n").replace("\r", "\n")
    if not normalized:
        raise ValueError("Enter at least one line of text.")

    lines = normalized.splitlines()
    if not lines:
        lines = [""]
    return lines


def divide_by_two_toward_zero(value: int) -> int:
    if value >= 0:
        return value // 2
    return -((-value) // 2)


def load_glyph_advances(font_path: Path, glyph_size: int) -> list[int]:
    try:
        from PIL import Image
    except ImportError as exc:
        raise ValueError(
            "Pillow is not installed. Install it with: pip install Pillow"
        ) from exc

    try:
        with Image.open(font_path) as image:
            rgb_image = image.convert("RGB")
            expected_width = glyph_size * FONT_GLYPHS_COUNT
            if rgb_image.height != glyph_size or rgb_image.width != expected_width:
                raise ValueError(
                    "Font BMP dimensions do not match the selected size. "
                    f"Expected {expected_width}x{glyph_size} px for "
                    f"{FONT_GLYPHS_COUNT} glyphs, got "
                    f"{rgb_image.width}x{rgb_image.height} px."
                )

            advances: list[int] = []
            for glyph_index in range(FONT_GLYPHS_COUNT):
                glyph_start_x = glyph_index * glyph_size
                advance = glyph_size
                for glyph_x in range(glyph_size):
                    source_x = glyph_start_x + glyph_x
                    if all(
                        rgb_image.getpixel((source_x, glyph_y))
                        == FONT_GLYPH_END_COLOR
                        for glyph_y in range(glyph_size)
                    ):
                        advance = glyph_x + 1
                        break
                advances.append(advance)
    except OSError as exc:
        raise ValueError(f"Cannot read font BMP: {font_path}") from exc

    return advances


def measure_text_width(
    text: str,
    font_size: int,
    glyph_advances: Sequence[int],
) -> int:
    if len(glyph_advances) != FONT_GLYPHS_COUNT:
        raise ValueError(
            f"Expected {FONT_GLYPHS_COUNT} glyph advances, got "
            f"{len(glyph_advances)}."
        )

    width = 0
    for character in text:
        if character == " ":
            width += font_size
        elif character == "\t":
            width += font_size * 4
        else:
            character_code = ord(character)
            if FONT_ASCII_FIRST <= character_code <= FONT_ASCII_LAST:
                width += glyph_advances[character_code - FONT_ASCII_FIRST]
            else:
                width += font_size
    return width


def calculate_x(
    text_width: int,
    alignment: Alignment,
    screen_width: int,
) -> int:
    if alignment is Alignment.LEFT:
        return 0
    if alignment is Alignment.RIGHT:
        return screen_width - text_width
    return divide_by_two_toward_zero(screen_width - text_width)


def validate_parameters(
    font_index: int,
    font_size: int,
    line_spacing: int,
    screen_width: int,
    color: int,
) -> None:
    if not 0 <= font_index <= MAX_UINT8:
        raise ValueError("fontIndex must be between 0 and 255.")
    if not is_power_of_two(font_size):
        raise ValueError("Font size must be a positive power of 2.")
    if line_spacing < 0:
        raise ValueError("Line spacing must not be negative.")
    if screen_width <= 0:
        raise ValueError("Screen width must be greater than 0.")
    if not 0 <= color <= MAX_UINT16:
        raise ValueError("Color must be between 0 and 65535.")


def generate_text_lines_code(
    text: str,
    font_index: int = DEFAULT_FONT_INDEX,
    font_size: int = DEFAULT_FONT_SIZE,
    line_spacing: int = DEFAULT_LINE_SPACING,
    alignment: Alignment = Alignment.LEFT,
    screen_width: int = DEFAULT_SCREEN_WIDTH,
    color: int = DEFAULT_COLOR,
    glyph_advances: Sequence[int] | None = None,
) -> str:
    validate_parameters(
        font_index=font_index,
        font_size=font_size,
        line_spacing=line_spacing,
        screen_width=screen_width,
        color=color,
    )
    lines = split_input_lines(text)
    if glyph_advances is None:
        raise ValueError("Load a font BMP before generating text lines.")

    output = ["const static text_line textLines[] = {"]
    for line_number, line in enumerate(lines):
        text_width = measure_text_width(
            text=line,
            font_size=font_size,
            glyph_advances=glyph_advances,
        )
        x = calculate_x(
            text_width=text_width,
            alignment=alignment,
            screen_width=screen_width,
        )
        y = line_number * font_size + line_spacing

        if not MIN_INT16 <= x <= MAX_INT16:
            raise ValueError(
                f"Line {line_number + 1} produces x={x}, outside the int16_t range."
            )
        if not MIN_INT16 <= y <= MAX_INT16:
            raise ValueError(
                f"Line {line_number + 1} produces y={y}, outside the int16_t range."
            )

        output.append(
            "    {"
            f'.text = "{c_escape(line)}", '
            f".x = {x}, "
            f".y = {y}, "
            f".fontIndex = {font_index}u, "
            f".color = 0x{color:04x}u"
            "},"
        )
    output.append("};")
    return "\n".join(output)


def run_gui() -> int:
    try:
        from PySide6.QtWidgets import (
            QApplication,
            QComboBox,
            QFileDialog,
            QFormLayout,
            QHBoxLayout,
            QLabel,
            QLineEdit,
            QMainWindow,
            QMessageBox,
            QPlainTextEdit,
            QPushButton,
            QSpinBox,
            QVBoxLayout,
            QWidget,
        )
    except ImportError:
        print(
            "PySide6 is not installed. Install it with: pip install PySide6",
            file=sys.stderr,
        )
        return 1

    class TextLineGeneratorWindow(QMainWindow):
        def __init__(self) -> None:
            super().__init__()
            self.setWindowTitle("Euzebia3D Text Line Generator")
            self.resize(900, 760)

            central_widget = QWidget()
            self.setCentralWidget(central_widget)
            layout = QVBoxLayout(central_widget)

            form = QFormLayout()
            layout.addLayout(form)

            self.font_path_input = QLineEdit(str(DEFAULT_FONT_PATH))
            self.font_browse_button = QPushButton("Browse...")
            font_path_row = QHBoxLayout()
            font_path_row.addWidget(self.font_path_input)
            font_path_row.addWidget(self.font_browse_button)
            form.addRow("Font BMP:", font_path_row)

            self.font_index_input = QSpinBox()
            self.font_index_input.setRange(0, MAX_UINT8)
            self.font_index_input.setValue(DEFAULT_FONT_INDEX)
            form.addRow("fontIndex:", self.font_index_input)

            self.font_size_input = QComboBox()
            for font_size in FONT_SIZE_OPTIONS:
                self.font_size_input.addItem(f"{font_size} px", font_size)
            self.font_size_input.setCurrentIndex(
                self.font_size_input.findData(DEFAULT_FONT_SIZE)
            )
            self.font_size_input.setToolTip(
                "Glyph cell size used to read the font BMP and calculate y positions."
            )
            form.addRow("Font size:", self.font_size_input)

            self.line_spacing_input = QSpinBox()
            self.line_spacing_input.setRange(0, MAX_INT16)
            self.line_spacing_input.setValue(DEFAULT_LINE_SPACING)
            self.line_spacing_input.setSuffix(" px")
            self.line_spacing_input.setToolTip(
                "Added to every y coordinate: y = line number * font size + spacing."
            )
            form.addRow("Line spacing / Y offset:", self.line_spacing_input)

            self.alignment_input = QComboBox()
            self.alignment_input.addItem("From the left", Alignment.LEFT.value)
            self.alignment_input.addItem("From the right", Alignment.RIGHT.value)
            self.alignment_input.addItem("Centered", Alignment.CENTER.value)
            form.addRow("Text alignment:", self.alignment_input)

            self.screen_width_input = QSpinBox()
            self.screen_width_input.setRange(1, MAX_INT16)
            self.screen_width_input.setValue(DEFAULT_SCREEN_WIDTH)
            self.screen_width_input.setSuffix(" px")
            form.addRow("Screen width:", self.screen_width_input)

            self.color_input = QLineEdit(f"0x{DEFAULT_COLOR:04x}")
            self.color_input.setPlaceholderText("RGB565, for example 0xffff")
            form.addRow("Color (RGB565):", self.color_input)

            input_label = QLabel("Text (one generated entry per line):")
            layout.addWidget(input_label)

            self.text_input = QPlainTextEdit()
            self.text_input.setPlaceholderText(
                "First line\nSecond line\nThird line"
            )
            layout.addWidget(self.text_input, 2)

            hint = QLabel(
                "Text width is read from the glyph end-marker columns in the font BMP. "
                "x is 0, screen width - text width, or "
                "(screen width - text width) / 2. "
                "y = line number * font size + spacing."
            )
            hint.setWordWrap(True)
            layout.addWidget(hint)

            actions = QHBoxLayout()
            self.generate_button = QPushButton("Generate C code")
            self.copy_button = QPushButton("Copy code")
            self.save_button = QPushButton("Save .inc file")
            actions.addWidget(self.generate_button)
            actions.addWidget(self.copy_button)
            actions.addWidget(self.save_button)
            layout.addLayout(actions)

            self.status_label = QLabel("Enter text and click Generate C code.")
            layout.addWidget(self.status_label)

            self.output = QPlainTextEdit()
            self.output.setReadOnly(True)
            layout.addWidget(self.output, 2)

            self.generate_button.clicked.connect(self.generate)
            self.copy_button.clicked.connect(self.copy_output)
            self.save_button.clicked.connect(self.save_output)
            self.font_browse_button.clicked.connect(self.select_font)

        def select_font(self) -> None:
            selected_file, _ = QFileDialog.getOpenFileName(
                self,
                "Select font BMP",
                str(DEFAULT_FONT_PATH.parent),
                "Bitmap files (*.bmp);;All files (*.*)",
            )
            if not selected_file:
                return

            self.font_path_input.setText(selected_file)
            try:
                from PIL import Image

                with Image.open(selected_file) as image:
                    size_index = self.font_size_input.findData(image.height)
                    if size_index >= 0:
                        self.font_size_input.setCurrentIndex(size_index)
            except (ImportError, OSError):
                pass

        def selected_alignment(self) -> Alignment:
            return Alignment(self.alignment_input.currentData())

        def selected_font_size(self) -> int:
            return int(self.font_size_input.currentData())

        def generate(self) -> bool:
            try:
                glyph_advances = load_glyph_advances(
                    font_path=Path(self.font_path_input.text().strip()),
                    glyph_size=self.selected_font_size(),
                )
                code = generate_text_lines_code(
                    text=self.text_input.toPlainText(),
                    font_index=self.font_index_input.value(),
                    font_size=self.selected_font_size(),
                    line_spacing=self.line_spacing_input.value(),
                    alignment=self.selected_alignment(),
                    screen_width=self.screen_width_input.value(),
                    color=parse_color(self.color_input.text()),
                    glyph_advances=glyph_advances,
                )
            except ValueError as exc:
                QMessageBox.warning(self, "Invalid input", str(exc))
                return False

            line_count = len(split_input_lines(self.text_input.toPlainText()))
            self.output.setPlainText(code)
            self.status_label.setText(f"Generated {line_count} text lines.")
            return True

        def ensure_output(self) -> bool:
            return bool(self.output.toPlainText()) or self.generate()

        def copy_output(self) -> None:
            if not self.ensure_output():
                return
            QApplication.clipboard().setText(self.output.toPlainText())
            self.status_label.setText("Generated code copied to the clipboard.")

        def save_output(self) -> None:
            if not self.ensure_output():
                return

            selected_file, _ = QFileDialog.getSaveFileName(
                self,
                "Save generated text lines",
                "scene_text.inc",
                "C include files (*.inc);;C source files (*.c);;All files (*.*)",
            )
            if not selected_file:
                return

            output_path = Path(selected_file)
            try:
                output_path.write_text(self.output.toPlainText() + "\n", encoding="utf-8")
            except OSError as exc:
                QMessageBox.critical(self, "Save failed", str(exc))
                return
            self.status_label.setText(f"Saved generated code to {output_path}.")

    app = QApplication(sys.argv)
    window = TextLineGeneratorWindow()
    window.show()
    return app.exec()


def non_negative_int(value: str) -> int:
    parsed = int(value)
    if parsed < 0:
        raise argparse.ArgumentTypeError("Value must not be negative.")
    return parsed


def positive_int(value: str) -> int:
    parsed = int(value)
    if parsed <= 0:
        raise argparse.ArgumentTypeError("Value must be greater than 0.")
    return parsed


def positive_power_of_two(value: str) -> int:
    parsed = int(value)
    if not is_power_of_two(parsed):
        raise argparse.ArgumentTypeError("Value must be a positive power of 2.")
    return parsed


def uint8_value(value: str) -> int:
    parsed = int(value)
    if not 0 <= parsed <= MAX_UINT8:
        raise argparse.ArgumentTypeError("Value must be between 0 and 255.")
    return parsed


def cli_color(value: str) -> int:
    try:
        return parse_color(value)
    except ValueError as exc:
        raise argparse.ArgumentTypeError(str(exc)) from exc


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Convert multiline text into a C text_line array."
    )
    input_group = parser.add_mutually_exclusive_group()
    input_group.add_argument(
        "--text",
        help="Input text. Newline characters create separate text_line entries.",
    )
    input_group.add_argument(
        "--input",
        type=Path,
        help="Read input text from a UTF-8 file.",
    )
    parser.add_argument(
        "--font-bmp",
        type=Path,
        default=DEFAULT_FONT_PATH,
        help=f"Font atlas used to measure glyph widths (default: {DEFAULT_FONT_PATH}).",
    )
    parser.add_argument(
        "--font-index",
        type=uint8_value,
        default=DEFAULT_FONT_INDEX,
        help=f"fontIndex written to every entry (default: {DEFAULT_FONT_INDEX}).",
    )
    parser.add_argument(
        "--font-size",
        type=positive_power_of_two,
        default=DEFAULT_FONT_SIZE,
        help=f"Character size used for position calculations (default: {DEFAULT_FONT_SIZE}).",
    )
    parser.add_argument(
        "--line-spacing",
        type=non_negative_int,
        default=DEFAULT_LINE_SPACING,
        help=f"Value added to every y coordinate (default: {DEFAULT_LINE_SPACING}).",
    )
    parser.add_argument(
        "--alignment",
        choices=[alignment.value for alignment in Alignment],
        default=Alignment.LEFT.value,
        help="Horizontal text alignment (default: left).",
    )
    parser.add_argument(
        "--screen-width",
        type=positive_int,
        default=DEFAULT_SCREEN_WIDTH,
        help=f"Screen width in pixels (default: {DEFAULT_SCREEN_WIDTH}).",
    )
    parser.add_argument(
        "--color",
        type=cli_color,
        default=DEFAULT_COLOR,
        help="RGB565 color as decimal or hex (default: 0xffff).",
    )
    parser.add_argument(
        "--output",
        type=Path,
        help="Write generated code to this file instead of stdout.",
    )
    parser.add_argument(
        "--no-ui",
        action="store_true",
        help="Use command-line mode. Requires --text or --input.",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    if args.text is None and args.input is None and not args.no_ui:
        return run_gui()

    if args.text is None and args.input is None:
        print("Missing --text or --input in --no-ui mode.", file=sys.stderr)
        return 2

    try:
        text = (
            args.input.read_text(encoding="utf-8")
            if args.input is not None
            else args.text
        )
        glyph_advances = load_glyph_advances(
            font_path=args.font_bmp,
            glyph_size=args.font_size,
        )
        code = generate_text_lines_code(
            text=text,
            font_index=args.font_index,
            font_size=args.font_size,
            line_spacing=args.line_spacing,
            alignment=Alignment(args.alignment),
            screen_width=args.screen_width,
            color=args.color,
            glyph_advances=glyph_advances,
        )
        if args.output is not None:
            args.output.parent.mkdir(parents=True, exist_ok=True)
            args.output.write_text(code + "\n", encoding="utf-8")
            print(f"Saved {len(split_input_lines(text))} text lines to {args.output}.")
        else:
            print(code)
    except (OSError, ValueError) as exc:
        print(f"Generation failed: {exc}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
