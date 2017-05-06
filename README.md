# NextionPassthru
Using a Nextion Panel with an Arduino Processor

This code sample builds an interface between the Arduino Micro controller and the Nextion Panel.

This project has a passthrou program which allows you to enter commands in the Arduino console and have
it passed straight through to the Nextion panel for processing.  It also displays the return data from the panel.

There is some debug code left in for your enjoyment.

It also includes a sample program called Maze that if you type the special command "run" it will excute this program.

The program will blank the panel and draw a random maze on it.  The panel has to be at least the 4.3" display.
Smaller displays will need to modify the code so that it will fit the screen size.

One short coming that was found is there is no getPixel or setPixel command.  No way to determine if pixel on the panel
is on or off or what color it might be.  It would have been nice to be able to set individual pixels.

