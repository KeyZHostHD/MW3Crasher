// ==================== MW3Crasher ========================
// Project: https://github.com/diamante0018/MW3Crasher
// Author: Diavolo (https://github.com/diamante0018)
// License: GNU GPL v3.0
// ========================================================

#include "std_include.hpp"
#include "dvars.hpp"
#include "structs.hpp"

#include "utils/hook.hpp"

namespace patches
{
    void DTLSPatches()
    {
        utils::hook::set<uint8_t>(0x06EA960, 0x55);
        utils::hook::set<uint8_t>(0x06EA961, 0x8B);
        utils::hook::set<uint8_t>(0x06EA962, 0xEC);
        utils::hook::set<uint8_t>(0x06EA963, 0x81);
        utils::hook::set<uint8_t>(0x06EA964, 0xEC);
    }

    void dvarPatches()
    {
        dvars::override::Dvar_RegisterBool("jump_slowdownEnable", false, game::DVAR_FLAG_SAVED);
        dvars::override::Dvar_RegisterBool("fx_draw", false, game::DVAR_FLAG_SAVED);
        dvars::override::Dvar_RegisterBool("cg_drawCrosshair", false, game::DVAR_FLAG_SAVED);
        dvars::override::Dvar_RegisterBool("cg_blood", false, game::DVAR_FLAG_SAVED);
        dvars::override::Dvar_RegisterBool("cg_drawDamageFlash", false, game::DVAR_FLAG_SAVED);

        dvars::override::Dvar_RegisterInt("compassPortableRadarSweepTime", 1000, 1, 10000, game::DVAR_FLAG_SAVED);
        dvars::override::Dvar_RegisterInt("com_maxfps", 125, 30, 1000, game::DVAR_FLAG_SAVED);
        dvars::override::Dvar_RegisterInt("cg_hudDamageIconTime", 1000, 0, 10000, game::DVAR_FLAG_SAVED);
        dvars::override::Dvar_RegisterInt("cg_chatTime", 5000, 0, 10000, game::DVAR_FLAG_SAVED);
        dvars::override::Dvar_RegisterInt("cg_chatHeight", 1, 0, 10, game::DVAR_FLAG_SAVED);

        dvars::override::Dvar_RegisterFloat("cg_fov", 110, 90, 120, game::DVAR_FLAG_SAVED);
        dvars::override::Dvar_RegisterFloat("cg_fovScale", 1.0f, 0.2f, 2.0f, game::DVAR_FLAG_SAVED);
        dvars::override::Dvar_RegisterFloat("compassRadarPingFadeTime", 7, 0, 10, game::DVAR_FLAG_SAVED);
        dvars::override::Dvar_RegisterFloat("compassRadarLineThickness", 0.2f, 0, 1, game::DVAR_FLAG_SAVED);
        dvars::override::Dvar_RegisterFloat("cg_hudGrenadeIconMaxRangeFrag", 750, 0, 10000, game::DVAR_FLAG_SAVED);

        dvars::override::Dvar_RegisterVector2("cg_hudChatPosition", 5, 175, 0, 640, game::DVAR_FLAG_SAVED);
    }
}