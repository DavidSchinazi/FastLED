#pragma once

#include "namespace.h"
#include "crgb.h"
#include "ptr.h"

FASTLED_NAMESPACE_BEGIN

DECLARE_SMART_PTR(Frame);

class Frame : public Referent {
public:
    explicit Frame(int pixels_per_frame);
    CRGB* rgb() { return mRgb.get(); }
    const CRGB* rgb() const { return mRgb.get(); }

    size_t size() const { return mPixelsCount; }

    
    uint8_t* alpha() { return mAlpha.get(); }
    const uint8_t* alpha() const { return mAlpha.get(); }

    void copy(const Frame& other);
private:
    const size_t mPixelsCount;
    scoped_array<CRGB> mRgb;
    scoped_array<uint8_t> mAlpha;  // Optional alpha channel.

};

FASTLED_NAMESPACE_END
