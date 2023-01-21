PT Pascal v5.1 (c) 2021 Queen's University, (c) 1980 University of Toronto
February 2021

This directory contains the sources and build scripts for 
PT Pascal and S/SL, version 5.1 as of February 2021.

To make PT Pascal and S/SL on a machine where PT and S/SL 
are already installed, use the command "make install".

To make PT Pascal and S/SL on a machine where PT and S/SL 
are not already installed, download and install Gnu Pascal
(command gpc), and use the command "make -f Makefile.GPC install".

To test the PT Pascal you have made before installation,
run the tests in the test subdirectory using the ptc and pti
command scripts in that directory.

Once PT Pascal and S/SL have been made and tested, install them 
globally on your machine using the command "sudo ./Install.sh".

Editor plugins for the syntax colorization of PT and S/SL
source files using Vim are in the editors subdirectory.
See the README there for installation information.
