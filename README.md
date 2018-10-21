# MidiRed
========MIDIRED========
MIDI connector for mobile devices and desktops

/*
__/\\\\____________/\\\\_______________/\\\_____________________/\\\\\\\\\____________________________/\\\__        
 _\/\\\\\\________/\\\\\\______________\/\\\___________________/\\\///////\\\_________________________\/\\\__       
  _\/\\\//\\\____/\\\//\\\__/\\\________\/\\\___/\\\___________\/\\\_____\/\\\_________________________\/\\\__      
   _\/\\\\///\\\/\\\/_\/\\\_\///_________\/\\\__\///____________\/\\\\\\\\\\\/________/\\\\\\\\_________\/\\\__     
    _\/\\\__\///\\\/___\/\\\__/\\\___/\\\\\\\\\___/\\\___________\/\\\//////\\\______/\\\/////\\\___/\\\\\\\\\__    
     _\/\\\____\///_____\/\\\_\/\\\__/\\\////\\\__\/\\\___________\/\\\____\//\\\____/\\\\\\\\\\\___/\\\////\\\__   
      _\/\\\_____________\/\\\_\/\\\_\/\\\__\/\\\__\/\\\___________\/\\\_____\//\\\__\//\\///////___\/\\\__\/\\\__  
       _\/\\\_____________\/\\\_\/\\\_\//\\\\\\\/\\_\/\\\___________\/\\\______\//\\\__\//\\\\\\\\\\_\//\\\\\\\/\\_ 
        _\///______________\///__\///___\///////\//__\///____________\///________\///____\//////////___\///////\//__
*/


*A Boilermake VI project*

Description:
Android app (MRApp) communicates over IP to a desktop server (MRServer), which relays MIDI inputs to another desktop client (MRDesktop). Desktop client then relays MIDI through LoopBe to a Digital Audio Workstation (DAW).

MRApp - Contains sliders and knobs to change pitch, tone, etc acting as a normal MIDI controller.

MRServer - Program intended to relay and parse information from the MRApp code to the MRServer.

MRDesktop - Recieves input data from MRServer and outputs it into a LoopBe virtual MIDI device.


Usage:

1) MRSERVER SETUP
	Server must be run on a machine with ports 7781 and 7782 open.

2) MRDESKTOP SETUP
	Desktop client will be run through command-line ( MRDesktop <host> ), where host is the hostname of the server.

3) MRAPP SETUP
	Download APK and run. App will prompt for the hostname of the server.
