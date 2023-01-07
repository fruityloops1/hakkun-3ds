#include "hk/system/Init.h"
#include "hk/debug/Log.h"
#include "hk/system/PatchHeader.h"
#include <nn/crt0.h>

extern "C" void hkMain();

namespace hk::sys {

void __attribute__((noinline)) InitializePost()
{
    hk::dbg::Log("hk::sys::Initialize");
    hkMain();
}

// using function pointers so Initialize function can be moved after linking

void (*const volatile nninitRegionPtr)() = &::nninitRegion;
void (*const volatile InitializePostPtr)() = &InitializePost;

void __attribute__((section(".hk.init"))) Initialize()
{
    auto GetCurrentProcessHandle = []() {
        u32 currentPid = 0;
        GetProcessId(&currentPid, 0xffff8001);

        nn::Handle handle;
        nn::Result result = OpenProcess(&handle, currentPid);

        return result ? nn::Handle() : handle;
    };

    u32 address = reinterpret_cast<u32>(&sPatchHeader); // sPatchHeader is always at the start
    u32 neededMemory = (sPatchHeader.codeSize + 0xFFF) & ~0xFFF;

    nn::Result res = ControlProcessMemory(GetCurrentProcessHandle(), address, address, neededMemory, nn::os::MemoryOperation_PROTECT, nn::os::MemoryPermission_RWX);

    InitializePostPtr();
    nninitRegionPtr(); // continue __ctr_start
}
const volatile u32 __attribute__((section(".hk.initend"))) endId = -1;

} // namespace hk::sys
