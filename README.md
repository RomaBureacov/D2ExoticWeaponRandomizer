# D2ExoticWeaponRandomizer
Destiny 2 Exotic Weapon Randomizer. Acts as a random wheel to choose your next exotic using simple command prompt arguments.

Intended for choosing a random exotic on your next outing based on custom parameters specified.

# Use
Under **Program**, download *WeaponsList.csv* and *D2WeaponRandomizer.exe*. The file *D2WeaponRandomizer.cpp* is the source code—should you wish to review it, modify it, or compile it yourself.

Download both required files, the *.exe* and *.csv*, into the same location, and then run the executable.

## Commands

| Command | Description |
| ------- | ----------- |
| y or Y  | Confirms the current option |
| n or N  | Rejects the current option  |
| any or Any | Confirms the current and all subsequent options |
| null or Null | Rejects the current and all subsequent options |

Note that the *WeaponsList.csv* is required to be in the same location and properly formatted as the executable or compiled program. 
If a new weapon is appended to the game, append the said weapon to the csv if it does not already exist in the csv. 

# Custom Items
If you wish to update the list yourself, or append some other items, then append items in the *WeaponsList.csv* under **Program**.

Note that, if you wish, you can replace the list with your own custom list of Destiny 2 weapons or append non-exotic weapons to the list. The only requirement is that the format is obeyed, the file name remains as *WeaponsList.csv*, and that the file remains in the same location as the executable program.

The format for adding additional weapons or creating your own list is:

| Order | Item | Possible Arguments | Description |
| ----- | ---- | ------------------ | ------------ |
| 1     | Name | *Anything* | Name of the weapon |
| 2     | Element | Kinetic, Solar, Void, Arc, Stasis, Strand | Element of the weapon. May be separated by */* if more than one element applies, such as with Hard Light |
| 3     | Slot | Kinetic, Energy, Power | What slot the weapon is in |
| 4     | Ammo | Primary, Special, Power | What ammo type the weapon uses |
| 5     | Type | Auto Rifle, Pulse Rifle, Scout Rifle, Hand Cannon, Submachine Gun, Sidearm, Bow, Fusion Rifle, Linear Fusion Rifle, Trace Rifle, Shotgun, Sniper Rifle, Breech Grenade Launcher, Grenade Launcher, Machine Gun, Rocket Launcher, Sword | What type of weapon it is |

Examples:

* Euphony,Strand,Kinetic,Special,Linear Fusion Rifle

* Hard Light,Solar/Void/Arc,Energy,Primary,Auto Rifle

* Microcosm,Kinetic,Power,Power,Trace Rifle

In addition, you can start lines with *#* to form comments in the csv file if need be. 
Empty lines are also allowed for spacing if need be. 
The very first line is ignored in the csv to work around the Byte Order Mark (BOM).

