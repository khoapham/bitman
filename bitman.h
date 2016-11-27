typedef int bool;
#define	true 1
#define	false 0

#define unused 0x00
#define routing 0x01
#define content 0x02
#define routing_content 0x03

#define ByDeviceID 1
#define ByWriteFLR 2
#define UserFRI 3
#define MaxFrames 5000
#define NoOfBRAMContRows 2	// Number of BRAM content rows

/* prototypes */
void mygetopt(int argc, char **argv);
int parsePartial(int argc, char **argv, int i, int j);
void help(void);
int file_handling(void);
unsigned int GetNextWord(FILE *ptr, int *flag);
int updateFLR(int val, int who);
int getColIndex(void);

void dumpPartial(void);
int getBufferByte(int BufferIndex, int Frame, int ByteIndex);

int FillBuffer(FILE *ptr, int WordCount, int BufferIndex, bool isPartial);
void bufferHeaderAndFooter(void);
int WriteFullBitstream(char *FullBitstreamFileName);
int WriteModuleBitstream(char *ModuleBitstreamFileName);
int WriteModuleBitstreamHeader(FILE *OutBitfilePtr);
int WriteModuleBitstreamCommand(FILE *OutBitfilePtr);
int WriteModuleBitstreamFooter(FILE *OutBitfilePtr);
int WriteModuleBitstreamData(FILE *OutBitfilePtr, int BA, int Offset, int Blocks, int FramesPerBlock);
void scan(FILE *in_ptr, int BufferIndex);

void echoCLBs(void);
int echoRAMs(char mode);
void echoCLKs(void);

void InitPartialMask(void);
void echoTXT(int level,char *msg);

void ParseRsgOptions(char **argv, int i, char type);
void ParseBitManipulations(char **argv, int i, char type);
void SetBit(int buffer, int col, int row, int offset);
void ClearBit(int buffer, int col, int row, int offset);
int TestBit(int buffer, int col, int row, int offset);
int TestRSGUsage(int buffer, int col, int row);
void ParseBoundingBox(char **argv, int i, int *left, int *right, int *top, int *bottom);
void ParseClbMask(char **argv, int i);
void ParseBRamMask(char **argv, int i);
void ParseBRamRoutingMask(char **argv, int i);
int CheckPartialMaskHight(void);
void ParseLinkPitFileOptions(char **argv, int i);
int LinkPitFile(char *PitFileName, int col, int row);
int UpdateRSG(unsigned Value, int Col, int Row, char type);
int ParsePitHeader(FILE *PitPtr, int *Top, int *Bottom, char ModuleDevice[]);
int CheckLinkModulePlacement(char ModuleDevice[], int col, int row, int Top, int Bottom);
int GetLeftResourceStringIndex(int col);
int GetModuleHight(char Device[], int Top, int Bottom);
int GetModuleStartRowByteIndex(char Device[], int row);
int GetPitStreamSize(char ModuleDevice[], int ModuleHight);
int GetNumberOfResourceColumns(unsigned char Resource);
int GetNumberOfSkippedResourceColumns(unsigned char Resource);
int countOtherColumns(int col, char countSymbol, char matchSymbol);
int WriteBufferByte(int BufferIndex, int Frame, int ByteIndex, char WriteByte);

char *input_file;
char *output_file;
char *file1;				/* the first file parameter */
char *file2;
char *file3;
FILE *in_ptr0;
FILE *in_ptr1;
FILE *out_ptr;
FILE *msg_ptr;

int verbose=0;
int header=false;

int Speed=0;
int Delay=0;
int twoInputMode;
int WritePartial=false;
int ColBoundingBox1=0;
int ColBoundingBox2=0;
int RowBoundingBox1=0;
int RowBoundingBox2=0;
char *PartialFile;
int DumpBitstream=false;
int WriteFullBitstreamMode=false;
int WriteModuleBitstreamMode=false;
int DumpCLBs=0;
char *DumpBitstreamFileName;
char *FullBitstreamFileName;
char *ModuleBitstreamFileName;
int LinkPitFileMode=0;
// global variables and functions' declarations for the Resource Replication
int RepRsc=0;
char *TypeOfResource;
void ParseRepRsc(char **argv, int i);
int RscCl1=0;
int RscRw1=0;
int RscCl2=0;
int RscRw2=0;

int RepClb(int Cl1, int Rw1, int Cl2, int Rw2, bool IsReplaced);
int RepBRAM(int Cl1, int Rw1, int Cl2, int Rw2, bool IsReplaced);
int duplicate_FPGA_region(int MJA1, int MJA2, int MJA3, bool IsReplaced);

void ParseMetaHeader(char **argv, int i);
char *metadata="";

struct LinkInformation {
	int PitCol;
	int PitRow;
	char *PitFileName;
};
struct LinkInformation LinkInformationList[20];

long int EndOfHeaderFilePosition=0;
long int SyncSequenceFilePosition=0;
char DirtyFrameMask[MaxFrames];

int WritePartialByMask=false;
char *PartialFileByMask;

int FLR = -1;  				// frane length 
int userFLR = -1;  			// frame length may be set by user (later)
int writeFLR = -1; 			// frame length can be set by writing the Frame Length Register FLR

unsigned char InitialHeader[50000];
int InitialHeaderSize = 0;
unsigned char InitialFooter[1000000];
int InitialFooterSize = 0;
short PartialMaskCLB[250][250];
short PartialMaskRAM[250][250];
//unsigned FrameBuffer[2][5000][500];	// this is the complete bitstream of a XC2V8000
unsigned FrameBuffer[2][30000][150];	// this is the complete bitstream of a XC7Z010
int FrameBufferState[5000];         // -1 stream is empty  

unsigned ShadowCol[500];			// A configuration column is firstly written into a shadow reg...
int ShadowState  = false;
int CLBState[6][100][60][50];	// denotes for each CLB if the entire frame contains some information
int DeviceID = 0;

int ClbMaskActive=false;
int BRamMaskActive=false;
int BRamRoutingMaskActive=false;
int ClbLeft=0, ClbRight=0, ClbTop=0, ClbBottom=0;
int BRamLeft=0, BRamRight=0, BRamTop=0, BRamBottom=0;
int BRamRoutingLeft=0, BRamRoutingRight=0, BRamRoutingTop=0, BRamRoutingBottom=0;
unsigned char ModuleResourceString[200];

int readstate;

int ModufiedRSGs = 0;
struct RsgUpdateInformation {
	int Col;
	int Row;
	unsigned Value;
	char type;
};

int BitManipulations = 0;
struct BitManipulationInformation {
	int Col;
	int Row;
	unsigned Offset;
	char type;
};

// up to 50 reconfigurable select generators, each with column, row, and the address information
struct RsgUpdateInformation RsgUpdateList[150];

struct BitManipulationInformation BitManipulationList[150];

int RSGUsage[250][250];	// denotes for each CLB if the entire frame contains some information

void dumpFrames(void)
{
  int i,j,k;
  FILE *DumpPtr;
	DumpPtr=fopen(DumpBitstreamFileName,"wb+");
	if (DumpPtr == NULL)
	{	
		fprintf(stderr,"Can not open output file for dumping complete bitstream.\n");
		exit(34);
	}
	for(i=0; i<NFrames; i++)				// over all frames
  		for(j=0; j<FLength*4; j++)			// over all words
		{
			k= getBufferByte(0,i,j);		// scan for used CLB-cell
            fwrite(&k, 1, 1, DumpPtr);
		}
	fclose(DumpPtr);
}

void dumpPartial(void)
{
  int i,j,k;
  int NRowCLB;
  int leftBound;
  int rightBound;
  int topBound;
  int bottomBound;
  FILE *PartialPtr;

	NRowCLB = ((FLR*4)-(2*CLBBytesIO))/CLBBytes;		// (we compute bytes) all-top&botttom IO-cell 

	PartialPtr=fopen(PartialFile,"wb+");
	if (PartialPtr == NULL)
	{	
		fprintf(stderr,"Can not open output file for dumping partial bitstream.\n");
		exit(34);
	}

	BA=0;   MJA=ColBoundingBox1;   MNA=0;
	leftBound=getFrameIndexGlobal();												//left

	BA=0;   MJA=ColBoundingBox2;   MNA=0;
	rightBound=getFrameIndexGlobal();												//right + width-1
	if(ColBoundingBox2==0)													// we defined the clock column
		rightBound=rightBound+CLKFrames-1;
	else if(ColBoundingBox2==1 || ColBoundingBox2==NRowCLB+4)				// we defined an IO-cell
		rightBound=rightBound+IOBFrames-1;
	else if(ColBoundingBox2==2 || ColBoundingBox2==NRowCLB+3)				// we defined an IO-cell interconnect
		rightBound=rightBound+IOIFrames-1;
	else																	// normal CLB column
		rightBound=rightBound+CLBFrames-1;

	if(RowBoundingBox1==0)													//top
		topBound=0;
	else
		topBound=CLBBytesIO + CLBBytes*(RowBoundingBox1-1);

	if(RowBoundingBox2==0)													//bottom
		bottomBound=CLBBytesIO;
	else if(RowBoundingBox2==NRowCLB+1)
		bottomBound=FLR*4;
	else
		bottomBound=CLBBytesIO + CLBBytes*RowBoundingBox2 -1;

	fprintf(out_ptr,"partial: topBound %i bottomBound %i leftBound %i rightBound %i\n",topBound, bottomBound, leftBound, rightBound);

	for(i=leftBound; i<=rightBound; i++)		// over all frames
  		for(j=0; j<FLength*4; j++)			// over all words
		{
			if(j>=topBound && j<=bottomBound)
			{
				k= getBufferByte(0,i,j);		// scan for used CLB-cell
		        fwrite(&k, 1, 1, PartialPtr);
			}
		}
	fclose(PartialPtr);
}

