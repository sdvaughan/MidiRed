# MidiRed
MIDI connector for mobile devices and desktops

```
__/\\\\____________/\\\\_______________/\\\_____________________/\\\\\\\\\____________________________/\\\__        
 _\/\\\\\\________/\\\\\\______________\/\\\___________________/\\\///////\\\_________________________\/\\\__       
  _\/\\\//\\\____/\\\//\\\__/\\\________\/\\\___/\\\___________\/\\\_____\/\\\_________________________\/\\\__      
   _\/\\\\///\\\/\\\/_\/\\\_\///_________\/\\\__\///____________\/\\\\\\\\\\\/________/\\\\\\\\_________\/\\\__     
    _\/\\\__\///\\\/___\/\\\__/\\\___/\\\\\\\\\___/\\\___________\/\\\//////\\\______/\\\/////\\\___/\\\\\\\\\__    
     _\/\\\____\///_____\/\\\_\/\\\__/\\\////\\\__\/\\\___________\/\\\____\//\\\____/\\\\\\\\\\\___/\\\////\\\__   
      _\/\\\_____________\/\\\_\/\\\_\/\\\__\/\\\__\/\\\___________\/\\\_____\//\\\__\//\\///////___\/\\\__\/\\\__  
       _\/\\\_____________\/\\\_\/\\\_\//\\\\\\\/\\_\/\\\___________\/\\\______\//\\\__\//\\\\\\\\\\_\//\\\\\\\/\\_ 
        _\///______________\///__\///___\///////\//__\///____________\///________\///____\//////////___\///////\//__
```


*A Boilermake VI project*

Most recent server executable will be found in the `bin` folder (*Server binary is x86*).

Description:
Android app (MRApp) communicates over IP to a desktop server (MRServer), which relays MIDI inputs to another desktop client (MRDesktop). Desktop client then relays MIDI through LoopBe to a Digital Audio Workstation (DAW).

MRApp - Contains sliders and knobs to change pitch, tone, etc., acting as a normal MIDI controller.

MRServer - Relays and parses information sent from the MRApp on a remote device.

MRDesktop - Receives input data from MRServer and outputs it into a LoopBe virtual MIDI device.


_Usage:_

1) *MRSERVER SETUP*

	Server must be run on a machine with selected port and its subsequent open through command `MRServer <port>`.

2) *MRDESKTOP SETUP*
	
	Desktop client will be run through command `MRDesktop <host>`, where host is the hostname of the server. (Compilation through IntelliJ currently necessary)

3) *MRAPP SETUP*
	
	Change preset IP in source code of MRApp project to desired host, then build and run

4) *LOOPBE SETUP*

	Go to https://www.nerds.de/en/download.html and download & install LoopBe1.
