/////////////////////////////////////////////////////////////////////////////
// Name:            powertabinputstream.h
// Purpose:         Input stream used to deserialize MFC based Power Tab data
// Author:          Brad Larsen
// Modified by:     
// Created:         Dec 19, 2004
// RCS-ID:          
// Copyright:       (c) Brad Larsen
// License:         wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __POWERTABINPUTSTREAM_H__
#define __POWERTABINPUTSTREAM_H__

#include <stdint.h>
#include <fstream>
using std::ifstream;

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "rect.h"
#include "colour.h"
#include "macros.h"

/// Input stream used to deserialize MFC based Power Tab data
class PowerTabInputStream
{
    friend class PowerTabFileHeader;

    // Member Variables
protected:
    ifstream					m_stream;

    // Constructor/Destructor
public:
    PowerTabInputStream(const string& filename);
    ~PowerTabInputStream();

    // Read Functions
    uint32_t ReadCount();
    bool ReadMFCString(string& string);
    bool ReadWin32ColorRef(Colour& color);
    bool ReadMFCRect(Rect& rect);

protected:
    void ReadClassInformation();
    uint32_t ReadMFCStringLength();

    // Error Checking Functions
public:
    /// Checks the current state of the stream
    /// @return True if the stream is OK, false if an error has occurred
    bool CheckState()
    {return !fail();}

public:    

    bool fail()
    {
        return m_stream.fail();
    }

    template <class T>
            bool ReadVector(vector<T*>& vect, uint16_t version)
    {
        uint32_t count = ReadCount();
        CHECK_THAT(CheckState(), false);

        vect.clear();
        if (count > 0)
        {
            vect.reserve(count);
        }

        while (count--)
        {
            ReadClassInformation();
            T* temp = new T();
            temp->Deserialize(*this, version);
            CHECK_THAT(CheckState(), false);
            vect.push_back(temp);
        }
        return true;
    }

    template<class T>
    PowerTabInputStream& operator>>(const T& data)
    {
        m_stream.read((char *)(&data), sizeof(data));
        return *this;
    }
};

#endif
