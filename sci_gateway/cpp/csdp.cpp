
#include "sci_iofunc.hpp"
extern "C"
{
	#include "stdio.h"
	#include "stdlib.h"

	#include </usr/include/scilab/api_scilab.h>
	#include </usr/include/scilab/Scierror.h>
	#include </usr/include/scilab/BOOL.h>
	#include </usr/include/scilab/sciprint.h>
	#include </home/mr_robot/csdp/thirdparty/linux/include/declarations.h>



	
	int csdp(char *fname, unsigned long fname_len)
	{
		SciErr sciErr;
		/*For the objective function*/
		int m1 = 0, n1 = 0;
	    double *C1= NULL;

		/*For the Constraint LHS*/
	    int m2 = 0, n2 = 0;
	    double *A1 = NULL;



	    /*For the Constraint RHS*/
	   	int m3 = 0, n3 = 0;
	    double *b0 = NULL;

	     /*For the blocksizes*/
	    int m4 = 0, n4 = 0;
	    double *K1 = NULL;

	    /*For the array containing number of elements in each block*/
	    int m5 = 0, n5 = 0;
	    double *nument = NULL;

	    int i = 0;
	    int j = 0;
	    int k = 0;
	    if( getDoubleMatrixFromScilab(1, &n1, &m1, &C1 ))
	    {
	    	return 1;
	    }
	    double (*C0)[n1] = (double (*)[n1])&C1[0];
	    
		sciprint("Loaded objective function\n" );
		
		/*Getting Constraints*/
		if( getDoubleMatrixFromScilab(2, &n2, &m2, &A1 ))
	    {
	    	return 1;
	    }
	    double (*A0)[n2] = (double (*)[n2])&A1[0];
	    
	    sciprint("Loaded constraints\n" );
	    
		sciprint("\n" );
		/*Getting RHS of constraints*/
		if( getDoubleMatrixFromScilab(3, &m3, &n3, &b0 ))
		{
			return 1;
		}
		/* Getting the blocksizes*/
		if(getDoubleMatrixFromScilab(4, &m4, &n4, &K1 ))
		{
			return 1;
		}
	    int K0[m4+1];
	    K0[0] = 0;
	    for (i = 1; i <= n4;i++)
	    {
	    	K0[i] = (int) K1[i-1];
	    }

	    /*Getting the array containing number of elements in each block*/
	    if(getDoubleMatrixFromScilab(5, &m5, &n5, &nument))
		{
			return 1;
		}


	   

struct blockmatrix C;
  double *b;
  struct constraintmatrix *constraints;

  /*
   * Storage for the initial and final solutions.
   */
sciprint("Defined blockmatrix\n" );
  struct blockmatrix X,Z;
  double *y;
  double pobj,dobj;

  /*
   * blockptr will be used to point to blocks in constraint matrices.
   */

  struct sparseblock *blockptr;

  /*
   * A return code for the call to easy_sdp().
   */
	int ret;
	C.nblocks= n4;

	C.blocks=(struct blockrec *)malloc((C.nblocks+1)*sizeof(struct blockrec));
	if (C.blocks == NULL)
    {
      sciprint("Couldn't allocate storage for C!\n");
      return 1;
    };

    C.nblocks= n4;

	C.blocks=(struct blockrec *)malloc((C.nblocks+1)*sizeof(struct blockrec));
	if (C.blocks == NULL)
    {
      sciprint("Couldn't allocate storage for C!\n");
      return 1;
    };

    for (i = 0; i<n4;i++)
    {

    	C.blocks[i+1].blockcategory=MATRIX;
		C.blocks[i+1].blocksize=K0[i+1];
		C.blocks[i+1].data.mat=(double *)malloc((int)(K0[i+1]*K0[i+1])*sizeof(double));
		if (C.blocks[i+1].data.mat == NULL)
		{
		    sciprint("Couldn't allocate storage for C!\n");
	      return 1;
		};
    }
    sciprint("Allocated blockmatrix\n" );

    int inc= 0; /*to increment the C0 matrix indices to obtain the block indices*/
   /* for (i = 0; i<n4;i++)
    {
    	
    	inc = inc+K0[i];
    	for (j = 0; j <K0[i+1];j++)
    	{
    		for (k = 0; k<K0[i+1];k++)
    		{
    			C.blocks[i+1].data.mat[ijtok(j,k,K0[i+1])]=C0[inc+j][inc+k];
    			sciprint("%.2f  ",C.blocks[i+1].data.mat[ijtok(j,k,K0[i+1])]);
    		}
    		sciprint("\n");	    		
    	}
    }*/
    j = 1;
    for (i = 0; i<m1;i++)
    {
    	C.blocks[int(C0[i][1])].data.mat[ijtok(int(C0[i][2]),int(C0[i][3]),K0[j+1])]=C0[i][4];
    	sciprint("Allocated %i %i element of %i block\n with value %.2f",int(C0[i][2]), int(C0[i][3]),int(C0[i][1]),C0[i][4]);
    	sciprint("%i\n",j);
    	if (i<m1-1)
    	{
    		if (int(C0[i][1]) != int(C0[i+1][1]))
    		{
    			j = j+1;
    		}
    	}
    }


     /*Build the RHS of the constraints*/
   	b=(double *)malloc((n3+1)*sizeof(double));
  	if (b==NULL)
    {
      sciprint("Failed to allocate storage for a!\n");
      exit(1);
    };
    for (i =0;i<n3;i++)
    {
    	b[i+1] = b0[i];
    }

   

    constraints=(struct constraintmatrix *)malloc((n3+1)*sizeof(struct constraintmatrix));
	if (constraints==NULL)
	{
	  sciprint("Failed to allocate storage for constraints!\n");
	  exit(1);
	};


	for (i = 1;i<=int(C0[m2][1]);i++)
	{
		constraints[i].blocks=NULL;
	}
	
		
		

	for (i = 0; i < m2;i++)
	{
		blockptr=(struct sparseblock *)malloc(sizeof(struct sparseblock));
		if (blockptr==NULL)
		{
			sciprint("Allocation of constraint block failed!\n");
			exit(1);
		};


	}



	



 
 


 /* write_prob("prob.dat-s",7,2,C,b,constraints);*/

 
 /* initsoln(7,2,C,b,constraints,&X,&y,&Z);

 
sciprint("\n");
  sciprint("Here We are again");
    sciprint("\n");
  ret=easy_sdp(7,2,C,b,constraints,0.0,&X,&y,&Z,&pobj,&dobj);

  if (ret == 0)
    sciprint("The objective value is %.7e \n",(dobj+pobj)/2);
  else
    sciprint("SDP failed.\n");

  

  write_sol("prob.sol",7,2,X,y,Z);*/

 

  /*free_prob(7,2,C,b,constraints,X,y,Z);
  exit(0);
	    */

	    
	}
}