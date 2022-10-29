#pragma once

#include <memory>

#include <juce_core/juce_core.h>
#include <juce_cryptography/juce_cryptography.h>

#ifndef MRALIASPRO

static juce::XmlElement::TextFormat getXmlFormat()
{
    juce::XmlElement::TextFormat format{};
    format.newLineChars = nullptr; // fit everything on one line
    return format;
}

static const juce::String encryptXml(const juce::XmlElement* xml,
                                     const juce::String& rsaPrivateKey)
{
    juce::BigInteger val;

    if (xml != nullptr)
    {
        const juce::String s(xml->toString(getXmlFormat()));
        const juce::MemoryBlock mb(s.toUTF8(), s.length());

        val.loadFromMemoryBlock(mb);
    }

    juce::RSAKey key(rsaPrivateKey);
    key.applyToValue(val);

    return val.toString(16);
}
#endif

static std::unique_ptr<juce::XmlElement> decodeEncryptedXml(const juce::String& hexData,
                                                            const juce::String& rsaPublicKey)
{
    if (hexData.isEmpty())
        return nullptr;

    juce::BigInteger val;
    val.parseString(hexData, 16);

    juce::RSAKey key(rsaPublicKey);
    key.applyToValue(val);

    const juce::MemoryBlock mb(val.toMemoryBlock());
    juce::XmlDocument doc(mb.toString());

    auto xml = doc.getDocumentElement();

#ifdef JUCE_DEBUG
    if (xml != nullptr)
    {
        DBG(xml->toString(getXmlFormat()));
    }
#endif

    return xml;
}
