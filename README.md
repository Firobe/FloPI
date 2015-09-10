FloPI
=====

A musical floppy drives software for Raspberry Pi.

The wiringPi library must be installed for FloPI to work.
It is available here : https://github.com/WiringPi/WiringPi.

How to use :
- Run compile.sh
- Use Convert to convert a midi file (one channel per drive) to .txt using the following syntax : 
	./Convert song.mid > song.txt
- Connect the drive following this order :
	Drive1/DirectionPin -> WiringPiPin0, Drive1/StepPin -> WPiPin1, Drive2/DirPin -> WpiPin2, etc.
	To each channel in the MIDI file will correspond a drive.
- Run FloPi. Track : name of the .txt file. Tempo : how fast should the track be played. Mesure : how many beats per bar there are. Defnote : note of reference (36 should be fine)
