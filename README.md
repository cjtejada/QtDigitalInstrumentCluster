![launcher icon](https://upload.wikimedia.org/wikipedia/commons/thumb/6/65/Circle-icons-car.svg/240px-Circle-icons-car.svg.png)


![alt text](https://github.com/cjtejada/QtDigitalInstrumentCluster/blob/master/gauges/day.PNG)
![alt text](https://github.com/cjtejada/QtDigitalInstrumentCluster/blob/master/gauges/night.PNG)
![alt text](https://github.com/cjtejada/QtDigitalInstrumentCluster/blob/master/gauges/nav.PNG)
# QtDigitalInstrumentCluster (In progress)
This is a digital instrument cluster project that when paired with an OBD II to USB cable it can display live data from the vehicle's computer (ECU). It can also display information that regular instrument clusters cannot, such as engine trouble codes, calculated engine load, intake air temperature, throtle position and much more.

This project is developed to run in a Linux environment, however you can also deploy it on Windows but you will not be able to communicate with an OBD serial port (This is only if you would like to see the Digital Cluster design). This only runs in MinGW, but with a few modifications it can run on MSVC.
# TODO
Add navigation by connecting a phone and using bluetooth tethering.
Make USB connection faster and reliable.
Develop Mobile App to use phone as controller for cluster navigation.
Deploy Qt Cluster app to raspberry pi.
Cut boot time as much as possible.
Redo the UI after all above TODOs are complete.

