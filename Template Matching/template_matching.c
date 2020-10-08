
	/*
	** Demonstrates how to load a PPM image from a file,
	** do some simple image processing, and then write out
	** the result to another PPM file.
	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main ()

{
FILE		*fpt;
unsigned char	*image,*template,*normalized,*threshold;
char		header[80];
char header_temp[80],gt_letter[80];
int		ROWS,COLS,BYTES,rows,cols,detect,detect_N,TP,FP,TN,FN,count;
int		i,j,r,c,r2,c2,r3,c3,sum,new_min,new_max,or,oc;
int final, initial,num_values,step;
float mean,min_old,max_old,*zero_mean,*convoluted,difference;
int *thresholdvalues;


fpt=fopen("parenthood.ppm","rb");
if (fpt == NULL)
  {
  printf("Unable to open %s for reading\n","parenthood.ppm");
  exit(0);
  }
	/* read image header (simple 8-bit greyscale PPM only) */
i=fscanf(fpt,"%s %d %d %d",header,&cols,&rows,&BYTES);
if (i != 4  ||  strcmp(header,"P5") != 0  ||  BYTES != 255)
  {
  printf("Image is not an 8-bit PPM greyscale (P5) image\n");
  fclose(fpt);
  exit(0);
  }
	/* allocate dynamic memory for image */
image=(unsigned char *)calloc(rows*cols,sizeof(unsigned char));
if (image == NULL)
  {
  printf("Unable to allocate %d x %d memory\n",cols,rows);
  exit(0);
  }
	/* read image data from file */
header[0] = fgetc(fpt);
fread(image,1,rows*cols,fpt);
fclose(fpt);


fpt=fopen("parenthood_e_template.ppm","rb");
if (fpt == NULL)
  {
  printf("Unable to open %s for reading\n","parenthood_e_template.ppm");
  exit(0);
  }
	/* read image header (simple 8-bit greyscale PPM only) */
i=fscanf(fpt,"%s %d %d %d",header,&COLS,&ROWS,&BYTES);
if (i != 4  ||  strcmp(header,"P5") != 0  ||  BYTES != 255)
  {
  printf("Image is not an 8-bit PPM greyscale (P5) image\n");
  fclose(fpt);
  exit(0);
  }
	/* allocate dynamic memory for image */
template=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
if (template == NULL)
  {
  printf("Unable to allocate %d x %d memory\n",COLS,ROWS);
  exit(0);
  }
	/* read image data from file */
header_temp[0] = fgetc(fpt); 
fread(template,1,ROWS*COLS,fpt);
fclose(fpt);


	/* simple CV operation to add b to every pixel */
sum = 0;
for (r=0; r<ROWS; r++)
for (c=0;c<COLS;c++)
  {
  sum = sum + template[r*COLS+c];
  }
printf("sum is %d\n",sum);
mean = (float)sum/(ROWS * COLS);
printf("mean is %f\n",mean);

zero_mean=(float *)calloc(ROWS*COLS,sizeof(float));

/*for (i=0;i<ROWS*COLS;i++){
  zero_mean[i] = template[i];
}*/

for (r=0; r<ROWS; r++){
for (c=0;c<COLS;c++){
 j = (float)template[r*COLS+c]-mean ;
 zero_mean[r*COLS+c] = (float)j;
}
}

convoluted = (float *)calloc(rows*cols,sizeof(float));

for (r=7; r<(rows-7); r++){
for (c=4;c<(cols-4);c++){
  sum = 0;
  for (r2=-7;r2<(ROWS-7);r2++){
  for (c2=-4;c2<(COLS-4);c2++){
      sum += (image[(r+r2)*cols+c+c2] * zero_mean[(r2+(ROWS/2))*COLS+(c2+(COLS/2))]);
    }
  }
  convoluted[r*cols+c] = sum;
}
}

max_old = 0;
for(i=0;i<rows*cols;i++){
  if (convoluted[i] > max_old){
    max_old = convoluted[i];
  }
}

min_old = 0;
for(i=0;i<rows*cols;i++){
  if (convoluted[i] < min_old){
    min_old = convoluted[i];
  }
}

printf("max %f min %f\n",max_old,min_old);

new_min = 0;
new_max =255;
normalized = (unsigned char *)calloc(rows*cols,sizeof(unsigned char));
for (r=0; r<rows; r++){
for (c=0;c<cols;c++){
  normalized[r*cols+c] = ((convoluted[r*cols+c]-min_old)*((new_max-new_min)/(max_old-min_old)))+new_min; 
}
}
fpt=fopen("normalized.ppm","wb");
fprintf(fpt,"P5 %d %d 255\n",cols,rows);
fwrite(normalized,cols*rows,1,fpt);
fclose(fpt);


threshold = (unsigned char *)calloc(rows*cols,sizeof(unsigned char));

for (i=0;i<rows*cols;i++){
  threshold[i] = normalized[i];
}

  initial = 0;
  final = 255;
  num_values = 255;
  thresholdvalues = (int *)calloc(num_values,sizeof(int));
  step = (final - initial) / num_values;
  int k = 0;
  int ct = 0;
  thresholdvalues[j] = initial;
  for(k = 1; k<=num_values; k++){
      thresholdvalues[k] = thresholdvalues[k-1] + step;
      ct+=1;
  }

float TPR=0.0; 
float FPR=0.0;
float *TPR_Array = (float *)calloc(num_values+1,sizeof(float));
float *FPR_Array = (float *)calloc(num_values+1,sizeof(float));
int *TP_Array = (int *)calloc(num_values+1,sizeof(int));
int *FP_Array = (int *)calloc(num_values+1,sizeof(int));    
int *T = (int *)calloc(num_values+1,sizeof(int)); 

/* For Loop for T*/
int a;
for (a =initial; a<=final;a++){
  threshold = (unsigned char *)calloc(rows*cols,sizeof(unsigned char));
  for (r=0; r<rows; r++){
  for (c=0;c<cols;c++){
    if (normalized[r*cols+c] >= thresholdvalues[a]) {
      threshold[r*cols+c] =255;
    }
    else {
      threshold[r*cols+c] =0;
    }
  }
  }
  /*fpt=fopen("threshold.ppm","wb");
  fprintf(fpt,"P5 %d %d 255\n",cols,rows);
  fwrite(threshold,cols*rows,1,fpt);
  fclose(fpt);*/


  fpt = fopen("parenthood_gt.txt","rb");
  detect = 0;
  detect_N = 0;
  TP = 0;
  FP = 0;
  FN = 0;
  TN = 0;

  while(1)
  {
    detect = 0;
    detect_N = 0;

    i = fscanf(fpt,"%s %d %d",&gt_letter, &oc,&or);
  
    if (i != 3)
    break;

    for (r=(-ROWS/2);r<=(ROWS/2);r++){
      if (detect == 0){
        for(c = (-COLS/2);c<=(COLS/2);c++){
          if (normalized[(or+r)*cols+(oc+c)] > thresholdvalues[a]){
            detect = 1;
          }
        }
      }
    }

    if (detect ==0){
      detect_N = 1;
    }
    if (detect ==1 && gt_letter[0] == 'e'){
      TP++;
      }
    else if(detect==1 && gt_letter[0] != 'e'){
      FP++;
      }
    else if(detect_N==1 && gt_letter[0] != 'e'){
      TN++;
      }
    else if(detect_N==1 && gt_letter[0] == 'e'){
      FN++;
      }   
    }
  printf("TP FP TN FN for %d are %d %d %d %d \n", a,TP,FP,TN,FN);

  fclose(fpt);
  TPR = TP/(float)(TP+FN);
  FPR = FP/(float)(FP+TN);
  printf("TPR and FPR for %d are %f %f\n", a,TPR,FPR);
  TP_Array[a] = TP;
  FP_Array[a] = FP;
  T[a] = a;
  TPR_Array[a] = TPR;
  FPR_Array[a] = FPR;

}

fpt=fopen("TP_FP.txt","wb");
    if (fpt == NULL)
    {
    printf("Unable to open TPR_FPR.txt for writing\n");
    exit(0);
    }
    for(int b=initial; b<=final;b++){
        fprintf(fpt,"%d %d %d %f %f\n",T[b],TP_Array[b], FP_Array[b],TPR_Array[b], FPR_Array[b]);
   
    }
    fclose(fpt);

}
    


