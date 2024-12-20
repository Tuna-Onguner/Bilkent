% @author: Deniz T. Onguner
% MATLAB version: R2024b
% This is the script for the self-implemented matrix convolution function.
% Please note that the performance of the function is disregarded.
% The four-level nested loop is likely to cause slow performance.
function convolution = my_conv2(image, kernel)
    % Y := convolution
    % X := image
    % H := kernel

    % Getting the dimensions of the parameters
    [n_image_rows, n_image_cols] = size(image);
    [n_kernel_rows, n_kernel_cols] = size(kernel);
    
    % Deciding the output matrix size
    n_conv_rows = n_image_rows - 1 + n_kernel_rows;
    n_conv_cols = n_image_cols - 1 + n_kernel_cols;
    
    % Padding the input image with zeros
    padded_image = zeros(n_conv_rows, n_conv_cols);
    padded_image((1 : n_image_rows), (1 : n_image_cols)) = image;
    
    % Initializing the output matrix, initially filled with zeros
    convolution = zeros(n_conv_rows, n_conv_cols);
    
    % Performing convolution using nested loops
    % Y[i, j] = ∑m∑n(X[m, n]H[i - m, j - n])
    for i = (1 : n_conv_rows)
        for j = (1 : n_conv_cols)
            for m = (1 : n_kernel_rows)
                for n = (1 : n_kernel_cols)
                    if (i - m + 1 > 0) && (j - n + 1 > 0) && ...
                       (i - m + 1 <= n_image_rows) && ...
                       (j - n + 1 <= n_image_cols)
                        convolution(i, j) = convolution(i, j) + ...
                        padded_image(i - m + 1, j - n + 1) * kernel(m, n);
                    end % end if
                end % end for
            end % end for
        end % end for
    end % end for
end % end of function
% end of script
