int getGlobalDeviceInformation(int DeviceID);
int SetGlobalDeviceParameters(char *Device);
int getFrameIndexGlobal(void);
int getFrameIndex(int BA, int MJA, int MNA, int TB_RA);	// TB_RA = TB + RA
int incFrameAddress(void);
int getROWByteIndex(int CLB_ROWindex);
int echoSupportedDevices(void);
int FrameADR2BlockADR(int FrameADR, int *BA, int *MJA, int *MNA);

int getDeviceInformation(int DeviceID);
int SetDeviceParameters(char *Device, struct DeviceParameters *Parameter);
int SetGlobalDeviceParameters(char *Device);
int getFrameIndexGlobal(void);
int getFrameIndex(int BA, int MJA, int MNA, int TB_RA);
int incFrameAddress(void);