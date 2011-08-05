#ifndef __INCLUDE_IMAGE_SCANNER_H
#define __INCLUDE_IMAGE_SCANNER_H
/*
Dmscanlib is a software library and standalone application that scans 
and decodes libdmtx compatible test-tubes. It is currently designed 
to decode 12x8 pallets that use 2D data-matrix laser etched test-tubes.
Copyright (C) 2010 Canadian Biosample Repository

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "dmtx.h"

#if defined (WIN32) && ! defined(__MINGW32__)
#include <Windows.h>
#else
typedef void* HANDLE;
#endif

/**
 * This class interfaces with the TWAIN driver to acquire images from the
 * scanner.
 */
class ImgScanner {
public:
	ImgScanner();

	virtual ~ImgScanner();

	virtual bool twainAvailable() = 0;

	virtual bool selectSourceAsDefault() = 0;

	virtual int getScannerCapability() = 0;

	virtual HANDLE acquireImage(unsigned dpi, int brightness, int contrast,
		double top, double left, double bottom, double right) = 0;

	virtual HANDLE acquireFlatbed(unsigned dpi, int brightness, int contrast) = 0;

	virtual DmtxImage* acquireDmtxImage(unsigned dpi, int brightness, int contrast) = 0;

	virtual void freeImage(HANDLE handle) = 0;

	int getErrorCode() { return errorCode; }

protected:
	int brightness;
	int contrast;
	int errorCode;
};

#endif /* __INCLUDE_IMAGE_SCANNER_H */
