//
// Created by henry on 12/17/22.
//

#ifndef HAGAME2_DATATYPES_H
#define HAGAME2_DATATYPES_H

namespace hg::graphics {
    // GLAD ORDERING
    /*
    #define GL_BYTE 0x1400
    #define GL_UNSIGNED_BYTE 0x1401
    #define GL_SHORT 0x1402
    #define GL_UNSIGNED_SHORT 0x1403
    #define GL_INT 0x1404
    #define GL_UNSIGNED_INT 0x1405
    #define GL_FLOAT 0x1406
    #define GL_2_BYTES 0x1407
    #define GL_3_BYTES 0x1408
    #define GL_4_BYTES 0x1409
    #define GL_DOUBLE 0x140A
    */

    enum DataType {
        Byte,
        UnsignedByte,
        Short,
        UnsignedShort,
        Int,
        UnsignedInt,
        Float,
        TwoBytes,
        ThreeBytes,
        FourBytes,
        Double
    };

    inline unsigned int getOpenGLEnum(DataType type) {
        return 0x1400 + type;
    }
}

#endif //HAGAME2_DATATYPES_H
