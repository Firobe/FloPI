FloPI
=====

A musical floppy drives software for Raspberry Pi.

The wiringPi library must be installed for FloPI to work.
It is available here : https://github.com/WiringPi/WiringPi.

How to use :
- Run compile.sh
- Get a MIDI file which have only one voice per channel and no more channel than the number of drives connected.
- Connect the drives following this order :
	Drive1/DirectionPin -> WiringPiPin0, Drive1/StepPin -> WPiPin1, Drive2/DirPin -> WpiPin2, etc.
	To each channel in the MIDI file will correspond a drive.
- Run FloPI
