# Module 3: Networking
This module, when run on two instances of the Aftr Burner Engine, demonstrates simple networking by message passing between the two.
## Instructions
- Run one instance of the module with NetServerListenPort=12682 in the aftr.conf file and another instance with NetServerListenPort=12683 in the aftr.conf file.
- Press the 'n' key to spawn a new cube above the world origin. (Repeated presses will spawn cubes stacking upward.)
- Ctrl+Click a cube to select it (an outline will appear on it), then press wasdqe to translate it and ijkluo to rotate it.
- Any new cubes spawned/manipulated in one module instance will be reflected in the other. All controls can be used from either instance.