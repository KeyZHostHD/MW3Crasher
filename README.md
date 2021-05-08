# MW3Crasher

## Disclaimer

This software has been created purely for the purposes of academic research. It is not intended to be used to attack other systems. Project maintainers are not responsible or liable for misuse of the software. Use responsibly.

## Summary

This software can be considered a collection of exploits that worked on Call of Duty: Modern Warfare 3. Some might still work, and it is probably best if you test this software created purely for academic research on your system, preferably over your local network.
As stated in the disclaimer, it is immoral to attempt to use this software to attack other systems even if most exploits have already been fixed.

## Important

If you are concerned about CVE-2019-11333 (server patch only), I have provided a fix in another repository of mine (TeknoHelper). I should tell you that it is best that you uninstall Tekno because the client can still be RCE'd from the server, and I don't provide a patch for that, and neither do the Tekno project maintainers. I only provide a patch for the server because I think it's not fair that the Tekno project maintainers do not make their patch public.

## Compile from source

- Clone the Git repo. Do NOT download it as ZIP, that won't work.
- Update the submodules and run `premake5 vs2019` or simply use the delivered `generate.bat`.
- Visual Studio Properties -> Linker -> Input -> Module Definition File -> add module.def located in src folder
- Build via solution file in `build\s1x.sln`.

## Commands

List of key binds:
- F1 AC-130 type of wallhack
- F2 Jugg Hack
- F3 Switches to axis team
- F4 Switches to allies team
- F5 Switches to spectator team
- END Quits the server (anti-rage-quit bypass)
- F6 Attempts to crash the server using NET Chan exploit (Might just freeze the server disconnecting all players in the meantime)
- F7 Unused
- F8 Old God mode exploit
- F9 Disables old God mode exploit
- F10 Enables client console
- F11 Reserved for Steam RCE
- F12 Simple force custom class (bypass class restrictions)

## Credits

- QUADFOST posted the original exploit. I wrote this software implementing the exploit.
- [CoD4x_Server](https://github.com/callofduty4x/CoD4x_Server) For networking functions and dvar functions.