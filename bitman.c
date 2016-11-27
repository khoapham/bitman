#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "devices.h"
#include "bitman.h"

//#include <time.h>

void mygetopt(int argc, char **argv)
{
  int i,j;
  int skip;
  char debug;
	for(i=1;i<argc;i++)
		{
		if (argv[i][0] == '-' || argv[i][0] == '/')
		/* if the argument is a switch */
			{
			j=1;
			skip=false;
			while ((unsigned)j-1 < strlen(argv[i]))
				{
				debug = argv[i][j];
				switch(argv[i][j])
					{
					case '-':
					case 'h':	help();
								break;
					case 'H':	header=true;
								break;
					case 'v':	verbose++;
								break;
					case 'p':	WritePartial=true;
								parsePartial(argc,argv,i,j);
								i=i+5;
								skip=true;
								break;
					case 'P':	WritePartialByMask=true;
								i++;
								skip=true;
								PartialFileByMask=argv[i];
								break;
					case 'A':	ParseRsgOptions(argv,i,'A');
								ModufiedRSGs++;
								i=i+3;
								skip=true;
								break;
					case 'I':	ParseRsgOptions(argv,i,'I');
								ModufiedRSGs++;
								i=i+3;
								skip=true;
								break;
					case 'C':	ParseClbMask(argv,i);
								i=i+4;
								skip=true;
								break;
					case 'B':	ParseBRamMask(argv,i);
								i=i+4;
								skip=true;
								break;
					case 'b':	ParseBRamRoutingMask(argv,i);
								i=i+4;
								skip=true;
								break;
					case 'd':	DumpBitstream=true;
								i++;
								skip=true;
								DumpBitstreamFileName=argv[i];
								break;
					case 'F':	WriteFullBitstreamMode=true;
								i++;
								skip=true;
								FullBitstreamFileName=argv[i];
								break;
					case 'M':	WriteModuleBitstreamMode=true;
								i++;
								skip=true;
								ModuleBitstreamFileName=argv[i];
								break;
					case 'c':	DumpCLBs++;
//								if (DumpCLBs > 1)
//									DumpCLBs=1;
								break;
					case 'L':	ParseLinkPitFileOptions(argv,i);
								LinkPitFileMode++;
								i=i+3;
								skip=true;
								break;
					case 'S':	ParseBitManipulations(argv,i,'S');  // set a bit in stream
								BitManipulations++;
								i=i+3;
								skip=true;
								break;
					case 'D':	ParseBitManipulations(argv,i,'D');  // delete a bit in stream
								BitManipulations++;
								i=i+3;
								skip=true;
								break;
					case 'T':	ParseBitManipulations(argv,i,'T');  // clear a bit in stream
								BitManipulations++;
								i=i+3;
								skip=true;
								break;
					case 'x':	RepRsc++;
								ParseRepRsc(argv,i);
								i=i+5;
								skip=true;
								break;
					case 'm':	ParseMetaHeader(argv,i);
								i=i+1;
								skip=true;
								break;
					default :	
								break;
					}
				if(skip)		// if we had a -p or -d go to next scan
					break;
				j++;
				}
			}
		else
			{
				if (file1 == NULL)
					{
						file1=argv[i];
					}
				else if (file2 == NULL)
					{
						file2=argv[i];
					}
				else if (file3 == NULL)
					{
						file3=argv[i];
					}
				else
					{
						fprintf(stderr,"To many input files or a missing '-' use -h for help \n");
						exit(1);
					}
			}
		}	/* end of parameter parsing */
}

int parsePartial(int argc, char **argv, int i, int j)
{
  int ColTmp1, ColTmp2, RowTmp1, RowTmp2;
//  int k,l;
    sscanf( argv[i+1], "%d", &ColTmp1);
    sscanf( argv[i+2], "%d", &RowTmp1);
    sscanf( argv[i+3], "%d", &ColTmp2);
    sscanf( argv[i+4], "%d", &RowTmp2);
	// CLB(0,0) is located top left
	if(ColTmp1<=ColTmp2)
	{
		ColBoundingBox1=ColTmp1;
		ColBoundingBox2=ColTmp2;
	}
	else
	{
		ColBoundingBox1=ColTmp2;
		ColBoundingBox2=ColTmp1;
	}
	if(RowTmp1<=RowTmp2)
	{
		RowBoundingBox1=RowTmp1;
		RowBoundingBox2=RowTmp2;
	}
	else
	{
		RowBoundingBox1=RowTmp2;
		RowBoundingBox2=RowTmp1;
	}
	PartialFile=argv[i+5];
	if(verbose)
		printf("BoundingBox %i %i %i %i File %s\n",ColBoundingBox1,RowBoundingBox1,ColBoundingBox2,RowBoundingBox2,PartialFile);
	return(0);
}

void ParseRepRsc(char **argv, int i)
{
  int ColTmp1, ColTmp2, RowTmp1, RowTmp2;
	TypeOfResource=argv[i+1];
    sscanf( argv[i+2], "%d", &ColTmp1);
    sscanf( argv[i+3], "%d", &RowTmp1);
    sscanf( argv[i+4], "%d", &ColTmp2);
    sscanf( argv[i+5], "%d", &RowTmp2);

	RscCl1 = ColTmp1;
	RscRw1 = RowTmp1;
	RscCl2 = ColTmp2;
	RscRw2 = RowTmp2;
}

void ParseMetaHeader(char **argv, int i)
{
	metadata = argv[i+1];

//	printf("Meta data for header %s\n", metadata);
}

void ParseBoundingBox(char **argv, int i, int *left, int *right, int *top, int *bottom)
{
	int ColTmp1, ColTmp2, RowTmp1, RowTmp2;
	int k;
	// read in bounding box coordinates
	k=sscanf( argv[i+1], "%d", &RowTmp1);
	if(k==0)
	{
		fprintf(stderr,"Wrong bounding box specification.\n"); 
		help();
		exit(-1);
	}
    k=sscanf( argv[i+2], "%d", &ColTmp1);
	if(k==0)
	{
		fprintf(stderr,"Wrong bounding box specification.\n"); 
		help();
		exit(-1);
	}
    k=sscanf( argv[i+3], "%d", &RowTmp2);
	if(k==0)
	{
		fprintf(stderr,"Wrong bounding box specification.\n"); 
		help();
		exit(-1);
	}
    k=sscanf( argv[i+4], "%d", &ColTmp2);
	if(k==0)
	{
		fprintf(stderr,"Wrong bounding box specification.\n"); 
		help();
		exit(-1);
	}
	// sort coordinates
	if(ColTmp1<=ColTmp2)
	{
		*left =ColTmp1;
		*right=ColTmp2;
	}
	else
	{
		*left =ColTmp2;
		*right=ColTmp1;
	}
	if(RowTmp1<=RowTmp2)
	{
		*top   =RowTmp1;
		*bottom=RowTmp2;
	}
	else
	{
		*top   =RowTmp2;
		*bottom=RowTmp1;
	}
}
void ParseClbMask(char **argv, int i)
{
	ParseBoundingBox(argv, i, &ClbLeft, &ClbRight, &ClbTop, &ClbBottom);
	if(verbose>=1)
		fprintf(stderr,"CLB Mask: left: %i, right %i, top %i, bottom %i\n",ClbLeft, ClbRight, ClbTop, ClbBottom);
	ClbMaskActive=true;
}
void ParseBRamMask(char **argv, int i)
{
//	int left, right, top, bottom;
	ParseBoundingBox(argv, i, &BRamLeft, &BRamRight, &BRamTop, &BRamBottom);
	if(verbose>=1)
		fprintf(stderr,"BRAM Mask: left: %i, right %i, top %i, bottom %i\n",BRamLeft, BRamRight, BRamTop, BRamBottom);
	BRamMaskActive=true;
}
void ParseBRamRoutingMask(char **argv, int i)
{
//	int left, right, top, bottom;
	ParseBoundingBox(argv, i, &BRamRoutingLeft, &BRamRoutingRight, &BRamRoutingTop, &BRamRoutingBottom);
	if(verbose>=1)
		fprintf(stderr,"BRAM Routing Mask: left: %i, right %i, top %i, bottom %i\n",BRamRoutingLeft, BRamRoutingRight, BRamRoutingTop, BRamRoutingBottom);
	BRamRoutingMaskActive=true;
}
int CheckPartialMaskHight(void)
{
	if(BRamMaskActive && ClbMaskActive &&((ClbTop != BRamTop) || (ClbBottom != BRamBottom)))
	{
		fprintf(stderr,"Partial mask definition missmatch.\n Top or bottom CLB definition does not match the corresponding BRAM content definition.\n");
		return(true);
	}
	if(BRamRoutingMaskActive && ClbMaskActive && ((ClbTop != BRamRoutingTop) || (ClbBottom != BRamRoutingBottom)))
	{
		fprintf(stderr,"Partial mask definition missmatch.\n Top or bottom CLB definition does not match the corresponding BRAM routing definition.\n");
		return(true);
	}
	if(BRamMaskActive && BRamRoutingMaskActive && (ClbMaskActive==false) && ((BRamTop != BRamRoutingTop) || (BRamBottom != BRamRoutingBottom)))
	{
		fprintf(stderr,"Partial mask definition missmatch.\n Top or bottom BRAM content definition does not match the corresponding BRAM routing definition.\n");
		return(true);
	}
	if(BRamMaskActive && BRamRoutingMaskActive && (((BRamTop % 4) != 1) || ((BRamRoutingTop % 4) != 1))) 
	{
		fprintf(stderr,"WARNING: Partial mask definition missmatch.\n Top BRAM tile is not the beginning of a 4 tile tall BRAM.\n");
	}
	if(BRamMaskActive && BRamRoutingMaskActive && (((BRamBottom % 4) != 0) || ((BRamRoutingBottom % 4) != 0)))
	{
		fprintf(stderr,"WARNING: Partial mask definition missmatch.\n Bottom BRAM tile is not the end of a 4 tile tall BRAM.\n");
	}
	return(false);
}
void WritePartialMask()
{
  int ClbCount, BRamCount;
  int ClbColumnCount, BRamColumnCount;
  int NRowCLB;
  int leftBound;
  int rightBound;
  int topBound;
  int bottomBound;
  char tmp;
  char ModuleResourceString[200];
  FILE *PartialPtr;
  int i, j;
  NRowCLB = ((FLR*4)-(2*CLBBytesIO))/CLBBytes;		// (we compute bytes) all-top&botttom IO-cell 

  //	if((strncmp(UserInput, "sadr" ,4)==0) || (strncmp(UserInput, "SADR" ,4)==0)) // Set ReCoBus ADR

	PartialPtr=fopen(PartialFileByMask,"wb+");
	if (PartialPtr == NULL)
	{	
		fprintf(stderr,"Can not open output file for dumping partial bitstream by Mask.\n");
		exit(34);
	}
	// if we know thart there is a -P output file, test if there is also a mask specification
	if(ClbMaskActive || ParseBRamMask || BRamRoutingMaskActive) 
	{
	}
	else //no mask
	{
		fprintf(stderr,"Please specify a partial mask.\n");
		help();
		exit(-1);
	}
	// init
	for(i=0; i<200; i++) 
		ModuleResourceString[i]=0x00;
	// note for bounding box checking that the first top left CLB is CLB[1][1]
	tmp=(80)/10;//(int)(FLength*4-CLBBytesIO*2)/CLBBytes;
	if(ClbRight>NColCLB || (ClbBottom-1)>((FLength*4-CLBBytesIO*2)/CLBBytes))
		fprintf(stderr,"CLB bounding box exceeds device.\n");
	if(BRamRight>NColRAM || BRamBottom>((FLength*4-CLBBytesIO*2)/CLBBytes))
		fprintf(stderr,"BRAM bounding box exceeds device.\n");
	if(BRamRoutingRight>NColRAM || BRamRoutingBottom>((FLength*4-CLBBytesIO*2)/CLBBytes))
		fprintf(stderr,"BRAM routing bounding box exceeds device.\n");
	// generate module resource string
	ClbCount=0;
	BRamCount=0;
	ClbColumnCount=0;
	BRamColumnCount=0;
	for(i=0; i<NColCLB+NColRAM; i++)
	{
		if(ResourceString[i]=='C')
		{
			if(ClbColumnCount+1>=ClbLeft && ClbColumnCount+1<=ClbRight)
			{
				ModuleResourceString[ClbCount+BRamCount]=0xC0;
				ClbCount++;
			}
			ClbColumnCount++;
		}
		if(ResourceString[i]=='R') 
		{
			if((BRamColumnCount+1>=BRamLeft && BRamColumnCount+1<=BRamRight) || (BRamColumnCount+1>=BRamRoutingLeft && BRamColumnCount+1<=BRamRoutingRight))
			{
				ModuleResourceString[ClbCount+BRamCount] = 0xB0;
				if(BRamColumnCount+1>=BRamLeft && BRamColumnCount+1<=BRamRight)
					ModuleResourceString[ClbCount+BRamCount] = ModuleResourceString[ClbCount+BRamCount] | content;
				if(BRamColumnCount+1>=BRamRoutingLeft && BRamColumnCount+1<=BRamRoutingRight)
					ModuleResourceString[ClbCount+BRamCount] = ModuleResourceString[ClbCount+BRamCount] | routing;
				BRamCount++;
			}
			else if(ClbColumnCount>=ClbLeft && ClbColumnCount+1<=ClbRight)
			{
				ModuleResourceString[ClbCount+BRamCount] = 0xB0;		
				BRamCount++;
			}
			
			BRamColumnCount++;
		}
	}
	if(CheckPartialMaskHight())
		return;

	fprintf(PartialPtr,"#PIT#%s#%s",Family, Device);
	fprintf(PartialPtr,"#%i#%i",    Speed, Delay);
	fprintf(PartialPtr,"#%i",  (BRamRoutingTop | BRamTop | ClbTop));
	fprintf(PartialPtr,"#%i",  (BRamRoutingBottom | BRamBottom | ClbBottom));
	fprintf(PartialPtr,"#");
	for(i=0; i<ClbCount+BRamCount; i++)
	{
		tmp = ModuleResourceString[i]; 
		fwrite(&tmp, 1, 1, PartialPtr);
	}
	// start marker for configuration raw data
	fprintf(PartialPtr,"##");


	if(ClbMaskActive)
	{

		BA=0;   MJA=ClbLeft+2;   MNA=0;		// ob dass so für virtex 4&5 geht?
		leftBound=getFrameIndexGlobal();										//left

		BA=0;   MJA=ClbRight+2;   MNA=0;
		rightBound=getFrameIndexGlobal() + CLBFrames-1;							//right + width-1

		if(ClbTop==0)															//top
			topBound=0;
		else
			topBound=CLBBytesIO + CLBBytes*(ClbTop-1);

		if(ClbBottom==0)														//bottom
			bottomBound=CLBBytesIO-1;
		else if(ClbBottom==NRowCLB+1)
			bottomBound=(FLength*4)-1;
		else
			bottomBound=CLBBytesIO + (CLBBytes*ClbBottom) -1;
	
		if(verbose >= 2)
			fprintf(out_ptr,"CLB_Bytes: topBound %i bottomBound %i leftBound %i rightBound %i\n",topBound, bottomBound, leftBound, rightBound);

		for(i=leftBound; i<=rightBound; i++)		// over all frames
  			for(j=0; j<FLength*4; j++)				// over all words
			{
				if(j>=topBound && j<=bottomBound)
				{
					tmp= getBufferByte(0,i,j);			// scan for used CLB-cell
			        fwrite(&tmp, 1, 1, PartialPtr);
				}
			}
	}


	if(BRamMaskActive)
	{

		BA=1;   MJA=BRamLeft-1;   MNA=0;
		leftBound=getFrameIndexGlobal();										//left

		BA=1;   MJA=BRamRight-1;   MNA=0;
		rightBound=getFrameIndexGlobal() + RAMFrames-1;							//right + width-1

		if(BRamTop==0)															//top
			topBound=0;
		else
			topBound=CLBBytesIO + CLBBytes*(BRamTop-1);

		if(BRamBottom==0)														//bottom
			bottomBound=CLBBytesIO;
		else if(BRamBottom==NRowCLB+1)
			bottomBound=FLR*4;
		else
			bottomBound=CLBBytesIO + (CLBBytes*BRamBottom) -1;
	
		if(verbose >= 2)
			fprintf(out_ptr,"BRAM_Bytes: topBound %i bottomBound %i leftBound %i rightBound %i\n",topBound, bottomBound, leftBound, rightBound);

		for(i=leftBound; i<=rightBound; i++)		// over all frames
  			for(j=0; j<FLength*4; j++)				// over all words
			{
				if(j>=topBound && j<=bottomBound)
				{
					tmp= getBufferByte(0,i,j);			// scan for used CLB-cell
			        fwrite(&tmp, 1, 1, PartialPtr);
				}
			}
	}

	if(BRamRoutingMaskActive)
	{

		BA=2;   MJA=BRamRoutingLeft-1;   MNA=0;
		leftBound=getFrameIndexGlobal();										//left

		BA=2;   MJA=BRamRoutingRight-1;   MNA=0;
		rightBound=getFrameIndexGlobal() + RI_Frames-1;							//right + width-1

		if(BRamRoutingTop==0)															//top
			topBound=0;
		else
			topBound=CLBBytesIO + CLBBytes*(BRamRoutingTop-1);

		if(BRamRoutingBottom==0)														//bottom
			bottomBound=CLBBytesIO;
		else if(BRamRoutingBottom==NRowCLB+1)
			bottomBound=FLR*4;
		else
			bottomBound=CLBBytesIO + (CLBBytes*BRamRoutingBottom) -1;
	
		if(verbose >= 2)
			fprintf(out_ptr,"BRAM_Routing_Bytes: topBound %i bottomBound %i leftBound %i rightBound %i\n",topBound, bottomBound, leftBound, rightBound);

		for(i=leftBound; i<=rightBound; i++)		// over all frames
  			for(j=0; j<FLength*4; j++)				// over all words
			{
				if(j>=topBound && j<=bottomBound)
				{
					tmp= getBufferByte(0,i,j);			// scan for used CLB-cell
			        fwrite(&tmp, 1, 1, PartialPtr);
				}
			}
	}
	fclose(PartialPtr);
}

void help(void)
{
	fprintf(stderr,"Bitscan - a configuration bitstream analizing tool for Xilinx FPGAs\n");
	fprintf(stderr,"  Usage: bitscan options [input_bitfile_1] [input_bitfile_2] [output_file]\n");
	fprintf(stderr,"  Options:\n");
	fprintf(stderr,"   -v verbose\n");
	fprintf(stderr,"   -d outfile : dump pure bitstream data frame by frame\n");
	fprintf(stderr,"   -c : print CLB info including bitstream encoding\n");
	fprintf(stderr,"\n");
	fprintf(stderr,"   -x CLB|BRAM COL1 ROW1 COL2 ROW2 : Replication a CLB|BRAM from COL1 ROW1 COL2 ROW2\n");
	fprintf(stderr,"   -S ROW COL INIT: Set a bit at row col bit-offset position (DANGEROUS - only for experts).\n");
	fprintf(stderr,"   -T ROW COL INIT: Toggle a bit at row col bit-offset position (DANGEROUS - only for experts).\n");
	fprintf(stderr,"   -D ROW COL INIT: Delete a bit at row col bit-offset position (only for experts).\n");
	fprintf(stderr,"   -C CLB_ROW1 CLB_COL1 CLB_ROW2 CLB_COL2: Define partial bounding box for CLBs. Top left CLB is 1 1.\n");
	fprintf(stderr,"   -B RAM_ROW1 RAM_COL1 RAM_ROW2 RAM_COL2: Define partial bounding box for BRAM content. Top left BRAM is 1 1.\n");
	fprintf(stderr,"   -b RAM_ROW1 RAM_COL1 RAM_ROW2 RAM_COL2: Define partial bounding box for BRAM routing. Top left BRAM is 1 1.\n");
	fprintf(stderr,"   -P pitfile: write partial bitfile as specified by partial bounding box definitions.\n");
	fprintf(stderr,"   -L pitfile ROW COL: Link pitfile to row col position.\n");
	fprintf(stderr,"   -I ROW COL INIT: Set 16 bit LUT init value (hex) at row col position (integer).\n");
	fprintf(stderr,"      You may need multiple -a sets if multiple resource slots are used.\n");
	fprintf(stderr,"   -A ROW COL ADR: Set address (hex) at row col position (integer).\n");
	fprintf(stderr,"      You may need multiple -A sets if multiple resource slots are used.\n");
	fprintf(stderr,"      The valid address range for VirtexII is 0..E (15 different chipselects).\n");
	fprintf(stderr,"      The valid address range for Spartan3 is 0..6 (7 different chipselects).\n");
	fprintf(stderr,"\n");
	fprintf(stderr,"   -F bitfile: Write (linked) full bitstream into file.\n");
	fprintf(stderr,"   -M bitfile: Write module bitstream into file (complete CLB columns).\n");
	fprintf(stderr,"               The width is specified by partial bounding box definitions\n");
	fprintf(stderr,"               ROW values have to be set, but will be ignored.\n");
	fprintf(stderr,"               This option is not required when used together with -L.\n");
	exit(0);
}

void echoTXT(int level,char *msg)
{
	if (level==-1)
		fprintf(stderr,msg);
	if (level >= verbose)
		fprintf(stdout,msg);
}
int file_handling(void)
{
	if (file3 != NULL)					// we got 3 input files
	{
		twoInputMode=true;
		in_ptr0=fopen(file1,"rb");
		in_ptr1=fopen(file2,"rb");
		out_ptr=fopen(file3,"wb+");
		if ((in_ptr0 == NULL) || (in_ptr1 == NULL) || (out_ptr == NULL))
			{ 
				printf("Error open input or output file, use -h for help");
				exit(2);
			}
	}
	else if (file2 != NULL)					// we got 2 input files
	{                                   
		twoInputMode=true;
		in_ptr0=fopen(file1,"rb");
		in_ptr1=fopen(file2,"rb");
		out_ptr=stdout;
		if ((in_ptr0 == NULL) || (out_ptr == NULL))
			{
				printf("Error open input file, use -h for help");
				exit(2);
			}
	}
	else if (file1 != NULL)					// we got 2 input files
	{
		twoInputMode=false;
		in_ptr0=fopen(file1,"rb");
		out_ptr=stdout;
		if (in_ptr0 == NULL)
			{
				printf("Error open input file, use -h for help");
				exit(2);
			}
	}
	else									/* no file option => use stdin and stdout */
	{
		twoInputMode=false;
		in_ptr0=stdin;
		out_ptr=stdout;
	}
	return(0);
}

void initForScan(int Buffer)
{
	int i,j,k,m;
	BA=-1;
	MJA=-1; 
	MNA=-1;
	Device="unknown";
	Family="unknown";
	NFrames=-1;
	FLength=-1;
	NColCLB=-1;
	NColRAM=-1;
	FLR=-1;
	userFLR=-1;
	writeFLR=-1;
	ShadowState=false;

	for(i=0; i<10000; i++)				// over all frames
  		for(j=0; j<500; j++)			// over all words
			FrameBuffer[Buffer][i][j]=0;
	for(i=0; i<5000; i++)
		FrameBufferState[i]=-1;         // -1 stream is empty  
	for(j=0; j<500; j++)
		ShadowCol[j]=0;					// clear shadow reg...
	for(m=0; m<2; m++)
		for(i=0; i<50; i++)
			for(j=0; j<150; j++)
				for(k=0; k<50; k++)
					CLBState[m][i][j][k]=0;
	for(i=0; i<250; i++)
		for(j=0; j<250; j++)
			RSGUsage[i][j]= -1; // -1: no reconfigurable select generator usage
}

unsigned int GetNextWord(FILE *ptr, int *flag)   /* read next 32-bit word */
{
	long int NextWord=0; 
	long int NextByte=0;
	int NBytes=0;

   	*flag=true;				// reading was OK
   		NBytes=fread(&NextByte, 1, 1, ptr);
   		if (NBytes==0)
   		{
			if(verbose >= 1)
    			fprintf(out_ptr,"Reached end of input file.\n");
			*flag=false;
   			return(0);
   		}
   		NextWord = NextWord | NextByte << 24;
		NextByte=0;
   		NBytes=fread(&NextByte, 1, 1, ptr);
    	if (NBytes==0)
    	{
    		fprintf(stderr,"Warning: input file ends not with 32-bit word allignment.\n");
			*flag=false;
   			return(0);
    	}
    	NextWord = NextWord | NextByte << 16;
		NextByte=0;
    	NBytes=fread(&NextByte, 1, 1, ptr);
    	if (NBytes==0)
    	{
    		fprintf(stderr,"Warning: input file ends not with 32-bit word allignment.\n");
			*flag=false;
   			return(0);
    	}
    	NextWord = NextWord | NextByte << 8;
		NextByte=0;
    	NBytes=fread(&NextByte, 1, 1, ptr); 
    	if (NBytes==0)
    		{
    		fprintf(stderr,"Warning: input file ends not with 32-bit word allignment.\n");
			*flag=false;
   			return(0);
    		}
    	NextWord = NextWord | NextByte;
		//fprintf(stderr,"NextWord: 0x%08X.\n",NextWord);
    	return(NextWord);
}

int updateFLR(int val, int who)
{
	if(who==ByDeviceID)
		if(FLR==-1)
		{
			FLR=val;
//			if(verbose)
//				fprintf(out_ptr,"Set framelength according to the device ID to %i.\n",FLR);
			return(val);
		}
		else if(FLR==val)
		{
			if(verbose)
				fprintf(out_ptr,"Framelength acknoledged by device ID - FLR=%i.\n",FLR);
			return(val);
		}
		else
		{
			fprintf(stderr,"Warning: prior framelength does not match with the one determined by the device ID - keeping FLR at %i.\n",FLR);
			return(val);
		}
	if(who==ByWriteFLR)
		if(FLR==-1)
		{
			FLR=(val+1);	// the bitstream value is coded length-1...
			if(verbose)
				fprintf(out_ptr,"Set framelength according to the Write Frame Length command to %i.\n",FLR);
			return(val+1);
		}
		else if(FLR==val+1)
		{
			if(verbose)
				fprintf(out_ptr,"Framelength acknoledged by FLR-command - FLR is=%i.\n",FLR);
			return(val);
		}
		else
		{
			fprintf(stderr,"Warning: prior framelength does not match with the one determined by the FLR-command - keeping FLR at %i.\n",FLR);
			return(val);
		}
	fprintf(stderr,"Warning: No idea how to set the FLR.\n");
	return(0);
}




int FillBuffer(FILE *ptr, int WordCount, int BufferIndex, bool isPartial)
{
  int i;
  long int NextWord;
  int FrameCount=0;

  int FrameSkip;
  int FrameMax;

//	printf("in FillBuffer.\n");
	if(strcmp(Device,"unknown")==0)
	{
		fprintf(stderr,"No supported device is detected from the bitstream.\n");
		exit(19);
	}
	else
	{				// for a first step, we handle only VirtexII-Devices...
		while(WordCount > 0)
		{
			//printf("Debug FillBuffer %i --- FrameLength %i --- FLR %i\n",WordCount,FLength, FLR);
			for(i=0; i<FLR; i++)
			{
				WordCount--;
				NextWord=GetNextWord(ptr,&readstate);
				if(readstate==false)
				{
					fprintf(stderr,"Unexpected end of file BA: 0x%02X MJA: %02X MNA: %02X\n",BA,MJA,MNA);
					break;
				}
				ShadowCol[i]=NextWord;
				ShadowState=true;
			}
			if(WordCount>=-FLR)
					// attention we speculative assume that this column will be flushed into the FPGA
			{
				FrameCount++;
					if	(strcmp(Device,"XC6VLX240T") == 0) {
						if (FrameCount == 1)			// the first frame of the first row, clears all resource variables
						{
							BA = 0;
							TB = 0;
							MJA = 0;
							MNA = 0;
							RA = 0;

							NColNULL = 0;
							FrameSkip = 0;
							FrameMax = (TB+RA+1)*RowFrames - 48;
							NColRI = 0;
							NColBFG = 0;
							NColM=0;
							NColD=0;
						}

						if (FrameCount == (RowFrames + 1))			// the first frame of the second row, sets TB = 1 and clears others
						{
							BA = 0;
							TB = 0;
							MJA = 0;
							MNA = 0;
							RA = 1;

							NColNULL = 0;
							FrameSkip = 0;
							FrameMax = (TB+RA+1)*RowFrames - 48;
							NColRI = 0;
							NColBFG = 0;
							NColM=0;
							NColD=0;
						}

						if (FrameCount == (2*RowFrames + 1))			// the first frame of the second row, sets TB = 1 and clears others
						{
							BA = 0;
							TB = 0;
							MJA = 0;
							MNA = 0;
							RA = 2;

							NColNULL = 0;
							FrameSkip = 0;
							FrameMax = (TB+RA+1)*RowFrames - 48;
							NColRI = 0;
							NColBFG = 0;
							NColM=0;
							NColD=0;
						}

						if (FrameCount == (3*RowFrames + 1))			// the first frame of the first row, clears all resource variables
						{
							BA = 0;
							TB = 0;
							MJA = 0;
							MNA = 0;
							RA = 3;

							NColNULL = 0;
							FrameSkip = 0;
							FrameMax = (TB+RA+1)*RowFrames - 48;
							NColRI = 0;
							NColBFG = 0;
							NColM=0;
							NColD=0;
						}

						if (FrameCount == (4*RowFrames + 1))			// the first frame of the second row, sets TB = 1 and clears others
						{
							BA = 0;
							TB = 0;
							MJA = 0;
							MNA = 0;
							RA = 4;

							NColNULL = 0;
							FrameSkip = 0;
							FrameMax = (TB+RA+1)*RowFrames - 48;
							NColRI = 0;
							NColBFG = 0;
							NColM=0;
							NColD=0;
						}

						if (FrameCount == (5*RowFrames + 1))			// the first frame of the second row, sets TB = 1 and clears others
						{
							BA = 0;
							TB = 0;
							MJA = 0;
							MNA = 0;
							RA = 5;

							NColNULL = 0;
							FrameSkip = 0;
							FrameMax = (TB+RA+1)*RowFrames - 48;
							NColRI = 0;
							NColBFG = 0;
							NColM=0;
							NColD=0;
						}
					}

					if	(strcmp(Device,"XC7Z010") == 0) {
						if (FrameCount == 1)			// the first frame of the first row, clears all resource variables
						{
							BA = 0;
							TB = 0;
							MJA = 0;
							MNA = 0;
							RA = 0;

							NColNULL = 0;
							FrameSkip = 0;
							FrameMax = (TB+1)*RowFrames - 48;
							NColRI = 0;
							NColBFG = 0;
						}

						if (FrameCount == (RowFrames + 1))			// the first frame of the second row, sets TB = 1 and clears others
						{
							BA = 0;
							TB = 1;
							MJA = 0;
							MNA = 0;
							RA = 0;

							NColNULL = 0;
							FrameSkip = 0;
							FrameMax = (TB+1)*RowFrames - 48;
							NColRI = 0;
							NColBFG = 0;
						}

						if (FrameCount == 4125)			// the first row of the BRAM content
						{
							BA = 1;
							TB = 0;
							MJA = 0;
							MNA = 0;
							RA = 0;
							FrameSkip = (FrameCount - 1);
							FrameMax = FrameSkip + (TB + 1)*643;
						}

						if (FrameCount == 4767)			// the second row of the BRAM content
						{
							BA = 1;
							TB = 1;
							MJA = 0;
							MNA = 0;
							RA = 0;

							FrameSkip = (FrameCount - 1);
							FrameMax = FrameSkip + (TB + 1)*643;
						}
					}

					if	(strcmp(Device,"XC7Z020") == 0) {
						if (FrameCount == 1)			// the first frame of the first row, clears all resource variables
						{
							BA = 0;
							TB = 0;
							MJA = 0;
							MNA = 0;
							RA = 0;

							NColNULL = 0;
							FrameSkip = 0;
							FrameMax = (TB+RA+1)*RowFrames - 48;
							NColRI = 0;
							NColBFG = 0;
						}

						if (FrameCount == (RowFrames + 1))			// the first frame of the second row, sets TB = 1 and clears others
						{
							BA = 0;
							TB = 1;
							MJA = 0;
							MNA = 0;
							RA = 0;

							NColNULL = 0;
							FrameSkip = 0;
							FrameMax = (TB+RA+1)*RowFrames - 48;
							NColRI = 0;
							NColBFG = 0;
						}

						if (FrameCount == (2*RowFrames + 1))			// the first frame of the second row, sets TB = 1 and clears others
						{
							BA = 0;
							TB = 1;
							MJA = 0;
							MNA = 0;
							RA = 1;

							NColNULL = 0;
							FrameSkip = 0;
							FrameMax = (TB+RA+1)*RowFrames - 48;
							NColRI = 0;
							NColBFG = 0;
						}
					}

					if	(strcmp(Device,"KU025") == 0) {
						if (FrameCount == 1)			// the first frame of the first row, clears all resource variables
						{
							BA = 0;
							TB = 0;
							MJA = 0;
							MNA = 0;
							RA = 0;

							NColNULL = 0;
							FrameSkip = 0;
							FrameMax = (TB+1)*RowFrames - 48;
							NColRI = 0;
							NColBFG = 0;
							NColM=0;
							NColD=0;
						}

						if (FrameCount == (RowFrames + 1))
						{
							BA = 0;
							TB = 0;
							MJA = 0;
							MNA = 0;
							RA = 1;

							NColNULL = 0;
							FrameSkip = 0;
							FrameMax = (TB+RA+1)*RowFrames - 48;
							NColRI = 0;
							NColBFG = 0;
							NColM=0;
							NColD=0;
						}

						if (FrameCount == (2*RowFrames + 1))
						{
							BA = 0;
							TB = 0;
							MJA = 0;
							MNA = 0;
							RA = 2;

							NColNULL = 0;
							FrameSkip = 0;
							FrameMax = (TB+RA+1)*RowFrames - 48;
							NColRI = 0;
							NColBFG = 0;
							NColM=0;
							NColD=0;
						}
					}
					if (ResourceString[MJA + NColNULL] == 'N') {
					// The NColNULL to adjust the frame offset for hardcore CPU and ICAP port inside the FPGA fabric
//							fprintf(stderr, "FrameCount %d\n", (FrameCount - 1));
//							printf("MJA %d NColNULL %d RSC %c \n", MJA, NColNULL, ResourceString[MJA + NColNULL]);
						NColNULL++;
						FrameSkip = (FrameCount - 1);

						if	((strncmp(Device,"XC7Z",4)==0) && (RA > 0))
							FrameSkip += NULLArea;	// temporarily hardcode for the RA 1
						else
							if (NColNULL == 1)
								FrameSkip += NULLArea1;
							else if (NColNULL == 2)
								FrameSkip += NULLArea2;
							else if (NColNULL == 3)
								FrameSkip += NULLArea3;
//						printf("FrameSkip %d MaxFrame %d\n", FrameSkip, FrameMax);
					}

					for(i=0; i<FLR; i++)
					{
						FrameBuffer[BufferIndex][getFrameIndexGlobal()][i]=ShadowCol[i];
//// For debug
//						if (ShadowCol[i])
//						{
//							fprintf(stderr, "FrameCount %d\n", (FrameCount - 1));
//							printf("FrameIndex %d, i %d\n", getFrameIndexGlobal(), i);
////							printf("MJA %d NColNULL %d RSC %c \n", MJA, NColNULL, ResourceString[MJA + NColNULL]);
////							fprintf(stderr, "BA %d, TB %d, RA %d, MJA %d, MNA %d, NColRI %d, NColBFG %d, NColNULL %d\n", BA, TB, RA, MJA, MNA, NColRI, NColBFG, NColNULL);
//////							fprintf(stderr, "ShadowCol[%d] 0x%08x\n", i, ShadowCol[i]);
//						}
					}

					if ((FrameCount > FrameSkip)
						&& (FrameCount <= FrameMax))
						incFrameAddress();
			}
		}
	}
	return(0);
}

void bufferHeaderAndFooter(void)
{
  unsigned char NextByte;
  int tmp;
  long int i;
	fseek(in_ptr0,0,SEEK_SET);				// lets copy the header
//	fprintf(stderr, "EndOfHeaderFilePosition %d\n", EndOfHeaderFilePosition);
	for(i=0; i<EndOfHeaderFilePosition; i++)
	{
		fread(&NextByte, 1, 1, in_ptr0);
		InitialHeader[i] = NextByte;
	}
	InitialHeaderSize = EndOfHeaderFilePosition;

	InitialFooterSize = 0;
	fseek(in_ptr0,EndOfHeaderFilePosition+(NFrames*FLength*4),SEEK_SET);	
	while(true)								// lets copy the footer
	{
		tmp=fread(&NextByte, 1, 1, in_ptr0);
		if(tmp==0)
			break;
		InitialFooter[InitialFooterSize] = NextByte;
		InitialFooterSize++;
	}
}
int WriteFullBitstream(char *FullBitstreamFileName)
{
  unsigned char NextByte;
  int tmp, i, j;
  FILE *OutBitfilePtr;

	OutBitfilePtr=fopen(FullBitstreamFileName,"wb+");
	if(OutBitfilePtr == NULL)
	{
		fprintf(stderr,"Unable to open output bitstream file %s.\n",FullBitstreamFileName);
		return(-1);
	}

	// Write the metadata on the bitstream header
	if (strcmp(metadata,"") == 0) {
		for(i=0; i<InitialHeaderSize; i++)
		{
			NextByte = InitialHeader[i];
			fwrite(&NextByte, 1, 1, OutBitfilePtr);
		}
	}
	else {
		for (i=0; i<(SyncSequenceFilePosition-52); i++)
		{
			NextByte = InitialHeader[i];
			fwrite(&NextByte, 1, 1, OutBitfilePtr);
		}

		tmp = strlen(metadata);
		// append the metadata to the header
		for (i=0; i<tmp; i++)
		{
			NextByte = metadata[i];
			fwrite(&NextByte, 1, 1, OutBitfilePtr);
		}

		for (i=(SyncSequenceFilePosition-52); i<InitialHeaderSize; i++)
		{
			NextByte = InitialHeader[i];
			fwrite(&NextByte, 1, 1, OutBitfilePtr);
		}
	}

	for(i=0; i<NFrames ; i++)			// write out the pure bitstream data
		for(j=0; j<(FLength*4); j++)
		{
			NextByte = (unsigned char)getBufferByte(0, i, j);
			fwrite(&NextByte, 1, 1, OutBitfilePtr);
		}

	for(i=0; i<InitialFooterSize; i++)
	{
		NextByte = InitialFooter[i];
		fwrite(&NextByte, 1, 1, OutBitfilePtr);
	}
}
int WriteModuleBitstream(char *ModuleBitstreamFileName)
{
  char NextByte;
  int tmp, i;
  FILE *OutBitfilePtr;
  int test[]={42,23,33};
  tmp=0;

	for(i=0; i<MaxFrames; i++)				// check for frames to write
		tmp = tmp | DirtyFrameMask[i];
//	if(tmp==0)
//	{
		//fprintf(stderr,"No frames to write into partial bitstream %s - skipping action\n", ModuleBitstreamFileName);
		//return(-1);
//	}

	OutBitfilePtr=fopen(ModuleBitstreamFileName,"wb+");
	if(OutBitfilePtr == NULL)
	{
		fprintf(stderr,"Unable to open output bitstream file %s.\n",ModuleBitstreamFileName);
		return(-1);
	}

	//for( int i=0; i<SyncSequenceFilePosition; i++)		// copy IMPACT header
	//{
	//	NextByte = InitialHeader[i];
	//	fwrite(&NextByte, 1, 1, OutBitfilePtr);
	//}

	//WriteModuleBitstreamHeader(OutBitfilePtr);			// write frame length, device

	// Write the metadata on the bitstream header
	if (strcmp(metadata,"") == 0) {
		for(i=0; i<InitialHeaderSize; i++)
		{
			NextByte = InitialHeader[i];
			fwrite(&NextByte, 1, 1, OutBitfilePtr);
		}
	}
	else {
		for (i=0; i<(SyncSequenceFilePosition-52); i++)
		{
			NextByte = InitialHeader[i];
			fwrite(&NextByte, 1, 1, OutBitfilePtr);
		}

		tmp = strlen(metadata);
		// append the metadata to the header
		for (i=0; i<tmp; i++)
		{
			NextByte = metadata[i];
			fwrite(&NextByte, 1, 1, OutBitfilePtr);
		}

		for (i=(SyncSequenceFilePosition-52); i<InitialHeaderSize; i++)
		{
			NextByte = InitialHeader[i];
			fwrite(&NextByte, 1, 1, OutBitfilePtr);
		}
	}

	if(ClbRight > 0)
	{
		if(verbose)
		{
			fprintf(stderr,"Writing CLB columns of partial module bitstream from col %i to col %i\n",ClbLeft,ClbRight);
		}
		if(ClbRight > NColCLB)
		{
			fprintf(stderr,"Partial CLB bounding box definition exeeds device when generating partial bitfile\n");
			return(-1);
		}
		WriteModuleBitstreamData(OutBitfilePtr, 0x00, 2+ClbLeft, ClbRight-ClbLeft+1, CLBFrames);
	}
//	fclose(OutBitfilePtr);  // for debug only
	if(BRamRight > 0)
	{
		if(BRamRight > NColRAM)
		{
			fprintf(stderr,"Partial RAM bounding box definition exeeds device when generating partial bitfile\n");
			return(-1);
		}
		WriteModuleBitstreamData(OutBitfilePtr, 0x01, BRamLeft-1, BRamRight-BRamLeft+1, RAMFrames);
	}
	if(BRamRoutingRight > 0)
	{
		if(BRamRoutingRight > NColRAM)
		{
			fprintf(stderr,"Partial RAM bounding box definition exeeds device when generating partial bitfile\n");
			return(-1);
		}
		WriteModuleBitstreamData(OutBitfilePtr, 0x02, BRamRoutingLeft-1, BRamRoutingRight-BRamRoutingLeft+1, RI_Frames);
	}
	
	WriteModuleBitstreamFooter(OutBitfilePtr);
	fclose(OutBitfilePtr);

	return 1;
}
int WriteModuleBitstreamHeader(FILE *OutBitfilePtr)
{
  unsigned char PartialBitfileHeader[] = {	0x30, 0x00, 0x80, 0x01, \
											0x00, 0x00, 0x00, 0x07, \
											0x30, 0x01, 0x60, 0x01	};	 // attach here the frame length
	unsigned int HeaderSize;
	unsigned int tmp;
	char NextByte;
	int i;

	HeaderSize = sizeof(PartialBitfileHeader) / sizeof(unsigned char);
	for(i=0; i<HeaderSize; i++)
	{
		NextByte=PartialBitfileHeader[i];
		tmp=fwrite(&NextByte, 1, 1, OutBitfilePtr);
	}
	for(i=3; i>=0; i--)									// frame length
	{
		NextByte = (char)(((FLength - 1) >> (8*i)) & 0x000000FF);
		tmp=fwrite(&NextByte, 1, 1, OutBitfilePtr);
	}

	NextByte = 0x30;   fwrite(&NextByte, 1, 1, OutBitfilePtr);	// write Device ID Register
	NextByte = 0x01;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	NextByte = 0xC0;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	NextByte = 0x01;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	if(DeviceID==0)
	{
		fprintf(stderr,"Could not determine device ID when generating output bitfile\n");
		return(-1);
	}
	for(i=3; i>=0; i--)									// DeviceID
	{
		NextByte = (char)((DeviceID >> (8*i)) & 0x000000FF);
		fwrite(&NextByte, 1, 1, OutBitfilePtr);
	}
	NextByte = 0x30;   fwrite(&NextByte, 1, 1, OutBitfilePtr);	// WCFG write configuration register
	NextByte = 0x00;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	NextByte = 0x80;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	NextByte = 0x01;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	NextByte = 0x00;   fwrite(&NextByte, 1, 1, OutBitfilePtr);	
	NextByte = 0x00;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	NextByte = 0x00;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	NextByte = 0x01;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	return(0);
}
int WriteModuleBitstreamCommand(FILE *OutBitfilePtr)
{
	// write configuration
  unsigned char PartialBitfileHeader[] = {	0x30, 0x00, 0x80, 0x01, \
											0x00, 0x00, 0x00, 0x01, \
											0x30, 0x01, 0x60, 0x01	};		// attach here the frame length
	unsigned int HeaderSize;
	unsigned int tmp;
	char NextByte;
	int i;

	HeaderSize = sizeof(PartialBitfileHeader) / sizeof(unsigned char);
	for(i=0; i<HeaderSize; i++)
	{
		NextByte=PartialBitfileHeader[i];
		tmp=fwrite(&NextByte, 1, 1, OutBitfilePtr);
	}
	for(i=3; i>=0; i--)									// frame length
	{
		NextByte = (char)(((FLength - 1) >> (8*i)) & 0x000000FF);
		tmp=fwrite(&NextByte, 1, 1, OutBitfilePtr);
	}

	return(0);
}

int WriteModuleBitstreamFooter(FILE *OutBitfilePtr)
{
  char PartialBitfileFooter[] = {	0x30, 0x00, 0x80, 0x01, \
									0x00, 0x00, 0x00, 0x0D,	\
									0x20, 0x00, 0x00, 0x00, \
									0x20, 0x00, 0x00, 0x00, \
									0x20, 0x00, 0x00, 0x00, \
									0x20, 0x00, 0x00, 0x00, \
									0x20, 0x00, 0x00, 0x00, \
									0x20, 0x00, 0x00, 0x00, };	 // send Desync and some Nops to flush config FSM
	unsigned int FooterSize;
	char NextByte;
	int i;

	FooterSize = sizeof(PartialBitfileFooter) / sizeof(char);
	for(i=0; i<FooterSize; i++)
	{
		NextByte=PartialBitfileFooter[i];
		fwrite(&NextByte, 1, 1, OutBitfilePtr);
	}
	return(0);
}
int WriteModuleBitstreamData(FILE *OutBitfilePtr, int BA, int Offset, int Blocks, int FramesPerBlock)
{
	unsigned int FrameAdressComposition; 
	unsigned int NumberOfFrames; 
	int FrameIndex;
	char NextByte;
	int tmp=0;
	int i;
	int BlockCount, FrameCount, ByteCount;

	NextByte = 0x30;   fwrite(&NextByte, 1, 1, OutBitfilePtr);	// write FAR frame address register
	NextByte = 0x00;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	NextByte = 0x20;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	NextByte = 0x01;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	FrameAdressComposition = (BA & 0x3) << 25;
	FrameAdressComposition = FrameAdressComposition | ((Offset & 0xFF) << 17);
	for(i=3; i>=0; i--)										// BA MJA MNA address
	{
		NextByte = (char)((FrameAdressComposition >> (8*i)) & 0x000000FF);
		fwrite(&NextByte, 1, 1, OutBitfilePtr);
	}
	
	if(strncmp(Device,"XC3S",4)==0)
	{
		NextByte = 0x30;   fwrite(&NextByte, 1, 1, OutBitfilePtr);	// write SNOWPLOW Register
		NextByte = 0x01;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
		NextByte = 0xE0;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
		NextByte = 0x01;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
		for(i=3; i>=0; i--)										// number of CLB-columns to delete
		{
			NextByte = (char)((Blocks-1 >> (8*i)) & 0x000000FF);
			fwrite(&NextByte, 1, 1, OutBitfilePtr);
		}
		for(i=16; i>=0; i--)									// flush FPGA FSM after Snowplow
		{
		NextByte = 0x20;   fwrite(&NextByte, 1, 1, OutBitfilePtr);	// NOP
		NextByte = 0x00;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
		NextByte = 0x00;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
		NextByte = 0x00;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
		}
	}

	NextByte = 0x30;   fwrite(&NextByte, 1, 1, OutBitfilePtr);	// write 0 words to FDRI register as a command
	NextByte = 0x00;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	NextByte = 0x40;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	NextByte = 0x00;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	NumberOfFrames = 0x50000000;								// packet 2 type write operation
	NumberOfFrames = NumberOfFrames | ((Blocks * FramesPerBlock * FLength) + FLength);
																// works without + FLength when using the MFWR command...
	for(i=3; i>=0; i--)										// # frames to write
	{
		NextByte = (char)((NumberOfFrames >> (8*i)) & 0x000000FF);
		fwrite(&NextByte, 1, 1, OutBitfilePtr);
	}
																// write the entire data
	for(BlockCount=0; BlockCount < Blocks; BlockCount++)
		for(FrameCount=0; FrameCount < FramesPerBlock; FrameCount++)
		{
			FrameIndex = getFrameIndex(BA, Offset+BlockCount, FrameCount, 0);	// temporarily set TB=0
			for(ByteCount=0; ByteCount < FLength*4; ByteCount++)
			{
				//NextByte=FrameBuffer[0][FrameIndex][ByteCount];
				NextByte=getBufferByte(0, FrameIndex, ByteCount);
				fwrite(&NextByte, 1, 1, OutBitfilePtr);
				tmp++;
			}
		}
	for(ByteCount=0; ByteCount < FLength*4; ByteCount++)	// flush FDR register
	{
		NextByte=0x00;
		fwrite(&NextByte, 1, 1, OutBitfilePtr);
	}
	
	NextByte = 0x00;   fwrite(&NextByte, 1, 1, OutBitfilePtr);	// write auto-CRC
	NextByte = 0x00;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	NextByte = 0xDE;   fwrite(&NextByte, 1, 1, OutBitfilePtr);
	NextByte = 0xFC;   fwrite(&NextByte, 1, 1, OutBitfilePtr);

	return(0);
}
void scan(FILE *in_ptr, int BufferIndex)
{
  int i,j,k;
  int NextByte;
  int NextWord,tst;
  int NBytes;
  int HeaderSize=0;
  int foundSync=0;
  int RegisterADR=-1;
  int WordCount=0;
  int AllowInterpret;
  unsigned char R1=0,R2=0,R3=0,R4=0;

  bool rm_cmd = false;
  int mask = 0x00000000;

  bool isPartial = false;
  bool isType2 = false;

	while(true)   /* search for sync-sequence */
	{
    	NBytes=fread(&NextByte, 1, 1, in_ptr);

    	if (NBytes==0)
    	{
    		fprintf(stderr,"Sorry - found no Sync inside the bitstream.\n");
    		break;
    	}

				/* scan for sync-word */ 
    	R4=R3;
    	R3=R2;
    	R2=R1;
    	R1=NextByte;
    	if(R4==0xAA && R3==0x99 && R2==0x55 && R1==0x66)
    	{
    		foundSync=1;
			if(SyncSequenceFilePosition==0)
				SyncSequenceFilePosition = ftell(in_ptr);

    		break;
		}
	}
	if (foundSync)
	while(true)   /* parse commands */
	{
		AllowInterpret=true;
		NextWord=GetNextWord(in_ptr,&readstate);
		if(readstate == false)
			break;													// the end of the story (bitstream)

		if((NextWord & 0xE0000000) == 0x20000000)  					// determine Packet Type
		{
			isType2 = false;

			if(verbose>=1)
				fprintf(stderr,"Type 1 packet: \n");
			if((NextWord  & 0x10000000) > 1)						// then Read or Write mode
			{
				if(verbose>=1)
					fprintf(stderr,"write ");
			}
			else if((NextWord  & 0x8000000) > 1)
			{
				if(verbose>=1)
					fprintf(stderr,"read ");
			}
			else if(NextWord == 0x20000000)
			{
				if(verbose>=2)
					fprintf(out_ptr,"Found one NOP\n");
			}
			else 
			{
				fprintf(stderr,"Got confused when analyzing Type 1 packet header 0x%08X \n",NextWord);
				exit(3);
			}														// get package details
			RegisterADR = (NextWord & 0x07FFE000) >> 13;			
			WordCount   = (NextWord & 0x7FF);
		}
		else if((NextWord & 0xE0000000) == 0x40000000)				// determine Packet Type
		{
			isType2 = true;

			if(verbose>=1)
				fprintf(stderr,"Type 2 packet: \n");
			if((NextWord  & 0x10000000) > 1)						// then Read or Write mode
			{
				if(verbose>=1)
					fprintf(stderr,"write ");
			}
			else if((NextWord  & 0x8000000) > 1)
			{
				if(verbose>=1)
					fprintf(stderr,"read ");
			}
			else if(NextWord == 0x20000000)
			{
				if(verbose>=2)
					fprintf(stderr,"Found one NOP\n");
			}
			else 
			{
				fprintf(stderr,"Got confused when analyzing Type 2 packet header\n");
				exit(3);
			}														// get package details
			WordCount   = (NextWord & 0x7FFFFFF);
		}
		else
		{	
			if(verbose>=1)
				fprintf(stderr,"Got confused when analyzing packet header of 0x%08X (could be the CRC) \n",NextWord);
			AllowInterpret=false;
		}														
//		fprintf(stderr,"Register Addr 0x%08X \n",RegisterADR);
		if(AllowInterpret) 
		switch(RegisterADR)
		{
			case 0x00:	if(NextWord != 0x20000000)
						{
							if(verbose>=1)
								fprintf(stderr,"CRC register -> ");
							while(WordCount > 0)
							{
								WordCount--;
								NextWord=GetNextWord(in_ptr,&readstate);

								if(readstate == false)
								{
									fprintf(stderr,"Unexpected end of file\n");
									exit(4);
								}
								else
									if(verbose>=1)
										fprintf(stderr,"0x%08X ",NextWord);
							}
							fprintf(stderr,"\n");
						}
						break;
			case 0x01:	if(verbose>=1)
							fprintf(stderr,"FAR frame address register -> ");
						while(WordCount > 0)
						{
							WordCount--;
							NextWord=GetNextWord(in_ptr,&readstate);

							if(readstate == false)
							{
								fprintf(stderr,"Unexpected end of file\n");
								exit(4);
							}
							else
							{
//								fprintf(stderr, "NextWord 0x%08X\n ", NextWord);
								BA  = (NextWord & 0x03800000) >> 23; // block type
								TB  = (NextWord & 0x00400000) >> 22; // top/bottom bit
								RA  = (NextWord & 0x003E0000) >> 17; // row address
								MJA = (NextWord & 0x00007F80) >> 7; // column address
								MNA = (NextWord & 0x0000007F);  // minor address
//								fprintf(stderr," BA: 0x%02X TB: 0x%02X RA: %02X MJA: 0x%02X MNA: 0x%02X \n",BA,TB,RA,MJA,MNA);
							}
						}
						break;
			case 0x02:	if(verbose>=1)
							fprintf(stderr,"FDRI register -> write %i words of configuration data \n",WordCount);

						if ((isType2 == false) && (WordCount > 0))
							isPartial = true;
						else
							isPartial = false;

//						printf("Is bitstream partial? %d\n", isPartial);
						EndOfHeaderFilePosition = ftell(in_ptr);

						FillBuffer(in_ptr, WordCount, BufferIndex, isPartial);	// read Configuration data into local array
						break;
			case 0x03:	if(verbose>=1)
							fprintf(stderr,"FDRO register -> readback configuration data \n");
						while(WordCount > 0)
						{
							WordCount--;
							NextWord=GetNextWord(in_ptr,&readstate);

							if(readstate == false)
							{
								fprintf(stderr,"Unexpected end of file\n");
								exit(4);
							}
							else
								if(verbose>=1)
									fprintf(stderr,"0x%08X ",NextWord);
							if(verbose>=1)
								fprintf(stderr,"\n");
						}
						break;
			case 0x04:	if(verbose>=1)
							fprintf(stderr,"command register -> ");
						while(WordCount > 0)
						{
							WordCount--;
							NextWord=GetNextWord(in_ptr,&readstate);

							if(readstate == false)
							{
								fprintf(stderr,"Unexpected end of file\n");
								exit(4);
							}
							if(verbose>=1)
							switch(NextWord)
							{
								case 0x0:	fprintf(stderr,"NULL command, does nothing\n");
											break;
								case 0x1:	fprintf(stderr,"WCFG Write Configuration Data\n");
											break;
								case 0x2:	fprintf(stderr,"MFWR perform multiple frame write\n");
											break;
								case 0x3:	fprintf(stderr,"LFRM Last Frame activate interconnect\n");
											break;
								case 0x4:	fprintf(stderr,"RCFG Read Configuration Data\n");
											break;
								case 0x5:	fprintf(stderr,"START begin Startup sequence\n");
											break;
								case 0x6:	fprintf(stderr,"RCAP Reset Capture\n");
											break;
								case 0x7:	fprintf(stderr,"RCRC Reset CRC\n");
											break;
								case 0x8:	fprintf(stderr,"AGHIGH tristate all interconnect\n");
											break;
								case 0x9:	fprintf(stderr,"SWITCH Config  CLK frequency\n");
											break;
								case 0xA:	fprintf(stderr,"GRESTORE copy bitstream state to flip-flops\n");
											break;
								case 0xB:	fprintf(stderr,"SHUTDOWN begin shutdown sequence\n");
											break;
								case 0xC:	fprintf(stderr,"GCUPTURE copy flip-flop state to bitstream\n");
											break;
								case 0xD:	fprintf(stderr,"DESYNC reset alignment\n");
											break;
								default :	fprintf(stderr,"found unknown command! Trying to continue...\n");
							}
						}
						break;
			case 0x05:	if(verbose>=1)
							fprintf(stderr,"Control register 0 -> ");
						while(WordCount > 0)
						{
							WordCount--;
							NextWord=GetNextWord(in_ptr,&readstate);

							if(readstate == false)
							{
								fprintf(stderr,"Unexpected end of file\n");
								exit(4);
							}
							else
								if(verbose>=1)
									fprintf(stderr,"0x%08X ",NextWord);
							fprintf(stderr,"\n");
						}
						break;
			case 0x06:	if(verbose>=1)
							fprintf(stderr,"Mask register for the control register \n");
						while(WordCount > 0)
						{
							WordCount--;
							NextWord=GetNextWord(in_ptr,&readstate);

							if(readstate == false)
							{
								fprintf(stderr,"Unexpected end of file\n");
								exit(4);
							}
							else
								if(verbose>=1)
									fprintf(out_ptr,"0x%08X ",NextWord);
						if(verbose>=1)
							fprintf(stderr,"\n");
						}
						break;
			case 0x07:	if(verbose>=1)
							fprintf(stderr,"STAT register - the status register ");
						while(WordCount > 0)
						{
							WordCount--;
							NextWord=GetNextWord(in_ptr,&readstate);

							if(readstate == false)
							{
								fprintf(stderr,"Unexpected end of file\n");
								exit(4);
							}
							else
								if(verbose>=1)
									fprintf(stderr,"0x%08X ",NextWord);
							if(verbose>=1)
								fprintf(stderr,"\n");
						}
						break;
			case 0x08:	if(verbose>=1)
							fprintf(stderr,"LOUT register - no further support.. \n");
						while(WordCount > 0)
						{
							WordCount--;
							NextWord=GetNextWord(in_ptr,&readstate);

							if(readstate == false)
							{
								fprintf(stderr,"Unexpected end of file\n");
								exit(4);
							}
							else
								if(verbose>=1)
									fprintf(stderr,"0x%08X ",NextWord);
							if(verbose>=1)
								fprintf(stderr,"\n");
						}
						break;
			case 0x09:	if(verbose>=1)
							fprintf(stderr,"COR register - the configuration option register ");
						while(WordCount > 0)
						{
							WordCount--;
							NextWord=GetNextWord(in_ptr,&readstate);

							if(readstate == false)
							{
								fprintf(stderr,"Unexpected end of file\n");
								exit(4);
							}
							else
								if(verbose>=1)
									fprintf(stderr,"0x%08X ",NextWord);
							if(verbose>=1)
								fprintf(stderr,"\n");
						}
						break;
			case 0x0A:	if(verbose>=1)
							fprintf(stderr,"MFWR register - multiple frame write ");
						while(WordCount > 0)
						{
							WordCount--;
							NextWord=GetNextWord(in_ptr,&readstate);

							if(readstate == false)
							{
								fprintf(stderr,"Unexpected end of file\n");
								exit(4);
							}
							else
							{
								if(verbose>=1)
									fprintf(stderr,"0x%08X ",NextWord);
								if(ShadowState==false)
									fprintf(stderr,"Warning: got multiple frame write command prior to writing a config frame to the device.\n");
								else if(FLR==-1)
									fprintf(stderr,"Warning: device ID or frame length register has to be written prior to a multiple frame write command.\n"); 
								else
									for(i=0; i<FLR; i++)
										FrameBuffer[BufferIndex][getFrameIndexGlobal()][i]=ShadowCol[i];
							}
						}
						if(verbose>=1)
							fprintf(stderr,"\n");
						break;
			case 0x0B:	fprintf(stderr,"CBC register - no further support.. \n");
						while(WordCount > 0)
						{
							WordCount--;
							NextWord=GetNextWord(in_ptr,&readstate);

							if(readstate == false)
							{
								fprintf(stderr,"Unexpected end of file\n");
								exit(4);
							}
							else
								if(verbose>=1)
									fprintf(stderr,"0x%08X ",NextWord);
							if(verbose>=1)
								fprintf(stderr,"\n");
						}
						break;
			case 0x0C:	if(verbose>=1)
							fprintf(stderr,"IDCODE register -> ");
						while(WordCount > 0)
						{
							WordCount--;
							NextWord=GetNextWord(in_ptr,&readstate);

							if(readstate == false)
							{
								fprintf(stderr,"Unexpected end of file\n");
								exit(4);
							}
							else
							{
								int returnFromSetGlobalDeviceParameters;
								DeviceID = NextWord;
								getDeviceInformation(NextWord); //get the device
								returnFromSetGlobalDeviceParameters = SetGlobalDeviceParameters(Device);
								if (returnFromSetGlobalDeviceParameters < 0) {
									fprintf(stderr,"Device %s is not supported\n", Device);
									Device="unknown";
								}
								updateFLR(FLength,ByDeviceID);
//								fprintf(stderr,"Device ID: %s; #Frames: %i; frame length %i; #CLB columns: %i; #RAM columns: %i; #RowFrames: %i; NullArea1: %i; NullArea2: %i\n",Device,NFrames,FLR,NColCLB,NColRAM,RowFrames,NULLArea1,NULLArea2);
							}
						}
						break;
			case 0x0F:	if(verbose>=1)
							fprintf(stderr,"SNOWPLOW register -> ");
						while(WordCount > 0)
						{
							WordCount--;
							NextWord=GetNextWord(in_ptr,&readstate);

							if(readstate == false)
							{
								fprintf(stderr,"Unexpected end of file\n");
								exit(4);
							}
							else
								if(verbose>=1)
									fprintf(stderr,"0x%08X ",NextWord);
							if(verbose>=1)
								fprintf(stderr,"\n");
						}
			default :	if(verbose>=1)
							fprintf(stderr,"NextWord: %08X \n",NextWord);
		}
	}
}

void echoCLBs(void)
{
  int i,j,k,l;
  int m;
  int ColumnState[100];
  int NRowCLB;
  int rowIndex;
  int FrameIndex;
  int WordIndex;
  int ByteIndex;
  int CLB_Index;
  int MinorIndex;
  int RowIndex;
  int TB_Index;
  unsigned tmp;

  int HalfRow;
  int HalfRow_Bytes;
  int FLR_Bytes;

  int MinorMax;
  int tmpNColNULL=0;

	FLR_Bytes = (FLR - HBytes)*4;
	NRowCLB = FLR_Bytes/CLBBytes;		// (we compute bytes) all-top&botttom IO-cell 
	HalfRow = NRowCLB/2;
	HalfRow_Bytes = HalfRow*CLBBytes;

//	fprintf(out_ptr,"FLR %i\n",FLR);
//	fprintf(out_ptr,"# Rows %i\n",NRowCLB);
//	printf("# NRowRsc %d # CLBFrames %d\n", NRowRsc, CLBFrames);

	for(TB_Index=0; TB_Index<NRowRsc; TB_Index++)
	{
		for(CLB_Index=0; CLB_Index<NColCLB; CLB_Index++)	// over all CLB columns
		{
			for(i=0; i<100; i++)							// reset counter for frames conting at least one '1'
				ColumnState[i]=0;							// we count over all minor Frames of a CLB-column

			tmpNColNULL=0;
			for (tmp=0; tmp<=CLB_Index; tmp++)
				if (ResourceString[tmp + tmpNColNULL] == 'N')
					tmpNColNULL++;
			if ((ResourceString[CLB_Index + tmpNColNULL] == 'C')
				|| (ResourceString[CLB_Index + tmpNColNULL] == 'L'))
				MinorMax = CLBFrames;
			else if (ResourceString[CLB_Index + tmpNColNULL] == 'R')
				MinorMax = RI_Frames;
			else if (ResourceString[CLB_Index + tmpNColNULL] == 'B')
				MinorMax = GFrames;
			else if (ResourceString[CLB_Index + tmpNColNULL] == 'M')
				MinorMax = MFrames;
			else if (ResourceString[CLB_Index + tmpNColNULL] == 'D')
				MinorMax = DFrames;
			else
				MinorMax = 0;

			for(MinorIndex=0; MinorIndex<MinorMax; MinorIndex++)
			{												// over all minor frames
				for(ByteIndex=0; ByteIndex<FLR_Bytes; ByteIndex++)
				{											// aver all Bytes of a frame including IO cells
					j = (ByteIndex-CLBBytesIO)/CLBBytes;
					if (ByteIndex<FLR_Bytes)
					{										// after the 12 Byte of the top IO-cell
						FrameIndex=getFrameIndex(0,CLB_Index,MinorIndex,TB_Index);

						if (ByteIndex <= HalfRow_Bytes)
							tmp=getBufferByte(0,FrameIndex,ByteIndex);	// CLB-cell init
						else
							tmp=getBufferByte(0,FrameIndex,ByteIndex + 4*HBytes);

						if(tmp != 0)						// evaluate preceeding cell
						{
							ColumnState[j]++;
							CLBState[TB_Index][CLB_Index][j][MinorIndex]=1;
						}
					}
					else									// we should not reach this path
					{
						FrameIndex=getFrameIndex(0,CLB_Index,MinorIndex,TB_Index);
						fprintf(stderr,"Error while scanning frame %i\n",FrameIndex);
					}
				}
			}

		}
	}
	for(i=0; i<NColCLB; i++)
	{
		///////////////////// emit CLB-information
		for(m=0; m<NRowRsc; m++)
		{
			for(j=0; j<NRowCLB; j++)			// j=1: we omit the IOB-cell
			{
				tmp=0;
				for(k=0; k<50; k++)
					if(CLBState[m][i][j][k]==1)	// test first if at least one frame was used
						tmp=1;
				if(tmp)
				{
					if(strncmp(Device,"XC7Z",4)==0)
						printf("CLB col %02i row %02i\n",(i),j+NRowCLB*(NRowRsc-1-m));
					else
						printf("CLB col %02i row %02i\n",(i),j+NRowCLB*m);
					for(k=0; k<=MinorMax; k++)
						if(CLBState[m][i][j][k]==1)
						{
							int FIndex, BIndex;

							FIndex = getFrameIndex(0, i, k, m);
//							printf("FrameIndex %d at column %d, row %d, minor %d, resource row %d, CLBState %d\n",FIndex,i,j,k,m, CLBState[m][i][j][k]);
							BIndex = CLBBytesIO+ CLBBytes*j;
							fprintf(out_ptr,"Frame %02i : ",k);
							for(l=0; l<CLBBytes; l++)	// report all bytes of the according CLB frame
							{
								if (j < HalfRow)
									fprintf(out_ptr," %02X",getBufferByte(0,FIndex, BIndex + l));
								else
									fprintf(out_ptr," %02X",getBufferByte(0,FIndex, BIndex + l + 4*HBytes));
							}
							fprintf(out_ptr,"\n");
						}
				}
			}
		}
	}
//	if(DumpCLBs>2)
//		echoRAMs('R');			// emit BRAM routing
//	if(DumpCLBs>3)
		echoRAMs('C');			// emit BRAM content
	
}
void echoOffsetCLBs(void)
{
	int i, j, k;
	for(i=1; i<=NColCLB; i++)										// over all CLBs
  		for(j=1; j<=((FLength*4)-(2*CLBBytesIO))/CLBBytes; j++)		// over all ROWs
			for(k=0; k<CLBFrames*CLBBytes*8; k++)					// over all bits
				if(TestBit(0,i,j,k)==1)
					fprintf(stdout," TestBit: col %i row %i offset %i\n",i,j,k);
}
int echoRAMs(char mode)
{
  int BA, MJA, MNA, TB;
  int FrameIndex;
  int tmp;
  int CellState;
  int FrameState;
  int FrameIterations;
  int i, j, k, l;
	if(mode=='R')			////////////////////// emit BRAM Routing
	{
		FrameIterations = RI_Frames;
		BA = 2;
	}
	else if(mode=='C')	    ////////////////////// emit BRAM Content
	{
		FrameIterations = RAMFrames;
		BA = 1;
	}
	else
		return(-1);
//	printf("printing BRAM content\n");
	for (TB = 0; TB < NRowRsc; TB++)
	{
		for(i=0; i<NColRAM; i++)											// RAM column index
		{
			for(j=CLBBytesIO; j<((FLength*4)-CLBBytesIO); j=j+CLBBytes)		// over all CLB rows
			{
				CellState = 0;
				for(k=0; k<FrameIterations; k++)					// and over all Frames
					for(l=0; l<CLBBytes; l++)						// over all Bytes witin a row of a frame
					{
						MJA = i;
						MNA = k;
						FrameIndex = getFrameIndex(BA, MJA, MNA, TB);
						// figure out of any frame within one cell is used (according column state will be not 0)
						if (j <=  FLength*2)
							CellState = CellState | getBufferByte(0,FrameIndex,j+l);
						else
							CellState = CellState | getBufferByte(0,FrameIndex,j+l+4*HBytes);
					}
				if(CellState != 0)		// cell contains data
				{
					if (TB == 0)
						fprintf(out_ptr,"RAM-%c col %02i row %02i\n", mode, i, ((j-CLBBytesIO)/CLBBytes)+50);	//50 ???
					else
						fprintf(out_ptr,"RAM-%c col %02i row %02i\n", mode, i, ((j-CLBBytesIO)/CLBBytes));
					for(k=0; k<FrameIterations; k++)					// and over all Frames
					{
						FrameState = 0;
						for(l=0; l<CLBBytes; l++)						// over all Bytes witin a row of a frame
						{
							MJA = i;
							MNA = k;
							FrameIndex = getFrameIndex(BA, MJA, MNA, TB);

							// figure out of any frame within one cell is used (according column state will be not 0)
							if (j <=  FLength*2)
								FrameState = FrameState | getBufferByte(0,FrameIndex,j+l);
							else
								FrameState = FrameState | getBufferByte(0,FrameIndex,j+l+4*HBytes);
						}
						if(FrameState!=0)	// frame contains data
						{
//							printf("BA %d, MJA %d, MNA %d, TB %d\n", BA, MJA, MNA, TB);
//							fprintf(stderr, "FrameIndex %d\n", FrameIndex);
							fprintf(out_ptr,"RAM-%c Frame%02i :", mode, k);
							for(l=0; l<CLBBytes; l++)						// over all Bytes witin a row of a frame
							{
								FrameIndex = getFrameIndex(BA, MJA, MNA, TB);
								if (j <=  FLength*2)
									fprintf(out_ptr," %02X", getBufferByte(0,FrameIndex,j+l));
								else
									fprintf(out_ptr," %02X", getBufferByte(0,FrameIndex,j+l+4*HBytes));
							}
							fprintf(out_ptr,"\n");
						}
					}
				}
			}
		}
	}
	return(0);
}

void echoCLKs(void)
{
  int MinorMax, MinorIndex, ByteIndex;
  int tmpNColNULL=0;

  int FrameIndex, tmpCLK;
  int FLR_Bytes, NRowCLB;
  int TB, i, tmp;

	FLR_Bytes = (FLR - HBytes)*4;
	NRowCLB = FLR_Bytes/CLBBytes;

	for (TB = 0; TB < NRowRsc; TB++)
	{
		for(i=0; i<NColCLB; i++)
		{
			tmpNColNULL=0;
			for (tmp=0; tmp<=i; tmp++)
				if (ResourceString[tmp + tmpNColNULL] == 'N')
					tmpNColNULL++;
			if ((ResourceString[i + tmpNColNULL] == 'C')
				|| (ResourceString[i + tmpNColNULL] == 'L'))
				MinorMax = CLBFrames;
			else if (ResourceString[i + tmpNColNULL] == 'R')
				MinorMax = RI_Frames;
			else if (ResourceString[i + tmpNColNULL] == 'B')
				MinorMax = GFrames;
			else if (ResourceString[i + tmpNColNULL] == 'M')
				MinorMax = MFrames;
			else if (ResourceString[i + tmpNColNULL] == 'D')
				MinorMax = DFrames;
			else
				MinorMax = 0;

			for(MinorIndex=0; MinorIndex<MinorMax; MinorIndex++)
			{
				for (ByteIndex =  FLength*2; ByteIndex < (FLength*2 + HBytes); ByteIndex++)
				{
					FrameIndex = getFrameIndex(0,i,MinorIndex,TB);

					tmpCLK = tmpCLK | getBufferByte(0,FrameIndex,ByteIndex);

					if (tmpCLK)
					{
						int j = ByteIndex/CLBBytes;

						if(strncmp(Device,"XC7Z",4)==0)
							printf("CLK col %02i row %02i\n",(i),j+NRowCLB*(NRowRsc-1-TB));
						else
							printf("CLK col %02i row %02i\n",(i),j+NRowCLB*TB);

						printf("Frame %02i : ",MinorIndex);

						printf(" %02X", tmpCLK);
					}
				}
			}
		}
	}
}

int getBufferByte(int BufferIndex, int Frame, int ByteIndex)
{
	unsigned word,masked;
	word=FrameBuffer[BufferIndex][Frame][ByteIndex/4];
	masked=word&(0xFF000000>>((ByteIndex%4)*8));
	return(masked>>(24-(ByteIndex%4)*8));
}

int duplicate_FPGA_region(int MJA1, int MJA2, int MJA3, bool IsReplaced)
{
//	int i, j, k, l;
	int l;
	int tmpNColNULL, MinorIndex, MinorMax, tmp, CLB_Index;
	int FrameIndex1, FrameIndex2, WordIndex;
	int word, mask=0;
//	int ByteIndex, FLR_Bytes;

//	FLR_Bytes = (FLR)*4;

	for (CLB_Index = MJA1; CLB_Index <= MJA2; CLB_Index++)
	{
		tmpNColNULL=0;
		for (tmp=0; tmp<=CLB_Index; tmp++)
			if (ResourceString[tmp + tmpNColNULL] == 'N')
				tmpNColNULL++;
		if ((ResourceString[CLB_Index + tmpNColNULL] == 'C')
			|| (ResourceString[CLB_Index + tmpNColNULL] == 'L'))
			MinorMax = CLBFrames;
		else if (ResourceString[CLB_Index + tmpNColNULL] == 'R')
			MinorMax = RI_Frames;
		else if (ResourceString[CLB_Index + tmpNColNULL] == 'B')
			MinorMax = GFrames;
		else if (ResourceString[CLB_Index + tmpNColNULL] == 'M')
			MinorMax = MFrames;
		else if (ResourceString[CLB_Index + tmpNColNULL] == 'D')
			MinorMax = DFrames;
		else
			MinorMax = 0;

		for(MinorIndex=0; MinorIndex<MinorMax; MinorIndex++)
		{												// over all minor frames
			for(WordIndex=0; WordIndex<FLR; WordIndex++)
			{
				FrameIndex1 = getFrameIndex(0,CLB_Index,MinorIndex,0);
				FrameIndex2 = getFrameIndex(0,(CLB_Index+MJA3),MinorIndex,0);
				for(l = 0; l < CLBBytes/4; l++)						// over all Bytes witin a row of a frame
				{
					WordIndex = WordIndex + l;
					word = FrameBuffer[0][FrameIndex1][WordIndex];

					if (IsReplaced)
						FrameBuffer[0][FrameIndex1][WordIndex] = mask;

					FrameBuffer[0][FrameIndex2][WordIndex] = word;
				}
			}
		}
	}
	return 0;
}
//int RepClb(bool IsReplaced)
//  @params receives an argument IsReplaced to do copy (if IsReplaced is false)
//  or move (IsReplaced is true) the CLB content
int RepClb(int Cl1, int Rw1, int Cl2, int Rw2, bool IsReplaced)
{
	int k, l;
	int FrameIndex1, FrameIndex2, WordIndex1, WordIndex2, TB1, TB2;
	int word, mask=0;
	int temp;
//	int i;
	printf("Replication CLBs ");
	printf("From %i %i To %i %i\n",Cl1,Rw1,Cl2,Rw2);

	BA = 0;
	if	(strcmp(Device,"XC7Z010") == 0) {
		if (Rw1 >= 50)
			TB1 = 0;
		else
			TB1 = 1;

		if (Rw2 >= 50)
			TB2 = 0;
		else
			TB2 = 1;
	}

	if	(strcmp(Device,"XC7Z020") == 0) {
		if (Rw1 >= 100)
			TB1 = 0;
		else if (Rw1 >= 50)
			TB1 = 1;
		else
			TB1 = 2;

		if (Rw2 >= 100)
			TB2 = 0;
		else if (Rw2 >= 50)
			TB2 = 1;
		else
			TB2 = 2;
	}

	for(k=0; k<CLBFrames; k++)	{				// and over all Frames
		MNA = k;
		if (Rw1 >= 50)
			WordIndex1 = CLBBytes*(Rw1 - 50)/4;
		else
			WordIndex1 = CLBBytes*Rw1/4;

		if (Rw2 >= 50)
			WordIndex2 = CLBBytes*(Rw2 - 50)/4;
		else
			WordIndex2 = CLBBytes*Rw2/4;

		for(l=0; l<CLBBytes/4; l++)						// over all Bytes witin a row of a frame
		{
			FrameIndex1 = getFrameIndex(BA, Cl1, MNA, TB1);
			FrameIndex2 = getFrameIndex(BA, Cl2, MNA, TB2);
			WordIndex1 = WordIndex1 + l;
			WordIndex2 = WordIndex2 + l;
			word=FrameBuffer[0][FrameIndex1][WordIndex1];	// get data from the first CLB

			FrameBuffer[0][FrameIndex2][WordIndex2]=word;	// copy data to the second CLB
			if (IsReplaced)
				FrameBuffer[0][FrameIndex1][WordIndex1]=mask;
		}
	}
	return 0;
}
//int RepBRAM(bool IsReplaced)
//  @params receives an argument IsReplaced to do copy (if IsReplaced is false)
//  or move (IsReplaced is true) the BRAM content
int RepBRAM(int Cl1, int Rw1, int Cl2, int Rw2, bool IsReplaced)
{
	int k, l;
	int FrameIndex1, FrameIndex2, WordIndex1, WordIndex2, TB1, TB2;
	int word, mask=0;
	int temp;
	int RAMRow;
	printf("Replication BRAMs ");
	printf("From %i %i To %i %i\n",Cl1,Rw1,Cl2,Rw2);

	BA = 1;	// For BRAM content
	if	(strcmp(Device,"XC7Z010") == 0) {
		if (Rw1 >= 10)	// A row in XC7Z010 has 10 BRAMs
			TB1 = 0;
		else
			TB1 = 1;

		if (Rw2 >= 10)
			TB2 = 0;
		else
			TB2 = 1;
	}

	for (RAMRow=0; RAMRow < NoOfBRAMContRows; RAMRow++) {
		Rw1 = Rw1 + RAMRow;
		Rw2 = Rw2 + RAMRow;

		for(k=0; k<RAMFrames; k++)	{				// and over all Frames
			MNA = k;
			if (Rw1 >= 10)
				WordIndex1 = CLBBytes*(Rw1 - 10)/4;
			else
				WordIndex1 = CLBBytes*Rw1/4;
	
			if (Rw2 >= 10)
				WordIndex2 = CLBBytes*(Rw2 - 10)/4;
			else
				WordIndex2 = CLBBytes*Rw2/4;
	
			for(l=0; l<CLBBytes/4; l++)						// over all Bytes witin a row of a frame
			{
				FrameIndex1 = getFrameIndex(BA, Cl1, MNA, TB1);
				FrameIndex2 = getFrameIndex(BA, Cl2, MNA, TB2);
				WordIndex1 = WordIndex1 + l;
				WordIndex2 = WordIndex2 + l;
				word=FrameBuffer[0][FrameIndex1][WordIndex1];	// get data from the first CLB

				FrameBuffer[0][FrameIndex2][WordIndex2]=word;	// copy data to the second CLB
				if (IsReplaced)
					FrameBuffer[0][FrameIndex1][WordIndex1]=mask;
			}
		}
	}
	return 0;
}

void InitPartialMask(void)
{
	int i, j;

	for(i=0; i<250; i++)
		for(j=0; j<250; j++)
		{
			PartialMaskCLB[i][j] = 0;
			PartialMaskRAM[i][j] = 0;
		}
}
void ParseRsgOptions(char **argv, int i, char type)
{
	sscanf( argv[i+1], "%d", &RsgUpdateList[ModufiedRSGs].Row);
	sscanf( argv[i+2], "%d", &RsgUpdateList[ModufiedRSGs].Col);
	sscanf( argv[i+3], "%x", &RsgUpdateList[ModufiedRSGs].Value);
	RsgUpdateList[ModufiedRSGs].type = type;
}
void ParseBitManipulations(char **argv, int i, char type)
{
	sscanf( argv[i+1], "%d", &BitManipulationList[BitManipulations].Row);
	sscanf( argv[i+2], "%d", &BitManipulationList[BitManipulations].Col);
	sscanf( argv[i+3], "%d", &BitManipulationList[BitManipulations].Offset);
	BitManipulationList[BitManipulations].type = type; //'D'elete 'S'et
}
void SetBit(int buffer, int col, int row, int offset)
{
	int FrameIndex;
	int ByteIndex;
	unsigned char ReadByte;
	unsigned char BitMask;
	// CLB origin is top left (1,1)
	FrameIndex = getFrameIndex(0,col+2,offset/(CLBBytes*8),0);	// temporarily set TB=0
	ByteIndex = CLBBytesIO + (CLBBytes*(row-1)) + ((offset%(8*CLBBytes))/8);
	ReadByte = getBufferByte(buffer,FrameIndex,ByteIndex);
	BitMask = 1 << (offset%8);
    WriteBufferByte(buffer,FrameIndex,ByteIndex,(ReadByte | BitMask));
}
void ClearBit(int buffer, int col, int row, int offset)
{
	int FrameIndex;
	int ByteIndex;
	unsigned char ReadByte;
	unsigned char BitMask;
	// CLB origin is top left (1,1)
	FrameIndex = getFrameIndex(0,col+2,offset/(CLBBytes*8),0);	// temporarily set TB=0
	ByteIndex = CLBBytesIO + (CLBBytes*(row-1)) + ((offset%(8*CLBBytes))/8);
	ReadByte = getBufferByte(buffer,FrameIndex,ByteIndex);
	BitMask = 1 << (offset%8);
    WriteBufferByte(buffer,FrameIndex,ByteIndex,(ReadByte & (~BitMask)));
}
int TestBit(int buffer, int col, int row, int offset)
{
	int FrameIndex;
	int ByteIndex;
	unsigned char ReadByte;
	unsigned char BitMask;
	// CLB origin is top left (1,1)
	FrameIndex = getFrameIndex(0,col+2,offset/(CLBBytes*8),0);	// temporarily set TB=0
	ByteIndex = CLBBytesIO + (CLBBytes*(row-1)) + ((offset%(8*CLBBytes))/8);
	ReadByte = getBufferByte(buffer,FrameIndex,ByteIndex);
	BitMask = 1 << (offset%8);
	if ((ReadByte & BitMask) > 0)
		return 1;
	else
		return 0;
}
int TestRSGUsage(int buffer, int col, int row)
{
	int result;
	int PossibleCanddidate;
	int i, j;

	result=-1;					// found no usage
	for(i=0; i<8; i++)		// over all LUTs
	{
		PossibleCanddidate=-1;
		for(j=0; j<4; j++)	// over all indicator bits

			if (LutShiftUsageOffset[i][j]>0)
			{
				if(j==0 && (TestBit(buffer, col, row, LutShiftUsageOffset[i][j])==1))
					PossibleCanddidate=1;
				if((j>0) && (PossibleCanddidate==1) && (TestBit(buffer, col, row, LutShiftUsageOffset[i][j])==1))
					PossibleCanddidate=0;
			}
		if(PossibleCanddidate==1)
			if (result==-1)
				result=i;
			else
				return -2;		// found two possible candidates (error)
	}
	return result;
}
void ParseLinkPitFileOptions(char **argv, int i)
{
	LinkInformationList[LinkPitFileMode].PitFileName=argv[i+1];
	sscanf( argv[i+2], "%d", &LinkInformationList[LinkPitFileMode].PitRow);
	sscanf( argv[i+3], "%d", &LinkInformationList[LinkPitFileMode].PitCol);
}
int LinkPitFile(char *PitFileName, int col, int row)
{
  int Top, Bottom;
  int tmp;
  int BA, MJA, MJA_and_skipped;
  int ModuleClbColumns;
  char ModuleDevice[200];
  int ModuleHight;
  int ModuleStartRowByteIndex;
  int StartFrame;
  long int CurrentFilePos;
  long int PitStreamSize;
  unsigned char NextByte;
  FILE *PitPtr;
  int i, j;

  PitPtr=fopen(PitFileName,"rb");
	if(PitPtr == NULL)
	{
		fprintf(stderr,"Unable to load pitfile %s\n",PitFileName);
		return(-1);
	}
	tmp = ParsePitHeader(PitPtr, &Top, &Bottom, ModuleDevice);
	if(tmp != 0)
	{
		fprintf(stderr,"Problem while parsing the pitfile - trying to continue.\n");
		return(-1);
	}
	ModuleClbColumns = CheckLinkModulePlacement(ModuleDevice, col, row, Top, Bottom);
	if(ModuleClbColumns < 0)
	{
		fprintf(stderr,"Pitfile fits not into target device %s - skipping it.\n");
		return(-1);
	}

	ModuleHight = GetModuleHight(ModuleDevice, Top, Bottom);
	ModuleStartRowByteIndex = GetModuleStartRowByteIndex(ModuleDevice, row);
	PitStreamSize = GetPitStreamSize(ModuleDevice, ModuleHight);
	CurrentFilePos= ftell(PitPtr);
	if((fseek(PitPtr,(PitStreamSize+CurrentFilePos),SEEK_SET) == 0) && (fseek(PitPtr,(PitStreamSize+CurrentFilePos+1),SEEK_SET) != 0))
	{
		fprintf(stderr,"Pitfile bitstream data size does not match header.\n");
		return(-1);
	}

	fseek(PitPtr,CurrentFilePos,SEEK_SET);					//set file pointer back to pure pitstream data

	for(i=0; i<5000; i++)				// over all frames
		for(j=0; j<500; j++)			// over all words
			FrameBuffer[1][i][j]=FrameBuffer[0][i][j];				// copy FrameBuffer for undo
	
	tmp=0;
	BA=0;
	if((ModuleResourceString[0] & 0xFF) == 0xC0)		// the start column is given by a CLB one
		MJA = col + 2;						// +GCLK + IOB + IOI - index starts with 0 while first CLB column is 1
	else									// the start column is given by a BRAM one
		MJA = countOtherColumns(col,'C','R') + 2;	// CLB columns firstly contain Clk information and left IOB configuration 
	StartFrame = getFrameIndex(BA, MJA, tmp,0);		// temporarily set TB=0
	tmp = GetNumberOfResourceColumns(0xC0);					// get # CLBs
	if((ClbLeft==0) && (ClbRight==0) && WriteModuleBitstreamMode==true) // in the case we want to generate partial bitfile without -C option
	{
		ClbLeft = MJA - 2;
		ClbRight= MJA - 2 + tmp -1;
	}
	for(i=0; i<(tmp*CLBFrames); i++)
		for(j=0; j<ModuleHight; j++)
		{
			fread(&NextByte, 1, 1, PitPtr);
			WriteBufferByte(0, i+StartFrame, j+getROWByteIndex(row), NextByte);
		}

	tmp=0;
	BA=1;							// BRAM Content
	if((ModuleResourceString[0] & 0xF0) == 0xB0)		// the start column is given by a RAM one
		MJA=col -1;
	else
		MJA=countOtherColumns(col,'R','C') -1;			// MJA starts at 0
	MJA_and_skipped = MJA + GetNumberOfSkippedResourceColumns((0xB0 | content));
	StartFrame = getFrameIndex(BA, MJA_and_skipped, tmp,0);	// temporarily set TB=0
	tmp = GetNumberOfResourceColumns((0xB0 | content));		// get # BRAM content
	if((BRamLeft==0) && (BRamRight==0) && WriteModuleBitstreamMode==true) // in the case we want to generate partial bitfile without -B option
	{
		BRamLeft  = MJA + 1;
		BRamRight = MJA + 1 + tmp - 1;
	}
	for(i=0; i<(tmp*RAMFrames); i++)
		for(j=0; j<ModuleHight; j++)
		{
			fread(&NextByte, 1, 1, PitPtr);
			WriteBufferByte(0, i+StartFrame, j+getROWByteIndex(row), NextByte);
		}
	tmp=0;
	BA=2;						// BRAM Routing
	MJA_and_skipped = MJA + GetNumberOfSkippedResourceColumns((0xB0 | routing));
	StartFrame = getFrameIndex(BA, MJA_and_skipped, tmp,0);	// temporarily set TB=0
	tmp = GetNumberOfResourceColumns((0xB0 | routing));		// get # BRAM routing
	if((BRamRoutingLeft==0) && (BRamRoutingRight==0) && WriteModuleBitstreamMode==true) // in the case we want to generate partial bitfile without -b option
	{
		BRamRoutingLeft  = MJA + 1;
		BRamRoutingRight = MJA + 1 + tmp -1;
	}
	for(i=0; i<(tmp*RI_Frames); i++)
		for(j=0; j<ModuleHight; j++)
		{
			fread(&NextByte, 1, 1, PitPtr);
			WriteBufferByte(0, i+StartFrame, j+getROWByteIndex(row), NextByte);
		}

	return(0);
}
int UpdateRSG(unsigned Value, int Col, int Row, char type)
{
	int UsedLUT;
	unsigned LocalValue;
	int i;
	LocalValue = Value;
	if(type=='A')
	{
		if (strcmp(Family,"Spartan3") == 0)
			switch(Value)
			{
				case 0x0: LocalValue = 0x0005;	break;
				case 0x1: LocalValue = 0x000A;	break;
				case 0x2: LocalValue = 0x0050;	break;
				case 0x3: LocalValue = 0x00A0;	break;
				case 0x4: LocalValue = 0x0500;	break;
				case 0x5: LocalValue = 0x0A00;	break;
				case 0x6: LocalValue = 0x5000;	break;
				case 0x7: LocalValue = 0xA000;	break;
				default: 		
					fprintf(stderr,"Wrong address specification.\n");
					exit(-3424);
					break;
			}
		else
			switch(Value)
			{
				case 0x0: LocalValue = 0x0001;	break;
				case 0x1: LocalValue = 0x0002;	break;
				case 0x2: LocalValue = 0x0004;	break;
				case 0x3: LocalValue = 0x0008;	break;
				case 0x4: LocalValue = 0x0010;	break;
				case 0x5: LocalValue = 0x0020;	break;
				case 0x6: LocalValue = 0x0040;	break;
				case 0x7: LocalValue = 0x0080;	break;
				case 0x8: LocalValue = 0x0100;	break;
				case 0x9: LocalValue = 0x0200;	break;
				case 0xA: LocalValue = 0x0400;	break;
				case 0xB: LocalValue = 0x0800;	break;
				case 0xC: LocalValue = 0x1000;	break;
				case 0xD: LocalValue = 0x2000;	break;
				case 0xE: LocalValue = 0x4000;	break;
				case 0xF: LocalValue = 0x8000;	break;
				default: 		
					fprintf(stderr,"Wrong address specification.\n");
					exit(-3424);
					break;
			}
	}
	UsedLUT = RSGUsage[Col][Row];
	if(UsedLUT < 0)
	{
		fprintf(stderr,"Error: cannot update reconfigurable select generator at position %i %i.\n", Col, Row);
		exit(-3424);
	}
	for(i=0; i<16; i++)		// clear LUT values
		ClearBit(0, Col, Row, LutBitOffset[UsedLUT][i]);

	for(i=0; i<16; i++)		// set address values
		if(((1 << i) & LocalValue) == 0)  // The LUT values are stored inverted!
			SetBit(0, Col, Row, LutBitOffset[UsedLUT][i]);
	return 0;
}
int ParsePitHeader(FILE *PitPtr, int *Top, int *Bottom, char ModuleDevice[])
{
	int i;
	char TmpStr[200];
	unsigned char NextByte;
//	int Top, Bottom;
	int NBytes;

		for(i=0; i<200; i++)
			TmpStr[i]='\0';

		NBytes=fread(&TmpStr, 1, 5, PitPtr);
   		if (NBytes==0)
		{
   			fprintf(stderr,"Unexpected end of pitfile.\n");
			return(-1);
		}
		if (strncmp(TmpStr, "#PIT#" ,5)!=0)
		{
   			fprintf(stderr,"Need a pitfile for linking.\n");
			return(-1);
		}
		i=0;
		NextByte=0;
		while(NextByte != '#')
		{
			NBytes=fread(&NextByte, 1, 1, PitPtr);
   			if (NBytes==0)
			{
   				fprintf(stderr,"Unexpected end of pitfile.\n");
				return(-1);
			}
			TmpStr[i] = NextByte;
			i++;
		}
		TmpStr[i-1] = '\0';
		if (strncmp(TmpStr, Family ,20)!=0)
		{
   			fprintf(stderr,"Device or Familie missmatch for linking.\n");
			return(-1);
		}

		//get device string
		i=0;
		NextByte=0;
		while(NextByte != '#')
		{
			NBytes=fread(&NextByte, 1, 1, PitPtr);
   			if (NBytes==0)
			{
   				fprintf(stderr,"Unexpected end of pitfile.\n");
				return(-1);
			}
			ModuleDevice[i] = NextByte;
			i++;
		}
		ModuleDevice[i-1] = '\0';
		// dump Device string		
		
		//get speed grade
		i=0;
		NextByte=0;
		while(NextByte != '#')
		{
			NBytes=fread(&NextByte, 1, 1, PitPtr);
   			if (NBytes==0)
			{
   				fprintf(stderr,"Unexpected end of pitfile.\n");
				return(-1);
			}
			TmpStr[i] = NextByte;
			i++;
		}
		TmpStr[i-1] = '\0';
		// dump speed grade		

		//get Delay
		i=0;
		NextByte=0;
		while(NextByte != '#')
		{
			NBytes=fread(&NextByte, 1, 1, PitPtr);
   			if (NBytes==0)
			{
   				fprintf(stderr,"Unexpected end of pitfile.\n");
				return(-1);
			}
			TmpStr[i] = NextByte;
			i++;
		}
		TmpStr[i-1] = '\0';
		// dump Delay	

		//get Top
		i=0;
		NextByte=0;
		while(NextByte != '#')
		{
			NBytes=fread(&NextByte, 1, 1, PitPtr);
   			if (NBytes==0)
			{
   				fprintf(stderr,"Unexpected end of pitfile.\n");
				return(-1);
			}
			TmpStr[i] = NextByte;
			i++;
		}
		TmpStr[i-1] = '\0';
		sscanf( TmpStr, "%d", Top);	

		//get Bottom
		i=0;
		NextByte=0;
		while(NextByte != '#')
		{
			NBytes=fread(&NextByte, 1, 1, PitPtr);
   			if (NBytes==0)
			{
   				fprintf(stderr,"Unexpected end of pitfile.\n");
				return(-1);
			}
			TmpStr[i] = NextByte;
			i++;
		}
		TmpStr[i-1] = '\0';
		sscanf( TmpStr, "%d", Bottom);	

		//get module resource string
		for(i=0; i<200; i++)
			ModuleResourceString[i]=0;
		i=0;
		NextByte=0;
		while(NextByte != '#')
		{
			NBytes=fread(&NextByte, 1, 1, PitPtr);
   			if (NBytes==0)
			{
   				fprintf(stderr,"Unexpected end of pitfile.\n");
				return(-1);
			}
			ModuleResourceString[i] = NextByte;
			i++;
		}
		ModuleResourceString[i-1] = 0;

		// get remaining # before the pure bitstream data
		NBytes=fread(&NextByte, 1, 1, PitPtr);
   		if (NBytes==0)
			{
   				fprintf(stderr,"Unexpected end of pitfile.\n");
				return(-1);
			}
		if(NextByte != '#')
		{
			fprintf(stderr,"Pitfile header corrupted.\n");
			return(-1);
		}
	return(0);
}
int CheckLinkModulePlacement(char ModuleDevice[], int col, int row, int Top, int Bottom)
{
  int ModuleResourceStringLength;
  int ModuleClbColumns;
  int ModuleStartRowByteIndex;
  int tmp;
  int LeftResourceStringIndex;
  int ModuleHight;
  int i;

	if(ModuleResourceString[0] == 0)
	{
		fprintf(stderr,"Warning: empty resource string in link object.\n");
		return(-1);
	}
	ModuleResourceStringLength=0;
	ModuleClbColumns=0;
	while(ModuleResourceString[ModuleResourceStringLength] != 0)
	{
		if((ModuleResourceString[ModuleResourceStringLength] & 0xF0) == 0xC0)				//clb column
			ModuleClbColumns++;
		ModuleResourceStringLength++;
	}

// horizontal checks
	LeftResourceStringIndex = GetLeftResourceStringIndex(col);
	if(LeftResourceStringIndex == -1)
	{
		fprintf(stderr,"Link Failure: module exeeds right border.\n");
		return(-1);
	}

	tmp=false;
	for(i=0; i<ModuleResourceStringLength; i++)
	{
		if((ModuleResourceString[i] & 0xF0) == 0xC0)				//clb column
			if(ResourceString[i+LeftResourceStringIndex] != 'C')
				tmp=true;
		if((ModuleResourceString[i] & 0xF0) == 0xB0)
			if(ResourceString[i+LeftResourceStringIndex] != 'R')
				tmp=true;
	}
	if(tmp)
	{
		fprintf(stderr,"Resource string missmatch when linking\n");
		return(-1);
	}

// vertical checks
	ModuleHight = GetModuleHight(ModuleDevice, Top, Bottom);
	if(ModuleHight == -1)
	{
		fprintf(stderr,"Pitfile exceeds top or bottom border when linking\n");
		return(-1);
	}
	ModuleStartRowByteIndex = GetModuleStartRowByteIndex(ModuleDevice, row);
	if(((ModuleStartRowByteIndex+ModuleHight) > (FLength*4)) || (ModuleStartRowByteIndex == -1))
	{
		fprintf(stderr,"Link Failure: module exeeds bottom border.\n");
		return(-1);
	}
	//return(0);					// passed all tests
	return ModuleClbColumns;		// we use this function to count the CLB colums
}
int GetModuleHight(char Device[], int Top, int Bottom)		// returns hight in bytes
{
	struct DeviceParameters ModuleDeviceParameters;
	int ClbRows;

	SetDeviceParameters(Device, &ModuleDeviceParameters);

	ClbRows = ((ModuleDeviceParameters.FLength*4) - 2*ModuleDeviceParameters.CLBBytesIO) / ModuleDeviceParameters.CLBBytes;

	if(Bottom < Top)										// origin is top
	{
		fprintf(stderr,"Bottom border was found above top one.\n");
		return(-1);
	}
	if(Bottom > ClbRows +1)									// larger than CLBs and bottom IO
	{
		fprintf(stderr,"Ptifile bottom definition exceeds the original device.\n");
		return(-1);
	}

	if((Top == 0) && (Bottom == 0))							// partial module contains only top IO cell
		return(ModuleDeviceParameters.CLBBytesIO);		
	if((Top == ClbRows+1) && (Bottom == ClbRows+1))			// partial Module containd only bottom IO cell
		return(ModuleDeviceParameters.CLBBytesIO);	

	if((Top == 0) && (Bottom == ClbRows+1))					// partial module contains complete CLB column including IOs
		return(ModuleDeviceParameters.FLength*4);

	if(Top == 0)											// partial module contains top IO and some CLBs
		return(ModuleDeviceParameters.CLBBytesIO + (Bottom*ModuleDeviceParameters.CLBBytes));
	if(Bottom == ClbRows+1)									// partial module contains bottom IO and some CLBs
		return(ModuleDeviceParameters.CLBBytesIO + ((Bottom-1)*ModuleDeviceParameters.CLBBytes));

	if((Top > 0) && (Bottom <=ClbRows))						// partial module contains only CLBs
		return((Bottom - Top + 1) * ModuleDeviceParameters.CLBBytes);

	return(-1);												// should not occur
}
int GetModuleStartRowByteIndex(char Device[], int row)
{
	struct DeviceParameters ModuleDeviceParameters;
	int ClbRows;

	SetDeviceParameters(Device, &ModuleDeviceParameters);
	ClbRows = ((ModuleDeviceParameters.FLength*4) - 2*ModuleDeviceParameters.CLBBytesIO) / ModuleDeviceParameters.CLBBytes;

	if(row == 0)				// start is top IO
		return(0);
	if(row == ClbRows+1)
		return((ClbRows*ModuleDeviceParameters.CLBBytes) + ModuleDeviceParameters.CLBBytesIO);
	if(row <= ClbRows)
		return(ModuleDeviceParameters.CLBBytesIO + ((row-1)*ModuleDeviceParameters.CLBBytes));
	return(-1);
}

int GetPitStreamSize(char ModuleDevice[], int ModuleHight)
{
	int Filesize=0;
	int i=0;
	struct DeviceParameters ModuleDeviceParameters;

	SetDeviceParameters(ModuleDevice, &ModuleDeviceParameters);

	while(true)
	{
		if(ModuleResourceString[i] == 0)
			break;
		else if((ModuleResourceString[i] & 0xF0) == 0xC0)					// one CLB column
			Filesize = Filesize + (ModuleHight * ModuleDeviceParameters.CLBFrames);
		else if((ModuleResourceString[i] & 0xF0) == 0xB0)					// one BRAM column
		{
			if((ModuleResourceString[i] & routing) > 0)
			Filesize = Filesize + (ModuleHight * ModuleDeviceParameters.RI_Frames);
			if((ModuleResourceString[i] & content) > 0)
			Filesize = Filesize + (ModuleHight * ModuleDeviceParameters.RAMFrames);
		}
		else
		{
			fprintf(stderr,"Unrecognized module resource string entry.\n");
			return(-1);													// Resource
		}
		i++;
	}
	return (Filesize);
}
int GetLeftResourceStringIndex(int col)
{
  int ColCounter;
  int i;

	ColCounter = 0;	// -1 would be the left IO column...
	for(i=0; i<NColCLB+NColRAM; i++)
	{
		if((ModuleResourceString[0] & 0xF0) == 0xC0)	//count only CLB-columns
			if(ResourceString[i] == 'C')
				ColCounter++;
		if((ModuleResourceString[0] & 0xF0) == 0xB0)	// count only BRAM-Columns
			if(ResourceString[i] == 'R')
				ColCounter++;
		if(ColCounter >= col)
			return(i);
	}
	return(-1);											// Module exeeds left border
}

int GetNumberOfResourceColumns(unsigned char Resource)
{
	int counter=0, i;
	for(i=0; i<250; i++)
	{
		if(ModuleResourceString[i]==0)			//reached end of resource string
			return(counter);
		if(Resource == ModuleResourceString[i]) // direct match
			counter++;

		else if((ModuleResourceString[i] & Resource) == Resource)
			counter++;
	}
	return(-1);									// should not occur
}
int GetNumberOfSkippedResourceColumns(unsigned char Resource) 
// returns initial number of BRAM (content or routing) columns that are not used within the first CLB columns
{
	int counter=0, i;
	for(i=0; i<250; i++)
	{
		if(ModuleResourceString[i]==0)			//reached end of resource string
			return(counter);
		if(((Resource & 0xF0) == (ModuleResourceString[i] & 0xF0)) && ((Resource & ModuleResourceString[i]) == Resource)) // direct match, first used column found
			return(counter);

		else if((Resource & 0xF0) == (ModuleResourceString[i] & 0xF0))
			counter++;
	}
	return(-1);									// should not occur
}
int countOtherColumns(int col, char countSymbol, char matchSymbol)
{
	int SymbolCounter=0;
	int MatchCounter=0;
	int i;
	for(i=0; i<250; i++)
	{
		if(ResourceString[i] == countSymbol)
			SymbolCounter++;
		if(ResourceString[i] == matchSymbol)
			MatchCounter++;
		if(MatchCounter == col)		
			return(SymbolCounter +1 );
	}
	return(-1);									// should not occur
}
int WriteBufferByte(int BufferIndex, int Frame, int ByteIndex, char WriteByte)
{
	unsigned word,masked,tmp,tmp2;

	word   = FrameBuffer[BufferIndex][Frame][ByteIndex/4];

	if((ByteIndex%4)==0)
	{
		tmp = 0;
		tmp = 0xFF000000 & (WriteByte << 24);
		masked = (word & 0x00FFFFFF);
		tmp2 = masked | tmp;
		FrameBuffer[BufferIndex][Frame][ByteIndex/4] = tmp2; // (word & 0x00FFFFFF) | tmp; //(WriteByte << 24);
		return(0);
	}
	if((ByteIndex%4)==1)
	{
		tmp = 0;
		tmp = 0x00FF0000 & (WriteByte << 16);
		masked = (word & 0xFF00FFFF);
		tmp2 = masked | tmp;
		FrameBuffer[BufferIndex][Frame][ByteIndex/4] = tmp2; //(word & 0xFF00FFFF) | tmp; //(WriteByte << 16);
		return(0);
	}
	if((ByteIndex%4)==2)
	{
		tmp = 0;
		tmp = 0x0000FF00 & (WriteByte << 8);
		masked = (word & 0xFFFF00FF);
		tmp2 = masked | tmp;
		FrameBuffer[BufferIndex][Frame][ByteIndex/4] = tmp2; // (word & 0xFFFF00FF) | tmp; //(WriteByte << 8);
		return(0);
	}
	if((ByteIndex%4)==3)
	{
		tmp = 0;
		tmp = 0x000000FF & WriteByte;
		masked = (word & 0xFFFFFF00);
		tmp2 = masked | tmp;
		FrameBuffer[BufferIndex][Frame][ByteIndex/4] = tmp2; // (word & 0xFFFFFF00) | tmp; //(WriteByte);
		return(0);
	}
	return(0);
}

int main(int argc, char **argv)
{
	int i, j;

	mygetopt(argc,argv);	/*	Parameter Parser */
	file_handling();		/*	open files */
	
	initForScan(0);
	InitPartialMask();
	scan(in_ptr0,0); 
	
	if(twoInputMode)
	{
		initForScan(1);
		scan(in_ptr1,1);
		for(i=0; i<5000; i++)				// over all frames
  			for(j=0; j<500; j++)			// over all words
				// XOR both buffers to get differences
				FrameBuffer[0][i][j]=FrameBuffer[0][i][j]^FrameBuffer[1][i][j];
	}

	bufferHeaderAndFooter();

	fclose(in_ptr0);
	if(in_ptr1 != NULL)
		fclose(in_ptr1);

	if(WritePartial)
		dumpPartial();

	if(WritePartialByMask)
		WritePartialMask();

	for(i=0; i<BitManipulations; i++)
		if(BitManipulationList[i].type=='S')
			SetBit(0, BitManipulationList[i].Col, BitManipulationList[i].Row, BitManipulationList[i].Offset);
		else if(BitManipulationList[i].type=='T')
			if(TestBit(0, BitManipulationList[i].Col, BitManipulationList[i].Row, BitManipulationList[i].Offset)==0)
				SetBit(0, BitManipulationList[i].Col, BitManipulationList[i].Row, BitManipulationList[i].Offset);
			else
				ClearBit(0, BitManipulationList[i].Col, BitManipulationList[i].Row, BitManipulationList[i].Offset);
		else
			ClearBit(0, BitManipulationList[i].Col, BitManipulationList[i].Row, BitManipulationList[i].Offset);

	if(DumpBitstream)
		dumpFrames();	

	if(DumpCLBs>0)
		echoCLBs();

	if(DumpCLBs>2)
		echoOffsetCLBs();

	if (RepRsc)
		if(strcmp(TypeOfResource,"CLB")==0)
			RepClb(RscCl1, RscRw1, RscCl2, RscRw2, true);
		else if(strcmp(TypeOfResource,"BRAM")==0)
			RepBRAM(RscCl1, RscRw1, RscCl2, RscRw2, true);
	
	if(WriteFullBitstreamMode)
		WriteFullBitstream(FullBitstreamFileName);
	
	if(WriteModuleBitstreamMode)
		WriteModuleBitstream(ModuleBitstreamFileName);

	fclose(out_ptr);

	if(in_ptr1 != NULL)
		fclose(in_ptr1);

	return(0);
}


