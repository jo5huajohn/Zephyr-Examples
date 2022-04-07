# Zephyr RTOS Examples
A repository of examples for Zephyr RTOS. The dev-board used here is ESP32-PICO-KIT.  
To build the examples, add these to a zephyr project workspace and run ```west build -p auto -b esp32``` within the app.  
To flash the examples, run ```west flash```.  

## Examples
- ### Switch (GPIO)
This example makes use of an LED and a switch. Every time the switch is pressed, the LED is toggled.
- ### Switch (GPIO Interrupt)
This example makes use of a switch. The microcontroller stays in a while loop. When a rising edge is detected, "Pressed" will be printed. 
- ### Encoder (GPIO)
This example makes use of an encoder. When the encoder is turned clockwise/anticlockwise, a counter will be incremented or decremented respectively.
- ### Enocder (GPIO Interrupt)
This example makes use of an encoder. The microcontroller stays inside a while loop. When the encoder is turned clockwise/anticlockwise (i.e., an active edge is detected), a counter will incremented/decremented and its value will be printed on the console.
