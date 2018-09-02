textme/ contains a go service that listens for incoming sms messages from twilio and then sends them to arduino via serial.

app.ino Application that demonstrates the use of the Max7219 library. Receives a message via serial and then displays it on LED matrix. 

Max7219/ library I wrote for 8x8 7219 LED matrix. Allows for different ways to display text (scrolling marquee, wipe (per character), flash character). 


Instructions
=============

Copy Max7219/ into Arduino/libraries

Build and upload sketch via arduino IDE. 


Notes
==============

Was tested with the following kit available from amazon: https://www.amazon.com/EL-KIT-008-Project-Complete-Ultimate-TUTORIAL/dp/B01EWNUUUA/


Further Fun
============

Also included is the textme/ go service used to implement and handle https://www.twilio.com/docs/sms a twilio messaging service webhook so that text messages can be received and forwarded to the arduino unit via COM/Serial 

You can see a demo of this here: https://www.youtube.com/watch?v=RwSEoitm6oQ
