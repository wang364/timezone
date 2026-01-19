# Timezone Tool

A powerful and user-friendly desktop application for tracking multiple time zones simultaneously on Windows.

![Version](https://img.shields.io/badge/version-0.1.0-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows%2064--bit-lightgrey.svg)
![License](https://img.shields.io/badge/license-Freeware-green.svg)

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Screenshots](#screenshots)
- [Installation](#installation)
- [Usage](#usage)
- [Development](#development)
- [Project Structure](#project-structure)
- [Data Sources](#data-sources)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## Overview

Timezone Tool is a Qt 6-based desktop application that allows users to monitor real-time time information for multiple cities around the world. With support for over 319,000 cities and 8 different languages, it provides an intuitive interface for managing and displaying time zones.

### Key Highlights

- **Real-time Updates**: Automatic time updates every second
- **Comprehensive Database**: 319,927 cities worldwide
- **Multi-language Support**: 8 languages including English, Chinese, Japanese, Korean, French, German, Spanish, and Portuguese
- **System Integration**: System tray icon with auto-start capability
- **Customizable Display**: Flexible time, date, and day format options
- **Offline Operation**: No internet connection required

## Features

### Core Functionality
- 🌍 **Multi-City Display**: Monitor multiple cities simultaneously
- 🔍 **Smart Search**: Quick city search with autocomplete
- 📋 **Custom Lists**: Create and manage favorite city lists
- ⏱️ **Real-time Updates**: Automatic time synchronization

### Display Options
- 🕐 **Time Format**: 24-hour or 12-hour format
- ⏱️ **Seconds Display**: Optional seconds in time display
- 📅 **Date Format**: Multiple formats (YYYY-MM-DD, MM/DD/YYYY, DD/MM/YYYY)
- 📆 **Day Format**: Abbreviated or full day names
- 👁️ **Toggle Visibility**: Show/hide date and day independently

### Internationalization
- 🇨🇳 Chinese (中文)
- 🇬🇧 English
- 🇯🇵 Japanese (日本語)
- 🇰🇷 Korean (한국어)
- 🇫🇷 French (Français)
- 🇩🇪 German (Deutsch)
- 🇪🇸 Spanish (Español)
- 🇵🇹 Portuguese (Português)

### System Features
- 🖥️ **System Tray**: Background operation with minimal resource usage
- 🚀 **Auto-Start**: Optional automatic launch at system startup
- 🎯 **Quick Access**: Double-click tray icon to show/hide window
- 💾 **Settings Persistence**: Automatic saving of preferences

## Screenshots

*Coming soon*

## Installation

### Quick Start

1. Download the latest release from the [Releases](https://github.com/yourusername/timezone/releases) page
2. Extract `timezone-v0.1-windows-x64.zip` to your desired location
3. Run `apptimezone.exe`
4. No installation required!

### System Requirements

**Minimum Requirements:**
- Windows 10 or later (64-bit)
- Intel Core i3 or equivalent processor
- 2 GB RAM
- 30 MB free disk space
- 1024×768 display resolution

**Recommended Requirements:**
- Windows 10 or later (64-bit)
- Intel Core i5 or equivalent processor
- 4 GB RAM
- 50 MB free disk space
- 1920×1080 display resolution

## Usage

### Adding Cities

1. Click the **Settings** button in the tray menu or timezone window
2. Navigate to the **City Management** tab
3. Enter a city name in the search box
4. Press Enter or select from the dropdown list
5. The city will be added to your list

### Managing Cities

- **Remove City**: Select a city and click **Delete**
- **Reorder Cities**: Use **Move Up** and **Move Down** buttons
- **Clear All**: Remove all cities from your list

### Changing Settings

1. Open **Settings** from the tray menu
2. Configure your preferred options in the **General Settings** tab
3. Click **Save** to apply changes

### Changing Language

1. Open **Settings** from the tray menu
2. Select your preferred language from the dropdown menu
3. Click **Restart Program** to apply the change
4. Confirm the restart when prompted

### Enabling Auto-Start

1. Open **Settings** from the tray menu
2. Navigate to the **General Settings** tab
3. Check **Start with System**
4. Click **Save** to apply

## Development

### Prerequisites

- **Qt 6.10.1** or later
- **CMake 3.16** or later
- **MinGW 13.1.0** or compatible compiler
- **Python 3.x** (for translation scripts)

### Building from Source

#### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/timezone.git
cd timezone
```

#### 2. Configure the Build

```bash
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
```

#### 3. Build the Project

```bash
cmake --build build --config Release
```

#### 4. Run the Application

```bash
cd build
./apptimezone.exe
```

### Updating Translations

#### Extract Translatable Strings

```bash
lupdate . -ts translations/timezone_zh.ts translations/timezone_en.ts translations/timezone_ja.ts translations/timezone_ko.ts translations/timezone_fr.ts translations/timezone_de.ts translations/timezone_es.ts translations/timezone_pt.ts
```

#### Update Translation Files

```bash
cd translations
python translate_ts.py
```

#### Compile Translation Files

```bash
lrelease translations/*.ts
```

### Creating a Release Package

```bash
# Build the project
cmake --build build --config Release

# Deploy Qt dependencies
windeployqt --release --no-translations --dir release build/apptimezone.exe

# Copy resources
cp -r build/translations release/
cp -r build/data release/
cp -r build/icon release/

# Create ZIP archive
cd release
zip -r ../timezone-v0.1-windows-x64.zip *
```

## Project Structure

```
timezone/
├── CMakeLists.txt              # CMake build configuration
├── main.cpp                   # Application entry point
├── TrayIcon.h/cpp            # System tray icon implementation
├── SettingsWindow.h/cpp       # Settings dialog
├── TimezoneWindow.h/cpp       # Main timezone display window
├── CityManager.h/cpp          # City data management
├── StartupManager.h/cpp       # Auto-start functionality
├── resources.qrc             # Qt resource file
├── data/
│   └── cities.txt           # City database (319,927 cities)
├── icon/
│   └── timezone.svg         # Application icon
├── translations/
│   ├── translate_ts.py      # Translation update script
│   ├── timezone_zh.ts/qm   # Chinese translations
│   ├── timezone_en.ts/qm   # English translations
│   ├── timezone_ja.ts/qm   # Japanese translations
│   ├── timezone_ko.ts/qm   # Korean translations
│   ├── timezone_fr.ts/qm   # French translations
│   ├── timezone_de.ts/qm   # German translations
│   ├── timezone_es.ts/qm   # Spanish translations
│   └── timezone_pt.ts/qm   # Portuguese translations
├── build/                   # Build output directory
├── release/                 # Release package directory
├── PRODUCT_INTRODUCTION.md   # Product introduction
├── RELEASE_NOTES.md        # Release notes
└── README.md              # This file
```

## Data Sources

### City Database

The city database is sourced from [Geonames.org](https://download.geonames.org/export/dump/), which provides comprehensive geographic data including:

- City names in multiple languages
- Geographic coordinates
- Timezone information
- Population data
- Administrative divisions

### Timezone Information

Timezone data is based on the IANA Time Zone Database, providing accurate and up-to-date timezone information including:

- Standard time offsets
- Daylight Saving Time rules
- Historical timezone changes

## Contributing

Contributions are welcome! Please follow these guidelines:

### Reporting Issues

When reporting issues, please include:
- Windows version
- Application version
- Steps to reproduce
- Expected behavior
- Actual behavior
- Screenshots (if applicable)

### Submitting Pull Requests

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Code Style

- Follow Qt coding conventions
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and concise

## License

Timezone Tool is distributed as freeware for personal and commercial use.

## Acknowledgments

- **Qt Framework**: Cross-platform application framework
- **Geonames.org**: Comprehensive geographic database
- **IANA Time Zone Database**: Accurate timezone information
- **MinGW**: GNU Compiler Collection for Windows

## Documentation

- [Product Introduction](PRODUCT_INTRODUCTION.md) - Comprehensive product guide
- [Release Notes](RELEASE_NOTES.md) - Version history and changes

## Support

For questions, bug reports, or feature requests, please:
- Check the [Issues](https://github.com/yourusername/timezone/issues) page
- Review the [Product Introduction](PRODUCT_INTRODUCTION.md) for usage guidance
- Consult the [Release Notes](RELEASE_NOTES.md) for version information

## Version

Current version: **0.1.0**

Release date: **January 17, 2026**

---

**Made with ❤️ using Qt 6**
