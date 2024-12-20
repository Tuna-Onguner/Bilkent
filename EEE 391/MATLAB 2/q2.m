% @author: Deniz T. Onguner
% MATLAB version: R2024b
% This is the script for the Question 2
clc;
clear;

addpath('EEE 391/Assignment 2');

filter10 = ones(10) / 10^2;

image_1 = imread('image1.png');
image_2 = imread('image2.png');

image_1 = im2double(image_1);
image_2 = im2double(image_2);

image_1 = image_1 * 10;
image_2 = image_2 * 5;

images_added_v1 = image_1 + image_2;
images_filtered = my_conv2(images_added_v1, filter10);

image_1 = my_conv2(image_1, filter10);
image_2 = my_conv2(image_2, filter10);

images_added_v2 = image_1 + image_2;

figure;

subplot(1,2,1);
imshow(images_filtered);
title('Images first summed then filtered');

subplot(1,2,2);
imshow(images_added_v2);
title('Images first filtered then summed');

% End of script