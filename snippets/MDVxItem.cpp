//
//  MDVxItem.cpp
//
//  Created by DJFio on 13/06/2020.
//  Copyright © 2020 DJFio. All rights reserved.
//
#include "MDVxItem.hpp"


MDVxUUID &MDVxUUID::makeFrom2(char * b){memcpy(UMIDx+16, b, 8);return *this;}
MDVxUUID &MDVxUUID::makeFrom8(char * b){memcpy(UMIDx, b, 32);return *this;}
        
    uint32_t MDVxUUID::hash (void) const { return (octet[4].UMIDx32t)^(octet[5].UMIDx32t);} // simple hash
    bool MDVxUUID::isEqualByUUID (const MDVxUUID *b) const
    {
        if (b!=NULL) {
            if (this->hash() == b->hash())
                if ((this->octet[4].UMIDx32t == b->octet[4].UMIDx32t) &&
                    (this->octet[5].UMIDx32t == b->octet[5].UMIDx32t))
                    return true;
        }
        return false;
    }
    bool MDVxUUID::isLessByUUID (const MDVxUUID *b) const
    {
        if (b!=NULL) {
            if (this->octet[4].UMIDx32t != b->octet[4].UMIDx32t){
                if (this->octet[4].UMIDx32t < b->octet[4].UMIDx32t) return true;
                else return false;
            }
            else if (this->octet[5].UMIDx32t != b->octet[5].UMIDx32t){
                if (this->octet[5].UMIDx32t < b->octet[5].UMIDx32t) return true;
                else return false;
            }
        }
        return false;
    }
    
    bool MDVxUUID::operator==(const MDVxUUID& b) const  { return (isEqualByUUID (&b));}
    bool MDVxUUID::operator<(const MDVxUUID& b) const   { return (isLessByUUID  (&b));}


MDVxItem::MDVxItem(){
        memset(filename, 0, sizeof(char)*256);
        memset(projectname, 0, sizeof(char)*256);
        
    };
MDVxItem::~MDVxItem(){};
    
    uint32_t MDVxItem::hash (void) const { return filePackageUID.hash();} // simple hash
    bool MDVxItem::isEqualByUUID (const MDVxItem *b) const { return filePackageUID == b->filePackageUID;}
    bool MDVxItem::isLessByUUID (const MDVxItem *b) const { return filePackageUID < b->filePackageUID;}
    bool MDVxItem::operator<(const MDVxItem& b) const { return (isLessByUUID (&b)); }
    bool MDVxItem::operator==(const MDVxItem& b) const{ return (isEqualByUUID (&b));}
    
    void MDVxItem::Print (void){
        getStringForUID (&filePackageUID);
        printf ("file            :: %s\nproject         :: %s\nfilePackageUID  ::%s\n",filename,projectname,printbuffer);
        getStringForUID (&materialSourcePackageUID);
        printf ("SourcePackageUID::%s\n***********************************\n",printbuffer);
        
    }

void MDVxItem::getStringForUID (MDVxUUID* uid)  {
        
        if (uid==NULL) return;
        int c=2;
        for (int i = 0; i < 32; ++i){
            printbuffer[c]  = hex_chars[ ( uid->UMIDx[i] & 0xF0 ) >> 4 ];
            printbuffer[c+1]= hex_chars[ ( uid->UMIDx[i] & 0x0F ) >> 0 ];
            c+=2;
            if (((i+1)<32)&&((i+1) % 4  == 0)) c+=2;
        }
        return;
    }
