KuStudio is an open source OSC editor, recorder and player, aimed to create timeline on an audiotrack.

It's made by [Kuflex](http://kuflex.com).


![Screenshot](https://raw.githubusercontent.com/kuflex/KuStudio/master/KuStudio/doc/kustudio-shot_750.png)

##Directory contents

	KuStudio - main folder with KuStudio
	DemoProject - Test project, you can start learning KuStudio 
	              by loading this project
	OSC-Demo-oF - Test visualizer, for using with DemoProject

##Quick Start

Download binaries for OSX and Windows at https://sourceforge.net/p/kustudio 

1. Run KuStudio
2. Open project "DemoProject.kus" in KuStudio, 
3. Run demo visualization "OSC-Demo-oF"
4. Press Play in KuStudio

Introductory video explaining Quick Start: https://www.youtube.com/watch?v=2cLwTnFIWEU

User guide: [bin/KuStudio-Guide.pdf](https://github.com/kuflex/KuStudio/blob/master/KuStudio/bin/KuStudio-Guide.pdf)

##Troubleshooting

####Running binaries in OSX
At the first run you can get the security warning.
To resolve it, right-click KuStudio to show context 
menu and click there "Open". 
You will see security dialog with two buttons, "Open" and "Cancel". 
Choose "Open" to start KuStudio. 

####Running binaries in Windows
If KuStudio doesn't start, 
install Microsoft Visual C++ 2015  Redistributable (x86) 
by running vc_redist.x86_vs2015.exe. 
Alternatively, you can download it from Microsoft website.

##Details

KuStudio lets create, record and OSC tracks, synchronized with given audio track.
Audiotrack can be WAV or AIFF file.

KuStudio is inspired by famous Duration OSC editor, but has different philosophy: KuStudio stores all OSC tracks as discrete arrays, not curves, that allows to record and edit them freely. Also, KuStudio performs very stable for 10 min and longer projects.

It is made with openFrameworks and is available for OSX and Windows.

##Compiling

To compile the project you need:
OSX: XCode, openFrameworks 0.9.3.
Windows: Visual Studio 2015 C++ (Express), openFrameworks 0.9.3.
Linux: not tested.

##Support

For quick support, please write to perevalovds@gmail.com


