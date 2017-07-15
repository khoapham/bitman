#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "devices.h"

int	CLKFrames;
int	IOBFrames;
int	IOIFrames;
int	CLBFrames;
int	RAMFrames;
int	RI_Frames;				// number of BRAM interconnect frames
int MFrames;
int DFrames;
int GFrames;
int SwFrames;
int PFrames;
int IFrames;
int mFrames;
int FFrames;
int VFrames;
int HWords;
int	CLBBytesIO;				// amount of Bytes for the CLB-IO-blocks within each frame
int	CLBBytes;
int	NFrames;
int FLength;			// frame length can be set by writing a device ID
int NColCLB;
int NColRAM;
int NULLArea;
int NULLArea1;
int NULLArea2;
int NULLArea3;
int RowFrames;
int RowIOPad;
int RowFrames2;
int RAMRowOff;
int RAMRowFrames;
int NRowRsc;
int LutBitOffset[8][16];		// frame offset values for all LUTs Slice3,G downto Slice0,F and Bit F downto 0
int LutShiftUsageOffset[8][4];		// for detecting reconfigurable select generator usage; we check up to 4 bit; set to -1 if unused
char *ResourceString;
char *ResourceString2;
char *Device;	// device identifier
char *Family;	// FPGA device family

int BA;   				// block address
int TB;				// top/bottom bit
int RA;				// row address
int MJA;  				// major address
int MNA;  				// minor address

int NColNULL;
int NColRI;
int NColBFG;
int NColM;
int NColD;
int NColS;
int NColP;
int NColI;
int NColm;
int NColF;
int NColV;
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
	int SwFrames;	// Switch matrix frames
	int PFrames;	// Primitive frames
	int IFrames;	// IO frames
	int mFrames;	// interconnect matrix frames
	int FFrames;	// FIFO out frames
	int VFrames;	// VFrame frames
	int HWords;	// RCLK frames
	int	CLBBytesIO;
	int	CLBBytes;
	int LutBitOffset[8][16];
	int LutShiftUsageOffset[8][4];
	int NULLArea;
	int NULLArea1;
	int NULLArea2;
	int NULLArea3;
	int RowFrames;
	int RowIOPad;
	int RowFrames2;
	int RAMRowOff;
	int RAMRowFrames;
};

int getDeviceInformation(int DeviceID)
{
	switch(DeviceID)
	{
		case 0x01008093:	Device="XC2V40";			break;
		case 0x01010093:	Device="XC2V80";			break;
		case 0x01018093:	Device="XC2V250";			break;
		case 0x01020093:	Device="XC2V500";			break;
		case 0x01028093:	Device="XC2V1000";			break;
		case 0x01030093:	Device="XC2V1500";			break;
		case 0x01038093:	Device="XC2V2000";			break;
		case 0x01040093:	Device="XC2V3000";			break;
		case 0x01050093:	Device="XC2V4000";			break;
		case 0x01060093:	Device="XC2V6000";			break;
		case 0x01070093:	Device="XC2V8000";			break;
		
		case 0x01226093:	Device="XC2VP2";			break;
		case 0x0123E093:	Device="XC2VP4";			break;
		case 0x0124A093:	Device="XC2VP7";			break;
		case 0x01266093:	Device="XC2VP20";			break;
		case 0x0127E093:	Device="XC2VP30";			break;
		case 0x01292093:	Device="XC2VP40";			break;
		case 0x0129E093:	Device="XC2VP50";			break;
		case 0x012BA093:	Device="XC2VP70";			break;
		case 0x012D6093:	Device="XC2VP100";			break;

		case 0x0140D093:	Device="XC3S50";			break;
		case 0x01414093:	Device="XC3S200";			break;
		case 0x0141C093:	Device="XC3S400";			break;
		case 0x11428093:	Device="XC3S1000";			break;
		case 0x01434093:	Device="XC3S1500";			break;
		case 0x01440093:	Device="XC3S2000";			break;
		case 0x01448093:	Device="XC3S4000";			break;
		case 0x01450093:	Device="XC3S5000";			break;

		case 0x01C10093:	Device="XC3S100E";			break;
		case 0x01C1A093:	Device="XC3S250E";			break;
		case 0x01C22093:	Device="XC3S500E";			break;
		case 0x01C2E093:	Device="XC3S1200E";			break;
		case 0x01C3A093:	Device="XC3S1600E";			break;

		case 0x04250093:	Device="XC6VLX240T";		break;
// Artix-7 family
		case 0x0362D093:	Device="XC7A35T";			break;
		case 0x0362C093:	Device="XC7A50T";			break;
		case 0x03622093:	Device="XC7A75T";			break;
		case 0x03631093:	Device="XC7A100T";			break;
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
// Zynq UltraScale+ family
		case 0x0484A093:	Device="XCZU9EG";			break;
		default :		printf("Sorry, can not handle this device ID: 0x%08X.\n",DeviceID);
					Device="unknown";
	}
	printf("%s\n",Device);
	return(0);
}

int SetDeviceParameters(char *Device, struct DeviceParameters *Parameter)
{
	if		(strcmp(Device,"XC2V40") == 0)		{Parameter->NFrames=404;	Parameter->FLength=26;	Parameter->NColCLB=8;	Parameter->NColRAM=2;	Parameter->ResourceString="CCRCCCCRCC";}
	else if	(strcmp(Device,"XC2V80") == 0)		{Parameter->NFrames=404;	Parameter->FLength=46;	Parameter->NColCLB=8;	Parameter->NColRAM=2;	Parameter->ResourceString="CCRCCCCRCC";}
	else if	(strcmp(Device,"XC2V250") == 0)		{Parameter->NFrames=752;	Parameter->FLength=66;	Parameter->NColCLB=16;	Parameter->NColRAM=4;	Parameter->ResourceString="CCRCCCCRCCCCRCCCCRCC";}
	else if	(strcmp(Device,"XC2V500") == 0)		{Parameter->NFrames=928;	Parameter->FLength=86;	Parameter->NColCLB=24;	Parameter->NColRAM=4;	Parameter->ResourceString="CCRCCCCCCCCRCCCCRCCCCCCCCRCC";}
	else if	(strcmp(Device,"XC2V1000") == 0)	{Parameter->NFrames=1104;	Parameter->FLength=106;	Parameter->NColCLB=32;	Parameter->NColRAM=4;	Parameter->ResourceString="CCRCCCCCCCCCCCCRCCCCRCCCCCCCCCCCCRCC";}
	else if	(strcmp(Device,"XC2V1500") == 0)	{Parameter->NFrames=1280;	Parameter->FLength=126;	Parameter->NColCLB=40;	Parameter->NColRAM=4;	Parameter->ResourceString="CCRCCCCCCCCCCCCCCCCRCCCCRCCCCCCCCCCCCCCCCRCC";}
	else if	(strcmp(Device,"XC2V2000") == 0)	{Parameter->NFrames=1456;	Parameter->FLength=146;	Parameter->NColCLB=48;	Parameter->NColRAM=4;	Parameter->ResourceString="CCRCCCCCCCCCCCCCCCCCCCCRCCCCRCCCCCCCCCCCCCCCCCCCCRCC";}
	else if	(strcmp(Device,"XC2V3000") == 0)	{Parameter->NFrames=1804;	Parameter->FLength=166;	Parameter->NColCLB=56;	Parameter->NColRAM=6;	Parameter->ResourceString="CCRCCCCCCCCCCCCRCCCCCCCCCCCCRCCCCRCCCCCCCCCCCCRCCCCCCCCCCCCRCC";}
	else if	(strcmp(Device,"XC2V4000") == 0)	{Parameter->NFrames=2156;	Parameter->FLength=206;	Parameter->NColCLB=72;	Parameter->NColRAM=6;	Parameter->ResourceString="CCRCCCCCCCCCCCCCCCCRCCCCCCCCCCCCCCCCRCCCCRCCCCCCCCCCCCCCCCRCCCCCCCCCCCCCCCCRCC";}
	else if	(strcmp(Device,"XC2V6000") == 0)	{Parameter->NFrames=2508;	Parameter->FLength=246;	Parameter->NColCLB=88;	Parameter->NColRAM=6;	Parameter->ResourceString="CCRCCCCCCCCCCCCCCCCCCCCRCCCCCCCCCCCCCCCCCCCCRCCCCRCCCCCCCCCCCCCCCCCCCCRCCCCCCCCCCCCCCCCCCCCRCC";}
	else if	(strcmp(Device,"XC2V8000") == 0)	{Parameter->NFrames=2860;	Parameter->FLength=286;	Parameter->NColCLB=104;	Parameter->NColRAM=6;	Parameter->ResourceString="CCRCCCCCCCCCCCCCCCCCCCCCCCCRCCCCCCCCCCCCCCCCCCCCCCCCRCCCCRCCCCCCCCCCCCCCCCCCCCCCCCRCCCCCCCCCCCCCCCCCCCCCCCCRCC";}

	else if	(strcmp(Device,"XC2VP2") == 0)		{Parameter->NFrames=884;	Parameter->FLength=46;	Parameter->NColCLB=22;	Parameter->NColRAM=4;	Parameter->ResourceString="CCRCCCCCCRCCCCCCRCCCCCCRCC";}
	else if	(strcmp(Device,"XC2VP4") == 0)		{Parameter->NFrames=884;	Parameter->FLength=106;	Parameter->NColCLB=22;	Parameter->NColRAM=4;	Parameter->ResourceString="CCRCCCCCCRCCCCCCRCCCCCCRCC";}
	else if	(strcmp(Device,"XC2VP7") == 0)		{Parameter->NFrames=1320;	Parameter->FLength=106;	Parameter->NColCLB=34;	Parameter->NColRAM=6;	Parameter->ResourceString="CCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCC";}
	else if	(strcmp(Device,"XC2VP20") == 0)		{Parameter->NFrames=1756;	Parameter->FLength=146;	Parameter->NColCLB=46;	Parameter->NColRAM=8;	Parameter->ResourceString="CCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCC";}
	else if	(strcmp(Device,"XC2VPX20") == 0)	{Parameter->NFrames=1756;	Parameter->FLength=146;	Parameter->NColCLB=46;	Parameter->NColRAM=8;	Parameter->ResourceString="CCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCC";}
	else if	(strcmp(Device,"XC2VP30") == 0)		{Parameter->NFrames=1756;	Parameter->FLength=206;	Parameter->NColCLB=46;	Parameter->NColRAM=8;	Parameter->ResourceString="CCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCC";}
	else if	(strcmp(Device,"XC2VP40") == 0)		{Parameter->NFrames=2192;	Parameter->FLength=226;	Parameter->NColCLB=58;	Parameter->NColRAM=10;	Parameter->ResourceString="CCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCC";}
	else if	(strcmp(Device,"XC2VP50") == 0)		{Parameter->NFrames=2628;	Parameter->FLength=226;	Parameter->NColCLB=70;	Parameter->NColRAM=12;	Parameter->ResourceString="CCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCC";}
	else if	(strcmp(Device,"XC2VP70") == 0)		{Parameter->NFrames=3064;	Parameter->FLength=266;	Parameter->NColCLB=82;	Parameter->NColRAM=14;	Parameter->ResourceString="CCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCC";}
	else if	(strcmp(Device,"XC2VPX70") == 0)	{Parameter->NFrames=3064;	Parameter->FLength=266;	Parameter->NColCLB=82;	Parameter->NColRAM=14;	Parameter->ResourceString="CCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCC";}
	else if	(strcmp(Device,"XC2VP100") == 0)	{Parameter->NFrames=3500;	Parameter->FLength=306;	Parameter->NColCLB=94;	Parameter->NColRAM=16;	Parameter->ResourceString="CCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCCCCCCRCC";}

	else if	(strcmp(Device,"XC3S50") == 0)		{Parameter->NFrames=368;	Parameter->FLength=37;	Parameter->NColCLB=12;	Parameter->NColRAM=1;	Parameter->ResourceString="CCRCCCCCCCCCC";}
	else if	(strcmp(Device,"XC3S200") == 0)		{Parameter->NFrames=615;	Parameter->FLength=53;	Parameter->NColCLB=20;	Parameter->NColRAM=2;	Parameter->ResourceString="CCRCCCCCCCCCCCCCCCCRCC";}
	else if	(strcmp(Device,"XC3S400") == 0)		{Parameter->NFrames=767;	Parameter->FLength=69;	Parameter->NColCLB=28;	Parameter->NColRAM=2;	Parameter->ResourceString="CCRCCCCCCCCCCCCCCCCCCCCCCCCRCC";}
	else if	(strcmp(Device,"XC3S1000") == 0)	{Parameter->NFrames=995;	Parameter->FLength=101;	Parameter->NColCLB=40;	Parameter->NColRAM=2;	Parameter->ResourceString="CCRCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCRCC";}
	else if	(strcmp(Device,"XC3S1500") == 0)	{Parameter->NFrames=1223;	Parameter->FLength=137;	Parameter->NColCLB=52;	Parameter->NColRAM=2;	Parameter->ResourceString="CCRCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCRCC";}
	else if	(strcmp(Device,"XC3S2000") == 0)	{Parameter->NFrames=1451;	Parameter->FLength=165;	Parameter->NColCLB=64;	Parameter->NColRAM=2;	Parameter->ResourceString="CCRCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCRCC";}
	else if	(strcmp(Device,"XC3S4000") == 0)	{Parameter->NFrames=1793;	Parameter->FLength=197;	Parameter->NColCLB=72;	Parameter->NColRAM=4;	Parameter->ResourceString="CCRCCCCCCCCCCCCCCCCCCCCCCRCCCCCCCCCCCCCCCCCCCCCCCCRCCCCCCCCCCCCCCCCCCCCCCRCC";}
	else if	(strcmp(Device,"XC3S5000") == 0)	{Parameter->NFrames=1945;	Parameter->FLength=213;	Parameter->NColCLB=80;	Parameter->NColRAM=4;	Parameter->ResourceString="CCRCCCCCCCCCCCCCCCCCCCCCCCCRCCCCCCCCCCCCCCCCCCCCCCCCCCCCRCCCCCCCCCCCCCCCCCCCCCCCCRCC";}

	else if	(strcmp(Device,"XC3S100E") == 0)	{Parameter->NFrames=368;	Parameter->FLength=196;	Parameter->NColCLB=16;	Parameter->NColRAM=0;	Parameter->ResourceString="CCCCCCCCCCCCCCCC";}
	else if	(strcmp(Device,"XC3S250E") == 0)	{Parameter->NFrames=577;	Parameter->FLength=292;	Parameter->NColCLB=26;	Parameter->NColRAM=0;	Parameter->ResourceString="CCCCCCCCCCCCCCCCCCCCCCCCCC";}
	else if	(strcmp(Device,"XC3S500E") == 0)	{Parameter->NFrames=729;	Parameter->FLength=388;	Parameter->NColCLB=34;	Parameter->NColRAM=0;	Parameter->ResourceString="CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC";}
	else if	(strcmp(Device,"XC3S1200E") == 0)	{Parameter->NFrames=958;	Parameter->FLength=500;	Parameter->NColCLB=46;	Parameter->NColRAM=0;	Parameter->ResourceString="CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC";}
	else if	(strcmp(Device,"XC3S1600E") == 0)	{Parameter->NFrames=1186;	Parameter->FLength=628;	Parameter->NColCLB=58;	Parameter->NColRAM=0;	Parameter->ResourceString="CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC";}

	else if	(strcmp(Device,"XC6VLX240T") == 0)	{Parameter->NFrames=28488;	Parameter->FLength=81;	Parameter->NColCLB=80;	Parameter->NColRAM=8;	Parameter->ResourceString="NMLMLRMMDMMMMDMMRMMMMMMMMRMMDMMMMDMMRMLMLNMLMLRMMDMMMMDMMRMMMMMMMMRMMDMMMMDMMRMLML";
	Parameter->NULLArea1=44;
	Parameter->NULLArea2=630;
	Parameter->NRowRsc=6;
	Parameter->RowFrames=3594;
	Parameter->RowIOPad=48;
//	Parameter->ResourceString2="NCCCCRCCRCCCCRCCRCCCCRCCRCCCCCCCBCCRCCCCCCCCCCCCCRCCCCCRCCRCCCCRCCRCCCC";
//	Parameter->NULLArea=44;
	}
	else if	(strcmp(Device,"XC7A35T") == 0)		{Parameter->NFrames=5420;	Parameter->FLength=101;	Parameter->NColCLB=32;	Parameter->NColRAM=3;
	Parameter->ResourceString="NLMLMRMMDMMNLMLMBLMLMLMRLMMDMMRLMLM";
	Parameter->NULLArea1=73;
	Parameter->NULLArea2=246;	//282;
	//Parameter->NULLArea3=124;
	Parameter->NRowRsc=3;
	Parameter->RowFrames=1534;
	Parameter->RowIOPad=72;
	Parameter->RowFrames2=1322;
	}
	else if	(strcmp(Device,"XC7Z010") == 0)		{Parameter->NFrames=5152;	Parameter->FLength=101;	Parameter->NColCLB=28;	Parameter->NColRAM=3;	Parameter->ResourceString="NCCCRCCRCCNCCCCBCCRCCCRCCRCCCC";
	Parameter->NULLArea1=653;
	Parameter->NULLArea2=246;
	Parameter->NRowRsc=2;
	Parameter->RowFrames=1934;
	Parameter->RowIOPad=73;
	Parameter->RowFrames2=0;
	Parameter->RAMRowOff=4124;
	Parameter->RAMRowFrames=643;}
	else if	(strcmp(Device,"XC7Z020") == 0)		{Parameter->NFrames=10008;	Parameter->FLength=101;	Parameter->NColCLB=46;	Parameter->NColRAM=4;	Parameter->ResourceString="NCCCRCCRCCCCCCCBCRCCCCCCCNCCCCCRCCRCCCCRCCRCCCC";
	Parameter->NULLArea1=653;
	Parameter->NULLArea2=282;
	Parameter->NRowRsc=3;
	Parameter->RowFrames=2566;
	Parameter->RowIOPad=72;
	Parameter->ResourceString2="NCCCCRCCRCCCCRCCRCCCCRCCRCCCCCCCBCCRCCCCCCCCCCCCCRCCCCCRCCRCCCCRCCRCCCC";
	Parameter->NULLArea=72;}
	else if	(strcmp(Device,"XC7VX690T") == 0)		{Parameter->NFrames=71120;	Parameter->FLength=101;	Parameter->NColCLB=151;	Parameter->NColRAM=15;
	Parameter->ResourceString="ILMLMRMMDMMRLMLMMMLRMMDMMLMMDMMRLMPFMMDMMRMMDMMMMDMMRLLLLLLLVLMLMRMMDMMBMMDMMRMMDMMMMDMMRMMDMMMMDMMRMMDMMMMDMMRMMDMMFPLMRMMDMMLMMDMMRMLMLMLMRMMDMMRLMLM";
	Parameter->NULLArea1=0;
	Parameter->NULLArea2=0;
	Parameter->NULLArea3=0;
	Parameter->NRowRsc=10;
	Parameter->RowFrames=5190;
	Parameter->RowIOPad=33;
	Parameter->RowFrames2=0;
	Parameter->RAMRowOff=51900;
	Parameter->RAMRowFrames=1922;
	}
	else if	(strcmp(Device,"KU025") == 0)		{Parameter->NFrames=32530;	Parameter->FLength=123;	Parameter->NColCLB=130;	Parameter->NColRAM=10;
	Parameter->ResourceString="NLMLRLMDMLMLRLMDMLMLMLMDMLMLRLMDBMLMLRLMDMLMLRLMDMLMLMLMDMLMLRLMDBNLMLMDMLMLMDMLRLMDMLMLMDMLMLMDMLRLMDBMLMDMLMDMLMLRLLNLMLRLMLMLMLMLM";
	Parameter->NULLArea1=84;
	Parameter->NULLArea2=84;
	Parameter->NULLArea3=124;
	Parameter->NRowRsc=3;
	Parameter->RowFrames=5224;
	Parameter->RowIOPad=48;
	}
	else if	(strcmp(Device,"XCZU9EG") == 0)		{Parameter->NFrames=71260;	Parameter->FLength=93;	Parameter->NColCLB=199;	Parameter->NColRAM=13;
	Parameter->ResourceString="ImsLMsLRsLMsDMsLMsLRsLMsDMsLMsDMsLMsLRsLMsDMsLMsLRsLMsDMsLMsDMsLMsLRsLMsDMsLMsLRsLMsDMsLMsDMsLMsLRsLMsDMsLMsDMsLMsDMsLMsLRsLMsDMsLMsLRsLMsDMsLMsLRsLMsDMsLMsDMsLMsLRsLMsLRsLMsmPmsLMsDMsLMsDMsLMsLRsLMsN";
	Parameter->NULLArea1=0;
	Parameter->NULLArea2=0;
	Parameter->NULLArea3=0;
	Parameter->NRowRsc=7;
	Parameter->RowFrames=6850;
	Parameter->RowIOPad=29;
	Parameter->RowFrames2=0;
	Parameter->RAMRowOff=47950;
	Parameter->RAMRowFrames=3330;
	}
	else	{	printf("Warning: Device '%s' not supported\n",Device);
				return(-1);
			}
	
	if(strncmp(Device,"XC2V",4)==0)
	{
		Parameter->Family="Virtex2";

		printf("found a VirtexII or a VirtexIIPro device\n");
		Parameter->CLKFrames=4;
		Parameter->IOBFrames=4;
		Parameter->IOIFrames=22;
		Parameter->CLBFrames=22;
		Parameter->RAMFrames=64;
		Parameter->RI_Frames=22;
		Parameter->CLBBytesIO=12;
		Parameter->CLBBytes=10;
		Parameter->LutBitOffset[1][0] = 128;  Parameter->LutBitOffset[3][0] = 88 ;  Parameter->LutBitOffset[5][0] = 208;  Parameter->LutBitOffset[7][0] = 168;  
		Parameter->LutBitOffset[1][1] = 129;  Parameter->LutBitOffset[3][1] = 89 ;  Parameter->LutBitOffset[5][1] = 209;  Parameter->LutBitOffset[7][1] = 169;  
		Parameter->LutBitOffset[1][2] = 130;  Parameter->LutBitOffset[3][2] = 90 ;  Parameter->LutBitOffset[5][2] = 210;  Parameter->LutBitOffset[7][2] = 170;  
		Parameter->LutBitOffset[1][3] = 131;  Parameter->LutBitOffset[3][3] = 91 ;  Parameter->LutBitOffset[5][3] = 211;  Parameter->LutBitOffset[7][3] = 171;  
		Parameter->LutBitOffset[1][4] = 132;  Parameter->LutBitOffset[3][4] = 92 ;  Parameter->LutBitOffset[5][4] = 212;  Parameter->LutBitOffset[7][4] = 172;  
		Parameter->LutBitOffset[1][5] = 133;  Parameter->LutBitOffset[3][5] = 93 ;  Parameter->LutBitOffset[5][5] = 213;  Parameter->LutBitOffset[7][5] = 173;  
		Parameter->LutBitOffset[1][6] = 134;  Parameter->LutBitOffset[3][6] = 94 ;  Parameter->LutBitOffset[5][6] = 214;  Parameter->LutBitOffset[7][6] = 174;  
		Parameter->LutBitOffset[1][7] = 135;  Parameter->LutBitOffset[3][7] = 95 ;  Parameter->LutBitOffset[5][7] = 215;  Parameter->LutBitOffset[7][7] = 175;  
		Parameter->LutBitOffset[1][8] = 120;  Parameter->LutBitOffset[3][8] = 80 ;  Parameter->LutBitOffset[5][8] = 200;  Parameter->LutBitOffset[7][8] = 160;  
		Parameter->LutBitOffset[1][9] = 121;  Parameter->LutBitOffset[3][9] = 81 ;  Parameter->LutBitOffset[5][9] = 201;  Parameter->LutBitOffset[7][9] = 161;  
		Parameter->LutBitOffset[1][10]= 122;  Parameter->LutBitOffset[3][10]= 82 ;  Parameter->LutBitOffset[5][10]= 202;  Parameter->LutBitOffset[7][10]= 162;  
		Parameter->LutBitOffset[1][11]= 123;  Parameter->LutBitOffset[3][11]= 83 ;  Parameter->LutBitOffset[5][11]= 203;  Parameter->LutBitOffset[7][11]= 163;  
		Parameter->LutBitOffset[1][12]= 124;  Parameter->LutBitOffset[3][12]= 84 ;  Parameter->LutBitOffset[5][12]= 204;  Parameter->LutBitOffset[7][12]= 164;  
		Parameter->LutBitOffset[1][13]= 125;  Parameter->LutBitOffset[3][13]= 85 ;  Parameter->LutBitOffset[5][13]= 205;  Parameter->LutBitOffset[7][13]= 165;  
		Parameter->LutBitOffset[1][14]= 126;  Parameter->LutBitOffset[3][14]= 86 ;  Parameter->LutBitOffset[5][14]= 206;  Parameter->LutBitOffset[7][14]= 166;  
		Parameter->LutBitOffset[1][15]= 127;  Parameter->LutBitOffset[3][15]= 87 ;  Parameter->LutBitOffset[5][15]= 207;  Parameter->LutBitOffset[7][15]= 167;  
		Parameter->LutBitOffset[0][0] = 151;  Parameter->LutBitOffset[2][0] = 111;  Parameter->LutBitOffset[4][0] = 231;  Parameter->LutBitOffset[6][0] = 191;  
		Parameter->LutBitOffset[0][1] = 150;  Parameter->LutBitOffset[2][1] = 110;  Parameter->LutBitOffset[4][1] = 230;  Parameter->LutBitOffset[6][1] = 190;  
		Parameter->LutBitOffset[0][2] = 149;  Parameter->LutBitOffset[2][2] = 109;  Parameter->LutBitOffset[4][2] = 229;  Parameter->LutBitOffset[6][2] = 189;  
		Parameter->LutBitOffset[0][3] = 148;  Parameter->LutBitOffset[2][3] = 108;  Parameter->LutBitOffset[4][3] = 228;  Parameter->LutBitOffset[6][3] = 188;  
		Parameter->LutBitOffset[0][4] = 147;  Parameter->LutBitOffset[2][4] = 107;  Parameter->LutBitOffset[4][4] = 227;  Parameter->LutBitOffset[6][4] = 187;  
		Parameter->LutBitOffset[0][5] = 146;  Parameter->LutBitOffset[2][5] = 106;  Parameter->LutBitOffset[4][5] = 226;  Parameter->LutBitOffset[6][5] = 186;  
		Parameter->LutBitOffset[0][6] = 145;  Parameter->LutBitOffset[2][6] = 105;  Parameter->LutBitOffset[4][6] = 225;  Parameter->LutBitOffset[6][6] = 185;  
		Parameter->LutBitOffset[0][7] = 144;  Parameter->LutBitOffset[2][7] = 104;  Parameter->LutBitOffset[4][7] = 224;  Parameter->LutBitOffset[6][7] = 184;  
		Parameter->LutBitOffset[0][8] = 159;  Parameter->LutBitOffset[2][8] = 119;  Parameter->LutBitOffset[4][8] = 239;  Parameter->LutBitOffset[6][8] = 199;  
		Parameter->LutBitOffset[0][9] = 158;  Parameter->LutBitOffset[2][9] = 118;  Parameter->LutBitOffset[4][9] = 238;  Parameter->LutBitOffset[6][9] = 198;  
		Parameter->LutBitOffset[0][10]= 157;  Parameter->LutBitOffset[2][10]= 117;  Parameter->LutBitOffset[4][10]= 237;  Parameter->LutBitOffset[6][10]= 197;  
		Parameter->LutBitOffset[0][11]= 156;  Parameter->LutBitOffset[2][11]= 116;  Parameter->LutBitOffset[4][11]= 236;  Parameter->LutBitOffset[6][11]= 196;  
		Parameter->LutBitOffset[0][12]= 155;  Parameter->LutBitOffset[2][12]= 115;  Parameter->LutBitOffset[4][12]= 235;  Parameter->LutBitOffset[6][12]= 195;  
		Parameter->LutBitOffset[0][13]= 154;  Parameter->LutBitOffset[2][13]= 114;  Parameter->LutBitOffset[4][13]= 234;  Parameter->LutBitOffset[6][13]= 194;  
		Parameter->LutBitOffset[0][14]= 153;  Parameter->LutBitOffset[2][14]= 113;  Parameter->LutBitOffset[4][14]= 233;  Parameter->LutBitOffset[6][14]= 193;  
		Parameter->LutBitOffset[0][15]= 152;  Parameter->LutBitOffset[2][15]= 112;  Parameter->LutBitOffset[4][15]= 232;  Parameter->LutBitOffset[6][15]= 192; 
Parameter->LutShiftUsageOffset[0][0] = 136; Parameter->LutShiftUsageOffset[0][1] = 0; Parameter->LutShiftUsageOffset[0][2] = 0; Parameter->LutShiftUsageOffset[0][3] = 0;  
Parameter->LutShiftUsageOffset[1][0] = 141; Parameter->LutShiftUsageOffset[1][1] = 0; Parameter->LutShiftUsageOffset[1][2] = 0; Parameter->LutShiftUsageOffset[1][3] = 0;  
Parameter->LutShiftUsageOffset[2][0] =  96; Parameter->LutShiftUsageOffset[2][1] = 0; Parameter->LutShiftUsageOffset[2][2] = 0; Parameter->LutShiftUsageOffset[2][3] = 0;  
Parameter->LutShiftUsageOffset[3][0] = 101; Parameter->LutShiftUsageOffset[3][1] = 0; Parameter->LutShiftUsageOffset[3][2] = 0; Parameter->LutShiftUsageOffset[3][3] = 0;  
Parameter->LutShiftUsageOffset[4][0] = 216; Parameter->LutShiftUsageOffset[4][1] = 0; Parameter->LutShiftUsageOffset[4][2] = 0; Parameter->LutShiftUsageOffset[4][3] = 0;  
Parameter->LutShiftUsageOffset[5][0] = 221; Parameter->LutShiftUsageOffset[5][1] = 0; Parameter->LutShiftUsageOffset[5][2] = 0; Parameter->LutShiftUsageOffset[5][3] = 0;  
Parameter->LutShiftUsageOffset[6][0] = 176; Parameter->LutShiftUsageOffset[6][1] = 0; Parameter->LutShiftUsageOffset[6][2] = 0; Parameter->LutShiftUsageOffset[6][3] = 0; 
Parameter->LutShiftUsageOffset[7][0] = 181; Parameter->LutShiftUsageOffset[7][1] = 0; Parameter->LutShiftUsageOffset[7][2] = 0; Parameter->LutShiftUsageOffset[7][3] = 0; 
	}
	if(strncmp(Device,"XC2VP",5)==0)
		Parameter->Family="Virtex2Pro";

	if(strncmp(Device,"XC3S",4)==0)
	{
		Parameter->Family="Spartan3";
//		if(verbose>=2)
			printf("found a Spartan3 device\n");
		Parameter->CLKFrames=3;
		Parameter->IOBFrames=2;
		Parameter->IOIFrames=19;
		Parameter->CLBFrames=19;
		Parameter->RAMFrames=76;
		Parameter->RI_Frames=19;
		Parameter->CLBBytesIO=10;
		Parameter->CLBBytes=8;
// Remember the Slice numbering!!!
Parameter->LutBitOffset[1][0] = 39 ;  Parameter->LutBitOffset[3][0] = 231 ;  Parameter->LutBitOffset[5][0] = 7  ;  Parameter->LutBitOffset[7][0] = 199 ;  
Parameter->LutBitOffset[1][1] = 38 ;  Parameter->LutBitOffset[3][1] = 230 ;  Parameter->LutBitOffset[5][1] = 6  ;  Parameter->LutBitOffset[7][1] = 198 ;  
Parameter->LutBitOffset[1][2] = 37 ;  Parameter->LutBitOffset[3][2] = 229 ;  Parameter->LutBitOffset[5][2] = 5  ;  Parameter->LutBitOffset[7][2] = 197 ;  
Parameter->LutBitOffset[1][3] = 36 ;  Parameter->LutBitOffset[3][3] = 228 ;  Parameter->LutBitOffset[5][3] = 4  ;  Parameter->LutBitOffset[7][3] = 196 ;  
Parameter->LutBitOffset[1][4] = 35 ;  Parameter->LutBitOffset[3][4] = 227 ;  Parameter->LutBitOffset[5][4] = 3  ;  Parameter->LutBitOffset[7][4] = 195 ;  
Parameter->LutBitOffset[1][5] = 34 ;  Parameter->LutBitOffset[3][5] = 226 ;  Parameter->LutBitOffset[5][5] = 2  ;  Parameter->LutBitOffset[7][5] = 194 ;  
Parameter->LutBitOffset[1][6] = 33 ;  Parameter->LutBitOffset[3][6] = 225 ;  Parameter->LutBitOffset[5][6] = 1  ;  Parameter->LutBitOffset[7][6] = 193 ;  
Parameter->LutBitOffset[1][7] = 32 ;  Parameter->LutBitOffset[3][7] = 224 ;  Parameter->LutBitOffset[5][7] = 0  ;  Parameter->LutBitOffset[7][7] = 192 ;  
Parameter->LutBitOffset[1][8] = 47 ;  Parameter->LutBitOffset[3][8] = 239 ;  Parameter->LutBitOffset[5][8] = 15 ;  Parameter->LutBitOffset[7][8] = 207 ;  
Parameter->LutBitOffset[1][9] = 46 ;  Parameter->LutBitOffset[3][9] = 238 ;  Parameter->LutBitOffset[5][9] = 14 ;  Parameter->LutBitOffset[7][9] = 206 ;  
Parameter->LutBitOffset[1][10]= 45 ;  Parameter->LutBitOffset[3][10]= 237 ;  Parameter->LutBitOffset[5][10]= 13 ;  Parameter->LutBitOffset[7][10]= 205 ;  
Parameter->LutBitOffset[1][11]= 44 ;  Parameter->LutBitOffset[3][11]= 236 ;  Parameter->LutBitOffset[5][11]= 12 ;  Parameter->LutBitOffset[7][11]= 204 ;  
Parameter->LutBitOffset[1][12]= 43 ;  Parameter->LutBitOffset[3][12]= 235 ;  Parameter->LutBitOffset[5][12]= 11 ;  Parameter->LutBitOffset[7][12]= 203 ;  
Parameter->LutBitOffset[1][13]= 42 ;  Parameter->LutBitOffset[3][13]= 234 ;  Parameter->LutBitOffset[5][13]= 10 ;  Parameter->LutBitOffset[7][13]= 202 ;  
Parameter->LutBitOffset[1][14]= 41 ;  Parameter->LutBitOffset[3][14]= 233 ;  Parameter->LutBitOffset[5][14]= 9  ;  Parameter->LutBitOffset[7][14]= 201 ;  
Parameter->LutBitOffset[1][15]= 40 ;  Parameter->LutBitOffset[3][15]= 232 ;  Parameter->LutBitOffset[5][15]= 8  ;  Parameter->LutBitOffset[7][15]= 200 ;  
Parameter->LutBitOffset[0][0] = 55 ;  Parameter->LutBitOffset[2][0] = 247 ;  Parameter->LutBitOffset[4][0] = 23 ;  Parameter->LutBitOffset[6][0] = 215 ;  
Parameter->LutBitOffset[0][1] = 54 ;  Parameter->LutBitOffset[2][1] = 246 ;  Parameter->LutBitOffset[4][1] = 22 ;  Parameter->LutBitOffset[6][1] = 214 ;  
Parameter->LutBitOffset[0][2] = 53 ;  Parameter->LutBitOffset[2][2] = 245 ;  Parameter->LutBitOffset[4][2] = 21 ;  Parameter->LutBitOffset[6][2] = 213 ;  
Parameter->LutBitOffset[0][3] = 52 ;  Parameter->LutBitOffset[2][3] = 244 ;  Parameter->LutBitOffset[4][3] = 20 ;  Parameter->LutBitOffset[6][3] = 212 ;  
Parameter->LutBitOffset[0][4] = 51 ;  Parameter->LutBitOffset[2][4] = 243 ;  Parameter->LutBitOffset[4][4] = 19 ;  Parameter->LutBitOffset[6][4] = 211 ;  
Parameter->LutBitOffset[0][5] = 50 ;  Parameter->LutBitOffset[2][5] = 242 ;  Parameter->LutBitOffset[4][5] = 18 ;  Parameter->LutBitOffset[6][5] = 210 ;  
Parameter->LutBitOffset[0][6] = 49 ;  Parameter->LutBitOffset[2][6] = 241 ;  Parameter->LutBitOffset[4][6] = 17 ;  Parameter->LutBitOffset[6][6] = 209 ;  
Parameter->LutBitOffset[0][7] = 48 ;  Parameter->LutBitOffset[2][7] = 240 ;  Parameter->LutBitOffset[4][7] = 16 ;  Parameter->LutBitOffset[6][7] = 208 ;  
Parameter->LutBitOffset[0][8] = 63 ;  Parameter->LutBitOffset[2][8] = 255 ;  Parameter->LutBitOffset[4][8] = 31 ;  Parameter->LutBitOffset[6][8] = 223 ;  
Parameter->LutBitOffset[0][9] = 62 ;  Parameter->LutBitOffset[2][9] = 254 ;  Parameter->LutBitOffset[4][9] = 30 ;  Parameter->LutBitOffset[6][9] = 222 ;  
Parameter->LutBitOffset[0][10]= 61 ;  Parameter->LutBitOffset[2][10]= 253 ;  Parameter->LutBitOffset[4][10]= 29 ;  Parameter->LutBitOffset[6][10]= 221 ;  
Parameter->LutBitOffset[0][11]= 60 ;  Parameter->LutBitOffset[2][11]= 252 ;  Parameter->LutBitOffset[4][11]= 28 ;  Parameter->LutBitOffset[6][11]= 220 ;  
Parameter->LutBitOffset[0][12]= 59 ;  Parameter->LutBitOffset[2][12]= 251 ;  Parameter->LutBitOffset[4][12]= 27 ;  Parameter->LutBitOffset[6][12]= 219 ;  
Parameter->LutBitOffset[0][13]= 58 ;  Parameter->LutBitOffset[2][13]= 250 ;  Parameter->LutBitOffset[4][13]= 26 ;  Parameter->LutBitOffset[6][13]= 218 ;  
Parameter->LutBitOffset[0][14]= 57 ;  Parameter->LutBitOffset[2][14]= 249 ;  Parameter->LutBitOffset[4][14]= 25 ;  Parameter->LutBitOffset[6][14]= 217 ;  
Parameter->LutBitOffset[0][15]= 56 ;  Parameter->LutBitOffset[2][15]= 248 ;  Parameter->LutBitOffset[4][15]= 24 ;  Parameter->LutBitOffset[6][15]= 216 ; 

Parameter->LutShiftUsageOffset[0][0] = 126; Parameter->LutShiftUsageOffset[0][1] = 112; Parameter->LutShiftUsageOffset[0][2] = 0 ; Parameter->LutShiftUsageOffset[0][3] = 0;  
Parameter->LutShiftUsageOffset[1][0] = 112; Parameter->LutShiftUsageOffset[1][1] = 126; Parameter->LutShiftUsageOffset[1][2] = 0 ; Parameter->LutShiftUsageOffset[1][3] = 0;  
Parameter->LutShiftUsageOffset[2][0] = 0  ; Parameter->LutShiftUsageOffset[2][1] = 0  ; Parameter->LutShiftUsageOffset[2][2] = 0  ; Parameter->LutShiftUsageOffset[2][3] = 0;  
Parameter->LutShiftUsageOffset[3][0] = 0  ; Parameter->LutShiftUsageOffset[3][1] = 0  ; Parameter->LutShiftUsageOffset[3][2] = 0  ; Parameter->LutShiftUsageOffset[3][3] = 0;  
Parameter->LutShiftUsageOffset[4][0] = 94 ; Parameter->LutShiftUsageOffset[4][1] = 80; Parameter->LutShiftUsageOffset[4][2] = 0 ;  Parameter->LutShiftUsageOffset[4][3] = 0;  
Parameter->LutShiftUsageOffset[5][0] = 80 ; Parameter->LutShiftUsageOffset[5][1] = 94; Parameter->LutShiftUsageOffset[5][2] = 0 ;  Parameter->LutShiftUsageOffset[5][3] = 0;  
Parameter->LutShiftUsageOffset[6][0] = 0  ; Parameter->LutShiftUsageOffset[6][1] = 0  ; Parameter->LutShiftUsageOffset[6][2] = 0  ; Parameter->LutShiftUsageOffset[6][3] = 0; 
Parameter->LutShiftUsageOffset[7][0] = 0  ; Parameter->LutShiftUsageOffset[7][1] = 0  ; Parameter->LutShiftUsageOffset[7][2] = 0  ; Parameter->LutShiftUsageOffset[7][3] = 0; 

	}

	if(strncmp(Device,"XC6V",4)==0) {
		Parameter->Family="Virtex 6";
		printf("found a Virtex 6 device\n");

		Parameter->CLBFrames=36;
		Parameter->CLBBytes=8;
		Parameter->RAMFrames=128;
		Parameter->CLBBytesIO=0;
		Parameter->CLKFrames=0;
		Parameter->IOBFrames=0;
		Parameter->IOIFrames=0;
		Parameter->RI_Frames=28;
		Parameter->MFrames=36;
		Parameter->DFrames=28;
		Parameter->GFrames=30;
		Parameter->HWords=1;
	}

	if(strncmp(Device,"XC7A",4)==0) {
		Parameter->Family="Artix 7";
		printf("found a Artix 7 device\n");

		Parameter->CLBFrames=36;
		Parameter->CLBBytes=8;
		Parameter->RAMFrames=128;
		Parameter->CLBBytesIO=0;
		Parameter->CLKFrames=0;
		Parameter->IOBFrames=0;
		Parameter->IOIFrames=0;
		Parameter->RI_Frames=28;
		Parameter->MFrames=36;
		Parameter->DFrames=28;
		Parameter->GFrames=30;
		Parameter->HWords=1;
	}

	if(strncmp(Device,"XC7Z",4)==0) {
		Parameter->Family="Zynq 7000";
		printf("found a Zynq 7000 device\n");

		Parameter->CLBFrames=36;
		Parameter->CLBBytes=8;
		Parameter->RAMFrames=128;
		Parameter->CLBBytesIO=0;
		Parameter->CLKFrames=0;
		Parameter->IOBFrames=0;
		Parameter->IOIFrames=0;
		Parameter->RI_Frames=28;
		Parameter->MFrames=36;
		Parameter->DFrames=28;
		Parameter->GFrames=30;
		Parameter->HWords=1;
	}

	if(strncmp(Device,"XC7V",4)==0) {
		Parameter->Family="Virtex 7";
		printf("found a Virtex 7 device\n");

		Parameter->CLBFrames=36;
		Parameter->CLBBytes=8;
		Parameter->RAMFrames=128;
		Parameter->CLBBytesIO=0;
		Parameter->CLKFrames=0;
		Parameter->IOBFrames=0;
		Parameter->IOIFrames=0;
		Parameter->RI_Frames=28;
		Parameter->MFrames=36;
		Parameter->DFrames=28;
		Parameter->GFrames=30;
		Parameter->HWords=1;
		Parameter->PFrames=42;
		Parameter->IFrames=33;
		Parameter->FFrames=30;
		Parameter->VFrames=30;
	}

	if(strncmp(Device,"KU0",3)==0) {
		Parameter->Family="Kintex Ultrascale";
		printf("found a Kintex Ultrascale device\n");

		Parameter->CLBFrames=12;
		Parameter->CLBBytes=8;
		Parameter->RAMFrames=128;
		Parameter->CLBBytesIO=0;
		Parameter->CLKFrames=0;
		Parameter->IOBFrames=0;
		Parameter->IOIFrames=0;
		Parameter->RI_Frames=62;
		Parameter->MFrames=70;
		Parameter->DFrames=4;
		Parameter->GFrames=2;
		Parameter->HWords=3;
	}

	if(strncmp(Device,"XCZU",4)==0) {
		Parameter->Family="Zynq Ultrascale+";
		printf("found a Zynq Ultrascale+ device\n");

		Parameter->CLBFrames=16;
		Parameter->CLBBytes=6;
		Parameter->RAMFrames=256;
		Parameter->CLBBytesIO=0;
		Parameter->CLKFrames=0;
		Parameter->IOBFrames=0;
		Parameter->IOIFrames=0;
		Parameter->RI_Frames=6;
		Parameter->MFrames=16;
		Parameter->DFrames=8;
		Parameter->GFrames=0;
		Parameter->HWords=3;
		Parameter->SwFrames=76;
		Parameter->PFrames=10;
		Parameter->IFrames=9;
		Parameter->mFrames=4;
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
	SwFrames= GlobalParameters.SwFrames;
	PFrames = GlobalParameters.PFrames;
	IFrames = GlobalParameters.IFrames;
	mFrames = GlobalParameters.mFrames;
	FFrames = GlobalParameters.FFrames;
	VFrames = GlobalParameters.VFrames;
	HWords = GlobalParameters.HWords;
	CLBBytesIO = GlobalParameters.CLBBytesIO;
	CLBBytes = GlobalParameters.CLBBytes;
	NULLArea = GlobalParameters.NULLArea;
	NULLArea1 = GlobalParameters.NULLArea1;
	NULLArea2 = GlobalParameters.NULLArea2;
	NULLArea3 = GlobalParameters.NULLArea3;
	RowFrames = GlobalParameters.RowFrames;
	RowIOPad  = GlobalParameters.RowIOPad;
	RowFrames2 = GlobalParameters.RowFrames2;
	RAMRowOff = GlobalParameters.RAMRowOff;
	RAMRowFrames = GlobalParameters.RAMRowFrames;
	//for(int i=0; i<8; i++)
	//	for(int j=0; j<16; j++)
	//		LutBitOffset[i][j] = GlobalParameters.LutBitOffset[i][j];
	//for(int i=0; i<8; i++)
	//	for(int j=0; j<4; j++)
	//		LutShiftUsageOffset[i][j] = GlobalParameters.LutShiftUsageOffset[i][j];
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
		if (RowFrames2 == 0)
			return (MNA + NColRI*RI_Frames + NColBFG*GFrames + NColM*MFrames + NColD*DFrames + NColS*SwFrames + NColP*PFrames + NColI*IFrames + NColm*mFrames + NColF*FFrames + NColV*VFrames + (MJA - (NColRI + NColBFG + NColM + NColD + NColS + NColP + NColI + NColm + NColF + NColV))*CLBFrames + FrameOffset + (TB + RA)*RowFrames);
		else  // be careful with this case
			return (MNA + NColRI*RI_Frames + NColBFG*GFrames + NColM*MFrames + NColD*DFrames + NColS*SwFrames + NColP*PFrames + NColI*IFrames + NColm*mFrames + NColF*FFrames + NColV*VFrames + (MJA - (NColRI + NColBFG + NColM + NColD + NColS + NColP + NColI + NColm + NColF + NColV))*CLBFrames + FrameOffset + (TB + RA)*RowFrames + TB*RowFrames2);
	}
	else if (BA == 1)
		return (MNA + MJA*RAMFrames + RAMRowOff + (TB + RA)*RAMRowFrames);

	printf("No valid base address - trying to continue.\n");
	return(-1);
}

int getFrameIndex(int BA, int MJA, int MNA, int TB_RA)
{
	int FrameOffset = 0;
	int tmp;
	int tmpNColNULL=0, tmpNColRI=0, tmpNColBFG=0, tmpNColM=0, tmpNColD=0, tmpNColS=0, tmpNColP=0, tmpNColI=0, tmpNColm=0, tmpNColF=0, tmpNColV=0;
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
				else if (ResourceString[tmp + tmpNColNULL] == 's')
					tmpNColS++;
				else if (ResourceString[tmp + tmpNColNULL] == 'P')
					tmpNColP++;
				else if (ResourceString[tmp + tmpNColNULL] == 'I')
					tmpNColI++;
				else if (ResourceString[tmp + tmpNColNULL] == 'm')
					tmpNColm++;
				else if (ResourceString[tmp + tmpNColNULL] == 'F')
					tmpNColF++;
				else if (ResourceString[tmp + tmpNColNULL] == 'V')
					tmpNColV++;
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
		if (RowFrames2 == 0)
			return (MNA + tmpNColRI*RI_Frames + tmpNColBFG*GFrames + tmpNColM*MFrames + tmpNColD*DFrames + tmpNColS*SwFrames + tmpNColP*PFrames + tmpNColI*IFrames + tmpNColm*mFrames + tmpNColF*FFrames + tmpNColV*VFrames + (MJA - (tmpNColRI + tmpNColBFG + tmpNColM + tmpNColD + tmpNColS + tmpNColP + tmpNColI + tmpNColm + tmpNColF + tmpNColV))*CLBFrames + FrameOffset + TB_RA*RowFrames);
		else  // be careful with this case
			// dedicated for the Artix-7 XC7A35T device
			if	(strcmp(Device,"XC7A35T") == 0) {
				if (TB_RA < 2)
					return (MNA + tmpNColRI*RI_Frames + tmpNColBFG*GFrames + tmpNColM*MFrames + tmpNColD*DFrames + tmpNColS*SwFrames + tmpNColP*PFrames + tmpNColI*IFrames + tmpNColm*mFrames + tmpNColF*FFrames + tmpNColV*VFrames + (MJA - (tmpNColRI + tmpNColBFG + tmpNColM + tmpNColD + tmpNColS + tmpNColP + tmpNColI + tmpNColm + tmpNColF + tmpNColV))*CLBFrames + FrameOffset + TB_RA*RowFrames);
				else
					return (MNA + tmpNColRI*RI_Frames + tmpNColBFG*GFrames + tmpNColM*MFrames + tmpNColD*DFrames + tmpNColS*SwFrames + tmpNColP*PFrames + tmpNColI*IFrames + tmpNColm*mFrames + tmpNColF*FFrames + tmpNColV*VFrames + (MJA - (tmpNColRI + tmpNColBFG + tmpNColM + tmpNColD + tmpNColS + tmpNColP + tmpNColI + tmpNColm + tmpNColF + tmpNColV))*CLBFrames + FrameOffset + (TB_RA - 1)*RowFrames + RowFrames2);
			}
	}
	else if (BA == 1)
		return (MNA + MJA*RAMFrames + RAMRowOff + TB_RA*RAMRowFrames);
	//printf("BA %d, MJA %d, TB_RA %d \n", BA, MJA, TB_RA);
	printf("No valid base address - trying to continue.\n");
	return(-1);
}

int incFrameAddress(void)
{
	if((BA==-1) || (MJA==-1) || (MNA==-1))
	{
		printf("Base frame address not found - cannot compute frame address.\n");
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
		else if (ResourceString[MJA + NColNULL] == 'R')
			if (MNA < (RI_Frames - 1))	// BRAM/DSP column has 28 frames for interconnect
				MNA++;
			else {
				MJA++;
				MNA = 0;
				NColRI++;
			}
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
			}
		else if (ResourceString[MJA + NColNULL] == 'D')
			if (MNA < (DFrames - 1))	// BRAM/DSP column has 28 frames for interconnect
				MNA++;
			else {
				MJA++;
				MNA = 0;
				NColD++;
			}
		else if (ResourceString[MJA + NColNULL] == 's')
			if (MNA < (SwFrames - 1))
				MNA++;
			else {
				MJA++;
				MNA = 0;
				NColS++;
			}
		else if (ResourceString[MJA + NColNULL] == 'P')
			if (MNA < (PFrames - 1))
				MNA++;
			else {
				MJA++;
				MNA = 0;
				NColP++;
			}
		else if (ResourceString[MJA + NColNULL] == 'I')
			if (MNA < (IFrames - 1))
				MNA++;
			else {
				MJA++;
				MNA = 0;
				NColI++;
			}
		else if (ResourceString[MJA + NColNULL] == 'm')
			if (MNA < (mFrames - 1))
				MNA++;
			else {
				MJA++;
				MNA = 0;
				NColm++;
			}
		else if (ResourceString[MJA + NColNULL] == 'F')
			if (MNA < (FFrames - 1))
				MNA++;
			else {
				MJA++;
				MNA = 0;
				NColF++;
			}
		else if (ResourceString[MJA + NColNULL] == 'V')
			if (MNA < (VFrames - 1))
				MNA++;
			else {
				MJA++;
				MNA = 0;
				NColV++;
			}
		return 0;
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
		printf("Internal error: invalid frame address\n");
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