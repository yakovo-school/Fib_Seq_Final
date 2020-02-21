Hello there, and thank you for downloading Fib_Seq and I hope it is useful for your creativity!

This is coded as an "effect", not a "synth", and was only rigorously tested in FL Studio 20 and LIVE 10.1.7.
I'm not a CS Student so if the code sucks... Let me know so I can fix it. Mainly the "effect" vs "Synth" thing for FL users.
Also: The GUI version is only for Ableton users. It doesn't even display any information. Weird bug but whatever.
This isn't meant to be a thing that is "looked at" like an extensive synth like Serum, Massive, Sytrus, etc, but just twiddled about with knobs.

Min Note and Note Range set the allowable midi note values from... A minimum up to one below whatever the range is. 3 octaves should be enough, right?
Velocity also has this feature, but allows you to set a flat range in order to have control over that. Send to PWM/Filter cutoff.

Interval sets the numerator of the time signature from 1-16. Div determines the denominator. 0== quarter notes, 1 == 1/8 notes, 2 == 1/16 notes. 
Example: If Interval = 7 and Div = 1 then an 1/8th note will play every 7 1/8th notes.

Gate determines the percentage of the Div value played out. 50% of Div = 0 is an 1/8 note, but will play at whatever the rate is in quarter notes. 
The range is set to that in order to avoid problems with overlapping notes and hardware synths that might not have fast-enough midi response. 

A note for FL users: because this is an "effect", load it into a Patcher and do the output ports thing with your target synth. 
Do not connect the "From FL Studio" cable. In order to access the parameters, right-click the module and they should all be there. 
I know recording midi internally is a bitch so audio is your bet right now, unless you want to deal with some LoopMIDI driver. 

The purpose is the explore generative music and have like.... 8 of these running at the same time. With as many speakers. 
3-4 letter acronyms of substance abuse optional. 
Jungle breaks mandatory. 

Contact: ofoot@hotmail.com

I guess send this over to loopop? I like his generative series. 

Hail Duda

Thank you's:
Chris T., Rich T., Rob H., Eileen Y., Xenakios, buftucker.

Anti-Thank-You's:
My dad, for telling me this wouldn't work. 