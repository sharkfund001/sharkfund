// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2014-2015 The Dacrs developers
// Copyright (c) 2016 The Sharkfund developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "assert.h"
#include "core.h"
#include "protocol.h"
#include "util.h"
#include "key.h"
#include "tx.h"
#include "main.h"

#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string/case_conv.hpp> // for to_lower()
#include <boost/algorithm/string/predicate.hpp> // for startswith() and endswith()
#include <boost/filesystem.hpp>
using namespace boost::assign;
using namespace std;

map<string, string> CBaseParams::m_mapArgs;
map<string, vector<string> > CBaseParams::m_mapMultiArgs;



vector<string> intPubKey_mainNet = {
		"03f5babeb838c0d8ac83d286e893aca445b730cb6fefd1b546350b2c018f6f04e4",
		"03b4c1dcec9f3e3fa5704c6b6c1267b89ddec81e7512150688e450617f963eecc2"
};

vector<string> initPubKey_testNet = { //
		"0340566d2095fc0b4e8a1cda58532daf90fc1a4fe7e6549b5178a09727e1645dbc",
		"02ce5683fc64e57319f5a68d415a8ffeb7740717146794bd0517497b6656d2877e"
};

vector<string> initPubkey_regTest = {
		"0316c8486c8e54b5a6ed6a35780fbb1a77adcbe38261089c69d3483fc4c785271c",
		"03dbccdbff241a6638a9e8d31d64cdadac71e1d57bbb6124acbcb09d5f98ec97bf",
		"02c3b0a46b3c8f83ef42a09052890857ec452975ac612083206dd2a06c07b3945e",
		"03cf65a994a27e2157f48945293618f7b49df9640d3cf2d47a1b9a3af472663e42",
		"02dd8fede0e8521fc7276efb5f6d0fe54e8c8ce33a617a35bdf81f3cb74487dfd9",
		"0200e8b6c791f66635a885577125f7296aa79ee0ba875d085f83417710bbc2e3ae",
};


/*
121.42.185.34  0x22b92a79
120.27.29.28   0x1c1d1b78
120.25.236.66  0x42ec1978
120.25.213.237 0xedd51978
112.124.18.57  0x39127c70
112.124.40.24  0x18287c70
*/

unsigned int pnSeed[] = //
		{0x22b92a79, 0x1c1d1b78, 0x42ec1978, 0xedd51978, 0x39127c70, 0x18287c70};

class CMainParams: public CBaseParams {
public:
	CMainParams() {

// The message start string is designed to be unlikely to occur in normal data.
// The characters are rarely used upper ASCII, not valid as UTF-8, and produce
// a large 4-byte int at any alignment.
		pchMessageStart[0] = 0xfd;
		pchMessageStart[1] = 0xef;
		pchMessageStart[2] = 0x4c;
		pchMessageStart[3] = 0x7d;
		vAlertPubKey =	ParseHex("03a8a1a0356d32ef6f4ff4d855fee848706bdd8f3f2d898016c66606a244dcfea3");
		nDefaultPort = 9533;
		nRPCPort = 9530;
		strDataDir = "main";
		bnProofOfStakeLimit =~arith_uint256(0) >> 10;        //00 3f ff ff
		nSubsidyHalvingInterval = 2590000;

		assert(CreateGenesisRewardTx(genesis.vptx, intPubKey_mainNet));
		genesis.SetHashPrevBlock(uint256());
		genesis.SetHashMerkleRoot(genesis.BuildMerkleTree());
		genesis.SetHashPos(uint256());
		genesis.SetVersion(1);
		genesis.SetTime(1468392092);
		genesis.SetBits(0x1f3fffff);        //00 3f ff
		genesis.SetNonce(108);
		genesis.SetFuelRate(INIT_FUEL_RATES);
		genesis.SetHeight(0);
		genesis.ClearSignature();
		hashGenesisBlock = genesis.GetHash();
		publicKey = "03a8a1a0356d32ef6f4ff4d855fee848706bdd8f3f2d898016c66606a244dcfea3";
//		{
//			cout << "main hashGenesisBlock:\r\n" << hashGenesisBlock.ToString() << endl;
//			cout << "main hashMerkleRoot:\r\n" << genesis.GetHashMerkleRoot().ToString() << endl;
//		}
		assert(hashGenesisBlock == uint256S("0xe543d04fba13042915c713690607f6009eff1772fbdb9779c87b766b615aa67d"));
		assert(genesis.GetHashMerkleRoot() == uint256S("0x5ead19fcc989660c769c6554a876704b53fe27d8a7cfb925c420194dab75a542"));

//      vSeeds.push_back(CDNSSeedData("soypay.org.cn", "seed_cn_0.dspay.org"));
//      vSeeds.push_back(CDNSSeedData("soypay.org.us", "seed_us_0.dspay.org"));

        base58Prefixes[PUBKEY_ADDRESS] = {63};
		base58Prefixes[SCRIPT_ADDRESS] = {70};
		base58Prefixes[SECRET_KEY] = {75};
		base58Prefixes[EXT_PUBLIC_KEY] = {0x04,0x75,0xCB,0x4F};
		base58Prefixes[EXT_SECRET_KEY] = {0x04,0x75,0x2C,0x5A};

		// Convert the pnSeeds array into usable address objects.
		for (unsigned int i = 0; i < ARRAYLEN(pnSeed); i++) {
			// It'll only connect to one or two seed nodes because once it connects,
			// it'll get a pile of addresses with newer timestamps.
			// Seed nodes are given a random 'last seen time' of between one and two
			// weeks ago.
			const int64_t nOneWeek = 7 * 24 * 60 * 60;
			struct in_addr ip;
			memcpy(&ip, &pnSeed[i], sizeof(ip));
			CAddress addr(CService(ip, GetDefaultPort()));
			addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
			vFixedSeeds.push_back(addr);
		}
	}

	virtual const CBlock& GenesisBlock() const {
		return genesis;
	}
	virtual Network NetworkID() const {
		return CBaseParams::MAIN;
	}
	virtual bool InitalConfig() {
		return CBaseParams::InitalConfig();
	}
	virtual int GetBlockMaxNonce() const
	{
		return 1000;
	}
	virtual const vector<CAddress>& FixedSeeds() const {
		return vFixedSeeds;
	}
	virtual bool IsInFixedSeeds(CAddress &addr) {
		vector<CAddress>::iterator iterAddr = find(vFixedSeeds.begin(), vFixedSeeds.end(), addr);
		return iterAddr != vFixedSeeds.end();
	}

protected:
	CBlock genesis;
	vector<CAddress> vFixedSeeds;
};

class CTestNetParams: public CMainParams {
public:
	CTestNetParams() {
		// The message start string is designed to be unlikely to occur in normal data.
		// The characters are rarely used upper ASCII, not valid as UTF-8, and produce
		// a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0x3d;
        pchMessageStart[2] = 0xf3;
        pchMessageStart[3] = 0x40;
		vAlertPubKey =	ParseHex("02aa5121aff0fa463e0d09bbf185c9ba0cd1b222f3ae0c8a9a28fed1ff3708ec9c");
		nDefaultPort = 15540;
		nRPCPort = 15549;
		strDataDir = "testnet";
		publicKey = "02aa5121aff0fa463e0d09bbf185c9ba0cd1b222f3ae0c8a9a28fed1ff3708ec9c";
		// Modify the testnet genesis block so the timestamp is valid for a later start.
		genesis.SetTime(1468392168);
		genesis.SetNonce(99);
		genesis.vptx.clear();
		assert(CreateGenesisRewardTx(genesis.vptx, initPubKey_testNet));
		genesis.SetHashMerkleRoot(genesis.BuildMerkleTree());
		hashGenesisBlock = genesis.GetHash();
		for(auto & item : vFixedSeeds)
			item.SetPort(GetDefaultPort());

//		{
//			cout << "testnet hashGenesisBlock:\r\n" << hashGenesisBlock.ToString() << endl;
//		}
		assert(hashGenesisBlock == uint256S("0x98416a29cd637afce088fbfebbe77e44f78ac80090dd46f8db451707c8637afe"));
//		vSeeds.clear();
//		vSeeds.push_back(CDNSSeedData("bluematt.me", "testnet-seed.bluematt.me"));

		base58Prefixes[PUBKEY_ADDRESS] = {125};
		base58Prefixes[SCRIPT_ADDRESS] = {132};
		base58Prefixes[SECRET_KEY]     = {137};
		base58Prefixes[EXT_PUBLIC_KEY] = {0x06,0x70,0x4F,0x3B};
		base58Prefixes[EXT_SECRET_KEY] = {0x06,0x70,0x2A,0x1A};
	}
	virtual Network NetworkID() const {return CBaseParams::TESTNET;}
	virtual bool InitalConfig()
	{
		CMainParams::InitalConfig();
		fServer = true;
		return true;
	}
	virtual int GetBlockMaxNonce() const
	{
		return 1000;
	}
};
//static CTestNetParams testNetParams;

//
// Regression test
//
class CRegTestParams: public CTestNetParams {
public:
	CRegTestParams() {
		pchMessageStart[0] = 0xfe;
		pchMessageStart[1] = 0x2d;
		pchMessageStart[2] = 0x1f;
		pchMessageStart[3] = 0x24;
		nSubsidyHalvingInterval = 500;
		bnProofOfStakeLimit = ~arith_uint256(0) >> 6;     //target:00000011 11111111 11111111
		genesis.SetTime(1468392211);
		genesis.SetBits(0x2003ffff);
		genesis.SetNonce(68);
		genesis.vptx.clear();
		assert(CreateGenesisRewardTx(genesis.vptx, initPubkey_regTest));
		genesis.SetHashMerkleRoot(genesis.BuildMerkleTree());
		hashGenesisBlock = genesis.GetHash();
		nDefaultPort = 14410;
		nTargetSpacing = 20;
		nTargetTimespan = 30 * 20;
		strDataDir = "regtest";
		{
//			CBigNum bnTarget;
//			bnTarget.SetCompact(genesis.GetBits());
//			cout << "regtest bnTarget:" << bnTarget.getuint256().GetHex() << endl;
//			cout << "regtest hashGenesisBlock:\r\n" << hashGenesisBlock.ToString() << endl;
//			cout << "regtest hashMerkleRoot:\r\n" << genesis.GetHashMerkleRoot().ToString() << endl;
		}
		assert(hashGenesisBlock == uint256S("0x250dd0661b198f099aad7c672d6a946580c54e495f00b24ac24231563e31c8f5"));

		vFixedSeeds.clear();
		vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
	}

	virtual bool RequireRPCPassword() const {
		return false;
	}
	virtual Network NetworkID() const {
		return CBaseParams::REGTEST;
	}
	virtual bool InitalConfig() {
		CTestNetParams::InitalConfig();
		fServer = true;
		return true;
	}
};

/********************************************************************************/
const vector<string> &CBaseParams::GetMultiArgs(const string& strArg) {
	return m_mapMultiArgs[strArg];
}
int CBaseParams::GetArgsSize() {
	return m_mapArgs.size();
}
int CBaseParams::GetMultiArgsSize() {
	return m_mapMultiArgs.size();
}

string CBaseParams::GetArg(const string& strArg, const string& strDefault) {
	if (m_mapArgs.count(strArg))
		return m_mapArgs[strArg];
	return strDefault;
}

int64_t CBaseParams::GetArg(const string& strArg, int64_t nDefault) {
	if (m_mapArgs.count(strArg))
		return atoi64(m_mapArgs[strArg]);
	return nDefault;
}

bool CBaseParams::GetBoolArg(const string& strArg, bool fDefault) {
	if (m_mapArgs.count(strArg)) {
		if (m_mapArgs[strArg].empty())
			return true;
		return (atoi(m_mapArgs[strArg]) != 0);
	}
	return fDefault;
}

bool CBaseParams::SoftSetArg(const string& strArg, const string& strValue) {
	if (m_mapArgs.count(strArg))
		return false;
	m_mapArgs[strArg] = strValue;
	return true;
}

bool CBaseParams::SoftSetArgCover(const string& strArg, const string& strValue) {
	m_mapArgs[strArg] = strValue;
	return true;
}

void CBaseParams::EraseArg(const string& strArgKey) {
	m_mapArgs.erase(strArgKey);
}

bool CBaseParams::SoftSetBoolArg(const string& strArg, bool fValue) {
	if (fValue)
		return SoftSetArg(strArg, string("1"));
	else
		return SoftSetArg(strArg, string("0"));
}

bool CBaseParams::IsArgCount(const string& strArg) {
	if (m_mapArgs.count(strArg)) {
		return true;
	}
	return false;
}

CBaseParams &SysCfg() {
	static shared_ptr<CBaseParams> pParams;

	if (pParams.get() == NULL) {
		bool fRegTest = CBaseParams::GetBoolArg("-regtest", false);
		bool fTestNet = CBaseParams::GetBoolArg("-testnet", false);
		if (fTestNet && fRegTest) {
			fprintf(stderr, "Error: Invalid combination of -regtest and -testnet.\n");
//			assert(0);
		}

		if (fRegTest) {
			//LogPrint("spark", "In Reg Test Net\n");
			pParams = make_shared<CRegTestParams>();
		} else if (fTestNet) {
			//LogPrint("spark", "In Test Net\n");
			pParams = make_shared<CTestNetParams>();
		} else {
			//LogPrint("spark", "In Main Net\n");
			pParams = make_shared<CMainParams>();
		}

	}
	assert(pParams != NULL);
	return *pParams.get();
}

//write for test code
const CBaseParams &SysParamsMain() {
	static std::shared_ptr<CBaseParams> pParams;
	pParams = make_shared<CMainParams>();
	assert(pParams != NULL);
	return *pParams.get();
}

//write for test code
const CBaseParams &SysParamsTest() {
	static std::shared_ptr<CBaseParams> pParams;
	pParams = make_shared<CTestNetParams>();
	assert(pParams != NULL);
	return *pParams.get();
}

//write for test code
const CBaseParams &SysParamsReg() {
	static std::shared_ptr<CBaseParams> pParams;
	pParams = make_shared<CRegTestParams>();
	assert(pParams != NULL);
	return *pParams.get();
}

static void InterpretNegativeSetting(string name, map<string, string>& mapSettingsRet) {
	// interpret -nofoo as -foo=0 (and -nofoo=0 as -foo=1) as long as -foo not set
	if (name.find("-no") == 0) {
		string positive("-");
		positive.append(name.begin() + 3, name.end());
		if (mapSettingsRet.count(positive) == 0) {
			bool value = !SysCfg().GetBoolArg(name, false);
			mapSettingsRet[positive] = (value ? "1" : "0");
		}
	}
}

void CBaseParams::ParseParameters(int argc, const char* const argv[]) {
	m_mapArgs.clear();
	m_mapMultiArgs.clear();
	for (int i = 1; i < argc; i++) {
		string str(argv[i]);
		string strValue;
		size_t is_index = str.find('=');
		if (is_index != string::npos) {
			strValue = str.substr(is_index + 1);
			str = str.substr(0, is_index);
		}
#ifdef WIN32
		boost::to_lower(str);
		if (boost::algorithm::starts_with(str, "/"))
			str = "-" + str.substr(1);
#endif
		if (str[0] != '-')
			break;

		m_mapArgs[str] = strValue;
		m_mapMultiArgs[str].push_back(strValue);
	}

	// New 0.6 features:
//	BOOST_FOREACH(const PAIRTYPE(string,string)& entry, m_mapArgs) {
	for (auto& entry : m_mapArgs) {
		string name = entry.first;

		//  interpret --foo as -foo (as long as both are not set)
		if (name.find("--") == 0) {
			string singleDash(name.begin() + 1, name.end());
			if (m_mapArgs.count(singleDash) == 0)
				m_mapArgs[singleDash] = entry.second;
			name = singleDash;
		}

		// interpret -nofoo as -foo=0 (and -nofoo=0 as -foo=1) as long as -foo not set
		InterpretNegativeSetting(name, m_mapArgs);
	}
#if 0
	for(const auto& tmp:m_mapArgs) {
		printf("key:%s - value:%s\n", tmp.first.c_str(), tmp.second.c_str());
	}
#endif
}

bool CBaseParams::CreateGenesisRewardTx(vector<std::shared_ptr<CBaseTransaction> > &vRewardTx, const vector<string> &vInitPubKey) {
	int length = vInitPubKey.size();
	for (int i = 0; i < length; ++i) {
		int64_t money(0);
		if (i > 0) {
			if (length > 5) {   //regtest net
				money = 400000000 * COIN;
			} else {
				money = 400000000 * COIN;
			}
		}
		shared_ptr<CRewardTransaction> pRewardTx = make_shared<CRewardTransaction>(ParseHex(vInitPubKey[i].c_str()),
				money, 0);
		pRewardTx->nVersion = nTxVersion1;
		if (pRewardTx.get())
			vRewardTx.push_back(pRewardTx);
		else
			return false;
	}
	return true;

};

bool CBaseParams::IntialParams(int argc, const char* const argv[]) {
	ParseParameters(argc, argv);
	if (!boost::filesystem::is_directory(GetDataDir(false))) {
		fprintf(stderr, "Error: Specified data directory \"%s\" does not exist.\n", CBaseParams::m_mapArgs["-datadir"].c_str());
		return false;
	}
	try {
		ReadConfigFile(CBaseParams::m_mapArgs, CBaseParams::m_mapMultiArgs);
	} catch (exception &e) {
		fprintf(stderr, "Error reading configuration file: %s\n", e.what());
		return false;
	}
	return true;
}

int64_t CBaseParams::GetTxFee() const{
     return paytxfee;
}
int64_t CBaseParams::SetDeflautTxFee(int64_t fee)const{
	paytxfee = fee;

	return fee;
}

CBaseParams::CBaseParams() {
	fImporting = false;
	fReindex = false;
	fBenchmark = false;
	fTxIndex = false;
	nIntervalPos = 1;
	nLogmaxsize = 100 * 1024 * 1024;//100M
	nTxCacheHeight = 500;
	nTimeBestReceived = 0;
	nScriptCheckThreads = 0;
	nViewCacheSize = 2000000;
	nTargetSpacing = 60;
	nTargetTimespan = 30 * 60;
	nSubsidyHalvingInterval = 0;
	paytxfee = 10000;
	nDefaultPort = 0;
	fPrintToConsole= 0;
	fPrintToToFile = 0;
	fLogTimestamps = 0;
	fLogPrintFileLine = 0;
	fDebug = 0;
	fDebugAll= 0 ;
	fServer = 0 ;
	fServer = 0;
	nRPCPort = 0;
	bOutPut = false;


}
/********************************************************************************/

