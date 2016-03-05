# ecodata-tracker

## Dependencies:
* SQlite3
* Qt5.5
* c++11

## Installation:
- cmake CMakeLists.txt
- sudo make install

## Configuration:
Configuration file gets installed in: /usr/local/etc/ecotracker/ecotracker.conf. Modify the content of this file to specify the location in which the ecodata tracking database is stored (default: ~/.ecotracker)

## Run:
Acts as a library for other applications although a GUI is available to edit/view the db content: execute **EcoDataTrackerGUI** on command line. 
