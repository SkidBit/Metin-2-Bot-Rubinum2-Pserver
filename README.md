# Metin-2-Bot-Rubinum2-Pserver

Completely inmemory farm bot for the MT2 Rubinum private server 2020

Prebuild DLL available as release now!

First of all: if you get banned by using this bot it's your own fault. Currently there seems to be no automatic bot detection but if someone
sees you botting you can get banned. And if the game updates and they put in an anticheat/antibot system you can also get banned. So take care.

How to use:
1. Download prebuild .dll from Release page and skip to 3.
2. Or: Download sources, and build with VS 2019 (Release, x86)
3. Inject via dll injector
4. If you start the bot after inject and the console closes, you injected the wrong process. There are 2 processes, make sure to inject the correct one.
5. Read the info in the console and configure the bot to your liking.
6. To completely shut down and remove the bot, press INSERT

How it works:
When started the bot gets the player position and sets that position as an anchor for all the
following distance calculation to metin stones. If the bot is really close to a stone he will prefer that stone
over the stone that is closest to the anchor to avoid a "ping pong" behaviour.
The bot also enables wallhack to avoid getting stuck on terrain and/or mobs.
After the stone died that the bot had as a target, it will collect items and search for a new target.

Big thanks to Rake and all the others at GuidedHacking for their guides and the DLL template.
And Thanks to the x64dbg project for the ntdll https://github.com/x64dbg/x64dbg.

And a big f*** y** to all the people on the forums that took down their tutorials on metin reversing because they
are mad that others sell bots made with their knowledge. KNOWLEDGE SHOULD BE A FREE, OPEN, ACCESSIBLE RESOURCE!

Oh btw, if you use a VPN and VM to bypass the "2 window farm restricion", paste this in your .vmx file of your vmware image to 
disable the Rubinum VM detection (?). For me the game wouldn't start without it.

```
hypervisor.cpuid.v0 = "FALSE"
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
monitor_control.restrict_backdoor = "TRUE"
```

I took that from this repo: https://github.com/hzqst/VmwareHardenedLoader.

Lastly, "why do you release this?".
Because I'm done with the game and I want to spread some knowledge.
