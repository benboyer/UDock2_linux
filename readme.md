# Udock

Udock is a free interactive protein docking system, intended for both naive and experienced users. This new version introduce the possibility to work with more than two molecules, and an improved camera control scheme. UDock is developed at the Conservatoire National des Arts et Metiers, France, by the Centre d'Etudes et de Recherche en Informatique et Communications (CEDRIC) and the Laboratoire de Genomique, Bioinformatique, et Applications (GBA).

## Compilation

### Linux

First install all the dependencies :

	sudo apt-get install cmake
	sudo apt-get install libboost-all-dev
	sudo apt-get install libbullet-dev
	sudo apt-get install libsdl2-dev

Go inside the main directory:
	
	mkdir build
	cd build
	cmake ..
	make

To launch:
	
	./udock

Optionally you can specify mol2 files :

	./udock ../molecules/1.mol2 ../molecules/2.mol2

Note that for now you can only launch udock from build.

### Windows

open the .sln file with visual studio.

make sure the visual studio is configured with the release and x86 option.

generate the solution.

the executable is in the release directory.


## Usage

### loading molecules

Udock accept mol2 file. You can either specify them as arguments when launching from the terminal, or add them while running by drag-n-dropping the mol2 file.

### Docking

You can manipulate the molecules with the mouse. Place grapnels ont them by ctrl + left click. Delete them with ctrl + right click. You can adjust their positions with shift + left click.

Once satisfied with the grapnels positions, you can drag them together by pressing the "space" key.

The geometry of the complex can be automatically optimized at any time by pressing the "o" key. Note that you can select which will move by selecting it in the menu. Once the optimization is done, the molecules positions will be frozen for your convenience, you can unfreeze them by pressing the corresponding button on the menu.

You can save the current conformation of your complexes by pressing the save button, a corresponding PDB file will be created in the savefile directory.

### Camera control

You can move the camera in a first person fashion by using the "w","a","s","d" key to change the position of the camera, and move the mouse while right-clicking to look around. You can also look around the focus (represented by a yellow sphere) by moving the mouse while shift + right-clicking.

Note that you can change the mouse sensitivity and inverse the y-axis inside the option menu. 

Alternatively, if you have a xbox gamepad connected, you can move the camera laterally with the left stick and rotate it with the right stick. You can move forward by pressing the right trigger and backward by pressing the left one.

### Spaceship control

By pressing the "tab" key, you can switch to the spaceship mode. You can move the spaceship by using the "w","a","s","d" key. you can roll around the forward axis with the "q" and "e" key. You can direct the view with the mouse and interact with the molecule by left clicking. You can change the mode of interaction with the "r" key.

The three mode of interaction are : modifying the position of a grapnel, pulling the molecule toward the spaceship, pushing the molecule.

You can switch between the interior and exterior view with the "v" key.

Alternatively, if you have a xbox gamepad connected, you can move the camera laterally with the left stick and rotate it with the right stick. You can move forward by pressing the right trigger and backward by pressing the left one. You can roll around the forward axis with the shoulder button. You can activate the interaction mode by pressing the "A" button and select the interaction mode with the horizontal button on the D-pad. You can change between view with the "Y" button. Finally you can reset the scene by pressing the back button.

