# QtDigitalInstrumentCluster
This is a digital instrument cluster project that when paired with an OBD II to USB cable it can display live data from the vehicle's computer (ECU). It can also display information that regular instrument clusters cannot, such as engine trouble codes, calculated engine load, intake air temperature, throtle position and much more.

This project is developed to run in a Linux environment, however you can also run it in Windows by going to serialobd.cpp and giving m_serial.waitForBytesWritten() and m_serial.waitForReadyRead() and argument of 10 milliseconds.
