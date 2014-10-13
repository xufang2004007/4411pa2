#include <iomanip>  
#include <iostream>
#include "mat.h"

#ifndef __WTF_HEADER__
#define __WTF_HEADER__

// for these two, not used here, but may be used by debuggers
#include <FL/fl_ask.h>
#include <sstream>

void WriteLogFile(const char* szString) {
	FILE* pFile = fopen("logFile.txt", "a");
	fprintf(pFile, "%s\n", szString);
	fclose(pFile);
}

void format(std::ostream& ss) {
	ss << std::fixed << std::setprecision(4);
}

std::ostream& operator<<(std::ostream& ss, const Mat4f& m) {
	format(ss);
	for (int i = 0; i < 4; i++) {
		ss << "[" << m[i][0] << " " << m[i][1] << " " << m[i][2] << " " << m[i][3] << "]" << std::endl;
	}
	return ss;
}

void alert(stringstream& ss) {
	fl_alert(ss.str().c_str());
}

#endif