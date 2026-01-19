# Timezone Tool - Product Introduction

## Overview

Timezone Tool is a powerful and user-friendly desktop application designed for Windows users who need to track multiple time zones simultaneously. Built with Qt 6, this application provides real-time time display for cities around the world, with support for 8 different languages and extensive customization options.

## Key Features

### Multi-City Time Display
- Display real-time time information for multiple cities simultaneously
- Access to a comprehensive database of over 319,000 cities worldwide
- Easy city search and management interface
- Support for custom city lists with drag-and-drop reordering

### Flexible Time Display Options
- **Time Format**: Choose between 24-hour and 12-hour formats
- **Seconds Display**: Option to show or hide seconds in time display
- **Date Format**: Multiple international date format options (YYYY-MM-DD, MM/DD/YYYY, DD/MM/YYYY)
- **Day Format**: Display day names in abbreviated or full format

### Multi-Language Support
- 8 languages supported:
  - Chinese (中文)
  - English
  - Japanese (日本語)
  - Korean (한국어)
  - French (Français)
  - German (Deutsch)
  - Spanish (Español)
  - Portuguese (Português)

### System Integration
- **System Tray Icon**: Runs in the background with minimal system resource usage
- **Auto-Start**: Option to automatically launch at system startup
- **Quick Access**: Double-click tray icon to show/hide the timezone window
- **Real-time Tooltip**: Tray icon shows current time and application name

### User Interface
- Clean and intuitive interface design
- Modern Windows-style UI with smooth animations
- Responsive window management
- Customizable display preferences

## System Requirements

### Minimum Requirements
- **Operating System**: Windows 10 or later (64-bit)
- **Processor**: Intel Core i3 or equivalent
- **Memory**: 2 GB RAM
- **Disk Space**: 30 MB free space
- **Display**: 1024×768 resolution or higher

### Recommended Requirements
- **Operating System**: Windows 10 or later (64-bit)
- **Processor**: Intel Core i5 or equivalent
- **Memory**: 4 GB RAM
- **Disk Space**: 50 MB free space
- **Display**: 1920×1080 resolution or higher

## Installation

### Quick Start
1. Download the `timezone-v0.1-windows-x64.zip` file
2. Extract the archive to your desired location
3. Run `apptimezone.exe` to launch the application

### No Installation Required
Timezone Tool is a portable application that doesn't require installation. Simply extract and run. All dependencies are included in the package.

## Usage Guide

### Adding Cities
1. Click the **Settings** button in the tray menu or timezone window
2. Navigate to the **City Management** tab
3. Enter a city name in the search box
4. Press Enter or select from the dropdown list
5. The city will be added to your list

### Managing Cities
- **Remove City**: Select a city and click the **Delete** button
- **Reorder Cities**: Use the **Move Up** and **Move Down** buttons to reorder your city list
- **Clear All**: Remove all cities from your list

### Changing Display Settings
1. Open **Settings** from the tray menu
2. Navigate to the **General Settings** tab
3. Configure your preferred options:
   - Time format (24h/12h)
   - Show/hide seconds
   - Date format
   - Day format
   - Show/hide date
   - Show/hide day
4. Click **Save** to apply changes

### Changing Language
1. Open **Settings** from the tray menu
2. Select your preferred language from the dropdown menu
3. Click **Restart Program** to apply the language change
4. Confirm the restart when prompted

### Auto-Start Configuration
1. Open **Settings** from the tray menu
2. Navigate to the **General Settings** tab
3. Check or uncheck **Start with System** to enable/disable auto-start
4. Click **Save** to apply changes

## Technical Specifications

### Architecture
- **Framework**: Qt 6.10.1
- **Compiler**: MinGW 13.1.0 (64-bit)
- **Language**: C++17
- **Build System**: CMake

### Performance
- **Startup Time**: < 2 seconds
- **Memory Usage**: ~50-100 MB
- **CPU Usage**: < 1% when idle
- **Database**: 319,927 cities with timezone information

### File Structure
```
timezone/
├── apptimezone.exe          # Main executable
├── Qt6*.dll                # Qt framework libraries
├── translations/            # Language files
│   ├── timezone_zh.qm      # Chinese
│   ├── timezone_en.qm      # English
│   ├── timezone_ja.qm      # Japanese
│   ├── timezone_ko.qm      # Korean
│   ├── timezone_fr.qm      # French
│   ├── timezone_de.qm      # German
│   ├── timezone_es.qm      # Spanish
│   └── timezone_pt.qm      # Portuguese
├── data/
│   └── cities.txt          # City database
├── icon/
│   └── timezone.svg        # Application icon
└── [Qt plugins]           # Required Qt plugins
```

## Data Sources

Timezone Tool uses comprehensive geographic and timezone data:
- **City Database**: Geonames.org (https://download.geonames.org/export/dump/)
- **Timezone Information**: IANA Time Zone Database
- **Regular Updates**: Database contains 319,927 cities worldwide

## Privacy and Security

- **No Internet Connection Required**: All data is stored locally
- **No User Data Collection**: No telemetry or analytics
- **No Cloud Services**: Complete offline functionality
- **Registry Usage**: Only for auto-start configuration (optional)

## Support and Feedback

### Known Issues
- None currently reported

### Future Enhancements
- Weather information integration
- Custom timezone creation
- Export/import city lists
- Dark mode theme
- Alarm and reminder features

### Contact
For bug reports, feature requests, or general inquiries, please refer to the project repository or contact the development team.

## License

Timezone Tool is distributed as freeware for personal and commercial use.

## Version History

### Version 0.1 (Current Release)
- Initial release
- Multi-city time display
- 8 language support
- System tray integration
- Auto-start functionality
- Comprehensive city database (319,927 cities)

---

**Thank you for using Timezone Tool!**

We hope this application helps you stay connected with time zones around the world. If you find it useful, please consider sharing it with others who might benefit from it.
