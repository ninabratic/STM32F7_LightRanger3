/*
 * TestDLG.h
 *
 *  Created on: Nov 21, 2018
 *      Author: Jovan
 */

#ifndef WINDOWS_TESTDLG_H_
#define WINDOWS_TESTDLG_H_

#include "DIALOG.h"

#define ID_WINDOW_0              (GUI_ID_USER + 0x00)
#define ID_BUTTON_0              (GUI_ID_USER + 0x01)
#define ID_PROGBAR_0             (GUI_ID_USER + 0x02)
#define ID_EDIT_0             (GUI_ID_USER + 0x03)
#define ID_TEXT_0             (GUI_ID_USER + 0x04)
#define ID_TEXT_1             (GUI_ID_USER + 0x05)
#define ID_TEXT_2             (GUI_ID_USER + 0x06)
#define ID_IMAGE_0             (GUI_ID_USER + 0x07)

#define ID_IMAGE_0_IMAGE_0       0x00

WM_HWIN CreateLightRang(void);


#endif /* WINDOWS_TESTDLG_H_ */
