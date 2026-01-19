# Release Notes

## Version 0.1.0 - January 17, 2026

### 🎉 Initial Release

We are proud to announce the first public release of Timezone Tool! This version provides a solid foundation for tracking multiple time zones with a clean, intuitive interface.

---

## 🚀 New Features

### Core Functionality
- **Multi-City Time Display**: Monitor real-time time for multiple cities simultaneously
- **Comprehensive City Database**: Access to 319,927 cities worldwide with accurate timezone information
- **Smart City Search**: Quick search functionality with autocomplete suggestions
- **Custom City Lists**: Create and manage your own list of favorite cities

### Time Display Customization
- **Flexible Time Formats**: Switch between 24-hour and 12-hour formats
- **Seconds Display**: Optional seconds in time display
- **Multiple Date Formats**: Support for YYYY-MM-DD, MM/DD/YYYY, and DD/MM/YYYY formats
- **Day Display Options**: Show abbreviated or full day names
- **Toggle Options**: Show/hide date and day information independently

### Internationalization
- **8 Languages Supported**:
  - 🇨🇳 Chinese (中文)
  - 🇬🇧 English
  - 🇯🇵 Japanese (日本語)
  - 🇰🇷 Korean (한국어)
  - 🇫🇷 French (Français)
  - 🇩🇪 German (Deutsch)
  - 🇪🇸 Spanish (Español)
  - 🇵🇹 Portuguese (Português)

### System Integration
- **System Tray Icon**: Minimal system resource usage with background operation
- **Auto-Start**: Optional automatic launch at system startup
- **Quick Access**: Double-click tray icon to show/hide timezone window
- **Real-time Tooltips**: Live time display in tray icon tooltip

### User Interface
- **Modern Design**: Clean, Windows-style interface with smooth animations
- **Intuitive Settings**: Easy-to-navigate settings window
- **Drag-and-Drop**: Reorder cities with simple up/down buttons
- **Responsive Layout**: Adapts to different screen sizes

---

## 📦 Package Information

### Distribution File
- **Filename**: `timezone-v0.1-windows-x64.zip`
- **Size**: 26.5 MB
- **Platform**: Windows 10+ (64-bit)
- **Architecture**: x64

### Package Contents
- **Executable**: `apptimezone.exe` (360 KB)
- **Qt Framework**: All required Qt 6.10.1 libraries and plugins
- **Translations**: 8 language files (.qm format)
- **Data**: City database with 319,927 entries
- **Resources**: Application icons and assets

### System Requirements
- **OS**: Windows 10 or later (64-bit)
- **CPU**: Intel Core i3 or equivalent
- **RAM**: 2 GB minimum, 4 GB recommended
- **Disk Space**: 30 MB minimum, 50 MB recommended
- **Display**: 1024×768 minimum, 1920×1080 recommended

---

## 🔧 Technical Details

### Build Information
- **Version**: 0.1.0
- **Release Date**: January 17, 2026
- **Build System**: CMake 3.16+
- **Compiler**: MinGW 13.1.0 (64-bit)
- **Framework**: Qt 6.10.1
- **Language**: C++17

### Performance Metrics
- **Startup Time**: < 2 seconds
- **Memory Usage**: 50-100 MB
- **CPU Usage**: < 1% when idle
- **Database Load Time**: ~1 second for full city database

### Dependencies
- Qt 6.10.1 (Core, Gui, Widgets, Network, Svg)
- MinGW Runtime (libgcc, libstdc++, libwinpthread)
- Windows API (for registry operations and system tray)

---

## ✅ What's Working

- ✅ Multi-city time display with real-time updates
- ✅ City search and management
- ✅ All 8 languages fully translated (65 strings each)
- ✅ System tray integration
- ✅ Auto-start functionality
- ✅ Settings persistence
- ✅ Language switching with restart
- ✅ Time/date format customization
- ✅ City reordering
- ✅ Offline operation (no internet required)

---

## 🐛 Known Issues

### Minor Issues
- **CSS Warning**: `Unknown property box-shadow` warnings appear in logs (cosmetic only, no functional impact)
- **Signal Warning**: Occasional `signal not found` warning during initialization (no functional impact)

### Limitations
- **Windows Only**: Currently only available for Windows platform
- **64-bit Only**: No 32-bit version available
- **No Weather Data**: Weather information not included in this release

---

## 🔄 Installation Instructions

### Quick Start
1. Download `timezone-v0.1-windows-x64.zip`
2. Extract to any folder on your computer
3. Run `apptimezone.exe`
4. No installation required!

### First Run
- Application will launch with default settings
- Default language: Chinese (can be changed in settings)
- No cities added by default (add your first city in settings)
- System tray icon will appear in notification area

---

## 📚 Documentation

### Included Documentation
- **PRODUCT_INTRODUCTION.md**: Comprehensive product introduction and user guide
- **README.md**: Development and build instructions

### Online Resources
- City Database: [Geonames.org](https://download.geonames.org/export/dump/)
- Qt Documentation: [Qt 6 Documentation](https://doc.qt.io/qt-6/)

---

## 🗺️ Roadmap

### Planned Features for Future Releases
- [ ] Weather information integration
- [ ] Custom timezone creation
- [ ] Import/export city lists
- [ ] Dark mode theme
- [ ] Alarm and reminder features
- [ ] Calendar view
- [ ] Time conversion calculator
- [ ] Meeting scheduler across time zones
- [ ] Linux and macOS support
- [ ] 32-bit Windows version

---

## 🙏 Acknowledgments

### Data Sources
- **Geonames.org**: Comprehensive geographic database
- **IANA Time Zone Database**: Accurate timezone information

### Third-Party Libraries
- **Qt Framework**: Cross-platform application framework
- **MinGW**: GNU Compiler Collection for Windows

---

## 📞 Support

### Getting Help
- Check the [PRODUCT_INTRODUCTION.md](PRODUCT_INTRODUCTION.md) for detailed usage instructions
- Review the [README.md](README.md) for build and development information

### Reporting Issues
If you encounter any issues not listed in the "Known Issues" section, please report them with:
- Windows version
- Steps to reproduce
- Expected behavior
- Actual behavior
- Screenshot (if applicable)

---

## 📄 License

Timezone Tool v0.1.0 is distributed as freeware for personal and commercial use.

---

## 🎯 What's Next?

We are actively working on the next version and would love to hear your feedback! Please let us know:
- What features you'd like to see
- Any bugs you've encountered
- Suggestions for improvement
- Your overall experience with the application

Thank you for trying Timezone Tool! We hope it helps you stay connected with time zones around the world.

---

**Download**: [timezone-v0.1-windows-x64.zip](timezone-v0.1-windows-x64.zip) (26.5 MB)

**Release Date**: January 17, 2026

**Version**: 0.1.0
