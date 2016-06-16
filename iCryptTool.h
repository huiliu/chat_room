#ifndef __CHARROOM_ICRYPT_TOOL_H__
#define __CHARROOM_ICRYPT_TOOL_H__ 1
#include <memory>
//
// file: iCryptTool.h
// Date: 
// Description:
//      通讯加密接口
//

enum CRYPT_TYPE
{
    CT_NULL = 0,        // 不加密
    CT_BLUEFISH = 1,    // bluefish算法
};

#define g_key_buff_len 33

class iCryptTool
{
public:
    virtual ~iCryptTool() {}

    virtual void Encrypt(char* pData, uint32_t uLen) = 0;
    virtual void Decrypt(char* pData, uint32_t uLen) = 0;
};


std::shared_ptr<iCryptTool> CryptFactory(CRYPT_TYPE type, const std::shared_ptr<char> spKey, uint32_t uLen);
/// 生成秘钥
std::shared_ptr<char>   GenerateRandomChar();
#endif /* ifndef __CHARROOM_ICRYPT_TOOL_H__ */
