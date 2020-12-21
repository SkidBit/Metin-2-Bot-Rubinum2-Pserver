# Metin-2-Bot-Rubinum2-Pserver

Completely inmemory farm bot for the MT2 Rubinum private server 2020

How to build:
1. Get BlackBone https://github.com/DarthTon/Blackbone
2. Create subproject
3. Set Compiler options etc. to be the same as in the "Samples" subproject
4. Add files from the repo to the project
5. Build

(6) Inject via dll injector
(7) Press F1 to start/pause the bot (you will see in the console if the bot is running)

How it works:
When started the bot gets the player position and sets that position as an anchor for all the
following distance calculation to metin stones. The bot also enables the "SkipCollision" function.
This is done to avoid getting stuck on mobs or terrain. Then the bot looks for the closest stone, sets the
"isVisible" bit and writes the UID of the metin stone to a location that makes the player move to
and attack the stone. After the stone is dead the "pickUpCloseItems" function is called via a "thiscall".

I know that the coding is sh** but this project still was a great learning experience.
I let two windows run with the bot for 24 hours and it worked perfectly, so I guess there are no memleaks or similar.
Working as of today, December 2020.

I probably won't update the offsets after major updates, so use it as long as it works.

Big thanks to Rake and all the others at GuidedHacking for their guides the DLL template and a big thanks to DathTon, Blackbone is insane.
Allthough there could be something like a handbook for usage.

And a big f*** y** to all the people on the forums that took down their tutorials on metin reversing because they
are mad that others sell bots made with their knowledge. KNOWLEDGE SHOULD BE A FREE, OPEN, ACCESSIBLE RESOURCE!

Oh btw, if you use a VPN and VM to bypass the "2 window farm restricion", paste this in your .vmx file of your vmware image to 
disable the Rubinum VM detection (?). For me the game wouldn't start without it.

**hypervisor.cpuid.v0 = "FALSE"
board-id.reflectHost = "TRUE"
hw.model.reflectHost = "TRUE"
serialNumber.reflectHost = "TRUE"
smbios.reflectHost = "TRUE"
SMBIOS.noOEMStrings = "TRUE"
isolation.tools.getPtrLocation.disable = "TRUE"
isolation.tools.setPtrLocation.disable = "TRUE"
isolation.tools.setVersion.disable = "TRUE"
isolation.tools.getVersion.disable = "TRUE"
monitor_control.disable_directexec = "TRUE"
monitor_control.disable_chksimd = "TRUE"
monitor_control.disable_ntreloc = "TRUE"
monitor_control.disable_selfmod = "TRUE"
monitor_control.disable_reloc = "TRUE"
monitor_control.disable_btinout = "TRUE"
monitor_control.disable_btmemspace = "TRUE"
monitor_control.disable_btpriv = "TRUE"
monitor_control.disable_btseg = "TRUE"
monitor_control.restrict_backdoor = "TRUE"**

I took that from this repo: https://github.com/hzqst/VmwareHardenedLoader.

Lastly, "why do you release this?".
Because I'm done with the game and I want to spread some knowledge.

