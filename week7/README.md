# Detect Humidity and Detect temperature in Smart Farm

## 1. Overview
There are two project. First project is Detect Humitidy and Working motor. it uses a particular module to measure the humidity, and when it is over a certain level of humidity, it operate the motor. Second project is Detect Temperature and Working Water Pump. As in the fisrt project, this project uses a particular module to measure to the temperature, and when it is over a certain leave of temperature, this program operate the water pump.

## 2. Implementation
* ## Detect Humidity  
    This project uses DHT22 Module, LED Module, DC motor, and Fan Module.  
    When humidity that is measured by DHT22 Module is over 40%, RED LED is turned on and, DC motor and Fan Module is operated 1 second. and then they are turned off.
    this process is repeated endlessly.
* ## Detect temperature  
    This project uses DHT22 Module, Water pump Module 
    When temperature that is measured by DHT22 Module is over 22℃, water pump is operated 1 second. and then they are turned off.
    this process is repeated endlessly.

## 3. Demo Video
* ### https://youtu.be/ed1Af4_tw14