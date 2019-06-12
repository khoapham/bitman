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
void setBufferByte(int BufferIndex, int Frame, int ByteIndex, int Value);
void change_LUT_content(int BufferIndex, int RscCl, int RscRw, int lut_pos, int lut_value_h, int lut_value_l);
void changeLutValueBit(int BufferIndex, int RscCl, int RscRw, int lut_pos, int bit_pos, int Bit);
int getBufferBytePartial(int Frame, int ByteIndex);
int getTBRA(int Row);
//unsigned int *inData, *outData;
//int ByteSwap(unsigned int *inData, unsigned int *outData, int inSize);
void ResetNCol();
int FillBuffer(FILE *ptr, int WordCount, int BufferIndex, bool isPartial);
void bufferHeaderAndFooter(FILE *in_ptr);
int WriteFullBitstream(char *FullBitstreamFileName, int BufferIndex);
int WriteFullBitstreamPartial(char *ModuleBitstreamFileName);
int WriteModuleBitstream(char *ModuleBitstreamFileName);
int WriteModuleBitstreamHeader(FILE *OutBitfilePtr);
int WriteModuleBitstreamCommand(FILE *OutBitfilePtr);
int WriteModuleBitstreamFooter(FILE *OutBitfilePtr);
int WriteModuleBitstreamData(FILE *OutBitfilePtr, int BA, int Offset, int Blocks, int FramesPerBlock);

int WritePartialBitstream(char *PartialBitstreamFileName);
int WritePartialBitstream_Z7(char *PartialBitstreamFileName);
int WritePartialBitstream_ZU(char *PartialBitstreamFileName);
int WritePartialBitstream_V6(char *PartialBitstreamFileName);
int WriteDebugPartialBitstream(char *PartialBitstreamFileName);

void scan(FILE *in_ptr, int BufferIndex);

void echoCLBs(int BufferIndex);
int echoRAMs(char mode, int BufferIndex);
void echoCLKs(int BufferIndex);

//void InitPartialMask(void);
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

int verbose;
int header;

int Speed;
int Delay;
int twoInputMode;
int WritePartial;
int ColBoundingBox1;
int ColBoundingBox2;
int RowBoundingBox1;
int RowBoundingBox2;
char *PartialFile;
int DumpBitstream;
int WriteFullBitstreamMode;
int WriteModuleBitstreamMode;
int DumpCLBs;
char *DumpBitstreamFileName;
char *FullBitstreamFileName;
char *ModuleBitstreamFileName;

char *PartialBitstreamFileName;

int LinkPitFileMode;
// global variables and functions' declarations for the Resource Replication
int RepRsc;
int MergeRsc;
int ReloRsc;
int DupliRsc;
int Readback = 0;

void ParseRepMergeRsc(char **argv, int i);
void ParseReloRsc(char **argv, int i);
void ParseReadback(char **argv, int i);

int RscCl1, RscRw1;
int RscCl2, RscRw2;
int RscCl3, RscRw3;
int lut_pos, lut_value_h, lut_value_l;
int RepClb(int BufferIndex, int Cl1, int Rw1, int Cl2, int Rw2, bool IsReplaced);
int RepBRAM(int BufferIndex, int Cl1, int Rw1, int Cl2, int Rw2, bool IsReplaced);
int duplicate_FPGA_region(int BufferIndex, int MJA1, int MJA2, int MJA3, bool IsReplaced);
int cut_FPGA_region(int MJA1, int Rw1, int MJA2, int Rw2, int MJA3, int Rw3);
int cut_FPGA_region_CLB(int MJA1, int Rw1, int MJA2, int Rw2, int MJA3, int Rw3);
int cut_FPGA_region_BRAM(int MJA1, int Rw1, int MJA2, int Rw2, int MJA3, int Rw3);
int NoOfFrames(int CLB_Index);

//int cutout_region[50000];
int *cutout_region;
int end_cutout;
int *cutout_region_BRAM;
int end_cutout_BRAM;

//void ParseMetaHeader(char **argv, int i);
//char *metadata;

struct LinkInformation {
	int PitCol;
	int PitRow;
	char *PitFileName;
};
struct LinkInformation LinkInformationList[20];

long int EndOfHeaderFilePosition;
long int SyncSequenceFilePosition;
char DirtyFrameMask[MaxFrames];

int WritePartialByMask;
char *PartialFileByMask;

int FLR;  				// frane length 
int userFLR;  			// frame length may be set by user (later)
//int deviceFLR = -1;			// frame length can be set by writing a device ID
int writeFLR; 			// frame length can be set by writing the Frame Length Register FLR

unsigned char InitialHeader[50000];
int InitialHeaderSize;
unsigned char InitialFooter[1000000];
int InitialFooterSize;
//short PartialMaskCLB[250][250];
//short PartialMaskRAM[250][250];
unsigned FrameBufferPartial[100000][150];	// this is the complete bitstream of a partial region
unsigned FrameBuffer[2][100000][150];	// this is the complete bitstream of a XC7Z010
int FrameBufferState[5000];         // -1 stream is empty  

unsigned ShadowCol[500];			// A configuration column is firstly written into a shadow reg...
int ShadowState;
int CLBState[2][10][250][60][80];	// denotes for each CLB if the entire frame contains some information
int DeviceID;

int ClbMaskActive;
int BRamMaskActive;
int BRamRoutingMaskActive;
int ClbLeft, ClbRight, ClbTop, ClbBottom;
int BRamLeft, BRamRight, BRamTop, BRamBottom;
int BRamRoutingLeft, BRamRoutingRight, BRamRoutingTop, BRamRoutingBottom;
unsigned char ModuleResourceString[200];

int readstate;

int ModufiedRSGs;
struct RsgUpdateInformation {
	int Col;
	int Row;
	unsigned Value;
	char type;
};

int BitManipulations;
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

int WriteNOOP(int times, FILE *OutBitfilePtr);
int WriteNoOfWords(int Words, bool AddFrame, FILE *OutBitfilePtr);
int WriteFAR(int FARValue, FILE *OutBitfilePtr);
int getFAR(int BA, int RscCl1, int RscRw1);
int WriteRemaniningHeader(FILE *OutBitfilePtr);
int WriteIDCODE(FILE *OutBitfilePtr);
int WriteCMDReg(int CMDValue, FILE *OutBitfilePtr);
int WriteCTL0Reg(int CTL0Value, FILE *OutBitfilePtr);
int WriteCOR0Reg(int COR0Value, FILE *OutBitfilePtr);
int WriteCTL1Reg(int CTL1Value, FILE *OutBitfilePtr);
int WriteMASKReg(int MASKValue, FILE *OutBitfilePtr);
int WriteSYNC(FILE *OutBitfilePtr);
int WriteV6BitfileHeader(FILE *OutBitfilePtr);
int WriteZUBitfileHeader(FILE *OutBitfilePtr);
int WriteZU3EGBitfileHeader_1_Slot(FILE *OutBitfilePtr);
int WriteZU3EGBitfileHeader_2_Slots(FILE *OutBitfilePtr);
int WriteZU3EGBitfileHeader_3_Slots(FILE *OutBitfilePtr);
int WriteZU9EGBitfileHeader_1_Slot(FILE *OutBitfilePtr);
int WriteZU9EGBitfileHeader_2_Slots(FILE *OutBitfilePtr);
int Write7SeriesPartialBitfileHeader(FILE *OutBitfilePtr);
int WriteZyboPartialBitfileHeader(FILE *OutBitfilePtr);
int WriteZedBoardPartialBitfileHeader(FILE *OutBitfilePtr);
int WriteZedBoardPartialBitfileHeaderBRAM(FILE *OutBitfilePtr);
int Write7SeriesCFGCLB(FILE *OutBitfilePtr);
void print_Lut_Bit_Offset();
