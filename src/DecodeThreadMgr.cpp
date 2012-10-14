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

#include "DmScanLib.h"
#include "DecodeThreadMgr.h"
#include "Decoder.h"
#include "WellDecoder.h"
#include "Dib.h"

#include <algorithm>
#include <memory>
#include <glog/logging.h>

#ifdef WIN32
#   include <windows.h>
#   undef ERROR
#endif

using namespace std;

const unsigned DecodeThreadMgr::THREAD_NUM = 8;

DecodeThreadMgr::DecodeThreadMgr(Decoder & _decoder) : decoder(_decoder) {
}

DecodeThreadMgr::~DecodeThreadMgr() {
}

void DecodeThreadMgr::decodeWells(vector<unique_ptr<WellDecoder> > & wellDecoders) {
	numThreads = wellDecoders.size();
	allThreads.resize(numThreads);

	for (unsigned i = 0; i < numThreads; ++i) {
		//wellDecoders[i]->run();
		VLOG(2) << *wellDecoders[i];
		allThreads.push_back(std::move(wellDecoders[i]));
	}

	threadHandler();
}

void DecodeThreadMgr::threadHandler() {
	unsigned first = 0;
	unsigned last = min(numThreads, THREAD_NUM);

	do {
		threadProcessRange(first, last);
		VLOG(2) << "Threads for cells finished: " << first << "/" << last - 1;

		first = last;
		last = min(last + THREAD_NUM, numThreads);
	} while (first < numThreads);
}

//first is inclusive , last is exclusive
void DecodeThreadMgr::threadProcessRange(unsigned first, unsigned last) {
	for (unsigned int i = first; i < last; i++) {
		allThreads[i]->start();
	}

	for (unsigned int j = first; j < last; j++) {
		allThreads[j]->join();
	}
}
