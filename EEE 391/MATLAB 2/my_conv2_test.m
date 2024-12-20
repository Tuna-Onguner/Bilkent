% @author: Deniz T. Onguner
% MATLAB version: R2024b
% This is the script for testing the self-implemented convolution function.
clc;
clear;

addpath('EEE 391/Assignment 2/');

% Test 1: Basic 3x3 image with a 2x2 kernel
input_image = [1, 2, 3;
               4, 5, 6;
               7, 8, 9];
conv_kernel = [1, 0; 0, -1];

output_my_conv2 = my_conv2(input_image, conv_kernel);
output_builtin = conv2(input_image, conv_kernel, 'full');

assert(isequal(output_my_conv2, output_builtin), 'Test 1 Failed!');
disp('Test 1 Passed');

% Test 2: Larger kernel
conv_kernel = [1, 2, 1; 0, 0, 0; -1, -2, -1];

output_my_conv2 = my_conv2(input_image, conv_kernel);
output_builtin = conv2(input_image, conv_kernel, 'full');

assert(isequal(output_my_conv2, output_builtin), 'Test 2 Failed!');
disp('Test 2 Passed');

% Test 3: Larger input image with a small kernel
input_image = randi(10, 5, 5); % Random 5x5 image
conv_kernel = [1, -1; -1, 1];

output_my_conv2 = my_conv2(input_image, conv_kernel);
output_builtin = conv2(input_image, conv_kernel, 'full');

assert(isequal(output_my_conv2, output_builtin), 'Test 3 Failed!');
disp('Test 3 Passed');

% Test 4: Edge case - kernel larger than image
input_image = [1, 2; 3, 4];
conv_kernel = [1, 0, -1; 2, 0, -2; 1, 0, -1];

output_my_conv2 = my_conv2(input_image, conv_kernel);
output_builtin = conv2(input_image, conv_kernel, 'full');

assert(isequal(output_my_conv2, output_builtin), 'Test 4 Failed!');
disp('Test 4 Passed');

% All tests passed
disp('All tests passed successfully!');

% End of script
