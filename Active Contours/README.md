# Active Contours
## Implementation of active contour.
1. The intial image and contourpoints are given (hawk.ppm and hawk_init.txt)
2. The program uses 3 energies i.e 2 internal energy and 1 external energy.
3. External energy is calculated by applyging Sobel filter gradient and taking inverse of the pixel value at each pixel location.
4. First internal energy is calculated by follwing formula
![]()
5. Second internal energy is the sqaure of the deviation from the average distance between the points. It can be found out by first calculating the average distance between all contour points, and then taking the square of the difference between that average and the distance between the current contour point and the next contour point.

# Results
![Original image]()
![Image with initial contour points]()
![Final image after appyling active contour algorithm]()
