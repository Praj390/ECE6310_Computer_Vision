#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


#define SQR(x) ((x)*(x))
void Sobelfilter(unsigned char *image, float *sobel_image,int ROWS, int COLS)
{
  int r,c,r1,c1;
  float sum1,sum2;
  int sobel_h[3][3] = {-1,0,1,-2,0,2,-1,0,1};
  int sobel_v[3][3] = {-1,-2,-1,0,0,0,1,2,1};
  
  for (r=1; r<=ROWS-1; r++){
  for (c=1; c<=COLS-1; c++){
  sum1 = 0;
  sum2 = 0;
    for (r1=0; r1<=2; r1++){
      for (c1=0; c1<=2; c1++){
        sum1+=  image[(r+r1)*COLS+(c+c1)] * sobel_v[r1][c1];
        sum2+=  image[(r+r1)*COLS+(c+c1)] * sobel_h[r1][c1];
      }
    }
    sobel_image[r*COLS+c] = sqrt(SQR(sum1)+SQR(sum2)) ;
  }
}
return;
} 

void normalization(float *sobel_image,unsigned char *normalized_image,int ROWS, int COLS)
{
  float max_old = 0;
  float min_old = 0;
  float new_min = 0;
  float new_max =255;
  int r,c,i;

  for(i=0;i<ROWS*COLS;i++){
    if (sobel_image[i] > max_old){
      max_old = sobel_image[i];
    }
    if (sobel_image[i] < min_old){
      min_old = sobel_image[i];
    }
  }

  for (r=0; r<ROWS; r++){
  for (c=0;c<COLS;c++){
    normalized_image[r*COLS+c] = ((sobel_image[r*COLS+c]-min_old)*((new_max-new_min)/(max_old-min_old)))+new_min; 
  }
  }
}

float distance(int y1, int y2, int x1, int x2){
  float dis;
  dis = sqrt(SQR(y1-y2) + SQR(x1-x2));
  return dis;
}


FILE		*fpt, *file;
unsigned char	*hawk_image,*hawk_init,*hawk_final,*hawk_sobel_final,*normalized;
char		header[320];
int		ROWS,COLS,BYTES;
int		con_y,con_x,r,c,i,r2,c2;
float *hawk_sobel_v;


int main()

{
  /* read input image */
  if ((fpt=fopen("hawk.ppm","rb")) == NULL)
    {
    printf("Unable to open hawk.ppm for reading\n");
    exit(0);
    }
  fscanf(fpt,"%s %d %d %d",header,&COLS,&ROWS,&BYTES);
  if (strcmp(header,"P5") != 0  ||  BYTES != 255)
    {
    printf("Not a greyscale 8-bit PPM image\n");
    exit(0);
    }

  /* Allocate dynamic memory*/
  hawk_image=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char)); 
  hawk_init=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
  hawk_sobel_v = (float *)calloc(ROWS*COLS,sizeof(float));
  hawk_final = (unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char)); 
  
 
  
  header[0]=fgetc(fpt);	          /* read white-space character that separates header */
  fread(hawk_image,1,ROWS*COLS,fpt);
  fclose(fpt);

  for (i=0;i<ROWS*COLS;i++){
    hawk_init[i] = hawk_image[i];
  }

  /* Reading hawk_init file */
  fpt = fopen("hawk_init.txt","rb");

  /* Plotting initial contour points */
  while(fscanf(fpt, "%d %d\n", &con_x,&con_y) != EOF)
  {
    for (r=-3; r<=3; r++){
      hawk_init[(con_y+r)*COLS+con_x] = 0;
    }
    for (c=-3; c<=3; c++){
      hawk_init[(con_y)*COLS+con_x+c] = 0;
    }
  }

  fclose(fpt);

  Sobelfilter(hawk_image,hawk_sobel_v,ROWS,COLS);
  normalized =(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
  normalization(hawk_sobel_v, normalized,ROWS,COLS);

  int contour_x[42];
  int contour_y[42];

  fpt = fopen("hawk_init.txt", "rb");
  int i=0;

  /* Storing initial contour points in array */
  while(fscanf(fpt, "%d %d", &con_x,&con_y) == 2) {
      contour_x[i] = con_x;
      contour_y[i] = con_y;
      i++;
  }
  fclose(file);

int iteration;
int window = 7;
float internal_energy1[window*window];
float internal_energy2[window*window];
float external_energy[window*window];
float total_energy[window*window];
float w1=1,w2=1,w3=1;

for (iteration = 0; iteration <=30; iteration++){
  float dist = 0;
  float avg_distance;

  for (i=0; i<=41; i++){
    if (i!=41){
      dist += distance(contour_y[i],contour_y[i+1], contour_x[i],contour_x[i+1]);
    }
    else{
      dist += distance(contour_y[i],contour_y[0], contour_x[i],contour_x[0]);
    }
  }
    avg_distance = dist/(42);
  
    for (i=0; i<=41;i++){
      
      int location = 0, location1 = 0, location2 = 0, location3 = 0, location4 = 0, location5 = 0, location6 = 0,location7=0,location8=0;
      int min_r,min_c;
      float new_min = 0;
      float new_max = 1;
      float max_energy1;
      float min_energy1;
      float max_energy2;
      float min_energy2;
      float max_energy_ext;
      float min_energy_ext;
      float max_total;
      float min_total;

      for (r = -(window/2); r<= (window/2); r++ ){
        for (c =-(window/2); c<=(window/2); c++ ){

          if (i!=41){
            internal_energy1[(r+(window/2))*window+(c+(window/2))] = w1*SQR(distance(contour_y[i+1],(contour_y[i]+r),contour_x[i+1],(contour_x[i]+c)));
            internal_energy2[(r+(window/2))*window+(c+(window/2))] = w2*(SQR(avg_distance - distance(contour_y[i+1],contour_y[i]+r,contour_x[i+1],contour_x[i]+c)));
            external_energy[(r+(window/2))*window+(c+(window/2))] =  w3*(0 - normalized[(contour_y[i]+r)*COLS+(contour_x[i]+c)]);
            total_energy[(r+(window/2))*window+(c+(window/2))] = internal_energy1[(r+(window/2))*window+(c+(window/2))]  + internal_energy2[(r+(window/2))*window+(c+(window/2))] +external_energy[(r+(window/2))*window+(c+(window/2))];
          }
          else{
            internal_energy1[(r+(window/2))*window+(c+(window/2))]  = w1*SQR(distance(contour_y[0],(contour_y[i]+r), contour_x[0],(contour_x[i]+c)));
            internal_energy2[(r+(window/2))*window+(c+(window/2))]  = w2*(SQR(avg_distance - distance(contour_y[0],contour_y[i]+r,contour_x[0],contour_x[i]+c)));
            external_energy[(r+(window/2))*window+(c+(window/2))]  =  w3*(0 - normalized[(contour_y[i]+r)*COLS+(contour_x[i]+c)]);
            total_energy[(r+(window/2))*window+(c+(window/2))] = internal_energy1[(r+(window/2))*window+(c+(window/2))]  + internal_energy2[(r+(window/2))*window+(c+(window/2))] +external_energy[(r+(window/2))*window+(c+(window/2))];
          }
          if (internal_energy1[(r+(window/2))*window+(c+(window/2))] < internal_energy1[location1]){
            min_energy1 = internal_energy1[(r+(window/2))*window+(c+(window/2))];
            location1 = (r+(window/2))*window+(c+(window/2));
          }
          if (internal_energy1[(r+(window/2))*window+(c+(window/2))] > internal_energy1[location2]){
            max_energy1 = internal_energy1[(r+(window/2))*window+(c+(window/2))];
            location2 = (r+(window/2))*window+(c+(window/2));
          }
          if (internal_energy2[(r+(window/2))*window+(c+(window/2))] < internal_energy2[location3]){
            min_energy2 = internal_energy2[(r+(window/2))*window+(c+(window/2))];
            location3 = (r+(window/2))*window+(c+(window/2));
          }
          if (internal_energy2[(r+(window/2))*window+(c+(window/2))] > internal_energy2[location4]){
            max_energy2 = internal_energy2[(r+(window/2))*window+(c+(window/2))];
            location4 = (r+(window/2))*window+(c+(window/2));
          }
        if (external_energy[(r+(window/2))*window+(c+(window/2))] < external_energy[location5]){
            min_energy_ext = external_energy[(r+(window/2))*window+(c+(window/2))];
            location5 = (r+(window/2))*window+(c+(window/2));
          }
        if (external_energy[(r+(window/2))*window+(c+(window/2))] > external_energy[location6]){
            max_energy_ext = external_energy[(r+(window/2))*window+(c+(window/2))];
            location6 = (r+(window/2))*window+(c+(window/2));
          }

        if (total_energy[(r+(window/2))*window+(c+(window/2))] < total_energy[location7]){
          min_total = total_energy[(r+(window/2))*window+(c+(window/2))];
          location7 = (r+(window/2))*window+(c+(window/2));
          }
        if (total_energy[(r+(window/2))*window+(c+(window/2))] > total_energy[location8]){
          max_total = total_energy[(r+(window/2))*window+(c+(window/2))];
          location8 = (r+(window/2))*window+(c+(window/2));
          }
        }
      }
      float normalized_energy1[window*window], normalized_energy2[window*window], normalized_energy_ext[window*window],normalized_total[window*window];

      for (r = -(window/2); r<= (window/2); r++ ){
        for (c =-(window/2); c<=(window/2); c++ ){
          normalized_energy1[(r+(window/2))*window+(c+(window/2))] = ((internal_energy1[(r+(window/2))*window+(c+(window/2))]-min_energy1)*((new_max-new_min)/(max_energy1-min_energy1)))+new_min; 
          normalized_energy2[(r+(window/2))*window+(c+(window/2))] = ((internal_energy2[(r+(window/2))*window+(c+(window/2))]-min_energy2)*((new_max-new_min)/(max_energy2-min_energy2)))+new_min; 
          normalized_energy_ext[(r+(window/2))*window+(c+(window/2))] =((external_energy[(r+(window/2))*window+(c+(window/2))]-min_energy_ext)*((new_max-new_min)/(max_energy_ext-min_energy_ext)))+new_min; 
          normalized_total[(r+(window/2))*window+(c+(window/2))] = ((total_energy[(r+(window/2))*window+(c+(window/2))]-min_total)*((new_max-new_min)/(max_total-min_total)))+new_min;  
      

          if (normalized_total[(r+(window/2))*window+(c+(window/2))] < normalized_total[location]){
            location = (r+(window/2))*window+(c+(window/2));
            min_r = r;
            min_c = c;
          }
        }
      }    

      //printf("%d %d are inital\n", contour_y[i],contour_x[i],i);
      contour_y[i] = contour_y[i]+ min_r; 
      contour_x[i] = contour_x[i]+ min_c;
    // printf("%d %d %d %d are final\n", contour_y[i],contour_x[i],iteration,i);
    }
    iteration++;
  }

  for (i=0;i<ROWS*COLS;i++){
    hawk_final[i] = hawk_image[i];
  }

    for (i=0;i<=41;i++){
      for (r=-3; r<=3; r++){
        hawk_final[(contour_y[i]+r)*COLS+(contour_x[i])] = 0;
      }
      for (c=-3; c<=3; c++){
        hawk_final[(contour_y[i])*COLS+(contour_x[i]+c)] = 0;
      }
    }

  fpt=fopen("final_contourpoints.txt","wb");
    if (fpt == NULL)
    {
    printf("Unable to open final_contourpoints.txt for writing\n");
    exit(0);
    }

  for (i=0; i<=41;i++){
    fprintf(fpt, "%d,%d\n",contour_x[i],contour_y[i]);
  }
  fclose(fpt);
  

  fpt=fopen("hawk_initial.ppm","wb");
  fprintf(fpt,"P5 %d %d 255\n",COLS,ROWS);
  fwrite(hawk_init,COLS*ROWS,1,fpt);
  fclose(fpt);

  fpt=fopen("hawk_original.ppm","wb");
  fprintf(fpt,"P5 %d %d 255\n",COLS,ROWS);
  fwrite(hawk_image,COLS*ROWS,1,fpt);
  fclose(fpt);

  fpt=fopen("hawk_sobel.ppm","wb");
  fprintf(fpt,"P5 %d %d 255\n",COLS,ROWS);
  fwrite(normalized,COLS*ROWS,1,fpt);
  fclose(fpt);

  fpt=fopen("hawk_final.ppm","wb");
  fprintf(fpt,"P5 %d %d 255\n",COLS,ROWS);
  fwrite(hawk_final,COLS*ROWS,1,fpt);
  fclose(fpt);
}