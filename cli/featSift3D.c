/* -----------------------------------------------------------------------------
 * sift3d_feature_extractor.c
 * -----------------------------------------------------------------------------
 * Utility for extracting SIFT3D features from given keypoints coordinates
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "immacros.h"
#include "imutil.h"
#include "sift.h"

/* Read keypoint coordinates from a CSV file into a Keypoint_store structure */
static int read_keypoint_coords_from_csv(const char *path, Keypoint_store *const kp) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int num_keypoints = 0;
    char *token;
    int i = 0;

    // Open the file
    fp = fopen(path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        return SIFT3D_FAILURE;
    }

    // First count the number of lines (keypoints)
    while ((read = getline(&line, &len, fp)) != -1) {
        num_keypoints++;
    }

    // Resize the keypoint store
    if (resize_Keypoint_store(kp, num_keypoints)) {
        free(line);
        fclose(fp);
        return SIFT3D_FAILURE;
    }

    // Reset file pointer to beginning
    rewind(fp);
    i = 0;

    // Read each line
    while ((read = getline(&line, &len, fp)) != -1) {
        Keypoint *const key = kp->buf + i;
        int j = 0;

        // Initialize the keypoint
        if (init_Keypoint(key)) {
            free(line);
            fclose(fp);
            return SIFT3D_FAILURE;
        }

        // Parse the CSV line for x,y,z coordinates
        token = strtok(line, ",");
        while (token != NULL && j < 3) {  // Only read x,y,z coordinates
            double value = atof(token);
            
            switch (j) {
                case 0: key->xd = value; break;
                case 1: key->yd = value; break;
                case 2: key->zd = value; break;
            }
            token = strtok(NULL, ",");
            j++;
        }

        // Set default values for scale and octave
        key->o = 0;  // Base octave
        key->sd = 1.0;  // Default scale

        // Initialize orientation matrix to identity
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                SIFT3D_MAT_RM_GET(&key->R, r, c, float) = (r == c) ? 1.0f : 0.0f;
            }
        }

        i++;
    }

    free(line);
    fclose(fp);
    return SIFT3D_SUCCESS;
}

int extract_features_from_keypoints(const char* image_path, const char* keypoints_path, 
                                  const char* output_descriptors_path) {
    
    Image im;
    SIFT3D sift3d;
    Keypoint_store kp;
    SIFT3D_Descriptor_store desc;
    int ret = 0;

    // Initialize all the structures
    init_Keypoint_store(&kp);
    init_SIFT3D_Descriptor_store(&desc);
    init_im(&im);

    if (init_SIFT3D(&sift3d)) {
        fprintf(stderr, "Failed to initialize SIFT3D\n");
        return 1;
    }

    // Read the image
    if (im_read(image_path, &im)) {
        fprintf(stderr, "Failed to read image: %s\n", image_path);
        ret = 1;
        goto cleanup;
    }

    // Read keypoint coordinates from file
    if (read_keypoint_coords_from_csv(keypoints_path, &kp)) {
        fprintf(stderr, "Failed to read keypoints from: %s\n", keypoints_path);
        ret = 1;
        goto cleanup;
    }

    // Extract descriptors for the given keypoints
    if (SIFT3D_extract_raw_descriptors(&sift3d, &im, &kp, &desc)) {
        fprintf(stderr, "Failed to extract descriptors\n");
        ret = 1;
        goto cleanup;
    }

    // Write the descriptors to output file
    if (write_SIFT3D_Descriptor_store(output_descriptors_path, &desc)) {
        fprintf(stderr, "Failed to write descriptors to: %s\n", output_descriptors_path);
        ret = 1;
        goto cleanup;
    }

    printf("Successfully extracted and wrote descriptors to: %s\n", output_descriptors_path);

cleanup:
    // Clean up all resources
    im_free(&im);
    cleanup_SIFT3D(&sift3d);
    cleanup_Keypoint_store(&kp);
    cleanup_SIFT3D_Descriptor_store(&desc);

    return ret;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <image_path> <keypoints_path> <output_descriptors_path>\n", argv[0]);
        fprintf(stderr, "Note: keypoints_path should be a CSV file with x,y,z coordinates\n");
        return 1;
    }

    return extract_features_from_keypoints(argv[1], argv[2], argv[3]);
} 