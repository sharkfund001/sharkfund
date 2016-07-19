// Copyright (c) 2014-2015 The Dacrs developers
// Copyright (c) 2016 The Sharkfund developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SHARKFUND_CHECKPOINT_H
#define SHARKFUND_CHECKPOINT_H

#include <map>
#include <vector>

class CBlockIndex;
class uint256;

/** Block-chain checkpoints are compiled-in sanity checks.
 * They are updated every release or three.
 */
namespace Checkpoints
{
    // Returns true if block passes checkpoint checks
    bool CheckBlock(int nHeight, const uint256& hash);

    // Return conservative estimate of total number of blocks, 0 if unknown
    int GetTotalBlocksEstimate();

    // Returns last CBlockIndex* in mapBlockIndex that is a checkpoint
    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex);

    double GuessVerificationProgress(CBlockIndex *pindex, bool fSigchecks = true);

    bool AddCheckpoint(int nHeight, uint256 hash);

    bool GetCheckpointByHeight(const int nHeight, std::vector<int> &vCheckpoints);

    bool LoadCheckpoint();

    void GetCheckpointMap(std::map<int, uint256> &checkpoints);

    extern bool fEnabled;
}

#endif
