#include "hk/Types.h"
#include "hk/debug/Break.h"
#include "hk/debug/Log.h"
#include "hk/hook/AsmPatch.h"
#include "hk/system/PatchHeader.h"
#include "nn/os/os_Types.h"

static nn::Handle GetCurrentProcessHandle()
{
    nn::Handle handle = 0;
    u32 currentPid = 0;

    nn::Result res1 = nn::svc::GetProcessId(&currentPid, 0xffff8001);
    nn::Result res2 = nn::svc::OpenProcess(&handle, currentPid);

    if (res1.Failed() || res2.Failed())
        hk::dbg::Break(res1, res2);

    return handle;
}

static bool setCodeRegionPerm(nn::os::MemoryPermission perm)
{
    nn::Result result = nn::svc::ControlProcessMemory(GetCurrentProcessHandle(), hk::sys::sPatchHeader.textOffset, hk::sys::sPatchHeader.textOffset, (hk::sys::sPatchHeader.textSize + 0xFFF) & ~0xFFF, nn::os::MemoryOperation::MemoryOperation_PROTECT, perm);

    HK_CHECK_RESULT(result);
    return true;
}

extern "C" void hkMain()
{
    // u32* place = reinterpret_cast<u32*>(0x0027d248);
    // setCodeRegionPerm(nn::os::MemoryPermission_RWX);
    // place[0] = 0x1E;
    // place[1] = 0xFF;
    // place[2] = 0x2F;
    // place[3] = 0xE1;
    // setCodeRegionPerm(nn::os::MemoryPermission_RW);

    hk::dbg::Log("hkMain called %d", 4);
}
