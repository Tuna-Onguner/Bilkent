% @author: Deniz Tuna ONGUNER
% MATLAB version: R2024b
% This code is written for the first MATLAB assignment of EEE 391 course.
% Part 1: Generating the F#m chord with amplitude=1 and phase=0
FREQ_SAMPLING = 8000; % as given in the assignment document
t = 0:1/FREQ_SAMPLING:1; % 1 second duration, also as given

% Frequencies for F#, A, and C# in the 4th octave
FREQ_F_SHARP = 370;  % F#
FREQ_A = 440;  % A
FREQ_C_SHARP = 277;  % C#

% Declaring initial sinusoidal signals
AMP = 1; % as recommended in the assignment document
PHI_deg = 0;

% Generating individual signals with initial amplitude and phase
note1 = AMP * sin(2 * pi * FREQ_F_SHARP * t + deg2rad(PHI_deg)); % F#
note2 = AMP * sin(2 * pi * FREQ_A * t + deg2rad(PHI_deg));       % A
note3 = AMP * sin(2 * pi * FREQ_C_SHARP * t + deg2rad(PHI_deg)); % C#

% Playing individual notes
sound(note1, FREQ_SAMPLING); pause(1.5);
sound(note2, FREQ_SAMPLING); pause(1.5);
sound(note3, FREQ_SAMPLING); pause(1.5);

% Combining the notes to form the chord F#m, and playing it
chord = note1 + note2 + note3;
sound(chord, FREQ_SAMPLING);

% Plotting individual notes and the chord
figure;

subplot(4,1,1); plot(t, note1); % Plotting the F# note
title('F# Note'); xlim([0 0.02]);

subplot(4,1,2); plot(t, note2); % Plotting the A note
title('A Note'); xlim([0 0.02]);

subplot(4,1,3); plot(t, note3); % Plotting the C# note
title('C# Note'); xlim([0 0.02]);

subplot(4,1,4); plot(t, chord); % Plotting the F#m chord
title('F#m Chord'); xlim([0 0.02]);