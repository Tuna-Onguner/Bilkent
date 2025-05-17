% @author: Deniz Tuna ONGUNER
% Part 3: Adding harmonics to the chord
FREQ_SAMPLING = 8000; % Hz
t = 0:1/FREQ_SAMPLING:1; % 1 second duration

% Frequencies for F#, A, and C# in the 4th octave
FREQ_F_SHARP = 370;  % F#
FREQ_A = 440;        % A
FREQ_C_SHARP = 277;  % C#

% Amplitude weights for harmonics
AMP = 1; % Fundamental amplitude
AMP_HARMONICS = [0.5, 0.25, 0.125]; % Amplitudes for 2f, 3f, 4f harmonics

% Function to generate a signal with fundamental and harmonic components
% Inputs:
%   signal - Fundamental frequency of the signal (in Hz).
%   AMP - Amplitude of the fundamental frequency component.
%   AMP_HARMONICS - Array containing amplitudes for the harmonics.
%   t - Time vector for signal generation.
% Output:
%   Signal with fundamental and specified harmonics.
add_harmonics = @(signal, t) AMP * sin(2 * pi * signal * t) + ...
                        AMP_HARMONICS(1) * sin(2 * pi * 2 * signal * t) + ...
                        AMP_HARMONICS(2) * sin(2 * pi * 3 * signal * t) + ...
                        AMP_HARMONICS(3) * sin(2 * pi * 4 * signal * t);

% Generate signals with harmonics
note1_with_harmonics = add_harmonics(FREQ_F_SHARP, t);
note2_with_harmonics = add_harmonics(FREQ_A, t);
note3_with_harmonics = add_harmonics(FREQ_C_SHARP, t);

% Combine harmonics to form the chord
chord_with_harmonics = note1_with_harmonics + ...
                       note2_with_harmonics + ...
                       note3_with_harmonics;

% Play the notes and the chord with harmonics
sound(note1_with_harmonics, FREQ_SAMPLING); pause(1.5);
sound(note2_with_harmonics, FREQ_SAMPLING); pause(1.5);
sound(note3_with_harmonics, FREQ_SAMPLING); pause(1.5);
sound(chord_with_harmonics, FREQ_SAMPLING);  

% Plot the time-domain signals
figure;
subplot(4, 1, 1);
plot(t(1:1000), note1_with_harmonics(1:1000));
title('F# with Harmonics');
xlim([0 0.02]);
xlabel('Time (s)');
ylabel('Amplitude');

subplot(4, 1, 2);
plot(t(1:1000), note2_with_harmonics(1:1000));
title('A with Harmonics');
xlim([0 0.02]);
xlabel('Time (s)');
ylabel('Amplitude');

subplot(4, 1, 3);
plot(t(1:1000), note3_with_harmonics(1:1000));
title('C# with Harmonics');
xlim([0 0.02]);
xlabel('Time (s)');
ylabel('Amplitude');

subplot(4, 1, 4);
plot(t(1:1000), chord_with_harmonics(1:1000));
title('F#m Chord with Harmonics');
xlim([0 0.02]);
xlabel('Time (s)');
ylabel('Amplitude');

% This code below is the continuation of the previous code
%   snippet in Listing 3 of Appendix A (see on the previous page).
% Which is why the code is not repeated here; please,
%   assume that all the necessary variables/constants are defined.

% @author: Deniz Tuna ONGUNER
% Part 4: 

% Fourier Transform of the chord with harmonics
chord_length = length(chord_with_harmonics); % Number of samples
fft_chord = fft(chord_with_harmonics); % Compute Fourier Transform
magnitudes = abs(fft_chord/chord_length); % Normalize the magnitude

% Frequency vector for plotting
freq_p = (0:chord_length/2-1) * (FREQ_SAMPLING/chord_length);
magnitudes = magnitudes(1:chord_length/2);

% Plot the spectrum
figure;

plot(freq_p, magnitudes);
title('Magnitude Spectrum of F#m Chord with Harmonics');
xlabel('Frequency (Hz)');
ylabel('Magnitude');
grid on;

% This code below is the continuation of the previous code
%   snippet in Listing 4 of Appendix A (see on the previous page).
% Which is why the code is not repeated here; please,
%   assume that all the necessary variables/constants are defined.

% @author: Deniz Tuna ONGUNER
% Part 5:

% Highest frequency, in 4th octave, is A with the value of 440 Hz
% Hence, the reduced sampling shall be lower than or equal to two times A
% Frequency Sampling Reduced <= 2 * 440 = 880
REDUCED_FREQ_SAMPLING = 800; % I have chosen 800 for simplicity
t_low = 0:1/REDUCED_FREQ_SAMPLING:1;

% Generate the chord at the reduced sampling frequency
note1_low = add_harmonics(FREQ_F_SHARP, t_low);
note2_low = add_harmonics(FREQ_A, t_low);
note3_low = add_harmonics(FREQ_C_SHARP, t_low);
chord_low = note1_low + note2_low + note3_low;

% Upsample the aliased signal to the original sampling rate
t_high = 0:1/FREQ_SAMPLING:1;
chord_upsampled = interp1(t_low, chord_low, t_high, 'linear');

% Plot the spectra
% Aliased chord spectrum
chord_length_fs_low = length(chord_low);
fft_chord_low = fft(chord_low);
magnitudes_low = abs(fft_chord_low/chord_length_fs_low);
freq_low = (0:chord_length_fs_low/2-1) * ...
    (REDUCED_FREQ_SAMPLING/chord_length_fs_low);
magnitudes_low = magnitudes_low(1:chord_length_fs_low/2);

sound(chord_low, FREQ_SAMPLING); pause(1.5);

% Upsampled chord spectrum
chord_length_fs_high = length(chord_upsampled);
fft_chord_high = fft(chord_upsampled);
magnitudes_high = abs(fft_chord_high/chord_length_fs_high);
freq_high = (0:chord_length_fs_high/2-1) * ...
    (FREQ_SAMPLING / chord_length_fs_high);
magnitudes_high = magnitudes_high(1:chord_length_fs_high/2);

% Plotting
figure;

subplot(2, 1, 1);
plot(freq_low, magnitudes_low);
title('Magnitude Spectrum of Aliased Chord');
xlabel('Frequency (Hz)');
ylabel('Magnitude');
grid on;

subplot(2, 1, 2);
plot(freq_high, magnitudes_high);
title('Magnitude Spectrum of Upsampled Chord');
xlabel('Frequency (Hz)');
ylabel('Magnitude');
grid on;

% Listen to the upsampled aliased signal
sound(chord_upsampled, FREQ_SAMPLING);