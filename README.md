Flysky-fs-i6x-control-car-using-4-channel-relay
Making a RC car using 4 channel relay and arduino uno as a microcontroller ,as communication I have used flysky fs-i6x 10 channel transmitter.

Explanation:
Relay Control: The controlRelays() function sets the relays' states based on the motor values derived from the receiver input.
Receiver Input Handling: The receive() function reads the pulse width from the receiver channels and maps them to a usable range.
Movement Logic: The car moves forward if both motors have positive speed values, backward if both have negative values, turns left if the left motor has a negative value and the right motor has a positive value, and turns right if the left motor has a positive value and the right motor has a negative value. If both motors are stopped (speed = 0), the car stops.
