#include <vector>
#include <iostream>

#include "RRR.h"
#include "debug.h"
#include "common.h"

using namespace indexes;

// Pad blocks with 0 if they don't allign correctly
const symbol_t RRR::PAD_VALUE = 0;

/** Constructs a RRR of specified arity, block size and super block factor. */
RRR::RRR(size_type arity, size_type block_size, size_type s_block_factor) :
    ARITY(arity), BLOCK_SIZE(block_size), SUPER_BLOCK_FACTOR(s_block_factor)
{
    // TODO: Init CountCube
}

/** Builds RRR Sequence from input vector. */
RRRSequence RRR::build(const vector<symbol_t> & seq)
{
    size_type classNum, offset;
    vector<symbol_t> block(BLOCK_SIZE, PAD_VALUE);
    
    // loop over in multiples of BLOCK_SIZE (padded with zeros)
    size_type block_ind = 0;
    vector<symbol_t>::const_iterator it;
    for (it = seq.begin(); it != seq.end(); )
    {
        // build buffer
        block[block_ind++] = *it++;
        
        // Finished block?
        // if block filled or if we got to the end of the vector...
        if ( (block_ind == BLOCK_SIZE) || (it == seq.end()) )
        {
            // insert block into CountCube
            // counts.add(block, &classNum, &offset);
            
            // Reset block to 0
            block.assign(BLOCK_SIZE, PAD_VALUE);
            block_ind = 0;
        }
    }
    
    return RRRSequence();
}
