#include "loadbody.hpp"

#include "esmreader.hpp"
#include "esmwriter.hpp"
#include "defs.hpp"
#include "util.hpp"

namespace ESM
{
    unsigned int BodyPart::sRecordId = REC_BODY;

    BodyPart::BodyPart()
        : mIsDeleted(false)
    {}

    void BodyPart::load(ESMReader &esm)
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
                case ESM::FourCC<'M','O','D','L'>::value:
                    mModel = esm.getHString();
                    break;
                case ESM::FourCC<'F','N','A','M'>::value:
                    mRace = esm.getHString();
                    break;
                case ESM::FourCC<'B','Y','D','T'>::value:
                    esm.getHT(mData, 4);
                    hasData = true;
                    break;
                default:
                    esm.fail("Unknown subrecord");
            }
        }

        if (!hasData)
            esm.fail("Missing BYDT subrecord");
    }

    void BodyPart::save(ESMWriter &esm) const
    {
        esm.writeHNCString("NAME", mId);
        if (mIsDeleted)
        {
            writeDeleSubRecord(esm);
            return;
        }

        esm.writeHNCString("MODL", mModel);
        esm.writeHNOCString("FNAM", mRace);
        esm.writeHNT("BYDT", mData, 4);
    }

    void BodyPart::blank()
    {
        mData.mPart = 0;
        mData.mVampire = 0;
        mData.mFlags = 0;
        mData.mType = 0;

        mModel.clear();
        mRace.clear();

        mIsDeleted = false;
    }
}
