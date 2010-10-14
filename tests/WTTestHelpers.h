#ifndef WTTESTHELPERS_H
#define WTTESTHELPERS_H

#include <vector>
#include <map>
#include "TestHelpers.h"
#include "indexes/WaveletTree.h"

using namespace std;
using namespace indexes;

template <class T>
bool testCounts(const WaveletTree<T> & wt, map<T, CountVect> counters,
    const basic_string<T> & alphabet, int seq_length, bool isChar)
{
    for (unsigned int i = 0; i < alphabet.length(); i++)
    {
        T sym = alphabet[i];
        // in each position
        for (int j = 0; j < seq_length; j++)
        {
            size_type result = wt.rank(sym, j);
            size_type expected = counters[sym][j];
            if (result != expected)
                if (isChar)
                {
                    TRACE(("***[WTTest] rank('%c', %d) -> %d (expected: %d)\n",
                        sym, j, result, expected));
                }
                else
                {
                    TRACE(("***[WTTest] rank('%d', %d) -> %d (expected: %d)\n",
                        sym, j, result, expected));
                }
            if (result != expected)
                return false;
        }
    }
    return true;
}

bool testCounts(const WaveletTree<int> & wt, const map<int,
    CountVect> & counters, const basic_string<int> & alphabet,
    int seq_length)
{
    return testCounts(wt, counters, alphabet, seq_length, false);
}

bool testCounts(const WaveletTree<char> & wt,
    const map<char, CountVect> & counters, const basic_string<char> & alphabet,
    int seq_length)
{
    return testCounts(wt, counters, alphabet, seq_length, true);
}

template <class T>
bool doCountTest(const basic_string<T> & input, map<T, CountVect> & counters,
    size_type arity, size_type blocksize, size_type s_block_factor)
{
    basic_string<T> alphabet = getAlphabet(input);
    
    WaveletTree<T> wt(input, arity, blocksize, s_block_factor);
    
    return testCounts(wt, counters, alphabet, input.length());
}

#endif