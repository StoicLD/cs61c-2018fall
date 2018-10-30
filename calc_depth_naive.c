/*
 * Project 2: Performance Optimization
 */

#include <math.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "calc_depth_naive.h"
#include "utils.h"

/* Implements the displacement function */
float displacement_naive(int dx, int dy) {
    return sqrt(dx * dx + dy * dy);
}

/* Helper function to return the square euclidean distance between two values. */
float square_euclidean_distance(float a, float b) {
    int diff = a - b;
    return diff * diff;
}

void calc_depth_naive(float *depth, float *left, float *right, int image_width,
        int image_height, int feature_width, int feature_height,
        int maximum_displacement) {
    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {
            if (y < feature_height || y >= image_height - feature_height
                    || x < feature_width || x >= image_width - feature_width) {
                //装不下的全部都是0
                depth[y * image_width + x] = 0;
                continue;
            }
            float min_diff = -1;
            int min_dy = 0;
            int min_dx = 0;

            /*maximum_displacement 是 search space的大小*/
            for (int dy = -maximum_displacement; dy <= maximum_displacement; dy++) {
                for (int dx = -maximum_displacement; dx <= maximum_displacement; dx++) {
                    //搜索范围内超出图片的部分全部跳过不搜索
                    if (y + dy - feature_height < 0
                            || y + dy + feature_height >= image_height
                            || x + dx - feature_width < 0
                            || x + dx + feature_width >= image_width) {
                        continue;
                    }
                    float squared_diff = 0;
                    //每个feature开始遍历
                    for (int box_y = -feature_height; box_y <= feature_height; box_y++) {
                        for (int box_x = -feature_width; box_x <= feature_width; box_x++) {
                            int left_x = x + box_x;
                            int left_y = y + box_y;
                            int right_x = x + dx + box_x;
                            int right_y = y + dy + box_y;
                            squared_diff += square_euclidean_distance(
                                    left[left_y * image_width + left_x],
                                    right[right_y * image_width + right_x]
                                    );
                        }
                    }
                    //更新取最小值
		    if(image_height == 10)
			    printf("%s %f\n", "origin diff is:",squared_diff);
                    if (min_diff == -1 || min_diff > squared_diff
                            || (min_diff == squared_diff
                                && displacement_naive(dx, dy) < displacement_naive(min_dx, min_dy))) {
                        min_diff = squared_diff;
                        min_dx = dx;
                        min_dy = dy;
                    }
                }
            }
            if (min_diff != -1) {
                if (maximum_displacement == 0) {
                    depth[y * image_width + x] = 0;
                } else {
                    depth[y * image_width + x] = displacement_naive(min_dx, min_dy);
                }
            } else {
                depth[y * image_width + x] = 0;
            }
        }
    }
}
