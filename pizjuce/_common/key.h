#pragma once

#include <memory>

#include "juce_core/juce_core.h"
#include "juce_cryptography/juce_cryptography.h"

using namespace juce;

#ifndef MRALIASPRO
static const String encryptXml (const XmlElement* xml,
                                const String& rsaPrivateKey)
{
    BigInteger val;

    if (xml != 0)
    {
        const String s (xml->createDocument (String(), true));
		const MemoryBlock mb (s.toUTF8(), s.length());

        val.loadFromMemoryBlock (mb);
    }

    RSAKey key (rsaPrivateKey);
    key.applyToValue (val);

    return val.toString (16);
}
#endif

static std::unique_ptr<XmlElement> decodeEncryptedXml (const String& hexData,
                                                       const String& rsaPublicKey)
{
    if (hexData.isEmpty())
        return 0;

    BigInteger val;
    val.parseString (hexData, 16);

    RSAKey key (rsaPublicKey);
    key.applyToValue (val);

    const MemoryBlock mb (val.toMemoryBlock());
    XmlDocument doc (mb.toString());

    auto xml = doc.getDocumentElement();

#ifdef JUCE_DEBUG
    if (xml != nullptr)
    {
        DBG (xml->createDocument (String(), true));
    }
#endif

    return xml;
}
