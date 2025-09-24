
# Smart Cat Feeding Project

This is an Arduino project of an **Automatic Pet Feeder** which can help pet owners to take better care of their loved pets by keeping an eye on feeding time and the proper diet (food amount). 

An RTC (Real Time Clock) module is used in order to be able to keep track of time and to be able to set a certain feeding time. Once that time has come, the code checks if there is any food in the container (the container being the place where all the food supply is stored). If there isn't - it notifies the user since no food can be dropped. If there is food available the Servo motor opens the container and supplies food until a certain amount, set by the user, is reached. After each feeding the food availability in the container is once again checked.

Parts used in this project:
- Arduino Uno R4 Wi-Fi
- Micro Servo 9g (opens the food container)
- Weight Sensor HX711 AD (placed under the bowl to check the food amount in it)
- RTC Module (keeps track of time)
- Ultrasonic Distance Sensor HC-SR04 (used to check if there is any food available in the container by measuring the distance to the food level)
- Power Supply 


Here you can see and play around with the project in real time: https://wokwi.com/projects/442999362420339713

## Screenshots

![Diagram](/images/project.png?raw=true "Project Diagram")


Block Diagram

![Block Diagram](/images/BlockDiagram.png?raw=true "Project Block Diagram")




## Notes

The constants in the code can be changed to the user's liking. I've used values (such as short feeding time) in order to be able to see quick changes in the output. For real life usage they would be different depending on:
- the type of pet, its weight, diet and so on
- size of the container


The constants in question:

How often the pet is fed (in milliseconds)
> const unsigned long FEED_INTERVAL = 20000;

The weight of the food intake (in grams)
> const int TARGET_WEIGHT = 65.00;

Distance between the Ultrasonic Distance Sensor and the food (in centimeters)
> const int EMPTY_CONTAINER_DISTANCE = 10; 



## Additional

For this project we were provided with **Arduino Uno R4 Wi-Fi** which has a LED matrix. I decided to play around with it and came up with an animation for the project. 
I used the Arduino LED Matrix Editor site in order to create it. (https://ledmatrix-editor.arduino.cc/) 


Matrix Animation

![Animation](/images/ArduinoMatrixAnimation.gif?raw=true "Matrix Animation")



In order to change this animation as you like, upload the file **MatrixProject_CatAnimation.mpj** on the site. Happy creating!