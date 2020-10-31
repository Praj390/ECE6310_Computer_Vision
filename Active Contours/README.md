# Active Contours
## Implementation of active contour.
1. The intial image and contourpoints are given (hawk.ppm and hawk_init.txt)
2. The program uses 3 energies i.e 2 internal energy and 1 external energy.
3. External energy is calculated by applyging Sobel filter gradient and taking inverse of the pixel value at each pixel location.
4. First internal energy is calculated by taking square of distances between the points.
5. Second internal energy is the sqaure of the deviation from the average distance between the points. It can be found out by first calculating the average distance between all contour points, and then taking the square of the difference between that average and the distance between the current contour point and the next contour point.

# Results
### Original Image
![Original image](https://github.com/Praj390/ECE6310_Computer_Vision/blob/master/Active%20Contours/hawk.jpg)

### Image after Sobel filter
![Image after sobel filter](https://github.com/Praj390/ECE6310_Computer_Vision/blob/master/Active%20Contours/hawk_sobel.jpg)

### Image with initial contour points
![Image with initial contour points](https://github.com/Praj390/ECE6310_Computer_Vision/blob/master/Active%20Contours/hawk_initial.jpg)

### Final image after appyling active contour algorithm
![Final image after appyling active contour algorithm](https://github.com/Praj390/ECE6310_Computer_Vision/blob/master/Active%20Contours/hawk_final.jpg)
