#pragma once
#include "Framework/Interface/Render/IRenderDevice.h"

namespace ParaEngine
{
	class RenderDeviceNull : public IRenderDevice
	{

	public:
		virtual uint32_t GetRenderState(const ERenderState& State) override;


		virtual bool SetRenderState(const ERenderState State, const uint32_t Value) override;


		virtual bool SetClipPlane(uint32_t Index, const float* pPlane) override;


		virtual bool ReadPixels(int nLeft, int nTop, int nWidth, int nHeight, void* pDataOut, uint32_t nDataFormat = 0, uint32_t nDataType = 0) override;


		virtual int GetMaxSimultaneousTextures() override;


		virtual bool SetTexture(uint32_t stage, DeviceTexturePtr_type texture) override;


		virtual bool DrawPrimitive(EPrimitiveType PrimitiveType, uint32_t StartVertex, uint32_t PrimitiveCount) override;


		virtual bool DrawIndexedPrimitive(EPrimitiveType Type, int BaseVertexIndex, uint32_t MinIndex, uint32_t NumVertices, uint32_t indexStart, uint32_t PrimitiveCount) override;


		virtual bool DrawPrimitiveUP(EPrimitiveType PrimitiveType, uint32_t PrimitiveCount, const void* pVertexStreamZeroData, uint32_t VertexStreamZeroStride) override;


		virtual bool DrawIndexedPrimitiveUP(EPrimitiveType PrimitiveType, uint32_t MinVertexIndex, uint32_t NumVertices, uint32_t PrimitiveCount, const void * pIndexData, PixelFormat IndexDataFormat, const void* pVertexStreamZeroData, uint32_t VertexStreamZeroStride) override;


		virtual bool SetTransform(ETransformsStateType State, DeviceMatrix_ptr pMatrix) override;


		virtual bool SetFVF(uint32_t FVF) override;


		virtual void SetCursorPosition(int X, int Y, uint32_t Flags) override;


		virtual bool SetSamplerState(uint32_t stage, ESamplerStateType type, uint32_t value) override;


		virtual bool GetSamplerState(uint32_t stage, ESamplerStateType type, uint32_t* value) override;


		virtual bool SetVertexDeclaration(VertexDeclarationPtr pVertexDeclaration) override;


		virtual bool CreateVertexDeclaration(VertexElement* pVertexElements, VertexDeclarationPtr* ppDecl) override;


		virtual bool SetIndices(IndexBufferDevicePtr_type pIndexData) override;


		virtual bool SetStreamSource(uint32_t StreamNumber, VertexBufferDevicePtr_type pStreamData, uint32_t OffsetInBytes, uint32_t Stride) override;


		virtual Rect GetViewport() override;


		virtual bool SetViewport(const Rect& viewport) override;


		virtual bool SetClearColor(const Color4f& color) override;


		virtual bool SetClearDepth(const float depth) override;


		virtual bool SetClearStencil(const int stencil) override;


		virtual bool Clear(bool color, bool depth, float stencil) override;


		virtual bool SetScissorRect(RECT* pRect) override;


		virtual bool GetScissorRect(RECT* pRect) override;


		virtual bool BeginScene() override;


		virtual bool EndScene() override;


		virtual bool Present() override;
	protected:
		Rect m_ViewPort;
	};
}