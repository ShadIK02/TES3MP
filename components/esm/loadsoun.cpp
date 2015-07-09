#include "loadsoun.hpp"

#include "esmreader.hpp"
#include "esmwriter.hpp"
#include "defs.hpp"
#include "util.hpp"

namespace ESM
{
    unsigned int Sound::sRecordId = REC_SOUN;

    Sound::Sound()
        : mIsDeleted(false)
    {}

    void Sound::load(ESMReader &esm)
    {
        mId = esm.getHNString("NAME");
        if (mIsDeleted = readDeleSubRecord(esm))
        {
            return;
        }

        bool hasData = false;
        while (esm.hasMoreSubs())
        {
            esm.getSubName();
            uint32_t name = esm.retSubName().val;
            switch (name)
            {
                case ESM::FourCC<'F','N','A','M'>::value:
                    mSound = esm.getHString();
                    break;
                case ESM::FourCC<'D','A','T','A'>::value:
                    esm.getHT(mData, 3);
                    hasData = true;
                    break;
                default:
                    esm.fail("Unknown subrecord");
            }
        }
        if (!hasData)
            esm.fail("Missing DATA");
    }

    void Sound::save(ESMWriter &esm) const
    {
        esm.writeHNCString("NAME", mId);
        if (mIsDeleted)
        {
            writeDeleSubRecord(esm);
            return;
        }

        esm.writeHNOCString("FNAM", mSound);
        esm.writeHNT("DATA", mData, 3);
    }

    void Sound::blank()
    {
        mSound.clear();

        mData.mVolume = 128;
        mData.mMinRange = 0;
        mData.mMaxRange = 255;
        
        mIsDeleted = false;
    }
}
