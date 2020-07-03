/*
 * Copyright (c) 2019, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.

 *
 */

//!
//! \file     RenderTarget.h
//! \brief    another implementation of RenderTarget to support multi render sources.
//!
#ifndef _RENDERTARGET_H_
#define _RENDERTARGET_H_

#include "../Common.h"
#include "../RegionData.h"
#include "RenderBackend.h"
#include "../MediaSource/RenderSourceFactory.h"
#include <map>

VCD_NS_BEGIN

typedef struct TileInformation{
    uint32_t video_id;
    uint32_t tile_id;
    uint32_t projRegLeft;
    uint32_t projRegTop;
    uint32_t projRegWidth;
    uint32_t projRegHeight;
    uint16_t packedRegLeft;
    uint16_t packedRegTop;
    uint16_t packedRegWidth;
    uint16_t packedRegHeight;
    uint16_t packedPicWidth;
    uint16_t packedPicHeight;
    uint8_t  transformType;
    uint32_t face_id;
}TileInformation;

typedef struct QualityRankingInfo{
    int32_t mainQualityRanking;
    int32_t numQuality;
    std::map<int32_t, std::vector<TileInformation>> mapQualitySelection;
}QualityRankingInfo;

class RenderTarget
{
public:
    RenderTarget()
    {
        m_rsFactory = NULL;
        m_transformType.clear();
        m_fboOnScreenHandle = 0;
        m_textureOfR2S = 0;
        m_avgChangedTime = 0;
        m_isAllHighQualityInView = true;
        mQualityRankingInfo.mainQualityRanking = 0;
        mQualityRankingInfo.numQuality = 0;
        mQualityRankingInfo.mapQualitySelection.clear();
    }
    virtual ~RenderTarget() = default;

    //! \brief Initialize RenderTarget data according to mediaSource Info
    //!
    //! \param  [in] struct MediaSourceInfo *
    //!         Media Source Info
    //!         [in] uint32_t
    //!         render to texture fbo handle.
    //! \return RenderStatus
    //!         RENDER_STATUS_OK if success, else fail reason
    //!
    virtual RenderStatus Initialize(RenderSourceFactory* rsFactory) = 0;

    //! \brief Create a render target
    //!
    //! \param  [in] RenderBackend*
    //!         RenderBackend interface
    //!         [in] struct SourceWH*
    //!         target width and height
    //! \return RenderStatus
    //!         RENDER_STATUS_OK if success, else fail reason
    //!
    virtual RenderStatus CreateRenderTarget() = 0;

    //! \brief Update the render target
    //!
    //! \param  [in] RenderBackend*
    //!         RenderBackend interface
    //!         [in] struct RegionInfo*
    //!         region information including source width and height and rwpk.
    //! \return RenderStatus
    //!         RENDER_STATUS_OK if success, else fail reason
    //!
    virtual RenderStatus Update( float yaw, float pitch, float hFOV, float vFOV ) = 0;
    //! \brief Get the texture Of R2S
    //!
    //! \return uint32_t
    //!         return texture Of R2S
    //!
    uint32_t GetTextureOfR2S(){ return m_textureOfR2S; };

    std::map<uint32_t, uint8_t> GetTransformType() { return m_transformType; };

protected:
    RenderSourceFactory*   m_rsFactory;                 //!RenderSource Factory;
    std::map<uint32_t, uint8_t> m_transformType;       //!transformtype
    uint32_t               m_fboOnScreenHandle;         //!output
    uint32_t               m_textureOfR2S;              //!render to screen
    float                  m_avgChangedTime;            //!average time to change from blur to clear
    bool                   m_isAllHighQualityInView;    //!isAllHighResoInView
    QualityRankingInfo     mQualityRankingInfo;
};

VCD_NS_END
#endif /* _RENDERTARGET_H_ */