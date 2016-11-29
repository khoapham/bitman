int	CLKFrames=0;
int	IOBFrames=0;
int	IOIFrames=0;
int	CLBFrames=0;
int	RAMFrames=0;
int	RI_Frames=0;				// number of BRAM interconnect frames
int MFrames=0;
int DFrames=0;
int GFrames=0;
int HBytes = 0;
int	CLBBytesIO;				// amount of Bytes for the CLB-IO-blocks within each frame
int	CLBBytes=0;
int	NFrames = -1;
int FLength = -1;			// frame length can be set by writing a device ID
int NColCLB = -1;
int NColRAM = -1;
int NULLArea = 0;
int NULLArea1 = 0;
int NULLArea2 = 0;
int NULLArea3 = 0;
int RowFrames = 0;
int NRowRsc = 1;

int LutBitOffset[8][16];		// frame offset values for all LUTs Slice3,G downto Slice0,F and Bit F downto 0
int LutShiftUsageOffset[8][4];		// for detecting reconfigurable select generator usage; we check up to 4 bit; set to -1 if unused
char *ResourceString;
char *ResourceString2;
char *Device ="unknown";	// device identifier
char *Family ="unknown";	// FPGA device family

int BA = -1;   				// block address
int TB = -1;				// top/bottom bit
int RA = -1;				// row address
int MJA = -1;  				// major address
int MNA = -1;  				// minor address

int NColNULL=0;
int NColRI=0;
int NColBFG=0;
int NColM=0;
int NColD=0;

struct DeviceParameters {
	int NFrames;
	int FLength;
	int NColCLB;
	int NColRAM;
	int NRowRsc;
	char *ResourceString;
	char *ResourceString2;
	char *Family;
	int CLKFrames;
	int	IOBFrames;
	int	IOIFrames;
	int	CLBFrames;	// CLBL frames
	int	RAMFrames;	// Block RAM frames
	int	RI_Frames;	// Block RAM interconnect frames
	int MFrames;	// CLBM frames
	int DFrames;	// DSP frames
	int GFrames;	// BUFG frames
	int HBytes;	// RCLK frames
	int	CLBBytesIO;
	int	CLBBytes;
	int LutBitOffset[8][16];
	int LutShiftUsageOffset[8][4];
	int NULLArea;
	int NULLArea1;
	int NULLArea2;
	int NULLArea3;
	int RowFrames;
};

//	struct DeviceParameters *GlobalParameters;

int getGlobalDeviceInformation(int DeviceID);
int SetGlobalDeviceParameters(char *Device);
int SetDeviceParameters(char *Device, struct DeviceParameters *Parameter);
int getFrameIndexGlobal(void);
int getFrameIndex(int BA, int MJA, int MNA, int TB_RA);	// TB_RA = TB + RA
int incFrameAddress(void);
int getROWByteIndex(int CLB_ROWindex);
//int echoSupportedDevices(void);
int FrameADR2BlockADR(int FrameADR, int *BA, int *MJA, int *MNA);

int getDeviceInformation(int DeviceID)

{

//	fprintf(stderr,"this device ID: 0x%08X.\n",DeviceID);
	switch(DeviceID)
	{
// Virtex-6 family
		case 0x04250093:	Device="XC6VLX240T";		break;
// Artix-7 family
		case 0x0362D093:	Device="XC7A15T";			break;
		case 0x0362C093:	Device="XC7A50T";			break;
		case 0x03622093:	Device="XC7A75T";			break;
		case 0x03621093:	Device="XC7A100T";			break;
		case 0x03626093:	Device="XC7A200T";			break;
// Kintex-7 family
		case 0x03647093:	Device="XC7K70T";			break;
		case 0x0364C093:	Device="XC7K160T";			break;
		case 0x03651093:	Device="XC7K325T";			break;
		case 0x03747093:	Device="XC7K355T";			break;
		case 0x03656093:	Device="XC7K410T";			break;
		case 0x03752093:	Device="XC7K420T";			break;
		case 0x03751093:	Device="XC7K480T";			break;
// Virtex-7 family
		case 0x03671093:	Device="XC7V585T";			break;
		case 0x03667093:	Device="XC7VX330T";			break;
		case 0x03682093:	Device="XC7VX415T";			break;
		case 0x03687093:	Device="XC7VX485T";			break;
		case 0x03692093:	Device="XC7VX550T";			break;
		case 0x03691093:	Device="XC7VX690T";			break;
		case 0x03696093:	Device="XC7VX980T";			break;
		case 0x036D5093:	Device="XC7VX1140T";			break;
		case 0x036D9093:	Device="XC7VH580T";			break;
		case 0x036DB093:	Device="XC7VH870T";			break;
// Zynq-7000 family
		case 0x03722093:	Device="XC7Z010";			break;
		case 0x03727093:	Device="XC7Z020";			break;
// Kintex UltraScale family
		case 0x03824093:	Device="KU025";				break;
		case 0x03823093:	Device="KU035";				break;

		default :			fprintf(stderr,"Sorry, can not handle this device ID: 0x%08X.\n",DeviceID);
							Device="unknown";
	}
//	fprintf(out_ptr,"%s\n",Device);
//	printf("%s\n",Device);
	return(0);
}
int SetDeviceParameters(char *Device, struct DeviceParameters *Parameter)
{
	if	(strcmp(Device,"XC6VLX240T") == 0)		{Parameter->NFrames=28488;	Parameter->FLength=81;	Parameter->NColCLB=80;	Parameter->NColRAM=8;	Parameter->ResourceString="NMLMLRMMDMMMMDMMRMMMMMMMMRMMDMMMMDMMRMLMLNMLMLRMMDMMMMDMMRMMMMMMMMRMMDMMMMDMMRMLML";
	Parameter->NULLArea1=44;
	Parameter->NULLArea2=630;
	Parameter->NRowRsc=6;
	Parameter->RowFrames=3594;
	}
	else if	(strcmp(Device,"XC7Z010") == 0)		{Parameter->NFrames=5152;	Parameter->FLength=101;	Parameter->NColCLB=28;	Parameter->NColRAM=3;	Parameter->ResourceString="NCCCRCCRCCNCCCCBCCRCCCRCCRCCCC";
	Parameter->NULLArea1=652;
	Parameter->NULLArea2=246;
	Parameter->NRowRsc=2;
	// (22*36 + 5*28 + 1*30=962) resource frames per row + 652 frames for PCIE_NULL + 246 frames for ICAP + 76 frames for IO = 1934
	Parameter->RowFrames=1934;}
	else if	(strcmp(Device,"XC7Z020") == 0)		{Parameter->NFrames=10008;	Parameter->FLength=101;	Parameter->NColCLB=46;	Parameter->NColRAM=4;	Parameter->ResourceString="NCCCRCCRCCCCCCCBCRCCCCCCCNCCCCCRCCRCCCCRCCRCCCC";
	Parameter->NULLArea1=652;
	Parameter->NULLArea2=282;
	Parameter->NRowRsc=3;
	Parameter->RowFrames=2566;
	Parameter->ResourceString2="NCCCCRCCRCCCCRCCRCCCCRCCRCCCCCCCBCCRCCCCCCCCCCCCCRCCCCCRCCRCCCCRCCRCCCC";
	Parameter->NULLArea=72;}
	else if	(strcmp(Device,"KU025") == 0)		{Parameter->NFrames=32530;	Parameter->FLength=123;	Parameter->NColCLB=130;	Parameter->NColRAM=0;
	Parameter->ResourceString="NLMLRLMDMLMLRLMDMLMLMLMDMLMLRLMDBMLMLRLMDMLMLRLMDMLMLMLMDMLMLRLMDBNLMLMDMLMLMDMLRLMDMLMLMDMLMLMDMLRLMDBMLMDMLMDMLMLRLLNLMLRLMLMLMLMLM";
	Parameter->NULLArea1=84;
	Parameter->NULLArea2=84;
	Parameter->NULLArea3=124;
	Parameter->NRowRsc=3;	// temporarily set the NRowRsc=1
	Parameter->RowFrames=5224;
	}

	else	{	fprintf(stderr,"Warning: Device '%s' not supported\n",Device);	
				return(-1);
			}

	if(strncmp(Device,"XC6V",4)==0) {
		Parameter->Family="Virtex 6";
		fprintf(stderr,"found a Virtex 6 device\n");

		Parameter->CLBFrames=36;
		Parameter->CLBBytes=8;
		Parameter->RAMFrames=128;	// 28
		Parameter->CLBBytesIO=0;
		Parameter->CLKFrames=0;		// temporarily set to 0
		Parameter->IOBFrames=0;
		Parameter->IOIFrames=0;
		Parameter->RI_Frames=28;
		Parameter->MFrames=36;
		Parameter->DFrames=28;
		Parameter->GFrames=30;
		Parameter->HBytes=1;
	}

	if(strncmp(Device,"XC7Z",4)==0) {
		Parameter->Family="Zynq 7000";
		fprintf(stderr,"found a Zynq 7000 device\n");

		Parameter->CLBFrames=36;
		Parameter->CLBBytes=8;
		Parameter->RAMFrames=128;	// 28
		Parameter->CLBBytesIO=0;
		Parameter->CLKFrames=0;		// temporarily set to 0
		Parameter->IOBFrames=0;
		Parameter->IOIFrames=0;
		Parameter->RI_Frames=28;
		Parameter->GFrames=30;
		Parameter->HBytes=1;
	}

	if(strncmp(Device,"KU0",3)==0) {
		Parameter->Family="Kintex Ultrascale";
		fprintf(stderr,"found a Kintex Ultrascale device\n");

		Parameter->CLBFrames=12;
		Parameter->CLBBytes=8;
		Parameter->RAMFrames=128;	// 28
		Parameter->CLBBytesIO=0;
		Parameter->CLKFrames=0;		// temporarily set to 0
		Parameter->IOBFrames=0;
		Parameter->IOIFrames=0;
		Parameter->RI_Frames=62;
		Parameter->MFrames=70;
		Parameter->DFrames=4;
		Parameter->GFrames=2;
		Parameter->HBytes=3;
	}
	return(0);
}

int SetGlobalDeviceParameters(char *Device)
{
	struct DeviceParameters GlobalParameters;
	int returnFromSetDeviceParameters;
	returnFromSetDeviceParameters = SetDeviceParameters(Device, &GlobalParameters);

	if (returnFromSetDeviceParameters < 0)
		return (-1);
	NFrames = GlobalParameters.NFrames;
	FLength = GlobalParameters.FLength;
	NColCLB = GlobalParameters.NColCLB;
	NColRAM = GlobalParameters.NColRAM;
	NRowRsc = GlobalParameters.NRowRsc;
	ResourceString = GlobalParameters.ResourceString;
	ResourceString2= GlobalParameters.ResourceString2;
	Family = GlobalParameters.Family;
	CLKFrames = GlobalParameters.CLKFrames;
	IOBFrames = GlobalParameters.IOBFrames;
	IOIFrames = GlobalParameters.IOIFrames;
	CLBFrames = GlobalParameters.CLBFrames;
	RAMFrames = GlobalParameters.RAMFrames;
	RI_Frames = GlobalParameters.RI_Frames;
	MFrames = GlobalParameters.MFrames;
	DFrames = GlobalParameters.DFrames;
	GFrames = GlobalParameters.GFrames;
	HBytes = GlobalParameters.HBytes;
	CLBBytesIO = GlobalParameters.CLBBytesIO;
	CLBBytes = GlobalParameters.CLBBytes;
	NULLArea = GlobalParameters.NULLArea;
	NULLArea1 = GlobalParameters.NULLArea1;
	NULLArea2 = GlobalParameters.NULLArea2;
	NULLArea3 = GlobalParameters.NULLArea3;
	RowFrames = GlobalParameters.RowFrames;

	return(0);
}
int getFrameIndexGlobal(void)
{
	int FrameOffset = 0;
	if (BA == 0) {
		if (NColNULL == 1)
			FrameOffset = NULLArea1;
		else if (NColNULL == 2)
			FrameOffset = NULLArea1 + NULLArea2;
		else if (NColNULL == 3)
			FrameOffset = NULLArea1 + NULLArea2 + NULLArea3;

		if	((strncmp(Device,"XC7Z",4)==0) && (RA > 0))
			FrameOffset = NULLArea;
		return (MNA + NColRI*RI_Frames + NColBFG*GFrames + NColM*MFrames + NColD*DFrames + (MJA - (NColRI + NColBFG + NColM + NColD))*CLBFrames + FrameOffset + (TB + RA)*RowFrames);
	}
	else if (BA == 1)
		// 2*1934 for 2 CLB/interconnect resources + 256 for unknown resources = 4124
		// 128 frames per column per row * 3 columns + 259 frames for unknown devices
		return (MNA + MJA*RAMFrames + 4124 + TB*643);
	fprintf(stderr,"No valid base address - trying to continue.\n");
	return(-1);
}

int getFrameIndex(int BA, int MJA, int MNA, int TB_RA)
{
	int FrameOffset = 0;
	int tmp;
	int tmpNColNULL=0, tmpNColRI=0, tmpNColBFG=0, tmpNColM=0, tmpNColD=0;
	if (BA == 0) {
// Scan ResourceString for how many columns of different resource types
		for (tmp=0; tmp<=MJA; tmp++) {
			if (ResourceString[tmp + tmpNColNULL] == 'N')
				tmpNColNULL++;
			if (tmp < MJA) {
				if (ResourceString[tmp + tmpNColNULL] == 'R')
					tmpNColRI++;
				else if (ResourceString[tmp + tmpNColNULL] == 'B')
					tmpNColBFG++;
				else if (ResourceString[tmp + tmpNColNULL] == 'M')
					tmpNColM++;
				else if (ResourceString[tmp + tmpNColNULL] == 'D')
					tmpNColD++;
			}
		}

		if (tmpNColNULL == 1)
			FrameOffset = NULLArea1;
		else if (tmpNColNULL == 2)
			FrameOffset = NULLArea1 + NULLArea2;
		else if (NColNULL == 3)
			FrameOffset = NULLArea1 + NULLArea2 + NULLArea3;

		if	((strncmp(Device,"XC7Z",4)==0) && (TB_RA > 1))
			FrameOffset = NULLArea;

		return (MNA + tmpNColRI*RI_Frames + tmpNColBFG*GFrames + tmpNColM*MFrames + tmpNColD*DFrames + (MJA - (tmpNColRI + tmpNColBFG + tmpNColM + tmpNColD))*CLBFrames + FrameOffset + TB_RA*RowFrames);
	}
	else if (BA == 1)
		// 128 frames per column per row * 3 columns + 259 frames for unknown devices
		return (MNA + MJA*RAMFrames + 4124 + TB_RA*643);
	fprintf(stderr,"No valid base address - trying to continue.\n");
	return(-1);
}

int incFrameAddress(void)
{
	if((BA==-1) || (MJA==-1) || (MNA==-1))
	{
		fprintf(stderr,"Base frame address not found - cannot compute frame address.\n");
		return(-1);
	}
	else if (BA == 0)	// in the CLB, I/O, CLK, interconnects section
	{
//		printf("MJA %d NColNULL %d RSC %c \n", MJA, NColNULL, ResourceString[MJA + NColNULL]);
		if ((ResourceString[MJA + NColNULL] == 'C')
			|| (ResourceString[MJA + NColNULL] == 'L'))
			if (MNA < (CLBFrames - 1))
				MNA++;
			else {
				MJA++;
				MNA = 0;
			}
//			printf("Frame Number %d\n", CLBFrames);
		else if (ResourceString[MJA + NColNULL] == 'R')
			if (MNA < (RI_Frames - 1))	// BRAM/DSP column has 28 frames for interconnect
				MNA++;
			else {
				MJA++;
				MNA = 0;
				NColRI++;
			}
//			printf("Frame Number %d\n", RI_Frames);
		else if (ResourceString[MJA + NColNULL] == 'B')
			if (MNA < (GFrames - 1))	// 30 frames for unknown resources when passes clock region boundary
				MNA++;
			else {
				MJA++;
				MNA = 0;
				NColBFG++;
			}
		else if (ResourceString[MJA + NColNULL] == 'M')
			if (MNA < (MFrames - 1))	// CLBM column has 70 frames
				MNA++;
			else {
				MJA++;
				MNA = 0;
				NColM++;
//				printf("Frame Number of M %d\n", MFrames);
			}
//			printf("Frame Number of M %d\n", MFrames);
		else if (ResourceString[MJA + NColNULL] == 'D')
			if (MNA < (DFrames - 1))	// BRAM/DSP column has 28 frames for interconnect
				MNA++;
			else {
				MJA++;
				MNA = 0;
				NColD++;
			}
		return(0);
	}
	else if (BA == 1)
		if (MNA < (RAMFrames - 1))
			MNA++;
		else {
			MJA++;
			MNA = 0;
		}
	else
	{
		fprintf(stderr,"Internal error: invalid frame address\n");
		exit(13);
	}
}
int getROWByteIndex(int CLB_ROWindex)
{
	if(CLB_ROWindex==0)
		return(0);
	else
		return(CLBBytesIO + (CLB_ROWindex-1)*CLBBytes);
}