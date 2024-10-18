#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the Zig-generated header file
#include "state.h"

int read_file_content(const char* path, unsigned char** content, int* length) {
    // Open the file
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }

    // Find the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the file content
    *content = (unsigned char*)malloc(file_size);
    if (*content == NULL) {
        perror("Failed to allocate memory for file content");
        fclose(file);
        return -1;
    }

    // Read the file into the buffer
    fread(*content, 1, file_size, file);
    fclose(file);

    // Set the length of the content
    *length = (int)file_size;

    return 0;
}

int add_command(const char* file1_path, const char* file2_path) {
    unsigned char* file1_content = NULL;
    int file1_len = 0;
    if (read_file_content(file1_path, &file1_content, &file1_len) != 0) {
        return EXIT_FAILURE;
    }
    unsigned char* file2_content = NULL;
    int file2_len = 0;
    if (read_file_content(file2_path, &file2_content, &file2_len) != 0) {
        free(file1_content);
        return EXIT_FAILURE;
    }

    // Call the MergeStates function
    int out_len = 0;
    unsigned char* merged_result = MergeStates(file1_content, file1_len, file2_content, file2_len, &out_len);

    if (merged_result == NULL) {
        fprintf(stderr, "Error: MergeStates failed\n");
        free(file1_content);
        free(file2_content);
        return EXIT_FAILURE;
    }

    // Print the merged result
    fwrite(merged_result, 1, out_len, stdout);
    printf("\n");

    // Free the allocated memory for the merged result and the string content
    FreeMergeStatesResult(merged_result, out_len);
    DeinitAllocators();
    free(file1_content);
    free(file2_content);

    return EXIT_SUCCESS;
}

int remove_command(const char* file1_path, const char* input_string_path) {
    unsigned char* file1_content = NULL;
    int file1_len = 0;
    if (read_file_content(file1_path, &file1_content, &file1_len) != 0) {
        return EXIT_FAILURE;
    }

    // Call the RemoveFromState function
    int out_len = 0;
    unsigned char* removed_result = RemoveFromState(file1_content, file1_len, (unsigned char*)input_string_path, &out_len);
    if (removed_result == NULL) {
        fprintf(stderr, "Error: RemoveFromState failed\n");
        free(file1_content);
        return EXIT_FAILURE;
    }

    // Print the removed result
    fwrite(removed_result, 1, out_len, stdout);
    printf("\n");

    // Free the allocated memory for the removed result
    FreeMergeStatesResult(removed_result, out_len);
    DeinitAllocators();
    free(file1_content);

    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <command> <file1> <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Determine the command
    const char* command = argv[1];

    if (strcmp(command, "add") == 0) {
        return add_command(argv[2], argv[3]);
    } else if (strcmp(command, "remove") == 0) {
        return remove_command(argv[2], argv[3]);
    } else {
        fprintf(stderr, "Invalid command: %s\n", command);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
