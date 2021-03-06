#include "iCryptTool.h"
#include <cstring>

class NullCrypt : public iCryptTool
{
public:
    NullCrypt(const std::shared_ptr<char> spKey, uint32_t iKeyLen);
    ~NullCrypt ();

    virtual void Encrypt(char* pData, uint32_t iLen);
    virtual void Decrypt(char* pData, uint32_t iLen);

private:
    std::shared_ptr<char>   m_spKey;
    uint32_t                m_uKeyLen;
};

NullCrypt::NullCrypt(const std::shared_ptr<char> cKey, uint32_t uKeyLen)
    : m_spKey(cKey)
    , m_uKeyLen(uKeyLen)
{
}

NullCrypt::~NullCrypt()
{
}

void NullCrypt::Encrypt(char* pData, uint32_t uLen)
{
    // do something
}

void NullCrypt::Decrypt(char* pData, uint32_t uLen)
{
    // do something
}

std::shared_ptr<iCryptTool> CryptFactory(CRYPT_TYPE type, const std::shared_ptr<char> spKey, uint32_t uLen)
{
    switch (type) {
        case CT_NULL:
            return std::shared_ptr<iCryptTool>(new NullCrypt(spKey, uLen));
            break;
        case CT_BLUEFISH:
            return nullptr;
            break;
        default:
            return nullptr;
    }
}

std::shared_ptr<char> GenerateRandomChar()
{
    char* pKeyBuff = (char*)::operator new(g_key_buff_len);
    memcpy(pKeyBuff, "249aee78e29e95f6741898949b53bc93", g_key_buff_len - 1);
    pKeyBuff[g_key_buff_len - 1] = '\0';
    return std::shared_ptr<char>(pKeyBuff);
}
