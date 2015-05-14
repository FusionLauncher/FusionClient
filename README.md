# FusionClient
This is the Fusion Client. It's just a GUI for FusionLib.

Please note that this is still a very early version. Features may not work, and some things may work incorrectly.

[![Build Status](https://semaphoreci.com/api/v1/projects/8a37b235-66df-4ebd-b9cb-e6d3151e8600/421616/badge.svg)](https://semaphoreci.com/kamnxt/fusionclient)      

# How to build (from CLI):

1. Create a directory for the files (for example Fusion).
2. Clone [LibFusion](https://github.com/FusionLauncher/LibFusion)
3. Go to Fusion/LibFusion
4. `qmake`
5. `make`
6. Go back to Fusion
7. Clone this repo
8. `qmake`
9. `make`
10. Run! (you may need to set your LD_LIBRARY_PATH so the client will be able to find it), for example `LD_LIBRARY_PATH=../LibFusion:$LD_LIBRARY_PATH ./FusionClient`
