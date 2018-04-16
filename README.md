# bitman

Use "bitman -help" for documentation

Any question on how to use the tool, please send an email to the author at khoa.pham@manchester.ac.uk!

How to use the bitscan/bitman:

* Use the help feature to explore console functionality

	* bitman.exe -h
	* BitMan - a configuration bitstream analizing tool for Xilinx FPGAs
	* Usage: bitman options [input_bitfile_1] [input_bitfile_2] option [output_file]
	* Options:
		+ -v verbose
		+ -c : print CLB info including bitstream encoding
		+ -x COL1 ROW1 COL2 ROW2 : Cut out an FPGA region	from COL1 ROW1 (bottom left) COL2 ROW2 (top right)
		+ -m COL1 ROW1 COL2 ROW2 [input_bitfile_1] [input_bitfile_2]: Cut out an FPGA region from COL1 ROW1 (bottom left)
									COL2 ROW2 (top right) in full bitstream bitfile_1 and merge
									it into the full bitstream bitfile_2
		+ -r COL1 ROW1 COL2 ROW2 COL3 ROW3 [input_bitfile_1]: relocate an FPGA region from COL1 ROW1 (bottom left)
									COL2 ROW2 (top right) to COL3 ROW3 (bottom left)
		+ -d COL1 ROW1 COL2 ROW2 COL3 ROW3 [input_bitfile_1]: duplicate an FPGA region from COL1 ROW1 (bottom left)
									COL2 ROW2 (top right) to COL3 ROW3 (bottom left)

		+ -S COL ROW LUT value_h value_l: Set value (value_h, valuel) in the 1 of 8 LUTs at the COL ROW

		+ -F [output_file]: Write (linked) full bitstream into file.
		+ -M COL3 ROW3 [output_file]: Write module bitstream into partial file in the location start at COL3 ROW3.

* How to calcute COL and ROW values in different FPGA families
	* In 7-Series, ROW1 and ROW2 are strictly limited within a clock height. Cross-clock height operations are not supported.
	* In the Virtex-7 family and XC7Z020 (ZedBoard): the COL and ROW values of a resource column are the X-Y
	coordinator of its Switchbox.
	* In the XC7Z010 (Zybo): the COL value is equal the X coordinator of its Switchbox minus 2 (X-2). The ROW value is the same as the Y coordinator.
	* In the Zynq UltraScale+ family: resource columns are organized as the pattern L-s-R
	(left resouce column - Switchbox - right resource column)
		+ Let say the Switchbox has the X-Y coordinator
		+ The COL value of the L column is the value of (X*3+1)
		+ The COL value of the s column is the value of (X*3+2)
		+ The COL value of the R column is the value of (X*3+3)
* Notes for "Set value for LUT" function
	* How to know the LUT position in a CLB
		* They are enumerated from bottom to top:
			* A6LUT (bottom) - postion 0
			* B6LUT          - postion 1
			* C6LUT          - postion 2
			* D6LUT          - postion 3
			* A6LUT (middle) - postion 4
			* B6LUT          - postion 5
			* C6LUT          - postion 6
			* D6LUT          - postion 7
	* (value_h, value_l) are in hexadecimal format to fulfill 64 bits of LUT init value
	* Example: set LUT 0 at position (13, 250) to the value 0x8000000000000001
	bitman.exe -S 13 250 0 80000000 00000001 none.bin -F clbm_13_250-3.bin
