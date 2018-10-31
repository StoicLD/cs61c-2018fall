/*
 * Project 2: Performance Optimization
 */

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#endif

#include <math.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

//#include "calc_depth_naive.h"
#include "calc_depth_optimized.h"
#include "utils.h"
/* Implements the displacement function */
float displacement_naive_2(int dx, int dy) {
    return sqrt(dx * dx + dy * dy);
}

/* Helper function to return the square euclidean distance between two values. */
float square_euclidean_distance_2(float a, float b) {
    int diff = a - b;
    return diff * diff;
}
__m128 square_e_d(__m128 x, __m128 y)
{
    __m128 de =  _mm_sub_ps(x, y);
    return _mm_mul_ps(de, de);
}

void calc_depth_optimized(float *depth, float *left, float *right,
        int image_width, int image_height, int feature_width,
        int feature_height, int maximum_displacement) {
    // Naive implementation
    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {
            if (y < feature_height || y >= image_height - feature_height
                    || x < feature_width || x >= image_width - feature_width) {
                depth[y * image_width + x] = 0;
                continue;
            }
            float min_diff = -1;
            int min_dy = 0;
            int min_dx = 0;
            for (int dy = -maximum_displacement; dy <= maximum_displacement; dy++) {
                for (int dx = -maximum_displacement; dx <= maximum_displacement; dx++) {
                    if (y + dy - feature_height < 0
                            || y + dy + feature_height >= image_height
                            || x + dx - feature_width < 0
                            || x + dx + feature_width >= image_width) {
                        continue;
                    }
                    float squared_diff = 0;

                    //-----------------可以优化feature内部的计算-----------------
                    // for (int box_y = -feature_height; box_y <= feature_height; box_y++) {
                    //     for (int box_x = -feature_width; box_x <= feature_width; box_x++) {
                    //         int left_x = x + box_x;
                    //         int left_y = y + box_y;
                    //         int right_x = x + dx + box_x;
                    //         int right_y = y + dy + box_y;
                    //         squared_diff += square_euclidean_distance(
                    //                 left[left_y * image_width + left_x],
                    //                 right[right_y * image_width + right_x]
                    //                 );
                    //     }
                    // }
                    //-------------------------------------------------------

                    float sum_by_four[4];

                    __m128 accumalte =  _mm_setzero_ps();
                    for (int box_y = -feature_height; box_y <= feature_height; box_y++) {
                    /* YOUR CODE GOES HERE */

                        int left_y = y + box_y;
                        int right_y = y + dy + box_y;
                        int box_x = -feature_width;
                        for ( ; box_x <= feature_width - 7; box_x += 8)
                        {
            			    int left_x = x + box_x;
			                int right_x = x + dx + box_x;
                            __m128 rigth_four = _mm_loadu_ps ((right + right_y*image_width + right_x));
                            __m128 left_four = _mm_loadu_ps ((left + left_y*image_width + left_x));

                            //以四个为一组计算欧式距离
                            __m128 edu_dis = square_e_d(rigth_four, left_four);

                            //累加
                            accumalte = _mm_add_ps(edu_dis, accumalte);

                            rigth_four = _mm_loadu_ps ((right + right_y*image_width + right_x + 4));
                            left_four = _mm_loadu_ps ((left + left_y*image_width + left_x + 4));

                            //以四个为一组计算欧式距离
                            edu_dis = square_e_d(rigth_four, left_four);

                            //累加
                            accumalte = _mm_add_ps(edu_dis, accumalte);

                        }
                        for ( ; box_x <= feature_width - 3; box_x += 4)
                        {
            			    int left_x = x + box_x;
			                int right_x = x + dx + box_x;
                            __m128 rigth_four = _mm_loadu_ps ((right + right_y*image_width + right_x));
                            __m128 left_four = _mm_loadu_ps ((left + left_y*image_width + left_x));

                            //以四个为一组计算欧式距离
                            __m128 edu_dis = square_e_d(rigth_four, left_four);

                            //累加
                            accumalte = _mm_add_ps(edu_dis, accumalte);
                        }

                        /*
                        _mm_storeu_ps(sum_by_four, accumalte);

                        for(int j = 0; j<4 ; j++)
                        {
                            squared_diff += sum_by_four[j];
                        }
                        */

                        //特殊情况的处理
                        for(; box_x <= feature_width ; box_x++)
                        {
                            int left_x = x + box_x;
                            int right_x = x + dx + box_x;
                            squared_diff += square_euclidean_distance_2(
                                    left[left_y * image_width + left_x],
                                    right[right_y * image_width + right_x]
                                    );
                        }
                    }
                    _mm_storeu_ps(sum_by_four, accumalte);
                    for(int j = 0; j<4 ; j++)
                    {
                        squared_diff += sum_by_four[j];
                    }
                    //-------------------------------------------------------
                    if (min_diff == -1 || min_diff > squared_diff
                            || (min_diff == squared_diff
                                && displacement_naive_2(dx, dy) < displacement_naive_2(min_dx, min_dy))) {
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
                    depth[y * image_width + x] = displacement_naive_2(min_dx, min_dy);
                }
            } else {
                depth[y * image_width + x] = 0;
            }
        }

}
}
