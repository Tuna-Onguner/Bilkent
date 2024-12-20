% @author: Deniz T. Onguner
% MATLAB version: R2024b
% This is the script for the Question 3
clc;
clear;

addpath('EEE 391/Assignment 2/');

text = im2double(imread('Text.png'));
letter_t = im2double(imread('lettert.png'));

letter_t = rot90(letter_t, 2); % Rotate the letter t by 180 degrees

result_conv = my_conv2(text, letter_t);
result_conv = rescale(result_conv);

treshold = result_conv >= 0.95;
n_ts = sum(treshold(:));

disp(['The number of horizontal "t" letters in the text is: ', ...
    num2str(n_ts)]);

% End of script