
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


	struct inputmat
	{
		int row;
		int col;
		int *Addr;
		double *names;
		int NbItem;
		int* ItemRow;
		int* ColPos;
	};


	inputmat getMatrixofSparse(int argNum, int rows, int cols, int *Address, double *name )
	{
		inputmat matrix;
		int iNbItem			= 0;
		int* piNbItemRow	= NULL;
		int* piColPos		= NULL;
		SciErr sciErr = getVarAddressFromPosition(pvApiCtx, argNum, &Address);
	    if (sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	    }
	    sciErr = getVarDimension(pvApiCtx, Address, &rows, &cols);

	    /* Check that the input argument is a real matrix (and not complex) */
	    if ( !isSparseType(pvApiCtx, Address) ||  isVarComplex(pvApiCtx, Address) )
	    {
	        Scierror(999, "Wrong type for input argument #%d: A sparse matrix expected.\n", argNum);
	    }

	    /* get matrix */
	    sciErr = getSparseMatrix(pvApiCtx, Address, &rows, &cols, &iNbItem, &piNbItemRow, &piColPos, &name);
	    if (sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	    }
	   
	    matrix.row = rows;
	    matrix.col = cols;
	    matrix.Addr = Address;
	    matrix.names = name;
	    matrix.NbItem = iNbItem;
	    matrix.ItemRow = piNbItemRow;
	    matrix.ColPos = piColPos;
	    return matrix;
	}
	int csdp(char *fname, unsigned long fname_len)
	{
		SciErr sciErr;
		/*For the objective function*/
		int m1 = 0, n1 = 0;
	    double *C1= NULL;

		/*For the Constraint LHS*/
	    int m2 = 0, n2 = 0;
	    int *A0Address = NULL;
	    double *A0 = NULL;
	    int *AItemRow = NULL;
	    int* AColPos = NULL;



	    /*For the Constraint RHS*/
	   	int m3 = 0, n3 = 0;
	    double *b0 = NULL;

	     /*For the tracker*/
	    int m4 = 0, n4 = 0;
	    double *K1 = NULL;

	    int i = 0;
	    int j = 0;
	    int k = 0;
	    int l = 0;
	    int m = 0;
	    int n = 0;
	    if( getDoubleMatrixFromScilab(1, &m1, &n1, &C1 ))
	    {
	    	return 1;
	    }
	    double (*C0)[n1] = (double (*)[n1])&C1[0];
	    for (i = 0;i<m1;i++)
		{
			for (j = 0; j < n1; j++)
			{
				sciprint("%.2f  ", C0[i][j]);
			}
			sciprint("\n" );
		}
		sciprint("\n" );

		inputmat mat2 = getMatrixofSparse(2, m2, n2, A0Address, A0);	
		m2 = mat2.row;
		n2 = mat2.col;
		A0Address = mat2.Addr;
	    A0 = mat2.names;
	    AItemRow = mat2.ItemRow;
	    AColPos = mat2.ColPos;
	    int quot = m2/n2;

	    int sparse_sizes[quot+1];
	    sparse_sizes[0] = 0;
   	    k = 0;
	    for(i = 0 ; i < m2 ; i++)
		{
			for(j = 0 ; j < AItemRow[i] ; j++)
			{
				sciprint("(%d,%d) = %f", i+1, AColPos[k], A0[k]);

				sciprint("\n");
				k++;
			}
					
			if ((i+1)%n2 == 0 )
			{
				m = m +sparse_sizes[l];
				sparse_sizes[l+1] = k -m;
				l++;
			}
			
			
		}
		sciprint("\n" );
		if( getDoubleMatrixFromScilab(3, &m3, &n3, &b0 ))
		{
			return 1;
		}

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
	   

struct blockmatrix C;
  double *b;
  struct constraintmatrix *constraints;

  /*
   * Storage for the initial and final solutions.
   */

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
	    int inc= 0; /*to increment the C0 matrix indices to obtain the block indices*/
	    for (i = 0; i<n4;i++)
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

	   

	      constraints=(struct constraintmatrix *)malloc((2+1)*sizeof(struct constraintmatrix));
  if (constraints==NULL)
    {
      sciprint("Failed to allocate storage for constraints!\n");
      exit(1);
    };

  /*
   * Setup the A1 matrix.  Note that we start with block 3 of A1 and then
   * do block 1 of A1.  We do this in this order because the blocks will
   * be inserted into the linked list of A1 blocks in reverse order.  
   */

  /*
   * Terminate the linked list with a NULL pointer.
   */

  constraints[1].blocks=NULL;

  /*
   * Now, we handle block 3 of A1.
   */

  /*
   * Allocate space for block 3 of A1.
   */

  blockptr=(struct sparseblock *)malloc(sizeof(struct sparseblock));
  if (blockptr==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };

  /*
   * Initialize block 3.
   */

 blockptr->blocknum=3;
  blockptr->blocksize=2;
  blockptr->constraintnum=1;
  blockptr->next=NULL;
  blockptr->nextbyblock=NULL;
  blockptr->entries=(double *) malloc((1+1)*sizeof(double));
  if (blockptr->entries==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };
  blockptr->iindices=(unsigned short *) malloc((1+1)*sizeof(int));
  if (blockptr->iindices==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };
  blockptr->jindices=(unsigned short *) malloc((1+1)*sizeof(int));
  if (blockptr->jindices==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };

  

   blockptr->numentries=1;



  blockptr->iindices[1]=1;
  blockptr->jindices[1]=1;
  blockptr->entries[1]=1.0;

 

  blockptr->next=constraints[1].blocks;
  constraints[1].blocks=blockptr;

 

  blockptr=(struct sparseblock *)malloc(sizeof(struct sparseblock));
  if (blockptr==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };

 

  blockptr->blocknum=1;
  blockptr->blocksize=2;
  blockptr->constraintnum=1;
  blockptr->next=NULL;
  blockptr->nextbyblock=NULL;
  blockptr->entries=(double *) malloc((3+1)*sizeof(double));
  if (blockptr->entries==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };
  blockptr->iindices=(unsigned short *) malloc((3+1)*sizeof(int));
  if (blockptr->iindices==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };
  blockptr->jindices=(unsigned short *) malloc((3+1)*sizeof(int));
  if (blockptr->jindices==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };

  

  blockptr->numentries=3;

 

  blockptr->iindices[1]=1;
  blockptr->jindices[1]=1;
  blockptr->entries[1]=3.0;

 
  blockptr->iindices[2]=1;
  blockptr->jindices[2]=2;
  blockptr->entries[2]=1.0;

 

  blockptr->iindices[3]=2;
  blockptr->jindices[3]=2;
  blockptr->entries[3]=3.0;

  

  blockptr->next=constraints[1].blocks;
  constraints[1].blocks=blockptr;

  

  constraints[2].blocks=NULL;

  

  blockptr=(struct sparseblock *)malloc(sizeof(struct sparseblock));
  if (blockptr==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };

  

  blockptr->blocknum=3;
  blockptr->blocksize=2;
  blockptr->constraintnum=2;
  blockptr->next=NULL;
  blockptr->nextbyblock=NULL;
  blockptr->entries=(double *) malloc((1+1)*sizeof(double));
    if (blockptr->entries==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };
  blockptr->iindices=(unsigned short *) malloc((1+1)*sizeof(int));
  if (blockptr->iindices==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };
  blockptr->jindices=(unsigned short *) malloc((1+1)*sizeof(int));
  if (blockptr->jindices==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };

  

  blockptr->numentries=1;



  blockptr->iindices[1]=2;
  blockptr->jindices[1]=2;
  blockptr->entries[1]=1.0;

 

  blockptr->next=constraints[2].blocks;
  constraints[2].blocks=blockptr;

 

  blockptr=(struct sparseblock *)malloc(sizeof(struct sparseblock));
  if (blockptr==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };

  

  blockptr->blocknum=2;
  blockptr->blocksize=3;
  blockptr->constraintnum=2;
  blockptr->next=NULL;
  blockptr->nextbyblock=NULL;
  blockptr->entries=(double *) malloc((4+1)*sizeof(double));
  if (blockptr->entries==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };
  blockptr->iindices=(unsigned short *) malloc((4+1)*sizeof(int));
  if (blockptr->iindices==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };
  blockptr->jindices=(unsigned short *) malloc((4+1)*sizeof(int));
  if (blockptr->jindices==NULL)
    {
      sciprint("Allocation of constraint block failed!\n");
      exit(1);
    };

 

  blockptr->numentries=4;


  

  blockptr->iindices[1]=1;
  blockptr->jindices[1]=1;
  blockptr->entries[1]=3.0;

  

  blockptr->iindices[2]=2;
  blockptr->jindices[2]=2;
  blockptr->entries[2]=4.0;

  

  blockptr->iindices[3]=3;
  blockptr->jindices[3]=3;
  blockptr->entries[3]=5.0;

  

  blockptr->iindices[4]=1;
  blockptr->jindices[4]=3;
  blockptr->entries[4]=1.0;

 

  blockptr->next=constraints[2].blocks;
  constraints[2].blocks=blockptr;

  sciprint("\n");
  sciprint("Here We are");
    sciprint("\n");


 /* write_prob("prob.dat-s",7,2,C,b,constraints);*/

 
  initsoln(7,2,C,b,constraints,&X,&y,&Z);

 
sciprint("\n");
  sciprint("Here We are again");
    sciprint("\n");
  ret=easy_sdp(7,2,C,b,constraints,0.0,&X,&y,&Z,&pobj,&dobj);

  if (ret == 0)
    sciprint("The objective value is %.7e \n",(dobj+pobj)/2);
  else
    sciprint("SDP failed.\n");

  

  write_sol("prob.sol",7,2,X,y,Z);

 

  /*free_prob(7,2,C,b,constraints,X,y,Z);
  exit(0);
	    */

	    
	}
}