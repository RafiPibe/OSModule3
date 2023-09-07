#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_EXTENSION_LENGTH 10
#define MAX_EXTENSIONS 256

int numExtensions = 0;
char extensions[MAX_EXTENSIONS][MAX_EXTENSION_LENGTH];

void create_directory(const char *path) {
    mkdir(path, 0755);
}


void process_file(const char *filename, const char *extension, const char *subdirectory) {
    // Convert extension to lowercase
    char lowercase_extension[MAX_EXTENSION_LENGTH];
    for (int i = 0; i < MAX_EXTENSION_LENGTH - 1 && extension[i] != '\0'; i++) {
        lowercase_extension[i] = tolower(extension[i]);
    }
    lowercase_extension[MAX_EXTENSION_LENGTH - 1] = '\0';

    // Check if the extension is in the list
    int found = 0;
    for (int i = 0; i < numExtensions; i++) {
        if (strcmp(lowercase_extension, extensions[i]) == 0) {
            found = 1;
            break;
        }
    }

    // Move the file to the appropriate directory
    char sourcePath[512];
    if (subdirectory != NULL) {
        snprintf(sourcePath, sizeof(sourcePath), "files/%s/%s", subdirectory, filename);
    } else {
        snprintf(sourcePath, sizeof(sourcePath), "files/%s", filename);
    }

    char destPath[512];
    if (found) {
        snprintf(destPath, sizeof(destPath), "categorized/%s/%s", lowercase_extension, filename);
    } else {
        snprintf(destPath, sizeof(destPath), "categorized/other/%s", filename);
    }

    if (rename(sourcePath, destPath) != 0) {
        printf("Failed to move file: %s\n", filename);
    }
}

void process_directory(const char *dirPath) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(dirPath);
    if (dir == NULL) {
        printf("Failed to open directory: %s\n", dirPath);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char *filename = entry->d_name;

            char *dot = strrchr(filename, '.');
            if (dot != NULL) {
                char extension[MAX_EXTENSION_LENGTH];
                strcpy(extension, dot + 1);
                process_file(filename, extension, strrchr(dirPath, '/') + 1);
            }
        } else if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char subdirPath[512];
                snprintf(subdirPath, sizeof(subdirPath), "%s/%s", dirPath, entry->d_name);
                process_directory(subdirPath);
            }
        }
    }

    closedir(dir);
}

int main() {
    // Read extensions from file
    FILE *file = fopen("extensions.txt", "r");
    if (file == NULL) {
        printf("Failed to open extensions.txt file.\n");
        return 1;
    }

    // Read extensions and validate the maximum number of extensions
    char extension[MAX_EXTENSION_LENGTH];
    while (fscanf(file, "%s", extension) == 1) {
        if (numExtensions >= MAX_EXTENSIONS) {
            printf("Maximum number of extensions reached.\n");
            break;
        }
        strcpy(extensions[numExtensions], extension);
        numExtensions++;
    }

    fclose(file);

    // Create 'categorized' directory if it doesn't exist
    create_directory("categorized");

    // Create directories for each extension
    for (int i = 0; i < numExtensions; i++) {
        // Convert extension to lowercase
        for (int j = 0; j < MAX_EXTENSION_LENGTH - 1 && extensions[i][j] != '\0'; j++) {
            extensions[i][j] = tolower(extensions[i][j]);
        }

        char path[MAX_EXTENSION_LENGTH + 12];
        snprintf(path, sizeof(path), "categorized/%s", extensions[i]);
        create_directory(path);
    }

    // Create 'other' directory if it doesn't exist
    create_directory("categorized/other");

    // Process files in the 'files' directory and its subdirectories
    process_directory("files");

    return 0;
}
