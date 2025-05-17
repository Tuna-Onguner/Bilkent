% @author: Deniz T. Onguner
% MATLAB version: R2024b
% This is the script for the Part 2 of the Question 1
clc;
clear;

addpath('EEE 391/Assignment 2/');

I = imread('flower.jpg'); % Reading the image
I = rgb2gray(I); % Converting the image to grayscale since it is colorful
A = mat2gray(I); % Converting it to J matrix

[rows, cols] = size(A); % Getting the image sizes for output matrix

noise = randn(rows, cols) * 0.5;  % Gaussian noise with mean 0 and std 0.5
noise = noise * 0.2; % Multiplying the new matrix by 0.2 to scale it down

image_noised = A + noise; % Adding generated matrix to input image

% Displaying the noisy image
figure;
imshow(image_noised); title('Noisy Image');

% Declaring the filters
filter03 = ones(03) / 03^2; 
filter10 = ones(10) / 10^2;
filter50 = ones(50) / 50^2;

I_filtered03 = my_conv2(image_noised, filter03);
I_filtered10 = my_conv2(image_noised, filter10);
I_filtered50 = my_conv2(image_noised, filter50);

% Displaying the results
figure;
subplot(2,2,1); imshow(image_noised); title('Noisy Image');
subplot(2,2,2); imshow(I_filtered03); title('Filtered with 3x3 Filter');
subplot(2,2,3); imshow(I_filtered10); title('Filtered with 10x10 Filter');
subplot(2,2,4); imshow(I_filtered50); title('Filtered with 50x50 Filter');

% End of script