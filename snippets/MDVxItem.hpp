//
//  MDVxItem.hpp
//
//  Created by DJFio on 13/06/2020.
//  Copyright © 2020 DJFio. All rights reserved.
//

#ifndef MDVxItem_hpp
#define MDVxItem_hpp



typedef union MDVxOctet{
    uint8_t UMIDx[4];
    uint32_t UMIDx32t;
}MDVxOctet;

typedef union MDVxUUID {
    uint8_t UMIDx[32];
    MDVxOctet octet[8];
    
    MDVxUUID &makeFrom2(char * b);
    MDVxUUID &makeFrom8(char * b);
    void swapUUIDfromMXFLibtoMDVx(void);
    uint32_t hash (void) const ; // simple hash
    bool isEqualByUUID (const MDVxUUID *b) const;
    bool isLessByUUID (const MDVxUUID *b) const;
    bool operator==(const MDVxUUID& b) const;
    bool operator<(const MDVxUUID& b) const;
}MDVxUUID;

class MDVxItem{
public:
    char filename[256];
    char projectname[256];
    MDVxUUID filePackageUID;
    MDVxUUID materialSourcePackageUID;
    
    MDVxItem();
    ~MDVxItem();
    
        uint32_t hash (void) const ;
        bool isEqualByUUID (const MDVxItem *b) const  ;
        bool isLessByUUID (const MDVxItem *b) const ;
        bool operator<(const MDVxItem& b) const ;
        bool operator==(const MDVxItem& b) const;
    
        void Print (void);
    
private:
    char printbuffer[84] = {"{ xxxxxxxx::xxxxxxxx::xxxxxxxx::xxxxxxxx::xxxxxxxx::xxxxxxxx::xxxxxxxx::xxxxxxxx }\0"};
    char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    
        void getStringForUID (MDVxUUID* uid)  ;
} ;




#endif /* MDVxItem_hpp */
