#pragma once
class ResourceBase
	{
	public:
		ResourceBase(void);
		virtual ~ResourceBase(void);
		virtual void Load() = 0;
		virtual void Destroy() = 0;
	};

