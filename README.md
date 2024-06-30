# D2ExoticWeaponRandomizer
A simple command-line randomizer for randomizing which exotic weapon to use.

# Use
Under **Program**, download *WeaponsList.csv* and *D2WeaponRandomizer.exe*. The file *D2WeaponRandomizer.cpp* is the source code, if you wish to review it, modify it, or compile it yourself.

Download both required files, the *.exe* and *.csv*, into the same location, and then run the executable.

## Commands

| Command | Description |
| ------- | ----------- |
| y or Y  | Confirms the selection |
| n or N  | Rejects the selection  |
| any or Any | Confirms the current and all subsequent selections |
| null or Null | Rejects the current and all subsequent selection |

Note that the WeaponsList.csv is required to be in the same location and properly formatted as the executable or compiled program. 
If a new weapon is appended to the game, append the said weapon to the csv. 
If a new weapon type is appended to the game, the source code will need to be modified to justify such. I will attempt to update it in such events.

# Custom Items
If you wish to update the list yourself, or append some other items, then append items in the *WeaponsList.csv* under **Required Files**.

For formatting, you can start lines with *#* to form comments in the csv file if need be. 
Empty lines are also allowed for spacing if need be. 
The very first line is ignored in the csv to work around the Byte Order Mark (BOM), put whatever you want there if you want.

