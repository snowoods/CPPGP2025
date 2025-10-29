﻿#include "ZD3D11.h"
#include "ZVertexBuffer.h"

namespace Bind
{
    ZVertexBuffer::ZVertexBuffer(ZGraphics& gfx, const Dvtx::VertexBuffer& vbuf)
        :
        stride((UINT)vbuf.GetLayout().Size())
    {
        INFOMAN(gfx);

        D3D11_BUFFER_DESC bd = {};
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.CPUAccessFlags = 0u;
        bd.MiscFlags = 0u;
        bd.ByteWidth = UINT(vbuf.SizeBytes());
        bd.StructureByteStride = stride;

        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = vbuf.GetData();

        // 멤버변수 스타일 변천사
        // m_iMember
        // miMember
        // _Member
        // this->m_iMember
        // iMember_
        // Member_
        // member_
        // member
        // _member (현재 리눅스, 신생언어)
        // 
        // GetDevice() 형태로 사용 이유 : ZGraphics private 멤버에 접근 목적 명시
        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));
    }

    void ZVertexBuffer::Bind(ZGraphics& gfx) noexcept
    {
        const UINT offset = 0u;
        GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
    }
}
