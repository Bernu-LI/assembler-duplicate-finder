#include <stdio.h>

int* bubbleSort(int n, int arr[]) {
    __asm__ (
            "mov w0, %w[n] \n"
            "mov x1, %[arr] \n\t"

            "1: \n\t"
            "sub w0, w0, #1 \n\t"
            "cmp w0, #0 \n\t"
            "b.lt 3f \n\t"
            "ldr w3, [x1, w0, sxtw #2] \n\t"
            "mov w2, w0 \n\t"

            "2: \n\t"
            "sub w2, w2, #1 \n\t"
            "cmp w2, #0 \n\t"
            "b.lt 1b \n\t"
            "ldr w4, [x1, w2, sxtw #2] \n\t"
            "cmp w4, w3 \n\t"
            "b.le 2b \n\t"
            "str w3, [x1, w2, sxtw #2] \n\t"
            "str w4, [x1, w0, sxtw #2] \n\t"
            "mov w3, w4 \n\t"
            "b 2b \n\t"

            "3: \n\t"
            :
            : [arr] "r" (arr), [n] "r" (n)
    : "w0", "x1", "w2", "w3", "w4", "memory"
    );
}

void find_duplicates(int *input_array, int input_size, int *output_array, int *output_counts, int *output_size) {
    __asm__(
            "mov x0, %x0\n"      // input_array pointer
            "mov w1, %w1\n"     // input_size
            "mov x2, %x2\n"     // output_array pointer
            "mov x3, %x3\n"      // output_counts pointer
            "mov x4, %x4\n"      // output_size pointer
            "mov x5, #0\n"      // Initialize i to 0
            "mov x6, #0\n"      // Initialize outputIndex to 0
            "mov x11, #0\n"     // Initialize auxiliaryIndex to 0

            "1:\n"              // Start of outer loop
            "cmp w5, w1\n"      // Compare i with input_size
            "b.ge 2f\n"         // If i >= input_size, then exit the outer loop
            "add x7, x5, #1\n"  // Initialize j to i + 1
            "mov x8, #1\n"      // Initialize count to 1

            "3:\n"              // Start of inner loop
            "cmp w7, w1\n"      // Compare j with input_size
            "b.ge 4f\n"         // If j >= input_size, then exit the inner loop
            "ldr w9, [x0, x5, lsl #2]\n"  // Load input_array[i] into w9
            "ldr w10, [x0, x7, lsl #2]\n" // Load input_array[j] into w10
            "cmp w9, w10\n"     // Compare input_array[i] with input_array[j]
            "b.ne 5f\n"         // If input_array[i] != input_array[j], then jump to increment j
            "add x8, x8, #1\n"  // Increment count

            "5:\n"              // Increment j
            "add x7, x7, #1\n"  // Increment j
            "b 3b\n"            // Jump back to the start of the inner loop

            "4:\n"              // Store the duplicate number and its count
            "ldr w9, [x0, x5, lsl #2]\n"  // Load input_array[i] into w9
            "cmp x8, #1\n"      // Compare count with 1
            "b.gt 6f\n"         // If x8 > 1, then jump to 6
            "add x5, x5, x8\n"  // Increment i by count
            "b 1b\n"            // Jump back to the start of the outer loop

            "6:\n"
            "str w9, [x2, x11, lsl #2]\n"    // Store input_array[i] in output_array[auxiliaryIndex]
            "str w8, [x3, x11, lsl #2]\n"  // Store count in output_counts[auxiliaryIndex]
            "add x11, x11, #1\n"      // Increment auxiliaryIndex
            "add x5, x5, x8\n"  // Increment i by count
            "b 1b\n"            // Jump back to the start of the outer loop

            "2:\n"              // End of outer loop
            "str x11, [x4]\n"    // Store auxiliaryIndex in output_size
            :
            : "r"(input_array), "r"(input_size), "r"(output_array), "r"(output_counts), "r"(output_size)
            : "x0", "x1", "x2", "x3", "x4", "x5", "x7", "x8", "x9", "x10", "x11"
            );
}

int main() {
    FILE *file = fopen("input.txt", "r");

    int input_size;
    fscanf(file, "%d", &input_size);

    int input_array[input_size];
    for (int i = 0; i < input_size; i++) {
        fscanf(file, "%d", &input_array[i]);
    }

    fclose(file);

    int output_array[input_size];
    int output_counts[input_size];
    int output_size = 0;

    bubbleSort(input_size, input_array);

    find_duplicates(input_array, input_size, output_array, output_counts, &output_size);

    file = fopen("output.txt", "w");
    for (int i = 0; i < output_size; i++) {
        fprintf(file, "%d-%d\n", output_array[i], output_counts[i]);
    }
    printf("\n");

    return 0;
}