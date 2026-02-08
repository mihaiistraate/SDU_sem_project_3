# Remote Controlled Car

Remotely controlled vehicles are an electronic toy commonly found in stores. This project sought to replicate this commodity with an addition of slowing down and potentially braking upon encountering an obstacle such as a wall. This feature was to be achieved with a proximity sensor which would send this information to the Arduino microcontroller, where the speed of the vehicle would be tempered from the input speed of the user in order to lessen the impact for the toy. This would be achieved in order to showcase the motor and its response to external stimuli to fulfil the requirements of the project specifications.

To achieve this product, it required simulations, programming of the microcontroller, research into the motor driver, PCB design for the vehicle and the controller, as well as 3D design of the casings of both the components.

The Gantt chart was used to showcase the project timeline and to make sure we as a team are in time to finalize the project within the given timeframe. It can be seen in the following picture:

<img width="1536" height="323" alt="Gantt chart" src="https://github.com/user-attachments/assets/4ec3dbd6-a855-4c08-bdfc-00a0337099f0" />

There was a fixed budget from the university of 1k DKK to order components necessary for the PCB and enclosure. As it can be seen in the following picture, we used a total of 970 DKK.

<img width="1556" height="283" alt="components list" src="https://github.com/user-attachments/assets/7f958040-57f5-4dfc-afa4-7b28fd875b54" />

For designing the PCB, KiCad was used and we generated a schematic and a PCB layout:

<img width="1510" height="1038" alt="Controller PCB schematic" src="https://github.com/user-attachments/assets/41d207e8-634f-4fcf-bebf-afd2c88e6d04" />
<img width="1813" height="1251" alt="Car PCB schematic" src="https://github.com/user-attachments/assets/044075c7-f805-41a2-ae41-0c214c969617" />

<img width="1354" height="934" alt="Car PCB layout" src="https://github.com/user-attachments/assets/7dae5ef4-8da2-46be-b78e-fbe198fd2de3" />

In the controller design, we decided to use two joysticks, even though the task they get done could have been solved with only one, mainly because then the controller can be used with both hands, and it would be more intuitive in this way. For each joystick, only one axis is taken into consideration, and it can therefore control only one motor in the car. For instance, one joystick (the X pin is connected to GPIO34 and the Y pin is connected to GPIO35) only considers the horizontal axis and controls the servo motor, which is responsible for left or right turning of the car and the other (the X pin is connected to GPIO32 and the Y pin is connected to GPIO33) considers only the vertical axis and can operate only the DC motor, which in turn spins the wheels either forward or backward.

The principle of operation for a joystick is that it has a variable resistance according to how it is moved on each axis. This variable resistance will translate in a different voltage that can be then measured at the microcontroller pin. Because one joystick controls only one motor, the voltage on only one axis must be measured for each. Each value is then constrained to the minimum and maximum voltage values measured from each joystick from tests (for the first joystick: [865, 2879], and for the second one: [850, 2900]), because in the receiver code, they will map the behaviour of motors, so they cannot exceed certain values. These two values are stored in a struct type and then sent over through the wireless modules to the receiver ESP32 each 15ms.

If data from the controller is received, then that will be stored in a similar struct variable and then a new measurement from the ultrasonic sensor is made. In addition, the data from the horizontal axis of the joystick is processed into the behaviour of the servo motor. Furthermore, if an obstacle is detected that is closer or equal with 15cm, then the DC motor and the RGB strip will have a different behaviour: further speed is limited to minimum, backwards speed will not be affected, and the RGB strip will turn red to show that the car is in potential danger of colliding.

The forward speed is not 0 because there could be situations when the car can move backward on a non-linear trajectory and come across an obstacle in both front and back, but only the front one is detected by the sensor and the car would be trapped.

If nothing is detected in front, the RGB strip will have a rainbow effect, and the forwards and backwards accelerations will be in accordance with the joysticks. Moreover, if no data is detected from the controller, the RGB strip will have the same behaviour as when an obstacle is detected: it will turn red.
