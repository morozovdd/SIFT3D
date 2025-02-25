# SIFT3D Feature Extractor

This tool extends the [SIFT3D library](https://github.com/bbrister/SIFT3D) with additional functionality for extracting SIFT3D descriptors from user-provided 3D keypoint coordinates. SIFT3D is an analogue of the scale-invariant feature transform (SIFT) for three-dimensional images.

## Features

- Read keypoint coordinates from CSV files
- Extract SIFT3D descriptors at specified 3D coordinates 
- Support for various medical image formats (DICOM, NIFTI)
- Multiple command-line tools for different SIFT3D operations

## Installation

### Prerequisites

The following libraries are required:
- [zlib](http://www.zlib.net/)
- [LAPACK](http://www.netlib.org/lapack/)
- [DCMTK](http://dicom.offis.de/dcmtk.php.en) (optional, for DICOM support)
- [nifticlib](http://sourceforge.net/projects/niftilib/files/nifticlib/) (optional, for NIFTI support)

On Ubuntu/Debian systems, install dependencies with:
```bash
sudo apt-get install zlib1g-dev liblapack-dev libdcmtk-dev libnifti-dev
```

### Building from Source

1. Clone the repository:
```bash
git clone https://github.com/bbrister/SIFT3D.git
```

2. If you encounter C++ standard compatibility issues with DCMTK, add these lines to the top of CMakeLists.txt:
```cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

3. Build the project:
```bash
mkdir build
cd build
cmake ..
make
```

## Command Line Tools

### 1. featSift3D - Extract Features from Keypoints
```bash
featSift3D <image_path> <keypoints_path> <output_descriptors_path>
```
Extracts SIFT3D descriptors at user-specified keypoint locations.

### 2. kpSift3D - Detect Keypoints and Extract Descriptors
```bash
kpSift3D [options] <image_path>

Options:
  --keys <filename>    Output file for keypoints (.csv, .csv.gz)
  --desc <filename>    Output file for descriptors (.csv, .csv.gz)
  --draw <filename>    Draw keypoints in image space
```

### 3. regSift3D - Image Registration
```bash
regSift3D [options] <source_image> <reference_image>

Options:
  --matches <filename>    Output feature matches
  --transform <filename>  Output transformation parameters
  --warped <filename>    Output warped source image
  --concat <filename>    Output concatenated images
  --keys <filename>      Output keypoints visualization
  --lines <filename>     Output matching lines visualization
  --nn_thresh <value>    Nearest neighbor threshold (0-1)
  --err_thresh <value>   RANSAC error threshold
  --num_iter <value>     Number of RANSAC iterations
  --type <value>         Transform type (affine)
  --resample            Resample images to same resolution
```

### 4. denseSift3D - Dense Feature Extraction
```bash
denseSift3D <input_image> <descriptors%.nii>

Extracts dense SIFT3D features, outputting multiple descriptor channels.
Output files will be named by replacing '%' with channel number (0-11).
```

### Input/Output Formats

- Images: DICOM (.dcm), NIFTI (.nii, .nii.gz), DICOM directories
- Keypoints/Descriptors: CSV (.csv, .csv.gz)

### Keypoint CSV Format
```csv
x,y,z
10,20,30
40,50,60
70,80,90
...
```

## License

Copyright (c) 2015-2019 Blaine Rister et al., see LICENSE for details.

## References

If you use this work, please cite:
B. Rister, M. A. Horowitz and D. L. Rubin, "Volumetric Image Registration From Invariant Keypoints," in IEEE Transactions on Image Processing, vol. 26, no. 10, pp. 4900-4910, Oct. 2017.




