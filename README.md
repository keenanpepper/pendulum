pendulum
========

Interactive driven pendulum simulator with real-time audio

Required libraries: GTK+ and PortAudio

This demo uses leapfrog integration (the simplest kind of Runge-Kutta 
integration) to solve in real time the equation of motion of a damped, 
driven pendulum. (Not a harmonic oscillator, but a real non-linear 
pendulum with potential energy proportional to the cosine of the angle 
variable.) This is a well-known example of a system that exhibits 
chaotic dynamics.

Instead of visualizing the solution graphically, it plays it as audio in 
real time, thus making it somewhat like a virtual musical instrument.

* Natural frequency (Hz)

This is the frequency of *small-amplitude* undriven oscillations of the 
pendulum. For larger amplitude oscillations, the frequency decreases 
(and the waveform becomes non-sinusoidal as well) because of the 
anharmonicity of the system.

* Damping coefficient

Controls the viscous damping term that works similarly to a damped 
harmonic oscillator. Units are fairly arbitrary (more meaningful units TBD).

* Driving amplitude
* Driving frequency (Hz)

These control the sinusoidal drive term.

* Ping strength

This controls the size of the "kick" delivered whenver the "Ping" button 
is pressed.
