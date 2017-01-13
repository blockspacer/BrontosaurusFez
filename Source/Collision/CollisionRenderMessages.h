#pragma once

namespace Intersection
{
	struct SCircle;
	struct STriangle;
}

namespace Collision
{
	class IRenderCommand
	{
	public:
		IRenderCommand();
		virtual ~IRenderCommand();
		virtual void DoRenderCall(CU::GrowingArray<char, unsigned short, false>& aVertexBuffer) = 0;
	};

	class CRenderCircle : public IRenderCommand
	{
	public:
		CRenderCircle(const Intersection::SCircle* aCircle);
		void DoRenderCall(CU::GrowingArray<char, unsigned short, false>& aVertexBuffer) override;

	private:
		const Intersection::SCircle* myCircle;
	};

	class CRenderTriangle : public IRenderCommand
	{
	public:
		CRenderTriangle(const Intersection::STriangle* aCircle);
		void DoRenderCall(CU::GrowingArray<char, unsigned short, false>& aVertexBuffer) override;

	private:
		const Intersection::STriangle* myTriangle;
	};
}