
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

	inputmat getDoubleMatrix(int argNum, int rows, int cols, int *Address, double *name )
	{
		inputmat matrix;
		SciErr sciErr = getVarAddressFromPosition(pvApiCtx, argNum, &Address);
	    if (sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	    }
	    sciErr = getVarDimension(pvApiCtx, Address, &rows, &cols);

	    /* Check that the input argument is a real matrix (and not complex) */
	    if ( !isDoubleType(pvApiCtx, Address) ||  isVarComplex(pvApiCtx, Address) )
	    {
	        Scierror(999, "Wrong type for input argument #%d: A real matrix expected.\n", argNum);
	    }

	    /* get matrix */
	    sciErr = getMatrixOfDouble(pvApiCtx, Address, &rows, &cols,&name);
	    if (sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	    }
	   
	    matrix.row = rows;
	    matrix.col = cols;
	    matrix.Addr = Address;
	    matrix.names = name;
	    return matrix;
	}


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
	    int *C0Address = NULL;
	    double *C1= NULL;

		/*For the Constraint LHS*/
	    int m2 = 0, n2 = 0;
	    int *A0Address = NULL;
	    double *A0 = NULL;
	    int *AItemRow = NULL;
	    int* AColPos = NULL;



	    /*For the Constraint RHS*/
	   	int m3 = 0, n3 = 0;
	    int *B0Address = NULL;
	    double *b0 = NULL;

	     /*For the tracker*/
	    int m4 = 0, n4 = 0;
	    int *K0Address = NULL;
	    double *K1 = NULL;

	    int i = 0;
	    int j = 0;
	    int k = 0;
	    int l = 0;
	    int m = 0;
	    int n = 0;
	    inputmat mat1 = getDoubleMatrix(1, m1, n1, C0Address, C1 );	
		m1 = mat1.row;
		n1 = mat1.col;
		C0Address = mat1.Addr;
	    C1 = mat1.names;
	    double (*C0)[n1] = (double (*)[n1])&C1[0];


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

		inputmat mat3 = getDoubleMatrix(3, m3, n3, B0Address, b0 );
		m3 = mat3.row;
		n3 = mat3.col;
		B0Address = mat3.Addr;
	    b0 = mat3.names;

		inputmat mat4 = getDoubleMatrix(4, m4, n4, K0Address, K1 );
		m4 = mat4.row;
		n4 = mat4.col;
		K0Address = mat4.Addr;
	    K1 = mat4.names;

	    int K0[n4+1];
	    K0[0] = 0;
	    for (i = 1; i <= n4;i++)
	    {
	    	K0[i] = (int)K1[i-1];
	    }

	    struct blockmatrix C;

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
	    double *b;
	   	b=(double *)malloc((n3+1)*sizeof(double));
	  	if (b==NULL)
	    {
	      printf("Failed to allocate storage for a!\n");
	      exit(1);
	    };
	    for (i =0;i<n3;i++)
	    {
	    	b[i+1] = b0[i];
	    }


	    /*Build the LHS of the constraints*/
	    struct constraintmatrix *constraints; /*Declare the constraintmatrix data structure*/
   	    /*Start by changing the constraints to a blockmatrix*/
	
	 	constraints=(struct constraintmatrix *)malloc((quot+1)*sizeof(struct constraintmatrix));
    	if (constraints==NULL)
	    {
	      printf("Failed to allocate storage for constraints!\n");
	      exit(1);
	    };
	    k = 0;
	    struct sparseblock *blockptr;
	    for(i = 1; i < quot+1; i++)
	    {
	    	inc = 0;
	    	
	    	constraints[i].blocks=NULL;

	    	for (j = 1; j <n4+1; j++)
	    	{
	    		/*This snippet computes the number of elements in each block*/
	    		int itemsum = 0;
	    		for ( k =((i-1)*n2)+inc; k <((i-1)*n2)+ inc+K0[j]; k ++)
	    		{
	    			itemsum = itemsum +AItemRow[k];
	    			
	    		}
	    		inc= inc+K0[j];
	    		/*End of the "number of elements" snippet*/
	    		inc = inc+K0[j];
	    		blockptr->blocknum=j;
				blockptr->blocksize=K0[j];
				blockptr->constraintnum=i;
				blockptr->next=NULL;
				blockptr->nextbyblock=NULL;
				/*The snippet below causes seg faults*/
				blockptr->entries=(double *) malloc((itemsum+1)*sizeof(double));
			   /*	if (blockptr->entries==NULL)
			    {
			        sciprint("Allocation of constraint block failed!\n");
			        return 1;
			    };*/
			    blockptr->iindices=(unsigned short *) malloc((itemsum+1)*sizeof(int));
			    /*if (blockptr->iindices==NULL)
			    {
			        printf("Allocation of constraint block failed!\n");
			        exit(1);
			    };*/
			    blockptr->jindices=(unsigned short *) malloc((itemsum+1)*sizeof(int));
			    /*if (blockptr->jindices==NULL)
			    {
			        printf("Allocation of constraint block failed!\n");
			        exit(1);
			    }*/

			   blockptr->numentries=itemsum;
			    m= 1;
			    for (l = 1; l< K0[i+1]; l++)
			    {
			    	if (AItemRow[l] != 0)
			    	{
			    		blockptr->iindices[m]=inc+m;
						blockptr->jindices[m]= AColPos[sparse_sizes[i]] - inc; 
						/*sciprint("j index = %i\n",blockptr->jindices[m]);*/
						blockptr->entries[m]=A0[sparse_sizes[i]] - inc;
						m++;
			    	}
			    	
			    }
			    

	    	}
	    	blockptr->next=constraints[i].blocks;
    		constraints[i].blocks=blockptr;
	    	
	    }

	    struct blockmatrix X,Z;
		double *y;

		write_prob("prob.dat-s",m1,quot,C,b,constraints);

		initsoln(m1,quot,C,b,constraints,&X,&y,&Z);
		double pobj,dobj;

		
		

		/*
		* A return code for the call to easy_sdp().
		*/

		int ret;
		ret=easy_sdp(7,2,C,b,constraints,0.0,&X,&y,&Z,&pobj,&dobj);

		free_prob(7,2,C,b,constraints,X,y,Z);

		/*Yet to return solutions to scilab*/


	    
	}
}