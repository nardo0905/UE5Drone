# UE5 Drone project
Game Engine Architecture and Development course project based on the main branch of this repo.
# How to setup the project

* Download Git from [here](https://git-scm.com/)   
* Download Git LFS from [here](https://git-lfs.github.com/)   
* Open your command-line interpreter and type:    
git clone https://github.com/k1p1/UE5TopDownARPG.git FOLDERPATH   
where FOLDERPATH is the path to the folder you want to store the project in (for example C:\Projects\)
* Mouse right click on UE5TopDownARPG.uproject -> "Generate Visual Studio Project Files"
# Project Overview
- The drone can fly around in 3D space
- It can shoot, deal and take damage
- It has physics simulation for the rotation of the body and movement when stationary (not animation, everything is code based)
- User interface for the drone (current health, current speed, status of toggle-able actions)
- Switchable camera perspective
- Switchable flying modes
# Classes Overview

- `Drone.h`
	This is a class that extends `APawn`. It's the "body" of the project. It includes logic for taking damage, implementing the components of the pawn, handling of the UI, and properties for shaking, tilting, max health. The collision is handled by the static mesh generated for the body mesh (the model for the drone is downloaded from free from here: [drone](https://www.cgtrader.com/free-3d-models/character/sci-fi-character/construction-robot-worker)). It has a `UCameraComponent` attached to a `USpringArmComponent`, which is attached to the collision mesh. It uses `UFloatingPawnMovement` for the movement component.

- `DroneController.h`
	This is a class that extends `APlayerController`. It's the "brains" of the project. It includes logic for setting up the enhanced input system for the drone and logic for the movement of the drone.
	
- `DroneShot.h`
	This is a class that extends `AActor`. It serves as a projectile class for the shot that the drone fires. It handles logic for hitting other actors and dealing damage to them. It uses `UProjectileMovementComponent`.

- `DroneHUD.h`
	This is a class that extends `UUserWidget`. It is the user interface for the drone. It shows the current health, the current speed and also weather advanced flying mode and first person mode are turned "on" or "off".

- `DroneCrosshair.h`
	This is a simple class that extends `AHUD`. It adds a crosshair to the display.
