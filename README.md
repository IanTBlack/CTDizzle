# CTDizzle

![ctdizzle]()


Welcome! This is the repository for the CTDizzle, an open source CTD that is built on an Arduino platform. 
For those not familiar with the CTD instrument, it is a generic sensor type that measures water conductivity, temperature, and pressure. It is commonly used in the ocean.
By doing some math, these sensors can compute other data products like salinity, density, and depth. 
This information has a variety of applications in oceanography and fisheries research.

The CTDizzle framework is considered charityware. You can use and copy it as much as you like, but you are encouraged to make
a donation to the American Civil Liberties Union (www.aclu.org) or another charity of your choice if you find it useful.

For issues related to the CTDizzle, please use GitHub's issue tracker.
If you would like to contribute to this project, please submit a pull request or consider making your work public so that others may benefit.
For questions and more information, contact Ian Black (iantimothyblack@gmail.com). Please put "CTD" in the subject line so it doesn't go to spam.

Here are some other open source CTDs used under different applications:
Arduino-based Sonde: https://github.com/glockridge/DrifterApplication
OpenCTD: https://github.com/OceanographyforEveryone/OpenCTD
PiCTD: https://github.com/haanhouse/pictd


## CTDizzle Specifications
Maximum Sampling Rate: 1Hz
Tested Depth: 80m
Maximum Depth: 100m (Acrylic Tube), 200m (Aluminum Tube)
Conductivity Accuracy: +/- 2% (After Calibration)
Conductivity Response Time: 1s
Temperature Accuracy: +/- 0.1 degC
Temperature Response Time: 3s
Pressure Accuracy: +/- 100 mbar
Pressure Response Time: 20ms
Clock Accuracy: 29 PPM (+/- 3s per day)
Battery Life: Dependent (~60 hours @ 1Hz on a 3.7v 2200 mAh)


## How do I navigate this repository?

### ctdizzle
This folder contains the Arduino code for driving the instrument. The main sketch is the sketch titled "ctdizzle.ino". 
You may notice that there are other sketches called "temperature.ino" or "conductivity.ino". These are split files that are still part of the main sketch, but 
contain functions specific to a sensor or task. For example, "clock.ino" contains functions that will initialize the real-time clock and allow 
you to ask it for the date and time. These functions are then implemented in the setup() and loop() functions in the main sketch.

### documentation
This folder contains vendor datasheets, a spreadsheet containing a list of build materials, and a build guide.
There is also an example of a "config.txt" file, which allows you to adjust the sensor sampling rate when it is placed in the root direction of the data SD card.
Please note that many of the build materials such as solder, PLA filament, tools, and epoxy can be substituted for versions that you prefer. 
Remember that this is a guide and you can change things however you want. 
If you come up with a better solution or method, please consider contributing to the open source CTD concept by creating your own repository and instructions so that others may benefit.

### prints
This folder contains the 3D prints used in the CTDizzle. If you want to modify these parts, they are provided as .ipt files which can be used with Autodesk Inventor.
If you just want to print these parts yourself or through a vendor, you can use the .stl files and a slicer to create the necessary gcode.

Parts with the word "dremel" in the filename are intended to act as a guide when using a rotary tool to remove anodized finish on the end caps. 
These parts should be made of PLA.

Parts with the word "potting" in the filename are intended to act as a jig when using potting compound to permanently connect the conductivity probe to the endcap. 
These parts should be made out of water-soluble PVA.