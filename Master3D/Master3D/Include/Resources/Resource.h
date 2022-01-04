#pragma once
namespace Resources {
	class Resource
	{
	public:
		virtual void Release() = 0;

		virtual std::string GetFileName() const = 0;
	};
}