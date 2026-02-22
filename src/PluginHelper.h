#pragma once

#include <PluginAPI.h>

namespace hdt
{
	constexpr bool operator<(const PluginInterface::Version& lhs, const PluginInterface::Version& rhs)
	{
		bool result = false;
		if (lhs.major < rhs.major) 
		{
			result = true;
		}
		else if (lhs.major == rhs.major) 
		{
			if (lhs.minor < rhs.minor) 
			{
				result = true;
			}
			else if (lhs.minor == rhs.minor) 
			{
				result = lhs.patch < rhs.patch;
			}
		}

		return result;
	}

	constexpr bool operator>(const PluginInterface::Version& lhs, const PluginInterface::Version& rhs)
	{
		return rhs < lhs;
	}

	constexpr bool operator>=(const PluginInterface::Version& lhs, const PluginInterface::Version& rhs)
	{
		return !(lhs < rhs);
	}

	constexpr bool operator<=(const PluginInterface::Version& lhs, const PluginInterface::Version& rhs)
	{
		return !(rhs < lhs);
	}
}