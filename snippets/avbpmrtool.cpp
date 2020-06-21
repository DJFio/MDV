//
//
//  read pmr file into the map
//
//  Created by DJFio on 12/06/2020.
//  Copyright © 2020 DJFio. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <map>

#include "MDVxItem.hpp"
#include"AVBFile.hpp"


#define BSWAP_16(val) ((val << 8) | ((val >> 8) & 0xFF))
#define BSWAP_32(val) (((val>>24)&0xff) | ((val<<8)&0xff0000) | ((val>>8)&0xff00) | ((val<<24)&0xff000000))
#define Byte_Cast16(val,use) (use ? BSWAP_16(val) : val)
#define Byte_Cast32(val,use) (use ? BSWAP_32(val) : val)




int main(int argc, const char * argv[]) {
//**************************
// simple utility takes two filenames as input
    if (argc <2) {
        std::cerr << "Usage: " << argv[0] << " file.pmr" << std::endl;
        return 1;
    }
    
//
    std::map<MDVxUUID, MDVxItem> UIDS_pmr;
    
//**************************
//open pmr file

    
    std::ifstream pmrfile(argv[1], std::ios::binary | std::ios::ate);
    if (!pmrfile.good()){
        std::cout << "problem with PMR File :: bad file name? " << std::endl;
        return 1;
    }
    std::streamsize pmrsize = pmrfile.tellg();
    pmrfile.seekg(0, std::ios::beg);
    if (pmrsize < 30)
    {
        std::cout << "problem with PMR File :: too small " << std::endl;
        return 1;
    }
    {
        char * bytes = new char [pmrsize];
        if (pmrfile.read(bytes, pmrsize))
        {


            size_t position = 12;
            uint32_t UUIDlength;
            uint32_t ItemsCount;
            uint32_t currentitem =0;
            bool usebyteswap = false;
            
            const uint32_t *uints32 = (const uint32_t *)bytes;
            if (uints32[0] != 0x000007a9)
            {
                if (uints32[0] == 0xa9070000)  {usebyteswap = true;}
                else {std::cout << "problem with PMR File :: Bad Magic bytes" << std::endl;return 1;}
           
            }
            
            
            if (usebyteswap) {
                ItemsCount= ntohl(uints32[2]);
                UUIDlength = ntohl(uints32[1]);
                
            } else {
                ItemsCount=uints32[2];
                UUIDlength = uints32[1];
            }
            
            
            
            uint16_t numberofchars = 0;
            
            while (position <= pmrsize) {
                MDVxItem xItem = MDVxItem();
                if (UUIDlength==0x2){
                    if ((position+8) >= pmrsize) break;
                    xItem.filePackageUID.makeFrom2(&bytes[position]);
                    position=position+8;
                }else if (UUIDlength==0x8){
                    if ((position+32) >= pmrsize) break;
                    xItem.filePackageUID.makeFrom8(&bytes[position]);
                    position=position+32;
                }
                if ((position+2) >= pmrsize) break;
                if (usebyteswap){numberofchars= ntohs(((uint16_t*)(&bytes[position]))[0]);
                } else {  numberofchars= ((uint16_t*)(&bytes[position]))[0];}
                position=position+2;
                if ((numberofchars == 0)||(numberofchars > 255)) break;
                if ((position+numberofchars) >= pmrsize) break;
                memcpy(xItem.filename, &bytes[position], numberofchars);
                position=position+numberofchars;
                
                if ((position+2) >= pmrsize) break;
                if (usebyteswap){
                    numberofchars= ntohs(((uint16_t*)(&bytes[position]))[0]);
                } else {
                    numberofchars= ((uint16_t*)(&bytes[position]))[0];
                }
                position=position+2;
                if ((position+numberofchars) >= pmrsize) break;
                if (numberofchars > 64) break;
                if (numberofchars == 0){
                    strcpy((char*)xItem.projectname, "_00_NULL_PROJECT\0");
                } else {
                    memcpy(xItem.projectname, &bytes[position], numberofchars);
                    position=position+numberofchars;
                }
                
                if (UUIDlength==0x2){
                    
                    xItem.materialSourcePackageUID.makeFrom2(&bytes[position]);
                    if ((position+8) >= pmrsize) break;
                    position=position+8;
                    
                }else if (UUIDlength==0x8){
                    xItem.materialSourcePackageUID.makeFrom8(&bytes[position]);
                    if ((position+32) >= pmrsize) break;
                    position=position+32;
                }
                if ((position+3) >= pmrsize) break;
                //xItem.PMRtimestamp =memcpy(&bytes[position],sizeof(uint32_t));
                position=position+4;
                UIDS_pmr.insert ( std::pair <MDVxUUID, MDVxItem>(xItem.filePackageUID,xItem) );
                currentitem++;
            }
            
            std::cout << "Parsed Items count in PMR file ::"<< ItemsCount <<"/"<< currentitem<< std::endl;
            
        }
        else {
            std::cout << "problem with PMR File :: read error" << std::endl;
            delete [] bytes;
            return 1;
        }
        delete [] bytes;
    }

    
    return 0;
}
