#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#if !defined(SOBEL_OPENMP) && !defined(SOBEL_SEQUENTIAL)
#define SOBEL_PTHREAD
#endif

#if defined(SOBEL_PTHREAD)
#include <pthread.h>
#elif defined(SOBEL_OPENMP)
#include <omp.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int Gx[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

int Gy[3][3] = {
    {-1, -2, -1},
    { 0,  0,  0},
    { 1,  2,  1}
};

#if !defined(SOBEL_OPENMP)
void convolve_sobel_rows(unsigned char *input, unsigned char *output,
                         int width, int first_row, int end_row)
{
    int k_offset = 1;
    int channels = 3;

    for (int i = first_row; i < end_row; i++) {
        for (int j = k_offset; j < width - k_offset; j++) {
            for (int c = 0; c < channels; c++) {
                float sum_x = 0.0f;
                float sum_y = 0.0f;

                for (int ki = 0; ki < 3; ki++) {
                    for (int kj = 0; kj < 3; kj++) {
                        int pixel_row = i + (ki - k_offset);
                        int pixel_col = j + (kj - k_offset);
                        int pixel_idx = (pixel_row * width + pixel_col) *
                                        channels + c;
                        unsigned char pixel_val = input[pixel_idx];

                        sum_x += (float)pixel_val * Gx[ki][kj];
                        sum_y += (float)pixel_val * Gy[ki][kj];
                    }
                }

                float magnitude = sqrtf((sum_x * sum_x) + (sum_y * sum_y));
                magnitude *= 2.0f;

                if (magnitude > 255.0f) magnitude = 255.0f;
                if (magnitude < 0.0f) magnitude = 0.0f;

                int out_idx = (i * width + j) * channels + c;
                output[out_idx] = (unsigned char)magnitude;
            }
        }
    }
}
#endif

#if defined(SOBEL_OPENMP)
void convolve_sobel_color(unsigned char *input, unsigned char *output,
                          int width, int height)
{
    int k_offset = 1;
    int channels = 3;

#pragma omp parallel for collapse(2) schedule(static)
    for (int i = k_offset; i < height - k_offset; i++) {
        for (int j = k_offset; j < width - k_offset; j++) {
            for (int c = 0; c < channels; c++) {
                float sum_x = 0.0f;
                float sum_y = 0.0f;

                for (int ki = 0; ki < 3; ki++) {
                    for (int kj = 0; kj < 3; kj++) {
                        int pixel_row = i + (ki - k_offset);
                        int pixel_col = j + (kj - k_offset);
                        int pixel_idx = (pixel_row * width + pixel_col) *
                                        channels + c;
                        unsigned char pixel_val = input[pixel_idx];

                        sum_x += (float)pixel_val * Gx[ki][kj];
                        sum_y += (float)pixel_val * Gy[ki][kj];
                    }
                }

                float magnitude = sqrtf((sum_x * sum_x) + (sum_y * sum_y));
                magnitude *= 2.0f;

                if (magnitude > 255.0f) magnitude = 255.0f;
                if (magnitude < 0.0f) magnitude = 0.0f;

                int out_idx = (i * width + j) * channels + c;
                output[out_idx] = (unsigned char)magnitude;
            }
        }
    }
}
#elif defined(SOBEL_PTHREAD)
typedef struct {
    unsigned char *input;
    unsigned char *output;
    int width;
    int first_row;
    int end_row;
} thread_data_t;

void *convolve_thread(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;
    convolve_sobel_rows(data->input, data->output, data->width,
                        data->first_row, data->end_row);
    return NULL;
}
#else
void convolve_sobel_color(unsigned char *input, unsigned char *output,
                          int width, int height)
{
    convolve_sobel_rows(input, output, width, 1, height - 1);
}
#endif

int main(int argc, char *argv[])
{
#if defined(SOBEL_SEQUENTIAL)
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_image_path>\n", argv[0]);
        return EXIT_FAILURE;
    }
#else
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_image_path> <num_of_threads>\n",
                argv[0]);
        return EXIT_FAILURE;
    }
    int num_threads = atoi(argv[2]);
    if (num_threads < 1) {
        fprintf(stderr, "Error: number of threads must be positive.\n");
        return EXIT_FAILURE;
    }
#endif

    char *input_path = argv[1];
    int width, height, actual_channels;

    printf("Loading color image: %s...\n", input_path);
    unsigned char *input_image =
        stbi_load(input_path, &width, &height, &actual_channels, 3);

    if (!input_image) {
        fprintf(stderr, "Error: Could not load image file.\n");
        return EXIT_FAILURE;
    }

    if (width < 3 || height < 3) {
        fprintf(stderr, "Error: Image must be at least 3 by 3 pixels.\n");
        stbi_image_free(input_image);
        return EXIT_FAILURE;
    }

    unsigned char *output_image =
        (unsigned char *)calloc(width * height * 3, sizeof(unsigned char));
    if (!output_image) {
        fprintf(stderr, "Error: Could not allocate output image.\n");
        stbi_image_free(input_image);
        return EXIT_FAILURE;
    }

    printf("Running Sobel operator...\n");
#if defined(SOBEL_OPENMP)
    omp_set_num_threads(num_threads);
    double start = omp_get_wtime();
    convolve_sobel_color(input_image, output_image, width, height);
    double elapsed_time = omp_get_wtime() - start;
#elif defined(SOBEL_PTHREAD)
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    thread_data_t *thread_data = malloc(num_threads * sizeof(thread_data_t));
    if (!threads || !thread_data) {
        fprintf(stderr, "Error: Could not allocate thread data.\n");
        free(threads);
        free(thread_data);
        free(output_image);
        stbi_image_free(input_image);
        return EXIT_FAILURE;
    }

    int rows = height - 2;
    int rows_per_thread = rows / num_threads;
    int remainder = rows % num_threads;
    int next_row = 1;
    int created = 0;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < num_threads; i++) {
        int assigned_rows = rows_per_thread + (i < remainder ? 1 : 0);
        thread_data[i].input = input_image;
        thread_data[i].output = output_image;
        thread_data[i].width = width;
        thread_data[i].first_row = next_row;
        thread_data[i].end_row = next_row + assigned_rows;
        next_row += assigned_rows;

        if (pthread_create(&threads[i], NULL, convolve_thread,
                           &thread_data[i]) != 0) {
            fprintf(stderr, "Error: Could not create thread.\n");
            for (int j = 0; j < created; j++)
                pthread_join(threads[j], NULL);
            free(threads);
            free(thread_data);
            free(output_image);
            stbi_image_free(input_image);
            return EXIT_FAILURE;
        }
        created++;
    }

    for (int i = 0; i < created; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error: Could not join thread.\n");
            free(threads);
            free(thread_data);
            free(output_image);
            stbi_image_free(input_image);
            return EXIT_FAILURE;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_time = (end.tv_sec - start.tv_sec) +
                          (end.tv_nsec - start.tv_nsec) / 1e9;
    free(threads);
    free(thread_data);
#else
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    convolve_sobel_color(input_image, output_image, width, height);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_time = (end.tv_sec - start.tv_sec) +
                          (end.tv_nsec - start.tv_nsec) / 1e9;
#endif

#if defined(SOBEL_OPENMP)
    char output_path[64];
    snprintf(output_path, sizeof(output_path),
             "output_sobel_openmp_t%d.bmp", num_threads);
    printf("Time taken with OpenMP: %.6f seconds\n", elapsed_time);
#elif defined(SOBEL_PTHREAD)
    char output_path[64];
    snprintf(output_path, sizeof(output_path),
             "output_sobel_pthread_t%d.bmp", num_threads);
    printf("Time taken with pthreads: %.6f seconds\n", elapsed_time);
#else
    char *output_path = "output_sobel_sequential.bmp";
    printf("Time taken sequentially: %.6f seconds\n", elapsed_time);
#endif

    if (!stbi_write_bmp(output_path, width, height, 3, output_image)) {
        fprintf(stderr, "Error: Could not save output image.\n");
        free(output_image);
        stbi_image_free(input_image);
        return EXIT_FAILURE;
    }
    printf("Saved edge visual to '%s'\n", output_path);

    stbi_image_free(input_image);
    free(output_image);
    return 0;
}
