/* $Id: vgamodes.h,v 1.2 2002/03/05 01:57:16 pavlovskii Exp $ */

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
/* =-                                                                         -= */
/* =-                   Tauron VGA Utilities Version 3.0                      -= */
/* =-                      Released September 20, 1998                        -= */
/* =-                                                                         -= */
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
/* =- Copyright (c) 1997, 1998 by Jeff Morgan  =-= This code is FREE provided -= */
/* =- All Rights Reserved.                     =-= that you put my name some- -= */
/* =-                                          =-= where in your credits.     -= */
/* =- DISCLAIMER:                              =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
/* =- I assume no responsibility whatsoever for any effect that this package, -= */
/* =- the information contained therein or the use thereof has on you, your   -= */
/* =- sanity, computer, spouse, children, pets or anything else related to    -= */
/* =- you or your existance. No warranty is provided nor implied with this    -= */
/* =- source code.                                                            -= */
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
/* =- VGA register variables for the various videomodes                       -= */
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

/*  Same as Mode 01H */
const uint8_t mode00h[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
   0x67,      0x00,          0x03,0x08,0x03,0x00,0x02,
/*  CRTC regs */
0x2D,0x27,0x28,0x90,0x2B,0xA0,0xBF,0x1F,0x00,0x4F,0x06,0x07,0x00,0x00,0x00,
0x31,0x9C,0x8E,0x8F,0x14,0x1F,0x96,0xB9,0xA3,0xFF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x10,0x0E,0x00,0xFF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x10,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x0C,0x00,0x0F,0x08,0x00
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */


/*  Same as Mode 02H */
const uint8_t mode03h[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
   0x67,      0x00,          0x03,0x00,0x03,0x00,0x02,
/*  CRTC regs */
0x5F,0x4F,0x50,0x82,0x55,0x81,0xBF,0x1F,0x00,0x4F,0x0E,0x0F,0x00,0x00,0x00,
0x00,0x9C,0x0E,0x8F,0x28,0x01,0x96,0xB9,0xA3,0xFF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x10,0x0E,0x00,0xFF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x10,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x0C,0x00,0x0F,0x08,0x00
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

/*  Same as Mode 05, */
const uint8_t mode04h[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
   0x63,      0x00,          0x03,0x09,0x03,0x00,0x02,
/*  CRTC regs */
0x2D,0x27,0x28,0x90,0x2B,0x80,0x0BF,0x1F,0x00,0x0C1,0x00,0x00,0x00,0x00,0x00,
0x31,0x9C,0x8E,0x8F,0x14,0x00,0x96,0xB9,0x0A2,0xFF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x30,0x0F,0x00,0xFF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x3B,0x3D,0x3F,0x02,0x04,0x06,0x07,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
0x01,0x00,0x03,0x00,0x00
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

const uint8_t mode06h[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
   0x63,      0x00,          0x03,0x01,0x01,0x00,0x06,
/*  CRTC regs */
0x5F,0x4F,0x50,0x82,0x54,0x80,0x0BF,0x1F,0x00,0x0C1,0x00,0x00,0x00,0x00,0x00,
0x00,0x9C,0x8E,0x8F,0x28,0x00,0x96,0xB9,0x0C2,0xFF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x00,0x0D,0x00,0xFF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
0x01,0x00,0x01,0x00,0x00
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

const uint8_t mode07h[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
   0x066,     0x00,          0x03,0x00,0x03,0x00,0x02,
/*  CRTC regs */
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x10,0x0A,0x00,0xFF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x10,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x0E,0x00,0x0F,0x00,0x00
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

const uint8_t mode0Dh[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
   0x063,     0x00,          0x03,0x09,0x0F,0x00,0x06,
/*  CRTC regs */
0x2D,0x27,0x28,0x90,0x2B,0x80,0x0BF,0x1F,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,
0x31,0x9C,0x8E,0x8F,0x14,0x00,0x96,0xB9,0xE3,0xFF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x10,0x05,0x00,0xFF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x10,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x01,0x00,0x0F,0x00,0x00
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

const uint8_t mode0Eh[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
   0x63,      0x00,          0x03,0x01,0x0F,0x00,0x06,
/*  CRTC regs */
0x5F,0x4F,0x50,0x82,0x54,0x80,0x0BF,0x1F,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,
0x59,0x9C,0x8E,0x8F,0x28,0x00,0x96,0xB9,0xE3,0xFF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x0F,0xFF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x08,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x01,0x00,0x0F,0x00,0x00
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

const uint8_t mode0Fh[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
   0x0A2,     0x00,          0x03,0x01,0x0F,0x00,0x06,
/*  CRTC regs */
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x05,0xFF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x08,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x18,0x00,0x00,
0x0B,0x00,0x05,0x00,0x00
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

const uint8_t mode10h[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
   0x0A3,     0x00,          0x03,0x01,0x0F,0x00,0x06,
/*  CRTC regs */
0x5F,0x4F,0x50,0x82,0x54,0x80,0x0BF,0x1F,0x00,0x40,0x00,0x00,0x00,0x00,0x00,
0x00,0x83,0x85,0x5D,0x28,0x0F,0x63,0x0BA,0xE3,0xFF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x0F,0xFF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
0x01,0x00,0x0F,0x00,0x00
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

const uint8_t mode11h[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
   0xE3,     0x00,          0x03,0x01,0x0F,0x00,0x06,
/*  CRTC regs */
0x5F,0x4F,0x50,0x82,0x54,0x80,0x0B,0x3E,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x59,
0xEA,0x8C,0x0DF,0x28,0x0F,0x0E7,0x004,0x0C3,0xFF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x05,0xFF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x08,0x3F,0x3F,0x18,0x18,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
0x01,0x00,0x0F,0x00,0x00

};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

const uint8_t mode12h[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
   0xE3,     0x00,          0x03,0x01,0x0F,0x00,0x06,
/*  CRTC regs */
0x5F,0x4F,0x50,0x82,0x54,0x80,0x0B,0x3E,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x59,
0xEA,0x8C,0x0DF,0x28,0x00,0x0E7,0x04,0xE3,0xFF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x0F,0xFF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
0x01,0x00,0x0F,0x00,0x00
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

const uint8_t mode13h[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
   0x63,      0x00,          0x03,0x01,0x0F,0x00,0x0E,
/*  CRTC regs */
0x5F,0x4F,0x50,0x82,0x54,0x80,0x0BF,0x1F,0x00,0x41,0x00,0x00,0x00,0x00,0x00,
0x00,0x9C,0x0E,0x8F,0x28,0x40,0x96,0xB9,0x0A3,0xFF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x40,0x05,0x0F,0xFF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
0x41,0x00,0x0F,0x00,0x00
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

const uint8_t modeC4[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
   0x63,      0x00,          0x03,0x01,0x0F,0x00,0x06,
/*  CRTC regs */
0x5F,0x4F,0x50,0x82,0x54,0x80,0x0BF,0x1F,0x00,0x41,0x00,0x00,0x00,0x00,0x00,0x00,
0x9C,0x0E,0x8F,0x28,0x00,0x96,0xB9,0xE3,0xFF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x40,0x05,0x0F,0xFF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
0x41,0x00,0x0F,0x00,0x00
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

const uint8_t modeJ[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
   0xa3,      0x00,          0x03,0x01,0x03,0x00,0x02,
/*  CRTC regs */
0x5F,0x4F,0x50,0x82,0x55,0x81,0xBF,0x1F,0x00,0x47,0x0E,0x0F,0x00,0x00,0x00,
0x00,0x83,0x85,0x57,0x28,0x1F,0x60,0xB8,0xA3,0xFF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x10,0x0E,0x00,0xFF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x10,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x0C,0x00,0x0F,0x00,0x00
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

const uint8_t modeK[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
   0x63,      0x00,          0x03,0x01,0x03,0x00,0x02,
/*  CRTC regs */
0x5F,0x4F,0x50,0x82,0x55,0x81,0xBF,0x1F,0x00,0x47,0x0E,0x0F,0x00,0x00,0x00,
0x00,0x9C,0x8E,0x8F,0x28,0x1F,0x96,0xB9,0xA3,0xFF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x10,0x0E,0x00,0xFF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x10,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x0C,0x00,0x0F,0x00,0x00
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

const uint8_t modeL[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
0x67,0x00,0x03,0x08,0x03,0x00,0x02,
/*  CRTC regs */
0x2D,0x27,0x28,0x90,0x2B,0x0A0,0x0BF,0x1F,0x00,0x47,0x06,0x07,0x00,0x00,0x00,
0x31,0x83,0x85,0x57,0x14,0x1F,0x60,0x0B8,0x0A3,0x0FF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x10,0x0E,0x00,0x0FF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x10,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x0C,0x00,0x0F,0x00,0x00,
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */


const uint8_t modeM[62] = {
/*  MISC reg,  STATUS reg,    SEQ regs */
0x67,0x00,0x03,0x08,0x03,0x00,0x02,
/*  CRTC regs */
0x2D,0x27,0x28,0x90,0x2B,0x0A0,0x0BF,0x1F,0x00,0x47,0x06,0x07,0x00,0x00,0x00,0x31,
0x9C,0x8E,0x8F,0x14,0x1F,0x96,0x0B9,0x0A3,0x0FF,
/*  GRAPHICS regs */
0x00,0x00,0x00,0x00,0x00,0x10,0x0E,0x00,0x0FF,
/*  ATTRIBUTE CONTROLLER regs */
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x0C,0x00,0x0F,0x00,0x00,
};
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
