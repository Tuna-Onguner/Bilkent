% @author: Deniz Tuna ONGUNER
% Part 2: Customizing the chord with amplitude and phase values by ID digits
FREQ_SAMPLING = 8000;
t = 0:1/FREQ_SAMPLING:1;

% Frequencies for F#, A, and C# in the 4th octave
FREQ_F_SHARP = 370;  % F#
FREQ_A = 440;  % A
FREQ_C_SHARP = 277;  % C#

% Amplitudes from ID digits, my ID is 22001788
A1 = 7 ; A2 = 8 ; A3 = 8 ;

% Phase values from ID digits in degrees
% d4d5d6 = 017, d5d6d7 = 178, d6d7d8 = 788
phi_d4d5d6_deg = 017;
phi_d5d6d7_deg = 178;
phi_d6d7d8_deg = 788;

% Converting degrees to radians for the sine function
phi_d4d5d6_rad = deg2rad(phi_d4d5d6_deg);
phi_d5d6d7_rad = deg2rad(phi_d5d6d7_deg);
phi_d6d7d8_rad = deg2rad(phi_d6d7d8_deg);

% Generating signals with custom amplitudes and phases
note1_prime = A1 * sin(2 * pi * FREQ_F_SHARP * t + phi_d4d5d6_rad);
note2_prime = A2 * sin(2 * pi * FREQ_A * t + phi_d5d6d7_rad);
note3_prime = A3 * sin(2 * pi * FREQ_C_SHARP * t + phi_d6d7d8_rad);

% Combined chord with custom settings
chord_prime = note1_prime + note2_prime + note3_prime;
sound(chord_prime, FREQ_SAMPLING);

% Plotting custom individual notes and the chord
figure;

subplot(4,1,1); plot(t, note1_prime); % Plotting the custom F# note
title('Custom F# Note'); xlim([0 0.02]);

subplot(4,1,2); plot(t, note2_prime); % Plotting the custom A note
title('Custom A Note'); xlim([0 0.02]);

subplot(4,1,3); plot(t, note3_prime); % Plotting the custom C# note
title('Custom C# Note'); xlim([0 0.02]);

subplot(4,1,4); plot(t, chord_prime); % Plotting the custom F#m chord
title('Custom F#m Chord'); xlim([0 0.02]);