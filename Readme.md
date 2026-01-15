cities.txt

alternatenames  timezone

alternatenames contains multilanguage devided by ','

timezone.txt

CountryCode	TimeZoneId	GMT offset 1. Jan 2026	DST offset 1. Jul 2026	rawOffset (independant of DST)

seperator between column :/t


[Data](https://download.geonames.org/export/dump/)

cd build

cmake --build . --config Release

.\apptimezone.exe


lupdate . -ts translations/timezone_zh.ts translations/timezone_en.ts translations/timezone_ja.ts translations/timezone_ko.ts translations/timezone_fr.ts translations/timezone_de.ts translations/timezone_es.ts translations/timezone_pt.ts

python translate_ts.py

lrelease translations/*.ts