int getGlobalDeviceInformation(int DeviceID);
int SetGlobalDeviceParameters(char *Device);
int getFrameIndexGlobal(void);
int getFrameIndex(int BA, int MJA, int MNA, int TB_RA);	// TB_RA = TB + RA
int incFrameAddress(void);
int getROWByteIndex(int CLB_ROWindex);
int echoSupportedDevices(void);
int FrameADR2BlockADR(int FrameADR, int *BA, int *MJA, int *MNA);

int getDeviceInformation(int DeviceID);
//int SetDeviceParameters(char *Device, struct DeviceParameters *Parameter);
int SetGlobalDeviceParameters(char *Device);
int getFrameIndexGlobal(void);
int getFrameIndex(int BA, int MJA, int MNA, int TB_RA);
int incFrameAddress(void);

typedef struct {
int encoding[2][8][64][2];	// 2 types M and L, 8 LUT positions from bottom to top A6-1 B6-1 C6-1 D6-1 A6-2 B6-2 C6-2 D6-2, 64 bits of value, 2 needed info: frame number and bitoffset
} Xilinx_Lut_Bit_Offset;
