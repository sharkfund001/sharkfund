// Copyright (c) 2014-2015 The Dacrs developers
// Copyright (c) 2016 The Sharkfund developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "leveldbwrapper.h"

#include "util.h"

#include <boost/filesystem.hpp>
#include <leveldb/cache.h>
#include <leveldb/env.h>
#include <leveldb/filter_policy.h>
#include <memenv.h>
#include "json/json_spirit_value.h"

void HandleError(const leveldb::Status &status) throw(leveldb_error) {
    if (status.ok())
        return;
    LogPrint("INFO","%s\n", status.ToString());
    if (status.IsCorruption())
        throw leveldb_error("Database corrupted");
    if (status.IsIOError())
        throw leveldb_error("Database I/O error");
    if (status.IsNotFound())
        throw leveldb_error("Database entry missing");
    throw leveldb_error("Unknown database error");
}

static leveldb::Options GetOptions(size_t nCacheSize) {
    leveldb::Options options;
    options.block_cache = leveldb::NewLRUCache(nCacheSize / 2);
    options.write_buffer_size = nCacheSize / 4; // up to two write buffers may be held in memory simultaneously
    options.filter_policy = leveldb::NewBloomFilterPolicy(10);
    options.compression = leveldb::kNoCompression;
    options.max_open_files = 64;
    return options;
}

CLevelDBWrapper::CLevelDBWrapper(const boost::filesystem::path &path, size_t nCacheSize, bool fMemory, bool fWipe) {
    penv = NULL;
    readoptions.verify_checksums = true;
    iteroptions.verify_checksums = true;
    iteroptions.fill_cache = false;
    syncoptions.sync = true;
    options = GetOptions(nCacheSize);
    options.create_if_missing = true;
    if (fMemory) {
        penv = leveldb::NewMemEnv(leveldb::Env::Default());
        options.env = penv;
    } else {
        if (fWipe) {
            LogPrint("INFO","Wiping LevelDB in %s\n", path.string());
            leveldb::DestroyDB(path.string(), options);
        }
        TryCreateDirectory(path);
        LogPrint("INFO","Opening LevelDB in %s\n", path.string());
    }
    leveldb::Status status = leveldb::DB::Open(options, path.string(), &pdb);
    HandleError(status);
    LogPrint("INFO","Opened LevelDB successfully\n");
}

CLevelDBWrapper::~CLevelDBWrapper() {
    delete pdb;
    pdb = NULL;
    delete options.filter_policy;
    options.filter_policy = NULL;
    delete options.block_cache;
    options.block_cache = NULL;
    delete penv;
    options.env = NULL;
}

bool CLevelDBWrapper::WriteBatch(CLevelDBBatch &batch, bool fSync) throw(leveldb_error) {
    leveldb::Status status = pdb->Write(fSync ? syncoptions : writeoptions, &batch.batch);
    HandleError(status);
    return true;
}

int64_t CLevelDBWrapper::GetDbCount()
   {
   	leveldb::Iterator *pcursor = NewIterator();
   	int64_t ret = 0;
   	pcursor->SeekToFirst();
   	while (pcursor->Valid()) {
   		boost::this_thread::interruption_point();
   		try {
   			ret++;
   			leveldb::Slice slKey = pcursor->key();
   			leveldb::Slice slValue = pcursor->value();
   			CDataStream ssKey(slKey.data(), slKey.data() + slKey.size(), SER_DISK, CLIENT_VERSION);
   			CDataStream ssValue(slValue.data(), slValue.data() + slValue.size(), SER_DISK, CLIENT_VERSION);
   			LogPrint("db", "Key:%s\n value:%s\n", HexStr(ssKey), HexStr(ssValue));
   			pcursor->Next();

   		} catch (std::exception &e) {
   			 if(pcursor)
   			 delete pcursor;
   			 ERRORMSG("%s : Deserialize or I/O error - %s", __func__, e.what());
   			 return 0;
   		}
   	}
   	delete pcursor;
   	return ret;
   }
