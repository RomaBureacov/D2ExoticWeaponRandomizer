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
If you wish to update the list yourself, or append some other items, then append items in the *WeaponsList.csv* under **Program**.

The format for adding additional weapons is:

| Order | Item | Possible Arguments | Description |
| ----- | ---- | ------------------ | ------------ |
| 1     | Name | *Anything* | Name of the weapon |
| 2     | Element | Kinetic, Solar, Void, Arc, Stasis, Strand | Element of the weapon. May be separated by */* if more than one element applies, such as Hard Light |
| 3     | Slot | Kinetic, Energy, Power | What slot the weapon is in |
| 4     | Ammo | Primary, Special, Power | What ammo type the weapon uses |
| 5     | Type | Auto Rifle, Pulse Rifle, Scout Rifle, Hand Cannon, Submachine Gun, Sidearm, Bow, Fusion Rifle, Linear Fusion Rifle, Trace Rifle, Shotgun, Sniper Rifle, Breech Grenade Launcher, Grenade Launcher, Machine Gun, Rocket Launcher, Sword | What type of weapon it is |

Examples:

* Euphony,Strand,Kinetic,Special,Linear Fusion Rifle

* Hard Light,Solar/Void/Arc,Energy,Primary,Auto Rifle

* Microcosm,Kinetic,Power,Power,Trace Rifle

For formatting, you can start lines with *#* to form comments in the csv file if need be. 
Empty lines are also allowed for spacing if need be. 
The very first line is ignored in the csv to work around the Byte Order Mark (BOM).

