
#pragma once

#include <stdint.h>
#include "chsv.h"
#include "namespace.h"
#include "color.h"
#include "lib8tion/types.h"
#include "force_inline.h"

// Whether to allow HD_COLOR_MIXING
#ifndef FASTLED_HD_COLOR_MIXING
#ifdef __AVR__
// Saves some memory on these constrained devices.
#define FASTLED_HD_COLOR_MIXING 0
#else
#define FASTLED_HD_COLOR_MIXING 1
#endif  // __AVR__
#endif  // FASTLED_HD_COLOR_MIXING

FASTLED_NAMESPACE_BEGIN

struct CRGB;

/// @defgroup PixelTypes Pixel Data Types (CRGB/CHSV)
/// @brief Structs that hold pixel color data
/// @{

/// Forward declaration of hsv2rgb_rainbow here,
/// to avoid circular dependencies.
extern void hsv2rgb_rainbow( const CHSV& hsv, CRGB& rgb);


/// Representation of an RGB pixel (Red, Green, Blue)
struct CRGB {
    union {
        struct {
            union {
                uint8_t r;    ///< Red channel value
                uint8_t red;  ///< @copydoc r
            };
            union {
                uint8_t g;      ///< Green channel value
                uint8_t green;  ///< @copydoc g
            };
            union {
                uint8_t b;     ///< Blue channel value
                uint8_t blue;  ///< @copydoc b
            };
        };
        /// Access the red, green, and blue data as an array.
        /// Where:
        /// * `raw[0]` is the red value
        /// * `raw[1]` is the green value
        /// * `raw[2]` is the blue value
        uint8_t raw[3];
    };

    static CRGB blend(const CRGB& p1, const CRGB& p2, fract8 amountOfP2);

    /// Array access operator to index into the CRGB object
    /// @param x the index to retrieve (0-2)
    /// @returns the CRGB::raw value for the given index
    FASTLED_FORCE_INLINE uint8_t& operator[] (uint8_t x)
    {
        return raw[x];
    }

    /// Array access operator to index into the CRGB object
    /// @param x the index to retrieve (0-2)
    /// @returns the CRGB::raw value for the given index
    FASTLED_FORCE_INLINE const uint8_t& operator[] (uint8_t x) const
    {
        return raw[x];
    }

    /// Default constructor
    /// @warning Default values are UNITIALIZED!
    FASTLED_FORCE_INLINE CRGB() = default;

    /// Allow construction from red, green, and blue
    /// @param ir input red value
    /// @param ig input green value
    /// @param ib input blue value
    constexpr CRGB(uint8_t ir, uint8_t ig, uint8_t ib)  __attribute__((always_inline))
        : r(ir), g(ig), b(ib)
    {
    }

    /// Allow construction from 32-bit (really 24-bit) bit 0xRRGGBB color code
    /// @param colorcode a packed 24 bit color code
    constexpr CRGB(uint32_t colorcode)  __attribute__((always_inline))
    : r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF), b((colorcode >> 0) & 0xFF)
    {
    }

    constexpr uint32_t as_uint32_t() const {
        return uint32_t(0xff000000) |
               (uint32_t{r} << 16) |
               (uint32_t{g} << 8) |
               uint32_t{b};
    }

    /// Allow construction from a LEDColorCorrection enum
    /// @param colorcode an LEDColorCorrect enumeration value
    constexpr CRGB(LEDColorCorrection colorcode) __attribute__((always_inline))
    : r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF), b((colorcode >> 0) & 0xFF)
    {
    }

    /// Allow construction from a ColorTemperature enum
    /// @param colorcode an ColorTemperature enumeration value
    constexpr CRGB(ColorTemperature colorcode) __attribute__((always_inline))
    : r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF), b((colorcode >> 0) & 0xFF)
    {
    }

    /// Allow copy construction
    FASTLED_FORCE_INLINE CRGB(const CRGB& rhs) = default;

    /// Allow construction from a CHSV color
    FASTLED_FORCE_INLINE CRGB(const CHSV& rhs)
    {
        hsv2rgb_rainbow( rhs, *this);
    }

    /// Allow assignment from one RGB struct to another
    FASTLED_FORCE_INLINE CRGB& operator= (const CRGB& rhs) = default;

    /// Allow assignment from 32-bit (really 24-bit) 0xRRGGBB color code
    /// @param colorcode a packed 24 bit color code
    FASTLED_FORCE_INLINE CRGB& operator= (const uint32_t colorcode)
    {
        r = (colorcode >> 16) & 0xFF;
        g = (colorcode >>  8) & 0xFF;
        b = (colorcode >>  0) & 0xFF;
        return *this;
    }

    /// Allow assignment from red, green, and blue
    /// @param nr new red value
    /// @param ng new green value
    /// @param nb new blue value
    FASTLED_FORCE_INLINE CRGB& setRGB (uint8_t nr, uint8_t ng, uint8_t nb)
    {
        r = nr;
        g = ng;
        b = nb;
        return *this;
    }

    /// Allow assignment from hue, saturation, and value
    /// @param hue color hue
    /// @param sat color saturation
    /// @param val color value (brightness)
    FASTLED_FORCE_INLINE CRGB& setHSV (uint8_t hue, uint8_t sat, uint8_t val)
    {
        hsv2rgb_rainbow( CHSV(hue, sat, val), *this);
        return *this;
    }

    /// Allow assignment from just a hue.
    /// Saturation and value (brightness) are set automatically to max.
    /// @param hue color hue
    FASTLED_FORCE_INLINE CRGB& setHue (uint8_t hue)
    {
        hsv2rgb_rainbow( CHSV(hue, 255, 255), *this);
        return *this;
    }

    /// Allow assignment from HSV color
    FASTLED_FORCE_INLINE CRGB& operator= (const CHSV& rhs)
    {
        hsv2rgb_rainbow( rhs, *this);
        return *this;
    }

    /// Allow assignment from 32-bit (really 24-bit) 0xRRGGBB color code
    /// @param colorcode a packed 24 bit color code
    FASTLED_FORCE_INLINE CRGB& setColorCode (uint32_t colorcode)
    {
        r = (colorcode >> 16) & 0xFF;
        g = (colorcode >>  8) & 0xFF;
        b = (colorcode >>  0) & 0xFF;
        return *this;
    }


    /// Add one CRGB to another, saturating at 0xFF for each channel
    FASTLED_FORCE_INLINE CRGB& operator+= (const CRGB& rhs);

    /// Add a constant to each channel, saturating at 0xFF.
    /// @note This is NOT an operator+= overload because the compiler
    /// can't usefully decide when it's being passed a 32-bit
    /// constant (e.g. CRGB::Red) and an 8-bit one (CRGB::Blue)
    FASTLED_FORCE_INLINE CRGB& addToRGB (uint8_t d);

    /// Subtract one CRGB from another, saturating at 0x00 for each channel
    FASTLED_FORCE_INLINE CRGB& operator-= (const CRGB& rhs);

    /// Subtract a constant from each channel, saturating at 0x00.
    /// @note This is NOT an operator+= overload because the compiler
    /// can't usefully decide when it's being passed a 32-bit
    /// constant (e.g. CRGB::Red) and an 8-bit one (CRGB::Blue)
    FASTLED_FORCE_INLINE CRGB& subtractFromRGB(uint8_t d);

    /// Subtract a constant of '1' from each channel, saturating at 0x00
    FASTLED_FORCE_INLINE CRGB& operator-- ();

    /// @copydoc operator--
    FASTLED_FORCE_INLINE CRGB operator-- (int );

    /// Add a constant of '1' from each channel, saturating at 0xFF
    FASTLED_FORCE_INLINE CRGB& operator++ ();

    /// @copydoc operator++
    FASTLED_FORCE_INLINE CRGB operator++ (int );

    /// Divide each of the channels by a constant
    FASTLED_FORCE_INLINE CRGB& operator/= (uint8_t d )
    {
        r /= d;
        g /= d;
        b /= d;
        return *this;
    }

    /// Right shift each of the channels by a constant
    FASTLED_FORCE_INLINE CRGB& operator>>= (uint8_t d)
    {
        r >>= d;
        g >>= d;
        b >>= d;
        return *this;
    }

    /// Multiply each of the channels by a constant,
    /// saturating each channel at 0xFF.
    FASTLED_FORCE_INLINE CRGB& operator*= (uint8_t d);

    /// Scale down a RGB to N/256ths of it's current brightness using
    /// "video" dimming rules. "Video" dimming rules means that unless the scale factor
    /// is ZERO each channel is guaranteed NOT to dim down to zero.  If it's already
    /// nonzero, it'll stay nonzero, even if that means the hue shifts a little
    /// at low brightness levels.
    /// @see nscale8x3_video
    FASTLED_FORCE_INLINE CRGB& nscale8_video (uint8_t scaledown);

    /// %= is a synonym for nscale8_video().  Think of it is scaling down
    /// by "a percentage"
    FASTLED_FORCE_INLINE CRGB& operator%= (uint8_t scaledown);

    /// fadeLightBy is a synonym for nscale8_video(), as a fade instead of a scale
    /// @param fadefactor the amount to fade, sent to nscale8_video() as (255 - fadefactor)
    FASTLED_FORCE_INLINE CRGB& fadeLightBy (uint8_t fadefactor );

    /// Scale down a RGB to N/256ths of its current brightness, using
    /// "plain math" dimming rules. "Plain math" dimming rules means that the low light
    /// levels may dim all the way to 100% black.
    /// @see nscale8x3
    FASTLED_FORCE_INLINE CRGB& nscale8 (uint8_t scaledown );

    /// Scale down a RGB to N/256ths of its current brightness, using
    /// "plain math" dimming rules. "Plain math" dimming rules means that the low light
    /// levels may dim all the way to 100% black.
    /// @see ::scale8
    FASTLED_FORCE_INLINE CRGB& nscale8 (const CRGB & scaledown );

    constexpr CRGB nscale8_constexpr (const CRGB scaledown ) const;


    /// Return a CRGB object that is a scaled down version of this object
    FASTLED_FORCE_INLINE CRGB scale8 (uint8_t scaledown ) const;

    /// Return a CRGB object that is a scaled down version of this object
    FASTLED_FORCE_INLINE CRGB scale8 (const CRGB & scaledown ) const;

    /// fadeToBlackBy is a synonym for nscale8(), as a fade instead of a scale
    /// @param fadefactor the amount to fade, sent to nscale8() as (255 - fadefactor)
    FASTLED_FORCE_INLINE CRGB& fadeToBlackBy (uint8_t fadefactor );

    /// "or" operator brings each channel up to the higher of the two values
    FASTLED_FORCE_INLINE CRGB& operator|= (const CRGB& rhs )
    {
        if( rhs.r > r) r = rhs.r;
        if( rhs.g > g) g = rhs.g;
        if( rhs.b > b) b = rhs.b;
        return *this;
    }

    /// @copydoc operator|=
    FASTLED_FORCE_INLINE CRGB& operator|= (uint8_t d )
    {
        if( d > r) r = d;
        if( d > g) g = d;
        if( d > b) b = d;
        return *this;
    }

    /// "and" operator brings each channel down to the lower of the two values
    FASTLED_FORCE_INLINE CRGB& operator&= (const CRGB& rhs )
    {
        if( rhs.r < r) r = rhs.r;
        if( rhs.g < g) g = rhs.g;
        if( rhs.b < b) b = rhs.b;
        return *this;
    }

    /// @copydoc operator&=
    FASTLED_FORCE_INLINE CRGB& operator&= (uint8_t d )
    {
        if( d < r) r = d;
        if( d < g) g = d;
        if( d < b) b = d;
        return *this;
    }

    /// This allows testing a CRGB for zero-ness
    FASTLED_FORCE_INLINE explicit operator bool() const
    {
        return r || g || b;
    }

    /// Converts a CRGB to a 32-bit color having an alpha of 255.
    FASTLED_FORCE_INLINE explicit operator uint32_t() const
    {
        return uint32_t(0xff000000) |
               (uint32_t{r} << 16) |
               (uint32_t{g} << 8) |
               uint32_t{b};
    }

    /// Converts a CRGB to a 32-bit color having an alpha of 255.
    FASTLED_FORCE_INLINE explicit operator const uint32_t() const
    {
        return uint32_t(0xff000000) |
               (uint32_t{r} << 16) |
               (uint32_t{g} << 8) |
               uint32_t{b};
    }

    /// Invert each channel
    FASTLED_FORCE_INLINE CRGB operator- () const
    {
        CRGB retval;
        retval.r = 255 - r;
        retval.g = 255 - g;
        retval.b = 255 - b;
        return retval;
    }

#if (defined SmartMatrix_h || defined SmartMatrix3_h)
    /// Convert to an rgb24 object, used with the SmartMatrix library
    /// @see https://github.com/pixelmatix/SmartMatrix
    operator rgb24() const {
        rgb24 ret;
        ret.red = r;
        ret.green = g;
        ret.blue = b;
        return ret;
    }
#endif

    /// Get the "luma" of a CRGB object. In other words, roughly how much
    /// light the CRGB pixel is putting out (from 0 to 255).
    FASTLED_FORCE_INLINE uint8_t getLuma() const;

    /// Get the average of the R, G, and B values
    FASTLED_FORCE_INLINE uint8_t getAverageLight() const;

    /// Maximize the brightness of this CRGB object.
    /// This makes the individual color channels as bright as possible
    /// while keeping the same value differences between channels.
    /// @note This does not keep the same ratios between channels,
    /// just the same difference in absolute values.
    FASTLED_FORCE_INLINE void maximizeBrightness( uint8_t limit = 255 )  {
        uint8_t max = red;
        if( green > max) max = green;
        if( blue > max) max = blue;

        // stop div/0 when color is black
        if(max > 0) {
            uint16_t factor = ((uint16_t)(limit) * 256) / max;
            red =   (red   * factor) / 256;
            green = (green * factor) / 256;
            blue =  (blue  * factor) / 256;
        }
    }

    /// Calculates the combined color adjustment to the LEDs at a given scale, color correction, and color temperature
    /// @param scale the scale value for the RGB data (i.e. brightness)
    /// @param colorCorrection color correction to apply
    /// @param colorTemperature color temperature to apply
    /// @returns a CRGB object representing the adjustment, including color correction and color temperature
    static CRGB computeAdjustment(uint8_t scale, const CRGB & colorCorrection, const CRGB & colorTemperature);

    /// Return a new CRGB object after performing a linear interpolation between this object and the passed in object
    FASTLED_FORCE_INLINE CRGB lerp8( const CRGB& other, fract8 frac) const;

    /// @copydoc lerp8
    FASTLED_FORCE_INLINE CRGB lerp16( const CRGB& other, fract16 frac) const;
    /// Returns 0 or 1, depending on the lowest bit of the sum of the color components.
    FASTLED_FORCE_INLINE uint8_t getParity()
    {
        uint8_t sum = r + g + b;
        return (sum & 0x01);
    }

    /// Adjusts the color in the smallest way possible
    /// so that the parity of the coloris now the desired value.
    /// This allows you to "hide" one bit of information in the color.
    ///
    /// Ideally, we find one color channel which already
    /// has data in it, and modify just that channel by one.
    /// We don't want to light up a channel that's black
    /// if we can avoid it, and if the pixel is 'grayscale',
    /// (meaning that R==G==B), we modify all three channels
    /// at once, to preserve the neutral hue.
    ///
    /// There's no such thing as a free lunch; in many cases
    /// this "hidden bit" may actually be visible, but this
    /// code makes reasonable efforts to hide it as much
    /// as is reasonably possible.
    ///
    /// Also, an effort is made to make it such that
    /// repeatedly setting the parity to different values
    /// will not cause the color to "drift". Toggling
    /// the parity twice should generally result in the
    /// original color again.
    ///
    FASTLED_FORCE_INLINE void setParity( uint8_t parity)
    {
        uint8_t curparity = getParity();

        if( parity == curparity) return;

        if( parity ) {
            // going 'up'
            if( (b > 0) && (b < 255)) {
                if( r == g && g == b) {
                    ++r;
                    ++g;
                }
                ++b;
            } else if( (r > 0) && (r < 255)) {
                ++r;
            } else if( (g > 0) && (g < 255)) {
                ++g;
            } else {
                if( r == g && g == b) {
                    r ^= 0x01;
                    g ^= 0x01;
                }
                b ^= 0x01;
            }
        } else {
            // going 'down'
            if( b > 1) {
                if( r == g && g == b) {
                    --r;
                    --g;
                }
                --b;
            } else if( g > 1) {
                --g;
            } else if( r > 1) {
                --r;
            } else {
                if( r == g && g == b) {
                    r ^= 0x01;
                    g ^= 0x01;
                }
                b ^= 0x01;
            }
        }
    }

    /// Predefined RGB colors
    typedef enum {
        AliceBlue=0xF0F8FF,             ///< @htmlcolorblock{F0F8FF}
        Amethyst=0x9966CC,              ///< @htmlcolorblock{9966CC}
        AntiqueWhite=0xFAEBD7,          ///< @htmlcolorblock{FAEBD7}
        Aqua=0x00FFFF,                  ///< @htmlcolorblock{00FFFF}
        Aquamarine=0x7FFFD4,            ///< @htmlcolorblock{7FFFD4}
        Azure=0xF0FFFF,                 ///< @htmlcolorblock{F0FFFF}
        Beige=0xF5F5DC,                 ///< @htmlcolorblock{F5F5DC}
        Bisque=0xFFE4C4,                ///< @htmlcolorblock{FFE4C4}
        Black=0x000000,                 ///< @htmlcolorblock{000000}
        BlanchedAlmond=0xFFEBCD,        ///< @htmlcolorblock{FFEBCD}
        Blue=0x0000FF,                  ///< @htmlcolorblock{0000FF}
        BlueViolet=0x8A2BE2,            ///< @htmlcolorblock{8A2BE2}
        Brown=0xA52A2A,                 ///< @htmlcolorblock{A52A2A}
        BurlyWood=0xDEB887,             ///< @htmlcolorblock{DEB887}
        CadetBlue=0x5F9EA0,             ///< @htmlcolorblock{5F9EA0}
        Chartreuse=0x7FFF00,            ///< @htmlcolorblock{7FFF00}
        Chocolate=0xD2691E,             ///< @htmlcolorblock{D2691E}
        Coral=0xFF7F50,                 ///< @htmlcolorblock{FF7F50}
        CornflowerBlue=0x6495ED,        ///< @htmlcolorblock{6495ED}
        Cornsilk=0xFFF8DC,              ///< @htmlcolorblock{FFF8DC}
        Crimson=0xDC143C,               ///< @htmlcolorblock{DC143C}
        Cyan=0x00FFFF,                  ///< @htmlcolorblock{00FFFF}
        DarkBlue=0x00008B,              ///< @htmlcolorblock{00008B}
        DarkCyan=0x008B8B,              ///< @htmlcolorblock{008B8B}
        DarkGoldenrod=0xB8860B,         ///< @htmlcolorblock{B8860B}
        DarkGray=0xA9A9A9,              ///< @htmlcolorblock{A9A9A9}
        DarkGrey=0xA9A9A9,              ///< @htmlcolorblock{A9A9A9}
        DarkGreen=0x006400,             ///< @htmlcolorblock{006400}
        DarkKhaki=0xBDB76B,             ///< @htmlcolorblock{BDB76B}
        DarkMagenta=0x8B008B,           ///< @htmlcolorblock{8B008B}
        DarkOliveGreen=0x556B2F,        ///< @htmlcolorblock{556B2F}
        DarkOrange=0xFF8C00,            ///< @htmlcolorblock{FF8C00}
        DarkOrchid=0x9932CC,            ///< @htmlcolorblock{9932CC}
        DarkRed=0x8B0000,               ///< @htmlcolorblock{8B0000}
        DarkSalmon=0xE9967A,            ///< @htmlcolorblock{E9967A}
        DarkSeaGreen=0x8FBC8F,          ///< @htmlcolorblock{8FBC8F}
        DarkSlateBlue=0x483D8B,         ///< @htmlcolorblock{483D8B}
        DarkSlateGray=0x2F4F4F,         ///< @htmlcolorblock{2F4F4F}
        DarkSlateGrey=0x2F4F4F,         ///< @htmlcolorblock{2F4F4F}
        DarkTurquoise=0x00CED1,         ///< @htmlcolorblock{00CED1}
        DarkViolet=0x9400D3,            ///< @htmlcolorblock{9400D3}
        DeepPink=0xFF1493,              ///< @htmlcolorblock{FF1493}
        DeepSkyBlue=0x00BFFF,           ///< @htmlcolorblock{00BFFF}
        DimGray=0x696969,               ///< @htmlcolorblock{696969}
        DimGrey=0x696969,               ///< @htmlcolorblock{696969}
        DodgerBlue=0x1E90FF,            ///< @htmlcolorblock{1E90FF}
        FireBrick=0xB22222,             ///< @htmlcolorblock{B22222}
        FloralWhite=0xFFFAF0,           ///< @htmlcolorblock{FFFAF0}
        ForestGreen=0x228B22,           ///< @htmlcolorblock{228B22}
        Fuchsia=0xFF00FF,               ///< @htmlcolorblock{FF00FF}
        Gainsboro=0xDCDCDC,             ///< @htmlcolorblock{DCDCDC}
        GhostWhite=0xF8F8FF,            ///< @htmlcolorblock{F8F8FF}
        Gold=0xFFD700,                  ///< @htmlcolorblock{FFD700}
        Goldenrod=0xDAA520,             ///< @htmlcolorblock{DAA520}
        Gray=0x808080,                  ///< @htmlcolorblock{808080}
        Grey=0x808080,                  ///< @htmlcolorblock{808080}
        Green=0x008000,                 ///< @htmlcolorblock{008000}
        GreenYellow=0xADFF2F,           ///< @htmlcolorblock{ADFF2F}
        Honeydew=0xF0FFF0,              ///< @htmlcolorblock{F0FFF0}
        HotPink=0xFF69B4,               ///< @htmlcolorblock{FF69B4}
        IndianRed=0xCD5C5C,             ///< @htmlcolorblock{CD5C5C}
        Indigo=0x4B0082,                ///< @htmlcolorblock{4B0082}
        Ivory=0xFFFFF0,                 ///< @htmlcolorblock{FFFFF0}
        Khaki=0xF0E68C,                 ///< @htmlcolorblock{F0E68C}
        Lavender=0xE6E6FA,              ///< @htmlcolorblock{E6E6FA}
        LavenderBlush=0xFFF0F5,         ///< @htmlcolorblock{FFF0F5}
        LawnGreen=0x7CFC00,             ///< @htmlcolorblock{7CFC00}
        LemonChiffon=0xFFFACD,          ///< @htmlcolorblock{FFFACD}
        LightBlue=0xADD8E6,             ///< @htmlcolorblock{ADD8E6}
        LightCoral=0xF08080,            ///< @htmlcolorblock{F08080}
        LightCyan=0xE0FFFF,             ///< @htmlcolorblock{E0FFFF}
        LightGoldenrodYellow=0xFAFAD2,  ///< @htmlcolorblock{FAFAD2}
        LightGreen=0x90EE90,            ///< @htmlcolorblock{90EE90}
        LightGrey=0xD3D3D3,             ///< @htmlcolorblock{D3D3D3}
        LightPink=0xFFB6C1,             ///< @htmlcolorblock{FFB6C1}
        LightSalmon=0xFFA07A,           ///< @htmlcolorblock{FFA07A}
        LightSeaGreen=0x20B2AA,         ///< @htmlcolorblock{20B2AA}
        LightSkyBlue=0x87CEFA,          ///< @htmlcolorblock{87CEFA}
        LightSlateGray=0x778899,        ///< @htmlcolorblock{778899}
        LightSlateGrey=0x778899,        ///< @htmlcolorblock{778899}
        LightSteelBlue=0xB0C4DE,        ///< @htmlcolorblock{B0C4DE}
        LightYellow=0xFFFFE0,           ///< @htmlcolorblock{FFFFE0}
        Lime=0x00FF00,                  ///< @htmlcolorblock{00FF00}
        LimeGreen=0x32CD32,             ///< @htmlcolorblock{32CD32}
        Linen=0xFAF0E6,                 ///< @htmlcolorblock{FAF0E6}
        Magenta=0xFF00FF,               ///< @htmlcolorblock{FF00FF}
        Maroon=0x800000,                ///< @htmlcolorblock{800000}
        MediumAquamarine=0x66CDAA,      ///< @htmlcolorblock{66CDAA}
        MediumBlue=0x0000CD,            ///< @htmlcolorblock{0000CD}
        MediumOrchid=0xBA55D3,          ///< @htmlcolorblock{BA55D3}
        MediumPurple=0x9370DB,          ///< @htmlcolorblock{9370DB}
        MediumSeaGreen=0x3CB371,        ///< @htmlcolorblock{3CB371}
        MediumSlateBlue=0x7B68EE,       ///< @htmlcolorblock{7B68EE}
        MediumSpringGreen=0x00FA9A,     ///< @htmlcolorblock{00FA9A}
        MediumTurquoise=0x48D1CC,       ///< @htmlcolorblock{48D1CC}
        MediumVioletRed=0xC71585,       ///< @htmlcolorblock{C71585}
        MidnightBlue=0x191970,          ///< @htmlcolorblock{191970}
        MintCream=0xF5FFFA,             ///< @htmlcolorblock{F5FFFA}
        MistyRose=0xFFE4E1,             ///< @htmlcolorblock{FFE4E1}
        Moccasin=0xFFE4B5,              ///< @htmlcolorblock{FFE4B5}
        NavajoWhite=0xFFDEAD,           ///< @htmlcolorblock{FFDEAD}
        Navy=0x000080,                  ///< @htmlcolorblock{000080}
        OldLace=0xFDF5E6,               ///< @htmlcolorblock{FDF5E6}
        Olive=0x808000,                 ///< @htmlcolorblock{808000}
        OliveDrab=0x6B8E23,             ///< @htmlcolorblock{6B8E23}
        Orange=0xFFA500,                ///< @htmlcolorblock{FFA500}
        OrangeRed=0xFF4500,             ///< @htmlcolorblock{FF4500}
        Orchid=0xDA70D6,                ///< @htmlcolorblock{DA70D6}
        PaleGoldenrod=0xEEE8AA,         ///< @htmlcolorblock{EEE8AA}
        PaleGreen=0x98FB98,             ///< @htmlcolorblock{98FB98}
        PaleTurquoise=0xAFEEEE,         ///< @htmlcolorblock{AFEEEE}
        PaleVioletRed=0xDB7093,         ///< @htmlcolorblock{DB7093}
        PapayaWhip=0xFFEFD5,            ///< @htmlcolorblock{FFEFD5}
        PeachPuff=0xFFDAB9,             ///< @htmlcolorblock{FFDAB9}
        Peru=0xCD853F,                  ///< @htmlcolorblock{CD853F}
        Pink=0xFFC0CB,                  ///< @htmlcolorblock{FFC0CB}
        Plaid=0xCC5533,                 ///< @htmlcolorblock{CC5533}
        Plum=0xDDA0DD,                  ///< @htmlcolorblock{DDA0DD}
        PowderBlue=0xB0E0E6,            ///< @htmlcolorblock{B0E0E6}
        Purple=0x800080,                ///< @htmlcolorblock{800080}
        Red=0xFF0000,                   ///< @htmlcolorblock{FF0000}
        RosyBrown=0xBC8F8F,             ///< @htmlcolorblock{BC8F8F}
        RoyalBlue=0x4169E1,             ///< @htmlcolorblock{4169E1}
        SaddleBrown=0x8B4513,           ///< @htmlcolorblock{8B4513}
        Salmon=0xFA8072,                ///< @htmlcolorblock{FA8072}
        SandyBrown=0xF4A460,            ///< @htmlcolorblock{F4A460}
        SeaGreen=0x2E8B57,              ///< @htmlcolorblock{2E8B57}
        Seashell=0xFFF5EE,              ///< @htmlcolorblock{FFF5EE}
        Sienna=0xA0522D,                ///< @htmlcolorblock{A0522D}
        Silver=0xC0C0C0,                ///< @htmlcolorblock{C0C0C0}
        SkyBlue=0x87CEEB,               ///< @htmlcolorblock{87CEEB}
        SlateBlue=0x6A5ACD,             ///< @htmlcolorblock{6A5ACD}
        SlateGray=0x708090,             ///< @htmlcolorblock{708090}
        SlateGrey=0x708090,             ///< @htmlcolorblock{708090}
        Snow=0xFFFAFA,                  ///< @htmlcolorblock{FFFAFA}
        SpringGreen=0x00FF7F,           ///< @htmlcolorblock{00FF7F}
        SteelBlue=0x4682B4,             ///< @htmlcolorblock{4682B4}
        Tan=0xD2B48C,                   ///< @htmlcolorblock{D2B48C}
        Teal=0x008080,                  ///< @htmlcolorblock{008080}
        Thistle=0xD8BFD8,               ///< @htmlcolorblock{D8BFD8}
        Tomato=0xFF6347,                ///< @htmlcolorblock{FF6347}
        Turquoise=0x40E0D0,             ///< @htmlcolorblock{40E0D0}
        Violet=0xEE82EE,                ///< @htmlcolorblock{EE82EE}
        Wheat=0xF5DEB3,                 ///< @htmlcolorblock{F5DEB3}
        White=0xFFFFFF,                 ///< @htmlcolorblock{FFFFFF}
        WhiteSmoke=0xF5F5F5,            ///< @htmlcolorblock{F5F5F5}
        Yellow=0xFFFF00,                ///< @htmlcolorblock{FFFF00}
        YellowGreen=0x9ACD32,           ///< @htmlcolorblock{9ACD32}

        // LED RGB color that roughly approximates
        // the color of incandescent fairy lights,
        // assuming that you're using FastLED
        // color correction on your LEDs (recommended).
        FairyLight=0xFFE42D,           ///< @htmlcolorblock{FFE42D}

        // If you are using no color correction, use this
        FairyLightNCC=0xFF9D2A         ///< @htmlcolorblock{FFE42D}

    } HTMLColorCode;
};


/// Check if two CRGB objects have the same color data
FASTLED_FORCE_INLINE bool operator== (const CRGB& lhs, const CRGB& rhs)
{
    return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b);
}

/// Check if two CRGB objects do *not* have the same color data
FASTLED_FORCE_INLINE bool operator!= (const CRGB& lhs, const CRGB& rhs)
{
    return !(lhs == rhs);
}

/// Check if two CHSV objects have the same color data
FASTLED_FORCE_INLINE bool operator== (const CHSV& lhs, const CHSV& rhs)
{
    return (lhs.h == rhs.h) && (lhs.s == rhs.s) && (lhs.v == rhs.v);
}

/// Check if two CHSV objects do *not* have the same color data
FASTLED_FORCE_INLINE bool operator!= (const CHSV& lhs, const CHSV& rhs)
{
    return !(lhs == rhs);
}

/// Check if the sum of the color channels in one CRGB object is less than another
FASTLED_FORCE_INLINE bool operator< (const CRGB& lhs, const CRGB& rhs)
{
    uint16_t sl, sr;
    sl = lhs.r + lhs.g + lhs.b;
    sr = rhs.r + rhs.g + rhs.b;
    return sl < sr;
}

/// Check if the sum of the color channels in one CRGB object is greater than another
FASTLED_FORCE_INLINE bool operator> (const CRGB& lhs, const CRGB& rhs)
{
    uint16_t sl, sr;
    sl = lhs.r + lhs.g + lhs.b;
    sr = rhs.r + rhs.g + rhs.b;
    return sl > sr;
}

/// Check if the sum of the color channels in one CRGB object is greater than or equal to another
FASTLED_FORCE_INLINE bool operator>= (const CRGB& lhs, const CRGB& rhs)
{
    uint16_t sl, sr;
    sl = lhs.r + lhs.g + lhs.b;
    sr = rhs.r + rhs.g + rhs.b;
    return sl >= sr;
}

/// Check if the sum of the color channels in one CRGB object is less than or equal to another
FASTLED_FORCE_INLINE bool operator<= (const CRGB& lhs, const CRGB& rhs)
{
    uint16_t sl, sr;
    sl = lhs.r + lhs.g + lhs.b;
    sr = rhs.r + rhs.g + rhs.b;
    return sl <= sr;
}



/// @copydoc CRGB::operator/=
FASTLED_FORCE_INLINE CRGB operator/( const CRGB& p1, uint8_t d)
{
    return CRGB( p1.r/d, p1.g/d, p1.b/d);
}


/// Combine two CRGB objects, taking the smallest value of each channel
FASTLED_FORCE_INLINE CRGB operator&( const CRGB& p1, const CRGB& p2)
{
    return CRGB( p1.r < p2.r ? p1.r : p2.r,
                 p1.g < p2.g ? p1.g : p2.g,
                 p1.b < p2.b ? p1.b : p2.b);
}

/// Combine two CRGB objects, taking the largest value of each channel
FASTLED_FORCE_INLINE CRGB operator|( const CRGB& p1, const CRGB& p2)
{
    return CRGB( p1.r > p2.r ? p1.r : p2.r,
                 p1.g > p2.g ? p1.g : p2.g,
                 p1.b > p2.b ? p1.b : p2.b);
}

/// @copydoc CRGB::operator+=
FASTLED_FORCE_INLINE CRGB operator+( const CRGB& p1, const CRGB& p2);

/// @copydoc CRGB::operator-=
FASTLED_FORCE_INLINE CRGB operator-( const CRGB& p1, const CRGB& p2);

/// @copydoc CRGB::operator*=
FASTLED_FORCE_INLINE CRGB operator*( const CRGB& p1, uint8_t d);

/// Scale using CRGB::nscale8_video()
FASTLED_FORCE_INLINE CRGB operator%( const CRGB& p1, uint8_t d);

/// Generic template for ostream operator to print CRGB objects. Usefull
/// for the unit_tests.
template<typename T>
T& operator<<(T& os, const CRGB& rgb) {
    os << "CRGB(" << static_cast<int>(rgb.r) << ", " 
       << static_cast<int>(rgb.g) << ", " 
       << static_cast<int>(rgb.b) << ")";
    return os;
}

FASTLED_NAMESPACE_END


