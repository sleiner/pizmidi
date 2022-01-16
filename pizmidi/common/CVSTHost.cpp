/*****************************************************************************/
/* CVSTHost.cpp: implementation of the CVSTHost class.                       */
/*****************************************************************************/

/******************************************************************************
Copyright (C) 2006  Hermann Seib

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
******************************************************************************/

#include <math.h>  /* math prototypes                   */
#include <stdio.h> /* file I/O prototypes               */

#include "CVSTHost.h" /* private prototypes                */

/*****************************************************************************/
/* Global Data                                                               */
/*****************************************************************************/

const int MyVersion = 1; /* highest known VST FX version      */

/*===========================================================================*/
/* CFxBase class members                                                     */
/*===========================================================================*/

static char szChnk[]     = "CcnK"; /* set up swapping flag              */
static long lChnk        = 'CcnK';
bool CFxBase::NeedsBSwap = ! ! memcmp (szChnk, &lChnk, 4);

/*****************************************************************************/
/* SwapBytes : swaps bytes for big/little-endian difference                  */
/*****************************************************************************/

void CFxBase::SwapBytes (long& l)
{
    unsigned char* b  = (unsigned char*) &l;
    long intermediate = ((long) b[0] << 24) | ((long) b[1] << 16) | ((long) b[2] << 8) | (long) b[3];
    l                 = intermediate;
}

void CFxBase::SwapBytes (float& f)
{
    long* pl = (long*) &f;
    SwapBytes (*pl);
}

#if defined(VST_2_4_EXTENSIONS)
void CFxBase::SwapBytes (VstInt32& vi)
{
    unsigned char* b      = (unsigned char*) &vi;
    VstInt32 intermediate = ((VstInt32) b[0] << 24) | ((VstInt32) b[1] << 16) | ((VstInt32) b[2] << 8) | (VstInt32) b[3];
    vi                    = intermediate;
}
#endif

/*===========================================================================*/
/* CFxBank class members                                                     */
/*===========================================================================*/

/*****************************************************************************/
/* CFxBank : constructor                                                     */
/*****************************************************************************/

CFxBank::CFxBank (char* pszFile)
{
    Init();                 /* initialize data areas             */
    if (pszFile)            /* if a file name has been passed    */
        LoadBank (pszFile); /* load the corresponding bank       */
}

CFxBank::CFxBank (int nPrograms, int nParams)
{
    Init();                       /* initialize data areas             */
    SetSize (nPrograms, nParams); /* set new size                      */
}

CFxBank::CFxBank (int nChunkSize)
{
    Init();               /* initialize data areas             */
    SetSize (nChunkSize); /* set new size                      */
}

/*****************************************************************************/
/* Init : initializes all data areas                                         */
/*****************************************************************************/

void CFxBank::Init()
{
    bBank = NULL; /* no bank data loaded               */
    Unload();     /* reset all parameters              */
}

/*****************************************************************************/
/* ~CFxBank : destructor                                                     */
/*****************************************************************************/

CFxBank::~CFxBank()
{
    Unload(); /* unload all data                   */
}

/*****************************************************************************/
/* DoCopy : combined for copy constructor and assignment operator            */
/*****************************************************************************/

CFxBank& CFxBank::DoCopy (const CFxBank& org)
{
    unsigned char* nBank = NULL;
    if (org.nBankLen)
    {
        unsigned char* nBank = new unsigned char[org.nBankLen];
        if (! nBank)
            throw (int) 1;
        memcpy (nBank, org.bBank, org.nBankLen);
    }
    Unload();         /* remove previous data              */
    bBank    = nBank; /* and copy in the other one's       */
    bChunk   = org.bChunk;
    nBankLen = org.nBankLen;
    strcpy (szFileName, org.szFileName);
    return *this;
}

/*****************************************************************************/
/* SetSize : sets new size                                                   */
/*****************************************************************************/

bool CFxBank::SetSize (int nPrograms, int nParams)
{
    int nTotLen  = sizeof (SFxBankBase);
    int nProgLen = sizeof (SFxProgramBase) + nParams * sizeof (float);
    nTotLen += nPrograms * nProgLen;
    unsigned char* nBank = new unsigned char[nTotLen];
    if (! nBank)
        return false;

    Unload();
    bBank    = nBank;
    nBankLen = nTotLen;
    bChunk   = false;

    memset (nBank, 0, nTotLen); /* initialize new bank               */
    SFxBank* pBank     = (SFxBank*) bBank;
    pBank->chunkMagic  = cMagic;
    pBank->byteSize    = 0;
    pBank->fxMagic     = bankMagic;
    pBank->version     = MyVersion;
    pBank->numPrograms = nPrograms;

    unsigned char* bProg = (unsigned char*) pBank->programs;
    for (int i = 0; i < nPrograms; i++)
    {
        SFxProgram* pProg = (SFxProgram*) (bProg + i * nProgLen);
        pProg->chunkMagic = cMagic;
        pProg->byteSize   = 0;
        pProg->fxMagic    = fMagic;
        pProg->version    = 1;
        pProg->numParams  = nParams;
        for (int j = 0; j < nParams; j++)
            pProg->params[j] = 0.f;
    }
    return true;
}

bool CFxBank::SetSize (int nChunkSize)
{
    int nTotLen = sizeof (VstInt32) + nChunkSize;
    //int nTotLen = ((size_t)((SFxBankChunk *)0)->chunk) + nChunkSize;
    unsigned char* nBank = new unsigned char[nTotLen];
    if (! nBank)
        return false;

    Unload();
    bBank    = nBank;
    nBankLen = nTotLen;
    bChunk   = true;

    memset (nBank, 0, nTotLen); /* initialize new bank               */
    SFxBankChunk* pBank = (SFxBankChunk*) bBank;
    pBank->chunkMagic   = cMagic;
    pBank->fxMagic      = chunkBankMagic;
    pBank->version      = MyVersion;
    pBank->numPrograms  = 1;
    pBank->size         = nChunkSize;

    return true;
}

/*****************************************************************************/
/* LoadBank : loads a bank file                                              */
/*****************************************************************************/

bool CFxBank::LoadBank (char* pszFile)
{
    FILE* fp = fopen (pszFile, "rb"); /* try to open the file              */
    if (! fp)                         /* upon error                        */
        return false;                 /* return an error                   */
    bool brc             = true;      /* default to OK                     */
    unsigned char* nBank = NULL;
    try
    {
        fseek (fp, 0, SEEK_END); /* get file size                     */
        size_t tLen = (size_t) ftell (fp);
        rewind (fp);

        nBank = new unsigned char[tLen]; /* allocate storage                  */
        if (! nBank)
            throw (int) 1;
        /* read chunk set to determine cnt.  */
        if (fread (nBank, 1, tLen, fp) != tLen)
            throw (int) 1;
        /* position on bank                  */
        SFxBankBase* pBank = (SFxBankBase*) nBank;
        if (NeedsBSwap) /* eventually swap necessary bytes   */
        {
            SwapBytes (pBank->chunkMagic);
            SwapBytes (pBank->byteSize);
            SwapBytes (pBank->fxMagic);
            SwapBytes (pBank->version);
            SwapBytes (pBank->fxID);
            SwapBytes (pBank->fxVersion);
            SwapBytes (pBank->numPrograms);
        }
        if ((pBank->chunkMagic != cMagic) || /* if erroneous data in there        */
            (pBank->version > MyVersion) || ((pBank->fxMagic != bankMagic) && (pBank->fxMagic != chunkBankMagic)))
            throw (int) 1; /* get out                           */

        if (pBank->fxMagic == bankMagic) /* if this is a bank of parameters   */
        {
            /* position on 1st program          */
            SFxProgram* pProg = ((SFxBank*) pBank)->programs;
            int nProg         = 0;
            while (nProg < pBank->numPrograms) /* walk program list                 */
            {
                if (NeedsBSwap) /* eventually swap necessary bytes   */
                {
                    SwapBytes (pProg->chunkMagic);
                    SwapBytes (pProg->byteSize);
                    SwapBytes (pProg->fxMagic);
                    SwapBytes (pProg->version);
                    SwapBytes (pProg->fxID);
                    SwapBytes (pProg->fxVersion);
                    SwapBytes (pProg->numParams);
                }
                /* if erroneous data                 */
                if ((pProg->chunkMagic != cMagic) || (pProg->fxMagic != fMagic))
                    throw (int) 1; /* get out                           */
                if (NeedsBSwap)    /* if necessary                      */
                {                  /* swap all parameter bytes          */
                    int j;
                    for (j = 0; j < pProg->numParams; j++)
                        SwapBytes (pProg->params[j]);
                }

                unsigned char* pNext = (unsigned char*) pProg;
                pNext += sizeof (SFxProgramBase);
                pNext += (sizeof (float) * pProg->numParams);
                if (pNext > nBank + tLen) /* VERY simple fuse                  */
                    throw (int) 1;

                pProg = (SFxProgram*) pNext;
                nProg++;
            }
        }
        /* if it's a chunk file              */
        else if (pBank->fxMagic == chunkBankMagic)
        {
            if (NeedsBSwap) /* eventually swap necessary bytes   */
            {
                SFxBankChunk* pChunk = (SFxBankChunk*) pBank;
                SwapBytes (pChunk->size);
                /* size check - must not be too large*/
                if (pChunk->size + sizeof (VstInt32) > tLen) //((size_t)((SFxBankChunk *)0)->chunk)
                    throw (int) 1;
            }
        }

        Unload();         /* otherwise remove eventual old data*/
        bBank    = nBank; /* and put in new data               */
        nBankLen = (int) tLen;
        bChunk   = (pBank->fxMagic == chunkBankMagic);
    }
    catch (...)
    {
        brc = false; /* if any error occured, say NOPE    */
        if (nBank)   /* and remove loaded data            */
            delete[] nBank;
    }

    fclose (fp); /* close the file                    */
    return brc;  /* and return                        */
}

/*****************************************************************************/
/* SaveBank : save bank to file                                              */
/*****************************************************************************/

bool CFxBank::SaveBank (char* pszFile)
{
    if (! IsLoaded())
        return false;
    /* create internal copy for mod      */
    unsigned char* nBank = new unsigned char[nBankLen];
    if (! nBank) /* if impossible                     */
        return false;
    memcpy (nBank, bBank, nBankLen);
    /* position on bank                  */
    SFxBankBase* pBank = (SFxBankBase*) nBank;
    int numPrograms    = pBank->numPrograms;
    if (NeedsBSwap) /* if byte-swapping needed           */
    {
        SwapBytes (pBank->chunkMagic);
        SwapBytes (pBank->byteSize);
        SwapBytes (pBank->fxMagic);
        SwapBytes (pBank->version);
        SwapBytes (pBank->fxID);
        SwapBytes (pBank->fxVersion);
        SwapBytes (pBank->numPrograms);
    }
    if (bChunk)
    {
        if (NeedsBSwap) /* if byte-swapping needed           */
            SwapBytes (((SFxBankChunk*) pBank)->size);
    }
    else
    {
        /* position on 1st program           */
        SFxProgram* pProg = ((SFxBank*) pBank)->programs;
        int numParams     = pProg->numParams;
        int nProg         = 0;
        while (nProg < numPrograms) /* walk program list                 */
        {
            if (NeedsBSwap) /* eventually swap all necessary     */
            {
                SwapBytes (pProg->chunkMagic);
                SwapBytes (pProg->byteSize);
                SwapBytes (pProg->fxMagic);
                SwapBytes (pProg->version);
                SwapBytes (pProg->fxID);
                SwapBytes (pProg->fxVersion);
                SwapBytes (pProg->numParams);
                for (int j = 0; j < numParams; j++)
                    SwapBytes (pProg->params[j]);
            }
            unsigned char* pNext = (unsigned char*) pProg;
            pNext += sizeof (SFxProgramBase);
            pNext += (sizeof (float) * numParams);
            if (pNext > nBank + nBankLen) /* VERY simple fuse                  */
                break;

            pProg = (SFxProgram*) pNext;
            nProg++;
        }
    }

    bool brc = true; /* default to OK                     */
    FILE* fp = NULL;
    try
    {
        fp = fopen (pszFile, "wb"); /* try to open the file              */
        if (! fp)                   /* upon error                        */
            throw (int) 1;          /* return an error                   */
        if (fwrite (nBank, 1, nBankLen, fp) != (size_t) nBankLen)
            throw (int) 1;
    }
    catch (...)
    {
        brc = false;
    }
    if (fp)
        fclose (fp);
    delete[] nBank;

    return brc;
}

/*****************************************************************************/
/* Unload : removes a loaded bank from memory                                */
/*****************************************************************************/

void CFxBank::Unload()
{
    if (bBank)
        delete[] bBank;
    *szFileName = '\0';  /* reset file name                   */
    bBank       = NULL;  /* reset bank pointer                */
    nBankLen    = 0;     /* reset bank length                 */
    bChunk      = false; /* and of course it's no chunk.      */
}

/*****************************************************************************/
/* GetProgram : returns pointer to one of the loaded programs                */
/*****************************************************************************/

SFxProgram* CFxBank::GetProgram (int nProgNum)
{
    if ((! IsLoaded()) || (bChunk)) /* if nothing loaded or chunk file   */
        return NULL;                /* return OUCH                       */

    SFxBank* pBank    = (SFxBank*) bBank; /* position on 1st program           */
    SFxProgram* pProg = pBank->programs;
    int nProgLen      = sizeof (SFxProgramBase) + pProg->numParams * sizeof (float);

    unsigned char* pThatProg = ((unsigned char*) pProg) + (nProgNum * nProgLen);
    pProg                    = (SFxProgram*) pThatProg;
    return pProg;
}
