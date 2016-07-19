// Copyright (c) 2011-2013 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#define BOOST_TEST_MODULE Process Test Suite



#include "main.h"
#include "txdb.h"
#include "ui_interface.h"
#include "util.h"
#include <string>

#include "./wallet/wallet.h"


#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>
#include "chainparams.h"
using namespace std;

//extern void noui_connect();

struct TestingSetup {
	TestingSetup() {
			bool bSetDataDir(false);
			for (int i = 1; i < boost::unit_test::framework::master_test_suite().argc; ++i) {
				string strArgv = boost::unit_test::framework::master_test_suite().argv[i];
				if (string::npos != strArgv.find("-datadir=")) {
					const char* newArgv[] = { boost::unit_test::framework::master_test_suite().argv[0], strArgv.c_str() };
					CBaseParams::IntialParams(2, newArgv);
					bSetDataDir = true;
					break;
				}
			}
			if (!bSetDataDir) {
				int argc = 2;
				char findchar;
				#ifdef WIN32
							findchar = '\\';
				#else
							findchar = '/';
				#endif

				string strCurDir = boost::filesystem::initial_path<boost::filesystem::path>().string();
				int index = strCurDir.find_last_of(findchar);
				int count = 3;
				while (count--) {
					index = strCurDir.find_last_of(findchar);
					strCurDir = strCurDir.substr(0, index);

				}
				#ifdef WIN32
							strCurDir += "\\SHARKFUND_test";
							string param = "-datadir=";
							param += strCurDir;
							const char* argv[] = { "D:\\cppwork\\sharkfund\\src\\sharkfundd.exe", param.c_str() };
				#else
							strCurDir +="/SHARKFUND_test";
							string param = "-datadir=";
							param +=strCurDir;
							const char* argv[] = {"D:\\cppwork\\sharkfund\\src\\sharkfundd.exe", param.c_str()};
				#endif
				CBaseParams::IntialParams(argc, argv);
			}
	}
    ~TestingSetup()
    {

    }
};

BOOST_GLOBAL_FIXTURE(TestingSetup);
