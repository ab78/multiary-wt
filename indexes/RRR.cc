#include <vector>
#include <cmath>
#include <iostream>

#include "RRR.h"
#include "debug.h"

#include "utility.h"

using namespace indexes;

// Pad blocks with 0 if they don't allign correctly
const symbol_t RRR::PAD_VALUE = 0;

/** Constructs a RRR of specified arity, block size and super block factor. */
RRR::RRR(size_type arity, size_type block_size, size_type s_block_factor) :
    ARITY(arity),
    BLOCK_SIZE(block_size),
    SUPER_BLOCK_FACTOR(s_block_factor),
    countCube(arity, block_size) {}

/** Builds RRR Sequence from input vector. */
RRRSequence RRR::build(const sequence_t & seq)
{
    TRACE(("[RRR.build]\n"));
    size_type classNum, offset;
    sequence_t block(BLOCK_SIZE, PAD_VALUE);
    
    vector<int> classes = vector<int>();
    vector<int> offsets = vector<int>();
    
    // loop over in multiples of BLOCK_SIZE (padded with zeros)
    size_type block_ind = 0;
    sequence_t::const_iterator it;
    
    // shouldn't be calling with an empty sequence
    myAssert(seq.begin() != seq.end());
    for (it = seq.begin(); it != seq.end(); )
    {
        // build buffer
        block[block_ind++] = *it++;
        
        // Finished block?
        // if block filled or if we got to the end of the vector...
        if ( (block_ind == BLOCK_SIZE) || (it == seq.end()) )
        {
            // insert block into CountCube
            countCube.add(block, classNum, offset);
            
            // add to the classes and offsets vectors...
            classes.push_back(classNum);
            offsets.push_back(offset);
            
            // Reset block to 0
            block.assign(BLOCK_SIZE, PAD_VALUE);
            block_ind = 0;
        }
    }
    
    return RRRSequence(classes, offsets, ARITY, BLOCK_SIZE,
        SUPER_BLOCK_FACTOR, countCube);
}

size_type RRR::rank(symbol_t symbol, size_type position,
    const RRRSequence & seq) const
{
    // TODO : update this so its not so stupid
    int a = sizeof(symbol); a=sizeof(position); a=sizeof(seq);
    return 0;
}

RRRSequence::RRRSequence(const vector<int> & classes_in,
    const vector<int> & offsets_in, const size_type arity,
    const size_type blocksize, const size_type s_block_factor, const CountCube
    & cc) :
    // these will have to be constructed in smarter ways :)
    // like, store a number to say how many bit are required for the classes?
    // and packing the offsets
    classes(classes_in), offsets(offsets_in)
{
    // these really must be the same length...
    myAssert(classes.size() == offsets.size());
    
    const unsigned int NUM_BLOCKS(classes.size());
    const int num_super_blocks(ceil(NUM_BLOCKS / (float)s_block_factor));
    TRACE(( "[RRRSequence.CTOR] Num Super Blocks: %d\n", num_super_blocks ));
    
    // Z = sym (arity)
    // Y = super block (num_super_blocks)
    // X = block (s_block_factor) // arranged this way for caching
    intermediates = inter_t(new int[arity * num_super_blocks * s_block_factor]);
    
    // Populate intermediates table
    
    // declare running total vector
    vector<int> totals(arity, 0);
    // for each (C, O) at i:
    for (size_type i = 0; i < NUM_BLOCKS; i++)
    {
        // superblock i/s_block_factor
        size_type super_block_idx = i / s_block_factor;
        size_type block_idx  = i % s_block_factor;
        size_type classNum = classes[i];
        size_type offset = offsets[i];
        
        // get last value in block, for each symbol
        for (size_type sym = 0; sym < arity; sym++)
        {
            size_type rank = cc.rank(classNum, offset, sym, 
                blocksize - 1);
            
            // update running totals for super-block
            totals[sym] += rank;
            intermediates[0] = totals[sym];
        }
        
        // Super block boundary:
        if (i != 0 && block_idx == 0)
        {
            // reset running total
            totals.assign(arity, 0);
        }
    }
}
