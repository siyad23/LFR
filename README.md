# Line Following Robot (LFR) with Rotary Encoder and OLED Display

This project is a Line Following Robot (LFR) system that uses a rotary encoder for menu navigation and an OLED display for user interaction. The system is built using an Arduino Nano and the Adafruit GFX library for the display.

## TODO

- Stop the counter from changing when it enters another menu
- Calibrate display calls a demo function
- Speed display changes a actual variable
- Add five more display item (exp:follow biasing turn or dmp, baising turn)
- Add the dmp editing menu

## Features

- **Rotary Encoder**: Navigate through the menu using a rotary encoder.
- **Switch Input**: A button is integrated with the rotary encoder for selection.
- **OLED Display**: Displays a menu with options and icons.
- **Menu System**: Includes options like "Calibrate," "Speed," and "DMP."
- **Debounce Logic**: Ensures stable input from the rotary encoder and switch.

## Components Used

- Arduino Nano
- Rotary Encoder
- OLED Display (SH1106)
- Push Button (integrated with the rotary encoder)

## File Structure

- **`main.cpp`**: The main entry point of the program.
- **`init.h`**: Handles basic initialization of the system.
- **`switch.h`**: Manages the rotary encoder and switch input.
- **`logo.h`**: Contains bitmap data for menu icons.
- **`display.h`**: Handles the OLED display and menu rendering.
- **`platformio.ini`**: PlatformIO configuration file.

## Installation

1. Clone this repository to your local machine.
2. Open the project in [PlatformIO](https://platformio.org/).
3. Ensure the required libraries are installed:
   - [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
4. Connect the hardware components as described in the **Wiring** section.
5. Upload the code to your Arduino Nano.

## Wiring

| Component          | Arduino Pin |
| ------------------ | ----------- |
| Rotary Encoder CLK | D4          |
| Rotary Encoder DT  | D5          |
| Rotary Encoder SW  | D6          |
| OLED Display SDA   | A4          |
| OLED Display SCL   | A5          |

## Usage

1. Power on the system.
2. Use the rotary encoder to navigate through the menu.
3. Press the button on the rotary encoder to select an option.
4. The OLED display will update based on the selected menu item.

## Menu Options

1. **Calibrate**: Displays the calibration icon and text.
2. **Speed**: Displays the speed icon and text.
3. **DMP**: Displays the DMP icon and text.

## Code Overview

### `main.cpp`

- Initializes the system using `basic_init()`, `switch_init()`, and `display_init()`.
- Handles the rotary encoder input and updates the menu display.

### `switch.h`

- Manages the rotary encoder and switch input.
- Includes debounce logic for stable input handling.

### `display.h`

- Renders the menu on the OLED display.
- Uses the Adafruit GFX library for drawing text and icons.

### `logo.h`

- Contains bitmap data for the menu icons.

### `platformio.ini`

- Configures the PlatformIO environment for the Arduino Nano.
- Specifies the required libraries and build flags.

## Dependencies

- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Acknowledgments

- [Adafruit](https://www.adafruit.com/) for their excellent libraries and hardware.
- [PlatformIO](https://platformio.org/) for providing a powerful development environment.
