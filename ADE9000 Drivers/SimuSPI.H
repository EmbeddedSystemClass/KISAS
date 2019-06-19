/*******************************************************************************
Copyright(c) 2014 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.
--------------------------------------------------------------------------------
Filename			: SimuSPI.h
Abstract			: List all public routine
Author				: Yiyi Zhao
Write date			: 2014-10-30(YYYY-MM-DD)
*******************************************************************************/

void Init_ADE9000(void);
void Read_ADE9000_SPI(uint16_t  us_ADE_Addr, uint16_t us_Nr_Bytes, uint8_t *puc_Reg_Data);
uint16_t Read_ADE9000_CRC_SPI(uint16_t  us_ADE_Addr, uint16_t us_Nr_Bytes, uint8_t *puc_Reg_Data);
void Write_ADE9000_SPI(uint16_t  us_ADE_Addr, uint8_t uc_Nr_Bytes, uint8_t *puc_Reg_Data);
void Read_OTest_ADE9000_SPI(uint16_t  us_ADE_Addr, uint16_t us_Nr_Bytes, uint8_t *puc_Reg_Data);
uint8_t IsADE9078OrADE9000(void);

/******************* (C) COPYRIGHT 2014 ADI *****END OF FILE****/

