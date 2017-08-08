# FormatMii
Format your 3DS system without wiping the SD card + extras. Similar to other format tools, but does a few things they do not:

* Backups movable.sed, ext/sysdata, and databases from CTRNAND, if for some reason you need them (like restoring to a pre-format state or decrypting the contents of the original /Nintendo 3DS/<id0>/<id1> on the SD card).   
* Deletes HWCAL0.dat and HWCAL1.dat from /ro/sys on CTRNAND so cfg retrieves the original files from the i2c EEPROM during the reboot after the format, in case they were somehow damaged or replaced with files from a different device (i.e. when CTRtransferring with any tool besides GM9).
* Deletes DSiWare titles; a normal System Format does this while other format tools do not, leaving the device "unclean" in comparison.

# Compilation
To compile this, you need:
* [bannertool](https://github.com/Steveice10/buildtools/tree/master/3ds)
* [devkitARM r47](https://sourceforge.net/projects/devkitpro/)
* [makerom](https://github.com/Steveice10/buildtools/tree/master/3ds)
