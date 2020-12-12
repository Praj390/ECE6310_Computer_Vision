# Range Image Segmentation

In this project each student must segment a range image based upon surface normal. A range image of a chair is given at the course website (note that the reflectance image is only for visualization and will not be used for the lab; make sure you work with the range image). Some C-code is also provided to convert the pixels into 3D coordinates. The segmentation process will use the image grid for grouping pixels but will use the 3D coordinates for calculating surface normal for region predicates.
The image should first be masked by thresholding at a distance that removes the background and leaves only the floor and the chair. Specify the threshold chosen in your report. Use the provided C-code or an equivalent translation from the dissertation chapter to calculate 3D coordinates for the pixels. The slant type can be assumed to be scan direction downward.
Surface normal should be calculated using the cross-product method as discussed in class. Specify the distance chosen between pixels for cross products in your report.
Region growing should be used to segment regions, using the queue-based C code previously provided. The region predicate should be that a pixel can join the region if its orientation is within a threshold of the average orientation of pixels already in the region. The angular difference should be calculated using the dot product. The region growing code must be modified to recalculate the average after every new pixel joins the region. Specify the angle threshold chosen in your report. You may take advantage of the fact that for this image, the surfaces all have strong orientation differences. Seed pixels for region growing should be found by identifying a complete 5x5 window of unlabeled (and not masked out in the first step) of still-unlabeled region. If any pixel within the 5x5 window is masked out or already labeled in a region, then the pixel cannot seed a new region. Region growing ends when there are no more possible seed pixels. 


# Results
## Original Image
![]()
## Threshold Image
![]()
## Segmented Image
![]()