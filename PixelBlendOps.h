#ifndef PIXELBLENDOPS_H
#define PIXELBLENDOPS_H
#pragma once
#include <QtDebug>
#include <functional>

#include "BPixelOp.h"
uint masTable[] = {0x00000000, 0x00000000, 0,  0x00000001, 0x00000000, 0,
                   0x00000001, 0x00000000, 1,  0xAAAAAAAB, 0x00000000, 33,
                   0x00000001, 0x00000000, 2,  0xCCCCCCCD, 0x00000000, 34,
                   0xAAAAAAAB, 0x00000000, 34, 0x49249249, 0x49249249, 33,
                   0x00000001, 0x00000000, 3,  0x38E38E39, 0x00000000, 33,
                   0xCCCCCCCD, 0x00000000, 35, 0xBA2E8BA3, 0x00000000, 35,
                   0xAAAAAAAB, 0x00000000, 35, 0x4EC4EC4F, 0x00000000, 34,
                   0x49249249, 0x49249249, 34, 0x88888889, 0x00000000, 35,
                   0x00000001, 0x00000000, 4,  0xF0F0F0F1, 0x00000000, 36,
                   0x38E38E39, 0x00000000, 34, 0xD79435E5, 0xD79435E5, 36,
                   0xCCCCCCCD, 0x00000000, 36, 0xC30C30C3, 0xC30C30C3, 36,
                   0xBA2E8BA3, 0x00000000, 36, 0xB21642C9, 0x00000000, 36,
                   0xAAAAAAAB, 0x00000000, 36, 0x51EB851F, 0x00000000, 35,
                   0x4EC4EC4F, 0x00000000, 35, 0x97B425ED, 0x97B425ED, 36,
                   0x49249249, 0x49249249, 35, 0x8D3DCB09, 0x00000000, 36,
                   0x88888889, 0x00000000, 36, 0x42108421, 0x42108421, 35,
                   0x00000001, 0x00000000, 5,  0x3E0F83E1, 0x00000000, 35,
                   0xF0F0F0F1, 0x00000000, 37, 0x75075075, 0x75075075, 36,
                   0x38E38E39, 0x00000000, 35, 0x6EB3E453, 0x6EB3E453, 36,
                   0xD79435E5, 0xD79435E5, 37, 0x69069069, 0x69069069, 36,
                   0xCCCCCCCD, 0x00000000, 37, 0xC7CE0C7D, 0x00000000, 37,
                   0xC30C30C3, 0xC30C30C3, 37, 0x2FA0BE83, 0x00000000, 35,
                   0xBA2E8BA3, 0x00000000, 37, 0x5B05B05B, 0x5B05B05B, 36,
                   0xB21642C9, 0x00000000, 37, 0xAE4C415D, 0x00000000, 37,
                   0xAAAAAAAB, 0x00000000, 37, 0x5397829D, 0x00000000, 36,
                   0x51EB851F, 0x00000000, 36, 0xA0A0A0A1, 0x00000000, 37,
                   0x4EC4EC4F, 0x00000000, 36, 0x9A90E7D9, 0x9A90E7D9, 37,
                   0x97B425ED, 0x97B425ED, 37, 0x94F2094F, 0x94F2094F, 37,
                   0x49249249, 0x49249249, 36, 0x47DC11F7, 0x47DC11F7, 36,
                   0x8D3DCB09, 0x00000000, 37, 0x22B63CBF, 0x00000000, 35,
                   0x88888889, 0x00000000, 37, 0x4325C53F, 0x00000000, 36,
                   0x42108421, 0x42108421, 36, 0x41041041, 0x41041041, 36,
                   0x00000001, 0x00000000, 6,  0xFC0FC0FD, 0x00000000, 38,
                   0x3E0F83E1, 0x00000000, 36, 0x07A44C6B, 0x00000000, 33,
                   0xF0F0F0F1, 0x00000000, 38, 0x76B981DB, 0x00000000, 37,
                   0x75075075, 0x75075075, 37, 0xE6C2B449, 0x00000000, 38,
                   0x38E38E39, 0x00000000, 36, 0x381C0E07, 0x381C0E07, 36,
                   0x6EB3E453, 0x6EB3E453, 37, 0x1B4E81B5, 0x00000000, 35,
                   0xD79435E5, 0xD79435E5, 38, 0x3531DEC1, 0x00000000, 36,
                   0x69069069, 0x69069069, 37, 0xCF6474A9, 0x00000000, 38,
                   0xCCCCCCCD, 0x00000000, 38, 0xCA4587E7, 0x00000000, 38,
                   0xC7CE0C7D, 0x00000000, 38, 0x3159721F, 0x00000000, 36,
                   0xC30C30C3, 0xC30C30C3, 38, 0xC0C0C0C1, 0x00000000, 38,
                   0x2FA0BE83, 0x00000000, 36, 0x2F149903, 0x00000000, 36,
                   0xBA2E8BA3, 0x00000000, 38, 0xB81702E1, 0x00000000, 38,
                   0x5B05B05B, 0x5B05B05B, 37, 0x2D02D02D, 0x2D02D02D, 36,
                   0xB21642C9, 0x00000000, 38, 0xB02C0B03, 0x00000000, 38,
                   0xAE4C415D, 0x00000000, 38, 0x2B1DA461, 0x2B1DA461, 36,
                   0xAAAAAAAB, 0x00000000, 38, 0xA8E83F57, 0xA8E83F57, 38,
                   0x5397829D, 0x00000000, 37, 0xA57EB503, 0x00000000, 38,
                   0x51EB851F, 0x00000000, 37, 0xA237C32B, 0xA237C32B, 38,
                   0xA0A0A0A1, 0x00000000, 38, 0x9F1165E7, 0x9F1165E7, 38,
                   0x4EC4EC4F, 0x00000000, 37, 0x27027027, 0x27027027, 36,
                   0x9A90E7D9, 0x9A90E7D9, 38, 0x991F1A51, 0x991F1A51, 38,
                   0x97B425ED, 0x97B425ED, 38, 0x2593F69B, 0x2593F69B, 36,
                   0x94F2094F, 0x94F2094F, 38, 0x24E6A171, 0x24E6A171, 36,
                   0x49249249, 0x49249249, 37, 0x90FDBC09, 0x90FDBC09, 38,
                   0x47DC11F7, 0x47DC11F7, 37, 0x8E78356D, 0x8E78356D, 38,
                   0x8D3DCB09, 0x00000000, 38, 0x23023023, 0x23023023, 36,
                   0x22B63CBF, 0x00000000, 36, 0x44D72045, 0x00000000, 37,
                   0x88888889, 0x00000000, 38, 0x8767AB5F, 0x8767AB5F, 38,
                   0x4325C53F, 0x00000000, 37, 0x85340853, 0x85340853, 38,
                   0x42108421, 0x42108421, 37, 0x10624DD3, 0x00000000, 35,
                   0x41041041, 0x41041041, 37, 0x10204081, 0x10204081, 35,
                   0x00000001, 0x00000000, 7,  0x0FE03F81, 0x00000000, 35,
                   0xFC0FC0FD, 0x00000000, 39, 0xFA232CF3, 0x00000000, 39,
                   0x3E0F83E1, 0x00000000, 37, 0xF6603D99, 0x00000000, 39,
                   0x07A44C6B, 0x00000000, 34, 0xF2B9D649, 0x00000000, 39,
                   0xF0F0F0F1, 0x00000000, 39, 0x077975B9, 0x00000000, 34,
                   0x76B981DB, 0x00000000, 38, 0x75DED953, 0x00000000, 38,
                   0x75075075, 0x75075075, 38, 0x3A196B1F, 0x00000000, 37,
                   0xE6C2B449, 0x00000000, 39, 0xE525982B, 0x00000000, 39,
                   0x38E38E39, 0x00000000, 37, 0xE1FC780F, 0x00000000, 39,
                   0x381C0E07, 0x381C0E07, 37, 0xDEE95C4D, 0x00000000, 39,
                   0x6EB3E453, 0x6EB3E453, 38, 0xDBEB61EF, 0x00000000, 39,
                   0x1B4E81B5, 0x00000000, 36, 0x36406C81, 0x00000000, 37,
                   0xD79435E5, 0xD79435E5, 39, 0xD62B80D7, 0x00000000, 39,
                   0x3531DEC1, 0x00000000, 37, 0xD3680D37, 0x00000000, 39,
                   0x69069069, 0x69069069, 38, 0x342DA7F3, 0x00000000, 37,
                   0xCF6474A9, 0x00000000, 39, 0xCE168A77, 0xCE168A77, 39,
                   0xCCCCCCCD, 0x00000000, 39, 0xCB8727C1, 0x00000000, 39,
                   0xCA4587E7, 0x00000000, 39, 0xC907DA4F, 0x00000000, 39,
                   0xC7CE0C7D, 0x00000000, 39, 0x634C0635, 0x00000000, 38,
                   0x3159721F, 0x00000000, 37, 0x621B97C3, 0x00000000, 38,
                   0xC30C30C3, 0xC30C30C3, 39, 0x60F25DEB, 0x00000000, 38,
                   0xC0C0C0C1, 0x00000000, 39, 0x17F405FD, 0x17F405FD, 36,
                   0x2FA0BE83, 0x00000000, 37, 0xBD691047, 0xBD691047, 39,
                   0x2F149903, 0x00000000, 37, 0x5D9F7391, 0x00000000, 38,
                   0xBA2E8BA3, 0x00000000, 39, 0x5C90A1FD, 0x5C90A1FD, 38,
                   0xB81702E1, 0x00000000, 39, 0x5B87DDAD, 0x5B87DDAD, 38,
                   0x5B05B05B, 0x5B05B05B, 38, 0xB509E68B, 0x00000000, 39,
                   0x2D02D02D, 0x2D02D02D, 37, 0xB30F6353, 0x00000000, 39,
                   0xB21642C9, 0x00000000, 39, 0x1623FA77, 0x1623FA77, 36,
                   0xB02C0B03, 0x00000000, 39, 0xAF3ADDC7, 0x00000000, 39,
                   0xAE4C415D, 0x00000000, 39, 0x15AC056B, 0x15AC056B, 36,
                   0x2B1DA461, 0x2B1DA461, 37, 0xAB8F69E3, 0x00000000, 39,
                   0xAAAAAAAB, 0x00000000, 39, 0x15390949, 0x00000000, 36,
                   0xA8E83F57, 0xA8E83F57, 39, 0x15015015, 0x15015015, 36,
                   0x5397829D, 0x00000000, 38, 0xA655C439, 0xA655C439, 39,
                   0xA57EB503, 0x00000000, 39, 0x5254E78F, 0x00000000, 38,
                   0x51EB851F, 0x00000000, 38, 0x028C1979, 0x00000000, 33,
                   0xA237C32B, 0xA237C32B, 39, 0xA16B312F, 0x00000000, 39,
                   0xA0A0A0A1, 0x00000000, 39, 0x4FEC04FF, 0x00000000, 38,
                   0x9F1165E7, 0x9F1165E7, 39, 0x27932B49, 0x00000000, 37,
                   0x4EC4EC4F, 0x00000000, 38, 0x9CC8E161, 0x00000000, 39,
                   0x27027027, 0x27027027, 37, 0x9B4C6F9F, 0x00000000, 39,
                   0x9A90E7D9, 0x9A90E7D9, 39, 0x99D722DB, 0x00000000, 39,
                   0x991F1A51, 0x991F1A51, 39, 0x4C346405, 0x00000000, 38,
                   0x97B425ED, 0x97B425ED, 39, 0x4B809701, 0x4B809701, 38,
                   0x2593F69B, 0x2593F69B, 37, 0x12B404AD, 0x12B404AD, 36,
                   0x94F2094F, 0x94F2094F, 39, 0x25116025, 0x25116025, 37,
                   0x24E6A171, 0x24E6A171, 37, 0x24BC44E1, 0x24BC44E1, 37,
                   0x49249249, 0x49249249, 38, 0x91A2B3C5, 0x00000000, 39,
                   0x90FDBC09, 0x90FDBC09, 39, 0x905A3863, 0x905A3863, 39,
                   0x47DC11F7, 0x47DC11F7, 38, 0x478BBCED, 0x00000000, 38,
                   0x8E78356D, 0x8E78356D, 39, 0x46ED2901, 0x46ED2901, 38,
                   0x8D3DCB09, 0x00000000, 39, 0x2328A701, 0x2328A701, 37,
                   0x23023023, 0x23023023, 37, 0x45B81A25, 0x45B81A25, 38,
                   0x22B63CBF, 0x00000000, 37, 0x08A42F87, 0x08A42F87, 35,
                   0x44D72045, 0x00000000, 38, 0x891AC73B, 0x00000000, 39,
                   0x88888889, 0x00000000, 39, 0x10FEF011, 0x00000000, 36,
                   0x8767AB5F, 0x8767AB5F, 39, 0x86D90545, 0x00000000, 39,
                   0x4325C53F, 0x00000000, 38, 0x85BF3761, 0x85BF3761, 39,
                   0x85340853, 0x85340853, 39, 0x10953F39, 0x10953F39, 36,
                   0x42108421, 0x42108421, 38, 0x41CC9829, 0x41CC9829, 38,
                   0x10624DD3, 0x00000000, 36, 0x828CBFBF, 0x00000000, 39,
                   0x41041041, 0x41041041, 38, 0x81848DA9, 0x00000000, 39,
                   0x10204081, 0x10204081, 36, 0x80808081, 0x00000000, 39};

template <class T>
T Clamp(T a, T min, T max) {
  if (a < min) return min;
  if (a > max) return max;
  return a;
}

int intscale(byte a, byte b) {
  int r = a * b + 0x80;
  r = (((r >> 8) + r) >> 8);
  return r;
}
int computeAlpha(byte a, byte b) {
  int r = intscale(a, 255 - b);
  r += b;
  return r;
}
byte retID(byte a) { return a; }
bgra blend(bgra lhs, bgra rhs, std::function<byte(byte, byte)> F,
           std::function<byte(byte)> h = retID,
           std::function<byte(byte)> j = retID) {
  int lhsA = h(lhs.A);
  int rhsA = j(rhs.A);
  int y = intscale(lhsA, 255 - rhsA);
  int totalA = y + rhs.A;
  if (totalA == 0)
    return bgra(0, 0, 0, 0);
  else {
    uint a = computeAlpha(lhsA, rhsA);
    byte fr = F(lhs.R, rhs.R);
    byte fg = F(lhs.G, rhs.G);
    byte fb = F(lhs.B, rhs.B);
    int x = intscale(lhsA, rhsA);
    int z = rhsA - x;
    int mindex = totalA * 3;
    uint tam = masTable[mindex];
    uint taa = masTable[mindex + 1];
    uint tas = masTable[mindex + 2];
    uint b = ((((lhs.B * y) + (rhs.B * z) + (fb * x)) * tam) + taa) >> tas;
    uint g = ((((lhs.G * y) + (rhs.G * z) + (fg * x)) * tam) + taa) >> tas;
    uint r = ((((lhs.R * y) + (rhs.R * z) + (fr * x)) * tam) + taa) >> tas;
    return bgra(b + (g << 8) + (r << 16) + (a << 24));
  }
}
class PBO_Normal : BPixelOp {
 public:
  using BPixelOp::apply;
  bgra apply(bgra lhs, bgra rhs) override {
    return blend(lhs, rhs, [](byte a, byte b) -> byte { return b; });
  }
};

class PBO_Add : BPixelOp {
 public:
  using BPixelOp::apply;
  bgra apply(bgra lhs, bgra rhs) override {
    return blend(lhs, rhs,
                 [](byte a, byte b) -> byte { return std::min(255, a + b); });
  }
};

class PBO_Multiply : BPixelOp {
 public:
  using BPixelOp::apply;
  bgra apply(bgra lhs, bgra rhs) override {
    return blend(lhs, rhs,
                 [](byte a, byte b) -> byte { return intscale(a, b); });
  }
};

class PBO_ColorBurn : BPixelOp {
 public:
  using BPixelOp::apply;
  bgra apply(bgra lhs, bgra rhs) override {
    return blend(lhs, rhs, [](byte a, byte b) -> byte {
      return (b == 0) ? 0 : std::max(0, (255 - (((255 - a) * 255) / b)));
    });
  }
};

class PBO_ColorDodge : BPixelOp {
 public:
  using BPixelOp::apply;
  bgra apply(bgra lhs, bgra rhs) override {
    return blend(lhs, rhs, [](byte a, byte b) -> byte {
      return (b == 255) ? 255 : std::min(255, (a * 255) / (255 - b));
    });
  }
};

class PBO_Reflect : BPixelOp {
 public:
  using BPixelOp::apply;
  bgra apply(bgra lhs, bgra rhs) override {
    return blend(lhs, rhs, [](byte a, byte b) -> byte {
      return (b == 255) ? 255 : std::min(255, (a * a) / (255 - b));
    });
  }
};

class PBO_Glow : BPixelOp {
 public:
  using BPixelOp::apply;
  bgra apply(bgra lhs, bgra rhs) override {
    return PBO_Reflect().apply(lhs, rhs);
  }
};

class PBO_Overlay : BPixelOp {
 public:
  using BPixelOp::apply;
  bgra apply(bgra lhs, bgra rhs) override {
    return blend(lhs, rhs, [](byte a, byte b) -> byte {
      return (a < 128) ? intscale(2 * a, b) : (255 - (2 * (255 - a), 255 - b));
    });
  }
};

class PBO_Difference : BPixelOp {
 public:
  using BPixelOp::apply;
  bgra apply(bgra lhs, bgra rhs) override {
    return blend(lhs, rhs, [](byte a, byte b) -> byte { return abs(b - a); });
  }
};

class PBO_Negation : BPixelOp {
 public:
  using BPixelOp::apply;
  bgra apply(bgra lhs, bgra rhs) override {
    return blend(lhs, rhs,
                 [](byte a, byte b) -> byte { return 255 - abs(255 - a - b); });
  }
};

class PBO_Screen : BPixelOp {
 public:
  using BPixelOp::apply;
  bgra apply(bgra lhs, bgra rhs) override {
    return blend(lhs, rhs,
                 [](byte a, byte b) -> byte { return b + a - intscale(b, a); });
  }
};

class PBO_Xor : BPixelOp {
 public:
  using BPixelOp::apply;
  bgra apply(bgra lhs, bgra rhs) override {
    return blend(lhs, rhs, [](byte a, byte b) -> byte { return a ^ b; });
  }
};

#endif  // PIXELBLENDOPS_H
