#ifndef COUNTCUBE_H
#define COUNTCUBE_H

#include <memory>
#include <vector>
#include "boost/shared_ptr.hpp"
#include "common.h"
#include "CountEntry.h"
#include "IndexMapper.h"

using namespace std;
using namespace boost;

namespace indexes
{

class CountCube
{
private:
    const size_type ARITY;
    const size_type BLOCK_SIZE;
    
    // Dimensions: [Class][Offset]([Symbol][Position])
    // maybe should use boost ptr_vector
    typedef vector<CountEntry> count_table_t;
    typedef shared_ptr<count_table_t> count_table_ptr;
    vector<count_table_ptr> class_table;
    
    typedef IndexMapper<sequence_t> Mapper;
    typedef shared_ptr<Mapper> Mapper_ptr;
    // maybe should pass these in so don't need to seal() it after...
    // inside a { } block so their destructors are called...
    // not a good idea since it makes calling it from the client 
    // harder...
    vector< Mapper_ptr > blockMappers;
    Mapper_ptr classMapper;
    
public:
    CountCube(size_type arity, size_type blocksize);
    void seal(); // delete mappers
    bool add(const sequence_t & block, size_type & classNum,
        size_type & offset);
    size_type rank(size_type classNum, size_type offset, symbol_t symbol,
        size_type position) const;
};

} // end of namespace

#endif // end of include guard
