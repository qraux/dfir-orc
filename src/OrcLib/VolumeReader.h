//
// SPDX-License-Identifier: LGPL-2.1-or-later
//
// Copyright © 2011-2019 ANSSI. All Rights Reserved.
//
// Author(s): Jean Gautier (ANSSI)
//
#pragma once

#include "OrcLib.h"

#include "DiskExtent.h"
#include "FSVBR.h"

#pragma managed(push, off)

#ifndef MAX_PATH_LEN
constexpr auto MAX_PATH_LEN = 256;
#endif

namespace Orc {

class LogFileWriter;

class ORCLIB_API VolumeReader
{
public:
    using CDiskExtentVector = std::vector<CDiskExtent>;

public:
    VolumeReader(logger pLog, const WCHAR* szLocation);

    logger GetLogger() { return _L_; }
    virtual const WCHAR* ShortVolumeName() PURE;

    const WCHAR* GetLocation() const { return m_szLocation; }
    FSVBR::FSType GetFSType() const { return m_fsType; }

    ULONGLONG VolumeSerialNumber() const { return m_llVolumeSerialNumber; }
    DWORD MaxComponentLength() const { return m_dwMaxComponentLength > 0 ? m_dwMaxComponentLength : 255; }

    virtual HRESULT LoadDiskProperties() PURE;
    virtual HANDLE GetDevice() PURE;

    bool IsReady() { return m_bReadyForEnumeration; }

    virtual ULONG GetBytesPerFRS() const { return m_BytesPerFRS; };
    virtual ULONG GetBytesPerCluster() const { return m_BytesPerCluster; }
    virtual ULONG GetBytesPerSector() const { return m_BytesPerSector; }

    virtual HRESULT Seek(ULONGLONG offset) PURE;
    virtual HRESULT Read(ULONGLONG offset, CBinaryBuffer& data, ULONGLONG ullBytesToRead, ULONGLONG& ullBytesRead) = 0;
    virtual HRESULT Read(CBinaryBuffer& data, ULONGLONG ullBytesToRead, ULONGLONG& ullBytesRead) = 0;

    const CBinaryBuffer& GetBootSector() const { return m_BoostSector; }

    virtual std::shared_ptr<VolumeReader> ReOpen(DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwFlags) PURE;

    virtual ~VolumeReader() {}

protected:
    logger _L_;
    bool m_bCanReadData = true;
    bool m_bReadyForEnumeration = false;
    ULONG m_BytesPerFRS = 0L;
    ULONG m_BytesPerSector = 0L;
    ULONG m_BytesPerCluster = 0L;
    ULONG m_LocalPositionOffset = 0L;
    LONGLONG m_NumberOfSectors = 0LL;
    ULONGLONG m_llVolumeSerialNumber = 0LL;
    DWORD m_dwMaxComponentLength = 0L;
    FSVBR::FSType m_fsType = FSVBR::FSType::UNKNOWN;

    // Represents the "Volume" being read.
    // this could be either a mounted volume, a dd image, an offline MFT, a snapshot, etc...
    WCHAR m_szLocation[MAX_PATH];

    HRESULT ParseBootSector(const CBinaryBuffer& buffer);
    CBinaryBuffer m_BoostSector;

    virtual std::shared_ptr<VolumeReader> DuplicateReader() PURE;
};

}  // namespace Orc

#pragma managed(pop)
