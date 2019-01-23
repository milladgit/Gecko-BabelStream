
// Copyright (c) 2015-16 Tom Deakin, Simon McIntosh-Smith,
// University of Bristol HPC
//
// For full license terms please see the LICENSE file distributed with this
// source code

#pragma once

#include <iostream>
#include <stdexcept>

#include "Stream.h"

// #include <openacc.h>

#define IMPLEMENTATION_STRING "Gecko"

template <class T>
class GeckoStream : public Stream<T>
{
  protected:
    // Size of arrays
    unsigned int array_size;
    // Device side pointers
    T *a;
    T *b;
    T *c;

    char *exec_location = "LocG1";
    char *exec_pol_chosen = "static";

  public:
    GeckoStream(const unsigned int, int);
    ~GeckoStream();

    virtual void copy() override;
    virtual void add() override;
    virtual void mul() override;
    virtual void triad() override;
    virtual T dot() override;

    virtual void init_arrays(T initA, T initB, T initC) override;
    virtual void read_arrays(std::vector<T>& a, std::vector<T>& b, std::vector<T>& c) override;



};
