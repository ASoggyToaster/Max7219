textme/ contains a go service that listens for incoming sms messages from twilio and then sends them to arduino via serial.

app.ino Application that demonstrates the use of the Max7219 library. Receives a message via serial and then displays it on LED matrix. 

Max7219/ library I wrote for 8x8 7219 LED matrix. Allows for different ways to display text (scrolling marquee, wipe (per character), flash character). 