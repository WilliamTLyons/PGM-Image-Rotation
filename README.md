# PGM-Image-Rotation - Parallel PGM Image Rotation

## Overview

`PGM-Image-Rotation` is a C-based tool designed to rotate grayscale Portable Graymap (PGM) images, specifically targeting 690x920 playing card images and producing a 920x920 output. Leveraging OpenMP for parallel processing, the project optimizes performance on multi-core systems through efficient in-place matrix rotation algorithms. It processes PGM images by applying user-specified clockwise and counterclockwise 90-degree rotations via command-line arguments, offering a versatile utility for image transformation tasks.

The tool efficiently handles image rotations with a focus on performance and usability, structured for clarity and practical application.

## Project Structure

- **matrix_rotation.c**: Source code implementing the rotation logic and PGM I/O.
- **Makefile**: Build script for compiling the project with OpenMP support.
- **2C-IN.pgm**: Sample 690x920 PGM input file for testing.

## Project Details

### Purpose and Scope

This tool is tailored for 690x920 PGM images (P5 binary format), commonly used to represent playing cards with a 3:4 aspect ratio. It transforms these into a 920x920 square matrix, applies the specified rotations, and generates an output file. The 920x920 output preserves the cumulative effect of both clockwise and counterclockwise rotations. The project extends beyond basic rotation by incorporating parallel processing, demonstrating an efficient approach to computationally intensive tasks.

### Key Features

- **Input Processing**: Reads a 690x920 PGM image into a zero-padded 920x920 matrix.
- **Rotation Flexibility**: Supports multiple 90-degree clockwise and counterclockwise rotations, configurable via command-line inputs.
- **Parallel Execution**: Utilizes OpenMP to parallelize rotation operations across multiple threads.
- **Performance Monitoring**: Logs execution time for each rotation, providing insight into parallel performance.
- **Output**: Generates a 920x920 PGM image reflecting the applied rotations.


### Core Functions

The project is built around key functions that form the image rotation pipeline:

- **`zero_matrix(unsigned char mat[][SQDIM])`**: Initializes the 920x920 pixel array to zero, padding unused regions outside the 690x920 input for consistent rotation handling.
- **`rotate(unsigned char mat[][SQDIM])`**: Coordinates the rotation process by invoking `rotate_right` and `rotate_left` based on specified counts, acting as the central transformation manager.
- **`rotate_right(unsigned char mat[][SQDIM])`**: Executes a 90-degree clockwise rotation using an in-place algorithm, parallelized with OpenMP.
- **`rotate_left(unsigned char mat[][SQDIM])`**: Performs a 90-degree counterclockwise rotation, also parallelized with OpenMP.
- **`read_pgm_header(int fd, char *header)`**: Reads the 38-byte PGM header to validate the file format before processing.
- **`read_pgm_data(int fd, unsigned char mat[][SQDIM])`**: Loads the 690x920 pixel data into the matrix, handling binary P5 format byte-by-byte.
- **`write_pgm(int fd, char *header, unsigned char mat[][SQDIM])`**: Outputs the rotated 920x920 matrix as a PGM file, updating the header to reflect square dimensions.

These functions create a robust pipeline for reading, transforming, and writing PGM images with parallel optimization.

### Technical Details

- **Input**: Requires a 690x920 PGM file (P5 format), zero-padded internally to 920x920.
- **Output**: Produces a 920x920 PGM file, capturing the full effect of rotations.
- **Dependencies**: Relies on a GCC compiler with OpenMP (`-fopenmp` flag).

### OpenMP Integration

OpenMP enhances the efficiency of the rotation algorithms by distributing the workload across multiple CPU cores, particularly effective for the 920x920 matrix rotations. In the `rotate_right` and `rotate_left` functions, the `#pragma omp parallel for num_threads(thread_count)` directive parallelizes the outer loop of the in-place rotation algorithm. This enables concurrent processing of matrix layers, reducing execution time on multi-core hardware. The number of threads is user-defined, allowing experimentation with parallel scaling, with timing data reported for each rotation.

### Usage Example

```bash
./matrix_rotate 2C-IN.pgm output.pgm 4 1 0
```
## Arguments

- **2C-IN.pgm**: Sample Input PGM file.
- **output.pgm**: Output PGM file.
- **4**: Number of threads.
- **1**: Clockwise rotations.
- **0**: Counterclockwise rotations.

**Output:** A 920x920 `output.pgm` file, rotated clockwise once, with timing logged (e.g., "Clockwise rotation took 0.012345 seconds with 4 threads").

## Images

The left image displays the sample input file, the middle image shows the file after a clockwise (rotate right) transformation, and the right image presents the result of a counterclockwise (rotate left) rotation.

<img src="https://github.com/user-attachments/assets/2d2403fb-5559-43e5-9352-837a429e1ae1" alt="Screenshot" width="175" />
<img src="https://github.com/user-attachments/assets/7cb59049-d331-44d9-8376-f2375273a3e3" alt="Screenshot" width="225" />
<img src="https://github.com/user-attachments/assets/793e32af-3908-47bd-bfb5-858426762eb1" alt="Screenshot" width="225" />



## Acknowledgements

- **William Lyons**: Creator and primary developer of this project.
- **Starter Code**: Dr. Sam Siewert
- **Rotation Algorithms**: Adapted from a GeeksforGeeks example and optimized for this project.

## Additional Resources

- [GeeksforGeeks](https://www.geeksforgeeks.org/inplace-rotate-square-matrix-by-90-degrees/): Original inspiration for the `rotate_right` and `rotate_left` algorithms, adapted for this project.
- [OpenMP](https://www.openmp.org/): Documentation and specifications for the OpenMP API used for parallelization.
- [PGM Format Specification](http://netpbm.sourceforge.net/doc/pgm.html): Details on the Portable Graymap (PGM) file format, including the P5 binary structure.
- [GNU GCC Documentation](https://gcc.gnu.org/onlinedocs/): Reference for the GCC compiler, including OpenMP support, used in the build process.

## Notes

- **macOS-Specific Makefile:**  
  The Makefile is tailored for macOS ARM64 systems, utilizing `/opt/homebrew/bin/gcc-14` (Homebrew-installed GCC 14) with OpenMP support. Users on other platforms will need to adjust the `CC` path to their GCC installation supporting OpenMP.
- **Output Customization:**  
  The `write_pgm` function outputs a 920x920 PGM file to preserve rotation effects. Minor modifications to this function could trim the output to 690x920 (original orientation) or 920x690 (rotated 90 degrees), adapting to different use cases.
