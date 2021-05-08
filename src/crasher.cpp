// ==================== MW3Crasher ========================
// Project: https://github.com/diamante0018/MW3Crasher
// Author: Diavolo (https://github.com/diamante0018)
// License: GNU GPL v3.0
// ========================================================

#include "crasher.hpp"

#include "utils/hook.hpp"

namespace crasher
{
	utils::hook::detour net_chan_hook;

	int netChanCrash(int a1, unsigned char* data, int size)
	{
		data[1] = 0xAA;
		data[8] = 0x80;

		
		return net_chan_hook.invoke<int>(a1, data, size);
	}

	void doCrash()
	{
		client::CL_printf("F6: Hooking freezer function\n");
		net_chan_hook.create(0x004400E0, &crasher::netChanCrash);
	}
}