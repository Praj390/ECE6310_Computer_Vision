#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

FILE		*fpt, *file;
unsigned char	*image,*temp, *copy_t,*copy, *copy_threshold, *normal_image, *final_image, *msf_e, *temporary,*branch;
char		header[320], header_1[320],letter[10],gt_letter[10];
int		ROWS,COLS,bytes,rows,cols,BYTES;
int		r,c,i,j,c1,c2,r1,r2,n,T,or,oc,tp,tn,fp,fn,detected=0;



/* Creates copy and thresholds */
unsigned char* img_copy(int r, int c) 
{  
  copy=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
  i = 0;
  for (r1 = r-7; r1 <= (r + 7); r1++)
  { 
    j = 0;
    for (c1 = c-4; c1 <= (c + 4); c1++)
    {
      copy[i*COLS+j] = image[r1*cols+c1];
      j++;
    }
    
    i++;
  }

  for (i = 0; i < ROWS*COLS; i++)
  {
    if (copy[i] < 128)
    {
      copy_threshold[i] = 255;
    }
    else
    {
      copy_threshold[i] = 0;
    }
  }
  
  return copy_threshold;
} 

void thinning(unsigned char *thinned)
{
  int row, col, i, neighbors, edge_to_nonedge;
  bool erase;
  bool check;


  erase = true;
  while (erase)
  {
    erase = false;
    for (row = 0; row < 15; row++)
    {
      for (col = 0; col < 9; col++)
      {
        i = row*9+col;
        /* Checking if pixel is on*/
        if ((thinned[i] > 0))
        {
          edge_to_nonedge = 0;
          neighbors = 0;
          /* North */
          if (thinned[i-9] == 255)
          {
            neighbors++;
            check = true;
          }
          else
          {
            check = false;
          }
          /* North East */
          if (thinned[i-9+1] == 255)
          {
            neighbors++;
            check = true;
          }
          else
          {
            if (check)
            {
              edge_to_nonedge++;
            }
            check = false;
          }
          /* East */
          if (thinned[i+1] == 255)
          {
            neighbors++;
            check = true;
          }
          else
          {
            if (check)
            {
              edge_to_nonedge++;
            }
            check = false;
          }
          /* South East */
          if (thinned[i+1+9] == 255)
          {
            neighbors++;
            check = true;
          }
          else
          {
            if (check)
            {
              edge_to_nonedge++;
            }
            check = false;
          }
          /* South */
          if (thinned[i+9] == 255)
          {
            neighbors++;
            check = true;
          }
          else
          {
            if (check)
            {
              edge_to_nonedge++;
            }
            check = false;
          }
          /* South West */
          if (thinned[i+9-1] == 255)
          {
            neighbors++;
            check = true;
          }
          else
          {
            if (check)
            {
              edge_to_nonedge++;
            }
            check = false;
          }
          /* West */
          if (thinned[i-1] == 255)
          {
            neighbors++;
            check = true;
          }
          else
          {
            if (check)
            {
              edge_to_nonedge++;
            }
            check = false;
          }
          /* North West */
          if (thinned[i-1-9] == 255)
          {
            neighbors++;
            check = true;
          }
          else
          {
            if (check)
            {
              edge_to_nonedge++;
            }
            check = false;
          }
          /* loop to check edge to non edge for north west to north*/
          if ((thinned[i-9] == 0) && check)
          {
            edge_to_nonedge++;
          }

          /* loop to erase the pixel */
          if (edge_to_nonedge == 1)
          {
            if (2 <= neighbors && neighbors <= 6)
            {
              if ((thinned[i-9] == 0) || (thinned[i+1] == 0) || ((thinned[i-1] == 0) && (thinned[i+9] == 0)))
              {
                thinned[i] = 0;
                erase = true;
              }
            }
          }
        }
      }
    }
  }
  return;
}





int branch_find(unsigned char *thinned)
{ 
  int edge_to_nonedge,row,col,branch_pt=0,end_pt=0,detected;
  bool check;
  /* funtion that return detected = 1 or 0 */
  for (row = 0; row < 15; row++)
    {
      for (col = 0; col < 9; col++)
      {
        i = row*9+col;
        /* Checking if pixel is on*/
        if ((thinned[i] > 0))
        {
          edge_to_nonedge = 0;
          /* North */
          if (thinned[i-9] == 255)
          {
            check = true;
          }
          else
          {
            check = false;
          }
          /* North East */
          if (thinned[i-9+1] == 255)
          {
            check = true;
          }
          else
          {
            if (check)
            {
              edge_to_nonedge++;
            }
            check = false;
          }
          /* East */
          if (thinned[i+1] == 255)
          {
            check = true;
          }
          else
          {
            if (check)
            {
              edge_to_nonedge++;
            }
            check = false;
          }
          /* South East */
          if (thinned[i+1+9] == 255)
          {
            check = true;
          }
          else
          {
            if (check)
            {
              edge_to_nonedge++;
            }
            check = false;
          }
          /* South */
          if (thinned[i+9] == 255)
          {
            check = true;
          }
          else
          {
            if (check)
            {
              edge_to_nonedge++;
            }
            check = false;
          }
          /* South West */
          if (thinned[i+9-1] == 255)
          {
            check = true;
          }
          else
          {
            if (check)
            {
              edge_to_nonedge++;
            }
            check = false;
          }
          /* West */
          if (thinned[i-1] == 255)
          {
            check = true;
          }
          else
          {
            if (check)
            {
              edge_to_nonedge++;
            }
            check = false;
          }
          /* North West */
          if (thinned[i-1-9] == 255)
          {
            check = true;
          }
          else
          {
            if (check)
            {
              edge_to_nonedge++;
            }
            check = false;
          }
          /* loop to check edge to non edge for north west to north*/
          if ((thinned[i-9] == 0) && check)
          {
            edge_to_nonedge++;
          }

          if (edge_to_nonedge == 1)
          {
            end_pt++;
          }
          if (edge_to_nonedge > 2)
          {
            branch_pt++;
          }
        }
      }
    }
  if (branch_pt == 1 && end_pt ==1)
  {
    detected = 1;
  }
  else
  {
    detected = 0;
  }

  return detected;
}

int main()

{
  /* read template */
  if ((fpt=fopen("parenthood_e_template.ppm","rb")) == NULL)
    {
    printf("Unable to open parenthood_e_template.ppm for reading\n");
    exit(0);
    }
  fscanf(fpt,"%s %d %d %d",header_1,&COLS,&ROWS,&bytes);
  if (strcmp(header_1,"P5") != 0  ||  bytes != 255)
    {
    printf("Not a greyscale 8-bit PPM image\n");
    exit(0);
    }

  /* Allocate dynamic memory for template and zero_min_template */
  temp=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
  temporary=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
  copy_t=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
  copy_threshold=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
  branch = (unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
  
  header_1[0]=fgetc(fpt);	          /* read white-space character that separates header */
  fread(temp,1,ROWS*COLS,fpt);
  fclose(fpt);

  /* Reading parenthood image */
  if ((fpt=fopen("parenthood.ppm","rb")) == NULL)
    {
    printf("Unable to open parenthood.ppm for reading\n");
    exit(0);
    }
  fscanf(fpt,"%s %d %d %d",header,&cols,&rows,&BYTES);
  if (strcmp(header,"P5") != 0  ||  BYTES != 255)
    {
    printf("Not a greyscale 8-bit PPM image\n");
    exit(0);
    }

  image=(unsigned char *)calloc(rows*cols,sizeof(unsigned char));
  header[0]=fgetc(fpt);	/* read white-space character that separates header */
  fread(image,1,cols*rows,fpt);
  fclose(fpt);


  /* Reading MSF_E image */
  if ((fpt=fopen("msf_e.ppm","rb")) == NULL)
    {
    printf("Unable to open msf_e.ppm for reading\n");
    exit(0);
    }
  fscanf(fpt,"%s %d %d %d",header,&cols,&rows,&BYTES);
  if (strcmp(header,"P5") != 0  ||  BYTES != 255)
    {
    printf("Not a greyscale 8-bit PPM image\n");
    exit(0);
    }

  msf_e = (unsigned char *)calloc(rows*cols,sizeof(unsigned char));
  header[0]=fgetc(fpt);	/* read white-space character that separates header */
  fread(msf_e,1,cols*rows,fpt);
  fclose(fpt);

  /* Creating a data.csv file to store tn fn and other */

  file = fopen("Data.csv", "wb");
  fprintf(file, "Threshold,TP,FP,TN,FN,TPR,FPR\n");

 

  for (T =0; T < 256; T++)
  {
    /* Reading parenthood_gt file */
    fpt = fopen("parenthood_gt.txt","rb");
    tp = fp = fn = tn = 0;
    strcpy(letter, "e");

    /* Checking pixel value above threshold */
    while((fscanf(fpt, "%s %d %d\n", gt_letter, &oc, &or)) != EOF)
    {
      for (r1 = or-7; r1 <= (or + 7); r1++)
      {
        for (c1 = oc-4; c1 <= (oc + 4); c1++)
        {
          if (msf_e[(r1 * cols) + c1] > T)
          {
            detected = 1;
          }
        }
      }
      
      if (detected == 1)
      {
        /* Creating a 9x15 copy of image(or,oc) and thresholding at value of 128 */
        copy_t = img_copy(or,oc);
        
        /* Apply thining on copied image */
        thinning(copy_t);

        for (i=0;i<9*15;i++){
          branch[i] = copy_t[i];
        }

        detected = branch_find(branch);
      }

      /* Finding values of TP, FP, FN and TN */
      if ((detected == 1) && (strcmp(gt_letter, letter) == 0))
      {
        tp++;
      }
      if ((detected == 1) && (strcmp(gt_letter, letter) != 0))
      {
        fp++;
      }
      if ((detected == 0) && (strcmp(gt_letter, letter) == 0))
      {
        fn++;
      }
      if ((detected == 0) && (strcmp(gt_letter, letter) != 0))
      {
        tn++;
      }
      detected = 0;
    }


    printf("Threshold: %d, TP: %d, FP: %d, TN: %d, FN: %d, TPR: %lf, FPR: %lf\n",T,tp,fp,tn,fn,(double)tp/(double)(tp+fn),(double)fp/(double)(fp+tn));
    fprintf(file, "%d,%d,%d,%d,%d,%lf,%lf\n",T,tp,fp,tn,fn,(double)tp/(double)(tp+fn),(double)fp/(double)(fp+tn));
  }
  fclose(file);

  fpt=fopen("center.ppm","wb");
  fprintf(fpt,"P5 %d %d 255\n",COLS,ROWS);
  fwrite(copy,COLS*ROWS,1,fpt);
  fclose(fpt);

  
  fpt=fopen("thinned.ppm","wb");
  fprintf(fpt,"P5 %d %d 255\n",COLS,ROWS);
  fwrite(copy_t,COLS*ROWS,1,fpt);
  fclose(fpt);
}