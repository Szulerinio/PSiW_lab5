 
    /*
     Wybór składowej jednego z kolorów R lub G lub B

     Kompilacja:
     gcc img_torgb.c -o img_torgb -ljpeg
     ./img_torgb oko_cut.jpg oko_r.jpg r
     */

    #include <stdio.h>
    #include <jpeglib.h>
    #include <stdlib.h>
    #include <string.h>
    #include <pthread.h>

    int generate(char **par) {

//          char *jPlikIn,char *jPlikOut ,char *rgb

      char *jPlikIn  = par[0];
      char *jPlikOut;
      char *rgb = par[1];
      char temp[100] ="\0";
      strcat(temp, rgb);
      strcat(temp, jPlikIn);
      jPlikOut=temp;

      struct jpeg_decompress_struct in;
      struct jpeg_error_mgr jInErr;
      JSAMPROW inRowPointer[1];

      FILE *inFile = fopen(jPlikIn,"rb");
      if (!inFile) {
        printf("Error opening jpeg file %s\n!", jPlikIn);
        return EXIT_FAILURE;
      }

      in.err = jpeg_std_error(&jInErr);
      jpeg_create_decompress(&in);
      jpeg_stdio_src(&in,inFile);
      jpeg_read_header(&in,TRUE);
      jpeg_start_decompress(&in);

      struct jpeg_compress_struct out;
      struct jpeg_error_mgr jOutErr;

      JSAMPROW outRowPointer[1];

      FILE *outFile = fopen(jPlikOut,"wb");
      if (!outFile) { printf("Error opening file %s\n",jPlikOut);
        return EXIT_FAILURE;
      }

      out.err = jpeg_std_error(&jOutErr);
      jpeg_create_compress(&out);
      jpeg_stdio_dest(&out,outFile);

      int width            = in.output_width;
      int height           = in.output_height;
      int bytesPerPixel    = in.num_components;
      out.image_width      = width;
      out.image_height     = height;
      out.input_components = bytesPerPixel;
      out.in_color_space   = JCS_RGB;

      jpeg_set_defaults(&out);
      jpeg_start_compress(&out,TRUE);

      inRowPointer[0]  = (unsigned char *)malloc(width*bytesPerPixel);
      outRowPointer[0] = (unsigned char *)malloc(width*bytesPerPixel);

      for (int i=0;i<height;i++) {
       in.output_scanline=i;
       jpeg_read_scanlines(&in,&inRowPointer[0],1);
       for (int j=0;j<width;j++) {
    	int jb = j*bytesPerPixel;

        outRowPointer[0][jb  ]=0;
        outRowPointer[0][jb+1]=0;
        outRowPointer[0][jb+2]=0;
    	if (strcmp("r",rgb)==0) outRowPointer[0][jb  ]  =inRowPointer[0][jb];
    	if (strcmp("g",rgb)==0) outRowPointer[0][jb +1 ]=inRowPointer[0][jb+1];
    	if (strcmp("b",rgb)==0) outRowPointer[0][jb +2 ]=inRowPointer[0][jb+2];
       }
       jpeg_write_scanlines(&out,&outRowPointer[0],1);
      }

      free(outRowPointer[0]);
      free(inRowPointer[0]);

      jpeg_finish_compress(&out);
      jpeg_destroy_compress(&out);
      fclose(outFile);
      jpeg_finish_decompress(&in);
      jpeg_destroy_decompress(&in);
      fclose(inFile);
      return EXIT_SUCCESS;
    }


    int main(int act, char **par) {
        par+=1;

        pthread_t red, green, blue;
        const char *arrayR[2], *arrayG[2], *arrayB[2];
        arrayR[0] = par[0];
        arrayG[0] = par[0];
        arrayB[0] = par[0];


        arrayR[1] = "r";
        arrayG[1] = "g";
        arrayB[1] = "b";

        pthread_create(&red, NULL, generate, arrayR);
        pthread_create(&green, NULL, generate,arrayG);
        pthread_create(&blue, NULL, generate, arrayB);
        pthread_join(red, NULL);
        pthread_join(green, NULL);
        pthread_join(blue, NULL);
        return EXIT_SUCCESS;

//         generate(par[0],par[1],par[2]);

    }
