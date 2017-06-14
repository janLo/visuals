#ifndef COLOR_HPP
#define COLOR_HPP

//===========================================================================
//
// Colors
//
//===========================================================================
class Color3;

//--------------------------
// 4D Color
//--------------------------
class Color4 {

public:
    Color4() {}
    explicit Color4( unsigned int argb );
    explicit Color4( const float* );
    Color4( const Color3&, float a );
    Color4( float r, float g, float b, float a );

    void set( float r, float g, float b, float a );

    // access grants
    float& operator [] ( int i )        { return (&r)[i]; }
    float  operator [] ( int i ) const  { return (&r)[i]; }

    void getFloat4 ( float* pf ) const  { pf[0] = r; pf[1] = g; pf[2] = b; pf[3] = a; }

    // casting (float* for vertex shader constants; uint32 for render states & co)
    //operator float* ()             { return &r; }
    //operator const float* () const { return &r; }

    // returns a D3DCOLOR [i.e. a color in bgra (= D3D 'argb') order, not rgba!]
    operator unsigned int () const;

    Color3 rgb () const;

    // no arithmetic operators defined for Color4 (but for Color3, see below)!
    bool operator == ( const Color4& ) const;
    bool operator != ( const Color4& ) const;

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 1.0f;

private:  // (not allowed!)
    // unary operators
    Color4 operator + () const;
    Color4 operator - () const;

    // binary operators
    Color4 operator + ( const Color4& ) const;
    Color4 operator - ( const Color4& ) const;
    Color4 operator * ( const Color4& ) const;
    Color4 operator * ( float ) const;
    Color4 operator / ( float ) const;

    friend Color4 operator * ( float, const Color4& );
};


//--------------------------
// 3D Color                   // (i.e. alpha component is always 1)
//--------------------------
class Color3 : public Color4 {

public:
    Color3() {}
    explicit Color3( unsigned int rgb );
    Color3( float r, float g, float b ) : Color4(r, g, b, 1) {}

    void set( float r, float g, float b );

    // access grants
    void getFloat3 ( float* pf ) const  { pf[0] = r; pf[1] = g; pf[2] = b; }

    // assignment operators
    Color3& operator += ( const Color3& );
    Color3& operator -= ( const Color3& );
    Color3& operator *= ( const Color3& );  // (componentwise multiply!)
    Color3& operator *= ( float );
    Color3& operator /= ( float );

    // unary operators
    Color3 operator + () const  { return *this; }
    Color3 operator - () const;

    // binary operators
    Color3 operator + ( const Color3& ) const;
    Color3 operator - ( const Color3& ) const;
    Color3 operator * ( const Color3& ) const;  // (componentwise multiply!)
    Color3 operator * ( float ) const;
    Color3 operator / ( float ) const;

    friend Color3 operator * ( float, const Color3& );
};


//--------------------------
// 4D Color
//--------------------------

inline
Color4::Color4( unsigned int argb )
{
    const float f = 1.0f / 255.0f;
    r = f * (float) (unsigned char) (argb >> 16);
    g = f * (float) (unsigned char) (argb >>  8);
    b = f * (float) (unsigned char) (argb >>  0);
    a = f * (float) (unsigned char) (argb >> 24);
}

inline
Color4::Color4( const float* pf )
{
    r = pf[0];
    g = pf[1];
    b = pf[2];
    a = pf[3];
}

inline
Color4::Color4( const Color3& c, float fa )
{
    r = c.r;
    g = c.g;
    b = c.b;
    a = fa;
}

inline
Color4::Color4( float fr, float fg, float fb, float fa )
{
    r = fr;
    g = fg;
    b = fb;
    a = fa;
}

inline
void Color4::set( float fr, float fg, float fb, float fa )
{
    r = fr;
    g = fg;
    b = fb;
    a = fa;
}


// casting
inline
Color4::operator unsigned int () const
{
    unsigned int dwR = r >= 1.0f ? 0xff : r <= 0.0f ? 0x00 : (unsigned int) (r * 255.0f + 0.5f);
    unsigned int dwG = g >= 1.0f ? 0xff : g <= 0.0f ? 0x00 : (unsigned int) (g * 255.0f + 0.5f);
    unsigned int dwB = b >= 1.0f ? 0xff : b <= 0.0f ? 0x00 : (unsigned int) (b * 255.0f + 0.5f);
    unsigned int dwA = a >= 1.0f ? 0xff : a <= 0.0f ? 0x00 : (unsigned int) (a * 255.0f + 0.5f);

    return (dwA << 24) | (dwR << 16) | (dwG << 8) | dwB;
}


inline Color3
Color4::rgb () const
{
   return Color3(r, g, b);
}


inline bool
Color4::operator == ( const Color4& c ) const
{
    return r == c.r && g == c.g && b == c.b && a == c.a;
}

inline bool
Color4::operator != ( const Color4& c ) const
{
    return r != c.r || g != c.g || b != c.b || a != c.a;
}



//--------------------------
// 3D Color
//--------------------------

inline
Color3::Color3( unsigned int rgb )
{
    const float f = 1.0f / 255.0f;
    r = f * (float) (unsigned char) (rgb >> 16);
    g = f * (float) (unsigned char) (rgb >>  8);
    b = f * (float) (unsigned char) (rgb >>  0);
    a = 1;
}

inline
void Color3::set( float fr, float fg, float fb )
{
    r = fr;
    g = fg;
    b = fb;
}


// assignment operators
inline Color3&
Color3::operator += ( const Color3& c )
{
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
}

inline Color3&
Color3::operator -= ( const Color3& c )
{
    r -= c.r;
    g -= c.g;
    b -= c.b;
    return *this;
}

inline Color3&
Color3::operator *= ( const Color3& c )
{
    r *= c.r;
    g *= c.g;
    b *= c.b;
    return *this;
}

inline Color3&
Color3::operator *= ( float f )
{
    r *= f;
    g *= f;
    b *= f;
    return *this;
}

inline Color3&
Color3::operator /= ( float f )
{
    float fInv = 1.0f / f;
    r *= fInv;
    g *= fInv;
    b *= fInv;
    return *this;
}


// unary operators
inline Color3
Color3::operator - () const
{
    return Color3(-r, -g, -b);
}


// binary operators
inline Color3
Color3::operator + ( const Color3& c ) const
{
    return Color3(r + c.r, g + c.g, b + c.b);
}

inline Color3
Color3::operator - ( const Color3& c ) const
{
    return Color3(r - c.r, g - c.g, b - c.b);
}

inline Color3
Color3::operator * ( const Color3& c ) const
{
    return Color3(r * c.r, g * c.g, b * c.b);
}

inline Color3
Color3::operator * ( float f ) const
{
    return Color3(r * f, g * f, b * f);
}

inline Color3
Color3::operator / ( float f ) const
{
    float fInv = 1.0f / f;
    return Color3(r * fInv, g * fInv, b * fInv);
}


inline Color3
operator * ( float f, const Color3& c )
{
    return Color3(f * c.r, f * c.g, f * c.b);
}

#endif