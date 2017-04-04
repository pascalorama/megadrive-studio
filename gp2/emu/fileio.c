
#include "shared.h"

/*
    Load a normal file, or ZIP/GZ archive.
    Returns NULL if an error occured.
*/
uint8 *load_archive(char *filename, int *file_size)
{
    int size = 0;
    uint8 *buf = NULL;
	struct		stat statbuf;

	FILE *gd = NULL;

    /* Open file */
    gd = fopen(filename, "rb");
    if(!gd) return (0);

    /* Get file size */
    if (stat(filename, &statbuf) != 0) 
	{	return(0);}
	size= statbuf.st_size;
	
    /* Allocate file data buffer */
    buf = malloc(size);
    if(!buf)
    {
        fclose(gd);
        return (0);
    }

    /* Read file data */
    fread(buf,size,1,gd);

    /* Close file */
    fclose(gd);

    /* Update file size and return pointer to file data */
    *file_size = size;
    return (buf);
}