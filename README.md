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

Compiling
---------

To build this demo you definitely need a C compiler, GTK+ development 
libraries, and PortAudio development libraries. It has been tested with 
GCC 4.9.1, GTK+ 3.12.2, and PortAudio 18.1 on Ubuntu Linux. (Changes to 
aid compilation on other systems are welcomed!)

If you have those three things and "Make", simply run "make" and the 
pendulum executable should be made.

Controls
--------

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

Note that there's no volume control - just use your OS's volume control. 
(Volume control TBD)

Things to try
-------------

* Set "Natural frequency" to an audible frequency (several hundred Hz) 
and set "Ping strength" to several hundred. Now hit "Ping". The decay is 
accompanied by a rise in pitch, because the oscillator is anharmonic and 
the oscillation frequency is lower for larger amplitudes.

* Set "Driving amplitude" to 10,000 or so. Now play with the driving 
frequency. As expected, there is a greater response near the natural 
frequency, but the interaction is more complicated than for a harmonic 
oscillator.

* In particular, hysteresis can be observed by slowly decreasing the 
driving frequency past the natural frequency (the arrow keys help with 
this). Note that the oscillation becomes "locked" to the drive and 
increases in amplitude as the frequency decreases, until a critical 
point when it "falls out of lock". The locking can only be 
re-established by returning the drive frequency much closer to the 
natural frequency.

* Try locking the oscillation to an odd harmonic of the drive such as 
1:3 or 1:5. In combination with the ping button this can lead to very 
musically interesting sounds.

Further reading
---------------

* J. Fajans et al. "Autoresonant (nonstationary) excitation of a 
collective nonlinear mode." Phys. Plasmas 6, 4497 (1999), 
http://dx.doi.org/10.1063/1.873737
