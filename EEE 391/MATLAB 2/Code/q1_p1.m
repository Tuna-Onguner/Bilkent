% @author: Deniz T. Onguner
% MATLAB version: R2024b
% This is the script for the Part 1 of the Question 1
clc;
clear;

addpath('EEE 391/Assignment 2/');

I = imread('flower.jpg'); % Reading the image
I = rgb2gray(I); % Converting the image to grayscale since it is colorful
A = mat2gray(I); % Converting to J matrix

% Declaring the filters
filter03 = ones(03) / 03^2; 
filter10 = ones(10) / 10^2;
filter50 = ones(50) / 50^2;

% Using self-implemented conv2 function, i.e. my_conv2,
%   to perform convolution and to apply filters
I_filtered03 = my_conv2(A, filter03);
I_filtered10 = my_conv2(A, filter10);
I_filtered50 = my_conv2(A, filter50);

% Displaying the results
figure;
subplot(2,2,1); imshow(A); title('Original Grayscale Image');
subplot(2,2,2); imshow(I_filtered03); title('Filtered with 3x3 Filter');
subplot(2,2,3); imshow(I_filtered10); title('Filtered with 10x10 Filter');
subplot(2,2,4); imshow(I_filtered50); title('Filtered with 50x50 Filter');
sgtitle('Using Self-Implemented Convolution Function');

% Doing the same but using built-in conv2 function this time
I_filtered03 = conv2(A, filter03);
I_filtered10 = conv2(A, filter10);
I_filtered50 = conv2(A, filter50);

% Again, displaying the results
figure;
subplot(2,2,1); imshow(A); title('Original Grayscale Image');
subplot(2,2,2); imshow(I_filtered03); title('Filtered with 3x3 Filter');
subplot(2,2,3); imshow(I_filtered10); title('Filtered with 10x10 Filter');
subplot(2,2,4); imshow(I_filtered50); title('Filtered with 50x50 Filter');
sgtitle('Using Built-In Convolution Function');

% End of script
