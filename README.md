# FusionClient
This is the Fusion Client. It's just a GUI for FusionLib.

Please note that this is still a very early version. Features may not work, and some things may work incorrectly.

[![Build Status](https://semaphoreci.com/api/v1/projects/8a37b235-66df-4ebd-b9cb-e6d3151e8600/421616/badge.svg)](https://semaphoreci.com/kamnxt/fusionclient)      

# How to build (from CLI):
1. Install QT 4.8 and the developments libraries from your distro package manager or you can download it from http://www.qt.io/download/ (this also downloads QT Creator)
2. Create a directory for the files (for example Fusion).
3. Clone [LibFusion](https://github.com/FusionLauncher/LibFusion)
4. Go to Fusion/LibFusion
5. `qmake`
6. `make`
7. Go back to Fusion
8. Clone this repo
9. `qmake`
10. `make`
11. Run! (you may need to set your LD_LIBRARY_PATH so the client will be able to find it), for example `LD_LIBRARY_PATH=../LibFusion:$LD_LIBRARY_PATH ./FusionClient`
